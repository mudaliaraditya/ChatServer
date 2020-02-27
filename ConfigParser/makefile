

mkfile_path= $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir= $(dir $(mkfile_path))

CXX=g++

all : .TEST   bin/libconfigparse.so
	touch  ${mkfile_dir}/.Complete
	cp ${mkfile_dir}/bin/libconfigparse.so example/.


.TEST:
	rm -f ${mkfile_dir}/CompileError.txt || true
	touch ${mkfile_dir}/CompileError.txt
	touch  ${mkfile_dir}/.TEST
	touch .TEST


bin/libconfigparse.so : obj/ConfigParser.o
	${CXX} -o   ${mkfile_dir}/bin/libconfigparse.so  ${mkfile_dir}/obj/ConfigParser.o -shared -fPIC -Ihdr/ -Wall -g -std=c++11 2>>  ${mkfile_dir}/CompileError.txt


obj/ConfigParser.o :  ${mkfile_dir}/src/ConfigParser.cpp  ${mkfile_dir}/hdr/ConfigParser.h
	mkdir  ${mkfile_dir}/obj || true
	mkdir  ${mkfile_dir}/bin || true
	${CXX} -c  ${mkfile_dir}/src/ConfigParser.cpp -shared -Ihdr/ -g -fPIC -Wall -std=c++11 2>>  ${mkfile_dir}/CompileError.txt
	mv  ${mkfile_dir}/ConfigParser.o  ${mkfile_dir}/obj/.


clean :
	rm  ${mkfile_dir}/obj/* || true
	rm  ${mkfile_dir}/bin/*.so || true
	rm  ${mkfile_dir}/.Complete || true
	rm  ${mkfile_dir}/example/*.so || true
	rm  ${mkfile_dir}/.TEST || true
