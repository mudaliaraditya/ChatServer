


all : client.out CompileError Logs

Logs :
	mkdir Logs

CompileError :
	touch CompileError

client.out : client.cpp libconfigparse.so
	g++ -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -DTESTLOGGING  -lconfigparse -L. -IConfigParser/hdr/ 2> CompileError
#	g++ -o client.out client.cpp -lEventResenderFunctions.cpp -std=c++11 -lpthread -g 

warn : client.cpp libconfigparse.so
	g++ -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -Wall -DTESTLOGGING  -lconfigparse -L. -IConfigParser/hdr/ 2> CompileError

libconfigparse.so : ConfigParser/.Complete
	cd ConfigParser && make
	cp ConfigParser/bin/*.so .
   


clean :
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm warn || true
	rm CompileError || true
	cd ConfigParser && make clean
