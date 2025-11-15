#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "sourcefile.h"
#include "token.h"
#include "scanner.h"

#define MAX_LEN_INT 20

void scan_raise_error(SCANNER scan, char *msg)
{
	assert(scan);
	assert(msg);

	fprintf(stderr, "Error in %s at %i/%i: %s\n\n",
		scan->module_name, scan->line, scan->pos, msg);
	exit(1);
}

void scan_init_root(SCANNER scan)
{
	strcpy(scan->module_name, "ROOT");
	scan->line = 0; scan->pos = 0;
}

void scan_init(SCANNER scan, SCANNER parent)
{
	assert(scan);
	assert(parent);

	char msg[80];
	char filename[64];

	strcpy(filename, scan->module_name);
	strcat(filename, ".np0");
	sf_open(&scan->sf, filename);
	if (scan->sf.ch == -1) {
		strcpy(msg, "Open file error: ");
		strcat(msg, filename);
		scan_raise_error(parent, msg);
	}
	scan->ch = scan->sf.ch;
}

static void get_char(SCANNER scan)
{
	sf_fetch_char(&scan->sf);
	if (scan->sf.ch == -1) {
		scan_raise_error(scan, "Unexpected eof");
	}
	scan->ch = scan->sf.ch;
}

static void unexpected_character(SCANNER scan)
{
	scan_raise_error(scan, "Unexpected character");
}

static void comment(SCANNER scan)
{
	get_char(scan);
	BOOLEAN ok = TRUE;
	while (ok) {
		if (scan->ch == '*') {
			get_char(scan);
			if (scan->ch == ')') {
				get_char(scan);
				ok = FALSE;
			}
		} else if (scan->ch == '(') {
			get_char(scan);
			if (scan->ch == '*') comment(scan);
		} else {
			get_char(scan);
		}
	}
}

static void number(SCANNER scan)
{
	CHAR buf[MAX_LEN_INT];
	INTEGER base = 10; BOOLEAN hex = 0; INTEGER i = 0;
	while (hex || (scan->ch >= '0' && scan->ch <= '9')) {
		if (i < MAX_LEN_INT) {
			buf[i++] = scan->ch;
			get_char(scan);
			hex = scan->ch >= 'A' && scan->ch <= 'F';
			if (hex) base = 16;
		} else {
			scan_raise_error(scan,"Integer too long");
		}
	}
	scan->tk = tk_int;
	if (scan->ch == 'H') {
		base = 16;
		get_char(scan);
	} else if (scan->ch == 'X') {
		scan->tk = tk_char;
		base = 16;
		get_char(scan);
	} else if (base == 16) {
		scan_raise_error(scan, "Postfix H or X ?");
	}
	INTEGER j = 0;
	scan->val = 0;
	while (j < i) {
		CHAR c = buf[j++];
		INTEGER n = c >= 'A' && c <= 'F' ? 10 - 'A' + c : c - '0';
		scan->val = base * scan->val + n;
	}
}

static void ident(SCANNER scan)
{
	INTEGER i = 0;
	INTEGER upper = 1;
	while ((scan->ch >= 'A' && scan->ch <= 'Z') ||
		(scan->ch >= '0' && scan->ch <= '9') ||
		(scan->ch >= 'a' && scan->ch <= 'z')) {
		if (i < 31) {
			upper = upper && scan->ch >= 'A' && scan->ch <= 'Z';
			scan->ident[i++] = scan->ch;
			get_char(scan);
		} else {
			scan_raise_error(scan, "Identifier too long");
		}
	}
	scan->ident[i] = 0;
	scan->tk = (upper && i < 10) ?
		tk_find_kw(scan->ident, i) : tk_none;
	if (scan->tk == tk_none) scan->tk = tk_ident;
}

void scan_take(SCANNER scan)
{
	assert(scan);

	scan->tk = tk_none;
	while (scan->tk == tk_none) {
		while (scan->ch <= ' ') get_char(scan);
		scan->line = scan->sf.line;
		scan->pos = scan->sf.pos;
		if (scan->ch < 'A') {
			if (scan->ch < '0') {
				if (scan->ch == '#') {
					scan->tk = tk_neq;
					get_char(scan);
				} else if (scan->ch == '&') {
					scan->tk = tk_amper;
					get_char(scan);
				} else if (scan->ch == '(') {
					get_char(scan);
					if (scan->ch == '*') {
						comment(scan);
					} else {
						scan->tk = tk_paren_l;
					}
				} else if (scan->ch == ')') {
					scan->tk = tk_paren_r;
					get_char(scan);
				} else if (scan->ch == '*') {
					scan->tk = tk_asterisk;
					get_char(scan);
				} else if (scan->ch == '+') {
					scan->tk = tk_plus;
					get_char(scan);
				} else if (scan->ch == ',') {
					scan->tk = tk_comma;
					get_char(scan);
				} else if (scan->ch == '-') {
					scan->tk = tk_minus;
					get_char(scan);
				} else if (scan->ch == '.') {
					scan->tk = tk_dot;
					get_char(scan);
				} else { /* ! " $ ' / */
					unexpected_character(scan);
				}
			} else if (scan->ch < ':') {
				number(scan);
			} else if (scan->ch == ':') {
				scan->tk = tk_colon;
				get_char(scan);
				if (scan->ch == '=') {
					scan->tk = tk_assign;
					get_char(scan);
				}
			} else if (scan->ch == ';') {
				scan->tk = tk_semic;
				get_char(scan);
			} else if (scan->ch == '<') {
				scan->tk = tk_lss;
				get_char(scan);
				if (scan->ch == '=') {
					scan->tk = tk_leq;
					get_char(scan);
				}
			} else if (scan->ch == '>') {
				scan->tk = tk_gtr;
				get_char(scan);
				if (scan->ch == '=') {
					scan->tk = tk_geq;
					get_char(scan);
				}
			} else if (scan->ch == '=') {
				scan->tk = tk_eq;
				get_char(scan);
			} else { /* ? @ */
				unexpected_character(scan);
			}
		} else if (scan->ch < '[') {
			ident(scan);
		} else if (scan->ch < 'a') {
			if (scan->ch == '[') {
				scan->tk = tk_brack_l;
				get_char(scan);
			} else if (scan->ch == ']') {
				scan->tk = tk_brack_r;
				get_char(scan);
			} else { /* \ ^ _ ` */
				unexpected_character(scan);
			}
		} else if (scan->ch < '{') {
			ident(scan);
		} else {
			if (scan->ch == '~') {
				scan->tk = tk_tilde;
				get_char(scan);
			} else { /* { | } */
				unexpected_character(scan);
			}
		}
	}
}

void scan_expect(SCANNER scan, enum token exp)
{
	assert(scan);

	char buf[16];

	if (scan->tk != exp) {
		tk_get_str(buf, exp);
		strcat(buf, " ?");
		scan_raise_error(scan, buf);
	}
}
