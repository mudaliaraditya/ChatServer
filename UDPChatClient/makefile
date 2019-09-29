


all : client.out CompileError Logs

Logs :
	mkdir Logs

CompileError :
	 touch CompileError

client.out : client.o libconfigparse.so
	g++ -o client.out client.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -IConfigParser/hdr 2> CompileError
#	g++ -o client.out client.cpp -lEventResenderFunctions.cpp -std=c++11 -lpthread -g 


client.o : client.cpp
	g++ -c  client.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -IConfigParser/hdr 2> CompileError


warn : client.cpp libconfigparse.so
	g++ -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -Wall -DTESTLOGGING  -lconfigparse -L.  -IConfigParser/hdr 2> CompileError

libconfigparse.so :  ConfigParser/.Complete
	cp ConfigParser/bin/*.so .

ConfigParser/.Complete :                           
	cd ConfigParser/ && make 2> CompileError.txt    



   


clean :
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm warn || true
	rm CompileError || true
	cd ConfigParser/ && make clean || true
	rm CompileError.txt || true
