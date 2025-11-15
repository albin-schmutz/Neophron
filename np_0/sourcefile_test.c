#include <assert.h>
#include <stdio.h>
#include "global.h"
#include "sourcefile.h"

static void test(void)
{
	struct SOURCEFILE sf;

	sf_open(&sf, "sourcefile_test.bin");

	assert(sf.ch == 'a');
	assert(sf.line == 1);
	assert(sf.pos == 1);

	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	assert(sf.line == 1);
	assert(sf.pos == 2);

	sf_fetch_char(&sf);
	assert(sf.ch == 'b');
	assert(sf.line == 2);
	assert(sf.pos == 1);

	sf_fetch_char(&sf);
	assert(sf.ch == 'b');
	assert(sf.line == 2);
	assert(sf.pos == 2);

	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	assert(sf.line == 2);
	assert(sf.pos == 3);

	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	assert(sf.line == 3);
	assert(sf.pos == 1);

	sf_fetch_char(&sf);
	assert(sf.ch == 'c');
	assert(sf.line == 4);
	assert(sf.pos == 1);

	sf_fetch_char(&sf);
	assert(sf.ch == 0xc3); /* ü */
	assert(sf.line == 4);
	assert(sf.pos == 2);

	sf_fetch_char(&sf);
	assert(sf.ch == 0xbc);
	assert(sf.line == 4);
	assert(sf.pos == 3);

	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	assert(sf.line == 4);
	assert(sf.pos == 4);

	sf_fetch_char(&sf);
	assert(sf.ch == ' ');
	assert(sf.line == 5);
	assert(sf.pos == 1);

	sf_fetch_char(&sf);
	assert(sf.ch == 'd');
	assert(sf.line == 5);
	assert(sf.pos == 2);

	sf_fetch_char(&sf);
	assert(sf.ch == 0xe7);
	assert(sf.line == 5);
	assert(sf.pos == 3);

	sf_fetch_char(&sf);
	assert(sf.ch == 0x99);
	assert(sf.line == 5);
	assert(sf.pos == 4);

	sf_fetch_char(&sf);
	assert(sf.ch == 0xbe);
	assert(sf.line == 5);
	assert(sf.pos == 5);

	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	assert(sf.line == 5);
	assert(sf.pos == 6);

	sf_fetch_char(&sf);

	INTEGER i = 0;
	while (i < 64) {
		INTEGER j = 0;
		while (j < 10) {
			assert(sf.ch == '0' + j);
			sf_fetch_char(&sf);
			j++;
		}
		j = 0;
		while (j < 26) {
			assert(sf.ch == 'A' + j);
			sf_fetch_char(&sf);
			j++;
		}
		j = 0;
		while (j < 26) {
			assert(sf.ch == 'a' + j);
			sf_fetch_char(&sf);
			j++;
		}
		assert(sf.ch == '!');
		sf_fetch_char(&sf);
		assert(sf.ch == '\n');
		sf_fetch_char(&sf);
		i++;
	}
	assert(sf.ch == 'H');
	sf_fetch_char(&sf);
	assert(sf.ch == 'a');
	sf_fetch_char(&sf);
	assert(sf.ch == 'l');
	sf_fetch_char(&sf);
	assert(sf.ch == 'l');
	sf_fetch_char(&sf);
	assert(sf.ch == 'o');
	sf_fetch_char(&sf);
	assert(sf.ch == '!');
	sf_fetch_char(&sf);
	assert(sf.ch == '\n');
	sf_fetch_char(&sf);
	assert(sf.ch == -1);

	sf_close(&sf);
}

int main(void)
{
	test();
	return 0;
}
