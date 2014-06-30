#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include "Stream.h"
#include "Log.h"
#include "IOLoop.h"

bool setNonblock(int fd){
	int opt=fcntl(fd,F_GETFL);
	log("getting file status flag",opt);
	if(opt==-1)
		return false;
	opt=fcntl(fd,F_SETFL,opt|O_NONBLOCK);
	log("setting fd nonblocking",opt);
	if(opt==-1)
		return false;
}

Stream* listenStream(IOLoop* loop,Protocol* proto){
	struct sockaddr_in address;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=htons(8099);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	log("creating socket",fd);
	if(fd==-1)
		return NULL;
	setNonblock(fd);
	int ret=bind(fd,&address,sizeof(address));
	log("binding socket",ret);
	if(ret==-1)
		return NULL;
	ret=listen(fd,128);
	log("make socket listening",ret);
	if(ret==-1)
		return NULL;
	Stream* stream=new Stream(fd,ACCEPTABLE);
	Stream::streams=&loop->streams;
	loop->addStream(stream);
	stream->iter=loop->streams.end()-1;
	
	stream->setProtocol(proto);
	return stream;
}

void Stream::write(){
	assert(flag&WRITABLE);
	size_t len=writeBuffer.readableLen();
	if(len==0)
		return;
	const char* buf=writeBuffer.readableData();
	int nwrite=0;
	do{
		nwrite=write(sockfd,buf,len);
	}while(nwrite==-1 && errno==EINTR);
	char buf[100];
	snprintf(buf,100,""writing data to %d",sockfd);
	log(buf,nwrite);
	if(nwrite==-1 && errno!=EAGAIN && errno!=EWOULDBLOCK){
		setClosing();
	}
	else if(nwrite!=len){
		writeBuffer.advance(nwrite);
	}
}

void Stream::read(){
	char buf[4096];
	while(1){
		int nread=read(sockfd,buf,4096);
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
		readBuffer.append(buf,nread);
		assert(readBuffer.readable()==nread);
		protocol->onMsgReceived(this);
		while(readBuffer.readable()<nread){
			nread=readBuffer.readable();
			protocol->onMsgReceived(this);
		}
	}
	if(nwrite==-1 && errno!=EAGAIN && errno!=EWOULDBLOCK){
		setClosing();
	}
}
	
	
	