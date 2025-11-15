#include <assert.h>
#include "global.h"
#include "clazz.h"
#include "tables.h"

#define INIT_LAST_TYPE 4
#define INIT_LAST_SYMBOL 16

static void test_init(void)
{
	INTEGER s;

	tbl_init();

	assert(tbl_level == 0);

	assert(tbl_short_int_type == 2);
	TYPE type = &tbl_types[tbl_short_int_type];
	assert(type->size == 2);
	assert(type->form == FORM_INT);

	assert(tbl_long_int_type == INIT_LAST_TYPE);

	assert(tbl_marker == 0);
	assert(tbl_top_scope == 1);

	tbl_set_marker("ASSERT");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == 1);
	assert(tbl_symbols[s].next == 2);

	tbl_set_marker("SHORTINT");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == INIT_LAST_SYMBOL - 4);
	assert(tbl_symbols[s].next == INIT_LAST_SYMBOL - 3);

	tbl_set_marker("LONGINT");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == INIT_LAST_SYMBOL - 2);
	assert(tbl_symbols[s].next == INIT_LAST_SYMBOL - 1);

	assert(tbl_symbols[INIT_LAST_SYMBOL].next == 0);
}

static void deallocate_scope_with_childs(BOOLEAN is_public[2])
{
	INTEGER s;

	/* scope with const a and b */
	tbl_init();
	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER a = INIT_LAST_SYMBOL + 2;
	INTEGER b = INIT_LAST_SYMBOL + 3;

	tbl_open_scope();
	assert(tbl_top_scope == scope);

	tbl_set_marker("a");
	SYMBOL symbol = tbl_def_symbol(&s, CLASS_CONST, -1);
	symbol->is_public = is_public[0];

	assert(tbl_symbols[scope].next == s);
	assert(s == a);

	tbl_set_marker("b");
	symbol = tbl_def_symbol(&s, CLASS_CONST, -1);
	symbol->is_public = is_public[1];
	assert(s == b);

	tbl_dealloc_scope(tbl_top_scope, 1);
}

static void test_deallocate_scope_with_childs(void)
{
	INTEGER s;

	BOOLEAN is_public[3][2] = {
		{ FALSE, FALSE },
		{ FALSE, TRUE },
		{ TRUE, TRUE },
	};

	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER a = INIT_LAST_SYMBOL + 2;
	INTEGER b = INIT_LAST_SYMBOL + 3;

	deallocate_scope_with_childs(is_public[0]);
	tbl_alloc_symbol(&s);
	assert(s == scope);
	tbl_alloc_symbol(&s);
	assert(s == b);
	tbl_alloc_symbol(&s);
	assert(s == a);

	deallocate_scope_with_childs(is_public[1]);
	tbl_alloc_symbol(&s);
	assert(s == a);

	tbl_set_marker("a");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == b);
	assert(tbl_symbols[s].next == tbl_marker);
	tbl_set_marker("b");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == scope);
	assert(tbl_symbols[s].next == b);

	deallocate_scope_with_childs(is_public[2]);
	tbl_alloc_symbol(&s);
	assert(s == b + 1);

	tbl_set_marker("a");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == scope);
	assert(tbl_symbols[s].next == a);
	tbl_set_marker("b");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == a);
	assert(tbl_symbols[s].next == b);
}


static void deallocate_array(BOOLEAN is_public[3])
{
	INTEGER s;
	INTEGER t;

	/* scope with array A containing record with members i and j */
	tbl_init();
	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER A = INIT_LAST_SYMBOL + 2;
	INTEGER A_type = INIT_LAST_TYPE + 1;
	INTEGER R_type = INIT_LAST_TYPE + 2;
	INTEGER R_type_scope = INIT_LAST_SYMBOL + 3;
	INTEGER i = INIT_LAST_SYMBOL + 4;
	INTEGER j = INIT_LAST_SYMBOL + 5;

	tbl_open_scope();
	assert(tbl_top_scope == scope);

	tbl_set_marker("A");
	SYMBOL symbol_A = tbl_def_symbol(&s, CLASS_TYPE, -1);
	symbol_A->is_public = is_public[0];
	assert(s == A);

	tbl_create_type(&t, FORM_ARRAY, 0);
	symbol_A->type = t;
	TYPE type_A = &tbl_types[symbol_A->type];
	assert(symbol_A->type == A_type);

	tbl_create_type(&t, FORM_RECORD, 0);
	type_A->base = t;
	TYPE type_R = &tbl_types[type_A->base];
	assert(type_A->base == R_type);

	tbl_open_scope();
	type_R->base = tbl_top_scope;
	assert(type_R->base == R_type_scope);

	tbl_set_marker("i");
	SYMBOL symbol_i = tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == i);
	symbol_i->type = tbl_integer_type;
	symbol_i->is_public = is_public[1];

	tbl_set_marker("j");
	SYMBOL symbol_j = tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == j);
	symbol_j->type = tbl_integer_type;
	symbol_j->is_public = is_public[2];

	tbl_close_scope();
	assert(tbl_top_scope == scope);

	tbl_dealloc_scope(scope, 1);
}

static void test_deallocate_array(void)
{
	INTEGER s;

	BOOLEAN is_public[3][3] = {
		{ FALSE, FALSE, FALSE },
		{ TRUE, FALSE, FALSE },
		{ TRUE, TRUE, FALSE },
	};

	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER A = INIT_LAST_SYMBOL + 2;
	INTEGER A_type = INIT_LAST_TYPE + 1;
	INTEGER R_type = INIT_LAST_TYPE + 2;
	INTEGER R_type_scope = INIT_LAST_SYMBOL + 3;
	INTEGER i = INIT_LAST_SYMBOL + 4;
	INTEGER j = INIT_LAST_SYMBOL + 5;

	/*

	A = ARRAY 10 OF RECORD i : INTEGER; j : INTEGER END;

	free i j R_type_scope R_type A_type A scope

	*/

	deallocate_array(is_public[0]);
	tbl_alloc_symbol(&s);
	assert(s == scope);
	tbl_alloc_symbol(&s);
	assert(s == A);
	tbl_alloc_type(&s);
	assert(s == A_type);
	tbl_alloc_type(&s);
	assert(s == R_type);
	tbl_alloc_symbol(&s);
	assert(s == R_type_scope);
	tbl_alloc_symbol(&s);
	assert(s == j);
	tbl_alloc_symbol(&s);
	assert(s == i);

	/*

	A* = ARRAY 10 OF RECORD i : INTEGER; j : INTEGER END;

	free i j R_type_scope R_type

	*/

	deallocate_array(is_public[1]);
	tbl_alloc_type(&s);
	assert(s == R_type);
	tbl_alloc_symbol(&s);
	assert(s == R_type_scope);
	tbl_alloc_symbol(&s);
	assert(s == j);
	tbl_alloc_symbol(&s);
	assert(s == i);

	/*

	A* = ARRAY 10 OF RECORD i* : INTEGER; j : INTEGER END;

	free j R_type

	*/

	deallocate_array(is_public[2]);
	tbl_alloc_type(&s);
	assert(s == R_type);
	tbl_alloc_symbol(&s);
	assert(s == j);
}


static void deallocate_record(BOOLEAN is_public[4])
{
	INTEGER s;
	INTEGER t;

	/* scope with record R and members i j and array a */
	tbl_init();
	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER R = INIT_LAST_SYMBOL + 2;
	INTEGER R_type = INIT_LAST_TYPE + 1;
	INTEGER R_type_scope = INIT_LAST_SYMBOL + 3;
	INTEGER i = INIT_LAST_SYMBOL + 4;
	INTEGER j = INIT_LAST_SYMBOL + 5;
	INTEGER a = INIT_LAST_SYMBOL + 6;
	INTEGER a_type = INIT_LAST_TYPE + 2;

	tbl_open_scope();
	assert(tbl_top_scope == scope);

	tbl_set_marker("R");
	tbl_def_symbol(&s, CLASS_TYPE, -1);
	assert(s == R);
	SYMBOL symbol_R = &tbl_symbols[s];
	symbol_R->is_public = is_public[0];

	tbl_create_type(&t, FORM_RECORD, 0);
	assert(t == R_type);
	symbol_R->type = t;
	TYPE type_R = &tbl_types[symbol_R->type];

	tbl_open_scope();
	type_R->base = tbl_top_scope;
	assert(R_type_scope == tbl_top_scope);
	assert(type_R->base == R_type_scope);

	tbl_set_marker("i");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == i);
	SYMBOL symbol_i = &tbl_symbols[s];
	symbol_i->is_public = is_public[1];
	symbol_i->type = tbl_integer_type;

	tbl_set_marker("j");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == j);
	SYMBOL symbol_j = &tbl_symbols[s];
	symbol_j->is_public = is_public[2];
	symbol_j->type = tbl_integer_type;

	tbl_set_marker("a");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == a);
	SYMBOL symbol_a = &tbl_symbols[s];
	symbol_a->is_public = is_public[3];

	tbl_create_type(&t, FORM_ARRAY, 0);
	assert(t == a_type);
	symbol_a->type = t;
	TYPE type_a = &tbl_types[symbol_a->type];
	type_a->base = tbl_integer_type;

	tbl_close_scope();
	assert(tbl_top_scope == scope);

	tbl_dealloc_scope(tbl_top_scope, 1);
}

static void test_deallocate_record(void)
{
	INTEGER s;

	BOOLEAN is_public[3][4] = {
		{ FALSE, FALSE, FALSE, FALSE },
		{ TRUE, FALSE, FALSE, FALSE },
		{ TRUE, FALSE, FALSE, TRUE }
	};

	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER R = INIT_LAST_SYMBOL + 2;
	INTEGER R_type = INIT_LAST_TYPE + 1;
	INTEGER R_type_scope = INIT_LAST_SYMBOL + 3;
	INTEGER i = INIT_LAST_SYMBOL + 4;
	INTEGER j = INIT_LAST_SYMBOL + 5;
	INTEGER a = INIT_LAST_SYMBOL + 6;
	INTEGER a_type = INIT_LAST_TYPE + 2;

	/*

	R = RECORD
		i : INTEGER;
		j : INTEGER;
		a : ARRAY[] OF INTEGER
	END;

	free i j a_type a R_type_scope R_type R scope

	*/

	deallocate_record(is_public[0]);
	tbl_alloc_symbol(&s);
	assert(s == scope);
	tbl_alloc_symbol(&s);
	assert(s == R);
	tbl_alloc_type(&s);
	assert(s == R_type);
	tbl_alloc_symbol(&s);
	assert(s == R_type_scope);
	tbl_alloc_type(&s);
	assert(s == a_type);
	tbl_alloc_symbol(&s);
	assert(s == a);
	tbl_alloc_symbol(&s);
	assert(s == j);
	tbl_alloc_symbol(&s);
	assert(s == i);

	/*

	R* = RECORD
		i : INTEGER;
		j : INTEGER;
		a : ARRAY[] OF INTEGER
	END;

	free i j a_type a R_type_scope

	*/

	deallocate_record(is_public[1]);
	assert(tbl_types[R_type].base == -1);
	tbl_alloc_symbol(&s);
	assert(s == R_type_scope);
	tbl_alloc_type(&s);
	assert(s == a_type);
	tbl_alloc_symbol(&s);
	assert(s == a);
	tbl_alloc_symbol(&s);
	assert(s == j);
	tbl_alloc_symbol(&s);
	assert(s == i);

	/*

	R* = RECORD
		i : INTEGER;
		j : INTEGER;
		a* : ARRAY[] OF INTEGER
	END;

	free i j

	*/

	deallocate_record(is_public[2]);
	tbl_alloc_symbol(&s);
	assert(s == j);
	tbl_alloc_symbol(&s);
	assert(s == i);
}

static void deallocate_procedure(BOOLEAN is_public[3])
{
	INTEGER s;

	/* scope with procedure P, params a b, variables c, d */
	tbl_init();
	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER P = INIT_LAST_SYMBOL + 2;
	INTEGER P_scope = INIT_LAST_SYMBOL + 3;
	INTEGER a = INIT_LAST_SYMBOL + 4;
	INTEGER b = INIT_LAST_SYMBOL + 5;
	INTEGER c = INIT_LAST_SYMBOL + 6;
	INTEGER d = INIT_LAST_SYMBOL + 7;

	tbl_open_scope();
	assert(tbl_top_scope == scope);

	tbl_set_marker("P");
	tbl_def_symbol(&s, CLASS_PROC, -1);
	assert(s == P);
	tbl_symbols[P].is_public = is_public[0];

	tbl_open_scope();
	assert(P_scope == tbl_top_scope);
	tbl_symbols[P].scope = P_scope;

	tbl_set_marker("a");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == a);
	tbl_symbols[a].is_public = is_public[1];

	tbl_set_marker("b");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == b);
	tbl_symbols[b].is_public = is_public[2];

	tbl_set_marker("c");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == c);

	tbl_set_marker("d");
	tbl_def_symbol(&s, CLASS_VAR, -1);
	assert(s == d);

	tbl_close_scope();
	assert(tbl_top_scope == scope);

	tbl_dealloc_scope(tbl_top_scope, 1);
}

static void test_deallocate_procedure(void)
{
	INTEGER s;

	BOOLEAN is_public[2][3] = {
		{ TRUE, TRUE, TRUE },
		{ FALSE, FALSE, FALSE }
	};

	INTEGER scope = INIT_LAST_SYMBOL + 1;
	INTEGER P = INIT_LAST_SYMBOL + 2;
	INTEGER P_scope = INIT_LAST_SYMBOL + 3;
	INTEGER a = INIT_LAST_SYMBOL + 4;
	INTEGER b = INIT_LAST_SYMBOL + 5;
	INTEGER c = INIT_LAST_SYMBOL + 6;
	INTEGER d = INIT_LAST_SYMBOL + 7;

	/*

	PROCEDURE P*(a : INTEGER; b : INTEGER);
	VAR c : INTEGER; d : INTEGER;

	free c d

	*/

	deallocate_procedure(is_public[0]);
	tbl_alloc_symbol(&s);
	assert(s == d);
	tbl_alloc_symbol(&s);
	assert(s == c);

	/*

	PROCEDURE P(a : INTEGER; b : INTEGER);
	VAR c : INTEGER; d : INTEGER;

	free a b c d P_scope P scope

	*/

	deallocate_procedure(is_public[1]);
	tbl_alloc_symbol(&s);
	assert(s == scope);
	tbl_alloc_symbol(&s);
	assert(s == P);
	tbl_alloc_symbol(&s);
	assert(s == P_scope);
	tbl_alloc_symbol(&s);
	assert(s == d);
	tbl_alloc_symbol(&s);
	assert(s == c);
	tbl_alloc_symbol(&s);
	assert(s == b);
	tbl_alloc_symbol(&s);
	assert(s == a);
}

static void test_def_use_symbol(void)
{
	INTEGER s;

	tbl_init();

	tbl_open_scope();
	assert(tbl_top_scope == INIT_LAST_SYMBOL + 1);

	tbl_set_marker("foo");
	tbl_def_symbol(&s, CLASS_CONST, -1);
	assert(s == INIT_LAST_SYMBOL + 2);

	tbl_set_marker("bar");
	tbl_def_symbol(&s, CLASS_CONST, -1);
	assert(s == INIT_LAST_SYMBOL + 3);

	tbl_set_marker("foo");
	tbl_def_symbol(&s, CLASS_CONST, -1);
	assert(s == -1);

	tbl_set_marker("bar");
	s = tbl_top_scope;
	tbl_find_symbol(&s);
	assert(s == INIT_LAST_SYMBOL + 2);
	assert(tbl_symbols[s].next == INIT_LAST_SYMBOL + 3);

	tbl_set_marker("BITO");
	tbl_use_symbol(&s, -1);
	assert(s == 8);

	tbl_set_marker("gugus");
	tbl_use_symbol(&s, -1);
	assert(s == -1);

	tbl_set_marker("foo");
	tbl_use_symbol(&s, -1);
	assert(s == INIT_LAST_SYMBOL + 2);

	tbl_set_marker("bar");
	tbl_use_symbol(&s, -1);
	assert(s == INIT_LAST_SYMBOL + 3);
}

static void test_open_module(void)
{
	tbl_init();

	INTEGER m1 = tbl_open_module("gugus");
	assert(m1 == INIT_LAST_SYMBOL + 1);
	assert(tbl_symbols[m1].scope == INIT_LAST_SYMBOL + 2);

	INTEGER m2 = tbl_open_module("foobar");
	assert(m2 == INIT_LAST_SYMBOL + 3);
	assert(tbl_symbols[m2].scope == INIT_LAST_SYMBOL + 4);

	INTEGER m3 = tbl_open_module("gugus");
	assert(m3 == m1);

	INTEGER m4 = tbl_open_module("foobar");
	assert(m4 == m2);
}

int main(void)
{
	test_init();
	test_deallocate_scope_with_childs();
	test_deallocate_array();
	test_deallocate_record();
	test_deallocate_procedure();
	test_def_use_symbol();
	test_open_module();
	return 0;
}
