#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "opcode.h"
#include "system.h"
#include "cpu.h"
#include "big_endian.h"
#include "runtime.h"
#include "sourcefile.h"
#include "parser.h"

#define mem32(a) *((INTEGER*)(a))

static void write_ident(FILE *file, char *ident)
{
	INTEGER n = fwrite(ident, 1, MAX_LEN_IDENTIFIER, file);
	assert(n == MAX_LEN_IDENTIFIER);
}

static void write_i32(FILE *file, INTEGER i)
{
	CHAR b[4];
	be_from_i32(b, i);
	INTEGER n = fwrite(b, 1, 4, file);
	assert(n == 4);
}

static void write_i64(FILE *file, LONGINT i)
{
	CHAR b[8];
	be_from_i64(b, i);
	INTEGER n = fwrite(b, 1, 8, file);
	assert(n == 8);
}

static void write_prog(FILE *file, LONGINT prog_addr, INTEGER prog_size)
{
	assert(file);
	assert(prog_addr);
	assert(prog_addr % WORD_SIZE == 0);
	assert(prog_size % WORD_SIZE == 0);

	struct OPCODE opc;
	void* pc = (void*)prog_addr;
	void* end = pc + prog_size;
	while (pc < end) {
		opc.instr = mem32(pc);
		write_i32(file, opc.instr);
		cpu_decode_instr(&opc);
		pc += INSTR_SIZE;
		if (opc.is64) {
			write_i64(file, *((LONGINT*)pc));
			pc += WORD_SIZE;
		}
	}
}

static void write_modules(FILE *file, struct HEADER *header)
{
	if (header) {
		write_modules(file, header->prev);
		write_ident(file, header->modulename);
		write_i32(file, header->var_size);
		write_i32(file, header->prog_size);
		write_i32(file, header->entry_point);
		write_prog(file, header->prog_addr, header->prog_size);
	}
}

static void write_file(void)
{
	char filename[MAX_LEN_IDENTIFIER + 8];
	if (last_module) {
		strcpy(filename, last_module->modulename);
		strcat(filename, ".npx0");
		FILE *file = fopen(filename, "wb");
		assert(file);
		write_modules(file, last_module);
		fclose(file);
	}
}

int main(int argc, char *argv[])
{
	assert(argc >= 2);

	char *module = argv[1];

	sys_init(argc - 1, &argv[1]);
	run_init();
	if (!run_load_file(module)) {
		parser(module);
		write_file();
	}
	return 0;
}
