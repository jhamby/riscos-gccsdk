;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/gcccompat/_divdi3.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

; This file is taken from the ARM port of Libgcc for GCC. This is
; a placeholder until a suitable (and fast) 64-bit divide by 10
; function can be written for `printf'.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	; (a1, a2) / (a3, a4)
	; result in a1 and a2
   	; remainder in a3 and a4
	EXPORT |__divdi3|
	NAME	__divdi3
|__divdi3|
	; fast exits
	; check the numerator for zero
	TEQ	a1, #0	; low
	TEQEQ	a2, #0	; high
	MOVEQ	pc, lr

	; test for a divide by zero
	TEQ	a3, #0	; low
	TEQEQ	a4, #0	; high
	BEQ	divbyzero

	; No stack checking since we are a leaf function.
	; We don't need to store ip - procedure calling standard
	; lets it be corrupted.
	STMFD	sp!, {v2, v3, v4, v6, lr}
	; v6 = sign bit
	ANDS	v6, a4, #&80000000
	BPL	|__divdi3.L00|
	RSBS	a3, a3, #0 ; low
	RSC	a4, a4, #0 ; high
|__divdi3.L00|
	EORS	v6, v6, a2, ASR#32
	BCC	|__divdi3.L01|
	RSBS	a1, a1, #0 ; low
	RSC	a2, a2, #0 ; high
|__divdi3.L01|
	; initialise the quotient to zero
	MOV	v2, #0 ; low
	MOV	v3, #0 ; high
	; remainder = numerator
	MOV	v4, a1 ; low
	MOV	ip, a2 ; high

	; lr is a count of the number of shifts needed
        MOVS    lr, #0
	; shift the denominator left until the next doubling
	;  generates a number larger than the numerator.
	;  Count the number of shifts needed
|__divdi3.bit_count|
	; double the denominator, we've finished when the top bit
	; is set.
	ADDS	a3, a3, a3 ; low
	ADCS	a4, a4, a4 ; high
	; too big so we've finished
	BCS	|__divdi3.count_done|
	CMP	a4, ip ; high
	CMPEQ	a3, v4 ; low
	; if denominator is lower or the same as the numerator
	; signify an extra shift and carry on.
	ADDLS	lr, lr, #1
	BLS	|__divdi3.bit_count|

	; clear the carry flag
	ADDS	lr, lr, #0

|__divdi3.count_done|
	; we've overflowed. shift the denominator back down
	; to bring in the overflowed bit
	MOVS	a4, a4, RRX ; high
	MOV	a3, a3, RRX ; low

|__divdi3.division|
	; numerator = remainder - denominator
	SUBS	a1, v4, a3 ; low
	SBCS	a2, ip, a4 ; high
	; if (remainder >= denominator) remainder = numerator
	MOVCS	v4, a1 ; low
	MOVCS	ip, a2 ; high
	; place next bit into the result
	ADCS	v2, v2, v2 ; low
	ADC	v3, v3, v3 ; high
	; shift the denominator right 1 bit
	MOVS	a4, a4, LSR #1 ; high
	MOV	a3, a3, RRX ; low
	SUBS	lr, lr, #1
	BGE	|__divdi3.division|

	; result
	MOVS	v6, v6, LSL#2
	BCS	|__divdi3.L02|

	MOV	a1, v2
	MOV	a2, v3
	MOV	a3, v4
	MOV	a4, ip
	LDMFD	sp!, {v2, v3, v4, v6, pc}

|__divdi3.L02|
	RSBS	a1, v2, #0 ; low
	RSC	a2, v3, #0 ; high
	RSBS	a3, v4, #0 ; low
	RSC	a4, ip, #0 ; high
	LDMFD	sp!, {v2, v3, v4, v6, pc}

divbyzero
	MOV	a1, #SIGFPE
	B	raise

	END
