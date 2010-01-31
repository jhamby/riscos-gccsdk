@ Fast memset
@ Copyright (c) 2005-2008 UnixLib Developers
@ Written by Adrian Lees

#include "internal/asm_dec.s"

	.text

        .align	5 @ For cache alignment of the code

	.global	memset
	.global	bzero

	NAME	bzero
bzero:
	MOV	a3,a2
	MOV	a2,#0

@ Fill a block of memory
@
@ entry	a1 -> destination buffer
@	a2 =  char used to fill buffer
@	a3 =  nof chars to be written
@ exit	a1 -> destination buffer

memset:
	AND	a2,a2,#0xFF
	MOV	a4,a1
	ORR	a2,a2,a2,LSL #8
	ANDS	ip,a1,#3
	ORR	a2,a2,a2,LSL #16
	BNE	mset_unaligned

mset_aligned:
	CMP	a3,#64
	BLO	mset16_lp

mset64_lp:
	SUBS	a3,a3,#64
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	BHI	mset64_lp
	ADDLO	a3,a3,#64

mset16_lp:
	SUBS	a3,a3,#16
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	STRHS	a2,[a4],#4
	BHI	mset16_lp
	ADDLO	a3,a3,#16

	@at most 15 bytes left

	SUBS	a3,a3,#4
	STRHS	a2,[a4],#4
	SUBHSS	a3,a3,#4
	STRHS	a2,[a4],#4
	SUBHSS	a3,a3,#4
	STRHS	a2,[a4],#4
	ADDLO	a3,a3,#4

	@at most 3 bytes left

	SUBS	a3,a3,#1
	STRHSB	a2,[a4],#1
	SUBHSS	a3,a3,#1
	STRHSB	a2,[a4],#1
	SUBHSS	a3,a3,#1
	STRHSB	a2,[a4],#1
	MOV	pc,lr

mset_unaligned:
	RSB	ip,ip,#4	@nof bytes til word aligned
	CMP	ip,a3
	MOVHI	ip,a3		@but don't overrun

	SUBS	ip,ip,#1
	SUBHS	a3,a3,#1
	STRHSB	a2,[a4],#1
	SUBHSS	ip,ip,#1
	SUBHS	a3,a3,#1
	STRHSB	a2,[a4],#1
	SUBHSS	ip,ip,#1
	SUBHS	a3,a3,#1
	STRHSB	a2,[a4],#1

	TEQ	a3,#0		@finished already?
	MOVEQ	pc,lr
	B	mset_aligned

	DECLARE_FUNCTION bzero
	DECLARE_FUNCTION memset

	.weak	memset
	.weak	bzero

	.end
