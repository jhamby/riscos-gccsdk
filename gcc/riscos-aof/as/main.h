#ifndef main_header_included
#define main_header_included

#include <setjmp.h>

/* AS options :
 */
extern int verbose;
extern int pedantic;
extern int fussy;
extern int throwback;
extern int dde;
extern int autocast;
extern int align;
extern int gcc_backend;
extern int gas_backend;
extern int local;
extern int objasm;
extern int uc;
extern int apcs_32bit;
extern int apcs_fpv3;
extern int elf;

extern const char *ProgName;

extern jmp_buf asmContinue;
extern BOOL asmContinueValid;
extern jmp_buf asmAbort;
extern BOOL asmAbortValid;

#endif
