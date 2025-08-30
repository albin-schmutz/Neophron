/*

Tokenizes a sourcefile.

*/

typedef struct SCANNER *SCANNER;

struct SCANNER {
	struct SOURCEFILE sf;
	char module[32];
	char ident[32];
	int64_t val;
	enum token tk;
	int line;
	int pos;
	int module_id;
	char ch;
};

extern void scan_init_root(SCANNER scan);
extern void scan_init(SCANNER scan, SCANNER parent);
extern void scan_raise_error(SCANNER scan, char *msg);
extern void scan_take(SCANNER scan);
extern void scan_expect(SCANNER scan, enum token exp);
