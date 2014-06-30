#include "IOLoop.h"
#include "Log.h"
#include "Stream.h"
void IOLoop::run(){
	while(1){
		int ret=epoll_wait(epollfd,fdevents,MAX_EVENTS,-1);
		log("epoll_wait",ret);
		if(ret==-1){
			if(errno==EINTR)
				continue;
			else {
				return;
			}
		}
		for(int i=0;i<ret;++i){
			Stream* stream=(Stream*)fdevents[i].data.ptr;
			if(fdevents[i].events & EPOLLERR || fdevents[i].events & EPOLLHUP || stream->isClosing()){
				streams.erase(stream->iter);
				delete stream;
				continue;
			}
			if(fdevents[i].events && EPOLLIN){
				if(stream->isAcceptable()){
					int listenfd=stream->getFd();
					int sockfd=accept(listenfd,NULL,NULL);
					setNonblock(sockfd);
					Stream* newstream=new Stream(sockfd,0,stream->getProtocol());
					struct epoll_event ev;
					ev.data.ptr=newstream;
					ev.events=EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
					int ret=epoll_ctl(epollfd,CTL_ADD,sockfd,ev);
					log("epoll_ctl",ret);
					if(ret==-1){
						delete newstream;
						continue;
					}
					loop->addStream(newstream);
					newstream->iter=loop->streams.end()-1;
				}
				else{
					if(stream->isClosing()){
						delete stream;
					}
					else{
						stream->read();
					}
				}
				if(stream->isWritable()){
					stream->write();
				}
			}
			if(fdevents[i].events && EPOLLOUT){
				stream->setWritable();
				stream->write();
			}
		}
		
}
