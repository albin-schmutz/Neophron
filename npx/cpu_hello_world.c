#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "opcode.h"
#include "emitter.h"
#include "system.h"
#include "cpu.h"

#define BUFFER_SIZE 104

int main(void)
{
	struct BUFFER buf;
	struct EMITTER emit;
	int64_t mem_size = 128;
	char mem[128];
	int64_t mem_start = (int64_t)mem;
	int64_t info;

	strcpy(buf.data, "Hello world!\n");
	buf.pos = 0;
	buf.limit = 13;

	emit_init(&emit, mem_start);

	emit_a_b_imm(&emit, MOVI, 0, 0, (int64_t)&buf.val[0]);
	emit_a_b_imm(&emit, MOVI, 1, 0, 3); /* SYS_INFO_IO_STDOUT */
	emit_a_b_imm(&emit, MOVI, 2, 0, (int64_t)&info);
	emit_a_b_imm(&emit, STQ, 1, 2, 0);
	emit_sys(&emit, 0, 2, 2); /* buf.val[0] = stdout */
	emit_a_b_imm(&emit, MOVI, 3, 0, (int64_t)&buf);
	emit_sys(&emit, 2, 3, 8);
	emit_a_b_imm(&emit, POP, PC, SP, 0);

	reg[PC] = mem_start;
	reg[SP] = mem_start + mem_size;
	cpu_execute(0, 0);

	return 0;
}
