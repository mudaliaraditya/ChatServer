
CXX=g++

all : libconfigparse.so  client.out libencrypt.so Logs

Logs :
	mkdir Logs



client.out : client.o
	${CXX} -o client.out client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr -I../EncyptingFunc -I../EncyptingFunc/encryptor ../EncyptingFunc/Encryptcall.c -ldl -Wl,-R.

client.o : client.cpp NetworkSources.cpp
	${CXX} -c  client.cpp NetworkSources.cpp -std=c++11 -lpthread -g  -lconfigparse -L. -I../ConfigParser/hdr  -I../EncyptingFunc -I../EncyptingFunc/encryptor ../EncyptingFunc/Encryptcall.c  


warn : client.cpp NetworkSources.cpp libconfigparse.so
	${CXX} -o client.out client.cpp -std=c++11 -lpthread -g -DLOGGING -Wall -DTESTLOGGING  -lconfigparse -L.  -I../ConfigParser/hdr  -I../EncyptingFunc 

libconfigparse.so :  ../ConfigParser/.Complete
	cp ../ConfigParser/bin/*.so .

../ConfigParser/.Complete :                           
	cd ../ConfigParser/ && make


libencrypt.so : ../EncyptingFunc/encryptor/encrypt.c ../EncyptingFunc/encryptor/encrypt.h
	cd ../EncyptingFunc && make
	cp ../EncyptingFunc/libencrypt.so .
   


clean :
	rm *.out || true
	rm *.so || true
	rm *.o || true
	rm warn || true
	cd ../ConfigParser/ && make clean || true
