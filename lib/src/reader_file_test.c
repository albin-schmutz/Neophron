#include <assert.h>
#include <stdio.h>
#include "reader.h"
#include "reader_file.h"

static void test(void)
{
	struct READER_FILE rdr_file;
	READER rdr = (READER)&rdr_file;
	char *filename = "tmp_reader_file_test.txt";

	remove(filename);

	rdr_file_init(&rdr_file, filename);
	assert(-1 == rdr->peek_char(rdr));
	assert(-1 == rdr->read_char(rdr));

	FILE *f = fopen(filename, "wb");
	assert(f);
	fputc('f', f);
	fputc('o', f);
	fputc('o', f);
	fclose(f);

	rdr_file_init(&rdr_file, filename);

	assert('f' == rdr->peek_char(rdr));
	assert('f' == rdr->read_char(rdr));
	assert('o' == rdr->read_char(rdr));
	assert('o' == rdr->peek_char(rdr));
	assert('o' == rdr->read_char(rdr));
	assert(-1 == rdr->peek_char(rdr));
	assert(-1 == rdr->read_char(rdr));
}

int main(void)
{
	test();
	return 0;
}

