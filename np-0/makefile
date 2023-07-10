CC = gcc
CFLAGS = -Wall

all: test

test: big_endian_wbt big_endian_bbt

big_endian.o: big_endian.c big_endian.h oberon.h
	$(CC) $(CFLAGS) -c big_endian.c

big_endian_wbt: big_endian_wbt.c big_endian.o
	$(CC) $(CFLAGS) big_endian_wbt.c -o tmp_big_endian_wbt
	./tmp_big_endian_wbt

big_endian_bbt: big_endian_bbt.c big_endian.o
	$(CC) $(CFLAGS) big_endian_bbt.c -o tmp_big_endian_bbt big_endian.o
	./tmp_big_endian_bbt

clean:
	rm *.o tmp*
