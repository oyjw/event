#ifndef _Ev_EPOLL_H_
#define _Ev_EPOLL_H_

#define WRITABLE 1<<0
#define READABLE 1<<1
#define ERROR 1<<2

struct Event{
	uint32_t flag;
	Stream* stream;
};

#endif