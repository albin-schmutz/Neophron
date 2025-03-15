CC = gcc
CFLAGS = -Wall -O

all: opcode_test

opcode.o: opcode.c opcode.h
	$(CC) $(CFLAGS) -c opcode.c

opcode_test: opcode_test.c \
		opcode.o
	$(CC) $(CFLAGS) opcode_test.c -o tmp_opcode_test \
		opcode.o
	./tmp_opcode_test

clean:
	rm *.o tmp*
