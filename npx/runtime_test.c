#include <assert.h>
#include <stdint.h>
#include "opcode.h"
#include "emitter.h"
#include "cpu.h"
#include "runtime.h"

static void test_align(void)
{
	assert(run_align(3, 8) == 8);
	assert(run_align(100, 8) == 104);
	assert(run_align(105, 8) == 112);

	assert(run_align(99, 4) == 100);
	assert(run_align(100, 4) == 100);
	assert(run_align(101, 4) == 104);

	assert(run_align(0, 1) == 0);
	assert(run_align(0, 2) == 0);
	assert(run_align(1, 2) == 2);
	assert(run_align(2, 2) == 2);
}

static void test_create_modules(void)
{
	int64_t prog_addr;
	struct EMITTER emit;
	int header_size = sizeof(struct HEADER);

	prog_addr = run_create_module("tmpM1", -16);
	emit_init(&emit, prog_addr);
	emit_a_b_imm(&emit, MOVI, 0, 0, 42);
	emit_a_b_imm(&emit, MOV, PC, BR, 0);
	run_exec_module(8, -1);

	prog_addr = run_create_module("tmpM2", 0);
	emit_init(&emit, prog_addr);
	emit_offs(&emit, CALL, header_size / -INSTR_SIZE - 3);
	emit_a_b_imm(&emit, POP, PC, SP, 0);
	run_exec_module(8, 0);
	run_write_file();

	assert(reg[0] == 42);
}

static void test_load_modules(void)
{
	reg[0] = 0;
	assert(run_load_file("tmpM2"));
	assert(reg[0] == 42);
}

int main(void)
{
	test_align();
	run_init();
	test_create_modules();
	run_init();
	test_load_modules();

	return 0;
}
