#include <string.h>
#include "token.h"

static char *texts = "None Char Int Ident "
	"= # <= < >= > & := * [ ] : , . - ( ) + ; ~ "
	"DO IF OF OR "
	"DIV END MOD VAR "
	"ELSE THEN TRUE TYPE "
	"ARRAY BEGIN CONST ELSIF FALSE UNTIL WHILE "
	"IMPORT MODULE RECORD REPEAT RETURN "
	"PROCEDURE ";

static int len;
static int kw_tk[10] = {
	kw_do, kw_div, kw_else, kw_array, kw_import, 0, 0, kw_procedure
};
static int kw_pos[10] = { 63, 75, 91, 111, 153, 188, 188, 188, 214 };

void tk_init(void)
{
	len = strlen(texts);
}

int tk_get_str(char *buf, int tk)
{
	int i = 0; int j = 0;
	while (i < len) {
		if (tk == 0) {
			while (texts[i] != ' ') buf[j++] = texts[i++];
			buf[j] = 0; i = len;
		} else {
			tk--;
			while (texts[i] != ' ') i++;
			i++;
		}
	}
	return j;
}

int tk_find_kw(char *id, int len)
{
	int kw = kw_tk[len - 2];
	int i = kw_pos[len - 2];
	int j = kw_pos[len - 1];
	while (i < j) {
		int k = 0;
		while (id[k] == texts[i]) { k++; i++; };
		if (texts[i] == ' ') { /* found */
			j = 0;
		} else {
			while (texts[i] != ' ') i++;
			i++; kw++;
		}
	}
	if (j != 0) kw = tk_none;
	return kw;
}
