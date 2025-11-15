#include <assert.h>
#include <string.h>
#include "global.h"
#include "clazz.h"
#include "alloc_free.h"
#include "tables.h"

INTEGER tbl_level;

INTEGER tbl_boolean_type;
INTEGER tbl_char_type;
INTEGER tbl_short_int_type;
INTEGER tbl_integer_type;
INTEGER tbl_long_int_type;

static struct ALLOC_FREE af_types;
struct TYPE tbl_types[MAX_TYPES];

static struct ALLOC_FREE af_symbols;
struct SYMBOL tbl_symbols[MAX_SYMBOLS];

INTEGER tbl_marker;
INTEGER tbl_top_scope;

static INTEGER top_module;

void tbl_alloc_type(INTEGER *t)
{
	af_alloc(&af_types, t);
}

void tbl_alloc_symbol(INTEGER *s)
{
	af_alloc(&af_symbols, s);
	SYMBOL symbol = &tbl_symbols[*s];
	symbol->name[0] = 0;
	symbol->val = 0;
	symbol->val2 = 0;
	symbol->val3 = 0;
	symbol->next = tbl_marker;
	symbol->clazz = 0;
	symbol->scope = -1;
	symbol->type = -1;
	symbol->level = -1;
	symbol->is_public = FALSE;
}

TYPE tbl_create_type(INTEGER *t, INTEGER form, INTEGER size)
{
	assert(t);

	tbl_alloc_type(t);
	TYPE type = &tbl_types[*t];
	type->size = size;
	type->length = 0;
	type->form = form;
	type->owner = -1;
	type->base = -1;
	return type;
}

static void init_symbol(INTEGER *s, char *name, INTEGER clazz, INTEGER t, LONGINT val)
{
	assert(s);
	assert(*s);
	assert(name);

	INTEGER s2;

	tbl_alloc_symbol(&s2);
	SYMBOL symbol = &tbl_symbols[s2];

	strcpy(symbol->name, name);
	tbl_symbols[*s].next = s2;
	symbol->clazz = clazz;
	symbol->type = t;
	symbol->level = 0;
	symbol->val = val;
	if (clazz == CLASS_TYPE) {
		tbl_types[t].owner = s2;
	}
	*s = s2;
}

void tbl_open_scope()
{
	INTEGER s;

	tbl_alloc_symbol(&s);
	SYMBOL symbol = &tbl_symbols[s];
	symbol->clazz = CLASS_SCOPE;
	symbol->scope = tbl_top_scope;
	tbl_top_scope = s;
}

void tbl_init(void)
{
	INTEGER s;

	af_init();
	tbl_level = 0;
	tbl_top_scope = -1;
	af_create(&af_types, MAX_TYPES);
	tbl_create_type(&tbl_boolean_type, FORM_BOOL, 1);
	tbl_create_type(&tbl_char_type, FORM_CHAR, 1);
	tbl_create_type(&tbl_short_int_type, FORM_INT, 2);
	tbl_create_type(&tbl_integer_type, FORM_INT, 4);
	tbl_create_type(&tbl_long_int_type, FORM_INT, 8);
	af_create(&af_symbols, MAX_SYMBOLS);
	tbl_alloc_symbol(&tbl_marker);
	tbl_open_scope();
	s = tbl_top_scope;
	init_symbol(&s, "ASSERT", CLASS_SPROC, -1, 1);
	init_symbol(&s, "SYS", CLASS_SPROC, -1, 2);
	init_symbol(&s, "CHR", CLASS_SFUNC, tbl_char_type, 1);
	init_symbol(&s, "ORD", CLASS_SFUNC, tbl_integer_type, 2);
	init_symbol(&s, "ASH", CLASS_SFUNC, tbl_long_int_type, 3);
	init_symbol(&s, "BITA", CLASS_SFUNC, tbl_long_int_type, 4);
	init_symbol(&s, "BITO", CLASS_SFUNC, tbl_long_int_type, 5);
	init_symbol(&s, "SIZE", CLASS_SFUNC, tbl_long_int_type, 6);
	init_symbol(&s, "ADDR", CLASS_SFUNC, tbl_long_int_type, 7);
	init_symbol(&s, "BOOLEAN", CLASS_TYPE, tbl_boolean_type, 0);
	init_symbol(&s, "CHAR", CLASS_TYPE, tbl_char_type, 0);
	init_symbol(&s, "SHORTINT", CLASS_TYPE, tbl_short_int_type, 0);
	init_symbol(&s, "INTEGER", CLASS_TYPE, tbl_integer_type, 0);
	init_symbol(&s, "LONGINT", CLASS_TYPE, tbl_long_int_type, 0);
	tbl_symbols[s].next = tbl_marker;
	tbl_alloc_symbol(&top_module);
}

void tbl_set_marker(char *id)
{
	assert(id);

	strcpy(tbl_symbols[tbl_marker].name, id);
}

void tbl_find_symbol(INTEGER *s)
{
	assert(s);

	while (strcmp(tbl_symbols[tbl_symbols[*s].next].name, tbl_symbols[tbl_marker].name)) {
		*s = tbl_symbols[*s].next;
	}
}

static void dealloc_type(INTEGER t, BOOLEAN private_only)
{
	TYPE type;
	INTEGER scope;

	type = &tbl_types[t];
	if (type->form == FORM_RECORD) {
		scope = type->base;
		tbl_dealloc_scope(scope, private_only);
		if (tbl_symbols[scope].next == tbl_marker) {
			type->base = -1;
		}
	} else if (type->form == FORM_ARRAY && tbl_types[type->base].owner == -1) {
		dealloc_type(type->base, private_only);
		af_free(&af_types, type->base);
	}
}

static void dealloc_symbol(INTEGER s, BOOLEAN private_only)
{
	INTEGER t;

	SYMBOL symbol = &tbl_symbols[s];
	if (symbol->clazz == CLASS_PROC && symbol->scope != -1) {
		tbl_dealloc_scope(symbol->scope, private_only);
	}
	t = symbol->type;
	if (t != -1) {
		TYPE type = &tbl_types[t];
 		if (type->owner == -1 || type->owner == s) {
			dealloc_type(t, private_only);
			if (!private_only || !symbol->is_public) {
				af_free(&af_types, t);
			}
		}
	}
}

void tbl_dealloc_scope(INTEGER scope, BOOLEAN private_only)
{
	assert(scope >= 0);

	SYMBOL prev_p;
	SYMBOL symbol;
	INTEGER prev;
	INTEGER s;

	prev = scope;
	prev_p = &tbl_symbols[prev];
	s = prev_p->next;
	while (s != tbl_marker) {
		symbol = &tbl_symbols[s];
		if (private_only && symbol->is_public) {
			dealloc_symbol(s, 1);
			prev = s;
			prev_p = &tbl_symbols[prev];
			s = symbol->next;
		} else {
			dealloc_symbol(s, 0);
			prev_p->next = symbol->next;
			af_free(&af_symbols, s);
			s = prev_p->next;
		}
	}
	if (tbl_symbols[scope].next == tbl_marker) {
		af_free(&af_symbols, scope);
	}
}

void tbl_close_scope(void)
{
	tbl_top_scope = tbl_symbols[tbl_top_scope].scope;
}

SYMBOL tbl_def_symbol(INTEGER *s, INTEGER clazz, INTEGER m)
{
	assert(s);

	SYMBOL symbol = NULL;
	INTEGER prev = tbl_top_scope;

	tbl_find_symbol(&prev);
	*s = -1;
	if (tbl_symbols[prev].next == tbl_marker) {
		tbl_alloc_symbol(s);
		symbol = &tbl_symbols[*s];
		if (clazz > CLASS_SCOPE) {
			strcpy(symbol->name, tbl_symbols[tbl_marker].name);
		}
		symbol->clazz = clazz;
		symbol->scope = m;
		tbl_symbols[prev].next = *s;
	}
	return symbol;
}

SYMBOL tbl_use_symbol(INTEGER *s, INTEGER imp)
{
	assert(s);

	INTEGER scope;

	*s = -1;
	if (imp == -1) {
		scope = tbl_top_scope;
	} else {
		scope = tbl_symbols[imp].scope;
	}
	assert(tbl_symbols[scope].clazz == CLASS_SCOPE);
	while (scope != -1) {
		*s = scope;
		tbl_find_symbol(s);
		*s = tbl_symbols[*s].next;
		if (*s == tbl_marker) *s = -1;
		if (*s == -1 && imp == -1) {
			scope = tbl_symbols[scope].scope;
		} else {
			scope = -1;
		}
	}

	return *s == -1 ? NULL : &tbl_symbols[*s];
}

INTEGER tbl_open_module(char *name)
{
	tbl_set_marker(name);
	INTEGER m = top_module;
	tbl_find_symbol(&m);
	if (tbl_symbols[m].next == tbl_marker) {
		tbl_alloc_symbol(&tbl_symbols[m].next);
		m = tbl_symbols[m].next;
		strcpy(tbl_symbols[m].name, name);
		tbl_open_scope();
		tbl_symbols[m].scope = tbl_top_scope;
	} else {
		m = tbl_symbols[m].next;
	}
	return m;
}
