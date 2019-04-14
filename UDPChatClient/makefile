


all : client.out 

client.out : client.cpp clean
	g++ -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -DTESTLOGGING
#	g++ -o client.out client.cpp -lEventResenderFunctions.cpp -std=c++11 -lpthread -g 

clean :
	rm *.out || true
