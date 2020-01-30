

all : example


example : main.o
	g++ -o example main.o  -I../hdr -L. -lconfigparse -g -Wl,-R.

main.o : main.cpp
	g++ -c main.cpp -I../hdr -g

clean :
	rm example || true
	rm *.o || true
