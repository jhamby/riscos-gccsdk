/*
 * ucontext.h: sigcontext structure adapted from Linux headers
 */

#ifndef _UCONTEXT_H
#define _UCONTEXT_H	1

#include <unixlib/types.h>
#include <signal.h>

typedef int greg_t;

/* Number of general registers.  */
#define NGREG   16

/* Container for all general registers.  */
typedef greg_t gregset_t[NGREG];

/* Number of each register is the `gregset_t' array.  */
enum
{
  R0 = 0,
#define R0  R0
  R1 = 1,
#define R1  R1
  R2 = 2,
#define R2  R2
  R3 = 3,
#define R3  R3
  R4 = 4,
#define R4  R4
  R5 = 5,
#define R5  R5
  R6 = 6,
#define R6  R6
  R7 = 7,
#define R7  R7
  R8 = 8,
#define R8  R8
  R9 = 9,
#define R9  R9
  R10 = 10,
#define R10 R10
  R11 = 11,
#define R11 R11
  R12 = 12,
#define R12 R12
  R13 = 13,
#define R13 R13
  R14 = 14,
#define R14 R14
  R15 = 15,
#define R15 R15
};

/* Structure to describe FPU registers.  */
typedef struct fpregset
  {
  } fpregset_t;

/* Context to describe whole processor state.  */
typedef struct mcontext
  {
    gregset_t gregs;
    fpregset_t fpregs;
  } mcontext_t;

#define PC(ctx) (ctx.arm_pc)

/*
 * Signal context structure - contains the state of the registers
 * at the time of the signal. Valid for SIGSEGV, SIGBUS and SIGILL.
 */
struct sigcontext {
	unsigned long arm_r0;
	unsigned long arm_r1;
	unsigned long arm_r2;
	unsigned long arm_r3;
	unsigned long arm_r4;
	unsigned long arm_r5;
	unsigned long arm_r6;
	unsigned long arm_r7;
	unsigned long arm_r8;
	unsigned long arm_r9;
	unsigned long arm_r10;
	unsigned long arm_fp;
	unsigned long arm_ip;
	unsigned long arm_sp;
	unsigned long arm_lr;
	unsigned long arm_pc;
	unsigned long arm_cpsr;
	unsigned long fault_address;
};

typedef struct _ucontext_t
{
  struct _ucontext_t *uc_link;
  sigset_t uc_sigmask;
  void *uc_stack;
  mcontext_t uc_mcontext;
} ucontext_t;

#endif
