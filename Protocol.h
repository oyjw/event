#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

class Stream;

class Protocol
{
public:
	Protocol() {}
	virtual ~Protocol() {}
 	virtual	void onMsgReceived(Stream* stream) {}
	virtual void onConnectionClosed() {}
};

#endif
