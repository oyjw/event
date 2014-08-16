#include <sys/socket.h>
#include <sys/types.h>
#include "IOLoop.h"
#include "Log.h"
#include "Stream.h"
void IOLoop::run(){
	while(1){
		int ret=epoll_wait(epollfd,fdevents,MAX_EVENTS,-1);
		char buf[100];
		log("epoll_wait",ret);
		if(ret==-1){
			if(errno==EINTR)
				continue;
			else {
				return;
			}
		}
		else{
			snprintf(buf,100,"%d events",ret);
			log(buf);
		}
		for(int i=0;i<ret;++i){
			Stream* stream=(Stream*)fdevents[i].data.ptr;
			if(fdevents[i].events & EPOLLERR || fdevents[i].events & EPOLLHUP || stream->isClosing()){
				streams.erase(stream->iter);
				delete stream;
				continue;
			}
			if(fdevents[i].events & EPOLLIN){
				if(stream->isAcceptable()){
					int listenfd=stream->getFd();
					int sockfd=accept(listenfd,NULL,NULL);
					log("accept socket",sockfd);
					if(sockfd==-1){
						continue;
					}
					setNonblock(sockfd);
					Stream* newstream=new Stream(sockfd,0,stream->getProtocol(),this);
					struct epoll_event ev;
					ev.data.ptr=newstream;
					ev.events=EPOLLIN ;
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
					stream->readSock();
				}
				if(stream->getWriteBuffer()->readableLen()>(size_t)0){
					struct epoll_event ev;
					ev.data.ptr=stream;
					ev.events=EPOLLIN |EPOLLOUT;
					int ret=epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&ev);
					log("epoll_ctl",ret);
					if(ret==-1){
						delete stream;
						continue;
					}
				}
			}
			if(fdevents[i].events & EPOLLOUT){
				stream->writeSock();
				if(stream->getWriteBuffer()->readableLen()==(size_t)0){
					if(stream->isDisconnecting()){
						shutdown(stream->getFd(),SHUT_WR);
					}
					struct epoll_event ev;
					ev.data.ptr=stream;
					ev.events=EPOLLIN;
					int ret=epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&ev);
					log("epoll_ctl",ret);
					if(ret==-1){
						delete stream;
					}
				}
			}
		}
	}
		
}
