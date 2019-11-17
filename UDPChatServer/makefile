

CXX=g++
all : .TEST server.out libconfigparse.so  libencrypt.so Logs

Logs :
	mkdir Logs

.TEST : server.out
	rm -rf CompileError.txt
	touch CompileError.txt
	touch .TEST
	
server.out :  Source.o ../ConfigParser/.Complete NetworkSources.o  globals.o EncyptingFunc/Encryptcall.o
	${CXX} -o server.out Source.o NetworkSources.o globals.o  -std=c++11 -ldl -lpthread -g  -L../ConfigParser/bin -lconfigparse -I../ConfigParser/hdr EncyptingFunc/Encryptcall.o -IEncyptingFunc  -IEncyptingFunc/encryptor/ -Wl,-R. 2>> CompileError.txt

libconfigparse.so : ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .
	

../ConfigParser/.Complete :
	cd ../ConfigParser/ && make 2>> CompileError.txt

Source.o : Source.cpp
	${CXX} -c Source.cpp -std=c++11 -lpthread -g -I../ConfigParser/hdr -IEncyptingFunc -IEncyptingFunc/encryptor/ 2>> CompileError.txt

NetworkSources.o : NetworkSources.cpp
	${CXX} -c NetworkSources.cpp -std=c++11 -lpthread -g -I../ConfigParser/hdr -IEncyptingFunc -IEncyptingFunc/encryptor/ 2>> CompileError.txt

globals.o : globals.cpp
	${CXX} -c globals.cpp -std=c++11 -lpthread -g -I../ConfigParser/hdr -IEncyptingFunc -IEncyptingFunc/encryptor/  2>> CompileError.txt

EncyptingFunc/Encryptcall.o : EncyptingFunc/Encryptcall.c
	${CXX} -c EncyptingFunc/Encryptcall.c -I../ConfigParser/hdr -IEncyptingFunc -lconfigparse -L../ConfigParser/bin -IEncyptingFunc/encryptor/ -g
	mv Encryptcall.o EncyptingFunc/.

libencrypt.so : EncyptingFunc/encryptor/encrypt.c
	cd EncyptingFunc/ && make


clean : 
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm .TEST || true
	cd ../ConfigParser/ && make clean || true
