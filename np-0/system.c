#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "system.h"

static char **args;
static int args_count;

void sys_init(int argc, char *argv[])
{
	assert(argc > 0);
	assert(argv);
	assert(argv[0]);

	LOG_MSG(LOG_SYS_INIT, "sys_init argc = %i", argc);

	args = &argv[1];
	args_count = argc - 1;
}

static void get_info(int64_t *result, int nr)
{
	LOG_MSG(LOG_SYS_GET_INFO, "sys get_info nr = %i", nr);

	switch (nr) {
	case SYS_INFO_OS:
		#ifdef WIN32
		*result = 1;
		#else
		*result = 2;
		#endif
		break;
	case SYS_INFO_IO_STDIN:
		*result = (int64_t)stdin;
		break;
	case SYS_INFO_IO_STDOUT:
		*result = (int64_t)stdout;
		break;
	case SYS_INFO_IO_STDERR:
		*result = (int64_t)stderr;
		break;
	case SYS_INFO_ARGC:
		*result = args_count;
		break;
	}

	LOG_MSG(LOG_SYS_GET_INFO, "sys get_info result = %016llx", result);
}

static void create_buffer_result(int64_t *result, BUFFER buf, char *s)
{
	buf->pos = strlen(s);
	if (buf->pos >= BUFFER_SIZE) buf->pos = BUFFER_SIZE;
	buf->limit = BUFFER_SIZE;
	*result = buf->pos;
	strncpy(buf->data, s, buf->pos);

	LOG_MSG(LOG_SYS_BUF_RESULT, "sys create_buffer_result s = %s", s);
	LOG_DUMP(LOG_SYS_BUF_RESULT, buf->data, BUFFER_SIZE);
}

static void get_arg(int64_t *result, BUFFER buf)
{
	LOG_MSG(LOG_SYS_GET_ARG, "sys get_arg buf->val[0] = %lli", buf->val[0]);

	if (args_count > 0) {
		create_buffer_result(result, buf, args[buf->val[0]]);
	} else {
		*result = 0;
	}

	LOG_MSG(LOG_SYS_GET_ARG, "sys get_arg result result = %lli", *result);
}

static void get_env(int64_t *result, BUFFER buf)
{
	LOG_MSG(LOG_SYS_GET_ENV, "sys get_env buf->data = %s", buf->data);

	char* env = getenv(buf->data);
	if (env) {
		create_buffer_result(result, buf, env);
	} else {
		*result = 0;
	}

	LOG_MSG(LOG_SYS_GET_ENV, "sys get_env result = %lli", *result);
}

static int64_t open_file(char *filename, int mode)
{
	assert(filename);

	switch (mode) {
	case SYS_MODE_READ:
		return (int64_t)fopen(filename, "rb");
	case SYS_MODE_WRITE:
		return (int64_t)fopen(filename, "wb");
	}
	return 0;
}

static void close_file(int64_t file)
{
	fclose((FILE*)file);
}

static int read_file(BUFFER buf)
{
	assert(buf);

	int n = fread(&buf->data[buf->pos], 1, buf->limit - buf->pos, (FILE*)buf->val[0]);
	buf->pos += n;

	LOG_MSG(LOG_SYS_READ_FILE, "sys read_file n = %lli", n);
	LOG_DUMP(LOG_SYS_READ_FILE, buf->data, BUFFER_SIZE);

	return n;
}

static int write_file(BUFFER buf)
{
	assert(buf);

	int n = fwrite(&buf->data[buf->pos], 1, buf->limit - buf->pos, (FILE*)buf->val[0]);
	buf->pos += n;

	LOG_MSG(LOG_SYS_WRITE_FILE, "sys write_file n = %lli", n);
	LOG_DUMP(LOG_SYS_WRITE_FILE, buf->data, BUFFER_SIZE);

	return n;
}

void sys_call(int nr, int64_t *result, void *data)
{
	assert(result);

	#define b ((BUFFER)data)
	#define l ((long long*)data)
	#define i (int)*l

	switch (nr) {
	case SYS_CALL_HALT:
		exit(i);
		break;
	case SYS_CALL_EXEC:
		*result = system(b->data);
		break;
	case SYS_CALL_GET_INFO:
		get_info(result, i);
		break;
	case SYS_CALL_GET_ARG:
		get_arg(result, b);
		break;
	case SYS_CALL_GET_ENV:
		get_env(result, b);
		break;
	case SYS_CALL_FILE_OPEN:
		*result = open_file(b->data, b->val[1]);
		break;
	case SYS_CALL_FILE_CLOSE:
		close_file(*l);
		break;
	case SYS_CALL_FILE_READ:
		*result = read_file(b);
		break;
	case SYS_CALL_FILE_WRITE:
		*result = write_file(b);
		break;
	}
}
