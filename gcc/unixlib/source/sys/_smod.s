;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_smod.s,v $
; $Date: 2005/01/03 22:55:13 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

divisor RN 0
dividend RN 1

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	EXPORT	|x$remainder|
	EXPORT	|_kernel_srem|
	EXPORT	|__rt_srem|
	NAME	__rt_srem
|_kernel_srem|
|x$remainder|
|__rt_srem|
	ANDS	ip, dividend, #&80000000
	RSBMI	dividend, dividend, #0
	MOVS	divisor, divisor
	; Raise an abort when there is a division by zero.
	BEQ	divbyzero
	RSBMI	divisor, divisor, #0
	MOV	a3, divisor

	CMP	a3, dividend, LSR #16
	MOVLS	a3, a3, LSL #16
	CMP	a3, dividend, LSR #8
	MOVLS	a3, a3, LSL #8
	CMP	a3, dividend, LSR #4
	MOVLS	a3, a3, LSL #4
	CMP	a3, dividend, LSR #2
	MOVLS	a3, a3, LSL #2
	CMP	a3, dividend, LSR #1
	MOVLS	a3, a3, LSL #1

divloop
	CMP	dividend, a3
	SUBCS	dividend, dividend, a3
	MOV	a3, a3, LSR #1
	CMP	a3, divisor
	BCS	divloop
	MOV	a1,  dividend
	TST	ip, #&80000000
	RSBNE	a1, a1, #0
	MOV	pc,  lr

divbyzero
	MOV	a1, #SIGFPE
	B	raise

	END
