#include "Log.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(){
	struct sockaddr_in address;
	struct in_addr addr;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("192.168.0.1");
	address.sin_port=htons(8088);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	log("creating socket",fd);
	if(fd==-1)
		return NULL;
	int ret=bind(fd,(sockaddr*)&address,sizeof(address));
	log("binding socket",ret);
	if(ret==-1)
		return NULL;
	ret=listen(fd,128);
	log("make socket listening",ret);
	if(ret==-1)
		return NULL;
	Stream* stream=new Stream(fd,ACCEPTABLE,proto,loop);
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

}