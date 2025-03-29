/*

Base interface for character based readers.

*/

typedef struct READER *READER;

typedef int (*char_reader)(READER rdr);

struct READER {
	char_reader peek_char;
	char_reader read_char;
};
