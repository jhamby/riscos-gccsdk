@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2010-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	.global	__fpclassify
	NAME	__fpclassify
__fpclassify:
	BICS	a1, a1, #0x80000000
	TEQEQ	a2, #0
	MOVEQ	pc, lr			@ a1 = 0 && a2 = 0 -> zero value = FP_ZERO

	MOVS	a3, a1, LSR#20		@ a3 = exponent value (11 bits wide)
	MOVEQ	r0, #FP_SUBNORMAL
	MOVEQ	pc, lr

	ADDS	a1, a1, #1<<20		@ Exponent value being 0x7ff means non-normal.
	MOVPL	a1, #FP_NORMAL
	MOVPL	pc, lr

	ORRS	a1, a2, a1, LSL#1	@ Shift exponent away and check for zero.
	MOVEQ	a1, #FP_INFINITE
	MOVNE	a1, #FP_NAN
	MOV	pc, lr
	.type	__fpclassify, %function
	.size	__fpclassify, . - __fpclassify
#endif

	.end
