#include <assert.h>
#include <stdint.h>
#include "logger.h"
#include "opcode.h"
#include "emitter.h"

void emit_init(EMITTER emit, int64_t start)
{
	assert(emit);
	assert(start);

	emit->base = (int*)start;
	emit->pc = 0;
}

void emit_offs(EMITTER emit, int oc, int imm)
{
	assert(emit);

	LOG_MSG(LOG_EMIT_OFFS, "emit_offs %04i %i %i", emit->pc, oc, imm);

	emit->opcode.oc = oc;
	emit->opcode.imm = imm;
	oc_offs(&emit->opcode);

	OC_LOG_OC_OFFS((int64_t)&emit->base[emit->pc], &emit->opcode, 0);

	emit->base[emit->pc++] = emit->opcode.instr;
}

void emit_a_b_imm(EMITTER emit, int oc, int a, int b, int64_t imm)
{
	assert(emit);

	LOG_MSG(LOG_EMIT_A_B_IMM, "emit_a_b_imm %04i %i %i %i %lli", emit->pc, oc, a, b, imm);

	emit->opcode.oc = oc;
	emit->opcode.a = a;
	emit->opcode.b = b;
	emit->opcode.imm = imm;
	oc_a_b_imm(&emit->opcode);

	if (emit->opcode.oc >= MOV) {
		OC_LOG_OC_A_B((int64_t)&emit->base[emit->pc], &emit->opcode, 0);
	} else {
		OC_LOG_OC_A_B_IMM((int64_t)&emit->base[emit->pc], &emit->opcode, 0);
	}

	emit->base[emit->pc++] = emit->opcode.instr;
	if (emit->opcode.is64) {
		*((int64_t*)(&emit->base[emit->pc])) = emit->opcode.imm;
		emit->pc += 2;
	}
}

void emit_a_b_c(EMITTER emit, int oc, int a, int b, int c)
{
	assert(emit);

	LOG_MSG(LOG_EMIT_A_B_C, "emit_a_b_c %04i %i %i %i %i", emit->pc, oc, a, b, c);

	emit->opcode.oc = oc;
	emit->opcode.a = a;
	emit->opcode.b = b;
	emit->opcode.imm = c;
	oc_a_b_c(&emit->opcode);

	OC_LOG_OC_A_B_C((int64_t)&emit->base[emit->pc], &emit->opcode, a, 0);

	emit->base[emit->pc++] = emit->opcode.instr;
}

void emit_sys(EMITTER emit, int a, int b, int nr)
{
	assert(emit);
	assert(a >= 0);
	assert(a <= BIT_REG_MASK);
	assert(b >= 0);
	assert(b <= BIT_REG_MASK);

	LOG_MSG(LOG_EMIT_SYS, "emit_sys %04i %i %i %i", emit->pc, a, b, nr);

	emit_a_b_imm(emit, SYS + nr, a, b, 0);
}
