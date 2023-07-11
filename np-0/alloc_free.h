/* allocation data structure for SHORTINT indexes starting with 0 */

typedef struct ALLOC_FREE *ALLOC_FREE;

struct ALLOC_FREE {
	INTEGER start;
	INTEGER size;
	INTEGER top;
	INTEGER free;
	INTEGER alloc;
};

extern void af_init(void);
extern void af_create(ALLOC_FREE af, INTEGER size);
extern void af_alloc(ALLOC_FREE af, SHORTINT *entry);
extern void af_free(ALLOC_FREE af, SHORTINT entry);
