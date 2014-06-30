#include "EchoProtocol.h"
#include "Stream.h"
#include "IOLoop.h"
int main(){
	EchoProtocol *p=new EchoProtocol;
	std::shared_ptr<IOLoop> loop=std::make_shared<IOLoop>();
	loop->addStream(listenStream(loop->get(),p));
	loop->run();
	return 0;
}