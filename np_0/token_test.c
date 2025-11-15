#include <assert.h>
#include <string.h>
#include "global.h"
#include "token.h"

static void test_token_int(void)
{
	assert(tk_none == 0);
	assert(tk_eq == 4);
	assert(tk_assign == 11);
	assert(tk_minus == 18);
	assert(kw_do == 24);
	assert(kw_array == 36);
	assert(kw_procedure == 48);
}

static void test_get_str(void)
{
	char id[16];
	assert(tk_get_str(id, tk_eq) == 1);
	assert(!strcmp("=", id));
	assert(tk_get_str(id, tk_amper) == 1);
	assert(!strcmp("&", id));
	assert(tk_get_str(id, kw_div) == 3);
	assert(!strcmp("DIV", id));
	assert(tk_get_str(id, tk_assign) == 2);
	assert(!strcmp(":=", id));
	assert(tk_get_str(id, kw_record) == 6);
	assert(!strcmp("RECORD", id));
	assert(tk_get_str(id, kw_procedure) == 9);
	assert(!strcmp("PROCEDURE", id));
}

static void test_find_kw(void)
{
	assert(tk_find_kw("IF", 2) == kw_if);
	assert(tk_find_kw("BEGIN", 5) == kw_begin);
	assert(tk_find_kw("IMPORT", 6) == kw_import);
	assert(tk_find_kw("ELSE", 4) == kw_else);
	assert(tk_find_kw("TYPE", 4) == kw_type);
	assert(tk_find_kw("PROCEDURE", 9) == kw_procedure);
	assert(tk_find_kw("PROCEDURe", 9) == tk_none);
}

int main(void)
{
	tk_init();
	test_token_int();
	test_get_str();
	test_find_kw();
	return 0;
}
