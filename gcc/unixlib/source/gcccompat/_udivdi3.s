;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/gcccompat/_udivdi3.s,v $
; $Date: 2001/01/29 15:10:19 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; This file is taken from the ARM port of Libgcc for GCC. This is
; a placeholder until a suitable (and fast) 64-bit divide by 10
; function can be written for `printf'.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; (a1, a2) / (a3, a4)
   	; result in v2 and v3
   	; remainder in v4 and ip
	EXPORT |__udivdi3|
	NAME	__udivdi3
|__udivdi3|
	CMP	a1, #0
	CMPEQ	a2, #0
	return	EQ, pc, lr
	CMP	a3, #0
	CMPEQ	a4, #0
	return	EQ, pc, lr
	STMFD	sp!, {v2, v3, v4, lr}
	MOV	v2, #0
	MOV	v3, #0
	MOV	v4, a1
	MOV	ip, a2
        MOVS    lr, #0
|__udivdi3.bit_count|
	ADDS	a3, a3, a3
	ADCS	a4, a4, a4
	BCS	|__udivdi3.count_done|
	CMP	a4, ip
	CMPEQ	a3, v4
	ADDLS	lr, lr, #1
	BLS	|__udivdi3.bit_count|
	ADDS	lr, lr, #0
|__udivdi3.count_done|
	MOVS	a4, a4, RRX
	MOV	a3, a3, RRX
|__udivdi3.division|
	SUBS	a1, v4, a3
	SBCS	a2, ip, a4
	MOVCS	v4, a1
	MOVCS	ip, a2
	ADCS	v2, v2, v2
	ADC	v3, v3, v3
	MOVS	a4, a4, LSR #1
	MOV	a3, a3, RRX
	SUBS	lr, lr, #1
	BGE	|__udivdi3.division|
	MOV	a1, v2
	MOV	a2, v3
	stackreturn	AL, "v2, v3, v4, pc"

	END
