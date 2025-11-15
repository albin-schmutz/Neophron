/*

Executes emitted opcodes.
- Provides 16 64-bit registers.
- Needs a modulename_provider for the ASSERT sys call

*/

extern LONGINT reg[16];

typedef char *(*modulename_provider)(void);

extern void cpu_decode_instr(OPCODE opc);
extern void cpu_execute(INTEGER entry_point, modulename_provider find_modulename);
