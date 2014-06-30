#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

class Stream;

class Protocol
{
public:
	Protocol() {}
	virtual ~Protocol() {}
	void onMsgReceived(Stream* stream) {}
	void onConnectionClosed() {}
};

#endif