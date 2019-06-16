


all : server.out libconfigparse.so
	mkdir Logs || true
	
server.out :  Source.o libconfigparse.so NetworkSources.o  globals.o
	g++ -o server.out Source.o NetworkSources.o globals.o -std=c++11 -lpthread -g -DLOGGING -L. -lconfigparse

libconfigparse.so : ConfigParser.o
	g++ -o libconfigparse.so ConfigParser.o -g -shared
	
ConfigParser.o : ConfigParser.cpp ConfigParser.h
	g++ -c ConfigParser.cpp -fPIC

Source.o : Source.cpp
	g++ -c Source.cpp -std=c++11 -lpthread -g

NetworkSources.o : NetworkSources.cpp
	g++ -c NetworkSources.cpp -std=c++11 -lpthread -g

globals.o : globals.cpp
	g++ -c globals.cpp -std=c++11 -lpthread -g

clean : 
	rm *.out || true
	rm *.so || true
