all:Buffer.o Log.o PrintProtocol.o Protocol.o stream.o IOLoop.o Source.o
	gcc -Wall -g -o pp Buffer.cpp Log.cpp PrintProtocol.cpp Protocol.cpp stream.cpp IOLoop.cpp Source.cpp
.PHONY:clean
clean:
	rm pp *.o
