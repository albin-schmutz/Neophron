#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "opcode.h"
#include "system.h"
#include "cpu.h"
#include "big_endian.h"
#include "runtime.h"

#define MEMORY_PAGE_SIZE 4096

#define mem32(a) *((INTEGER*)(a))

struct HEADER *last_module;

void run_init(void)
{
	heap_start = stack_start;
	reg[SP] = heap_start;
}

LONGINT run_align(LONGINT addr, INTEGER size)
{
	assert(addr >= 0);
	assert(size > 0);

	LONGINT remainder = addr % size;
	return remainder ? addr += (size - remainder) : addr;
}

static char *find_modulename(void)
{
	struct HEADER *h = last_module;
	while (h && (LONGINT)h > reg[PC]) h = h->prev;
	return h->modulename;
}

LONGINT run_create_module(char *name, INTEGER var_size,
	INTEGER prog_size, INTEGER entry_point)
{
	assert(var_size <= 0);
	assert(-var_size % WORD_SIZE == 0);

	struct HEADER *header = (void*)heap_start;
	header->prev = last_module;
	header->var_size = var_size;
	header->prog_size = prog_size;
	header->entry_point = entry_point;
	last_module = header;
	strcpy(header->modulename, name);
	heap_start = (LONGINT)header + sizeof(struct HEADER) - var_size;
	header->prog_addr = heap_start;
	return heap_start;
}

void run_exec_module(INTEGER prog_size, INTEGER entry_point)
{
	assert(last_module);
	assert(prog_size % WORD_SIZE == 0);

	if (last_module->var_size == 0 && prog_size == 0) {
		heap_start = heap_start - sizeof(struct HEADER);
		last_module = last_module->prev;
	} else {
		if (prog_size) {
			last_module->prog_size = prog_size;
			reg[PC] = heap_start;
			heap_start += last_module->prog_size;
		}
		last_module->entry_point = entry_point;
		if (entry_point != -1) {
			cpu_execute(entry_point, find_modulename);
		}
	}
}

static BOOLEAN read_ident(FILE *file, char *ident)
{
	INTEGER n = fread(ident, 1, MAX_LEN_IDENTIFIER, file);
	return n == MAX_LEN_IDENTIFIER;
}

static BOOLEAN read_i32(FILE *file, INTEGER *i)
{
	CHAR buf[4];
	INTEGER n = fread(buf, 1, 4, file);
	*i = be_to_i32(buf);
	return n == 4;
}

static BOOLEAN read_i64(FILE *file, LONGINT *i)
{
	CHAR b[8];
	INTEGER n = fread(b, 1, 8, file);
	*i = be_to_i64(b);
	return n == 8;
}

static BOOLEAN read_prog(FILE *file, struct HEADER *header)
{
	struct OPCODE opc;
	LONGINT pc;

	pc = run_create_module(header->modulename,header->var_size,
		header->prog_size, header->entry_point);
	LONGINT end = pc + header->prog_size;
	while (pc < end) {
		if (!read_i32(file, &opc.instr)) return FALSE;
		cpu_decode_instr(&opc);
		mem32(pc) = opc.instr;
		pc += INSTR_SIZE;
		if (opc.is64) {
			if (!read_i64(file, (LONGINT*)pc)) return FALSE;
			pc += WORD_SIZE;
		}
	}
	return TRUE;
}

BOOLEAN run_load_file(char *modulename)
{
	assert(modulename);

	char filename[MAX_LEN_IDENTIFIER + 8];
	struct HEADER header;

	strcpy(filename, modulename);
	strcat(filename, ".npx0");
	FILE *file = fopen(filename, "rb");
	if (!file) return FALSE;

	while (!feof(file) && read_ident(file, header.modulename)) {
		if (!(read_i32(file, &header.var_size) &&
			read_i32(file, &header.prog_size) &&
			read_i32(file, &header.entry_point))) return 0;
		if (!read_prog(file, &header)) return FALSE;
		run_exec_module(header.prog_size, header.entry_point);
	}
	return TRUE;
}
