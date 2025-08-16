#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "opcode.h"
#include "emitter.h"
#include "system.h"
#include "cpu.h"

int main(void)
{
	struct BUFFER buf;
	struct EMITTER emit;
	int64_t mem_size = 256;
	char mem[256];
	int64_t mem_start = (int64_t)mem;
	int64_t info;

	strcpy(buf.data, "Hello world!\n");
	buf.pos = 0;
	buf.limit = 13;

	emit_init(&emit, mem_start);

	emit.opcode.oc = MOVI;
	emit.opcode.a = 0;
	emit.opcode.b = 0;
	emit.opcode.imm = (int64_t)&buf.val[0];
	emit_a_b_imm(&emit);

	emit.opcode.oc = MOVI;
	emit.opcode.a = 1;
	emit.opcode.b = 0;
	emit.opcode.imm = SYS_INFO_IO_STDOUT;
	emit_a_b_imm(&emit);

	emit.opcode.oc = MOVI;
	emit.opcode.a = 2;
	emit.opcode.b = 0;
	emit.opcode.imm = (int64_t)&info;
	emit_a_b_imm(&emit);

	emit.opcode.oc = STQ;
	emit.opcode.a = 1;
	emit.opcode.b = 2;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit);

	emit.opcode.oc = SYS + SYS_CALL_GET_INFO;
	emit.opcode.a = 0;
	emit.opcode.b = 2;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit); /* buf.val[0] = stdout */

	emit.opcode.oc = MOVI;
	emit.opcode.a = 3;
	emit.opcode.b = 2;
	emit.opcode.imm = (int64_t)&buf;
	emit_a_b_imm(&emit);

	emit.opcode.oc = SYS + SYS_CALL_FILE_WRITE;
	emit.opcode.a = 2;
	emit.opcode.b = 3;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit);

	emit.opcode.oc = POP;
	emit.opcode.a = PC;
	emit.opcode.b = SP;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit);

	reg[PC] = mem_start;
	reg[SP] = mem_start + mem_size;
	cpu_execute(0, 0);

	return 0;
}
