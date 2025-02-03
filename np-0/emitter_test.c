#include <assert.h>
#include <stdint.h>
#include "logger.h"
#include "opcode.h"
#include "emitter.h"

static void test_offs(void)
{
	int prog_space[4];
	struct EMITTER emit;
	struct OPCODE opcode;

	LOG_MSG(LOG_EMIT_TEST, "\nemit test_offs");

	emit_init(&emit, (int64_t)prog_space);
	emit_offs(&emit, JUMP, 666);
	emit_offs(&emit, BLS, -4);

	opcode.instr = prog_space[0];
	oc_instr(&opcode);
	assert(opcode.oc == JUMP);
	assert(opcode.imm == 666);

	opcode.instr = prog_space[1];
	oc_instr(&opcode);
	assert(opcode.oc == BLS);
	assert(opcode.imm == -4);
}

static void test_a_b_imm(void)
{
	int prog_space[5];
	struct EMITTER emit;
	struct OPCODE opcode;

	LOG_MSG(LOG_EMIT_TEST, "\nemit test_a_b_imm");

	emit_init(&emit, (int64_t)prog_space);
	emit_a_b_imm(&emit, SUBI, 1, 2, -1000000000);
	emit_a_b_imm(&emit, PSH, 2, 3, 100);
	emit_a_b_imm(&emit, NEG, 7, 1, 0);

	opcode.instr = prog_space[0];
	oc_instr(&opcode);
	assert(opcode.oc == SUBI2);
	assert(opcode.a == 1);
	assert(opcode.b == 2);
	assert(opcode.is64);
	assert(*((long long*)&prog_space[1]) == -1000000000);

	opcode.instr = prog_space[3];
	oc_instr(&opcode);
	assert(opcode.oc == PSH);
	assert(opcode.a == 2);
	assert(opcode.b == 3);
	assert(opcode.imm == 100);

	opcode.instr = prog_space[4];
	oc_instr(&opcode);
	assert(opcode.oc == NEG);
	assert(opcode.a == 7);
	assert(opcode.b == 1);
}

static void test_a_b_c(void)
{
	int prog_space[4];
	struct EMITTER emit;
	struct OPCODE opcode;

	LOG_MSG(LOG_EMIT_TEST, "\nemit test_a_b_c");

	emit_init(&emit, (int64_t)prog_space);
	emit_a_b_c(&emit, MUL, 6, 2, 3);

	opcode.instr = prog_space[0];
	oc_instr(&opcode);
	assert(opcode.oc == MUL + 6);
	assert(opcode.a == 2);
	assert(opcode.b == 3);
}

static void test_emit_sys(void)
{
	int prog_space[4];
	struct EMITTER emit;
	struct OPCODE opcode;

	LOG_MSG(LOG_EMIT_TEST, "\nemit test_emit_sys");

	emit_init(&emit, (int64_t)prog_space);
	emit_sys(&emit, 1, 2, 10);

	opcode.instr = prog_space[0];
	oc_instr(&opcode);
	assert(opcode.oc == SYS + 10);
	assert(opcode.a == 1);
	assert(opcode.b == 2);
}

int main(void)
{
	LOG_INIT();
	//LOG_ON_OFF(LOG_OC_OFFS, 1);
	//LOG_ON_OFF(LOG_OC_A_B_IMM, 1);
	//LOG_ON_OFF(LOG_OC_A_B_IMM_2, 1);
	//LOG_ON_OFF(LOG_OC_A_IMM, 1);
	//LOG_ON_OFF(LOG_OC_A_B, 1);
	//LOG_ON_OFF(LOG_OC_A_B_C, 1);
	//LOG_ON_OFF(LOG_EMIT_OFFS, 1);
	//LOG_ON_OFF(LOG_EMIT_A_B_IMM, 1);
	//LOG_ON_OFF(LOG_EMIT_A_B_C, 1);
	//LOG_ON_OFF(LOG_EMIT_SYS, 1);
	//LOG_ON_OFF(LOG_EMIT_TEST, 1);

	test_offs();
	test_a_b_imm();
	test_a_b_c();
	test_emit_sys();
	return 0;
}
