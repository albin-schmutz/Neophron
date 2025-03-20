#include <assert.h>
#include <stdint.h>
#include "opcode.h"
#include "emitter.h"
#include "cpu.h"

static int64_t mem_size = 128;
static char mem[128];
static int64_t mem_start = (int64_t)mem;

static void finish(EMITTER emit)
{
	emit_a_b_imm(emit, POP, PC, SP, 0);
	reg[PC] = mem_start;
	reg[SP] = mem_start + mem_size;
	cpu_execute(0, 0);
}

static void test_minimal(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	finish(&emit);
	assert(reg[PC] == 0);
}

static void test_mov(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 3, 0, 123);
	emit_a_b_imm(&emit, MOV, 0, 3, 0);
	finish(&emit);
	assert(reg[0] == 123);
}

static void test_movi(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, 123);
	emit_a_b_imm(&emit, MOVI, 1, 0, -1048576);
	finish(&emit);
	assert(reg[0] == 123);
	assert(reg[1] == -1048576);
}

static void test_movi2(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, -1048577);
	emit_a_b_imm(&emit, MOVI, 1, 0, 1048575);
	emit_a_b_imm(&emit, MOVI, 2, 0, 0x7fffffffffffffff);
	emit_a_b_imm(&emit, MOVI, 3, 0, -0x8000000000000000);
	finish(&emit);
	assert(reg[0] == -1048577);
	assert(reg[1] == 1048575);
	assert(reg[2] == 0x7fffffffffffffff);
	assert(reg[3] == -0x8000000000000000);
}

static void test_loop(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, 5);
	emit_a_b_imm(&emit, SUBI, 0, 0, 1);
	emit_a_b_imm(&emit, SUBI, BR, 0, 0);
	emit_offs(&emit, BNE, -3);
	finish(&emit);
	assert(reg[0] == 0);
}

static void test_stack(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, -9999);
	emit_a_b_imm(&emit, PSH, 0, SP, -WORD_SIZE);
	emit_a_b_imm(&emit, POP, 1, SP, WORD_SIZE);
	finish(&emit);
	assert(reg[1] == -9999);
}

static void test_ld_st(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, -12345);
	emit_a_b_imm(&emit, STQ, 0, SP, -16);
	emit_a_b_imm(&emit, LDQ, 1, SP, -16);
	emit_a_b_imm(&emit, MOVI, 0, 0, 255);
	emit_a_b_imm(&emit, STB, 0, SP, -17);
	emit_a_b_imm(&emit, LDB, 1, SP, -17);
	finish(&emit);
	assert(reg[1] == 255);
}

static void test_calc_i(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 0, 0, -12345);
	emit_a_b_imm(&emit, ADDI, 1, 0, 12346);
	emit_a_b_imm(&emit, MOVI, 2, 0, -0x8000000000000000);
	emit_a_b_imm(&emit, DIVI, 3, 2, 8);
	finish(&emit);
	assert(reg[0] == -12345);
	assert(reg[1] == 1);
	assert(reg[2] == -0x8000000000000000);
	assert(reg[3] == -0x1000000000000000);
}

static void test_calc(void)
{
	struct EMITTER emit;

	emit_init(&emit, mem_start);
	emit_a_b_imm(&emit, MOVI, 2, 0, 200);
	emit_a_b_imm(&emit, MOVI, 3, 0, -4000);
	emit_a_b_imm(&emit, DIV + 1, 3, 2, 0);
	finish(&emit);
	assert(reg[1] == -20);
}

int main(void)
{
	test_minimal();
	test_mov();
	test_movi();
	test_movi2();
	test_loop();
	test_stack();
	test_ld_st();
	test_calc_i();
	test_calc();

	return 0;
}
