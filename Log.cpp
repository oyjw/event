#include "Log.h"
#include <cstdlib>
void log(const std::string& str){
	fprintf(stderr,"%s %s %s\n",__FUNCTION__,__LINE__,str);
}	

void log(const std::string& str,int ret){
	std::string msg=str;
	if(ret==-1){
		msg+=" failed";
	}
	log(msg);
	if(ret==-1){
		exit(1);
	}
}