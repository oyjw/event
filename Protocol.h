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
	void onMsgReceived(Stream* stream){
		Buffer *readBuffer=stream->readBuffer();
		Buffer *writeBuffer=stream->writeBuffer();
		if(readBuffer->len()>4){
			const char* buf=readBuffer->buf();
			int size=0;
			memcpy(size,buf,4);
			if(readBuffer->len()<4+size)
				return;
			vector<char> tmp;
			copy(std::back_inserter(tmp),buf+4,buf+4+size);
			writeBuffer->append(tmp.data(),size);
		}
	}
	void onConnectionClosed(){
	}
};

#endif