#define BUFFER_MAX_LEN 488

typedef struct BUFFER *BUFFER;

struct BUFFER {
	CHAR data[BUFFER_MAX_LEN];
	LONGINT ref[2];
	INTEGER pos;
	INTEGER limit;
};

extern void buf_reset(BUFFER buf);
extern void buf_flip(BUFFER buf);
extern void buf_get_char(BUFFER buf, CHAR *c);
extern void buf_put_char(BUFFER buf, CHAR c);
extern void buf_get_chars(BUFFER buf, INTEGER size, LONGINT *i);
extern void buf_put_chars(BUFFER buf, INTEGER size, LONGINT i);
extern void buf_put_buffer(BUFFER buf, BUFFER buf2);
