@; Utility functions for ELF program loader
@; Mostly taken from GCC compiled unixlib code

	.text

@;* r0 = addr to fill from
@;* r1 = number of bytes
@;* r2 = byte to fill with
@;* r0, r1, r2 corrupted
	.global	fillmem
fillmem:
	TEQ	r1,#0
	MOVEQ	pc,lr
0:
	STRB	r2,[r0],#1
	SUBS	r1,r1,#1
	BNE	0b

	MOV	pc,lr

@;* entry:
@;*  r0 = str addr
@;* exit:
@;*  r0 = str len not inc terminator
	.global	strlen
strlen:
	MOV	r2,r0
	SUB	r0,r0,#1
0:
	LDRB	r3,[r0,#1]!
	CMP	r3,#0
	BNE	0b
	RSB	r0,r2,r0
	MOV	pc,lr

@;* entry:
@;*  r0 = dst addr
@;*  r1 = src addr
	.global	strcpy
strcpy:
	MOV	r2,r0
0:
	LDRB	r3,[r1],#1
	CMP	r3,#0
	STRB	r3,[r2],#1
	BNE	0b
	MOV	pc,lr

@;*	r0 = dst addr
@;*	r1 = src addr
@;*	r2 = number of bytes
	.global	strncpy
strncpy:
	MOV	r3,r0
	MOVS	r12,r2
	SUB	r2,r2,#1
	MOVEQ	pc,lr
0:
	LDRB	r12,[r1],#1
	STRB	r12,[r3],#1
	TEQ	r12,#0
	BNE	2f
	MOVS	r1,r2
	SUB	r2,r2,#1
	MOVEQ	pc,lr
	MOV	r12,#0
1:
	STRB	r12,[r3],#1
	MOVS	r1,r2
	SUB	r2,r2,#1
	BNE	1b
	MOV	pc,lr
2:
	MOVS	r12,r2
	SUB	r2,r2,#1
	BNE	0b
	MOV	pc,lr

@;*	r0 = s1
@;*	r1 = s2
@;*	r2,r12 corrupted
	.global	strcmp
strcmp:
	LDRB	r2,[r0,#0]
	B	0f
2:
	LDRB	r2,[r0,#1]!
	ADD	r1,r1,#1
0:
	CMP	r2,#0
	BEQ	1f
	LDRB	r12,[r1,#0]
	CMP	r2,r12
	BEQ	2b
1:
	LDRB	r12,[r0,#0]
	LDRB	r0,[r1,#0]
	RSB	r0,r0,r12
	MOV	pc,lr

	.global	strchr
strchr:
	LDRB	r3, [r0, #0]
	AND	r1, r1, #0xff
	CMP	r3, #0
	BEQ	0f

	CMP	r1, r3
	MOVEQ	pc, lr
1:
	LDRB	r3, [r0, #1]!
	CMP	r3, #0
	BEQ	0f

	CMP	r1, r3
	BNE	1b
	MOV	pc, lr
0:
	CMP	r1, r3
	MOVNE	r0, #0
	MOV	pc, lr

	.global	strrchr
strrchr:
	AND	r1, r1, #0xff
	MOV	r2, #0
0:
	LDRB	r3, [r0, #0]
	CMP	r3, r1
	MOVEQ	r2, r0
	CMP	r3, #0
	ADD	r0, r0, #1
	BNE	0b
	MOV	r0, r2
	MOV	pc, lr

	.global	strcat
strcat:
	MOV	r2, r0
0:
	LDRB	r3, [r2], #1
	CMP	r3, #0
	BNE	0b

	SUB	r2, r2, #1
1:
	LDRB	r3, [r1], #1
	CMP	r3, #0
	STRB	r3, [r2], #1
	BNE	1b
	MOV	pc, lr

@;* entry:
@;*  r0 = dst
@;*  r1 = src
@;*  r2 = number of bytes
@;* Assumes input & output are word aligned and that number of
@;* bytes is a multiple of 4.
	.global memcpy
memcpy:
	STMFD	sp!, {r4-r7}

	CMP	r2, #16
	BLT	1f
0:
	LDMIA	r1!, {r4-r7}
	STMIA	r0!, {r4-r7}
	SUBS	r2, r2, #16
	BGE	0b
1:
	TST	r2, #8
	LDMEQIA	r1!, {r4,r5}
	STMEQIA r0!, {r4,r5}
	SUBEQ	r2, r2, #8

	TST	r2, #4
	LDREQ	r4, [r1], #4
	STREQ	r4, [r0], #4

	LDMFD	sp!, {r4-r7}
	MOV	pc, lr

	.end
