CC = gcc
CFLAGS = -Wall -O
LOGGER = -D LOGGER

all: logger_test system_test opcode_test emitter_test cpu_test \
	big_endian_test runtime_test

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c -o logger.o

logger_test: logger_test.c \
		logger.o
	$(CC) $(CFLAGS) logger_test.c -o tmp_logger_test \
		logger.o
	./tmp_logger_test 2> tmp_logger.log
	diff tmp_logger.log logger_test.txt

system.o: system.c system.h logger.h
	$(CC) $(CFLAGS) $(LOGGER) -c system.c

system_test: system_test.c \
		system.o logger.o
	$(CC) $(CFLAGS) $(LOGGER) system_test.c -o tmp_system_test \
		system.o logger.o
	./tmp_system_test

opcode.o: opcode.c opcode.h logger.h
	$(CC) $(CFLAGS) $(LOGGER) -c opcode.c

opcode_test: opcode_test.c \
		opcode.o logger.o
	$(CC) $(CFLAGS) $(LOGGER) opcode_test.c -o tmp_opcode_test \
		opcode.o logger.o
	./tmp_opcode_test

emitter.o: emitter.c emitter.h opcode.h logger.h
	$(CC) $(CFLAGS) $(LOGGER) -c emitter.c

emitter_test: emitter_test.c \
		emitter.o opcode.o logger.o
	$(CC) $(CFLAGS) $(LOGGER) emitter_test.c -o tmp_emitter_test \
		emitter.o opcode.o logger.o
	./tmp_emitter_test

cpu.o: cpu.c cpu.h emitter.h opcode.h system.h logger.h
	$(CC) $(CFLAGS) $(LOGGER) -c cpu.c

cpu_test: cpu_test.c \
		cpu.o emitter.o opcode.o system.o logger.o
	$(CC) $(CFLAGS) $(LOGGER) cpu_test.c -o tmp_cpu_test \
		cpu.o emitter.o opcode.o system.o logger.o
	./tmp_cpu_test

big_endian.o: big_endian.c big_endian.h
	$(CC) $(CFLAGS) -c big_endian.c

big_endian_test: big_endian_test.c big_endian.o
	$(CC) $(CFLAGS) big_endian_test.c -o tmp_big_endian_test big_endian.o
	./tmp_big_endian_test

runtime.o: runtime.c runtime.h big_endian.h cpu.h opcode.h logger.h
	$(CC) $(CFLAGS) $(LOGGER) -c runtime.c

runtime_test: runtime_test.c \
		runtime.o big_endian.o cpu.o emitter.o opcode.o system.o logger.o
	$(CC) $(CFLAGS) $(LOGGER) runtime_test.c -o tmp_runtime_test \
		runtime.o big_endian.o cpu.o emitter.o opcode.o system.o logger.o
	./tmp_runtime_test

clean:
	rm *.o tmp*
