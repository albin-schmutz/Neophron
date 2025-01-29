#include <assert.h>
#include <stdint.h>
#include "logger.h"
#include "opcode.h"

static void test_offs(void)
{
	struct OPCODE opc;

	LOG_MSG(LOG_OC_TEST, "\noc test_offs");

	opc.oc = JUMP;
	opc.imm = 666;
	oc_offs(&opc);
	assert(opc.instr == 21314);

	OC_LOG_OC_OFFS(0, &opc, 0);

	opc.oc = BLS;
	opc.imm = -4;
	oc_offs(&opc);
	assert(opc.instr == -122);

	OC_LOG_OC_OFFS(0, &opc, 0);

	opc.instr = 21314;
	oc_instr(&opc);
	assert(opc.oc == JUMP);
	assert(opc.imm == 666);

	OC_LOG_OC_OFFS(0, &opc, 0);
	
	opc.instr = -122;
	oc_instr(&opc);
	assert(opc.oc == BLS);
	assert(opc.imm == -4);

	OC_LOG_OC_OFFS(0, &opc, 0);
}

static void test_a_b_imm(void)
{
	struct OPCODE opc;

	LOG_MSG(LOG_OC_TEST, "\noc test_a_b_imm");

	opc.oc = SUBI;
	opc.a = 1;
	opc.b = 2;
	opc.imm = -1000000000;
	oc_a_b_imm(&opc);
	assert(opc.instr == 50208);

	OC_LOG_OC_A_B_IMM(0, &opc, 0);

	opc.oc = PSH;
	opc.a = 2;
	opc.b = 3;
	opc.imm = 100;
	oc_a_b_imm(&opc);
	assert(opc.instr == 820814);

	OC_LOG_OC_A_B_IMM(0, &opc, 0);

	opc.oc = MOV;
	opc.a = 5;
	opc.b = 9;
	oc_a_b_imm(&opc);
	assert(opc.instr == 815776);

	OC_LOG_OC_A_B(0, &opc, 0);

	opc.instr = 50208;
	opc.imm = 0;
	oc_instr(&opc);
	assert(opc.oc == SUBI2);
	assert(opc.a == 1);
	assert(opc.b == 2);
	assert(opc.is64);

	OC_LOG_OC_A_B_IMM(0, &opc, 0);

	opc.instr = 820814;
	oc_instr(&opc);
	assert(opc.oc == PSH);
	assert(opc.a == 2);
	assert(opc.b == 3);
	assert(opc.imm == 100);

	OC_LOG_OC_A_B_IMM(0, &opc, 0);

	opc.instr = 815776;
	oc_instr(&opc);
	assert(opc.oc == MOV);
	assert(opc.a == 5);
	assert(opc.b == 9);

	OC_LOG_OC_A_B(0, &opc, 0);
}

static void test_a_b_c(void)
{
	struct OPCODE opc;

	LOG_MSG(LOG_OC_TEST, "\noc test_a_b_c");

	opc.oc = SUB;
	opc.a = 1;
	opc.b = 2;
	opc.imm = 3;
	oc_a_b_c(&opc);
	assert(opc.instr == 665152);

	OC_LOG_OC_A_B_C(0, &opc, 1, 0);

	opc.oc = 0;
	opc.a = 0;
	opc.b = 0;
	oc_instr(&opc);
	assert(opc.oc == SUB + 1);
	assert(opc.a == 2);
	assert(opc.b == 3);

	OC_LOG_OC_A_B_C(0, &opc, 1, 0);
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
	//LOG_ON_OFF(LOG_OC_TEST, 1);

	test_offs();
	test_a_b_imm();
	test_a_b_c();
	return 0;
}
