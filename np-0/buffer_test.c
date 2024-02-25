#include <assert.h>
#include "global.h"
#include "buffer.h"

static void test_size(void)
{
	assert(sizeof(struct BUFFER) == 512);
}

static void test_put_get_char(void)
{
	struct BUFFER buf;
	CHAR ch;

	buf_reset(&buf);
	assert(buf.pos == 0);
	assert(buf.pos < buf.limit);
	buf_put_char(&buf, 'A');
	assert(buf.pos == 1);
	assert(buf.pos < buf.limit);
	buf_put_char(&buf, 'B');
	assert(buf.pos == 2);
	assert(buf.pos < buf.limit);

	buf_flip(&buf);
	assert(buf.pos == 0);
	assert(buf.limit == 2);
	buf_get_char(&buf, &ch);
	assert(ch == 'A');
	assert(buf.pos < buf.limit);
	buf_get_char(&buf, &ch);
	assert(ch == 'B');
	assert(buf.pos == buf.limit);
}

static void test_put_get_chars(void)
{
	struct BUFFER buf;
	LONGINT i;

	buf_reset(&buf);
	buf_put_char(&buf, 0x22);
	buf_put_chars(&buf, 5, 0x01234567);
	assert(buf.pos == 6);
	buf_flip(&buf);
	buf_get_chars(&buf, 6, &i);
	assert(i == 0x220001234567);

	buf_reset(&buf);
	buf_put_chars(&buf, 4, -848374958); /* FFFFFFFFCD6ED352 */
	buf_flip(&buf);
	buf_get_chars(&buf, 4, &i);
	assert(i == 0x0cd6ed352);

	buf_reset(&buf);
	buf_put_chars(&buf, 4, 1);
	buf_put_chars(&buf, 4, 0);
	buf_put_chars(&buf, 4, -1);
	buf_flip(&buf);
	buf_get_chars(&buf, 4, &i);
	assert(i == 1);
	buf_get_chars(&buf, 4, &i);
	assert(i == 0);
	buf_get_chars(&buf, 4, &i);
	assert(i == 0x0ffffffff);

	buf_reset(&buf);
	buf_put_chars(&buf, 4, 0x01234567);
	buf_put_chars(&buf, 4, 0x9abcdef0);
	buf_flip(&buf);
	buf_get_chars(&buf, buf.limit, &i);
	assert(i == 0x012345679abcdef0);
}

static void test_put_buffer(void)
{
	struct BUFFER buf;
	struct BUFFER buf2;
	LONGINT i;

	buf_reset(&buf2);
	buf_put_chars(&buf2, 6, 0x48616c6c6f00);
	buf_flip(&buf2);

	buf_reset(&buf);
	buf_put_char(&buf, 0xff);
	buf_put_buffer(&buf, &buf2);
	assert(buf.pos == 7);
	buf_flip(&buf);
	buf_get_chars(&buf, 7, &i);
	assert(i == 0x0ff48616c6c6f00);
}

int main(void)
{
	test_size();
	test_put_get_char();
	test_put_get_chars();
	test_put_buffer();
	return 0;
}
