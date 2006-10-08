@; Division routine
@; Taken from Acorn Assembler manual

reg_a .req r0
reg_b .req r1
reg_c .req r2
reg_d .req r3

@; entry:
@;  r0 = dividend
@;  r1 = divisor
@; exit:
@;  r0 = result
@;  r1 = remainder
	.global	div
div:
	MOVS	reg_d, reg_b
	MOVEQ	pc, lr
	CMP	reg_d, reg_a, LSR#1
1:
	MOVLS	reg_d, reg_d, LSL#1
	CMP	reg_d, reg_a, LSR#1
	BLS	1b
	MOV	reg_c, #0
2:
	CMP	reg_a, reg_d
	SUBCS	reg_a, reg_a, reg_d
	ADC	reg_c, reg_c, reg_c
	MOV	reg_d, reg_d, LSR#1
	CMP	reg_d, reg_b
	BHS	2b

	MOV	r1, r0
	MOV	r0, reg_c
	MOV	pc, lr
