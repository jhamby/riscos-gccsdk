
/*
 * help_cop.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _help_cop_h
#define _help_cop_h

#define CP_NUMBER(n) ((n)<< 8)
#define CP_DCODE(n)  ((n)<<20)
#define CP_RTRAN(n)  ((n)<<21)
#define CPDST_OP(n)  ((n)<<12)
#define CPRHS_OP(n)  ((n)<< 0)
#define CPLHS_OP(n)  ((n)<<16)
#define CP_INFO(n)   ((n)<< 5)

#define FROM_CP_BIT  (1<<20)

int help_copInt (int max, const char *msg);

WORD help_copAddr (WORD ir, BOOL stack);

#endif
