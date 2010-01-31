@ Fast strlen
@ Written by Adrian Lees
@ Copyright (c) 2005-2009 UnixLib Developers

@ Determine the length of a NUL-terminated string
@
@ entry	a1 -> NUL-terminated string
@ exit	a1 = length excluding terminator

#include "internal/asm_dec.s"

	.text

	.global	strlen
	NAME	strlen

        .align	5 @ For cache alignment of the code
strlen:
	ANDS	a3,a1,#3
	MOV	a4,#1
	LDR	ip,[a1,-a3]	@read 1st word, guaranteed to be aligned
	ORR	a4,a4,a4,LSL #8
	MOV	a2,a1
	ORR	a4,a4,a4,LSL #16
	BNE	strlen_align

strlen_lp:
	SUBS	a3,ip,a4	@3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2,#4]!	@safe to read next word now
	BNE	strlen_end

	SUBS	ip,a3,a4	@3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2,#4]!	@safe to read next word now
	BNE	strlen_end_a3

	SUBS	a3,ip,a4	@3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2,#4]!	@safe to read next word now
	BNE	strlen_end

	SUBS	ip,a3,a4	@3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2,#4]!	@safe to read next word now
	BEQ	strlen_lp

strlen_end_a3:
	MOV	ip,a3

strlen_end:
	SUB	a1,a2,a1	@excludes last word read
	TST	ip,#0xFF
	ADDNE	a1,a1,#1
	TSTNE	ip,#0xFF00
	ADDNE	a1,a1,#1
	TSTNE	ip,#0xFF0000
	ADDNE	a1,a1,#1
	MOV	pc,lr

strlen_align:
	CMP	a3,#2
	BHI	strlen3
	BEQ	strlen2
	TST	ip,#0xFF00
	MOVEQ	a1,#0
	MOVEQ	pc,lr
	ADD	a2,a2,#1
strlen2:
	TST	ip,#0xFF0000
	ADDNE	a2,a2,#1
strlen3:
	TSTNE	ip,#0xFF000000
	LDRNE	ip,[a2,#1]!
	SUBEQ	a1,a2,a1
	MOVEQ	pc,lr
	B	strlen_lp

	DECLARE_FUNCTION strlen

	.end
