CC = gcc
CFLAGS = -Wall -O

all: scanner_test

scanner.o: scanner.c scanner.h ../../Lib/src/source.h \
		../../Lib/src/reader_file.h ../../Lib/src/reader.h
	$(CC) $(CFLAGS) -c scanner.c

scanner_test: scanner_test.c scanner.h ../../Lib/src/source.h \
		../../Lib/src/reader_file.h ../../Lib/src/reader.h \
		scanner.o ../../Lib/src/source.o ../../Lib/src/reader_file.o
	$(CC) $(CFLAGS) scanner_test.c -o tmp_scanner_test \
		scanner.o ../../Lib/src/source.o ../../Lib/src/reader_file.o
	./tmp_scanner_test

clean:
	rm *.o tmp*
