CFLAGS=-std=c++11 -g -wall
all:Buffer.o Log.o PrintProtocol.o Protocol.o Stream.o IOLoop.o Source.o
	g++ -std=c++11 -Wall -g -o pp Buffer.cpp Log.cpp PrintProtocol.cpp Protocol.cpp Stream.cpp IOLoop.cpp Source.cpp
%.o:%.cpp
	g++ -std=c++11 -Wall -g -c $^
.PHONY:clean
clean:
	rm pp *.o
