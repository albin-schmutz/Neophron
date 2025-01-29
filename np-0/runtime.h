/*
	Loads, saves and executes bytecode modules.
*/

struct HEADER {
	char modulename[32];
	struct HEADER *prev;
	void *prog_addr;
	int var_size;
	int prog_size;
	int entry_point;
	int dummy;
};

extern int64_t mem_start;
extern int64_t mem_size;

extern void run_init(void);
extern void run_align(int *addr, int size);
extern void run_create_module(int64_t *prog_addr, char *name, int var_size);
extern void run_exec_module(int prog_size, int entry_point);
extern void run_write_file(void);
extern int run_load_file(char *modulename);
