

all : .TEST bin/libconfigparse.so
	touch .Complete
	cp bin/libconfigparse.so example/.


.TEST:
	"." > CompileError.txt
	touch .TEST


bin/libconfigparse.so : obj/ConfigParser.o
	g++ -o bin/libconfigparse.so obj/ConfigParser.o -shared -fPIC -Ihdr/ -g 2>> CompileError.txt


obj/ConfigParser.o : src/ConfigParser.cpp hdr/ConfigParser.h
	g++ -c src/ConfigParser.cpp -shared -Ihdr/ -g -fPIC 2>> CompileError.txt
	mv ConfigParser.o obj/.


clean :
	rm obj/*
	rm bin/*.so
	rm .Complete
	rm example/*.so
