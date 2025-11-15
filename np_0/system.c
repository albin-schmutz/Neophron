#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "system.h"

#define SYSTEM_TOTAL_PAGES 4096
#define MEMORY_PAGE_SIZE 4096

LONGINT mem_start;
LONGINT mem_size;
LONGINT stack_start;
LONGINT heap_start;

/*

stack    prog     heap
-------- -------- ----------------
    <---          --->

*/

static char **args;
static INTEGER args_count;

void sys_init(int argc, char *argv[])
{
	assert(argc > 0);
	assert(argv);
	assert(argv[0]);

	mem_size = SYSTEM_TOTAL_PAGES * MEMORY_PAGE_SIZE;
	mem_start = (LONGINT)malloc(mem_size);
	if (!mem_start) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	LONGINT stack_size = mem_size / 4;
	stack_start = mem_start + stack_size;

	args = &argv[1];
	args_count = argc - 1;
}

static LONGINT get_info(INTEGER nr)
{
	switch (nr) {
	#ifdef WIN32
	case SYS_INFO_OS: return 1;
	#else
	case SYS_INFO_OS: return 2;
	#endif
	case SYS_INFO_MEM_START: return (LONGINT)mem_start;
	case SYS_INFO_MEM_SIZE: return mem_size;
	case SYS_INFO_STACK_START: return (LONGINT)stack_start;
	case SYS_INFO_HEAP_START: return (LONGINT)heap_start;
	case SYS_INFO_IO_STDIN: return (LONGINT)stdin;
	case SYS_INFO_IO_STDOUT: return (LONGINT)stdout;
	case SYS_INFO_IO_STDERR: return (LONGINT)stderr;
	case SYS_INFO_ARGC: return args_count;
	}
	return 0;
}

static LONGINT create_buffer_result(BUFFER buf, char *s)
{
	INTEGER len = strlen(s);
	if (len > buf->limit - buf->pos) len = buf->limit - buf->pos;
	strncpy((char*)buf->addr + buf->pos, s, len);
	buf->pos += len;
	return len;
}

static LONGINT get_arg(BUFFER buf)
{
	if (args_count > 0 && buf->i32 >= 0 && buf->i32 < args_count) {
		return create_buffer_result(buf, args[buf->i32]);
	}
	return 0;
}

static LONGINT get_env(BUFFER buf)
{
	char* env = getenv((char*)buf->addr + buf->pos);
	if (env) {
		return create_buffer_result(buf, env);
	}
	return 0;
}

static LONGINT open_file(char *filename, INTEGER mode)
{
	assert(filename);

	switch (mode) {
	case SYS_MODE_READ: return (LONGINT)fopen(filename, "rb");
	case SYS_MODE_WRITE: return (LONGINT)fopen(filename, "wb");
	}
	return 0;
}

static LONGINT close_file(LONGINT file)
{
	assert(file);

	fclose((FILE*)file);
	return 0;
}

static INTEGER read_file(BUFFER buf)
{
	assert(buf);

	INTEGER n = fread((char*)buf->addr + buf->pos, 1, buf->limit - buf->pos, (FILE*)buf->i64);
	buf->pos += n;

	return n;
}

static INTEGER write_file(BUFFER buf)
{
	assert(buf);

	INTEGER n = fwrite((char*)buf->addr + buf->pos, 1, buf->limit - buf->pos, (FILE*)buf->i64);
	buf->pos += n;

	return n;
}

LONGINT sys_call(INTEGER nr, LONGINT data)
{
	assert(nr >= SYS_CALL_HALT);
	assert(nr <= SYS_CALL_FILE_WRITE);
	assert(data);

	#define b ((BUFFER)data)
	#define l ((LONGINT*)data)

	switch (nr) {
	case SYS_CALL_HALT: exit(*l);
	case SYS_CALL_EXEC: return system((char*)b->addr + b->pos);
	case SYS_CALL_GET_INFO: return get_info(*l);
	case SYS_CALL_GET_ARG: return get_arg(b);
	case SYS_CALL_GET_ENV: return get_env(b);
	case SYS_CALL_FILE_OPEN: return open_file((char*)b->addr + b->pos, b->i32);
	case SYS_CALL_FILE_CLOSE: return close_file(*l);
	case SYS_CALL_FILE_READ: return read_file(b);
	case SYS_CALL_FILE_WRITE: return write_file(b);
	}
	return 0;
}
