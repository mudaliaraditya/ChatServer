

all : example example2


example : main.o
	gcc -o example main.o  -I../hdr -L. -lconfigparse -g -no-pie -Wl,-R.

main.o : main.c
	gcc -c main.c -I../hdr -g

example2 : timecheck.o
	gcc -o example2 timecheck.o  -I../hdr -L. -lconfigparse -g -no-pie -Wl,-R.

timecheck.o : timecheck.c
	gcc -c timecheck.c -I../hdr -g



clean :
	rm example || true
	rm example2 || true
	rm *.o || true
