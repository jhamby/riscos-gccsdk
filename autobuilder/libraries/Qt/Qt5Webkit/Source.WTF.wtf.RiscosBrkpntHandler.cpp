/* RiscosBrkpntHandler.cpp
 * 
 * Written by Lee Noar.
 * 
 * Handler to dump the processor registers when a SWI "OS_BreakPt"
 * instruction is executed in JIT code. The following instruction
 * is jumped over, and can be used as a kind of ID word when
 * trying to identify what generated the break point.
 */

#include <stdlib.h>
#include <stdio.h>
#include "oslib/os.h"
#include "swis.h"

#include "config.h"

#define BREAKPT_HANDLER_RETURNS 1
#define BREAKPT_HANDLER_TO_STDOUT 0
#define BREAKPT_HANDLER_TO_REPORTER 1

#define N_FLAG (1 << 31)
#define Z_FLAG (1 << 30)
#define C_FLAG (1 << 29)
#define V_FLAG (1 << 28)

#define I_FLAG (1 << 27)

#define USR_MODE 0x0
#define FIQ_MODE 0x1
#define IRQ_MODE 0x2
#define SVC_MODE 0x3

namespace riscos {

static os_register_block breakpoint_registers;
static asm_routine old_breakpoint_handler;
static byte *old_breakpoint_registers;
static byte *old_breakpoint_r12;

extern void __pthread_disable_ints(void);
extern void __pthread_enable_ints(void);

/* 32 bit hex to string
 * return pointer to terminating null in buffer
 */
static inline char *
hex_to_string(unsigned int __v, char *__buffer, int __l)
{
  register unsigned int v asm("r0") = __v;
  register char *buffer asm("r1") = __buffer;
  register int l asm("r2") = __l;
  register char *term asm("r1");

  __asm__ volatile ("	SWI	%[os_converthex8];\n"
		: "=r" (term)
		: "r" (v), "r" (buffer), "rI" (l), [os_converthex8] "i" (0xD4)
		: "cc");
  return term;
}

/* Signed 32 bit integer to string
 * return pointer to terminating null in buffer
 */
static inline char *
sint32_to_string(int v, char *buffer, int l)
{
char *term;

  __asm__ volatile ("	MOV	r0, %1;\n"
		"	MOV	r1, %2;\n"
		"	MOV	r2, %3;\n"
		"	SWI	%[os_convertint4];\n"
		"	MOV	%0, r1;\n"
		: "=r" (term)
		: "r" (v), "r" (buffer), "rI" (l), [os_convertint4] "i" (0xDC)
		: "a1", "a2", "a3", "cc");
  return term;
}

static inline void print_text(const char *s)
{
  __asm__ volatile ("	mov r0, %[s];\n"
		"	swi %[SWI_OS_Write0];\n"
		: /* no outputs */
		: [s] "r" (s),
		  [SWI_OS_Write0] "i" (0x2)
		: "a1", "lr", "cc");
}

static inline void print_hex(unsigned int v)
{
  __asm__ volatile ("	mov r0, %[v];\n"
		"	sub r1, sp, #20;\n"
		"	mov r2, #20;\n"
		"	swi %[SWI_OS_ConvertHex4];\n"
		"	mov r2, #0;\n"
		"	strb r2, [r1, #0];\n"
		"	swi %[SWI_OS_Write0];\n"
		: /* no outputs */
		: [v] "r" (v),
		  [SWI_OS_ConvertHex4] "i" (0xD4),
		  [SWI_OS_Write0] "i" (0x2)
		: "a1", "a2", "a3", "lr", "cc");
}

static inline void print_dec(unsigned int v)
{
  __asm__ volatile ("	mov r0, %[v];\n"
  		"	sub r1, sp, #20;\n"
		"	mov r2, #20;\n"
		"	swi %[SWI_OS_ConvertInteger4];\n"
		"	mov r2, #0;\n"
		"	strb r2, [r1, #0];\n"
		"	swi %[SWI_OS_Write0];\n"
		: /* no outputs */
		: [v] "r" (v),
		  [SWI_OS_ConvertInteger4] "i" (0xDC),
		  [SWI_OS_Write0] "i" (0x2)
		: "a1", "a2", "a3", "lr", "cc");
}

static inline void print_nl(void)
{
  __asm__ volatile ("	swi 0x10a;\n"
		"	swi 0x10d;\n"
	       ::: "lr");
}

static inline void
report_text0(const char *__s)
{
  register const char *s asm("r0") = __s;

  __asm__ volatile ("	SWI	%[report_text0];\n"
		: /* no outputs */
		: "r" (s), [report_text0] "i" (0x54C80)
		: "cc");
}

static inline void
report_hex(unsigned int v)
{
  asm volatile ("	mov r0, %[v];\n"
		"	sub r1, sp, #20;\n"
		"	mov r2, #20;\n"
		"	swi %[SWI_OS_ConvertHex4];\n"
		"	mov r2, #0;\n"
		"	strb r2, [r1, #0];\n"
		"	swi %[SWI_ReportText0];\n"
		: /* no outputs */
		: [v] "r" (v),
		  [SWI_OS_ConvertHex4] "i" (0xD4),
		  [SWI_ReportText0] "i" (0x54C80)
		: "a1", "a2", "a3", "cc");
}

static void
restoreBreakpointHandler (void)
{
  if (old_breakpoint_handler &&
      old_breakpoint_r12 &&
      old_breakpoint_registers)
  {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _swix(OS_ChangeEnvironment, _INR(0,3)|_OUTR(1,3),
	  os_HANDLER_BREAK_PT,
	  old_breakpoint_handler,
	  old_breakpoint_r12,
	  old_breakpoint_registers,
	  NULL,
	  NULL,
	  NULL);
#else
    xos_change_environment (os_HANDLER_BREAK_PT,
			    old_breakpoint_handler,
			    old_breakpoint_r12,
			    old_breakpoint_registers,
			    NULL,
			    NULL,
			    NULL);
#endif
    old_breakpoint_handler = NULL;
    old_breakpoint_r12 = NULL;
    old_breakpoint_registers = NULL;
  }
}

static void __attribute__ ((naked))
breakpoint_handler (void)
{
    asm volatile ("	MRS	r0, CPSR;\n"
		  "	BIC	r0, r0, #0x1F;\n"
		  "	ORR	r0, r0, #0x10;\n"	// USR32 Mode
		  "	MSR	CPSR_c, r0;\n"
		  "	MOV	r0, r0;\n"		// NOP for StrongARM MSR bug
		  "	SUB	r4, sp, #60;\n"
		  "\n"
		  "	BL	__pthread_disable_ints;\n"
		  "\n"
#if BREAKPT_HANDLER_TO_REPORTER
		  "	ADR	r0, 100f;\n"		// Enter breakpoint handler
		  "	SWI	%[SWI_ReportText0];\n"
		  "\n"
#endif
		  "	ADR	r5, 98f;\n"
		  "	MOV	r6, #0;\n"		// Register loop counter
		  "0:\n"
		  "	MOV	r7, r4;\n"		// Buffer pointer
		  "	ADD	r9, r5, r6, LSL#3;\n"	// r9 = register name
		  "	MOV	r0, r9;\n"
		  "1:\n"
		  "	LDRB	r3, [r0], #1;\n"
		  "	CMP	r3, #0;\n"
		  "	STRNEB	r3, [r7], #1;\n"
		  "	BNE	1b;\n"
		  "\n"
		  "	LDR	r0, [r12, r6, LSL#2];\n"
		  "	MOV	r1, r7;\n"
		  "	MOV	r2, #20;\n"
		  "	SWI	%[SWI_OS_ConvertHex8];\n"
#if BREAKPT_HANDLER_TO_REPORTER
		  "	MOV	r0, r4;\n"
		  "	SWI	%[SWI_ReportText0];\n"
#endif
		  "	STMFD	sp!,{r12,r14};\n"
		  "	LDR	r2, [r12, r6, LSL#2];\n"
		  "	MOV	r1, r9;\n"
		  "	ADR	r0, 101f;\n"
		  "	BL	printf;\n"
		  "	LDMFD	sp!,{r12,r14};\n"
		  "	ADD	r6, r6, #1;\n"
		  "	CMP	r6, #16;\n"
		  "	BLT	0b;\n"
		  "\n"
#if BREAKPT_HANDLER_TO_REPORTER
		  "	ADR	r0, 99f;\n"
		  "	SWI	%[SWI_ReportText0];\n"
		  "\n"
#endif
		  "	BL	__pthread_enable_ints;\n"
		  "\n"
		  "	MRS	r0, CPSR;\n"
		  "	BIC	r0, r0, #0x1F;\n"
		  "	ORR	r0, r0, #0x13;\n"	// SVC32 Mode
		  "	MSR	CPSR_c, r0;\n"
		  "	MOV	r0, r0;\n"		// NOP for StrongARM MSR bug
		  "\n"
		  "	MOV	lr, r12;\n"
#if BREAKPT_HANDLER_RETURNS
		  // Alter stored PC to skip the breakpoint SWI and ID word when we return.
		  "	LDR	r0, [lr, #15 * 4];\n"
		  "	ADD	r0, r0, #8;\n"
#else
		  "	ADR	r0, 999f;\n"
#endif
		  "	STR	r0, [lr, #15 * 4];\n"
		  "	LDR	r0, [lr, #16 * 4];\n"
		  "	MRS	r1, cpsr;\n"
		  "	ORR	r1, r1, #0x90;\n"	// USR32 + Disable IRQ
		  "	MSR	cpsr_c, r1;\n"
		  "	MSR	spsr_cxsf, r0;\n"
		  "	LDMIA	lr!, {r0-r13}^;\n"
		  "	MOV	r0, r0;\n"
		  "	LDMIA	lr, {lr, pc}^;\n"
		  :
		  : [SWI_ReportText0] "i" (0x54C80),
		    [SWI_OS_ConvertHex8] "i" (0xD4));

    // Keep this section separate from above to prevent GCC complaining about missing
    // operand numbers after %.
    asm volatile ("101:\n"
		  "	.asciz	\"%s%X\n\" \n"
		  "100:\n"
		  "	.asciz	\"Enter breakpoint handler\"\n"
		  "99:\n"
		  "	.asciz	\"Exit breakpoint handler\"\n"
		  "98:\n"
		  "	.asciz	\"r0:    \"\n"
		  "	.asciz	\"r1:    \"\n"
		  "	.asciz	\"r2:    \"\n"
		  "	.asciz	\"r3:    \"\n"
		  "	.asciz	\"r4:    \"\n"
		  "	.asciz	\"r5:    \"\n"
		  "	.asciz	\"r6:    \"\n"
		  "	.asciz	\"r7:    \"\n"
		  "	.asciz	\"r8:    \"\n"
		  "	.asciz	\"r9:    \"\n"
		  "	.asciz	\"sl:    \"\n"
		  "	.asciz	\"fp:    \"\n"
		  "	.asciz	\"ip:    \"\n"
		  "	.asciz	\"sp:    \"\n"
		  "	.asciz	\"lr:    \"\n"
		  "	.asciz	\"pc:    \"\n"
		  "	.align\n"
		  "999:\n");

#if !BREAKPT_HANDLER_RETURNS
    restoreBreakpointHandler ();

//    asm volatile ("	SWI	0x11;\n");
    exit(0);
#endif
}

void
installBreakpointHandler (void)
{
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
  _kernel_oserror *err;
  err = _swix(OS_ChangeEnvironment, _INR(0,3)|_OUTR(1,3),
	      os_HANDLER_BREAK_PT,
	      breakpoint_handler,
	      &breakpoint_registers,
	      &breakpoint_registers,
	      &old_breakpoint_handler,
	      &old_breakpoint_r12,
	      &old_breakpoint_registers);
#else
  os_error *err;
  
  err = xos_change_environment (os_HANDLER_BREAK_PT,
				breakpoint_handler,
				(byte *)&breakpoint_registers,
				(byte *)&breakpoint_registers,
				&old_breakpoint_handler,
				&old_breakpoint_r12,
				&old_breakpoint_registers);
#endif
  if (err) {
    printf ("[%s:%d:%s] - RISC OS Error: %s\n",
	    __func__, __LINE__, __FILE__, err->errmess);
    report_text0 (err->errmess);
  }

  if (!err &&
      old_breakpoint_handler &&
      old_breakpoint_registers)
    atexit (restoreBreakpointHandler);
}

} // namespace riscos
