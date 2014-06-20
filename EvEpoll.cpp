#include "EvEpoll.h"




int EvWait(int epollfd,Event* events,int maxevent){
	struct epoll_event fdevents[maxevent];
	while(1){
		int ret=epoll_wait(epollfd,fdevents,maxevent,-1);
		if(ret==-1){
			if(errno==EINTR)
				continue;
			else {
				perror("epoll_wait error");
				return -1;
			}
		}
		for(int i=0;i<ret;++i){
			if(fdevents[i] & EPOLLIN)
				events[i].flag=events[i].flag & READABLE;
			if(fdevents[i] & EPOLLOUT)
				events[i].flag=events[i].flag & WRITABLE;
			if(fdevents[i] & EPOLLERR || fdevents[i] & EPOLLHUP)
				events[i].flag=events[i].flag & ERROR;	
			events[i].stream=(Stream*)fdevents[i].data.ptr;
		}
		return 0;
	}
}









