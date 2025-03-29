/*

Implements reader interface for files.

*/

typedef struct READER_FILE *READER_FILE;

struct READER_FILE {
	struct READER reader;
	FILE *file;
};

extern void rdr_file_init(READER_FILE rdr, char *filename);
