#include <assert.h>
#include <stdio.h>
#include "reader.h"
#include "reader_file.h"

static int peek_char(READER rdr)
{
	assert(rdr);

	READER_FILE rdr_file = (READER_FILE)rdr;

	if (!rdr_file->file) return -1;

	int c = fgetc(rdr_file->file);
	ungetc(c, rdr_file->file);
	return c;
}

static int read_char(READER rdr)
{
	assert(rdr);

	READER_FILE rdr_file = (READER_FILE)rdr;

	if (!rdr_file->file) return -1;

	return fgetc(rdr_file->file);
}

void rdr_file_init(READER_FILE rdr, char *filename)
{
	assert(rdr);
	assert(filename);

	rdr->reader.peek_char = peek_char;
	rdr->reader.read_char = read_char;
	rdr->file = fopen(filename, "rb");
}
