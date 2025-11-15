CC = gcc
CFLAGS = -Wall -O

all: arch_test \
	emitter_test system_test cpu_test big_endian_test runtime_test \
	sourcefile_test token_test scanner_test \
	alloc_free_test tables_test \
	np_0 npx_0 test_nx

arch_test: arch_test.c global.h
	$(CC) $(CFLAGS) arch_test.c -o tmp_arch_test
	./tmp_arch_test

emitter.o: emitter.c emitter.h opcode.h global.h
	$(CC) $(CFLAGS) -c emitter.c

emitter_test: emitter_test.c emitter.h opcode.h global.h \
		emitter.o
	$(CC) $(CFLAGS) emitter_test.c -o tmp_emitter_test \
		emitter.o
	./tmp_emitter_test

system.o: system.c system.h global.h
	$(CC) $(CFLAGS) -c system.c

system_test: system_test.c system.h global.h \
		system.o
	$(CC) $(CFLAGS) system_test.c -o tmp_system_test \
		system.o
	./tmp_system_test

cpu.o: cpu.c cpu.h system.h opcode.h global.h
	$(CC) $(CFLAGS) -c cpu.c

cpu_test: cpu_test.c cpu.h emitter.h opcode.h global.h \
		cpu.o system.o emitter.o
	$(CC) $(CFLAGS) cpu_test.c -o tmp_cpu_test \
		cpu.o system.o emitter.o
	./tmp_cpu_test

cpu_hello_world: cpu_hello_world.c cpu.h \
		system.h emitter.h opcode.h global.h \
		cpu.o system.o emitter.o
	$(CC) $(CFLAGS) cpu_hello_world.c -o tmp_cpu_hello_world \
		cpu.o system.o emitter.o
	./tmp_cpu_hello_world

big_endian.o: big_endian.c big_endian.h global.h
	$(CC) $(CFLAGS) -c big_endian.c

big_endian_test: big_endian_test.c big_endian.h global.h \
		big_endian.o
	$(CC) $(CFLAGS) big_endian_test.c -o tmp_big_endian_test \
		big_endian.o
	./tmp_big_endian_test

runtime.o: runtime.c runtime.h big_endian.h cpu.h system.h opcode.h global.h
	$(CC) $(CFLAGS) -c runtime.c

runtime_test: runtime_test.c runtime.h cpu.h emitter.h opcode.h global.h \
		runtime.o big_endian.o cpu.o system.o emitter.o
	$(CC) $(CFLAGS) runtime_test.c -o tmp_runtime_test \
		runtime.o big_endian.o cpu.o system.o emitter.o
	./tmp_runtime_test

sourcefile.o: sourcefile.c sourcefile.h global.h
	$(CC) $(CFLAGS) -c sourcefile.c

sourcefile_test: sourcefile_test.c sourcefile.h global.h \
		sourcefile.o
	$(CC) $(CFLAGS) sourcefile_test.c -o tmp_sourcefile_test \
		sourcefile.o
	./tmp_sourcefile_test

token.o: token.c token.h global.h
	$(CC) $(CFLAGS) -c token.c

token_test: token_test.c token.h global.h \
		token.o
	$(CC) $(CFLAGS) token_test.c -o tmp_token_test \
		token.o
	./tmp_token_test

scanner.o: scanner.c scanner.h token.h sourcefile.h global.h
	$(CC) $(CFLAGS) -c scanner.c

scanner_test: scanner_test.c scanner.h sourcefile.h global.h \
		scanner.o token.o sourcefile.o
	$(CC) $(CFLAGS) scanner_test.c -o tmp_scanner_test \
		scanner.o token.o sourcefile.o
	./tmp_scanner_test

alloc_free.o: alloc_free.c alloc_free.h global.h
	$(CC) $(CFLAGS) -c alloc_free.c

alloc_free_test: alloc_free_test.c alloc_free.h global.h \
		alloc_free.o
	$(CC) $(CFLAGS) alloc_free_test.c -o tmp_alloc_free_test \
		alloc_free.o
	./tmp_alloc_free_test

tables.o: tables.c tables.h alloc_free.h clazz.h global.h
	$(CC) $(CFLAGS) -c tables.c

tables_test: tables_test.c tables.h clazz.h global.h \
		tables.o alloc_free.o
	$(CC) $(CFLAGS) tables_test.c -o tmp_tables_test \
		tables.o alloc_free.o
	./tmp_tables_test

generator.o: generator.c generator.h clazz.h emitter.h opcode.h global.h
	$(CC) $(CFLAGS) -c generator.c

parser.o: parser.c parser.h generator.h tables.h clazz.h \
		scanner.h token.h sourcefile.h \
		runtime.h cpu.h opcode.h global.h
	$(CC) $(CFLAGS) -c parser.c

np_0: np_0.c parser.h sourcefile.h \
		runtime.h big_endian.h cpu.h system.h opcode.h global.h \
		parser.o generator.o \
		tables.o alloc_free.o \
		scanner.o token.o sourcefile.o \
		runtime.o big_endian.o cpu.o system.o emitter.o
	$(CC) $(CFLAGS) np_0.c -o np_0 \
		parser.o generator.o \
		tables.o alloc_free.o \
		scanner.o token.o sourcefile.o \
		runtime.o big_endian.o cpu.o system.o emitter.o

npx_0: npx_0.c runtime.h system.h global.h \
		runtime.o big_endian.o cpu.o system.o
	$(CC) $(CFLAGS) npx_0.c -o npx_0 \
		runtime.o big_endian.o cpu.o system.o

test_nx: np_0
	./np_0 test01
	./np_0 test05
	./np_0 test07
	./np_0 test11
	./np_0 test13
	./np_0 test17
	./np_0 test19

clean:
	rm *.o *.npx0 tmp* np_0 npx_0
