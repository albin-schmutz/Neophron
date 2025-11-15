/*

Provides big endian converters for 32 and 64 bit integers.

*/

extern INTEGER be_to_i32(CHAR b[]);
extern LONGINT be_to_i64(CHAR b[]);
extern void be_from_i32(CHAR b[], INTEGER i);
extern void be_from_i64(CHAR b[], LONGINT i);
