#ifndef _IOLOOP_H_
#define _IOLOOP_H_
#include <list>
#include <sys/epoll.h>
#include <unistd.h>
#include "Log.h"
#include "Stream.h"
#define MAX_EVENTS 64
class IOLoop
{
public:
	IOLoop(){
		epollfd=epoll_create(1024);
		log("creating epollfd",epollfd);
	}
	void run();
	void addStream(Stream* stream){	
		streams.push_back(stream);
	}
	~IOLoop(){
		int ret = close(epollfd);
		log("closing epollfd",ret);
		for(auto& stream:streams){
			delete stream;
		}
	}
	std::list<Stream*> streams;
	int getEpollFd(){
		return epollfd;
	}
private:
	int nClosedStreams;
	int epollfd;
	struct epoll_event fdevents[MAX_EVENTS];
};

#endif
