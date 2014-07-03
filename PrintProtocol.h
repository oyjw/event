#ifndef _PRINT_PROTOCOL_H_
#define _PRINT_PROTOCOL_H_
#include "Protocol.h"
#include "Stream.h"
#include "Buffer.h"
#include <string>
#include <iostream>
class PrintProtocol:public Protocol{
public:
	void onMsgReceived(Stream* stream){
		Buffer *readBuffer=stream->getReadBuffer();
		size_t len=readBuffer->readableLen();
		if(len==0)
			return;
		const char* buf=readBuffer->readableData();
		std::string str(buf,len);
		std::cerr<<str;
		readBuffer->advance(len);
	}

};

#endif
