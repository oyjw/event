#ifndef _PRINT_PROTOCOL_H_
#define _PRINT_PROTOCOL_H_
#include "Protocol.h"
#include "Stream.h"
#include "Buffer.h"
#include <string>
#include <iostream>
class EchoProtocol:public Protocol{
public:
	void onMsgReceived(Stream* stream){
		Buffer *readBuffer=stream->getReadBuffer();
		Buffer *writeBuffer=stream->getWriteBuffer();
		size_t len=writeBuffer.readableLen();
		if(len==0)
			return;
		const char* buf=writeBuffer.readableData();
		std::string str(buf,len);
		std::cout<<str;
	}

};

#endif
