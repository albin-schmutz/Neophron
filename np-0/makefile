CC = gcc
CFLAGS = -Wall -O


all: buffer_test


buffer.o: buffer.c buffer.h global.h
	$(CC) $(CFLAGS) -c buffer.c

buffer_test: buffer.h global.h buffer.o
	$(CC) $(CFLAGS) buffer_test.c -o tmp_buffer_test buffer.o
	./tmp_buffer_test


clean:
	rm *.o tmp*
