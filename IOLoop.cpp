#include "IOLoop.h"
#include "Log.h"

void IOLoop::run(){
	while(1){
		int ret=epoll_wait(epollfd,fdevents,MAX_EVENTS,-1);
		if(ret==-1){
			if(errno==EINTR)
				continue;
			else {
				perror("epoll_wait");
				return;
			}
		}
		for(int i=0;i<ret;++i){
			Stream* stream=(Stream*)fdevents[i].data.ptr;
			if(fdevents[i].events & EPOLLERR || fdevents[i].events & EPOLLHUP){
				delete stream;
			}
			else if(fdevents[i].events && EPOLLIN){
				if(stream->isAcceptable()){
					int listenfd=stream->getfd();
					int sockfd=accept(listenfd,NULL,NULL);
					Stream* newstream=new Stream(sockfd);
					struct epoll_event ev;
					ev.data.ptr=newstream;
					ev.events=EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
					if(epoll_ctl(epollfd,CTL_ADD,sockfd,ev)==-1){
						delete newstream;
						perror("epoll_ctl");
						continue;
					}
				}
				else{
					stream->read();
				}
			}else if(fdevents[i].events && EPOLLOUT){
				stream->write();
			}
		}
		
}