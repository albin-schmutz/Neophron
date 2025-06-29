#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "system.h"

static void test_buffer_size(void)
{
	assert(sizeof(struct BUFFER) == 256);
}

static void test_exec(void)
{
	struct BUFFER buf;
	int64_t i;

	i = SYS_INFO_OS;
	sys_call(SYS_CALL_GET_INFO, &i, &i);

	if (i == 1) {
		strcpy(buf.data, "system_test.exe gugus \"Hello !\"   -123");
	} else {
		strcpy(buf.data, "./tmp_system_test gugus \"Hello !\"   -123");
	}
	sys_call(SYS_CALL_EXEC, &i, &buf);
	assert(i == 0);
}

static void test_get_arg(void)
{
	struct BUFFER buf;
	char *args[] = { "gugus", "Hello !", "-123" };
	int lens[] = { 5, 7, 4 };
	int64_t i;
	int argc;
	int arg;

	i = SYS_INFO_ARGC;
	sys_call(SYS_CALL_GET_INFO, &i, &i);
	argc = i;
	assert(argc == 3);
	arg = 0;
	while (arg < argc) {
		buf.val[0] = arg;
		sys_call(SYS_CALL_GET_ARG, &i, &buf);
		assert(i == lens[arg]);
		buf.data[i] = 0;
		assert(buf.pos == i);
		assert(buf.limit == BUFFER_SIZE);
		buf.data[buf.pos] = 0;
		assert(!strcmp(args[arg], buf.data));
		arg++;
	}
}

static void test_get_env(void)
{
	struct BUFFER buf;
	int64_t i;

	i = SYS_INFO_OS;
	sys_call(SYS_CALL_GET_INFO, &i, &i);

	if (i == 1) {
		strcpy(buf.data, "windir");
		sys_call(SYS_CALL_GET_ENV, &i, &buf);
		assert(i == 10);
		assert(buf.pos == i);
		assert(buf.limit == BUFFER_SIZE);
		buf.data[buf.pos] = 0;
		assert(!strcmp("C:\\WINDOWS", buf.data));
	} else {
		strcpy(buf.data, "HOME");
		sys_call(SYS_CALL_GET_ENV, &i, &buf);
		assert(i > 0);
		assert(buf.pos > 0);
		assert(buf.limit == BUFFER_SIZE);
		buf.data[5] = 0;
		assert(!strcmp("/home", buf.data));
	}

	strcpy(buf.data, "_gugus_");
	sys_call(SYS_CALL_GET_ENV, &i, &buf);
	assert(i == 0);
}

static void test_file(void)
{
	struct BUFFER buf;
	int64_t n;
	int i;
	char *tmp_file = "tmp";

	strcpy(buf.data, tmp_file);
	buf.val[1] = SYS_MODE_WRITE;
	sys_call(SYS_CALL_FILE_OPEN, &buf.val[0], &buf);
	assert(buf.val[0]);
	i = 0;
	while (i++ < 23) {
		strcpy(buf.data, "Hallo Welt!\n");
		buf.pos = 0;
		buf.limit = 12;
		sys_call(SYS_CALL_FILE_WRITE, &n, &buf);
		assert(n == 12);
		assert(buf.pos == n);
	}
	n = buf.val[0];
	sys_call(SYS_CALL_FILE_CLOSE, &n, &n);

	strcpy(buf.data, tmp_file);
	buf.val[1] = SYS_MODE_READ;
	sys_call(SYS_CALL_FILE_OPEN, &buf.val[0], &buf);
	assert(buf.val[0]);

	buf.pos = 0;
	buf.limit = BUFFER_SIZE;
	sys_call(SYS_CALL_FILE_READ, &n, &buf);
	assert(n == BUFFER_SIZE);
	assert(buf.pos == n);
	assert(buf.limit == n);

	buf.pos = 0;
	buf.limit = BUFFER_SIZE;
	sys_call(SYS_CALL_FILE_READ, &n, &buf);
	assert(n == 23 * 12 - BUFFER_SIZE);
	assert(buf.pos == n);
	assert(buf.limit == BUFFER_SIZE);

	n = buf.val[0];
	sys_call(SYS_CALL_FILE_CLOSE, &n, &n);
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		sys_init(argc, argv);
		test_exec();
		return 0;
	}
	sys_init(argc, argv);
	test_buffer_size();
	test_get_arg();
	test_get_env();
	test_file();
	return 0;
}
