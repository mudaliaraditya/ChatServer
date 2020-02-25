all: UDPChatClient/client.out UDPChatServer/server.out ConfigParser/.Complete
	#make UDPChatClient/.
	#make UDPChatServer/.
	

clean:
	cd UDPChatClient/. && make clean
	cd UDPChatServer/. && make clean
	cd ConfigParser/. && make clean

UDPChatClient/client.out:
	cd UDPChatClient/. && make 

UDPChatServer/server.out:
	cd UDPChatServer/. && make

ConfigParser/.Complete :
	cd ConfigParser/ && make 

