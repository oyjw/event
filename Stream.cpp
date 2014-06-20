#include "Stream.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
bool setNonblock(int fd){
	int flag=0;
	int ret=fcntl(fd,F_GETFL,&flag);
	log("getting file flag",ret);
	if(ret==-1)
		return false;
	ret=fcntl(fd,F_SETFL,flag|O_NONBLOCK);
	log("setting fd non-blocking",ret);
	if(ret==-1)
		return false;
}
Stream* listenStream(){
	struct sockaddr_in address;
	address.s_addr=inet_addr("127.0.0.1");
	address. =htons(8099);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	Log("creating socket",fd);
	int ret=bind(fd,&address,sizeof(address));
	Log("binding socket",ret);
	Stream* stream=new Stream(fd,ACCEPTABLE);
	return stream;
}


Stream::Stream()
{
}


Stream::~Stream()
{
}

void Stream::write(Stream* stream,char* buf,int size){
	if(writeBuffer.len()==0)
		return;

}

void Stream::read(){
	char buf[4096];
	while(1){
		int nread=read(sockfd,buf,4096);
		if(nread==-1){
			if(errno==EINTR)
				continue;
			else if(errno==EAGAIN)
				break;
		}
		readBuffer.append(buf,nread);
		protocol->onMsgReceived(this);
	}
}
	
	
	