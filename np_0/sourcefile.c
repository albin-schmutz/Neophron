#include <assert.h>
#include <stdio.h>
#include "global.h"
#include "sourcefile.h"

void sf_fetch_char(SOURCEFILE sf)
{
	assert(sf);
	assert(sf->file);
	assert(sf->ch != -1);

	if (sf->last_eol == -1) {
		sf->last_eol = 0;
	} else if (sf->eol) {
		sf->line++;
		sf->pos = 1;
		sf->eol = 0;
	} else {
		sf->pos++;
	}
	sf->ch = fgetc(sf->file);
	if (sf->ch != -1) {
		if (sf->ch == '\n' || sf->ch == '\r') {
			if (sf->last_eol == 0) {
				sf->last_eol = sf->ch;
				sf->ch = '\n';
				sf->eol = 1;
			} else if (sf->last_eol == sf->ch) {
				sf->ch = '\n';
				sf->eol = 1;
			} else {
				sf->last_eol = -1;
				sf_fetch_char(sf);
			}
		} else if (sf->last_eol != 0) {
			sf->last_eol = 0;
		}
	}
}

void sf_open(SOURCEFILE sf, char *filename)
{
	assert(sf);
	assert(filename);

	sf->file = fopen(filename, "rb");
	if (sf->file) {
		sf->last_eol = 0; sf->eol = 1; sf->line = 0;
		sf_fetch_char(sf);
	} else {
		sf->ch = -1;
	}
}

void sf_close(SOURCEFILE sf)
{
	assert(sf);

	if (sf->file) {
		fclose(sf->file);
	}
}
