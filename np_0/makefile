CC = gcc
CFLAGS = -Wall -O

all: system_test

system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c

system_test: system_test.c system.h \
		system.o
	$(CC) $(CFLAGS) system_test.c -o tmp_system_test \
		system.o
	./tmp_system_test

clean:
	rm *.o tmp*
