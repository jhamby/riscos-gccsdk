@ ffsll
@ Written by John Tytgat
@ Copyright (c) 2004-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	ffsll
	@ int ffsll(long long int i)
	@ Return the position of the first bit set in i, or 0 if none are
	@ set.  The least-significant bit is position 1, the
	@ most-significant 64.
	NAME	ffsll
ffsll:
#if __ARM_ARCH__ >= 5
	RSBS	a3, a1, #0
	BEQ	ffsll_high
	AND	a1, a3, a1
	CLZNE	a1, a1
	RSBNE	a1, a1, #32
	MOV	pc, lr
ffsll_high:
	RSB	a3, a2, #0
	ANDS	a1, a3, a2
	CLZNE	a1, a1
	RSBNE	a1, a1, #64
	MOV	pc, lr
#else
	@ The following algorithm & code is from David Seal (cfr.
	@ comp.sys.arm discussion).
	RSBS	a3, a1, #0
	BEQ	ffsll_high
	ANDS	a3, a3, a1		@ a3 = X with 0 or 1 bits set

	ORRNE	a3, a3, a3, LSL #4	@ a3 = X * 0x11
	ORRNE	a3, a3, a3, LSL #6	@ a3 = X * 0x0451
	RSBNE	a3, a3, a3, LSL #16	@ a3 = X * 0x0450FBAF

	LDRNEB	a1, [pc, a3, LSR #26]	@ a1 = result taken from top 6 bits

	MOV	pc, lr
	.byte	 0,  1,  2, 13,  3,  7,  0, 14
	.byte	 4,  0,  8,  0,  0,  0,  0, 15
	.byte	11,  5,  0,  0,  9,  0,  0, 26
	.byte	 0,  0,  0,  0,  0, 22, 28, 16
	.byte	32, 12,  6,  0,  0,  0,  0,  0
	.byte	10,  0,  0, 25,  0,  0, 21, 27
	.byte	31,  0,  0,  0,  0, 24,  0, 20
	.byte	30,  0, 23, 19, 29, 18, 17,  0 
	.align
ffsll_high:
	RSB	a3, a2, #0
	ANDS	a3, a3, a2		@ a3 = X with 0 or 1 bits set

	ORRNE	a3, a3, a3, LSL #4	@ a3 = X * 0x11
	ORRNE	a3, a3, a3, LSL #6	@ a3 = X * 0x0451
	RSBNE	a3, a3, a3, LSL #16	@ a3 = X * 0x0450FBAF

	LDRNEB	a1, [pc, a3, LSR #26]	@ a1 = result taken from top 6 bits

	MOV	pc, lr
	.byte	32, 33, 34, 45, 35, 39, 32, 46
	.byte	36, 32, 40, 32, 32, 32, 32, 47
	.byte	43, 37, 32, 32, 41, 32, 32, 58
	.byte	32, 32, 32, 32, 32, 54, 60, 48
	.byte	64, 44, 38, 32, 32, 32, 32, 32
	.byte	42, 32, 32, 57, 32, 32, 53, 59
	.byte	63, 32, 32, 32, 32, 56, 32, 52
	.byte	62, 32, 55, 51, 61, 50, 49, 32 
#endif
	DECLARE_FUNCTION ffsll

	.end
