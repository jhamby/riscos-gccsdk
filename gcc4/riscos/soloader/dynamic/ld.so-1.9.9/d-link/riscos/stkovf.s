	.macro PRINT_TEXT string

	swi	0x1
	.ascii	"\string\0"
	.align

	.endm

	.macro PRINT_HEX int
	stmfd	sp!,{r0-r12,r14}
	mov	r12,\int
	b	9f
8:
	.word	0,0,0,0,0
9:
	mov	r0,r12
	adr	r1,8b
	mov	r2,#20
	swi	0xd4
	mov	r2,#0
	strb	r2,[r1,#0]
	swi	0x2
	swi	0x10a
	swi	0x10d

	ldmfd	sp!,{r0-r12,r14}
	.endm

	.global	_dl_stkovf_split_small
	.protected _dl_stkovf_split_small
_dl_stkovf_split_small:
	STR	r7, [sp, #-4]!

	LDR	r7, .L0+4
.LPIC0:
	ADD	r7, pc, r7			@ r7 = Library public GOT
	LDMIA	r7, {r7, r8}			@ r7 = Object index, r8 = GOT ptr array location
	LDR	r8, [r8, #0]			@ r8 = GOT ptr array
	LDR	r7, [r8, r7, LSL#4]		@ r7 = Library private GOT

	LDR	r8, .L0
	LDR	r8, [r7, r8]
	LDR	r7, [sp], #4
	LDR	r8, [r8, #0]
	TEQ	r8, #0
	MOVNE	pc, r8
	MOVEQ	pc, lr
.L0:
	.word	_dl_stkovf_split_small_function(GOT)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)
	.type	_dl_stkovf_split_small, %function
	.size	_dl_stkovf_split_small, . - _dl_stkovf_split_small

	.global	_dl_stkovf_split_big
	.protected _dl_stkovf_split_big
_dl_stkovf_split_big:
	STR	r7, [sp, #-4]!

	LDR	r7, .L1+4
.LPIC1:
	ADD	r7, pc, r7			@ r7 = Library public GOT
	LDMIA	r7, {r7, r8}			@ r7 = Object index, r8 = GOT ptr array location
	LDR	r8, [r8, #0]			@ r8 = GOT ptr array
	LDR	r7, [r8, r7, LSL#4]		@ r7 = Library private GOT

	LDR	r8, .L1
	LDR	r8, [r7, r8]
	LDR	r7, [sp], #4
	LDR	r8, [r8, #0]
	TEQ	r8, #0
	MOVNE	pc, r8
	MOVEQ	pc, lr
.L1:
	.word	_dl_stkovf_split_big_function(GOT)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+4)
	.type	_dl_stkovf_split_big, %function
	.size	_dl_stkovf_split_big, . - _dl_stkovf_split_big
