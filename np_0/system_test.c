#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "system.h"

static void test_exec(void)
{
	char data[64];
	struct BUFFER buf;
	LONGINT i;

	buf.addr = (LONGINT)data;
	buf.size = 64;
	buf.pos = 6;
	buf.limit = buf.size;

	i = SYS_INFO_OS;
	i = sys_call(SYS_CALL_GET_INFO, (LONGINT)&i);

	if (i == 1) {
		strcpy(data + buf.pos, "system_test.exe gugus \"Hello !\"   -123");
	} else {
		strcpy(data + buf.pos, "./tmp_system_test gugus \"Hello !\"   -123");
	}
	assert(sys_call(SYS_CALL_EXEC, (LONGINT)&buf) == 0);
}

static void test_get_info(void)
{
	LONGINT mem_start;
	LONGINT mem_size;
	LONGINT stack_start;

	mem_start = SYS_INFO_MEM_START;
	mem_start = sys_call(SYS_CALL_GET_INFO, (LONGINT)&mem_start);
	mem_size = SYS_INFO_MEM_SIZE;
	mem_size = sys_call(SYS_CALL_GET_INFO, (LONGINT)&mem_size);
	stack_start = SYS_INFO_STACK_START;
	stack_start = sys_call(SYS_CALL_GET_INFO, (LONGINT)&stack_start);

	assert(stack_start - mem_size / 4 == mem_start);
}

static void test_get_arg(void)
{
	char data[32] = "xyz";
	struct BUFFER buf;
	char *args[] = { "gugus", "Hello !", "-123" };
	INTEGER lens[] = { 5, 7, 4 };
	LONGINT i;
	INTEGER argc;
	INTEGER arg;

	buf.addr = (LONGINT)data;
	buf.size = 32;

	i = SYS_INFO_ARGC;
	argc = sys_call(SYS_CALL_GET_INFO, (LONGINT)&i);
	assert(argc == 3);
	arg = 0;
	while (arg < argc) {
		buf.pos = 3;
		buf.limit = buf.size;
		buf.i32 = arg;
		i = sys_call(SYS_CALL_GET_ARG, (LONGINT)&buf);
		assert(i == lens[arg]);
		assert(buf.pos == i + 3);
		data[buf.pos] = 0;
		assert(buf.limit == buf.size);
		assert(!strcmp(args[arg], data + 3));
		arg++;
	}
}

static void test_get_env(void)
{
	char data[32];
	struct BUFFER buf;
	LONGINT i;

	buf.addr = (LONGINT)data;
	buf.size = 32;
	buf.pos = 0;
	buf.limit = buf.size;

	i = SYS_INFO_OS;
	i = sys_call(SYS_CALL_GET_INFO, (LONGINT)&i);

	if (i == 1) {
		strcpy(data, "windir");
		i = sys_call(SYS_CALL_GET_ENV, (LONGINT)&buf);
		assert(i == 10);
		assert(buf.pos == i);
		assert(buf.limit == buf.size);
		data[buf.pos] = 0;
		assert(!strcmp("C:\\WINDOWS", data));
	} else {
		strcpy(data, "HOME");
		i = sys_call(SYS_CALL_GET_ENV, (LONGINT)&buf);
		assert(i > 0);
		assert(buf.pos > 0);
		assert(buf.limit == buf.size);
		data[5] = 0;
		assert(!strcmp("/home", data));
	}

	buf.pos = 0;
	strcpy(data, "_gugus_");
	assert(sys_call(SYS_CALL_GET_ENV, (LONGINT)&buf) == 0);
}

static void test_file(void)
{
	char data[232];
	struct BUFFER buf;
	LONGINT n;
	INTEGER i;
	char *tmp_file = "tmp";

	buf.addr = (LONGINT)data;
	buf.size = 232;

	strcpy(data, tmp_file);
	buf.i32 = SYS_MODE_WRITE;
	buf.i64 = sys_call(SYS_CALL_FILE_OPEN, (LONGINT)&buf);
	assert(buf.i64);
	i = 0;
	while (i++ < 23) {
		strcpy(data, "Hallo Welt!\n");
		buf.pos = 0;
		buf.limit = 12;
		n = sys_call(SYS_CALL_FILE_WRITE, (LONGINT)&buf);
		assert(n == 12);
		assert(buf.pos == n);
	}
	n = buf.i64;
	n = sys_call(SYS_CALL_FILE_CLOSE, (LONGINT)&n);

	buf.pos = 0;
	buf.limit = buf.size;
	strcpy(data, tmp_file);
	buf.i32 = SYS_MODE_READ;
	buf.i64 = sys_call(SYS_CALL_FILE_OPEN, (LONGINT)&buf);
	assert(buf.i64);

	buf.pos = 0;
	buf.limit = buf.size;
	n = sys_call(SYS_CALL_FILE_READ, (LONGINT)&buf);
	assert(n == buf.size);
	assert(buf.pos == n);
	assert(buf.limit == n);

	buf.pos = 0;
	buf.limit = buf.size;
	n = sys_call(SYS_CALL_FILE_READ, (LONGINT)&buf);
	assert(n == 23 * 12 - buf.size);
	assert(buf.pos == n);
	assert(buf.limit == buf.size);

	n = buf.i64;
	n = sys_call(SYS_CALL_FILE_CLOSE, (LONGINT)&n);
}

int main(int argc, char *argv[])
{
	assert(sizeof(struct BUFFER) == 32);
	if (argc == 1) {
		sys_init(argc, argv);
		test_exec();
		return 0;
	}
	sys_init(argc, argv);
	test_get_info();
	test_get_arg();
	test_get_env();
	test_file();
	return 0;
}
