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

#define ACCEPTABLE     1
#define WRITABLE       4
#define CLOSINGWRITE   1<<4
#define WRITECLOSED    1<<5
#define READCLOSED     1<<6
#define DISCONNECTING  1<<7

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
	}
	void setWritable(){
		flag=flag|WRITABLE;
	}
	bool isWritable(){
		return flag&WRITABLE;
	}
	void setDisconnecting(){
		flag=flag|DISCONNECTING;
	}
	bool isDisconnecting(){
		return flag&DISCONNECTING;
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
