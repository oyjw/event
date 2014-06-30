#ifndef _STREAM_H_
#define _STREAM_H_
#include <list>
#include "Log.h"
Stream* listenStream();
bool setNonblock(int fd);

#define ACCEPTABLE 1
#define WRITABLE 2
#define CLOSING  4
class Stream
{
public:
	static std::list<Stream*>* streams;
	std::list<Stream*>::iterator iter;
	static std::list<Stream*>& getAllStreams(){
		return *streams;
	}
	Stream(int fd,Protocol* pro):sockfd(fd),flag(0),protocol(pro) {}
	Stream(int fd,int f,Protocol* pro):sockfd(fd),flag(f),protocol(pro) {}
	~Stream(){
		int ret=close(sockfd);
		Log("closing sockfd",ret);
		if(isAcceptable()){
			delete protocol;
		}
	}
	void setWritable(){
		flag=flag|WRITABLE;
	}
	bool isWritable(){
		return flag&WRITABLE;
	}
	void setClosing(){
		flag=flag|CLOSING;
	}
	bool isClosing(){
		return flag&CLOSING;
	}
	void write();
	void read();
	int getFd() {return sockfd; }
	Protocol* getProtocol() {return protocol; }
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
	void setProtocol(Protocol* proto){
		protocol=proto;
	}
private:
	int flag;
	int sockfd;
	Protocol* protocol;
	Buffer readBuffer;
	Buffer writeBuffer;
};

#endif