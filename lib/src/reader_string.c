#include <assert.h>
#include "reader.h"
#include "reader_string.h"

static int peek_char(READER rdr)
{
	assert(rdr);

	READER_STRING rdr_str = (READER_STRING)rdr;

	int c = *rdr_str->string;
	return c ? c : -1;
}

static int read_char(READER rdr)
{
	assert(rdr);

	READER_STRING rdr_str = (READER_STRING)rdr;

	int c = *rdr_str->string;
	if (c == 0) return -1;
	rdr_str->string++;
	return c;
}

void rdr_str_init(READER_STRING rdr, char *string)
{
	assert(rdr);
	assert(string);

	rdr->reader.peek_char = peek_char;
	rdr->reader.read_char = read_char;
	rdr->string = string;
}
