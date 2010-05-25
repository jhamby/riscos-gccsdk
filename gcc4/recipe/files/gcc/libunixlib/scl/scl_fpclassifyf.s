@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	.global	__fpclassifyf
	NAME	__fpclassifyf
__fpclassifyf:
	BICS	a1, a1, #0x80000000
	MOVEQ	pc, lr			@ a1 = 0 -> zero value = FP_ZERO

	MOVS	a2, a1, LSR#23		@ a2 = exponent value (8 bits wide)
	MOVEQ	a1, #FP_SUBNORMAL
	MOVEQ	pc, lr

	TEQ	a2, #255		@ Exponent value being 255 means non-normal
	MOVNE	a1, #FP_NORMAL
	MOVNE	pc, lr

	MOVS	a2, a1, LSL#9		@ Shift exponent value away, check for zero
	MOVEQ	a1, #FP_INFINITE
	MOVNE	a1, #FP_NAN
	MOV	pc, lr
	.type	__fpclassifyf, %function
	.size	__fpclassifyf, . - __fpclassifyf

	.end
