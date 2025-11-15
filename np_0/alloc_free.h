/*

An API that allocates and releases numbers starting with 0.
Can be used for allocating and releasing array entries.

*/

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
extern void af_alloc(ALLOC_FREE af, INTEGER *entry);
extern void af_free(ALLOC_FREE af, INTEGER entry);
