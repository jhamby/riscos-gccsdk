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
#include "armfpe.h"
#include "armrom.h"

static void initialise_armrom (void);
static void initialise_fpe (void);
static void initialise_immediates (void);

WORD rg[16], r15, r15flags;
WORD r8fiq, r9fiq, r10fiq, r11fiq, r12fiq, r13fiq, r14fiq;
WORD r13usr, r14usr;
WORD r13irq, r14irq;
WORD r13svc, r14svc;
WORD r13abt, r14abt; /* ARM6 and later.  */
WORD r13und, r14und; /* ARM6 and later.  */
WORD cpsr; /* Current processor status register.  */

/* Saved processor status registers.  */
WORD spsr_usr, spsr_svc, spsr_irq, spsr_fiq, spsr_abt, spst_und;


WORD fpsr; /* Floating point status register.  */

volatile int IRQint, FIQint;

int PCwrite;
WORD HUGE *wmem;
BYTE HUGE *bmem;

jmp_buf exception;

/* arm_registers points to the particular bank that is set by the
   mode bits of R15.  */
WORD *arm_registers;

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

WORD immediate_table[4096];	/* immediate DP LHS values */

unsigned int total_instructions;
clock_t execution_start;

#define DPImmRHS (ARMul_ImmedTable[BITS(0,11)])
#define DPSImmRHS temp = BITS(0,11) ; \
                  rhs = ARMul_ImmedTable[temp] ; \
                  if (temp > 255) /* there was a shift */ \
                     ASSIGNC(rhs >> 31) ;

/*****************************************************************************
 Read/Write Byte/Word MEMC routines
 *****************************************************************************/

#ifdef FAST
__inline__ WORD rwmem (WORD a)
{
  a = wmem [a];
  return SWAPWORD (a);
}

__inline__ BYTE rbmem (WORD a)
{
  return bmem[a];
}

__inline__ void wwmem (WORD a, WORD b)
{
  wmem[a] = SWAPWORD (b);
}

__inline__ void wbmem (WORD a, WORD b)
{
  bmem[a] = b;
}

#else
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
#endif

/*****************************************************************************
 undefined: undefined instruction
 *****************************************************************************/

void 
undefined (register WORD ins)
{
#if 0
  *arm_regs[SVC_MODE][14] = addPC (r15, -4);
  *arm_regs[SVC_MODE][13] -= 4;
  wwmem ((*arm_regs[SVC_MODE][13]) >> 2, r(14));
  /* *arm_regs[SVC_MODE][14] &= ~0xFC000003; */

  r15 = (r15 & 0xFC000003)
        | VECTOR_UNDEFINED_INSTRUCTION | SVC_MODE | I_BIT;
  PCwrite = TRUE;
#endif

  /* longjmp (exception, VECTOR_UNDEFINED_INSTRUCTION); */

#ifdef DEBUG
  printf ("******** Undefined\n");
#endif
  *arm_regs[SVC_MODE][14] = addPC (r15, -4);
  r15 = 0x7D029C | SVC_MODE | I_BIT;
  PCwrite = TRUE;
}

/*****************************************************************************
 branch: Branch e.g. B &12345
 *****************************************************************************/

void 
op_b (register WORD ins)
{
  r15 = addPC (r15, ((ins & 0x00ffffff) << 2));
  PCwrite = TRUE;
}

/* branch_link: Branch and link e.g. BL &12345.  */
void
op_bl (register WORD ins)
{
  r(14) = addPC (r15, -4);
  r15 = addPC (r15, ((ins & 0x00ffffff) << 2));
  PCwrite = TRUE;
}

/*****************************************************************************
 swi: Software interrupt number SWI "OS_WriteC"
 *****************************************************************************/

extern void swi (register WORD ins);

void (*main_dec[256]) (WORD);

WORD op_lsl_i (WORD ins, WORD rm)
{
  return rm << (ins & 0x1f);
}

WORD op_lsl_r (WORD ins, WORD rm)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;

  return rm << sh;
}

WORD op_lsr_i (WORD ins, WORD rm)
{
  WORD sh = (ins & 0x1f);

  if (sh == 0)
    sh = 32;
  return rm >> sh;
}

WORD op_lsr_r (WORD ins, WORD rm)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;

  if (sh == 0)
    sh = 32;
  return rm >> sh;
}

WORD op_asr_i (WORD ins, WORD rm)
{
  /* By casting to int we can do an arithmetic shift right.  */
  int sh = (int) (ins & 0x1f);
  int temp = (int) rm;

  if (sh == 0)
    sh = 32;

  return (temp >> sh);
}

WORD op_asr_r (WORD ins, WORD rm)
{
  int sh = (int) r((ins >> 1) & 0xf) & 0xff;
  int temp = (int) rm;

  if (sh == 0)
    sh = 32;

  return (temp >> sh);
}

WORD op_ror_i (WORD ins, WORD rm)
{
  WORD sh = ins & 0x1f;

  /* MOV R0, R0, R0R #15 is equivalent to
       R0 = (R0 >> 15) | (R0 << (32 - 15)); */ 

  if (sh)
    return (rm >> sh) | (rm << (32 - sh));

  return (C_FLAG << 31) | (rm >> 1);
} 

WORD op_ror_r (WORD ins, WORD rm)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;

  if (sh)
    {
      /* Repeatedly substract 32 from sh until sh is in the range 1 - 32.  */
      while (sh > 32)
        sh -= 32;

      return (rm >> sh) | (rm << (32 - sh));
    }

  return (C_FLAG << 31) | (rm >> 1);
}

WORD op_c_lsl_i (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = (ins & 0x1f);

  if (sh == 0)
    {
      *carry = C_FLAG;
      return rm;
    }

  *carry = (rm >> (32 - sh)) & 1;
  return rm << sh;
}

WORD op_c_lsl_r (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;

  if (sh == 0)
    {
      *carry = C_FLAG;
      return rm;
    }

  *carry = (rm >> (32 - sh)) & 1;
  return rm << sh;
}

WORD op_c_lsr_i (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = (ins & 0x1f);
  if (sh == 0)
    sh = 32;

  *carry = (rm >> (sh - 1)) & 1;
  return rm >> sh;
}

WORD op_c_lsr_r (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;

  if (sh == 0)
    sh = 32;

  *carry = (rm >> (sh - 1)) & 1;
  return rm >> sh;
}

WORD op_c_asr_i (WORD ins, WORD rm, WORD *carry)
{
  int sh = (int) (ins & 0x1f);
  int temp = (int) rm;

  if (sh == 0)
    sh = 32;

  *carry = (temp >> (sh - 1)) & 1;

  return temp >> sh;
}

WORD op_c_asr_r (WORD ins, WORD rm, WORD *carry)
{
  int sh = (int) r((ins >> 1) & 0xf) & 0xff;
  int temp = (int) rm;

  if (sh == 0)
    sh = 32;

  *carry = (temp >> (sh - 1)) & 1;
  return temp >> sh;
}

WORD op_c_ror_i (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = (ins & 0x1f);
  if (sh)
    {
      *carry = (rm >> (sh - 1)) & 1;
      return (rm >> sh) | (rm << (32 - sh));
    }

  /* RRX */
  *carry = rm & 1;
  return (C_FLAG << 31) | (rm >> 1);
}

WORD op_c_ror_r (WORD ins, WORD rm, WORD *carry)
{
  WORD sh = r((ins >> 1) & 0xf) & 0xff;
  if (sh)
    {
      while (sh > 32)
	sh -= 32;

      *carry = (rm >> (sh - 1)) & 1;
      return (rm >> sh) | (rm << (32 - sh));
    }

  /* RRX */
  *carry = rm & 1;
  return (C_FLAG << 31) | (rm >> 1);
}

/* Bits 4, 5 and 6.  */
WORD (*shift_register[8]) (WORD, WORD) =
{
  op_lsl_i,			/* 00 0 -- LSL # */
  op_lsl_r,			/* 00 1 -- LSL reg */
  op_lsr_i,			/* 01 0 -- LSR # */
  op_lsr_r,			/* 01 1 -- LSR reg */
  op_asr_i,			/* 10 0 -- ASR # */
  op_asr_r,			/* 10 1 -- ASR reg */
  op_ror_i,			/* 11 0 -- ROR # */
  op_ror_r,			/* 11 1 -- ROR reg */
};

/* Perform a shift and set the carry.  Bits 4, 5 and 6.  */
WORD (*shift_register_carry[8]) (WORD, WORD, WORD *) =
{
  op_c_lsl_i,			/* 00 0 -- LSL # */
  op_c_lsl_r,			/* 00 1 -- LSL reg */
  op_c_lsr_i,			/* 01 0 -- LSR # */
  op_c_lsr_r,			/* 01 1 -- LSR reg */
  op_c_asr_i,			/* 10 0 -- ASR # */
  op_c_asr_r,			/* 10 1 -- ASR reg */
  op_c_ror_i,			/* 11 0 -- ROR # */
  op_c_ror_r,			/* 11 1 -- ROR reg */
};

#define UPDATE_BITWISE_R15_S					\
  if (rd == 15)							\
    {								\
      if (r15 & 3)						\
	r15 = result;						\
      else							\
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);	\
      PCwrite = TRUE;						\
    }								\
  else								\
    {								\
      r(rd) = result;						\
								\
      r15 = (r15 & 0x1fffffff) | (result & 0x80000000)	\
	  | ((WORD)(result == 0) << Z_SHFT) | (carry << C_SHFT);\
    }

#define UPDATE_BITWISE_R15_S_CMP       				\
  if (rd == 15)							\
    {								\
      if (r15 & 3)						\
	r15 = (r15 & 0x03fffffc) | (result & 0xfc000003);	\
      else							\
	r15 = (r15 & 0x0fffffff) | (result & 0xf0000000);	\
    }								\
  else								\
    {								\
      r15 = (r15 & 0x1fffffff) | (result & 0x80000000)	\
	  | ((WORD)(result == 0) << Z_SHFT) | (carry << C_SHFT);\
    }

#define POS(i) ((~i) >> 31)
#define NEG(i) ((i) >> 31)

#define CALCULATE_ADD_CV				\
  carry = (NEG(rn) && NEG(rm))				\
     || (NEG(rn) && POS(result))			\
     || (NEG(rm) && POS(result));			\
  overflow = (NEG(rn) && NEG(rm) && POS(result))	\
     || (POS(rn) && POS(rm) && NEG(result));

#define CALCULATE_SUB_CV				\
  carry = (NEG(rn) && POS(rm))				\
     || (NEG(rn) && POS(result))			\
     || (POS(rm) && POS(result));			\
  overflow = (NEG(rn) && POS(rm) && POS(result))	\
     || (POS(rn) && NEG(rm) && NEG(result));

#if 0
#define CALCULATE_ADD_CV						\
  carry = ((rn & 0x80000000) && (rm & 0x80000000))		\
    || ((rn & 0x80000000) && !(result & 0x80000000))		\
    || ((rm & 0x80000000) && !(result & 0x80000000));		\
  overflow = ((rn & 0x80000000) && (rm & 0x80000000) && !(result & 0x80000000)) \
    || (!(rn & 0x80000000) && !(rm & 0x80000000) && (result & 0x80000000));

#define CALCULATE_SUB_CV					\
  carry = ((rn & 0x80000000) && !(rm & 0x80000000))		\
    || ((rn & 0x80000000) && !(result & 0x80000000))		\
    || (!(rm & 0x80000000) && !(result & 0x80000000));		\
  overflow = ((rn & 0x80000000) && !(rm & 0x80000000) && !(result & 0x80000000)) \
    || (!(rn & 0x80000000) && (rm & 0x80000000) && (result & 0x80000000));
#endif


void op_mul (WORD ins)
{
  WORD rd, rs, rm;

  rd = (ins >> 16) & 0xf;
  if (rd != 15)
    {
      rs = (ins >> 8) & 0xf;
      if (rs == 15)
	rs = r(rs) & 0xfc000003;
      else
	rs = r(rs);

      rm = ins & 0xf;
      if (rm == rd)
	r(rd) = 0;
      else
	{
	  if (rm == 15)
	    rm = r(rm) + 4;
	  else
	    rm = r(rm);

	  r(rd) = rm * rs;
	}
    }
}

void op_mul_s (WORD ins)
{
  WORD rd, rs, rm, result;

  rd = (ins >> 16) & 0xf;
  if (rd != 15)
    {
      rs = (ins >> 8) & 0xf;
      if (rs == 15)
	rs = r(rs) & 0xfc000003;
      else
	rs = r(rs);

      rm = ins & 0xf;
      if (rm == rd)
	result = 0;
      else
	{
	  if (rm == 15)
	    rm = r(rm) + 4;
	  else
	    rm = r(rm);

	  result = rm * rs;
	}

      r(rd) = result;
      /* Don't modify V or C.  */
      r15 = (r15 & 0x3fffffff)
	| (result & 0x80000000)
	| ((result == 0) << Z_SHFT);
    }
}

void op_mla (WORD ins)
{
  WORD rd, rs, rm, rn;

  rd = (ins >> 16) & 0xf;
  if (rd != 15)
    {
      rn = (ins >> 12) & 0xf;

      rs = (ins >> 8) & 0xf;
      if (rs == 15)
	rs = r(rs) & 0xfc000003;
      else
	rs = r(rs);

      rm = ins & 0xf;
      if (rm == rd)
	r(rd) = 0;
      else
	{
	  if (rm == 15)
	    rm = r(rm) + 4;
	  else
	    rm = r(rm);

	  r(rd) = r(rm) * rs + r(rn);
	}
    }
}

void op_mla_s (WORD ins)
{
  WORD rd, rs, rm, rn, result;

  rd = (ins >> 16) & 0xf;
  if (rd != 15)
    {
      rn = (ins >> 12) & 0xf;

      rs = (ins >> 8) & 0xf;
      if (rs == 15)
	rs = r(rs) & 0xfc000003;
      else
	rs = r(rs);

      rm = ins & 0xf;
      if (rm == rd)
	result = 0;
      else
	{
	  if (rm == 15)
	    rm = r(rm) + 4;
	  else
	    rm = r(rm);

	  result = rm * rs + r(rn);
	}

      r(rd) = result;
      r15 = (r15 & 0x3fffffff)
	| (result & 0x80000000)
	| ((result == 0) << Z_SHFT);
    }
}

/* DATA PROCESSING GROUP.  */
void op_and (WORD ins)
{
  WORD rn, rd, rm, result;

  if ((ins & 0xf0) == 0x90) /* MUL instruction.  */
    {
      op_mul (ins);
      return;
    }

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn & rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_and_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  if ((ins & 0xf0) == 0x90) /* MULS instruction.  */
    {
      op_mul_s (ins);
      return;
    }

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn & rm;

  UPDATE_BITWISE_R15_S;
}

void op_eor (WORD ins)
{
  WORD rn, rd, rm, result;

  if ((ins & 0xf0) == 0x90) /* MLA instruction.  */
    {
      op_mla (ins);
      return;
    }

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn ^ rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_eor_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  if ((ins & 0xf0) == 0x90) /* MLAS instruction.  */
    {
      op_mla_s (ins);
      return;
    }

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn ^ rm;

  UPDATE_BITWISE_R15_S;
}

struct flag_struct
{
  unsigned int c:1; /* first byte */
  int :5;
  unsigned int nz:2;
  /* unsigned int z:1;
     unsigned int n:1; */
  int :3;
  unsigned int v:1;
  int :20;
};

void op_sub (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn - rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}


void op_sub_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  if (rd == 15)
    {
      result = rn - rm;
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);

      PCwrite = TRUE;
    }
  else
    {
#ifdef INLINE_ASSEMBLER
      __asm__ __volatile__ ("subl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			    : "=r" (regflags), "=r" (rm)
			    : "r" (rn), "1" (rm)
			    : "cc");

      r(rd) = rm;

      r15 = (r15 & 0x0fffffff) | ((!regflags.c) << C_SHFT)
	| (regflags.v << V_SHFT) | (regflags.nz << Z_SHFT);
#else
      result = rn - rm;
      CALCULATE_SUB_CV
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD) result == 0) << Z_SHFT);

#endif
    }
}

void op_rsb (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rm - rn;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_rsb_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rm = (ins >> 16) & 0xf;
  if (rm == 15)
    rm = r15 & 0x03fffffc;
  else
    rm = r(rm);

  rd = (ins >> 12) & 0xf;
  rn = (ins & 0xf);
  rn = shift_register [(ins >> 4) & 7] (ins >> 7, r(rn));

  result = rn - rm;
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_add (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn + rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_add_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow, scratch;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(ins & 0xf));

  result = rn + rm;

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      result = rn + rm;

      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
#ifdef INLINE_ASSEMBLER
      __asm__ __volatile__ ("addl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			    : "=r" (regflags), "=r" (result)
			    : "r" (rn), "1" (rm)
			    : "cc");

      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (regflags.c << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
#else
      result = rn + rm;
      CALCULATE_ADD_CV
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
#endif
    }
}

void op_adc (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn + rm + C_FLAG;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_adc_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm)) + C_FLAG;

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      result = rn + rm;

      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
#ifdef INLINE_ASSEMBLER
      __asm__ __volatile__ ("addl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			    : "=r" (regflags), "=r" (result)
			    : "r" (rn), "1" (rm)
			    : "cc");
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (regflags.c << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
#else
      result = rn + rm;
      CALCULATE_ADD_CV
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
#endif
    }
}

void op_sbc (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn - (rm + C_FLAG);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_sbc_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn - (rm + C_FLAG);
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_rsc (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rm - (rn + C_FLAG);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_rsc_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rm = (ins >> 16) & 0xf;
  if (rm == 15)
    rm = r15 & 0x03fffffc;
  else
    rm = r(rm);

  rd = (ins >> 12) & 0xf;
  rn = (ins & 0xf);
  rn = shift_register [(ins >> 4) & 7] (ins >> 7, r(rn));

  result = rn - (rm + C_FLAG);
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_tst_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn & rm;
  UPDATE_BITWISE_R15_S_CMP
}

void op_teq_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn ^ rm;
  UPDATE_BITWISE_R15_S_CMP
}

void op_cmp_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  __asm__ __volatile__ ("subl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			: "=r" (regflags), "=r" (result)
			: "r" (rm), "1" (rn)
			: "cc");

  if (rd == 15)
    {
      if (r15 & 3)
	r15 = (r15 & 0x03fffffc) | (result & 0xfc000003);
      else
	r15 = (r15 & 0x0fffffff) | (result & 0xf0000000);
    }
  else
    {
      r15 = (r15 & 0x0fffffff)
	| ((!regflags.c) << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
    }
}

void op_cmn_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn + rm;
  CALCULATE_ADD_CV

  if (rd == 15)
    {
      if (r15 & 3)
	r15 = (r15 & 0x03fffffc) | (result & 0xfc000003);
      else
	r15 = (r15 & 0x0fffffff) | (result & 0xf0000000);
    }
  else
    {
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| ((WORD)(result == 0) << Z_SHFT);
    }
}

void op_orr (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn | rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_orr_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn | rm;

  UPDATE_BITWISE_R15_S
}

void op_mov (WORD ins)
{
  WORD rd, rm;

  rd = (ins >> 12) & 0xf;
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(ins & 0xf));

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rm & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rm;
}

void op_mov_s (WORD ins)
{
  WORD rd, rm, result;
  WORD carry;
  struct flag_struct regflags;

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  result = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  __asm__ __volatile__ ("cmpl $0, %k1\n\tpushfl\n\tpopl %0\n"
			: "=r" (regflags)
			: "r" (result)
			: "cc");

  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x1fffffff) | (regflags.nz << Z_SHFT)
	  | (carry << C_SHFT);
    }
}

void op_bic (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register [(ins >> 4) & 7] (ins >> 7, r(rm));

  result = rn & ~rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_bic_s (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = rn & ~rm;

  UPDATE_BITWISE_R15_S
}

void op_mvn (WORD ins)
{
  WORD rd, rm;

  rm = ~shift_register [(ins >> 4) & 7] (ins >> 7, r(ins & 0xf));

  rd = (ins >> 12) & 0xf;
  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rm & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rm;
}

void op_mvn_s (WORD ins)
{
  WORD rd, rm, result;
  WORD carry;

  rd = (ins >> 12) & 0xf;
  rm = (ins & 0xf);
  rm = shift_register_carry [(ins >> 4) & 7] (ins >> 7, r(rm), &carry);

  result = ~rm;
  UPDATE_BITWISE_R15_S
}

/* DATA PROCESSING GROUP - IMMEDIATE OPERANDS.  */
void op_and_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = immediate_table [ins & 0xfff];
  result = rn & rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_and_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;

  result = rn & rm;

  UPDATE_BITWISE_R15_S
}

void op_eor_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = immediate_table[ins & 0xfff];
  result = rn ^ rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_eor_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;

  result = rn ^ rm;
  UPDATE_BITWISE_R15_S
}

void op_sub_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rn - rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_sub_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];

  result = rn - rm;
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_rsb_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rm - rn;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_rsb_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rm = (ins >> 16) & 0xf;
  if (rm == 15)
    rm = r15 & 0x03fffffc;
  else
    rm = r(rm);

  rd = (ins >> 12) & 0xf;

  rn = immediate_table[ins & 0xfff];

  result = rn - rm;
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_add_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rn + rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_add_si (WORD ins)
{
  WORD rn, rd, rm, result;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];

  __asm__ __volatile__ ("addl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			: "=r" (regflags), "=r" (result)
			: "r" (rn), "1" (rm)
			: "cc");

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (regflags.c << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
    }
}

void op_adc_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rn + rm + C_FLAG;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_adc_si (WORD ins)
{
  WORD rn, rd, rm, result;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff] + C_FLAG;

  result = rn + rm;
  __asm__ __volatile__ ("addl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			: "=r" (regflags), "=r" (result)
			: "r" (rn), "1" (rm)
			: "cc");

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (regflags.c << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
    }
}

void op_sbc_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rn - (rm + C_FLAG);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_sbc_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = immediate_table[ins & 0xfff];

  result = rn - (rm + C_FLAG);
  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_rsc_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rm - (rn + C_FLAG);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_rsc_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;

  rm = (ins >> 16) & 0xf;
  if (rm == 15)
    rm = r15 & 0x03fffffc;
  else
    rm = r(rm);

  rd = (ins >> 12) & 0xf;

  rn = immediate_table[ins & 0xfff];
  result = rn - (rm + C_FLAG);

  CALCULATE_SUB_CV

  /* Update CNZV flags.  */
  if (rd == 15)
    {
      if (r15 & 3)
	r15 = result;
      else
	r15 = (r15 & 0x0c000003) | (result & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    {
      r(rd) = result;
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| (((WORD)result == 0) << Z_SHFT);
    }
}

void op_tst_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;

  result = rn & rm;
  UPDATE_BITWISE_R15_S_CMP
}

void op_teq_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, imm_val;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  imm_val = ins & 0xfff;
  rm = immediate_table[imm_val];

  carry = (imm_val > 255) ? (rm >> 31) : C_FLAG;
  result = rn ^ rm;

  UPDATE_BITWISE_R15_S_CMP
}

void op_cmp_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry, overflow;
  struct flag_struct regflags;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;
  rm = immediate_table[ins & 0xfff];

  __asm__ __volatile__ ("subl %k2, %k1\n\tpushfl\n\tpopl %0\n"
			: "=r" (regflags), "=r" (result)
			: "r" (rm), "1" (rn)
			: "cc");

  if (rd == 15)
    {
      if (r15 & 3)
	r15 = (r15 & 0x03fffffc) | (result & 0xfc000003);
      else
	r15 = (r15 & 0x0fffffff) | (result & 0xf0000000);
    }
  else
    {
      r15 = (r15 & 0x0fffffff)
	| ((!regflags.c) << C_SHFT) | (regflags.v << V_SHFT)
	| (regflags.nz << Z_SHFT);
    }
}


void op_cmn_si (WORD ins)
{
  WORD rn, rm, rd, result, carry, overflow;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];

  result = rn + rm;
  CALCULATE_ADD_CV

  if (rd == 15)
    {
      if (r15 & 3)
	r15 = (r15 & 0x03fffffc) | (result & 0xfc000003);
      else
	r15 = (r15 & 0x0fffffff) | (result & 0xf0000000);
    }
  else
    {
      r15 = (r15 & 0x0fffffff)
	| (carry << C_SHFT)
	| (overflow << V_SHFT)
	| (result & 0x80000000)
	| ((WORD)(result == 0) << Z_SHFT);
    }
}

void op_orr_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = rn | rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_orr_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;

  result = rn | rm;
  UPDATE_BITWISE_R15_S
}

void op_mov_i (WORD ins)
{
  WORD rd, result;

  rd = (ins >> 12) & 0xf;
  result = immediate_table[ins & 0xfff];

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_mov_si (WORD ins)
{
  WORD rd, rm, result;
  WORD carry;

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;

  result = rm;
  UPDATE_BITWISE_R15_S
}

void op_bic_i (WORD ins)
{
  WORD rn, rd, rm, result;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];

  result = rn & ~rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_bic_si (WORD ins)
{
  WORD rn, rd, rm, result;
  WORD carry;

  rn = (ins >> 16) & 0xf;
  if (rn == 15)
    rn = r15 & 0x03fffffc;
  else
    rn = r(rn);

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;
  result = rn & ~rm;

  UPDATE_BITWISE_R15_S
}

void op_mvn_i (WORD ins)
{
  WORD rd, rm, result;

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  result = ~rm;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (result & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = result;
}

void op_mvn_si (WORD ins)
{
  WORD rd, rm, result;
  WORD carry;

  rd = (ins >> 12) & 0xf;

  rm = immediate_table[ins & 0xfff];
  carry = ((ins & 0xfff) > 255) ? (rm >> 31) : C_FLAG;
  result = ~rm;
  UPDATE_BITWISE_R15_S
}

/* SINGLE DATA TRANSFER.  */
void op_str (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_ldr (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_str_w (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}

void op_ldr_w (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}

void op_strb (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_ldrb (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_strb_w (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}

void op_ldrb_w (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}


void op_str_u (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_ldr_u (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_str_wu (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_ldr_wu (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_strb_u (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_ldrb_u (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_strb_wu (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_ldrb_wu (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

/* SINGLE DATA TRANSFER - PRE-INDEXED.  */
void op_str_p (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_p (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_str_wp (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_wp (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_strb_p (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_p (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}

void op_strb_wp (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_wp (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}


void op_str_up (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_up (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_str_wup (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_wup (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_strb_up (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_up (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}

void op_strb_wup (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_wup (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xfff);

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}

/* SINGLE DATA TRANSFER - Register form.  */
void op_str_i (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_ldr_i (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_str_wi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}

void op_ldr_wi (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}


void op_strb_i (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_ldrb_i (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  r(rn) = base - offset;
}

void op_strb_wi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}

void op_ldrb_wi (WORD ins)
{
  WORD rn, rd, base, offset, word;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  word = rwmem (base >> 2);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base - offset;
}


void op_str_ui (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_ldr_ui (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_str_wui (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_ldr_wui (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_strb_ui (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_ldrb_ui (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  r(rn) = base + offset;
}

void op_strb_wui (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

void op_ldrb_wui (WORD ins)
{
  WORD rn, rd, base, offset, word;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = *arm_regs[USR_MODE][rn]; /* Trans bit.  */

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);

  /* Post writeback.  */
  *arm_regs[USR_MODE][rn] = base + offset;
}

/* SINGLE DATA TRANSFER - PRE-INDEXED, REGISTER FORM  */
void op_str_pi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_pi (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_str_wpi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_wpi (WORD ins)
{
  WORD rn, rd, base, sh, offset, word;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_strb_pi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_pi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}

void op_strb_wpi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_wpi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base -= offset;
  r(rn) = base;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}


void op_str_upi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_upi (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_str_wupi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    wwmem (base >> 2, addPC (r15, 4));
  else
    wwmem (base >> 2, r(rd));
}

void op_ldr_wupi (WORD ins)
{
  WORD rn, rd, base, offset, word, sh;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  word = rwmem (base >> 2);
  /* Handle non-aligned loads.  */
  if (base & 3)
    {
      sh = (base & 3) * 8;
      word = (word >> sh) | (word << (32 - sh));
    }

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (word & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = word;
}

void op_strb_upi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_upi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}

void op_strb_wupi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    wbmem (base, addPC (r15, 4) & 0xff);
  else
    wbmem (base, r(rd) & 0xff);
}

void op_ldrb_wupi (WORD ins)
{
  WORD rn, rd, base, offset;

  rd = (ins >> 12) & 0xf;
  offset = (ins & 0xf);
  offset = shift_register [(ins >> 4) & 7] (ins >> 7, r(offset));

  rn = (ins >> 16) & 0xf; 
  if (rn == 15)
    base = r15 & 0x03fffffc;
  else
    base = r(rn);

  base += offset;
  r(rn) = base;

  if (rd == 15)
    {
      r15 = (r15 & 0xfc000003) | (rbmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  else
    r(rd) = rbmem (base);
}


/* BLOCK DATA TRANSFER.  */
void op_stm (WORD ins)
{
  WORD base, mode;

  /* post indexed, down, no-write-back, do not load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  if (ins & 0x8000) wwmem (base--, addPC (r15, 4));
  if (ins & 0x4000) wwmem (base--, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (base--, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (base--, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (base--, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (base--, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (base--, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (base--, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (base--, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (base--, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (base--, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (base--, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (base--, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (base--, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (base--, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (base, *arm_regs[mode][0]);
}

void op_ldm (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* post indexed, down, no-write-back, do not load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (base--) & 0x03fffffc);
      PCwrite = TRUE;
    }
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base--);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base--);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base--);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base--);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base--);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base--);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base--);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base--);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base--);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base--);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base--);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base--);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base--);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base--);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base);
}

void op_stm_w (WORD ins)
{
  WORD rn, base, mode;

  /* post indexed, down, write-back, do not load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  if (ins & 0x8000) wwmem (base--, addPC (r15, 4));
  if (ins & 0x4000) wwmem (base--, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (base--, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (base--, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (base--, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (base--, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (base--, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (base--, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (base--, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (base--, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (base--, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (base--, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (base--, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (base--, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (base--, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (base--, *arm_regs[mode][0]);
  r(rn) = base << 2;
}

void op_ldm_w (WORD ins)
{
  WORD rn, base, mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  /* post indexed, down, write-back, do not load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (base--) & 0x03fffffc);
      PCwrite = TRUE;
    }
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base--);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base--);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base--);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base--);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base--);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base--);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base--);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base--);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base--);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base--);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base--);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base--);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base--);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base--);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base--);

  *arm_regs[mode][rn] = base << 2;
}

void op_stm_s (WORD ins)
{
  WORD base, mode;

  /* post indexed, down, no-write-back, load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = USR_MODE;

  if (ins & 0x8000) wwmem (base--, addPC (r15, 4));
  if (ins & 0x4000) wwmem (base--, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (base--, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (base--, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (base--, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (base--, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (base--, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (base--, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (base--, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (base--, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (base--, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (base--, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (base--, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (base--, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (base--, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (base, *arm_regs[mode][0]);
}

void op_ldm_s (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* post indexed, down, no-write-back, load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (base--);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (base--) & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    /* If S bit is set and R15 is not in the transfer list then
       force USR mode.  */
    mode = USR_MODE;

  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base--);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base--);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base--);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base--);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base--);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base--);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base--);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base--);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base--);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base--);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base--);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base--);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base--);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base--);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base);
}

void op_stm_ws (WORD ins)
{
  WORD rn, base, mode;

  /* post indexed, down, write-back, load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = USR_MODE;

  if (ins & 0x8000) wwmem (base--, addPC (r15, 4));
  if (ins & 0x4000) wwmem (base--, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (base--, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (base--, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (base--, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (base--, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (base--, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (base--, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (base--, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (base--, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (base--, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (base--, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (base--, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (base--, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (base--, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (base--, *arm_regs[mode][0]);
  r(rn) = base << 2;
}

void op_ldm_ws (WORD ins)
{
  WORD rn, base, mode, old_mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  old_mode = r15 & 3;

  /* post indexed, down, write-back, load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (base--);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (base--) & 0xf3fffffc);
      PCwrite = TRUE;
      mode = old_mode;
    }
  else
    /* If S bit is set and R15 is not in the transfer list then
       force USR mode.  */
    mode = USR_MODE;

  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base--);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base--);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base--);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base--);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base--);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base--);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base--);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base--);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base--);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base--);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base--);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base--);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base--);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base--);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base--);

  *arm_regs[old_mode][rn] = base << 2;
}


/* BLOCK DATA TRANSFER - UP/DOWN bits.  */
void op_stm_u (WORD ins)
{
  WORD base, mode;

  /* post indexed, up, no-write-back, do not load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  if (ins & 0x0001) wwmem (base++, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (base++, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (base++, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (base++, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (base++, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (base++, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (base++, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (base++, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (base++, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (base++, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (base++, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (base++, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (base++, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (base++, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (base++, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (base, addPC (r15, 4));
}

void op_ldm_u (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* post indexed, up, no-write-back, do not load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base++);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base++);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base++);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base++);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base++);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base++);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base++);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base++);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base++);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base++);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base++);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base++);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base++);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base++);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base++);
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (base) & 0x03fffffc);
      PCwrite = TRUE;
    }
}

void op_stm_wu (WORD ins)
{
  WORD rn, base, mode;

  /* post indexed, up, write-back, do not load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  if (ins & 0x0001) wwmem (base++, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (base++, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (base++, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (base++, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (base++, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (base++, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (base++, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (base++, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (base++, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (base++, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (base++, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (base++, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (base++, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (base++, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (base++, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (base++, addPC (r15, 4));
  r(rn) = base << 2;
}

void op_ldm_wu (WORD ins)
{
  WORD rn, base, mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  /* post indexed, up, write-back, do not load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base++);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base++);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base++);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base++);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base++);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base++);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base++);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base++);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base++);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base++);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base++);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base++);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base++);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base++);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base++);
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (base++) & 0x03fffffc);
      PCwrite = TRUE;
    }

  *arm_regs[mode][rn] = base << 2;
}

void op_stm_su (WORD ins)
{
  WORD base, mode;

  /* post indexed, up, no-write-back, load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = USR_MODE;

  if (ins & 0x0001) wwmem (base++, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (base++, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (base++, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (base++, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (base++, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (base++, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (base++, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (base++, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (base++, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (base++, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (base++, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (base++, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (base++, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (base++, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (base++, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (base, addPC (r15, 4));
}

void op_ldm_su (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;

  /* If S bit is set and R15 is not in the transfer list then
     force USR mode.  */
  mode = (ins & 0x8000) ? (r15 & 3) : USR_MODE;

  /* post indexed, up, no-write-back, load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base++);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base++);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base++);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base++);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base++);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base++);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base++);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base++);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base++);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base++);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base++);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base++);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base++);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base++);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base++);
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (base);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (base) & 0xf3fffffc);
      PCwrite = TRUE;
    }
}

void op_stm_wsu (WORD ins)
{
  WORD rn, base, mode;

  /* post indexed, up, write-back, load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = USR_MODE;

  if (ins & 0x0001) wwmem (base++, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (base++, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (base++, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (base++, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (base++, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (base++, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (base++, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (base++, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (base++, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (base++, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (base++, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (base++, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (base++, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (base++, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (base++, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (base++, addPC (r15, 4));
  r(rn) = base << 2;
}

void op_ldm_wsu (WORD ins)
{
  WORD rn, base, mode, old_mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;

  /* If S bit is set and R15 is not in the transfer list then
     force USR mode.  */
  old_mode = (r15 & 3);
  mode = (ins & 0x8000) ? old_mode : USR_MODE;

  /* post indexed, up, write-back, load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (base++);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (base++);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (base++);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (base++);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (base++);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (base++);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (base++);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (base++);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (base++);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (base++);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (base++);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (base++);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (base++);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (base++);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (base++);
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (base++);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (base++) & 0xf3fffffc);
      PCwrite = TRUE;
    }

  *arm_regs[old_mode][rn] = base << 2;
}

/* BLOCK DATA TRANSFER - PRE-INDEXED.  */
void op_stm_p (WORD ins)
{
  WORD base, mode;

  /* pre indexed, down, no-write-back, do not load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  if (ins & 0x8000) wwmem (--base, addPC (r15, 4));
  if (ins & 0x4000) wwmem (--base, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (--base, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (--base, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (--base, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (--base, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (--base, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (--base, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (--base, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (--base, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (--base, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (--base, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (--base, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (--base, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (--base, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (--base, *arm_regs[mode][0]);
}

void op_ldm_p (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* pre indexed, down, no-write-back, do not load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (--base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (--base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (--base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (--base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (--base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (--base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (--base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (--base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (--base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (--base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (--base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (--base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (--base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (--base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (--base);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (--base);
}

void op_stm_wp (WORD ins)
{
  WORD rn, base, mode;

  /* pre indexed, down, write-back, do not load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  if (ins & 0x8000) wwmem (--base, addPC (r15, 4));
  if (ins & 0x4000) wwmem (--base, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (--base, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (--base, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (--base, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (--base, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (--base, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (--base, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (--base, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (--base, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (--base, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (--base, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (--base, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (--base, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (--base, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (--base, *arm_regs[mode][0]);
  r(rn) = base << 2;
}

void op_ldm_wp (WORD ins)
{
  WORD rn, base, mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  /* pre indexed, down, write-back, do not load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (--base) & 0x03fffffc);
      PCwrite = TRUE;
    }
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (--base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (--base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (--base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (--base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (--base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (--base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (--base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (--base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (--base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (--base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (--base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (--base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (--base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (--base);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (--base);

  *arm_regs[mode][rn] = base << 2;
}

void op_stm_sp (WORD ins)
{
  WORD base, mode;

  /* pre indexed, down, no-write-back, load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = USR_MODE;

  if (ins & 0x8000) wwmem (--base, addPC (r15, 4));
  if (ins & 0x4000) wwmem (--base, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (--base, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (--base, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (--base, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (--base, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (--base, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (--base, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (--base, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (--base, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (--base, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (--base, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (--base, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (--base, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (--base, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (--base, *arm_regs[mode][0]);
}

void op_ldm_sp (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* pre indexed, down, no-write-back, load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (--base);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (--base) & 0xf3fffffc);
      PCwrite = TRUE;
    }
  else
    mode = USR_MODE;

  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (--base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (--base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (--base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (--base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (--base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (--base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (--base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (--base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (--base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (--base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (--base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (--base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (--base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (--base);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (--base);
}

void op_stm_wsp (WORD ins)
{
  WORD rn, base, mode;

  /* pre indexed, down, write-back, load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = USR_MODE;

  if (ins & 0x8000) wwmem (--base, addPC (r15, 4));
  if (ins & 0x4000) wwmem (--base, *arm_regs[mode][14]);
  if (ins & 0x2000) wwmem (--base, *arm_regs[mode][13]);
  if (ins & 0x1000) wwmem (--base, *arm_regs[mode][12]);
  if (ins & 0x0800) wwmem (--base, *arm_regs[mode][11]);
  if (ins & 0x0400) wwmem (--base, *arm_regs[mode][10]);
  if (ins & 0x0200) wwmem (--base, *arm_regs[mode][9]);
  if (ins & 0x0100) wwmem (--base, *arm_regs[mode][8]);
  if (ins & 0x0080) wwmem (--base, *arm_regs[mode][7]);
  if (ins & 0x0040) wwmem (--base, *arm_regs[mode][6]);
  if (ins & 0x0020) wwmem (--base, *arm_regs[mode][5]);
  if (ins & 0x0010) wwmem (--base, *arm_regs[mode][4]);
  if (ins & 0x0008) wwmem (--base, *arm_regs[mode][3]);
  if (ins & 0x0004) wwmem (--base, *arm_regs[mode][2]);
  if (ins & 0x0002) wwmem (--base, *arm_regs[mode][1]);
  if (ins & 0x0001) wwmem (--base, *arm_regs[mode][0]);
  r(rn) = base << 2;
}

void op_ldm_wsp (WORD ins)
{
  WORD rn, base, mode, old_mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  old_mode = r15 & 3;

  /* pre indexed, down, write-back, load PSR or force user mode.  */
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (--base);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (--base) & 0xf3fffffc);
      PCwrite = TRUE;
      mode = old_mode;
    }
  else
    mode = USR_MODE;

  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (--base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (--base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (--base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (--base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (--base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (--base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (--base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (--base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (--base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (--base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (--base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (--base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (--base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (--base);
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (--base);

  *arm_regs[old_mode][rn] = base << 2;
}


/* BLOCK DATA TRANSFER - UP/DOWN bits.  */
void op_stm_up (WORD ins)
{
  WORD base, mode;

  /* pre indexed, up, no-write-back, do not load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  if (ins & 0x0001) wwmem (++base, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (++base, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (++base, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (++base, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (++base, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (++base, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (++base, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (++base, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (++base, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (++base, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (++base, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (++base, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (++base, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (++base, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (++base, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (++base, addPC (r15, 4));
}

void op_ldm_up (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = r15 & 3;

  /* pre indexed, up, no-write-back, do not load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (++base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (++base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (++base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (++base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (++base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (++base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (++base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (++base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (++base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (++base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (++base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (++base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (++base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (++base);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (++base);
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (+base) & 0x03fffffc);
      PCwrite = TRUE;
    }
}

void op_stm_wup (WORD ins)
{
  WORD rn, base, mode;

  /* pre indexed, up, write-back, do not load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  if (ins & 0x0001) wwmem (++base, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (++base, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (++base, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (++base, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (++base, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (++base, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (++base, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (++base, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (++base, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (++base, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (++base, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (++base, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (++base, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (++base, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (++base, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (++base, addPC (r15, 4));
  r(rn) = base << 2;
}

void op_ldm_wup (WORD ins)
{
  WORD rn, base, mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = r15 & 3;

  /* pre indexed, up, write-back, do not load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (++base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (++base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (++base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (++base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (++base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (++base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (++base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (++base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (++base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (++base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (++base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (++base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (++base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (++base);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (++base);
  if (ins & 0x8000)
    {
      r15 = (r15 & 0xfc000003) | (rwmem (++base) & 0x03fffffc);
      PCwrite = TRUE;
    }

  *arm_regs[mode][rn] = base << 2;
}

void op_stm_sup (WORD ins)
{
  WORD base, mode;

  /* pre indexed, up, no-write-back, load PSR or force user mode.  */
  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;
  mode = USR_MODE;

  if (ins & 0x0001) wwmem (++base, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (++base, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (++base, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (++base, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (++base, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (++base, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (++base, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (++base, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (++base, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (++base, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (++base, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (++base, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (++base, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (++base, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (++base, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (++base, addPC (r15, 4));
}

void op_ldm_sup (WORD ins)
{
  WORD base, mode;

  base = (ins >> 16) & 0xf;
  base = r(base) >> 2;

  /* If S bit is set and R15 is not in the transfer list then
     force USR mode.  */
  mode = (ins & 0x8000) ? (r15 & 3) : USR_MODE;

  /* pre indexed, up, no-write-back, load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (++base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (++base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (++base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (++base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (++base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (++base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (++base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (++base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (++base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (++base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (++base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (++base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (++base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (++base);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (++base);
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (++base);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (++base) & 0xf3fffffc);
      PCwrite = TRUE;
    }
}

void op_stm_wsup (WORD ins)
{
  WORD rn, base, mode;

  /* pre indexed, up, write-back, load PSR or force user mode.  */
  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;
  mode = USR_MODE;

  if (ins & 0x0001) wwmem (++base, *arm_regs[mode][0]);
  if (ins & 0x0002) wwmem (++base, *arm_regs[mode][1]);
  if (ins & 0x0004) wwmem (++base, *arm_regs[mode][2]);
  if (ins & 0x0008) wwmem (++base, *arm_regs[mode][3]);
  if (ins & 0x0010) wwmem (++base, *arm_regs[mode][4]);
  if (ins & 0x0020) wwmem (++base, *arm_regs[mode][5]);
  if (ins & 0x0040) wwmem (++base, *arm_regs[mode][6]);
  if (ins & 0x0080) wwmem (++base, *arm_regs[mode][7]);
  if (ins & 0x0100) wwmem (++base, *arm_regs[mode][8]);
  if (ins & 0x0200) wwmem (++base, *arm_regs[mode][9]);
  if (ins & 0x0400) wwmem (++base, *arm_regs[mode][10]);
  if (ins & 0x0800) wwmem (++base, *arm_regs[mode][11]);
  if (ins & 0x1000) wwmem (++base, *arm_regs[mode][12]);
  if (ins & 0x2000) wwmem (++base, *arm_regs[mode][13]);
  if (ins & 0x4000) wwmem (++base, *arm_regs[mode][14]);
  if (ins & 0x8000) wwmem (++base, addPC (r15, 4));
  r(rn) = base << 2;
}

void op_ldm_wsup (WORD ins)
{
  WORD rn, base, mode, old_mode;

  rn = (ins >> 16) & 0xf;
  base = r(rn) >> 2;

  /* If S bit is set and R15 is not in the transfer list then
     force USR mode.  */
  old_mode = (r15 & 3);
  mode = (ins & 0x8000) ? old_mode : USR_MODE;

  /* pre indexed, up, write-back, load PSR or force user mode.  */
  if (ins & 0x0001) *arm_regs[mode][0] = rwmem (++base);
  if (ins & 0x0002) *arm_regs[mode][1] = rwmem (++base);
  if (ins & 0x0004) *arm_regs[mode][2] = rwmem (++base);
  if (ins & 0x0008) *arm_regs[mode][3] = rwmem (++base);
  if (ins & 0x0010) *arm_regs[mode][4] = rwmem (++base);
  if (ins & 0x0020) *arm_regs[mode][5] = rwmem (++base);
  if (ins & 0x0040) *arm_regs[mode][6] = rwmem (++base);
  if (ins & 0x0080) *arm_regs[mode][7] = rwmem (++base);
  if (ins & 0x0100) *arm_regs[mode][8] = rwmem (++base);
  if (ins & 0x0200) *arm_regs[mode][9] = rwmem (++base);
  if (ins & 0x0400) *arm_regs[mode][10] = rwmem (++base);
  if (ins & 0x0800) *arm_regs[mode][11] = rwmem (++base);
  if (ins & 0x1000) *arm_regs[mode][12] = rwmem (++base);
  if (ins & 0x2000) *arm_regs[mode][13] = rwmem (++base);
  if (ins & 0x4000) *arm_regs[mode][14] = rwmem (++base);
  if (ins & 0x8000)
    {
      if (mode)
	r15 = rwmem (++base);
      else
	r15 = (r15 & 0x0c000003) | (rwmem (++base) & 0xf3fffffc);
      PCwrite = TRUE;
    }

  /* Writeback goes to the register in the original mode
     and not to the register in the new mode, iff r15 was
     changed by this instruction.  */
  *arm_regs[old_mode][rn] = base << 2;
}


/* Bits 20 to 27.  */
void (*main_decode[256]) (WORD) =
{
  /* Data processing group.        00 I OPCO S */
  op_and,                       /* 00 0 0000 0 -- AND (MUL) */
  op_and_s,                     /* 00 0 0000 1 -- ANDS (MULS) */
  op_eor,                       /* 00 0 0001 0 -- EOR (MLA) */
  op_eor_s,			/* 00 0 0001 1 -- EORS (MLAS) */ 
  op_sub,			/* 00 0 0010 0 -- SUB */
  op_sub_s,			/* 00 0 0010 1 -- SUBS */
  op_rsb,			/* 00 0 0011 0 -- RSB */
  op_rsb_s,			/* 00 0 0011 1 -- RSBS */
  op_add,			/* 00 0 0100 0 -- ADD */
  op_add_s,			/* 00 0 0100 1 -- ADDS */
  op_adc,			/* 00 0 0101 0 -- ADC */
  op_adc_s,			/* 00 0 0101 1 -- ADCS */
  op_sbc,			/* 00 0 0110 0 -- SBC */
  op_sbc_s,			/* 00 0 0110 1 -- SBCS */
  op_rsc,			/* 00 0 0111 0 -- RSC */
  op_rsc_s,			/* 00 0 0111 1 -- RSCS */

  op_tst_s,			/* 00 0 1000 0 -- TSTP (SWP) */
  op_tst_s,			/* 00 0 1000 1 -- TSTS */
  op_teq_s,			/* 00 0 1001 0 -- TEQP */
  op_teq_s,			/* 00 0 1001 1 -- TEQS */ 
  op_cmp_s,			/* 00 0 1010 0 -- CMPP (SWPB) */
  op_cmp_s,			/* 00 0 1010 1 -- CMPS */
  op_cmn_s,			/* 00 0 1011 0 -- CMNP */
  op_cmn_s,			/* 00 0 1011 1 -- CMNS */
  op_orr,			/* 00 0 1100 0 -- ORR */
  op_orr_s,			/* 00 0 1100 1 -- ORRS */
  op_mov,			/* 00 0 1101 0 -- MOV */
  op_mov_s,			/* 00 0 1101 1 -- MOVS */
  op_bic,			/* 00 0 1110 0 -- BIC */
  op_bic_s,			/* 00 0 1110 1 -- BICS */
  op_mvn,			/* 00 0 1111 0 -- MVN */
  op_mvn_s,			/* 00 0 1111 1 -- MVNS */

  /* Data processing - immediate form.  */
  op_and_i,			/* 00 1 0000 0 -- AND */
  op_and_si,			/* 00 1 0000 1 -- ANDS */
  op_eor_i,			/* 00 1 0001 0 -- EOR */
  op_eor_si,			/* 00 1 0001 1 -- EORS */ 
  op_sub_i,			/* 00 1 0010 0 -- SUB */
  op_sub_si,			/* 00 1 0010 1 -- SUBS */
  op_rsb_i,			/* 00 1 0011 0 -- RSB */
  op_rsb_si,			/* 00 1 0011 1 -- RSBS */
  op_add_i,			/* 00 1 0100 0 -- ADD */
  op_add_si,			/* 00 1 0100 1 -- ADDS */
  op_adc_i,			/* 00 1 0101 0 -- ADC */
  op_adc_si,			/* 00 1 0101 1 -- ADCS */
  op_sbc_i,			/* 00 1 0110 0 -- SBC */
  op_sbc_si,			/* 00 1 0110 1 -- SBCS */
  op_rsc_i,			/* 00 1 0111 0 -- RSC */
  op_rsc_si,			/* 00 1 0111 1 -- RSCS */

  op_tst_si,			/* 00 1 1000 0 -- TSTP */
  op_tst_si,			/* 00 1 1000 1 -- TSTS */
  op_teq_si,			/* 00 1 1001 0 -- TEQP */
  op_teq_si,			/* 00 1 1001 1 -- TEQS */ 
  op_cmp_si,			/* 00 1 1010 0 -- CMPP */
  op_cmp_si,			/* 00 1 1010 1 -- CMPS */
  op_cmn_si,			/* 00 1 1011 0 -- CMNP */
  op_cmn_si,			/* 00 1 1011 1 -- CMNS */
  op_orr_i,			/* 00 1 1100 0 -- ORR */
  op_orr_si,			/* 00 1 1100 1 -- ORRS */
  op_mov_i,			/* 00 1 1101 0 -- MOV */
  op_mov_si,			/* 00 1 1101 1 -- MOVS */
  op_bic_i,			/* 00 1 1110 0 -- BIC */
  op_bic_si,			/* 00 1 1110 1 -- BICS */
  op_mvn_i,			/* 00 1 1111 0 -- MVN */
  op_mvn_si,			/* 00 1 1111 1 -- MVNS */

  /* Single data transfer          01 I P U B W L */
  op_str,			/* 01 0 0 0 0 0 0 - STR */
  op_ldr,			/* 01 0 0 0 0 0 1 - LDR */
  op_str_w,			/* 01 0 0 0 0 1 0 - STR (wback) */
  op_ldr_w,			/* 01 0 0 0 0 1 1 - LDR (wback) */
  op_strb,			/* 01 0 0 0 1 0 0 - STRB */
  op_ldrb,			/* 01 0 0 0 1 0 1 - LDRB */
  op_strb_w,			/* 01 0 0 0 1 1 0 - STRB (wback) */
  op_ldrb_w,			/* 01 0 0 0 1 1 1 - LDRB (wback) */

  op_str_u,			/* 01 0 0 1 0 0 0 - STR (up) */
  op_ldr_u,			/* 01 0 0 1 0 0 1 - LDR (up) */
  op_str_wu,			/* 01 0 0 1 0 1 0 - STR (wback, up) */
  op_ldr_wu,			/* 01 0 0 1 0 1 1 - LDR (wback, up)*/
  op_strb_u,			/* 01 0 0 1 1 0 0 - STRB (up) */
  op_ldrb_u,			/* 01 0 0 1 1 0 1 - LDRB (up) */
  op_strb_wu,			/* 01 0 0 1 1 1 0 - STRB (wback, up) */
  op_ldrb_wu,			/* 01 0 0 1 1 1 1 - LDRB (wback, up) */

  op_str_p,			/* 01 0 1 0 0 0 0 - STR (pre) */
  op_ldr_p,			/* 01 0 1 0 0 0 1 - LDR (pre) */
  op_str_wp,			/* 01 0 1 0 0 1 0 - STR (wback, pre) */
  op_ldr_wp,			/* 01 0 1 0 0 1 1 - LDR (wback, pre) */
  op_strb_p,			/* 01 0 1 0 1 0 0 - STRB (pre) */
  op_ldrb_p,			/* 01 0 1 0 1 0 1 - LDRB (pre) */
  op_strb_wp,			/* 01 0 1 0 1 1 0 - STRB (wback, pre) */
  op_ldrb_wp,			/* 01 0 1 0 1 1 1 - LDRB (wback, pre) */

  op_str_up,			/* 01 0 1 1 0 0 0 - STR (up, pre) */
  op_ldr_up,			/* 01 0 1 1 0 0 1 - LDR (up, pre) */
  op_str_wup,			/* 01 0 1 1 0 1 0 - STR (wback, up, pre) */
  op_ldr_wup,			/* 01 0 1 1 0 1 1 - LDR (wback, up, pre) */
  op_strb_up,			/* 01 0 1 1 1 0 0 - STRB (up, pre) */
  op_ldrb_up,			/* 01 0 1 1 1 0 1 - LDRB (up, pre) */
  op_strb_wup,			/* 01 0 1 1 1 1 0 - STRB (wback, up, pre) */
  op_ldrb_wup,			/* 01 0 1 1 1 1 1 - LDRB (wback, up, pre) */

  /* Single data transfer (if bit 4 is set, then undefined instruction)
			           01 I P U B W L (immediate offset) */
  op_str_i,			/* 01 1 0 0 0 0 0 - STR */
  op_ldr_i,			/* 01 1 0 0 0 0 1 - LDR */
  op_str_wi,			/* 01 1 0 0 0 1 0 - STR (wback) */
  op_ldr_wi,			/* 01 1 0 0 0 1 1 - LDR (wback) */
  op_strb_i,			/* 01 1 0 0 1 0 0 - STRB */
  op_ldrb_i,			/* 01 1 0 0 1 0 1 - LDRB */
  op_strb_wi,			/* 01 1 0 0 1 1 0 - STRB (wback) */
  op_ldrb_wi,			/* 01 1 0 0 1 1 1 - LDRB (wback) */

  op_str_ui,			/* 01 1 0 1 0 0 0 - STR (up) */
  op_ldr_ui,			/* 01 1 0 1 0 0 1 - LDR (up) */
  op_str_wui,			/* 01 1 0 1 0 1 0 - STR (wback, up) */
  op_ldr_wui,			/* 01 1 0 1 0 1 1 - LDR (wback, up)*/
  op_strb_ui,			/* 01 1 0 1 1 0 0 - STRB (up) */
  op_ldrb_ui,			/* 01 1 0 1 1 0 1 - LDRB (up) */
  op_strb_wui,			/* 01 1 0 1 1 1 0 - STRB (wback, up) */
  op_ldrb_wui,			/* 01 1 0 1 1 1 1 - LDRB (wback, up) */

  op_str_pi,			/* 01 1 1 0 0 0 0 - STR (pre) */
  op_ldr_pi,			/* 01 1 1 0 0 0 1 - LDR (pre) */
  op_str_wpi,			/* 01 1 1 0 0 1 0 - STR (wback, pre) */
  op_ldr_wpi,			/* 01 1 1 0 0 1 1 - LDR (wback, pre) */
  op_strb_pi,			/* 01 1 1 0 1 0 0 - STRB (pre) */
  op_ldrb_pi,			/* 01 1 1 0 1 0 1 - LDRB (pre) */
  op_strb_wpi,			/* 01 1 1 0 1 1 0 - STRB (wback, pre) */
  op_ldrb_wpi,			/* 01 1 1 0 1 1 1 - LDRB (wback, pre) */

  op_str_upi,			/* 01 1 1 1 0 0 0 - STR (up, pre) */
  op_ldr_upi,			/* 01 1 1 1 0 0 1 - LDR (up, pre) */
  op_str_wupi,			/* 01 1 1 1 0 1 0 - STR (wback, up, pre) */
  op_ldr_wupi,			/* 01 1 1 1 0 1 1 - LDR (wback, up, pre) */
  op_strb_upi,			/* 01 1 1 1 1 0 0 - STRB (up, pre) */
  op_ldrb_upi,			/* 01 1 1 1 1 0 1 - LDRB (up, pre) */
  op_strb_wupi,			/* 01 1 1 1 1 1 0 - STRB (wback, up, pre) */
  op_ldrb_wupi,			/* 01 1 1 1 1 1 1 - LDRB (wback, up, pre) */

  /* Block data transfer           100 P U S W L */
  op_stm,			/* 100 0 0 0 0 0 - STM */
  op_ldm,			/* 100 0 0 0 0 1 - LDM */
  op_stm_w,			/* 100 0 0 0 1 0 - STM (wb) */
  op_ldm_w,			/* 100 0 0 0 1 1 - LDM (wb) */
  op_stm_s,			/* 100 0 0 1 0 0 - STM (psr) */
  op_ldm_s,			/* 100 0 0 1 0 1 - LDM (psr) */
  op_stm_ws,			/* 100 0 0 1 1 0 - STM (wb, psr) */
  op_ldm_ws,			/* 100 0 0 1 1 1 - LDM (wb, psr) */

  op_stm_u,			/* 100 0 1 0 0 0 - STM (up) */
  op_ldm_u,			/* 100 0 1 0 0 1 - LDM (up) */
  op_stm_wu,			/* 100 0 1 0 1 0 - STM (wb, up) */
  op_ldm_wu,			/* 100 0 1 0 1 1 - LDM (wb, up) */
  op_stm_su,			/* 100 0 1 1 0 0 - STM (psr, up) */
  op_ldm_su,			/* 100 0 1 1 0 1 - LDM (psr, up) */
  op_stm_wsu,			/* 100 0 1 1 1 0 - STM (wb, psr, up) */
  op_ldm_wsu,			/* 100 0 1 1 1 1 - LDM (wb, psr, up) */

  op_stm_p,			/* 100 1 0 0 0 0 - STM (pre) */
  op_ldm_p,			/* 100 1 0 0 0 1 - LDM (pre) */
  op_stm_wp,			/* 100 1 0 0 1 0 - STM (wb, pre) */
  op_ldm_wp,			/* 100 1 0 0 1 1 - LDM (wb, pre) */
  op_stm_sp,			/* 100 1 0 1 0 0 - STM (psr, pre) */
  op_ldm_sp,			/* 100 1 0 1 0 1 - LDM (psr, pre) */
  op_stm_wsp,			/* 100 1 0 1 1 0 - STM (wb, psr, pre) */
  op_ldm_wsp,			/* 100 1 0 1 1 1 - LDM (wb, psr, pre) */

  op_stm_up,			/* 100 1 1 0 0 0 - STM (up, pre) */
  op_ldm_up,			/* 100 1 1 0 0 1 - LDM (up, pre) */
  op_stm_wup,			/* 100 1 1 0 1 0 - STM (wb, up, pre) */
  op_ldm_wup,			/* 100 1 1 0 1 1 - LDM (wb, up, pre) */
  op_stm_sup,			/* 100 1 1 1 0 0 - STM (psr, up, pre) */
  op_ldm_sup,			/* 100 1 1 1 0 1 - LDM (psr, up, pre) */
  op_stm_wsup,			/* 100 1 1 1 1 0 - STM (wb, psr, up, pre) */
  op_ldm_wsup,			/* 100 1 1 1 1 1 - LDM (wb, psr, up, pre) */

  /* Branch			   101 L x x x x */
  op_b,				/* 101 0 0 0 0 0 - B address */
  op_b,				/* 101 0 0 0 0 1 - B address */
  op_b,				/* 101 0 0 0 1 0 - B address */
  op_b,				/* 101 0 0 0 1 1 - B address */
  op_b,				/* 101 0 0 1 0 0 - B address */
  op_b,				/* 101 0 0 1 0 1 - B address */
  op_b,				/* 101 0 0 1 1 0 - B address */
  op_b,				/* 101 0 0 1 1 1 - B address */
  op_b,				/* 101 0 1 0 0 0 - B address */
  op_b,				/* 101 0 1 0 0 1 - B address */
  op_b,				/* 101 0 1 0 1 0 - B address */
  op_b,				/* 101 0 1 0 1 1 - B address */
  op_b,				/* 101 0 1 1 0 0 - B address */
  op_b,				/* 101 0 1 1 0 1 - B address */
  op_b,				/* 101 0 1 1 1 0 - B address */
  op_b,				/* 101 0 1 1 1 1 - B address */

  op_bl,       			/* 101 1 0 0 0 0 - BL address */
  op_bl,       			/* 101 1 0 0 0 1 - BL address */
  op_bl,       			/* 101 1 0 0 1 0 - BL address */
  op_bl,       			/* 101 1 0 0 1 1 - BL address */
  op_bl,       			/* 101 1 0 1 0 0 - BL address */
  op_bl,       			/* 101 1 0 1 0 1 - BL address */
  op_bl,       			/* 101 1 0 1 1 0 - BL address */
  op_bl,       			/* 101 1 0 1 1 1 - BL address */
  op_bl,       			/* 101 1 1 0 0 0 - BL address */
  op_bl,       			/* 101 1 1 0 0 1 - BL address */
  op_bl,       			/* 101 1 1 0 1 0 - BL address */
  op_bl,       			/* 101 1 1 0 1 1 - BL address */
  op_bl,       			/* 101 1 1 1 0 0 - BL address */
  op_bl,       			/* 101 1 1 1 0 1 - BL address */
  op_bl,       			/* 101 1 1 1 1 0 - BL address */
  op_bl,       			/* 101 1 1 1 1 1 - BL address */

#ifdef FPE_COPRO
  /* Coprocessor data transfer	   110 P U N W L - undefined */
  fpe_stf_sfm,			/* 110 0 0 0 0 0 - undefined */
  fpe_ldf_lfm,			/* 110 0 0 0 0 1 - undefined */
  undefined,			/* 110 0 0 0 1 0 - undefined */
  undefined,			/* 110 0 0 0 1 1 - undefined */
  undefined,			/* 110 0 0 1 0 0 - undefined */
  undefined,			/* 110 0 0 1 0 1 - undefined */
  undefined,			/* 110 0 0 1 1 0 - undefined */
  undefined,			/* 110 0 0 1 1 1 - undefined */
  undefined,			/* 110 0 1 0 0 0 - undefined */
  undefined,			/* 110 0 1 0 0 1 - undefined */
  undefined,			/* 110 0 1 0 1 0 - undefined */
  undefined,			/* 110 0 1 0 1 1 - undefined */
  undefined,			/* 110 0 1 1 0 0 - undefined */
  undefined,			/* 110 0 1 1 0 1 - undefined */
  undefined,			/* 110 0 1 1 1 0 - undefined */
  undefined,			/* 110 0 1 1 1 1 - undefined */
#else
  /* Coprocessor data transfer	   110 P U N W L - undefined */
  undefined,			/* 110 0 0 0 0 0 - undefined */
  undefined,			/* 110 0 0 0 0 1 - undefined */
  undefined,			/* 110 0 0 0 1 0 - undefined */
  undefined,			/* 110 0 0 0 1 1 - undefined */
  undefined,			/* 110 0 0 1 0 0 - undefined */
  undefined,			/* 110 0 0 1 0 1 - undefined */
  undefined,			/* 110 0 0 1 1 0 - undefined */
  undefined,			/* 110 0 0 1 1 1 - undefined */
  undefined,			/* 110 0 1 0 0 0 - undefined */
  undefined,			/* 110 0 1 0 0 1 - undefined */
  undefined,			/* 110 0 1 0 1 0 - undefined */
  undefined,			/* 110 0 1 0 1 1 - undefined */
  undefined,			/* 110 0 1 1 0 0 - undefined */
  undefined,			/* 110 0 1 1 0 1 - undefined */
  undefined,			/* 110 0 1 1 1 0 - undefined */
  undefined,			/* 110 0 1 1 1 1 - undefined */
#endif

  undefined,			/* 110 1 0 0 0 0 - undefined */
  undefined,			/* 110 1 0 0 0 1 - undefined */
  undefined,			/* 110 1 0 0 1 0 - undefined */
  undefined,			/* 110 1 0 0 1 1 - undefined */
  undefined,			/* 110 1 0 1 0 0 - undefined */
  undefined,			/* 110 1 0 1 0 1 - undefined */
  undefined,			/* 110 1 0 1 1 0 - undefined */
  undefined,			/* 110 1 0 1 1 1 - undefined */
  undefined,			/* 110 1 1 0 0 0 - undefined */
  undefined,			/* 110 1 1 0 0 1 - undefined */
  undefined,			/* 110 1 1 0 1 0 - undefined */
  undefined,			/* 110 1 1 0 1 1 - undefined */
  undefined,			/* 110 1 1 1 0 0 - undefined */
  undefined,			/* 110 1 1 1 0 1 - undefined */
  undefined,			/* 110 1 1 1 1 0 - undefined */
  undefined,			/* 110 1 1 1 1 1 - undefined */

#ifdef FPE_COPRO
  /* Coprocessor data op	   1110 x x x x - undefined */
  fpe_adf_mvf_flt,		/* 1110 0 0 0 0 - undefined */
  fpe_muf_mnf_fix,		/* 1110 0 0 0 1 - undefined */
  fpe_suf_abs_wfs,		/* 1110 0 0 1 0 - undefined */
  fpe_rsf_rnd_rfs,		/* 1110 0 0 1 1 - undefined */
  fpe_dvf_sqt_wfc,		/* 1110 0 1 0 0 - undefined */
  fpe_rdf_log_rfc,		/* 1110 0 1 0 1 - undefined */
  fpe_pow_lgn,			/* 1110 0 1 1 0 - undefined */
  fpe_rpw_exp,			/* 1110 0 1 1 1 - undefined */
  fpe_rmf_sin,			/* 1110 1 0 0 0 - undefined */
  fpe_fml_cos_cmf,		/* 1110 1 0 0 1 - undefined */
  fpe_fdv_tan,			/* 1110 1 0 1 0 - undefined */
  fpe_frd_asn_cnf,		/* 1110 1 0 1 1 - undefined */
  fpe_pol_acs,			/* 1110 1 1 0 0 - undefined */
  fpe_cme_atn,			/* 1110 1 1 0 1 - undefined */
  fpe_urd,			/* 1110 1 1 1 0 - undefined */
  fpe_nrm_cne,			/* 1110 1 1 1 1 - undefined */
#else
  /* Coprocessor data op	   1110 x x x x - undefined */
  undefined,			/* 1110 0 0 0 0 - undefined */
  undefined,			/* 1110 0 0 0 1 - undefined */
  undefined,			/* 1110 0 0 1 0 - undefined */
  undefined,			/* 1110 0 0 1 1 - undefined */
  undefined,			/* 1110 0 1 0 0 - undefined */
  undefined,			/* 1110 0 1 0 1 - undefined */
  undefined,			/* 1110 0 1 1 0 - undefined */
  undefined,			/* 1110 0 1 1 1 - undefined */
  undefined,			/* 1110 1 0 0 0 - undefined */
  undefined,			/* 1110 1 0 0 1 - undefined */
  undefined,			/* 1110 1 0 1 0 - undefined */
  undefined,			/* 1110 1 0 1 1 - undefined */
  undefined,			/* 1110 1 1 0 0 - undefined */
  undefined,			/* 1110 1 1 0 1 - undefined */
  undefined,			/* 1110 1 1 1 0 - undefined */
  undefined,			/* 1110 1 1 1 1 - undefined */
#endif

  /* Software interrupt		   1111 x x x x - SWI */
  swi,				/* 1111 0 0 0 0 - swi */
  swi,				/* 1111 0 0 0 1 - swi */
  swi,				/* 1111 0 0 1 0 - swi */
  swi,				/* 1111 0 0 1 1 - swi */
  swi,				/* 1111 0 1 0 0 - swi */
  swi,				/* 1111 0 1 0 1 - swi */
  swi,				/* 1111 0 1 1 0 - swi */
  swi,				/* 1111 0 1 1 1 - swi */
  swi,				/* 1111 1 0 0 0 - swi */
  swi,				/* 1111 1 0 0 1 - swi */
  swi,				/* 1111 1 0 1 0 - swi */
  swi,				/* 1111 1 0 1 1 - swi */
  swi,				/* 1111 1 1 0 0 - swi */
  swi,				/* 1111 1 1 0 1 - swi */
  swi,				/* 1111 1 1 1 0 - swi */
  swi,				/* 1111 1 1 1 1 - swi */
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

      /* if (pc >= (ROM >> 2))
	longjmp (exception, VECTOR_ABORT_PREFETCH);
      */
      ins = wmem[pc];
      ins = SWAPWORD (ins);

      cond = (ins & 0xf0000000) >> 28;

      switch (cond)
	{
	case 0: /* EQ */
	  if (Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 1: /* NE */
	  if (!Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 2: /* CS */
	  if (C_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 3: /* CC */
	  if (!C_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 4: /* MI */
	  if (N_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 5: /* PL */
	  if (!N_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 6: /* VS */
	  if (V_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 7: /* VC */
	  if (!V_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 8: /* HI */
	  if (C_FLAG & !Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 9: /* LS */
	  if (!C_FLAG | Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 10: /* GE */
	  if ((N_FLAG & V_FLAG) | (!N_FLAG & !V_FLAG))
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 11: /* LT */
	  if ((N_FLAG & !V_FLAG) | (!N_FLAG & V_FLAG))
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 12: /* GT */
	  if (((N_FLAG & V_FLAG) | (!N_FLAG & !V_FLAG)) & !Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 13: /* LE */
	  if ((N_FLAG & !V_FLAG) | (!N_FLAG & V_FLAG) | Z_FLAG)
	    main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	case 14: /* AL */
	  main_decode[(ins >> 20) & 0xff] (ins);
	  break;
	}

      total_instructions ++;

#if 0
      if (cond == 14 || conditie[cond] ())
	{
	  main_decode[(ins >> 20) & 0xff] (ins);
	}
#endif

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

  printf ("ARM2 emulator v1.01 - memory size %d Kbytes\n\n", MEMSIZE / 1024);

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
  initialise_immediates ();
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

  printf ("\nARM2EMU - Aborted by unknown reason!\n");

  return (0);
}

#define ROTATER(n,b) (((n)>>(b))|((n)<<(32-(b))))

static void initialise_immediates (void)
{
  unsigned long i;

  for (i = 0; i < 4096; i++)
    {				/* the values of 12 bit dp rhs's */
      immediate_table[i] = ROTATER (i & 0xffL, (i >> 7L) & 0x1eL);
    }
}

static void initialise_armrom (void)
{
  int x;

  for (x = 0; x < sizeof (armrom_code); x += 4)
    wmem[x >> 2] = armrom_code[x >> 2];
}

static void initialise_fpe (void)
{
  int x;

  /* Store the FPE in ROM.  */
  for (x = 0; x < fpesize; x += 4)
     wmem[(RAM + x) >> 2] = fpecode[x >> 2]; 

  /* Fix the Undefined Instruction vector.  */
  wmem[VECTOR_UNDEFINED_INSTRUCTION >> 2] = 0xea000000 | (((RAM + 0x278) >> 2) - 3);
}


