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

uninstall:
	rm ${DESTDIR}/${PREFIX}/lib/libslog.a
	rm ${DESTDIR}/${PREFIX}/lib/libslog.so
	rm ${DESTDIR}/${PREFIX}/include/slog.h

headers_install: src/slog.h
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp src/slog.h ${DESTDIR}/${PREFIX}/include/slog.h

libslog.so: slog.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,libslog.so -o libslog.so slog.o

libslog.a: slog.o
	ar rcs libslog.a slog.o

slog.o: src/slog.cc
	${CXX} ${CXXFLAGS} -c src/slog.cc -o slog.o

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
