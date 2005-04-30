; This file is taken from the ARM port of Libgcc for GCC. This is
; a placeholder until a suitable (and fast) 64-bit divide by 10
; function can be written for `printf'.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	; (a1, a2) / (a3, a4)
   	; result in a1 and a2
   	; remainder in a3 and a4
	EXPORT |__udivdi3|
	NAME	__udivdi3
|__udivdi3|
	TEQ	a3, #0
	TEQEQ	a4, #0
	BEQ	|divbyzero|

	TEQ	a1, #0
	TEQEQ	a2, #0
	BNE	|start|
	MOV	a3, #0
	MOV	a4, #0
	MOV	pc, lr

|start|
	STMFD	sp!, {v2, v3, v4, lr}
	MOV	v2, #0
	MOV	v3, #0
	MOV	v4, a1
	MOV	ip, a2
        MOVS    lr, #0
|bit_count|
	ADDS	a3, a3, a3
	ADCS	a4, a4, a4
	BCS	|count_done|
	CMP	a4, ip
	CMPEQ	a3, v4
	ADDLS	lr, lr, #1
	BLS	|bit_count|
	ADDS	lr, lr, #0
|count_done|
	MOVS	a4, a4, RRX
	MOV	a3, a3, RRX
|division|
	SUBS	a1, v4, a3
	SBCS	a2, ip, a4
	MOVCS	v4, a1
	MOVCS	ip, a2
	ADCS	v2, v2, v2
	ADC	v3, v3, v3
	MOVS	a4, a4, LSR #1
	MOV	a3, a3, RRX
	SUBS	lr, lr, #1
	BGE	|division|
	MOV	a1, v2
	MOV	a2, v3
	MOV	a3, v4
	MOV	a4, ip
	LDMFD	sp!, {v2, v3, v4, pc}

|divbyzero|
	MOV	a1, #SIGFPE
	B	raise

	END
