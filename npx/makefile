CC = gcc
CFLAGS = -Wall -O

all: opcode_test emitter_test system_test cpu_test big_endian_test runtime_test

opcode.o: opcode.c opcode.h
	$(CC) $(CFLAGS) -c opcode.c

opcode_test: opcode_test.c opcode.h \
		opcode.o
	$(CC) $(CFLAGS) opcode_test.c -o tmp_opcode_test \
		opcode.o
	./tmp_opcode_test

emitter.o: emitter.c emitter.h opcode.h
	$(CC) $(CFLAGS) -c emitter.c

emitter_test: emitter_test.c emitter.h opcode.h \
		emitter.o opcode.o
	$(CC) $(CFLAGS) emitter_test.c -o tmp_emitter_test \
		emitter.o opcode.o
	./tmp_emitter_test

system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c

system_test: system_test.c system.h \
		system.o
	$(CC) $(CFLAGS) system_test.c -o tmp_system_test \
		system.o
	./tmp_system_test

cpu.o: cpu.c cpu.h opcode.h system.h
	$(CC) $(CFLAGS) -c cpu.c

cpu_test: cpu_test.c cpu.h emitter.h opcode.h \
		cpu.o system.o emitter.o opcode.o
	$(CC) $(CFLAGS) cpu_test.c -o tmp_cpu_test \
		cpu.o system.o emitter.o opcode.o
	./tmp_cpu_test

cpu_hello_world: cpu_hello_world.c cpu.h system.h emitter.h opcode.h \
		cpu.o system.o emitter.o opcode.o
	$(CC) $(CFLAGS) cpu_hello_world.c -o tmp_cpu_hello_world \
		cpu.o system.o emitter.o opcode.o
	./tmp_cpu_hello_world

big_endian.o: big_endian.c big_endian.h
	$(CC) $(CFLAGS) -c big_endian.c

big_endian_test: big_endian_test.c big_endian.h \
		big_endian.o
	$(CC) $(CFLAGS) big_endian_test.c -o tmp_big_endian_test \
		big_endian.o
	./tmp_big_endian_test

runtime.o: runtime.c runtime.h big_endian.h cpu.h opcode.h
	$(CC) $(CFLAGS) -c runtime.c

runtime_test: runtime_test.c runtime.h cpu.h emitter.h opcode.h \
		runtime.o big_endian.o cpu.o system.o emitter.o opcode.o
	$(CC) $(CFLAGS) runtime_test.c -o tmp_runtime_test \
		runtime.o big_endian.o cpu.o system.o emitter.o opcode.o
	./tmp_runtime_test


clean:
	rm *.o tmp*
