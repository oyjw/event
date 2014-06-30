#include "Log.h"
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdio>

void log(const std::string& str){
	fprintf(stderr,"%s\n",str.c_str());
}	

void log(const std::string& str,int ret,bool willExit){
	std::string msg=str;
	if(ret==-1){
		msg=msg+" failed: "+strerror(errno);
	}
	log(msg);
	if(ret==-1 && willExit){
		exit(1);
	}
}
