all: UDPChatClient/a.out UDPChatServer/a.out
	make UDPChatClient/.
	make UDPChatServer/.

clean:
	cd UDPChatClient/. && make clean
	cd UDPChatServer/. && make clean

UDPChatClient/a.out:
	cd UDPChatClient/. && make 

UDPChatServer/a.out:
	cd UDPChatServer/. && make


