/*

Generates bytecode.

*/

typedef struct GEN *GEN;

struct GEN {
	LONGINT val;
	LONGINT val2;
	INTEGER size;
	INTEGER cond;
	INTEGER reg;
	INTEGER mode;
	INTEGER type;
	BOOLEAN exp_const;
};

extern void gen_start(LONGINT pc);
extern void gen_exit(void);
extern void gen_align(void);
extern void gen_get_pc_rel(INTEGER *pc);
extern void gen_get_pc_abs(LONGINT *pc);

extern void gen_enter(INTEGER var_size);
extern void gen_return(INTEGER par_size);

extern void gen_fix_jumps(INTEGER label);

extern void gen_param(GEN x, BOOLEAN is_ref);
extern void gen_deref(GEN x);

extern void gen_assert(GEN x, INTEGER line);
extern void gen_sys(INTEGER call_nr, GEN x, GEN y);
extern void gen_ash(GEN x, GEN y);
extern void gen_bita(GEN x, GEN y);
extern void gen_bito(GEN x, GEN y);
extern void gen_addr(GEN x);

extern void gen_not(GEN x);
extern void gen_neg(GEN x);

extern void gen_eq(GEN x, GEN y);
extern void gen_neq(GEN x, GEN y);
extern void gen_lss(GEN x, GEN y);
extern void gen_leq(GEN x, GEN y);
extern void gen_gtr(GEN x, GEN y);
extern void gen_geq(GEN x, GEN y);

extern void gen_or(GEN x);
extern void gen_or2(GEN x, GEN y);
extern void gen_and(GEN x);
extern void gen_and2(GEN x, GEN y);

extern void gen_mul(GEN x, GEN y);
extern void gen_div(GEN x, GEN y);
extern void gen_mod(GEN x, GEN y);
extern void gen_add(GEN x, GEN y);
extern void gen_sub(GEN x, GEN y);

extern void gen_store(GEN x, GEN y);
extern void gen_index(GEN x, GEN y, INTEGER arr_len, INTEGER size);
extern void gen_field(GEN x, INTEGER addr);

extern void gen_bcall(LONGINT addr);
extern void gen_fcall(INTEGER *label);
extern void gen_fjump(INTEGER *label);
extern void gen_bjump(INTEGER label);
extern void gen_cond_bjump(GEN x, INTEGER label);
extern void gen_cond_fjump(GEN x);
