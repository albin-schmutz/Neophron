CC = gcc
CFLAGS = -Wall -O

all: arch_test

arch_test: arch_test.c global.h
	$(CC) $(CFLAGS) arch_test.c -o tmp_arch_test
	./tmp_arch_test

clean:
	rm tmp*
