

CXX=g++
all : .TEST server.out libconfigparse.so Logs

Logs :
	mkdir Logs

.TEST :
	rm -rf CompileError.txt
	touch CompileError.txt
	
server.out :  Source.o ConfigParser/.Complete NetworkSources.o  globals.o
	${CXX} -o server.out Source.o NetworkSources.o globals.o  -std=c++11 -lpthread -g  -LConfigParser/bin -lconfigparse -IConfigParser/hdr 2>> CompileError.txt
	touch .TEST

libconfigparse.so : ConfigParser/.Complete
	cp ConfigParser/bin/*.so .
	

ConfigParser/.Complete :
	cd ConfigParser/ && make 2> CompileError.txt

Source.o : Source.cpp
	${CXX} -c Source.cpp -std=c++11 -lpthread -g -IConfigParser/hdr 2>> CompileError.txt

NetworkSources.o : NetworkSources.cpp
	${CXX} -c NetworkSources.cpp -std=c++11 -lpthread -g -IConfigParser/hdr 2>> CompileError.txt

globals.o : globals.cpp
	${CXX} -c globals.cpp -std=c++11 -lpthread -g -IConfigParser/hdr 2>> CompileError.txt

clean : 
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm .TEST || true
	cd ConfigParser/ && make clean || true
