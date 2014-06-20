#include "Protocol.h"
#include "Stream.h"
#include "IOLoop.h"
int main(){
	Protocol *p=new Protocol;
	std::shared_ptr<IOLoop> loop=std::make_shared<IOLoop>(p);
	loop->addStream(listenStream());
	loop->run();
	return 0;
}