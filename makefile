all: UDPChatClient/client.out UDPChatServer/server.out
	make UDPChatClient/.
	make UDPChatServer/.

clean:
	cd UDPChatClient/. && make clean
	cd UDPChatServer/. && make clean

UDPChatClient/client.out:
	cd UDPChatClient/. && make 

UDPChatServer/server.out:
	cd UDPChatServer/. && make


