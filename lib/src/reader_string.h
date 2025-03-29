/*

Implements reader interface for strings.

*/

typedef struct READER_STRING *READER_STRING;

struct READER_STRING {
	struct READER reader;
	char *string;
};

extern void rdr_str_init(READER_STRING rdr, char *string);
