#include <assert.h>
#include <stdint.h>
#include "opcode.h"

static void test_offs(void)
{
	struct OPCODE opc;

	opc.oc = JUMP;
	opc.imm = 666;
	opc_offs(&opc);
	assert(opc.instr == 21314);

	opc.oc = BLS;
	opc.imm = -4;
	opc_offs(&opc);
	assert(opc.instr == -122);

	opc.instr = 21314;
	opc_instr(&opc);
	assert(opc.oc == JUMP);
	assert(opc.imm == 666);
	
	opc.instr = -122;
	opc_instr(&opc);
	assert(opc.oc == BLS);
	assert(opc.imm == -4);
}

static void test_a_b_imm(void)
{
	struct OPCODE opc;

	opc.oc = SUBI;
	opc.a = 1;
	opc.b = 2;
	opc.imm = -1000000000;
	opc_a_b_imm(&opc);
	assert(opc.instr == 50208);

	opc.oc = PSH;
	opc.a = 2;
	opc.b = 3;
	opc.imm = 100;
	opc_a_b_imm(&opc);
	assert(opc.instr == 820814);

	opc.oc = MOV;
	opc.a = 5;
	opc.b = 9;
	opc_a_b_imm(&opc);
	assert(opc.instr == 815776);

	opc.instr = 50208;
	opc.imm = 0;
	opc_instr(&opc);
	assert(opc.oc == SUBI2);
	assert(opc.a == 1);
	assert(opc.b == 2);
	assert(opc.is64);

	opc.instr = 820814;
	opc_instr(&opc);
	assert(opc.oc == PSH);
	assert(opc.a == 2);
	assert(opc.b == 3);
	assert(opc.imm == 100);

	opc.instr = 815776;
	opc_instr(&opc);
	assert(opc.oc == MOV);
	assert(opc.a == 5);
	assert(opc.b == 9);
}

static void test_a_b_c(void)
{
	struct OPCODE opc;

	opc.oc = SUB;
	opc.a = 1;
	opc.b = 2;
	opc.imm = 3;
	opc_a_b_c(&opc);
	assert(opc.instr == 665152);

	opc.oc = 0;
	opc.a = 0;
	opc.b = 0;
	opc_instr(&opc);
	assert(opc.oc == SUB + 1);
	assert(opc.a == 2);
	assert(opc.b == 3);
}

int main(void)
{
	test_offs();
	test_a_b_imm();
	test_a_b_c();
	return 0;
}
