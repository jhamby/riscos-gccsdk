
/*
 * get.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _get_h
#define _get_h

#ifndef _global_h
#include "global.h"
#endif


WORD getCpuReg (void);
WORD getFpuReg (void);
WORD getCopReg (void);
WORD getCopNum (void);
WORD getRhs (BOOL immonly, BOOL shift, WORD ir);


#define DST_OP(op) ((op)<<12)
#define LHS_OP(op) ((op)<<16)
#define RHS_OP(op) ( op     )

#define DST_MUL(r) ((r)<<16)
#define LHS_MUL(r) ( r     )
#define RHS_MUL(r) ((r)<< 8)
#define ACC_MUL(r) ((r)<<12)

#define BASE_MULTI(r) ((r)<<16)

#define IMM_RHS   0x02000000

#endif
