/* -> $.ARMemu.C.ARMemu

   ARM2 emulator v1.00

   (C) Edwin Dorre 1990
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>

#ifdef P_PC
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#endif

#include "armhdr.h"
#include "armrom.h"
#ifdef NWFPE
#include "fpa11.h"
#else
#include "armfpe.h"
#endif

static void initialise_armrom (void);
static void initialise_fpe (void);

WORD rg[16], r15, r8fiq, r9fiq, r10fiq, r11fiq, r12fiq, r13fiq, r14fiq,
  r13irq, r14irq, r13svc, r14svc;

volatile int IRQint, FIQint;

int PCwrite;
WORD HUGE *wmem;
BYTE HUGE *bmem;

jmp_buf exception;

WORD *arm_regs[4][16] =
{
  &rg[0], &rg[1], &rg[2], &rg[3], &rg[4], &rg[5], &rg[6], &rg[7],
  &rg[8], &rg[9], &rg[10], &rg[11], &rg[12], &rg[13], &rg[14], &r15,

  &rg[0], &rg[1], &rg[2], &rg[3], &rg[4], &rg[5], &rg[6], &rg[7],
  &r8fiq, &r9fiq, &r10fiq, &r11fiq, &r12fiq, &r13fiq, &r14fiq, &r15,

  &rg[0], &rg[1], &rg[2], &rg[3], &rg[4], &rg[5], &rg[6], &rg[7],
  &rg[8], &rg[9], &rg[10], &rg[11], &rg[12], &r13irq, &r14irq, &r15,

  &rg[0], &rg[1], &rg[2], &rg[3], &rg[4], &rg[5], &rg[6], &rg[7],
  &rg[8], &rg[9], &rg[10], &rg[11], &rg[12], &r13svc, &r14svc, &r15,
};

int num_of_one_bits[256] =
{
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

unsigned int total_instructions;
clock_t execution_start;

/*****************************************************************************
 Read/Write Byte/Word MEMC routines
 *****************************************************************************/

WORD 
rwmem (register WORD a)
{
  if (a < (ROM >> 2))
    {
      a = wmem[a];
      return (SWAPWORD (a));
    }

  if ((a << 2) > 0x3ffffff)
    longjmp (exception, VECTOR_ADDRESS_EXCEPTION);
  else
    longjmp (exception, VECTOR_ABORT_DATA);
  return (0);
}

BYTE 
rbmem (register WORD a)
{
  if (a < ROM)
    return (bmem[a]);

  if ((a << 2) > 0x3ffffff)
    longjmp (exception, VECTOR_ADDRESS_EXCEPTION);
  else
    longjmp (exception, VECTOR_ABORT_DATA);
  return (0);
}

void 
wwmem (register WORD a, register WORD b)
{
  /* if ((a) < (RAM >> 2)) */
  if (a < (MEMSIZE >> 2))
    wmem[a] = SWAPWORD (b);
  else
    {
      if ((a << 2) > 0x3ffffff)
	longjmp (exception, VECTOR_ADDRESS_EXCEPTION);
      else
	longjmp (exception, VECTOR_ABORT_DATA);
    }
}

void 
wbmem (register WORD a, register WORD b)
{
  /* if (a < RAM) */
  if (a < MEMSIZE)
    bmem[a] = b;
  else
    {
      if ((a << 2) > 0x3ffffff)
	longjmp (exception, VECTOR_ADDRESS_EXCEPTION);
      else
	longjmp (exception, VECTOR_ABORT_DATA);
    }
}

/*****************************************************************************
 updR_CZN: update register rd with setting of flags CZN - AND R0,R1,R2
 *****************************************************************************/

#define updR_CZN  if (ins & 0x100000)     /* If S bit set */ \
                  { if (rd == 15)                                             \
                    { if (r15&3) r15 = rn ;                                   \
                      else r15 = (r15 & 0x0c000003) | (rn & 0xf3fffffc) ;     \
                      PCwrite = TRUE ;                                        \
                    }                                                         \
                    else { r(rd) = rn ;                                       \
                           r15 = (r15 & 0x1fffffff) | (c << C_SHFT) |         \
				 (rn  & 0x80000000) | ((long)(rn == 0) << Z_SHFT) ; \
			 }                                                    \
		  } else                                                      \
		  { if (rd == 15)                                             \
		    { r15 = (r15 & 0xfc000003) | (rn & 0x03fffffc) ;          \
		      PCwrite = TRUE ;                                        \
		    }                                                         \
		    else r(rd) = rn ;                                         \
		  }

/*****************************************************************************
 updR_CZNV: update register rd with setting of flags CZNV - ADD R0,R1,R2
 *****************************************************************************/

#define updR_CZNV if (ins & 0x100000)     /* If S bit set */                  \
                  { if (rd == 15)                                             \
                    { if (r15&3) r15 = rn ;                                   \
                      else r15 = (r15 & 0x0c000003) | (rn & 0xf3fffffc) ;     \
                      PCwrite = TRUE ;                                        \
                    }                                                         \
                    else { r(rd) = rn ;                                       \
                           r15 = (r15 & 0x0fffffff) | (c << C_SHFT) |         \
                                 (v << V_SHFT) | (rn  & 0x80000000) |         \
				 ((long)(rn == 0) << Z_SHFT) ;                      \
			 }                                                    \
		  } else                                                      \
		  { if (rd == 15)                                             \
		    { r15 = (r15 & 0xfc000003) | (rn & 0x03fffffc) ;          \
		      PCwrite = TRUE ;                                        \
		    }                                                         \
		    else r(rd) = rn ;                                         \
		  }

/*****************************************************************************
 updF_CZN: update flags CZN - TEQ R0,R1
 *****************************************************************************/

#define updF_CZN  if (ins & 0x100000)    /* If S bit set */                   \
                  { if (rd == 15)                                             \
                    { if (r15&3) r15 = (r15 & 0x03fffffc) | (rn & 0xfc000003);\
                      else r15 = (r15 & 0x0fffffff) | (rn & 0xf0000000) ;     \
                    }                                                         \
                    else r15 = (r15 & 0x1fffffff) | (c << C_SHFT) |           \
			       (rn  & 0x80000000) | ((long)(rn == 0) << Z_SHFT) ;   \
		  }

/*****************************************************************************
 updF_CZNV: update flags CZNV - CMP R0,R1
 *****************************************************************************/

#define updF_CZNV if (ins & 0x100000)    /* If S bit set */                   \
                  { if (rd == 15)                                             \
                    { if (r15&3) r15 = (r15 & 0x03fffffc) | (rn & 0xfc000003);\
                      else r15 = (r15 & 0x0fffffff) | (rn & 0xf0000000) ;     \
                    }                                                         \
                    else r15 = (r15 & 0x0fffffff) | (c << C_SHFT) |           \
                               (v << V_SHFT) | (rn  & 0x80000000) |           \
			       ((long)(rn == 0) << Z_SHFT) ;                        \
		  }

/*****************************************************************************
 do_operator: decode a data operation operator and execute it
 *****************************************************************************/

#define do_operator                                                           \
switch((ins >> 21) & 0xf)                                                     \
{                                                                             \
  case  0: rn = rn & rm ;                                 updR_CZN  ; break ; \
  case  1: rn = rn ^ rm ;                                 updR_CZN  ; break ; \
  case  2: c = (rn >= rm) ;                                                   \
           v = c ^ ((rn & 0x7fffffff) >= (rm &0x7fffffff)) ;                  \
           rn -= rm ;                                     updR_CZNV ; break ; \
  case  3: c = rm >= rn ;                                                     \
           v = c ^ ((rm & 0x7fffffff) >= (rn &0x7fffffff)) ;                  \
           rn  = rm - rn ;                                updR_CZNV ; break ; \
  case  4: c = (0xffffffff - rn) < rm ;                                       \
           v = c ^ ((0x7fffffff - (rn & 0x7fffffff)) < (rm & 0x7fffffff)) ;   \
           rn += rm ;                                     updR_CZNV ; break ; \
  case  5: rm += C_FLAG ;                                                     \
           c = (0xffffffff - rn) < rm ;                                       \
           v = c ^ ((0x7fffffff - (rn & 0x7fffffff)) < (rm & 0x7fffffff)) ;   \
           rn += rm ;                                     updR_CZNV ; break ; \
  case  6: rm += !C_FLAG ;                                                    \
           c = (rn >= rm) ;                                                   \
           v = c ^ ((rn & 0x7fffffff) >= (rm &0x7fffffff)) ;                  \
           rn -= rm ;                                     updR_CZNV ; break ; \
  case  7: rn += !C_FLAG ;                                                    \
           c = rm>=rn ;                                                       \
           v = c ^ ((rm & 0x7fffffff) >= (rn & 0x7fffffff)) ;                 \
           rn = rm - rn ;                                 updR_CZNV ; break ; \
  case  8: rn = rn & rm ;                                 updF_CZN  ; break ; \
  case  9: rn = rn ^ rm ;                                 updF_CZN  ; break ; \
  case 10: c = rn>=rm ;                                                       \
           v = c ^ ((rn & 0x7fffffff) >= (rm & 0x7fffffff)) ;                 \
           rn = rn - rm ;                                 updF_CZNV ; break ; \
  case 11: c = (0xffffffff - rn) < rm ;                                       \
           v = c ^ ((0x7fffffff - (rn & 0x7fffffff)) < (rm & 0x7fffffff)) ;   \
           rn = rn + rm ;                                 updF_CZNV ; break ; \
  case 12: rn = rn | rm    ;                              updR_CZN  ; break ; \
  case 13: rn =      rm    ;                              updR_CZN  ; break ; \
  case 14: rn = rn & (~rm) ;                              updR_CZN  ; break ; \
  case 15: rn = ~rm        ;                              updR_CZN  ; break ; \
}

/*****************************************************************************
 oper_shift_con: Operator with constant shift ADD R0,R1,R2,ASL #5
 *****************************************************************************/

void 
oper_shift_con (register WORD ins)
{
  register WORD rn, ty, sh, rm, c, v, rd;

  rm = r (ins & 0x00f);
  sh = (ins & 0xf80) >> 7;
  ty = (ins & 0x060) >> 5;

  switch (ty)
    {
    case 0:
      if (sh)			/* ASL LSL */
	{
	  c = (rm >> (32 - sh)) & 1;
	  rm = rm << sh;
	}
      else
	c = C_FLAG;
      break;

    case 1:
      if (sh == 0)
	sh = 32;		/* LSR */
      c = (rm >> (sh - 1)) & 1;
      rm = rm >> sh;
      break;

    case 2:
      if (sh == 0)
	sh = 32;		/* ASR */
      c = (rm >> (sh - 1)) & 1;
      rm = (rm >> sh) |
	(((rm & 0x80000000) ? 0xffffffff : 0) << (32 - sh));
      break;

    case 3:
      if (sh)			/* ROR */
	{
	  c = (rm >> (sh - 1)) & 1;
	  rm = (rm >> sh) | ((rm & (0xffffffff >> (32 - sh))) << (32 - sh));
	}
      else
	/* RRX */
	{
	  c = rm & 1;
	  rm = (C_FLAG << 31) | (rm >> 1);
	}
      break;
    }

  rn = (ins & 0xf0000) >> 16;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r (rn);

  rd = (ins & 0x0f000) >> 12;

  do_operator;
}

/*****************************************************************************
 oper_shift_reg: Operator with register shift ADD R0,R1,R2,ASL R3
 *****************************************************************************/

void 
oper_shift_reg (register WORD ins)
{
  register WORD rn, ty, sh, rm, c, v, rd, rs;

  rm = ins & 0x0000f;
  if (rm == 15)
    rm = addPC (r15, 4);
  else
    rm = r (rm);

  rs = (ins & 0xf00) >> 8;
  if (rs == 15)
    sh = r15 & 0xfc;
  else
    sh = r (rs) & 0xff;

  ty = (ins & 0x060) >> 5;

  if (sh)
    {
      switch (ty)
	{
	case 0:
	  if (sh <= 32)		/* ASL LSL */
	    {
	      c = (rm >> (32 - sh)) & 1;
	      rm = rm << sh;
	    }
	  else
	    rm = c = 0;
	  break;

	case 1:
	  if (sh <= 32)		/* LSR */
	    {
	      c = (rm >> (sh - 1)) & 1;
	      rm = rm >> sh;
	    }
	  else
	    rm = c = 0;
	  break;

	case 2:
	  if (sh >= 32)
	    sh = 32;		/* ASR */
	  c = (rm >> (sh - 1)) & 1;
	  rm = (rm >> sh) |
	    (((rm & 0x80000000) ? 0xffffffff : 0) << (32 - sh));
	  break;

	case 3:
	  while (sh > 32)
	    sh -= 32;		/* ROR */
	  c = (rm >> (sh - 1)) & 1;
	  rm = (rm >> sh) | ((rm & (0xffffffff >> (32 - sh))) << (32 - sh));
	  break;
	}
    }
  else
    c = C_FLAG;

  rn = (ins & 0xf0000) >> 16;
  if (rn == 15)
    rn = addPC (r15 & 0x03fffffc, 4);
  else
    rn = r (rn);

  rd = (ins & 0x0f000) >> 12;

  do_operator;
}

/*****************************************************************************
 oper_constant: Operator with constant ADD R0,R1,#123
 *****************************************************************************/

void 
oper_constant (register WORD ins)
{
  register WORD rn, sh, rm, c, v, rd;

  rm = ins & 0x0ff;
  sh = (ins & 0xf00) >> 7;

  if (sh)
    {
      c = (rm >> (sh - 1)) & 1;	/* ROR */
      rm = (rm >> sh) | ((rm & (0xffffffff >> (32 - sh))) << (32 - sh));
    }
  else
    c = 0;

  rn = (ins & 0xf0000) >> 16;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r (rn);

  rd = (ins & 0x0f000) >> 12;

  do_operator;
}

/*****************************************************************************
 multiply: Multiply (with add) MUL R0,R1,R2
 *****************************************************************************/

void 
multiply (register WORD ins)
{
  register WORD rd, rn, rs, rm;

  rd = (ins >> 16) & 0xf;

  if (rd != 15)
    {
      rm = ins & 0xf;
      if (rm == 15)
	rm = addPC (r15, 4);
      else
	rm = r (rm);

      rs = (ins >> 8) & 0xf;
      if (rs == 15)
	rs = r15 & 0x03fffffc;
      else
	rs = r (rs);

      if (ins & 0x200000)	/* If add bit set */
	{
	  rn = r ((ins >> 12) & 0xf);
	  rn = (rm * rs) + rn;
	}
      else
	rn = rm * rs;

      r (rd) = rn;

      if (ins & 0x100000)	/* If S bit set */
	r15 = (r15 & 0x1fffffff) | (rn & 0x80000000) | ((rn == 0) << Z_SHFT);
    }
}

/*****************************************************************************
 ldr_constant: Load/store single with constant LDR R0,[R1,#1234]
 *****************************************************************************/

void 
ldr_constant (register WORD ins)
{
  register WORD rn, rd, offset, base, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = ins & 0xfff;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    {
      if (!L_PRE_INDEX && L_WRITEBACK)
	base = *arm_regs[USR_MODE][rn]; /* Trans bit is set.  */
      else
	base = r (rn);
    }

  if (L_PRE_INDEX)
    {
      if (L_UP)
	base += offset;
      else
	base -= offset;
    }

  if (L_BYTE)
    {
      if (L_LOAD)
	{
	  if (rd == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rd) = rbmem (base);
	}
      else
	{
	  if (rd == 15)
	    wbmem (base, addPC (r15, 4) & 0xff);
	  else
	    wbmem (base, r (rd) & 0xff);
	}
    }
  else
    {
      if (L_LOAD)
	{
	  word = rwmem (base >> 2);
	  if (base & 3)
	    {
	      sh = (base & 3) * 8;
	      word = (word >> sh) | ((word & (0xffffffff >> (32 - sh))) << (32 - sh));
	    }

	  if (rd == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rd) = word;
	}
      else
	{
	  if (rd == 15)
	    wwmem (base >> 2, addPC (r15, 4));
	  else
	    wwmem (base >> 2, r (rd));
	}
    }

  if ((rn != rd) || !L_LOAD)
    {
      if (!L_PRE_INDEX || L_WRITEBACK)
	{
	  if (!L_PRE_INDEX)
	    {
	      if (L_UP)
		base += offset;
	      else
		base -= offset;
	    }

	  if (rn == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (base & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rn) = base;
	}
    }
}

/*****************************************************************************
 ldr_shift: Load/store single with register shift LDR R0,[R1,R2,ASL #15]
 *****************************************************************************/

void 
ldr_shift (register WORD ins)
{
  register WORD rn, rd, offset, base, word, sh, ty;

  rd = (ins & 0x0f000) >> 12;

  offset = r (ins & 0x00f);
  sh = (ins & 0xf80) >> 7;
  ty = (ins & 0x060) >> 5;

  switch (ty)
    {
    case 0:
      offset = offset << sh;	/* ASL LSL */
      break;

    case 1:
      if (sh == 0)
	sh = 32;		/* LSR */
      offset = offset >> sh;
      break;

    case 2:
      if (sh == 0)
	sh = 32;		/* ASR */
      offset = (offset >> sh) |
	(((offset & 0x80000000) ? 0xffffffff : 0) << (32 - sh));
      break;

    case 3:
      if (sh)			/* ROR */
	offset = (offset >> sh) |
	  ((offset & (0xffffffff >> (32 - sh))) << (32 - sh));
      else			/* RRX */
	offset = (C_FLAG << 31) | (offset >> 1);
      break;
    }

  rn = (ins & 0xf0000) >> 16;
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    {
      if (!L_PRE_INDEX && L_WRITEBACK)
	base = *arm_regs[USR_MODE][rn];
      else
	base = r (rn);
    }

  if (L_PRE_INDEX)
    {
      if (L_UP)
	base += offset;
      else
	base -= offset;
    }

  if (L_BYTE)
    {
      if (L_LOAD)
	{
	  if (rd == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rd) = rbmem (base);
	}
      else
	{
	  if (rd == 15)
	    wbmem (base, addPC (r15, 4) & 0xff);
	  else
	    wbmem (base, r (rd) & 0xff);
	}
    }
  else
    {
      if (L_LOAD)
	{
	  word = rwmem (base >> 2);
	  if (base & 3)
	    {
	      sh = (base & 3) * 8;
	      word = (word >> sh) | ((word & (0xffffffff >> (32 - sh))) << (32 - sh));
	    }

	  if (rd == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rd) = word;
	}
      else
	{
	  if (rd == 15)
	    wwmem (base >> 2, addPC (r15, 4));
	  else
	    wwmem (base >> 2, r (rd));
	}
    }

  if ((rd != rn) || !L_LOAD)
    {
      if (!L_PRE_INDEX || L_WRITEBACK)
	{
	  if (!L_PRE_INDEX)
	    {
	      if (L_UP)
		base += offset;
	      else
		base -= offset;
	    }

	  if (rn == 15)
	    {
	      r15 = (r15 & 0xfc000003) | (base & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  else
	    r (rn) = base;
	}
    }
}

/*****************************************************************************
 undefined: undefined instruction
 *****************************************************************************/

void 
undefined (register WORD ins)
{
#ifdef NWFPE
  *arm_regs[SVC_MODE][14] = addPC (r15, -4);
  *arm_regs[SVC_MODE][13] -= 4;
  wwmem ((*arm_regs[SVC_MODE][13]) >> 2, r(14));
  /* *arm_regs[SVC_MODE][14] &= ~0xFC000003; */

  r15 = (r15 & 0xFC000003)
        | VECTOR_UNDEFINED_INSTRUCTION | SVC_MODE | I_BIT;
  PCwrite = TRUE;
#else
  /* longjmp (exception, VECTOR_UNDEFINED_INSTRUCTION); */

  *arm_regs[SVC_MODE][14] = addPC (r15, -4);
  r15 = 0x7D029C | SVC_MODE | I_BIT;
  PCwrite = TRUE;
#endif
}

/*****************************************************************************
 ldm: Load/store multiple STMFD R13!,{R0-R12,R14}
 *****************************************************************************/

void 
ldm (register WORD ins)
{
  register WORD rn, base, md, oldbase, rest;

  rn = (ins & 0xf0000) >> 16;
  base = r (rn);
  rest = base & 3;
  base = base >> 2;

  if (L_LOAD)
    {
      if ((ins & 0x8000) == 0 && L_PSR)		/* If r15 not in list and S bit */
	{
	  md = USR_MODE;
	}
      else
	md = r15 & 3;

      if (L_UP)
	{
	  if (L_PRE_INDEX)
	    base++;
	  if (ins & 0x0001)
	    *arm_regs[md][0] = rwmem (base++);
	  if (ins & 0x0002)
	    *arm_regs[md][1] = rwmem (base++);
	  if (ins & 0x0004)
	    *arm_regs[md][2] = rwmem (base++);
	  if (ins & 0x0008)
	    *arm_regs[md][3] = rwmem (base++);
	  if (ins & 0x0010)
	    *arm_regs[md][4] = rwmem (base++);
	  if (ins & 0x0020)
	    *arm_regs[md][5] = rwmem (base++);
	  if (ins & 0x0040)
	    *arm_regs[md][6] = rwmem (base++);
	  if (ins & 0x0080)
	    *arm_regs[md][7] = rwmem (base++);
	  if (ins & 0x0100)
	    *arm_regs[md][8] = rwmem (base++);
	  if (ins & 0x0200)
	    *arm_regs[md][9] = rwmem (base++);
	  if (ins & 0x0400)
	    *arm_regs[md][10] = rwmem (base++);
	  if (ins & 0x0800)
	    *arm_regs[md][11] = rwmem (base++);
	  if (ins & 0x1000)
	    *arm_regs[md][12] = rwmem (base++);
	  if (ins & 0x2000)
	    *arm_regs[md][13] = rwmem (base++);
	  if (ins & 0x4000)
	    *arm_regs[md][14] = rwmem (base++);
	  if (ins & 0x8000)
	    {
	      if (L_PSR)
		{
		  if (r15 & 3)
		    r15 = rwmem (base++);
		  else
		    r15 = (r15 & 0x0c000003) | (rwmem (base++) & 0xf3fffffc);
		}
	      else
		r15 = (r15 & 0xfc000003) | (rwmem (base++) & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  if (L_PRE_INDEX)
	    base--;
	}
      else
	{
	  base = oldbase = (base - num_of_one_bits[ins & 0xff] -
			    num_of_one_bits[(ins & 0xff00) >> 8]);

	  if (!L_PRE_INDEX)
	    base++;
	  if (ins & 0x0001)
	    *arm_regs[md][0] = rwmem (base++);
	  if (ins & 0x0002)
	    *arm_regs[md][1] = rwmem (base++);
	  if (ins & 0x0004)
	    *arm_regs[md][2] = rwmem (base++);
	  if (ins & 0x0008)
	    *arm_regs[md][3] = rwmem (base++);
	  if (ins & 0x0010)
	    *arm_regs[md][4] = rwmem (base++);
	  if (ins & 0x0020)
	    *arm_regs[md][5] = rwmem (base++);
	  if (ins & 0x0040)
	    *arm_regs[md][6] = rwmem (base++);
	  if (ins & 0x0080)
	    *arm_regs[md][7] = rwmem (base++);
	  if (ins & 0x0100)
	    *arm_regs[md][8] = rwmem (base++);
	  if (ins & 0x0200)
	    *arm_regs[md][9] = rwmem (base++);
	  if (ins & 0x0400)
	    *arm_regs[md][10] = rwmem (base++);
	  if (ins & 0x0800)
	    *arm_regs[md][11] = rwmem (base++);
	  if (ins & 0x1000)
	    *arm_regs[md][12] = rwmem (base++);
	  if (ins & 0x2000)
	    *arm_regs[md][13] = rwmem (base++);
	  if (ins & 0x4000)
	    *arm_regs[md][14] = rwmem (base++);
	  if (ins & 0x8000)
	    {
	      if (L_PSR)
		{
		  if (r15 & 3)
		    r15 = rwmem (base++);
		  else
		    r15 = (r15 & 0x0c000003) | (rwmem (base++) & 0xf3fffffc);
		}
	      else
		r15 = (r15 & 0xfc000003) | (rwmem (base++) & 0x03fffffc);
	      PCwrite = TRUE;
	    }
	  base = oldbase;
	}

      if (rn != 15 && L_WRITEBACK)
	{
	  if (!(ins & (1 << rn)))
	    *arm_regs[md][rn] = (base << 2) + rest;
	          /* r (rn) = (base << 2) + rest; */
	}
    }
  else
    {
      if (L_PSR)		/* If S bit then USR mode */
	{
	  md = USR_MODE;
	}
      else
	md = r15 & 3;

      if (L_UP)
	{
	  oldbase = (base + num_of_one_bits[ins & 0xff] +
		     num_of_one_bits[(ins & 0xff00) >> 8]);

	  if (rn != 15 && L_WRITEBACK)
	    r (rn) = (oldbase << 2) + rest;

	  if (L_PRE_INDEX)
	    base++;
	  if (ins & 0x0001)
	    wwmem (base++, *arm_regs[md][0]);
	  if (ins & 0x0002)
	    wwmem (base++, *arm_regs[md][1]);
	  if (ins & 0x0004)
	    wwmem (base++, *arm_regs[md][2]);
	  if (ins & 0x0008)
	    wwmem (base++, *arm_regs[md][3]);
	  if (ins & 0x0010)
	    wwmem (base++, *arm_regs[md][4]);
	  if (ins & 0x0020)
	    wwmem (base++, *arm_regs[md][5]);
	  if (ins & 0x0040)
	    wwmem (base++, *arm_regs[md][6]);
	  if (ins & 0x0080)
	    wwmem (base++, *arm_regs[md][7]);
	  if (ins & 0x0100)
	    wwmem (base++, *arm_regs[md][8]);
	  if (ins & 0x0200)
	    wwmem (base++, *arm_regs[md][9]);
	  if (ins & 0x0400)
	    wwmem (base++, *arm_regs[md][10]);
	  if (ins & 0x0800)
	    wwmem (base++, *arm_regs[md][11]);
	  if (ins & 0x1000)
	    wwmem (base++, *arm_regs[md][12]);
	  if (ins & 0x2000)
	    wwmem (base++, *arm_regs[md][13]);
	  if (ins & 0x4000)
	    wwmem (base++, *arm_regs[md][14]);
	  if (ins & 0x8000)
	    wwmem (base++, addPC (r15, 4));
	  if (L_PRE_INDEX)
	    base--;
	}
      else
	{
	  base = oldbase = (base - num_of_one_bits[ins & 0xff] -
			    num_of_one_bits[(ins & 0xff00) >> 8]);

	  if (rn != 15 && L_WRITEBACK)
	    r (rn) = (oldbase << 2) + rest;

	  if (!L_PRE_INDEX)
	    base++;
	  if (ins & 0x0001)
	    wwmem (base++, *arm_regs[md][0]);
	  if (ins & 0x0002)
	    wwmem (base++, *arm_regs[md][1]);
	  if (ins & 0x0004)
	    wwmem (base++, *arm_regs[md][2]);
	  if (ins & 0x0008)
	    wwmem (base++, *arm_regs[md][3]);
	  if (ins & 0x0010)
	    wwmem (base++, *arm_regs[md][4]);
	  if (ins & 0x0020)
	    wwmem (base++, *arm_regs[md][5]);
	  if (ins & 0x0040)
	    wwmem (base++, *arm_regs[md][6]);
	  if (ins & 0x0080)
	    wwmem (base++, *arm_regs[md][7]);
	  if (ins & 0x0100)
	    wwmem (base++, *arm_regs[md][8]);
	  if (ins & 0x0200)
	    wwmem (base++, *arm_regs[md][9]);
	  if (ins & 0x0400)
	    wwmem (base++, *arm_regs[md][10]);
	  if (ins & 0x0800)
	    wwmem (base++, *arm_regs[md][11]);
	  if (ins & 0x1000)
	    wwmem (base++, *arm_regs[md][12]);
	  if (ins & 0x2000)
	    wwmem (base++, *arm_regs[md][13]);
	  if (ins & 0x4000)
	    wwmem (base++, *arm_regs[md][14]);
	  if (ins & 0x8000)
	    wwmem (base++, addPC (r15, 4));
	}
    }
}

/*****************************************************************************
 branch: Branch e.g. B &12345
 *****************************************************************************/

void 
branch (register WORD ins)
{
  r15 = addPC (r15, ((ins & 0x00ffffff) << 2));
  PCwrite = TRUE;
}

/* branch_link: Branch and link e.g. BL &12345.  */
void
branch_link (register WORD ins)
{
  r(14) = addPC (r15, -4);
  r15 = addPC (r15, ((ins & 0x00ffffff) << 2));
  PCwrite = TRUE;
}

#ifdef NWFPE
void
nwfpe_emulateall (register WORD ins)
{
  (void) EmulateAll ((unsigned int) ins);
}
#endif

/*****************************************************************************
 swi: Software interrupt number SWI "OS_WriteC"
 *****************************************************************************/

extern void swi (register WORD ins);

/*****************************************************************************
 main_decode: decode table for decoding an instruction to its group
 *****************************************************************************/

void (*main_decode[64]) (WORD) =
{
    oper_shift_con,		/* 0000 0 0 -- add r0,r1,r2,asl #30   */
    oper_shift_reg,		/* 0000 0 1 -- add r0,r1,r2,asl r3    */
    oper_shift_con,		/* 0000 1 0 -- add r0,r1,r2,asl #30   */
    multiply,			/* 0000 1 1 -- mla r0,r1,r2,r3        */

    oper_shift_con,		/* 0001 0 0 -- add r0,r1,r2,asl #30   */
    oper_shift_reg,		/* 0001 0 1 -- add r0,r1,r2,asl r3    */
    oper_shift_con,		/* 0001 1 0 -- add r0,r1,r2,asl #30   */
    undefined,			/* 0001 1 1 -- Undefined instruction  */

    oper_constant,		/* 0010 0 0 -- add r0,r1,#30          */
    oper_constant,		/* 0010 0 1 -- add r0,r1,#30          */
    oper_constant,		/* 0010 1 0 -- add r0,r1,#30          */
    oper_constant,		/* 0010 1 1 -- add r0,r1,#30          */

    oper_constant,		/* 0011 0 0 -- add r0,r1,#30          */
    oper_constant,		/* 0011 0 1 -- add r0,r1,#30          */
    oper_constant,		/* 0011 1 0 -- add r0,r1,#30          */
    oper_constant,		/* 0011 1 1 -- add r0,r1,#30          */

    ldr_constant,		/* 0100 0 0 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0100 0 1 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0100 1 0 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0100 1 1 -- ldr r0,[r1,#30]        */

    ldr_constant,		/* 0101 0 0 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0101 0 1 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0101 1 0 -- ldr r0,[r1,#30]        */
    ldr_constant,		/* 0101 1 1 -- ldr r0,[r1,#30]        */

    ldr_shift,			/* 0110 0 0 -- ldr r0,[r1,r2,asl #15] */
    undefined,			/* 0110 0 1 -- Undefined instruction  */
    ldr_shift,			/* 0110 1 0 -- ldr r0,[r1,r2,asl #15] */
    undefined,			/* 0110 1 1 -- Undefined instruction  */

    ldr_shift,			/* 0111 0 0 -- ldr r0,[r1,r2,asl #15] */
    undefined,			/* 0111 0 1                           */
    ldr_shift,			/* 0111 1 0 -- ldr r0,[r1,r2,asl #15] */
    undefined,			/* 0111 1 1                           */

    ldm,			/* 1000 0 0 -- ldm r0,{r0-r14}        */
    ldm,			/* 1000 0 1 -- ldm r0,{r0-r14}        */
    ldm,			/* 1000 1 0 -- ldm r0,{r0-r14}        */
    ldm,			/* 1000 1 1 -- ldm r0,{r0-r14}        */

    ldm,			/* 1001 0 0 -- ldm r0,{r0-r14}        */
    ldm,			/* 1001 0 1 -- ldm r0,{r0-r14}        */
    ldm,			/* 1001 1 0 -- ldm r0,{r0-r14}        */
    ldm,			/* 1001 1 1 -- ldm r0,{r0-r14}        */

    branch,			/* 1010 0 0 -- B veryfar              */
    branch,			/* 1010 0 1 -- B veryfar              */
    branch,			/* 1010 1 0 -- B veryfar              */
    branch,			/* 1010 1 1 -- B veryfar              */

    branch_link,       		/* 1011 0 0 -- BL veryfar             */
    branch_link,	       	/* 1011 0 1 -- BL veryfar             */
    branch_link,	       	/* 1011 1 0 -- BL veryfar             */
    branch_link,	       	/* 1011 1 1 -- BL veryfar             */

#ifdef NWFPE
    nwfpe_emulateall,		/* 1100 0 0 -- Co-processor           */
    nwfpe_emulateall,	       	/* 1100 0 1    data                   */
    nwfpe_emulateall,	       	/* 1100 1 0    transfers              */
    nwfpe_emulateall,	       	/* 1100 1 1                           */

    nwfpe_emulateall,	       	/* 1101 0 0 -- Co-processor           */
    nwfpe_emulateall,	       	/* 1101 0 1    data                   */
    nwfpe_emulateall,	       	/* 1101 1 0    transfers              */
    nwfpe_emulateall,	       	/* 1101 1 1                           */

    nwfpe_emulateall,	       	/* 1110 0 0 -- Co-proc data operation */
    nwfpe_emulateall,	       	/* 1110 1 0 -- Co-proc data operation */
    nwfpe_emulateall,	       	/* 1110 0 1 -- Co-proc reg transfers  */
    nwfpe_emulateall,	       	/* 1110 1 1 -- Co-proc reg transfers  */
#else
    undefined,			/* 1100 0 0 -- Co-processor           */
    undefined,			/* 1100 0 1    data                   */
    undefined,			/* 1100 1 0    transfers              */
    undefined,			/* 1100 1 1                           */

    undefined,			/* 1101 0 0 -- Co-processor           */
    undefined,			/* 1101 0 1    data                   */
    undefined,			/* 1101 1 0    transfers              */
    undefined,			/* 1101 1 1                           */

    undefined,			/* 1110 0 0 -- Co-proc data operation */
    undefined,			/* 1110 1 0 -- Co-proc data operation */
    undefined,			/* 1110 0 1 -- Co-proc reg transfers  */
    undefined,			/* 1110 1 1 -- Co-proc reg transfers  */
#endif
    swi,			/* 1111 0 0 -- SWI OS_Something       */
    swi,			/* 1111 1 0 -- SWI OS_Something       */
    swi,			/* 1111 0 1 -- SWI OS_Something       */
    swi				/* 1111 1 1 -- SWI OS_Something       */
};

/*****************************************************************************
 cond_xx: functions and decode table for the instruction conditions
 *****************************************************************************/

int 
cond_eq (void)
{
  return (Z_FLAG);
}
int 
cond_ne (void)
{
  return (!Z_FLAG);
}
int 
cond_cs (void)
{
  return (C_FLAG);
}
int 
cond_cc (void)
{
  return (!C_FLAG);
}
int 
cond_mi (void)
{
  return (N_FLAG);
}
int 
cond_pl (void)
{
  return (!N_FLAG);
}
int 
cond_vs (void)
{
  return (V_FLAG);
}
int 
cond_vc (void)
{
  return (!V_FLAG);
}
int 
cond_hi (void)
{
  return (C_FLAG & !Z_FLAG);
}
int 
cond_ls (void)
{
  return (!C_FLAG | Z_FLAG);
}
int 
cond_ge (void)
{
  return ((N_FLAG & V_FLAG) | (!N_FLAG & !V_FLAG));
}
int 
cond_lt (void)
{
  return ((N_FLAG & !V_FLAG) | (!N_FLAG & V_FLAG));
}
int 
cond_gt (void)
{
  return (((N_FLAG & V_FLAG) | (!N_FLAG & !V_FLAG)) & !Z_FLAG);
}
int 
cond_le (void)
{
  return ((N_FLAG & !V_FLAG) | (!N_FLAG & V_FLAG) | Z_FLAG);
}
int 
cond_al (void)
{
  return (TRUE);
}
int 
cond_nv (void)
{
  return (FALSE);
}

int (*conditie[16]) (void) =
{
  cond_eq, cond_ne, cond_cs, cond_cc, cond_mi, cond_pl, cond_vs, cond_vc,
    cond_hi, cond_ls, cond_ge, cond_lt, cond_gt, cond_le, cond_al, cond_nv
};

/*****************************************************************************
 emulator: the ARM emulator, gets an instruction decodes and executes it!
 *****************************************************************************/

void 
emulator (void)
{
  register WORD ins, cond, pc;
  register int vector;

  vector = setjmp (exception);

  switch (vector)
    {
    case VECTOR_UNDEFINED_INSTRUCTION:
    case VECTOR_SOFTWARE_INTERRUPT:
    case VECTOR_ABORT_PREFETCH:
      *arm_regs[SVC_MODE][14] = addPC (r15, -4);
      r15 = (r15 & 0xfc000003) | vector | SVC_MODE | I_BIT;
      PCwrite = TRUE;
      break;

    case VECTOR_ABORT_DATA:
    case VECTOR_ADDRESS_EXCEPTION:
      *arm_regs[SVC_MODE][14] = r15;
      r15 = (r15 & 0xfc000003) | vector | SVC_MODE | I_BIT;
      PCwrite = TRUE;
      break;

    case VECTOR_IRQ:
      *arm_regs[IRQ_MODE][14] = addPC (r15, -4);
      r15 = (r15 & 0xfc000003) | vector | IRQ_MODE | I_BIT;
      PCwrite = TRUE;
      break;

    case VECTOR_FIQ:
      *arm_regs[FIQ_MODE][14] = addPC (r15, -4);
      r15 = (r15 & 0xfc000003) | vector | FIQ_MODE | F_BIT | I_BIT;
      PCwrite = TRUE;
      break;
    }

  if (PCwrite)
    {
      PCwrite = FALSE;
      r15 = addPC (r15, 8);
    }

  while (TRUE)
    {
      pc = ((r15 & 0x3fffffc) >> 2) - 2;


      if (pc >= (ROM >> 2))
	longjmp (exception, VECTOR_ABORT_PREFETCH);
      
      ins = wmem[pc];
      ins = SWAPWORD (ins);

      cond = (ins & 0xf0000000) >> 28;

#ifdef DEBUG
      total_instructions ++;
#endif

      if (cond == 14 || conditie[cond] ())
	{
	  main_decode[((ins & 0x0f000000) >> 22) |
		      ((ins & 0x00000080) >> 6) |
		      ((ins & 0x00000010) >> 4)] (ins);
	}

      if (PCwrite)
	{
	  PCwrite = FALSE;
	  r15 = addPC (r15, 8);
	}
      else
	r15 = addPC (r15, 4);

/********************************************
    if (FIQint)
    { if ((r15 & F_BIT) == 0)
      { FIQint = FALSE ;
        longjmp(exception, VECTOR_FIQ) ;
      }
    }
*********************************************/

      if (IRQint)
	{
	  if ((r15 & I_BIT) == 0)
	    {
	      IRQint = FALSE;
	      longjmp (exception, VECTOR_IRQ);
	    }
	}
    }
}

#ifdef P_PC
#define PGSIZE 4096
long 
to_long (char huge * f)
{
  unsigned long l;
  l = FP_SEG (f) * 16L + (unsigned long) FP_OFF (f);
  return (l);
}
long 
far_fread (char huge * base, long length, long nmember, FILE * io)
{
  char huge *fp;
  long len;

  len = length * nmember;
  fp = base;

  while (len)
    {
      static char dbuf[PGSIZE];
      long rlen, tlen;
      tlen = (len > PGSIZE) ? PGSIZE : len;
      rlen = fread (dbuf, 1, tlen, io);

      movedata (_DS, (unsigned) dbuf, FP_SEG (fp), FP_OFF (fp), rlen);

      fp = fp + rlen;
      len = len - rlen;
      if (rlen != tlen)
	break;
    }

  return (to_long (fp) - to_long (base));
}
#endif

char *program_command_line;

/*****************************************************************************
 main: This is the place where everything starts!
 *****************************************************************************/

int 
main (int argc, char *argv[])
{
  FILE *in;
  BYTE HUGE *clear;
  WORD count, addr;
  int x, length;

#ifdef DEBUG
  printf ("ARM/RISC OS simulator v1.00 - memory size %d Kbytes\n",
	  MEMSIZE / 1024);
#endif

  if (argc <= 1)
    {
      fprintf (stderr, "Syntax: ARMemu <image file> [<param0> ... <paramN>]\n");
      exit (1);
    }

#ifdef P_PC
  wmem = farmalloc (MEMSIZE);
  bmem = (BYTE HUGE *) wmem;
#else
  wmem = malloc (MEMSIZE);
  bmem = (BYTE HUGE *) wmem;
#endif

  if (wmem == NULL)
    {
      printf ("Not enough memory - aborting!\n");
      exit (0);
    }

  for (count = 0, clear = bmem; count < MEMSIZE; count++)
    *clear++ = 0;

  addr = 0x8000;

  /* printf ("Loading '%s' at address &%lX\n", argv[1], addr); */
  in = fopen (argv[1], "rb");
  if (in == NULL)
    {
      fprintf (stderr, "Error: Can't open '%s' for reading\n", argv[1]);
      exit (2);
    }
#ifdef P_PC
  far_fread (bmem + addr, 1, MEMSIZE - addr, in);
#else
  fread (bmem + addr, 1, MEMSIZE - addr, in);
#endif
  fclose (in);

  /* Count program command line length.  */
  length = 0;
  for (x = 1; x < argc; x++)
    length += strlen (argv[x]) + 1;

  program_command_line = malloc (length + 1);
  for (x = 1; x < argc; x++)
    {
      strcat (program_command_line, argv[x]);
      if (x != argc - 1)
        strcat (program_command_line, " ");
    }

  initialise_armrom ();
  initialise_fpe ();
  initswi ();

  r15 = 0;
  IRQint = FALSE;
  FIQint = FALSE;
  PCwrite = TRUE;

  *arm_regs[SVC_MODE][13] = RAM - 4;

  total_instructions = 0;
  execution_start = clock ();

  if (argc == 4)
    debugger ();
  else
    emulator ();

  printf ("\narmemu - Aborted by unknown reason!\n");

  return 0;
}

static void initialise_armrom (void)
{
#ifndef NWFPE
  int x;

  for (x = 0; x < sizeof (armrom_code); x += 4)
    wmem[x >> 2] = armrom_code[x >> 2];
#endif
}

static void initialise_fpe (void)
{
#ifdef NWFPE
  fpa11 = (FPA11 *) malloc (sizeof (FPA11));
  resetFPA11 ();
#else
  int x;

  /* Store the FPE in ROM.  */
  for (x = 0; x < fpesize; x += 4)
     wmem[(RAM + x) >> 2] = fpecode[x >> 2]; 

  /* Fix the Undefined Instruction vector.  */
  wmem[VECTOR_UNDEFINED_INSTRUCTION >> 2] = 0xea000000 | (((RAM + 0x278) >> 2) - 3);
#endif
}

#ifdef NWFPE
void float_raise (signed char flags)
{
  printf ("NWFPE: exception %08x", flags);
}
#endif


