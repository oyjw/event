#include "Stream.h"

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
