#include <assert.h>
#include <stdio.h>
#include "reader.h"
#include "reader_string.h"
#include "reader_file.h"
#include "source.h"

static void test_string(void)
{
	struct READER_STRING rdr;
	struct SOURCE src;

	rdr_str_init(&rdr,
		"/* comment /* */ */\r"
		"\n"
		"a");
	src_init(&src, (READER)&rdr, "%i%i: %s\n", "/**/");

	assert(src_read_char(&src) == '\n');
	assert(src.line == 1);
	assert(src.pos == 20);
}

static void test_file(void)
{
	struct READER_FILE rdr;
	struct SOURCE src;
	char *filename = "source_test.bin";

	rdr_file_init(&rdr, filename);
	src_init(&src, (READER)&rdr, "%i%i: %s\n", "(**)");

	assert(src_read_char(&src) == 'a');
	assert(src.line == 1);
	assert(src.pos == 1);

	assert(src_read_char(&src) == '\n');
	assert(src.line == 1);
	assert(src.pos == 2);

	assert(src_read_char(&src) == 'b');
	assert(src.line == 2);
	assert(src.pos == 1);

	assert(src_read_char(&src) == 'b');
	assert(src.line == 2);
	assert(src.pos == 7);

	assert(src_read_char(&src) == '\n');
	assert(src.line == 2);
	assert(src.pos == 8);

	assert(src_read_char(&src) == '\n');
	assert(src.line == 3);
	assert(src.pos == 1);

	assert(src_read_char(&src) == 'c');
	assert(src.line == 4);
	assert(src.pos == 1);

	assert(src_read_char(&src) == 'c');
	assert(src.line == 4);
	assert(src.pos == 2);

	assert(src_read_char(&src) == 'c');
	assert(src.line == 4);
	assert(src.pos == 3);

	assert(src_read_char(&src) == '\n');
	assert(src.line == 4);
	assert(src.pos == 4);

	assert(src_read_char(&src) == ' ');
	assert(src.line == 5);
	assert(src.pos == 1);

	assert(src_read_char(&src) == 'd');
	assert(src.line == 5);
	assert(src.pos == 2);

	assert(src_read_char(&src) == '\n');
	assert(src.line == 5);
	assert(src.pos == 3);

	int c = src_read_char(&src);

	int i = 0;
	while (i < 64) {
		int j = 0;
		while (j < 10) {
			assert(c == '0' + j);
			c = src_read_char(&src);
			j++;
		}
		j = 0;
		while (j < 26) {
			assert(c == 'A' + j);
			c = src_read_char(&src);
			j++;
		}
		j = 0;
		while (j < 26) {
			assert(c == 'a' + j);
			c = src_read_char(&src);
			j++;
		}
		assert(c == '!');
		c = src_read_char(&src);
		assert(c == '\n');
		c = src_read_char(&src);
		i++;
	}
	assert(c == 'H');
	c = src_read_char(&src);
	assert(c == 'a');
	c = src_read_char(&src);
	assert(c == 'l');
	c = src_read_char(&src);
	assert(c == 'l');
	c = src_read_char(&src);
	assert(c == 'o');
	c = src_read_char(&src);
	assert(c == '!');
	c = src_read_char(&src);
	assert(c == '\n');
	c = src_read_char(&src);
	assert(c == -1);
}

int main(void)
{
	test_string();
	test_file();
	return 0;
}
