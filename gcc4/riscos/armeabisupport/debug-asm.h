@ Copyright (c) 2019 GCCSDK Developers
@ Written by Lee Noar

	.macro	REPORT_HEX int
	STMFD	sp!,{r0-r3}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x74c80
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3}
	.endm

	.macro	REPORT_DEC int
	STMFD	sp!,{r0-r3}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xdc
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x74c80
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3}
	.endm

	.macro REPORT_TEXT string
	SWI	0x74c81
	.asciz	"\string"
	.align
	.endm

	.macro REPORT_TEXT_R0
	SWI	0x74c80
	.endm

	.macro	PRINT_HEX int
	STMFD	sp!,{r0-r3}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3}
	.endm

	.macro	PRINT_DEC int
	STMFD	sp!,{r0-r3}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xdc
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3}
	.endm

	.macro PRINT_NL

	SWI	0x10D
	SWI	0x10A

	.endm

	.macro PRINT_TEXT string
	SWI	0x1
	.asciz	"\string"
	.align
	.endm
