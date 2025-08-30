/*

Provides a reader for source files.
- Tracks line and position.
- Can handle mixed end-of-lines.

*/

typedef struct SOURCEFILE *SOURCEFILE;

struct SOURCEFILE {
	FILE *file;
	int line, pos;
	int last_eol;
	int ch;
	int eol;
};

extern void sf_open(SOURCEFILE sf, char *filename);
extern void sf_close(SOURCEFILE sf);
extern void sf_fetch_char(SOURCEFILE sf);
