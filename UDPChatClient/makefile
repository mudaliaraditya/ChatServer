


all : client.out

client.out : client.cpp libconfigparse.so
	g++ -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -DTESTLOGGING -Wall -lconfigparse -L.
#	g++ -o client.out client.cpp -lEventResenderFunctions.cpp -std=c++11 -lpthread -g 

libconfigparse.so : ConfigParser.o
	g++ -o libconfigparse.so ConfigParser.o -g -shared
   
ConfigParser.o : ConfigParser.cpp ConfigParser.h
	g++ -c ConfigParser.cpp -fPIC -g


clean :
	rm *.out || true
	rm *.so || true
