/*
 * get.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef get_header_included
#define get_header_included

#include "global.h"

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
