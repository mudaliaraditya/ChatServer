


all : .TEST server.out libconfigparse.so 
	mkdir Logs || true

.TEST :
	rm -rf CompileError.txt
	touch CompileError.txt
	
server.out :  Source.o libconfigparse.so NetworkSources.o  globals.o
	g++ -o server.out Source.o NetworkSources.o globals.o -std=c++11 -lpthread -g -DLOGGING -L. -lconfigparse 2>> CompileError.txt
	touch .TEST

libconfigparse.so : ConfigParser.o
	g++ -o libconfigparse.so ConfigParser.o -g -shared 2>> CompileError.txt
	
ConfigParser.o : ConfigParser.cpp ConfigParser.h
	g++ -c ConfigParser.cpp -fPIC 2>> CompileError.txt

Source.o : Source.cpp
	g++ -c Source.cpp -std=c++11 -lpthread -g 2>> CompileError.txt

NetworkSources.o : NetworkSources.cpp
	g++ -c NetworkSources.cpp -std=c++11 -lpthread -g 2>> CompileError.txt

globals.o : globals.cpp
	g++ -c globals.cpp -std=c++11 -lpthread -g 2>> CompileError.txt

clean : 
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm .TEST
