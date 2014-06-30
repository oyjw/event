#ifndef _PRINT_PROTOCOL_H_
#define _PRINT_PROTOCOL_H_
#include "Protocol.h"
#include "Stream.h"
#include "Buffer.h"
class EchoProtocol:public Protocol{
public:
	void onMsgReceived(Stream* stream){
		Buffer *readBuffer=stream->readBuffer();
		Buffer *writeBuffer=stream->writeBuffer();
		int size=0;
		if(!readBuffer->peek(&size,4)){
			return;
		}
		if(readBuffer->readable()<4+size){
			return;
		}
		readBuffer->advance(4);
		const char* data=readBuffer.readableData();
		writeBuffer->append(data,size);
		readBuffer->advance(size);
	}

};

#endif