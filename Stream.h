#ifndef _STREAM_H_
#define _STREAM_H_

#include "Log.h"
Stream* listenFd();

#define ACCEPTABLE

class Stream
{
public:
	Stream(int fd):sockfd(fd),flag(0),readBuffer(NULL),writeBuffer(NULL) {
	}
	~Stream(){
		int ret=close(sockfd);
		Log("closing sockfd",ret);
	}
	bool write(Stream* stream,char* buf,int size);
	void read(){
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
	int getfd() {return sockfd; }
	bool isAcceptable() {
		return flag&ACCEPTABLE;
	}
	void close(){
	}
	Buffer* readBuffer(){
		return &readBuffer;
	}
	Buffer* writeBuffer(){
		return &writeBuffer;
	}
private:
	int flag;
	int sockfd;
	Protocol* protocol;
	Buffer readBuffer;
	Buffer writeBuffer;
};

#endif