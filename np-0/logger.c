#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "logger.h"

static uint8_t bit_array[MAX_ID / 8 + 1];

void log_init(void)
{
	for (int i = 0; i < MAX_ID; i++) {
		log_on_off(i, 0);
	}
}

void log_on_off(log_id id, int on_off)
{
	if (id < MAX_ID) {
		if (on_off) {
			bit_array[id / 8] |= (1 << id % 8);
		} else {
			bit_array[id / 8] &= (1 << id % 8);
		}
    }
}

void log_msg(log_id id, const char *format, ...)
{
	if (id >= MAX_ID || !(bit_array[id / 8] & (1 << id % 8))) {
		return;
    }
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void log_dump(log_id id, char *bytes, int bytes_n)
{
	if (id >= MAX_ID || !(bit_array[id / 8] & (1 << id % 8))) {
		return;
    }
	char dump[80];
	char *hex_digits = "0123456789ABCDEF";
	for (int i = 0; i < 80; ++i) dump[i] = ' ';
	int dump_ix = 0;
	int i16 = 0;
	for (int bytes_ix = 0; bytes_ix < bytes_n; ++bytes_ix) {
		int v = (unsigned char)bytes[bytes_ix];
		dump[dump_ix + i16 * 3] = hex_digits[v >> 4];
		dump[dump_ix + i16 * 3 + 1] = hex_digits[v & 0x0f];
		dump[dump_ix + i16 + 48] = v >= 32 && v < 128 ? (char)v : '.';
		i16++;
		if (i16 == 16) {
			dump_ix += i16 + 48;
			dump[dump_ix++] = '\n';
			dump[dump_ix] = 0;
			fprintf(stderr, "%s", dump);
			for (int i = 0; i < 80; ++i) dump[i] = ' ';
			dump_ix = 0;
			i16 = 0;
		}
	}
	if (i16 != 0) {
		dump_ix += i16 + 48;
		dump[dump_ix++] = '\n';
		dump[dump_ix] = 0;
		fprintf(stderr, "%s", dump);
	}
}
