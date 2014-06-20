#ifndef _IOLOOP_H_
#define _IOLOOP_H_
#include <vector>
class Protocol;

class IOLoop
{
public:
	IOLoop(Protocol* p){
		epollfd=epoll_create(1024);
		log("creating epollfd",epollfd);
	}
	void run();
	void addStream(Stream* stream)[
		streams.push_back(stream);
	}
	~IOLoop(){
		int ret = close(epollfd);
		log("closing epollfd",ret);
		delete protocol;
	}
	bool write(Stream* stream,char* buf,int size);
	void readStart(Stream* stream,char* buf,int size);
private:
	list<Stream*> streams;
	int nClosedStreams;
	Protocol *protocol;
	int epollfd;
	struct epoll_event fdevents[MAX_EVENTS];
};

#endif