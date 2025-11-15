#include <assert.h>
#include "global.h"

int main(void)
{
	assert(sizeof(CHAR) == 1);
	assert(sizeof(SHORTINT) == 2);
	assert(sizeof(INTEGER) == 4);
	assert(sizeof(LONGINT) == 8);
	return 0;
}
