/*
 * ucontext.h: sigcontext structure adapted from Linux headers
 */

#ifndef _UCONTEXT_H
#define _UCONTEXT_H	1

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

#endif
