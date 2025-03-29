CC = gcc
CFLAGS = -Wall -O

all: reader_string_test reader_file_test source_test

reader_string.o: reader_string.c reader_string.h reader.h
	$(CC) $(CFLAGS) -c reader_string.c

reader_string_test: reader_string_test.c reader_string.h reader.h \
		reader_string.o
	$(CC) $(CFLAGS) reader_string_test.c -o tmp_reader_string_test \
		reader_string.o
	./tmp_reader_string_test

reader_file.o: reader_file.c reader_file.h reader.h
	$(CC) $(CFLAGS) -c reader_file.c

reader_file_test: reader_file_test.c reader_file.h reader.h \
		reader_file.o
	$(CC) $(CFLAGS) reader_file_test.c -o tmp_reader_file_test \
		reader_file.o
	./tmp_reader_file_test

source.o: source.c source.h reader.h
	$(CC) $(CFLAGS) -c source.c

source_test: source_test.c source.h reader_file.h reader_string.h reader.h \
		source.o reader_file.o reader_string.o
	$(CC) $(CFLAGS) source_test.c -o tmp_source_test \
		source.o reader_file.o reader_string.o
	./tmp_source_test

clean:
	rm *.o tmp*
