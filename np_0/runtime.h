/*

Loads, saves and executes bytecode modules.
- Allocates memory for programcode and stack. 

Memory layout:

h = header
v = static variables
p = programcode
last_module = last module pointer

|  Module 1          |  Module 2          | ...
------------------------------------------- ...
|  h1  |  v1  |  p1  |  h2  |  v2  |  p2  | ...
                     |
                    last_module

Header
	prev : Address previous Module

Size
	h : fixed sized
	v : known at end of compilation
	p : known at end of compilation

*/

#define MAX_LEN_IDENTIFIER 32

struct HEADER {
	char modulename[MAX_LEN_IDENTIFIER];
	struct HEADER *prev;
	LONGINT prog_addr;
	INTEGER var_size;
	INTEGER prog_size;
	INTEGER entry_point;
	INTEGER dummy;
};

extern struct HEADER *last_module;

extern void run_init(void);
extern LONGINT run_align(LONGINT addr, INTEGER size);
extern LONGINT run_create_module(char *name, INTEGER var_size,
	INTEGER prog_size, INTEGER entry_point);
extern void run_exec_module(INTEGER prog_size, INTEGER entry_point);
extern BOOLEAN run_load_file(char *modulename);
