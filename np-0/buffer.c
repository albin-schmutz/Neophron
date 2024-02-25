#include <assert.h>
#include "global.h"
#include "buffer.h"

void buf_reset(BUFFER buf)
{
	assert(buf);
	buf->pos = 0;
	buf->limit = BUFFER_MAX_LEN;
}

void buf_flip(BUFFER buf)
{
	assert(buf);
	buf->limit = buf->pos;
	buf->pos = 0;
}

void buf_get_char(BUFFER buf, CHAR *c)
{
	assert(buf);
	assert(c);
	assert(buf->pos < buf->limit);
	*c = buf->data[buf->pos++];
}

void buf_put_char(BUFFER buf, CHAR c)
{
	assert(buf);
	assert(buf->pos < buf->limit);
	buf->data[buf->pos++] = c;
}

void buf_get_chars(BUFFER buf, INTEGER size, LONGINT *i)
{
	assert(buf);
	assert(size > 0);
	assert(size <= 8);
	assert(i);
	*i = 0;
	INTEGER end = buf->pos + size;
	assert(end <= buf->limit);
	while (buf->pos < end) {
		*i = (*i << 8) | buf->data[buf->pos++];
	}
}

void buf_put_chars(BUFFER buf, INTEGER size, LONGINT i)
{
	assert(buf);
	assert(size > 0);
	assert(size <= 8);
	INTEGER end = buf->pos + size;
	assert(end < buf->limit);
	while (end-- > buf->pos) {
		buf->data[end] = i & 255;
		i >>= 8;
	}
	buf->pos += size;
}

void buf_put_buffer(BUFFER buf, BUFFER buf2)
{
	assert(buf);
	assert(buf2);
	INTEGER pos = buf2->pos;
	while (buf->pos < buf->limit && pos < buf2->limit) {
		buf_put_char(buf, buf2->data[pos++]);
	}
}
