

all : example.out example2.out example3-cpp.out


example.out : main.o
	gcc -o example.out main.o  -I../hdr -L. -lconfigparse -g -no-pie -Wl,-R.

main.o : main.c
	gcc -c main.c -I../hdr -g

example2.out : timecheck.o
	gcc -o example2.out timecheck.o  -I../hdr -L. -lconfigparse -g -no-pie -Wl,-R.

example3-cpp.out : config-static.o
	g++ -o example3-cpp.out "config-static.cpp" -static -I../hdr -L. -lconfigparse-static -g -no-pie 

config-static.o : config-static.cpp
	g++ -c  config-static.cpp -static -I../hdr -L. -lconfigparse-static -g -no-pie 


timecheck.o : timecheck.c
	gcc -c timecheck.c -I../hdr -g



clean :
	rm *.out || true
	rm *.o || true
