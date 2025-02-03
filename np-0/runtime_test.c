#include <assert.h>
#include <stdint.h>
#include "logger.h"
#include "system.h"
#include "opcode.h"
#include "emitter.h"
#include "cpu.h"
#include "runtime.h"

static void test_align(void)
{
	int addr;

	LOG_MSG(LOG_RUN_TEST, "\nrun test_align");

	addr = 99; run_align(&addr, 4); assert(addr == 100);
	addr = -99; run_align(&addr, 4); assert(addr == -104);
	addr = 100; run_align(&addr, 4); assert(addr == 100);
	addr = -100; run_align(&addr, 4); assert(addr == -104);
	addr = 101; run_align(&addr, 4); assert(addr == 104);
	addr = -101; run_align(&addr, 4); assert(addr == -108);

	addr = -2; run_align(&addr, 2); assert(addr == -4);
	addr = -1; run_align(&addr, 2); assert(addr == -4);
	addr = 0; run_align(&addr, 1); assert(addr == -1);
	addr = 0; run_align(&addr, 2); assert(addr == -2);
	addr = 1; run_align(&addr, 2); assert(addr == 2);
	addr = 2; run_align(&addr, 2); assert(addr == 2);
}

static void test_create_modules(void)
{
	int64_t prog_addr;
	struct EMITTER emit;
	int header_size = sizeof(struct HEADER);

	LOG_MSG(LOG_RUN_TEST, "\nrun test_create_modules");

	run_create_module(&prog_addr, "tmpM1", -16);
	emit_init(&emit, prog_addr);
	emit_a_b_imm(&emit, MOVI, 0, 0, 42);
	emit_a_b_imm(&emit, MOV, PC, BR, 0);
	run_exec_module(8, -1);

	run_create_module(&prog_addr, "tmpM2", 0);
	emit_init(&emit, prog_addr);
	emit_offs(&emit, CALL, header_size / -INSTR_SIZE - 3);
	emit_a_b_imm(&emit, POP, PC, SP, 0);
	run_exec_module(8, 0);
	run_write_file();

	assert(reg[0] == 42);
}

static void test_load_modules(void)
{
	LOG_MSG(LOG_RUN_TEST, "\nrun test_load_modules");

	reg[0] = 0;
	assert(run_load_file("tmpM2"));
	assert(reg[0] == 42);
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
	//LOG_ON_OFF(LOG_CPU_EXECUTE, 1);
	//LOG_ON_OFF(LOG_CPU_TEST, 1);
	//LOG_ON_OFF(LOG_RUN_CREATE_MODULE, 1);
	//LOG_ON_OFF(LOG_RUN_EXEC_MODULE, 1);
	//LOG_ON_OFF(LOG_RUN_TEST, 1);

	run_init();
	test_align();
	test_create_modules();
	run_init();
	test_load_modules();

	return 0;
}
