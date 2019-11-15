
CXX=g++

all : client.out CompileError Logs

Logs :
	mkdir Logs

CompileError :
	 touch CompileError

client.out : client.o libconfigparse.so
	${CXX} -o client.out client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr 2> CompileError

client.o : client.cpp NetworkSources.cpp
	${CXX} -c  client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr 2> CompileError


warn : client.cpp NetworkSources.cpp libconfigparse.so
	${CXX} -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -Wall -DTESTLOGGING  -lconfigparse -L.  -I../ConfigParser/hdr 2> CompileError

libconfigparse.so :  ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .

../ConfigParser/.Complete :                           
	cd ../ConfigParser/ && make 2> CompileError.txt    



   


clean :
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm warn || true
	rm CompileError || true
	cd ../ConfigParser/ && make clean || true
	rm CompileError.txt || true
