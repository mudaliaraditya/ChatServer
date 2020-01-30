

CXX=g++
all : .TEST server.out libconfigparse.so  libencrypt.so Logs

Logs :
	mkdir Logs

.TEST : server.out
	rm -rf CompileError.txt
	touch CompileError.txt
	touch .TEST
	
server.out :  Source.o Logger.o ../ConfigParser/.Complete NetworkSources.o  globals.o ../EncyptingFunc/Encryptcall.o
	${CXX} -o server.out -no-pie  Source.o Logger.o NetworkSources.o globals.o  -std=c++11  -lpthread -g -ggdb3 -DLOGGING -L../ConfigParser/bin -lconfigparse -I../ConfigParser/hdr ../EncyptingFunc/Encryptcall.o -ldl -I../EncyptingFunc  -I../EncyptingFunc/encryptor/ -Wl,-R. 2>> CompileError.txt || make serverA.out


serverA.out :  Source.o Logger.o ../ConfigParser/.Complete NetworkSources.o  globals.o ../EncyptingFunc/Encryptcall.o
	${CXX} -o serverA.out   Source.o Logger.o NetworkSources.o globals.o  -std=c++11  -lpthread -g -ggdb3 -DLOGGING -L../ConfigParser/bin -lconfigparse -I../ConfigParser/hdr ../EncyptingFunc/Encryptcall.o -ldl -I../EncyptingFunc  -I../EncyptingFunc/encryptor/ -Wl,-R. 2>> CompileError.txt
	mv serverA.out server.out


libconfigparse.so : ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .
	

../ConfigParser/.Complete :
	cd ../ConfigParser/ && make 2>> CompileError.txt

Source.o : Source.cpp
	${CXX} -c Source.cpp Logger.cpp -std=c++11 -ggdb3 -lpthread -g -I../ConfigParser/hdr -I../EncyptingFunc -DLOGGING -I../EncyptingFunc/encryptor/ 2>> CompileError.txt

NetworkSources.o : NetworkSources.cpp
	${CXX} -c NetworkSources.cpp -ldl -std=c++11 -ggdb3 -lpthread -g -I../ConfigParser/hdr -I../EncyptingFunc -I../EncyptingFunc/encryptor/ 2>> CompileError.txt

globals.o : globals.cpp
	${CXX} -c globals.cpp -std=c++11 -lpthread -g -ggdb3 -I../ConfigParser/hdr -I../EncyptingFunc -I../EncyptingFunc/encryptor/  2>> CompileError.txt

Logger.o : Logger.cpp
	${CXX} -c Logger.cpp  -ggdb3 -I../ConfigParser/hdr -I../ConfigParser/hdr -I../EncyptingFunc/encryptor/ -I../EncyptingFunc

../EncyptingFunc/Encryptcall.o : ../EncyptingFunc/Encryptcall.c
	${CXX} -c ../EncyptingFunc/Encryptcall.c -I../ConfigParser/hdr -I../EncyptingFunc -lconfigparse -L../ConfigParser/bin -I../EncyptingFunc/encryptor/ -g
	mv Encryptcall.o ../EncyptingFunc/.

libencrypt.so : ../EncyptingFunc/encryptor/encrypt.c
	cd ../EncyptingFunc/ && make
	cp ../EncyptingFunc/libencrypt.so .


clean : 
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm .TEST || true
	rm CompileError.txt
	cd ../ConfigParser/ && make clean || true
