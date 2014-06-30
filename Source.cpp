//#include "EchoProtocol.h"
#include "PrintProtocol.h"
#include "Stream.h"
#include "IOLoop.h"
#include <memory>
int main(){
	//EchoProtocol *p=new EchoProtocol;
	PrintProtocol *p=new PrintProtocol;
	std::shared_ptr<IOLoop> loop=std::make_shared<IOLoop>();
	loop->addStream(listenStream(loop.get(),p));
	loop->run();
	return 0;
}
