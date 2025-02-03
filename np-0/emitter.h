/*
	Provides an API for generating opcode instructions.
	- Encodes instructions.
	- Manages the program counter for a target memory region.
*/

typedef struct EMITTER *EMITTER;

struct EMITTER {
	struct OPCODE opcode;
	int *base;
	int pc;
};

extern void emit_init(EMITTER emit, int64_t start);
extern void emit_offs(EMITTER emit, int oc, int imm);
extern void emit_a_b_imm(EMITTER emit, int oc, int a, int b, int64_t imm);
extern void emit_a_b_c(EMITTER emit, int oc, int a, int b, int c);
extern void emit_sys(EMITTER emit, int a, int b, int nr);
