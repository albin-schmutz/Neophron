/*

Implements symbol tables for types, symbols and modules.

*/

#define MAX_TYPES 90
#define MAX_SYMBOLS 900

#define FORM_BOOL 1
#define FORM_CHAR 2
#define FORM_INT 3
#define FORM_ARRAY 10
#define FORM_RECORD 11

typedef struct TYPE *TYPE;

struct TYPE {
	INTEGER size;
	INTEGER length; /* array length */
	INTEGER form;
	INTEGER owner; /* symbol type */
	INTEGER base; /* array base (TYPE) or record fields (SYMBOL) */
};

typedef struct SYMBOL *SYMBOL;

struct SYMBOL {
	char name[32];
	LONGINT val;
	INTEGER val2;
	INTEGER val3;
	INTEGER next;
	INTEGER clazz;
	INTEGER scope; /*
		CLASS_SCOPE: parent scope
		CLASS_IMPORT: imported module scope
		CLASS_PROC: procedure scope
		other: owner module */
	INTEGER type;
	INTEGER level;
	BOOLEAN is_public;
};

extern INTEGER tbl_level;

extern INTEGER tbl_boolean_type;
extern INTEGER tbl_char_type;
extern INTEGER tbl_short_int_type;
extern INTEGER tbl_integer_type;
extern INTEGER tbl_long_int_type;

extern struct TYPE tbl_types[MAX_TYPES];
extern struct SYMBOL tbl_symbols[MAX_SYMBOLS];

extern INTEGER tbl_marker;
extern INTEGER tbl_top_scope;

extern void tbl_alloc_type(INTEGER *t);
extern void tbl_alloc_symbol(INTEGER *s);
extern void tbl_init(void);
extern TYPE tbl_create_type(INTEGER *t, INTEGER form, INTEGER size);
extern void tbl_set_marker(char *id);
extern void tbl_find_symbol(INTEGER *s);
extern void tbl_open_scope();
extern void tbl_dealloc_scope(INTEGER scope, BOOLEAN private_only);
extern void tbl_close_scope(void);
extern SYMBOL tbl_def_symbol(INTEGER *s, INTEGER clazz, INTEGER m);
extern SYMBOL tbl_use_symbol(INTEGER *s, INTEGER imp);
extern INTEGER tbl_open_module(char *name);
