;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_udiv10.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_kernel_udiv10|
	EXPORT	|__rt_udiv10|
	NAME	__rt_udiv10
|_kernel_udiv10|
|__rt_udiv10|
	SUB	a2, a1, #10		; Keep a1 - 10 for later
	SUB	a1, a1, a1, LSR #2
	ADD	a1, a1, a1, LSR #4
	ADD	a1, a1, a1, LSR #8
	ADD	a1, a1, a1, LSR #16
	MOV	a1, a1, LSR #3		; a1 = a1 (on entry) / 10
	ADD	a3, a1, a1, LSL #2
	SUBS	a2, a2, a3, LSL #1	; Calculate a1/10 - (a1/10)*10
	ADDPL	a1, a1, #1		; Fix quotient
	ADDMI	a2, a2, #10		; Fix remainder
	return	AL, pc, lr

	END
