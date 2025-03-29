#include <assert.h>
#include "reader.h"
#include "reader_string.h"

static void test(void)
{
	struct READER_STRING rdr_str;
	READER rdr = (READER)&rdr_str;

	rdr_str_init(&rdr_str, "foo");

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
