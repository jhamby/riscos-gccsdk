@;* macros.h
@;* ELF Loader
@;* by Lee Noar
@;* 28th June, 2004

	.macro PRINT_TEXT string

	STR	lr,[sp,#-4]! @; stack lr in case we're in SVC

	SWI	XOS_WriteS
	.asciz	"\string"
	.align

	LDR	lr,[sp],#4

	.endm

	.macro	PRINT_HEX int
	STMFD	sp!,{r0-r12,r14}
	MOV	r12,\int
	B	89f
88:
	.word	0,0,0,0,0
89:
	MOV	r0,r12
	ADR	r1,88b
	MOV	r2,#20
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2

	LDMFD	sp!,{r0-r12,r14}
	.endm

	.macro	PRINT_HEX_NL int
	STMFD	sp!,{r0-r12,r14}
	MOV	r12,\int
	B	89f
88:
	.word	0,0,0,0,0
89:
	MOV	r0,r12
	ADR	r1,88b
	MOV	r2,#20
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2
	SWI	0x10a
	SWI	0x10d

	LDMFD	sp!,{r0-r12,r14}
	.endm

	.macro	PRINT_R0_WITH_LABEL	label
	PRINT_TEXT	"\label"
	BL	print_hex
	SWI	0x10a
	SWI	0x10d

	.endm
