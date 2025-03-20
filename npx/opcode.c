#include <assert.h>
#include <stdint.h>
#include "opcode.h"

void opc_offs(OPCODE opc)
{
	assert(opc);
	assert(opc->oc >= JUMP);
	assert(opc->oc <= BGT);
	assert(opc->imm >= -67108864);
	assert(opc->imm < 67108864);

	opc->instr = opc->oc + (opc->imm << BIT_OC);
}

void opc_a_b_imm(OPCODE opc)
{
	assert(opc);
	assert(opc->oc >= POP);
	assert(opc->a >= 0);
	assert(opc->a <= BIT_REG_MASK);
	assert(opc->b >= 0);
	assert(opc->b <= BIT_REG_MASK);

	if (opc->oc <= STQ) {
		if (opc->imm >= -262144 && opc->imm < 262144) {
			opc->instr = opc->oc +
				(opc->a << BIT_OC) +
				(opc->b << (BIT_OC + BIT_REG)) +
				(opc->imm << (BIT_OC + BIT_REG + BIT_REG));
			opc->is64 = 0;
		} else {
			opc->instr = (opc->a << BIT_OC) +
				(opc->b << (BIT_OC + BIT_REG)) +
				((opc->oc - 16) << (BIT_OC + BIT_REG + BIT_REG));
			opc->is64 = 1;
		}
	} else {
		opc->instr = (opc->a << BIT_OC) +
			(opc->b << (BIT_OC + BIT_REG)) +
			((opc->oc - 32) << (BIT_OC + BIT_REG + BIT_REG));
		opc->is64 = opc->oc <= STQ2;
	}
}

void opc_a_b_c(OPCODE opc)
{
	assert(opc);
	assert(opc->oc >= MUL);
	assert(opc->oc <= SYS);
	assert(opc->a >= 0);
	assert(opc->a <= BIT_REG_MASK);
	assert(opc->b >= 0);
	assert(opc->b <= BIT_REG_MASK);
	assert(opc->imm >= 0);
	assert(opc->imm <= BIT_REG_MASK);

	opc->oc += opc->a;
	opc->a = opc->b;
	opc->b = opc->imm;
	opc->imm = 0;
	opc_a_b_imm(opc);
}

void opc_instr(OPCODE opc)
{
	assert(opc);

	opc->oc = opc->instr & BIT_OC_MASK;
	if (opc->oc == 0) {
		opc->oc = 32 + (opc->instr >> (BIT_OC + BIT_REG + BIT_REG));
		opc->is64 = opc->oc >= MOVI2 && opc->oc <= STQ2;
		opc->a = (opc->instr >> BIT_OC) & BIT_REG_MASK;
		opc->b = (opc->instr >> (BIT_OC + BIT_REG)) & BIT_REG_MASK;
	} else if (opc->oc <= BGT) {
		opc->is64 = 0;
		opc->imm = opc->instr >> BIT_OC;
	} else {
		opc->is64 = 0;
		opc->a = (opc->instr >> BIT_OC) & BIT_REG_MASK;
		opc->b = (opc->instr >> (BIT_OC + BIT_REG)) & BIT_REG_MASK;
		opc->imm = opc->instr >> (BIT_OC + BIT_REG + BIT_REG);
	}
}
