INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=clang++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -O2 -g -std=c++11 -fPIC -Wall -Wextra

all: libslog.so libslog.a src/slog.h

install: all headers_install
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	cp libslog.a ${DESTDIR}/${PREFIX}/lib
	cp libslog.so ${DESTDIR}/${PREFIX}/lib
	chmod -R 544 ${DESTDIR}/${PREFIX}/lib/libslog.a
	chmod -R 544 ${DESTDIR}/${PREFIX}/lib/libslog.so

uninstall:
	rm ${DESTDIR}/${PREFIX}/lib/libslog.a
	rm ${DESTDIR}/${PREFIX}/lib/libslog.so
	rm -r ${DESTDIR}/${PREFIX}/include/slog/

headers_install: src/slog.h src/syslog.h src/file.h
	mkdir -p ${DESTDIR}/${PREFIX}/include/slog/
	cp src/slog.h ${DESTDIR}/${PREFIX}/include/slog/slog.h
	cp src/file.h ${DESTDIR}/${PREFIX}/include/slog/file.h
	cp src/syslog.h ${DESTDIR}/${PREFIX}/include/slog/syslog.h
	chmod -R 544 ${DESTDIR}/${PREFIX}/include/slog/

libslog.so: slog.o syslog.o file.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,libslog.so -o libslog.so slog.o syslog.o file.o

libslog.a: slog.o syslog.o file.o
	ar rcs libslog.a slog.o syslog.o file.o

slog.o: src/slog.cc
	${CXX} ${CXXFLAGS} -c src/slog.cc -o slog.o

syslog.o: src/syslog.cc
	${CXX} ${CXXFLAGS} -c src/syslog.cc -o syslog.o

file.o: src/file.cc
	${CXX} ${CXXFLAGS} -c src/file.cc -o file.o

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
