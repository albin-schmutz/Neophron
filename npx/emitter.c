#include <assert.h>
#include <stdint.h>
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

	emit->opcode.oc = oc;
	emit->opcode.imm = imm;
	opc_offs(&emit->opcode);

	emit->base[emit->pc++] = emit->opcode.instr;
}

void emit_a_b_imm(EMITTER emit, int oc, int a, int b, int64_t imm)
{
	assert(emit);

	emit->opcode.oc = oc;
	emit->opcode.a = a;
	emit->opcode.b = b;
	emit->opcode.imm = imm;
	opc_a_b_imm(&emit->opcode);

	emit->base[emit->pc++] = emit->opcode.instr;
	if (emit->opcode.is64) {
		*((int64_t*)(&emit->base[emit->pc])) = emit->opcode.imm;
		emit->pc += 2;
	}
}

void emit_a_b_c(EMITTER emit, int oc, int a, int b, int c)
{
	assert(emit);

	emit->opcode.oc = oc;
	emit->opcode.a = a;
	emit->opcode.b = b;
	emit->opcode.imm = c;
	opc_a_b_c(&emit->opcode);

	emit->base[emit->pc++] = emit->opcode.instr;
}

void emit_sys(EMITTER emit, int a, int b, int nr)
{
	assert(emit);
	assert(a >= 0);
	assert(a <= BIT_REG_MASK);
	assert(b >= 0);
	assert(b <= BIT_REG_MASK);

	emit_a_b_imm(emit, SYS + nr, a, b, 0);
}
