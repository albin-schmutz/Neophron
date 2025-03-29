#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "source.h"

void src_init(SOURCE src, READER rdr, char *error_format, char comment_marker[4])
{
	assert(src);
	assert(rdr);

	src->reader = rdr;
	src->last_eol = 0;
	src->peeked = -1;
	src->eol = 1;
	src->line = 0;
	strcpy(src->error_format, error_format);
	for (int i = 0; i < 4; ++i) {
		src->comment_marker[i] = comment_marker[i];
	}
}

void src_error(SOURCE src, char *msg)
{
	assert(src);
	assert(msg);

	fprintf(stderr, src->error_format, src->line, src->pos, msg);
	exit(1);
}

int src_peek_char(SOURCE src)
{
	assert(src);

	if (src->peeked != -1) return src->peeked;

	int c = src->reader->read_char(src->reader);
	if (c == '\n' || c == '\r') {
		int c2 = src->reader->peek_char(src->reader);
		if ((c2 == '\n' || c2 == '\r') && c2 != c) {
			src->reader->read_char(src->reader);
		}
		c = '\n';
	}
	src->peeked = c;
	return c;
}

static int read_char(SOURCE src)
{
	assert(src);

	if (src->eol) {
		src->line++;
		src->pos = 1;
		src->eol = 0;
	} else {
		src->pos++;
	}

	int c = src_peek_char(src);
	src->peeked = -1;

	if (c == '\n') {
		src->eol = 1;
	}

	return c;
}

static void check_eof(SOURCE src, int c)
{
	assert(src);

	if (c == -1) {
		src_error(src, "Unexpected eof");
	}
}

static int comment(SOURCE src, int level)
{
	assert(src);

	int c = read_char(src);
	check_eof(src, c);
	int ok = 1;
	while (ok) {
		if (c == src->comment_marker[2]) {
			c = read_char(src);
			check_eof(src, c);
			if (c == src->comment_marker[3]) {
				c = read_char(src);
				if (level > 0) check_eof(src, c);
				ok = 0;
			}
		} else if (c == src->comment_marker[0]) {
			c = read_char(src);
			check_eof(src, c);
			if (c == src->comment_marker[1]) c = comment(src, level + 1);
		} else {
			c = read_char(src);
			check_eof(src, c);
		}
	}
	return c;
}

int src_read_char(SOURCE src)
{
	assert(src);

	int c = read_char(src);
	if (c == src->comment_marker[0] && src_peek_char(src) == src->comment_marker[1]) {
		read_char(src);
		return comment(src, 0);
	}
	return c;
}
