#include <sys/socket.h>
#include <sys/types.h>
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
					log("accept socket",sockfd);
					if(sockfd==-1){
						continue;
					}
					setNonblock(sockfd);
					Stream* newstream=new Stream(sockfd,0,stream->getProtocol());
					struct epoll_event ev;
					ev.data.ptr=newstream;
					ev.events=EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
					int ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
					log("epoll_ctl",ret);
					if(ret==-1){
						delete newstream;
						continue;
					}
					addStream(newstream);
					newstream->iter=--streams.end();
				}
				else{
					if(stream->isClosing()){
						delete stream;
					}
					else{
						stream->readSock();
					}
				}
				if(stream->isWritable()){
					stream->writeSock();
				}
			}
			if(fdevents[i].events && EPOLLOUT){
				stream->setWritable();
				stream->writeSock();
			}
		}
	}
		
}
