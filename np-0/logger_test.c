#include <assert.h>
#include "logger.h"

static void test_msg(void)
{
	log_init();
	log_msg(LOG_TEST_0, "Must not be logged");
	log_on_off(LOG_TEST_0, 1);
	log_msg(LOG_TEST_0, "%i Must be logged", 1);
	log_on_off(LOG_TEST_1, 0);
	log_msg(LOG_TEST_1, "Must not be logged");
	log_on_off(LOG_TEST_1, 1);
	log_msg(LOG_TEST_1, "%i Must be logged", 2);
}

static void test_dump(void)
{
	char bytes[] = {
		'\x00', '\x01', '\xf1', 'A', '\n', '\r', 'c', '!',
		'C', '@', 'x', '\x0b', '\xc0', '\xab', '\x67', '\x00',
		'\x00', '\x01', '\xf1', 'A', '\n', '\r', 'c', '!'
	};
	log_on_off(LOG_TEST_0, 1);
	log_dump(LOG_TEST_0, bytes, 24);
};

int main(int argc, char *argv[])
{
	test_msg();
	test_dump();
	return 0;
}
