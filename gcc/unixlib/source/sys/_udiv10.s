;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_udiv10.s,v $
; $Date: 2000/07/15 14:52:35 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

dividend RN 1
divisor RN 0

	AREA	|C$code|, CODE, READONLY

	EXPORT	|_kernel_udiv10|
	EXPORT	|__rt_udiv10|

|_kernel_udiv10|
|__rt_udiv10|
	MOV	a2, a1
	MOV	a1, a1, LSR #1
	ADD	a1, a1, a1, LSR #1
	ADD	a1, a1, a1, LSR #4
	ADD	a1, a1, a1, LSR #8
	ADD	a1, a1, a1, LSR #16
	MOV	a1, a1, LSR #3
	ADD	a3, a1, a1, LSL #2
	SUB	a2, a2, a3, LSL #1
	CMP	a2, #10
	ADDGE	a1, a1, #1
	SUBGE	a2, a2, #10
	return	AL, pc, lr

	END
