CFLAGS=-std=c++11 -g -wall
all:
	g++ -std=c++11 -g -gdwarf-2 -O0 -o pp Buffer.cpp Log.cpp PrintProtocol.cpp Protocol.cpp Stream.cpp IOLoop.cpp Source.cpp
%.o:%.cpp
	g++ -std=c++11 -Wall -g -c $^
.PHONY:clean
clean:
	rm pp *.o
