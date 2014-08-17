#ifndef _STREAM_H_
#define _STREAM_H_
#include <list>
#include <unistd.h>
#include "Log.h"
#include "Buffer.h"
#include "Protocol.h"
class Stream;
class IOLoop;
Stream* listenStream(IOLoop* loop,Protocol* proto);
bool setNonblock(int fd);

#define ACCEPTABLE 1
#define ERROR    2
#define WRITABLE 4
#define CLOSING  8
class Stream
{
public:
	static std::list<Stream*>* streams;
	std::list<Stream*>::iterator iter;
	static std::list<Stream*>& getAllStreams(){
		return *streams;
	}
	Stream(int fd,Protocol* pro,IOLoop* l):flag(0),sockfd(fd),protocol(pro),loop(l) {}
	Stream(int fd,int f,Protocol* pro,IOLoop* l):flag(f),sockfd(fd),protocol(pro),loop(l) {}
	~Stream(){
		int ret=close(sockfd);
		log("closing sockfd",ret);
		if(isAcceptable()){
			delete protocol;
		}
	}/
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
	void setError(){
		flag=ERROR;
	}
	bool isError(){
		return flag==ERROR;
	}
	void writeSock();
	void readSock();
	int getFd() {return sockfd; }
	Protocol* getProtocol() {return protocol; }
	bool isAcceptable() {
		return flag&ACCEPTABLE;
	}
	Buffer* getReadBuffer(){
		return &readBuffer;
	}
	Buffer* getWriteBuffer(){
		return &writeBuffer;
	}
private:
	int flag;
	int sockfd;
	IOLoop* loop;
	Protocol* protocol;
	Buffer readBuffer;
	Buffer writeBuffer;
};

#endif
