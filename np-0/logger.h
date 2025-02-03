/*
	Provides an fine-grained logging system.
	- Can log formatted messages.
	- Can log byte arrays as hexadecimal dumps.
	- Output goes to stderr.
	- Allows to switch on/off single or group of log statements.
*/

#ifdef LOGGER

#define LOG_INIT() log_init()
#define LOG_ON_OFF(id, on_off) log_on_off(id, on_off)
#define LOG_MSG(id, format, ...) log_msg(id, format, ##__VA_ARGS__)
#define LOG_DUMP(id, bytes, bytes_n) log_dump(id, bytes, bytes_n)

#else

#define LOG_INIT()
#define LOG_ON_OFF(id, on_off)
#define LOG_MSG(id, format, ...)
#define LOG_DUMP(id, bytes, bytes_n)

#endif

typedef enum {
	LOG_TEST_0, LOG_TEST_1,

	LOG_SYS_INIT, LOG_SYS_GET_INFO, LOG_SYS_BUF_RESULT,
	LOG_SYS_GET_ARG, LOG_SYS_GET_ENV, LOG_SYS_READ_FILE,
	LOG_SYS_WRITE_FILE, LOG_SYS_TEST,

	LOG_OC_OFFS, LOG_OC_A_B_IMM, LOG_OC_A_B_IMM_2, LOG_OC_A_IMM,
	LOG_OC_A_B, LOG_OC_A_B_C, LOG_OC_TEST,

	LOG_EMIT_OFFS, LOG_EMIT_A_B_IMM, LOG_EMIT_A_B_C,
	LOG_EMIT_SYS, LOG_EMIT_TEST,

	LOG_CPU_EXECUTE, LOG_CPU_TEST,

	LOG_RUN_CREATE_MODULE, LOG_RUN_EXEC_MODULE, LOG_RUN_TEST,

	MAX_ID
} log_id;

extern void log_init(void);
extern void log_on_off(log_id id, int on_off);
extern void log_msg(log_id id, const char *format, ...);
extern void log_dump(log_id id, char *bytes, int bytes_n);
