#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include "Stream.h"
#include "Log.h"
#include "IOLoop.h"
#include "Protocol.h"
bool setNonblock(int fd){
	int opt=fcntl(fd,F_GETFL);
	log("getting file status flag",opt);
	if(opt==-1)
		return false;
	opt=fcntl(fd,F_SETFL,opt|O_NONBLOCK);
	log("setting fd nonblocking",opt);
	if(opt==-1)
		return false;
	return true;
}

std::list<Stream*>* Stream::streams=NULL;
Stream* listenStream(IOLoop* loop,Protocol* proto){
	struct sockaddr_in address;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=htonl(INADDR_ANY);
	address.sin_port=htons(8088);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	log("creating socket",fd);
	if(fd==-1)
		return NULL;
	setNonblock(fd);
	int ret=bind(fd,(sockaddr*)&address,sizeof(address));
	log("binding socket",ret);
	if(ret==-1)
		return NULL;
	ret=listen(fd,128);
	log("make socket listening",ret);
	if(ret==-1)
		return NULL;
	Stream* stream=new Stream(fd,ACCEPTABLE,proto);
	Stream::streams=&loop->streams;
	struct epoll_event ev;
 	ev.data.ptr=stream;
	ev.events=EPOLLIN ;
	ret=epoll_ctl(loop->getEpollFd(),EPOLL_CTL_ADD,stream->getFd(),&ev);
	log("epoll_ctl",ret);
	if(ret==-1){
		delete stream;
		return NULL;
	}
	loop->addStream(stream);
	stream->iter=--loop->streams.end();
	return stream;
}

void Stream::writeSock(){
	assert(flag&WRITABLE);
	size_t len=writeBuffer.readableLen();
	if(len==0)
		return;
	const char* buf=writeBuffer.readableData();
	int nwrite=0;
	do{
		nwrite=write(sockfd,buf,len);
	}while(nwrite==-1 && errno==EINTR);
	char tmp[100];
	snprintf(tmp,100,"writing data to %d",sockfd);
	log(tmp,nwrite);
	if(nwrite==-1 && errno!=EAGAIN && errno!=EWOULDBLOCK){
		setClosing();
	}
	else if((size_t)nwrite!=len){
		writeBuffer.advance(nwrite);
	}
}

void Stream::readSock(){
	char buf[4096];
	ssize_t nread=0;
	while(1){
		nread=read(sockfd,buf,4096);
		if(nread==-1){
			if(errno==EINTR)
				continue;
			else if(errno==EAGAIN || errno==EWOULDBLOCK)
				break;
			else{
				log("reading sockfd",-1);
				break;
			}
		}
		else if(nread==0){
			setClosing();
			break;
		}
		readBuffer.append(buf,nread);
		size_t toread=nread;
		assert(readBuffer.readableLen()==toread);
		protocol->onMsgReceived(this);
		while(readBuffer.readableLen()<toread){
			toread=readBuffer.readableLen();
			protocol->onMsgReceived(this);
		}
	}
	if(nread==-1 && errno!=EAGAIN && errno!=EWOULDBLOCK){
		setClosing();
	}
}



