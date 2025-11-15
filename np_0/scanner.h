/*

Tokenizes a sourcefile.

*/

typedef struct SCANNER *SCANNER;

struct SCANNER {
	struct SOURCEFILE sf;
	char module_name[32];
	char ident[32];
	LONGINT val;
	enum token tk;
	INTEGER line;
	INTEGER pos;
	INTEGER module_id;
	CHAR ch;
};

extern void scan_init_root(SCANNER scan);
extern void scan_init(SCANNER scan, SCANNER parent);
extern void scan_raise_error(SCANNER scan, char *msg);
extern void scan_take(SCANNER scan);
extern void scan_expect(SCANNER scan, enum token exp);
