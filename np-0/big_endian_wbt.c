#include "big_endian.c"

static void test_get(void)
{
	CHAR B[] = {
		'\xf0', '\x00', '\xe1', '\x00', '\x44', '\xab', '\x67', '\x09'
	};
	assert(get(&B[0], 1) == 0xf0);
	assert(get(&B[1], 1) == 0x00);
	assert(get(&B[0], 2) == 0xf000);
	assert(get(&B[0], 3) == 0xf000e1);
	assert(get(&B[1], 7) == 0xe10044ab6709);
	assert(get(&B[0], 8) == 0xf000e10044ab6709);
}

static void test_put(void)
{
	CHAR b[8];
	put(&b[0], 1, 0x77);
	assert(b[0] == 0x77);
	put(&b[1], 2, 0xe451);
	assert(b[0] == 0x77);
	assert(b[1] == 0xe4);
	assert(b[2] == 0x51);
	put(&b[3], 5, 0x12345678ae);
	assert(b[0] == 0x77);
	assert(b[1] == 0xe4);
	assert(b[2] == 0x51);
	assert(b[3] == 0x12);
	assert(b[4] == 0x34);
	assert(b[5] == 0x56);
	assert(b[6] == 0x78);
	assert(b[7] == 0xae);
}

int main(void)
{
	test_get();
	test_put();
	return 0;
}
