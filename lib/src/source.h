/*

Reads sources like files or strings.
- Tracks line and position.
- Can handle mixed end-of-lines.
- Skip comments inside start/end marker with length of 2.
- Can raise exception.

*/

typedef struct SOURCE *SOURCE;

struct SOURCE {
	READER reader;
	char error_format[64];
	int last_eol;
	int peeked;
	int eol;
	int line;
	int pos;
	char comment_marker[4];
};

extern void src_init(SOURCE src, READER rdr, char *error_format, char comment_marker[4]);
extern void src_error(SOURCE src, char *msg);
extern int src_peek_char(SOURCE src);
extern int src_read_char(SOURCE src);
