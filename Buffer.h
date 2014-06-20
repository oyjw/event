#ifndef _BUFFER_H
#define _BUFFER_H
#include <vector>
#include <algorithm>

class Buffer{
private:
	std::vector<char> vec;
public:
	void append(const char* buf,size_t size){
		copy(std::back_inserter(vec),buf,buf+size);
	}
	size_t len(){
		return vec.size();
	}
	const char* buf(){
		return vec.data();
	}
};