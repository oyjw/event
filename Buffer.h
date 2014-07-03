#ifndef _BUFFER_H
#define _BUFFER_H
#include <vector>
#include <algorithm>
#include <cassert>

#define RESERVED 8
#define INITIALSIZE 1024
class Buffer{
private:
	std::vector<char> vec;
	size_t readIdx;
	size_t writeIdx;
public:
	Buffer():readIdx(RESERVED),writeIdx(RESERVED) {
		vec.resize(RESERVED+INITIALSIZE);
	}
	void append(const char* buf,size_t size){
		if(size>vec.size()-writeIdx){
			size_t curBufSize=writeIdx-readIdx;
			copy(vec.begin()+readIdx,vec.begin()+writeIdx,vec.begin()+RESERVED);
			readIdx=RESERVED;
			writeIdx=RESERVED+curBufSize;
			if(size>vec.size()-RESERVED-curBufSize){
				copy(buf,buf+vec.size()-writeIdx,vec.begin()+writeIdx);
				copy(buf+vec.size()-writeIdx,buf+size,std::back_inserter(vec));
				writeIdx+=size;
				return;
			}
		}
		copy(buf,buf+size,vec.begin()+writeIdx);
		writeIdx+=size;
	}
	bool retrieve(char* buf,size_t size){
		if(!peek(buf,size)){
			return false;
		}
		readIdx+=size;
		return true;
	}
	bool peek(char* buf,size_t size){
		if(size>writeIdx-readIdx){
			return false;
		}
		copy(vec.begin()+readIdx,vec.begin()+readIdx+size,buf);
		return true;
	}
	size_t readableLen(){
		return writeIdx-readIdx;
	}
	void advance(size_t step){
		assert(readIdx+step<=writeIdx);
		readIdx+=step;
	}
	const char* readableData(){
		return vec.data()+readIdx;
	}
};

#endif
