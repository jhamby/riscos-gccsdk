; ffs, ffsl
; Written by John Tytgat
; Copyright (c) 2004-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|ffs|
	EXPORT	|ffsl|
	; int ffs(int i)
	; Return the position of the first bit set in i, or 0 if none are
	; set.  The least-significant bit is position 1, the
	; most-significant 32.
	NAME	ffs
|ffs|
|ffsl|
	[ __UNIXLIB_CLZ_SUPPORTED > 0
	CLZ	a1, a1
	RSB	a1, a1, #32
	MOV	pc, r14
	|
	MOVS	a2, a1
	MOVEQ	pc, r14

	; Invert
	RSB	a1, a2, #0
	AND	a2, a2, a1
	
	MOVS	ip, a2, LSR#16		;Highest 16 bits set ?
	MOVEQ	a1, #16
	MOVEQ	a2, a2, LSL#16
	MOVNE	a1, #32

	TST	a2, #&FF<<24		;Highest 8 bits set ?
	SUBEQ	a1, a1, #8
	MOVEQ	a2, a2, LSL#8

	TST	a2, #&F<<28		;Highest 4 bits set ?
	SUBEQ	a1, a1, #4
	MOVEQ	a2, a2, LSL#4

	TST	a2, #3<<30		;Highest 2 bits set ?
	SUBEQ	a1, a1, #2
	MOVEQS	a2, a2, LSL#2

	SUBPL	a1, a1, #1
	MOV	pc, r14
	]

	END
