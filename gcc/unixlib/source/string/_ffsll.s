;----------------------------------------------------------------------------
;
; $Source: $
; $Date: $
; $Revision: $
; $State: $
; $Author: $
;
;----------------------------------------------------------------------------

; Written by John Tytgat / BASS

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|ffsll|
	; int ffsll(long long int i)
	; Return the position of the first bit set in i, or 0 if none are
	; set.  The least-significant bit is position 1, the
	; most-significant 64.
	NAME	ffsll
|ffsll|
	MOVS	a3, a2
	MOVNE	a3, #32
	MOVEQS	a2, a1
	MOVEQ	pc, r14

	[ __UNIXLIB_CLZ_SUPPORTED > 0
	CLZ	a1, a2
	ADD	a3, a3, #32
	SUB	a1, a3, a1
	MOV	pc, r14
	|
	MOVS	ip, a2, LSR#16		;Highest 16 bits set ?
	SUBEQ	a3, a3, #16
	MOVEQ	a2, a2, LSL#16

	TST	a2, #&FF<<24		;Highest 8 bits set ?
	SUBEQ	a3, a3, #8
	MOVEQ	a2, a2, LSL#8

	TST	a2, #&F<<28		;Highest 4 bits set ?
	SUBEQ	a3, a3, #4
	MOVEQ	a2, a2, LSL#4

	TST	a2, #3<<30		;Highest 2 bits set ?
	SUBEQ	a3, a3, #2
	MOVEQS	a2, a2, LSL#2

	ADDPL	a1, a3, #32 - 1
	ADDMI	a1, a3, #32
	MOV	pc, r14
	]

	END
