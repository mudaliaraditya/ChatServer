

CXX=g++
all :  server.out libconfigparse.so  libencrypt.so Logs

Logs :
	mkdir Logs

	
server1.out :  Source.o Logger.o ../ConfigParser/.Complete NetworkSources.o  globals.o ../EncyptingFunc/Encryptcall.o
	${CXX} -o server1.out -no-pie  Source.o Logger.o NetworkSources.o globals.o  -std=c++11  -lpthread -g -ggdb3 -L../ConfigParser/bin -lconfigparse -I../ConfigParser/hdr ../EncyptingFunc/Encryptcall.o -ldl -I../EncyptingFunc  -I../EncyptingFunc/encryptor/ -Wl,-R. 


server2.out :  Source.o Logger.o ../ConfigParser/.Complete NetworkSources.o  globals.o ../EncyptingFunc/Encryptcall.o
	${CXX} -o server2.out  Source.o Logger.o NetworkSources.o globals.o  -std=c++11  -lpthread -g -ggdb3 -L../ConfigParser/bin -lconfigparse -I../ConfigParser/hdr ../EncyptingFunc/Encryptcall.o -ldl -I../EncyptingFunc  -I../EncyptingFunc/encryptor/ -Wl,-R. 


server.out : Source.o Logger.o ../ConfigParser/.Complete NetworkSources.o  globals.o ../EncyptingFunc/Encryptcall.o
	make server1.out || make server2.out
	mv server1.out server.out || mv server2.out server.out 


libconfigparse.so : ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .
	

../ConfigParser/.Complete :
	cd ../ConfigParser/ && make 

Source.o : Source.cpp defines.h includes.h types.h enums.h globals.h
	${CXX} -c Source.cpp -std=c++11 -ggdb3 -lpthread -g -I../ConfigParser/hdr -I../EncyptingFunc -DLOGGING -I../EncyptingFunc/encryptor/ 

NetworkSources.o : NetworkSources.cpp
	${CXX} -c NetworkSources.cpp -ldl -std=c++11 -ggdb3 -lpthread -g -I../ConfigParser/hdr -I../EncyptingFunc -I../EncyptingFunc/encryptor/

globals.o : globals.cpp
	${CXX} -c globals.cpp -std=c++11 -lpthread -g -ggdb3 -I../ConfigParser/hdr -I../EncyptingFunc -I../EncyptingFunc/encryptor/  

Logger.o : Logger.c
	${CXX} -c Logger.c  -ggdb3 -I../ConfigParser/hdr -I../ConfigParser/hdr -I../EncyptingFunc/encryptor/ -I../EncyptingFunc

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
	rm CompileError.txt || true
	cd ../ConfigParser/ && make clean || true
