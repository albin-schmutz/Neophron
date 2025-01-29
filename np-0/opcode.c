#include <assert.h>
#include <stdint.h>
#include "logger.h"
#include "opcode.h"

#ifdef LOGGER

static char *rt[] = {
	"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
	"R8", "R9", "R10", "R11", "FP", "SP", "BR", "PC"
};

static char *ot[] = {
	/*   0 */ "", "", "JUMP", "CALL", "BEQ", "BNE", "BLS", "BGE",
	/*   8 */ "BLE", "BGT", "", "", "", "POP", "PSH", "",
	/*  16 */ "MOVI", "", "MULI", "DIVI", "MODI", "ADDI", "SUBI", "",
	/*  24 */ "LDB", "LDW", "LDD", "LDQ", "STB", "STW", "STD", "STQ",
	/*  32 */ "MOVI2", "", "MULI2", "DIVI2", "MODI2", "ADDI2", "SUBI2", "",
	/*  40 */ "LDB2", "LDW2", "LDD2", "LDQ2", "STB2", "STW2", "STD2", "STQ2",
	/*  48 */ "MUL0", "MUL1", "MUL2", "MUL3", "MUL4", "MUL5", "MUL6", "MUL7",
	/*  56 */ "MUL8", "MUL9", "MUL10", "MUL11", "MUL12", "MUL13", "MUL14", "MUL15",
	/*  64 */ "DIV0", "DIV1", "DIV2", "DIV3", "DIV4", "DIV5", "DIV6", "DIV7",
	/*  72 */ "DIV8", "DIV9", "DIV10", "DIV11", "DIV12", "DIV13", "DIV14", "DIV15",
	/*  80 */ "MOD0", "MOD1", "MOD2", "MOD3", "MOD4", "MOD5", "MOD6", "MOD7",
	/*  88 */ "MOD8", "MOD9", "MOD10", "MOD11", "MOD12", "MOD13", "MOD14", "MOD15",
	/*  96 */ "ADD0", "ADD1", "ADD2", "ADD3", "ADD4", "ADD5", "ADD6", "ADD7",
	/* 104 */ "ADD8", "ADD9", "ADD10", "ADD11", "ADD12", "ADD13", "ADD14", "ADD15",
	/* 112 */ "SUB0", "SUB1", "SUB2", "SUB3", "SUB4", "SUB5", "SUB6", "SUB7",
	/* 120 */ "SUB8", "SUB9", "SUB10", "SUB11", "SUB12", "SUB13", "SUB14", "SUB15",
	/* 128 */ "ASH", "BITA", "BITO", "MOV", "NEG", "NOT", "SYS0", "SYS1",
	/* 136 */ "SYS2", "SYS3", "SYS4", "SYS5", "SYS6", "SYS7", "SYS8", "SYS9",
	/* 144 */ "SYS10", "SYS11", "SYS12", "SYS13", "SYS14", "SYS15"
};

void oc_log_oc_offs(int64_t pc, OPCODE opc, int64_t reg)
{
	LOG_MSG(LOG_OC_OFFS, "%016llx %s %i (BR = %016llx)",
		pc, ot[opc->oc], (int)opc->imm, reg);
}

void oc_log_oc_a_b_imm(int64_t pc, OPCODE opc, int64_t reg)
{
	LOG_MSG(LOG_OC_A_B_IMM, "%016llx %s %s %s %lli (%016llx, %lli)",
		pc, ot[opc->oc], rt[opc->a], rt[opc->b], opc->imm, reg, reg);
}

void oc_log_oc_a_b_imm_2(int64_t pc, OPCODE opc, int64_t addr, int64_t reg)
{
	LOG_MSG(LOG_OC_A_B_IMM_2, "%016llx %s %s %s %lli [%016llx] (%016llx, %lli)",
		pc, ot[opc->oc], rt[opc->a], rt[opc->b], opc->imm, addr, reg, reg);
}

void oc_log_oc_a_imm(int64_t pc, OPCODE opc, int64_t reg)
{
	LOG_MSG(LOG_OC_A_IMM, "%016llx %s %s %lli (%016llx, %lli)",
		pc, ot[opc->oc], rt[opc->a], opc->imm, reg, reg);
}

void oc_log_oc_a_b(int64_t pc, OPCODE opc, int64_t reg)
{
	LOG_MSG(LOG_OC_A_B, "%016llx %s %s %s (%016llx, %lli)",
		pc, ot[opc->oc], rt[opc->a], rt[opc->b], reg, reg);
}

void oc_log_oc_a_b_c(int64_t pc, OPCODE opc, int a, int64_t reg)
{
	LOG_MSG(LOG_OC_A_B_C, "%016llx %s %s %s %s (%016llx, %lli)",
		pc, ot[opc->oc], rt[a], rt[opc->a], rt[opc->b], reg, reg);
}

#endif

void oc_offs(OPCODE opc)
{
	assert(opc);
	assert(opc->oc > 0);
	assert(opc->oc <= BGT);
	assert(opc->imm >= -67108864);
	assert(opc->imm < 67108864);

	opc->instr = opc->oc + (opc->imm << BIT_OC);
}

void oc_a_b_imm(OPCODE opc)
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

void oc_a_b_c(OPCODE opc)
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
	oc_a_b_imm(opc);
}

void oc_instr(OPCODE opc)
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
