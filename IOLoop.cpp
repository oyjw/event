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
		std::vector<Stream*> closingStreams;
		for(int i=0;i<ret;++i){
			Stream* stream=(Stream*)fdevents[i].data.ptr;
			if(fdevents[i].events & EPOLLERR || fdevents[i].events & EPOLLHUP){
				closeStream(stream);
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
					assert(!(stream->isDisconnecting()));
					if(!receive(stream)){
						continue;
					}
				}
				if(stream->isWritable()){
					if(!send(stream)){
						continue;
					}
				}
			}
			if(fdevents[i].events & EPOLLOUT ){
				assert(!stream->isDisconnected());
				stream->setWritable();
				stream->writeSock();
			}
		}
        for(auto& stream:closingStreams){
            streams.erase(stream->iter);
            delete stream;
        }
	}
}

bool IOLoop::receive(Stream* stream){
	stream->readSock();
	if(stream->isDisconnecting()){
		closeStream(stream);
		return false;
	}
	if(stream->readClosed()){
		return true;
	}
}

bool IOLoop::send(Stream* stream){
	if(!stream->isWritable() || stream->writeClosed())
		return true;
	stream->writeSock();
	if(stream->isDisconnecting()){
		closeStream(stream);
		return false;
	}
	Buffer* writeBuffer=stream->getWriteBuffer();
	size_t len=writeBuffer.readableLen();
	if(len==0 && stream->readClosed()){
		closeStream(stream);
		return false;
	}
	else if(len==0 && stream->closingWrite()){
		stream->setWriteClosed();
	}
	return true;
}

void IOLoop::closeStream(Stream* stream){
	streams.erase(stream->iter);
    delete stream;
}
