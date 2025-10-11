CC = gcc
CFLAGS = -Wall -O

all: arch_test emitter_test 

arch_test: arch_test.c
	$(CC) $(CFLAGS) arch_test.c -o tmp_arch_test
	./tmp_arch_test

emitter.o: emitter.c emitter.h opcode.h
	$(CC) $(CFLAGS) -c emitter.c

emitter_test: emitter_test.c emitter.h opcode.h \
		emitter.o
	$(CC) $(CFLAGS) emitter_test.c -o tmp_emitter_test \
		emitter.o
	./tmp_emitter_test

clean:
	rm *.o tmp*
