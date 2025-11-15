#include <assert.h>
#include "global.h"
#include "big_endian.h"

static LONGINT get(CHAR b[], INTEGER size)
{
	assert(size > 0);
	assert(size <= 8);

	LONGINT result = 0;
	INTEGER pos = 0;
	while (pos < size) {
		result = (result << 8) | (CHAR)b[pos++];
	}
	return result;
}

static void put(CHAR b[], INTEGER size, LONGINT i)
{
	assert(size > 0);
	assert(size <= 8);
	INTEGER pos = size;
	while (pos--) {
		b[pos] = i & 255;
		i >>= 8;
	}
}

INTEGER be_to_i32(CHAR b[])
{
	return get(b, 4);
}

LONGINT be_to_i64(CHAR b[])
{
	return get(b, 8);
}

void be_from_i32(CHAR b[], INTEGER i)
{
	put(b, 4, i);
}

void be_from_i64(CHAR b[], LONGINT i)
{
	put(b, 8, i);
}
