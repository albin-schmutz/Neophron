#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "global.h"
#include "sourcefile.h"
#include "token.h"
#include "scanner.h"

static struct SCANNER parent;
static struct SCANNER scan;

static void test_simple_tokens(void)
{
	scan_take(&scan);
	assert(scan.tk == tk_amper);
	scan_take(&scan);
	assert(scan.tk == tk_leq);
	scan_take(&scan);
	assert(scan.tk == tk_assign);
	scan_take(&scan);
	assert(scan.tk == tk_geq);
}

static void test_numbers(void)
{
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 9223372036854775806);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 9223372036854775807);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == -9223372036854775807 - 1);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 0);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 0);
	scan_take(&scan);
	assert(scan.tk == tk_char);
	assert(scan.val == 1);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 2147483647);
	scan_take(&scan);
	assert(scan.tk == tk_int);
	assert(scan.val == 0x7fffffffffffffff);
}

static void test_identifiers(void)
{
	scan_take(&scan);
	assert(scan.tk == tk_ident);
	assert(!strcmp("M012345678901234567890123456789", scan.ident));
	scan_take(&scan);
	assert(scan.tk == kw_module);
	scan_take(&scan);
	assert(scan.tk == kw_procedure);
}

int main(void)
{
	scan_init_root(&parent);
	strcpy(scan.module_name, "scanner_testfile");
	scan_init(&scan, &parent);
	test_simple_tokens();
	test_numbers();
	test_identifiers();
	return 0;
}
