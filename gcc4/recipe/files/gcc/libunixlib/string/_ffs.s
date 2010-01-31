@ ffs, ffsl
@ Written by John Tytgat
@ Copyright (c) 2004-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	ffs
	.global	ffsl
	@ int ffs(int i)
	@ Return the position of the first bit set in i, or 0 if none are
	@ set.  The least-significant bit is position 1, the
	@ most-significant 32.
	NAME	ffs
ffs:
ffsl:
#if __ARM_ARCH__ >= 5
	RSB	a2, a1, #0
	ANDS	a1, a2, a1
	CLZNE	a1, a1
	RSBNE	a1, a1, #32
	MOV	pc, lr
#else
	@ The following algorithm & code is from David Seal (cfr.
	@ comp.sys.arm discussion).
	RSB	a2, a1, #0
	ANDS	a2, a2, a1		@ a2 = X with 0 or 1 bits set

	ORRNE	a2, a2, a2, LSL #4	@ a2 = X * 0x11
	ORRNE	a2, a2, a2, LSL #6	@ a2 = X * 0x0451
	RSBNE	a2, a2, a2, LSL #16	@ a2 = X * 0x0450FBAF

	LDRNEB	a1, [pc, a2, LSR #26]	@ a1 = result taken from top 6 bits

	MOV	pc, lr
	.byte	 0,  1,  2, 13,  3,  7,  0, 14
	.byte	 4,  0,  8,  0,  0,  0,  0, 15
	.byte	11,  5,  0,  0,  9,  0,  0, 26
	.byte	 0,  0,  0,  0,  0, 22, 28, 16
	.byte	32, 12,  6,  0,  0,  0,  0,  0
	.byte	10,  0,  0, 25,  0,  0, 21, 27
	.byte	31,  0,  0,  0,  0, 24,  0, 20
	.byte	30,  0, 23, 19, 29, 18, 17,  0 
#endif
	DECLARE_FUNCTION ffs

	.end
