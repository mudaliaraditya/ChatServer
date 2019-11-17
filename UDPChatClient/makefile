
CXX=g++

all : libconfigparse.so  client.out libencrypt.so CompileError Logs .TEST

Logs :
	mkdir Logs

.TEST : 
	rm CompileError.txt || true
	touch CompileError.txt

CompileError :
	 touch CompileError

client.out : client.o
	${CXX} -o client.out client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr -IEncyptingFunc -IEncyptingFunc/encryptor EncyptingFunc/Encryptcall.c -ldl  2> CompileError
	touch .TEST

client.o : client.cpp NetworkSources.cpp
	${CXX} -c  client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr  -IEncyptingFunc -IEncyptingFunc/encryptor EncyptingFunc/Encryptcall.c  2> CompileError


warn : client.cpp NetworkSources.cpp libconfigparse.so
	${CXX} -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -Wall -DTESTLOGGING  -lconfigparse -L.  -I../ConfigParser/hdr  -IEncyptingFunc 2> CompileError

libconfigparse.so :  ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .

../ConfigParser/.Complete :                           
	cd ../ConfigParser/ && make 2> CompileError.txt    


libencrypt.so : EncyptingFunc/encryptor/encrypt.c EncyptingFunc/encryptor/encrypt.h
	cd EncyptingFunc && make 
   


clean :
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm warn || true
	rm CompileError || true
	cd ../ConfigParser/ && make clean || true
	rm CompileError.txt || true
	rm .TEST
