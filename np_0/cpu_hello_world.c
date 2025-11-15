#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "opcode.h"
#include "emitter.h"
#include "system.h"
#include "cpu.h"

int main(void)
{
	char data[32];  
	struct BUFFER buf;
	struct EMITTER emit;
	INTEGER mem_size = 256;
	char mem[256];
	LONGINT info;

	buf.addr = (LONGINT)data;
	buf.size = 32;
	strcpy(data, "Hello world!\n");
	buf.pos = 0;
	buf.limit = 13;

	reg[PC] = (LONGINT)mem + mem_size / 2;
	reg[SP] = reg[PC];

	emit_init(&emit, reg[PC]);

	emit.opcode.oc = MOVI; /* R0 := &buf.val[0] */
	emit.opcode.a = 0;
	emit.opcode.imm = (LONGINT)&buf.i64;
	emit_a_b_imm(&emit);

	emit.opcode.oc = MOVI; /* R1 := SYS_INFO_IO_STDOUT */
	emit.opcode.a = 1;
	emit.opcode.imm = SYS_INFO_IO_STDOUT;
	emit_a_b_imm(&emit);

	emit.opcode.oc = MOVI; /* R2 := &info */
	emit.opcode.a = 2;
	emit.opcode.imm = (LONGINT)&info;
	emit_a_b_imm(&emit);

	emit.opcode.oc = STQ; /* info := SYS_INFO_IO_STDOUT */
	emit.opcode.a = 1;
	emit.opcode.b = 2;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit);

	emit.opcode.oc = SYS + SYS_CALL_GET_INFO;
	emit.opcode.a = 0;
	emit.opcode.b = 2;
	emit.opcode.imm = 0;
	emit_a_b_imm(&emit); /* buf.im64 = stdout */

	emit.opcode.oc = MOVI; /* R3 := &buf */
	emit.opcode.a = 3;
	emit.opcode.imm = (LONGINT)&buf;
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

	cpu_execute(0, 0);

	return 0;
}
