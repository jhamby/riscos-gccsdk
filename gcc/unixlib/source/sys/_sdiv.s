;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

divisor RN 0
dividend RN 1

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_kernel_sdiv|
	EXPORT	|x$divide|
	EXPORT	|__rt_sdiv|
	NAME	__rt_sdiv
|_kernel_sdiv|
|x$divide|
|__rt_sdiv|
	; Just return for divide by zero
	MOVS	a4, divisor
	return	EQ, pc, lr
	AND	a3, dividend, #&80000000
	EOR	ip, a4, dividend
	ORR	a3, a3, ip, LSR #1
	RSBMI	a4, a4, #0
	MOVS	a2, dividend
	RSBMI	a2, a2, #0
	MOV	ip, a4

	CMP	ip, a2, LSR #16
	MOVLS	ip, ip, LSL #16
	CMP	ip, a2, LSR #8
	MOVLS	ip, ip, LSL #8
	CMP	ip, a2, LSR #4
	MOVLS	ip, ip, LSL #4
	CMP	ip, a2, LSR #2
	MOVLS	ip, ip, LSL #2
	CMP	ip, a2, LSR #1
	MOVLS	ip, ip, LSL #1

	MOV	a1, #0
divloop
	CMP	a2, ip
	SUBCS	a2, a2, ip
	ADC	a1, a1, a1
	MOV	ip, ip, LSR #1
	CMP	ip, a4
	BCS	divloop
	TST	a3,  #&40000000
	RSBNE	a1, a1, #0
	TST	a3,  #&80000000
	RSBNE	a2, a2, #0
	return	AL, pc, lr

	END
