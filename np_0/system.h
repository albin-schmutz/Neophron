/*

Implements SYS opcode calls.
- Allocates runtime memory.
- Provides system infos.
- Can execute OS commands.
- Provides command line arguments.
- Provides environment variables.
- Provides file operations.

*/

#define SYS_INFO_OS 1
#define SYS_INFO_MEM_START 2
#define SYS_INFO_MEM_SIZE 3
#define SYS_INFO_STACK_START 4
#define SYS_INFO_HEAP_START 5
#define SYS_INFO_IO_STDIN 6
#define SYS_INFO_IO_STDOUT 7
#define SYS_INFO_IO_STDERR 8
#define SYS_INFO_ARGC 9

#define SYS_MODE_READ 1
#define SYS_MODE_WRITE 2

#define SYS_CALL_HALT 1
#define SYS_CALL_EXEC 2
#define SYS_CALL_GET_INFO 3
#define SYS_CALL_GET_ARG 4
#define SYS_CALL_GET_ENV 5
#define SYS_CALL_FILE_OPEN 6
#define SYS_CALL_FILE_CLOSE 7
#define SYS_CALL_FILE_READ 8
#define SYS_CALL_FILE_WRITE 9

typedef struct BUFFER *BUFFER;

struct BUFFER {
	LONGINT addr;
	LONGINT i64;
	INTEGER i32;
	INTEGER size;
	INTEGER pos;
	INTEGER limit;
};

extern LONGINT mem_start;
extern LONGINT mem_size;
extern LONGINT stack_start;
extern LONGINT heap_start;

extern void sys_init(int argc, char *argv[]);
extern LONGINT sys_call(INTEGER nr, LONGINT data);
