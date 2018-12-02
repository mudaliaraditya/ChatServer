
all : serverthreadmain.out client.out
	
serverthreadmain.out : serverthreadmain.o
	g++ -o serverthreadmain.out serverthreadmain.o -std=c++11 -g -lpthread

serverthreadmain.o : serverthreadmain.cpp
	g++ -c serverthreadmain.cpp -std=c++11 -g -lpthread

client.out : client.o	
	g++ -o client.out client.cpp -g -lpthread -std=c++11

client.o : client.cpp	
	g++ -c  client.cpp -g -lpthread -std=c++11
	
clean :
	rm *.out || true
