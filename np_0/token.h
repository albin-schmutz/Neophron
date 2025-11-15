/*

Print tokens and find keywords.

*/

enum token {
	tk_none, tk_char, tk_int, tk_ident, tk_eq, tk_neq, tk_lss, tk_leq,
	tk_gtr, tk_geq, tk_amper, tk_assign, tk_asterisk, tk_brack_l, tk_brack_r,
	tk_colon, tk_comma, tk_dot, tk_minus, tk_paren_l, tk_paren_r, tk_plus,
	tk_semic, tk_tilde,
	kw_do, kw_if, kw_of, kw_or,
	kw_div, kw_end, kw_mod, kw_var,
	kw_else, kw_then, kw_true, kw_type,
	kw_array, kw_begin, kw_const, kw_elsif, kw_false, kw_until, kw_while,
	kw_import, kw_module, kw_record, kw_repeat, kw_return,
	kw_procedure
};

extern void tk_init(void);
extern INTEGER tk_get_str(char *buf, INTEGER tk);
extern INTEGER tk_find_kw(char *id, INTEGER len);
