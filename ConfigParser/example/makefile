

all : example


example : main.o
	gcc -o example main.o  -I../hdr -L. -lconfigparse -g -no-pie -Wl,-R.

main.o : main.c
	gcc -c main.c -I../hdr -g

clean :
	rm example || true
	rm *.o || true
