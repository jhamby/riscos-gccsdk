;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_sdiv10.s,v $
; $Date: 2001/09/11 13:05:55 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_kernel_sdiv10|
	EXPORT	|__rt_sdiv10|

|_kernel_sdiv10|
|__rt_sdiv10|
	MOVS	a4, a1
	RSBMI	a1, a1, #0
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
	CMP	a4, #0
	RSBMI	a1, a1, #0
	RSBMI	a2, a2, #0
	return	AL, pc, lr

	END
