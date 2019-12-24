

CXX=g++

all : .TEST bin/libconfigparse.so
	touch .Complete
	cp bin/libconfigparse.so example/.


.TEST:
	"." > CompileError.txt
	touch .TEST


bin/libconfigparse.so : obj/ConfigParser.o
	${CXX} -o bin/libconfigparse.so obj/ConfigParser.o -shared -fPIC -Ihdr/ -Wall -g 2>> CompileError.txt


obj/ConfigParser.o : src/ConfigParser.cpp hdr/ConfigParser.h
	mkdir obj || true
	mkdir bin || true
	${CXX} -c src/ConfigParser.cpp -shared -Ihdr/ -g -fPIC -Wall 2>> CompileError.txt
	mv ConfigParser.o obj/.


clean :
	rm obj/* || true
	rm bin/*.so || true
	rm .Complete || true
	rm example/*.so || true
	rm .TEST || true
