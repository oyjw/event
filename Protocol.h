#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "Stream.h"
#include "Buffer.h"
#include <algorithm>
class Protocol
{
public:
	Protocol();
	virtual ~Protocol() {}
	void onMsgReceived(Stream* stream) {}
	void onConnectionClosed() {}
};

#endif