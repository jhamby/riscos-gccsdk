@ Fast strcpy
@ Written by Adrian Lees
@ Copyright (c) 2005-2008 UnixLib Developers

@ ----- Remember that string-handling routines can only read one
@       word ahead without risking a data abort :-(

@ Copy a NUL-terminated string
@
@ entry	a1 -> destination buffer
@	a2 -> string
@ exit	a1 -> destination buffer

#include "internal/asm_dec.s"

	.text

	.global	strcpy
	NAME	strcpy

        .align	5 @ For cache alignment of the code
strcpy:	@strongly biased in favour of word-aligned source and destination
	STR	a1,[sp,#-4]!
	MOV	a4,#1
	ORR	ip,a1,a2
	ORR	a4,a4,a4,LSL #8
	TST	ip,#3
	LDREQ	ip,[a2],#4
	ORR	a4,a4,a4,LSL #16
	BNE	strcpy_misaligned

strcpy_lp:
	SUBS	a3,ip,a4	@3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2],#4	@safe to read next word now
	STREQ	ip,[a1],#4
	BNE	strcpy_fin

	SUBS	ip,a3,a4	@3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2],#4	@safe to read next word now
	STREQ	a3,[a1],#4
	BNE	strcpy_a3

	SUBS	a3,ip,a4	@3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2],#4	@safe to read next word now
	STREQ	ip,[a1],#4
	BNE	strcpy_fin

	SUBS	ip,a3,a4	@3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2],#4	@safe to read next word now
	STREQ	a3,[a1],#4
	BEQ	strcpy_lp

strcpy_a3:
	MOV	ip,a3

	@we still need to store the 1 to 4 bytes used in the last word

strcpy_fin:
	STRB	ip,[a1],#1	@store the first byte of the last word
				@ (this may be the terminator)
	TST	ip,#0xFF
	MOVNE	ip,ip,LSR #8
	STRNEB	ip,[a1],#1
	TSTNE	ip,#0xFF
	MOVNE	ip,ip,LSR #8
	STRNEB	ip,[a1],#1
	TSTNE	ip,#0xFF
	MOVNE	ip,ip,LSR #8
	STRNEB	ip,[a1],#1

	@finished (we copied the terminator byte before checking its value)

	LDR	a1,[sp],#4
	MOV	pc,lr

strcpy_misaligned:
	TST	a2,#3
	LDRNEB	ip,[a2],#1
	BEQ	strcpy_chkdest

	@copy individual bytes until source ptr is word-aligned or finished
	@ (between 1 and 3 iterations performed)

strcpy_align_lp:
	TST	a2,#3		@is source word-aligned yet
	TEQNE	ip,#0		@  or is this the end?
	STRB	ip,[a1],#1
	LDRNEB	ip,[a2],#1
	BNE	strcpy_align_lp

	TEQ	ip,#0		@end of string reached?
	LDREQ	a1,[sp],#4
	MOVEQ	pc,lr

strcpy_chkdest:
	@is destination also word-aligned now?
	ANDS	a3,a1,#3
	LDREQ	ip,[a2],#4
	BEQ	strcpy_lp
	CMP	a3,#2
	BHI	strcpy3
	BEQ	strcpy2

strcpy1:
	LDR	a3,[a2],#4
	LDRB	ip,[a1,#-1]!
	STR	lr,[sp,#-4]!	@need another register

strcpy1_lp:
	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy1_fin
	ORR	lr,ip,a3,LSL #8
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #24

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy1_ip
	ORR	lr,a3,ip,LSL #8
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #24

	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy1_fin
	ORR	lr,ip,a3,LSL #8
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #24

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy1_ip
	ORR	lr,a3,ip,LSL #8
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #24
	B	strcpy1_lp

strcpy1_ip:
	MOV	lr,a3
	MOV	a3,ip
	MOV	ip,lr

strcpy1_fin:
	@a3 contains the last loaded word which contains a 0 byte
	@ip contains (in its LSB) the next byte to be written to dest

	STRB	ip,[a1],#1
	STRB	a3,[a1],#1
	TST	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1

	@finished

	LDR	a1,[sp,#4]
	LDR	pc,[sp],#8


strcpy2:
	LDR	ip,[a1,#-2]!
	LDR	a3,[a2],#4
	STR	lr,[sp,#-4]!	@need another register
	BIC	ip,ip,#0xFF000000
	BIC	ip,ip,#0x00FF0000

strcpy2_lp:
	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy2_fin
	ORR	lr,ip,a3,LSL #16
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #16

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy2_ip
	ORR	lr,a3,ip,LSL #16
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #16

	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy2_fin
	ORR	lr,ip,a3,LSL #16
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #16

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy2_ip
	ORR	lr,a3,ip,LSL #16
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #16
	B	strcpy2_lp

strcpy2_ip:
	MOV	lr,a3
	MOV	a3,ip
	MOV	ip,lr

strcpy2_fin:	@a3 contains the last loaded word which contains a 0 byte
		@ip contains (in its 2 LSBs) the 2 next bytes to be written to dest

	MOV	lr,ip,LSR #8
	STRB	ip,[a1],#1
	STRB	lr,[a1],#1
	STRB	a3,[a1],#1
	TST	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1

	@finished

	LDR	a1,[sp,#4]
	LDR	pc,[sp],#8


strcpy3:
	LDR	ip,[a1,#-3]!
	LDR	a3,[a2],#4
	STR	lr,[sp,#-4]!	@need another register
	BIC	ip,ip,#0xFF000000

strcpy3_lp:
	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy3_fin
	ORR	lr,ip,a3,LSL #24
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #8

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy3_ip
	ORR	lr,a3,ip,LSL #24
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #8

	SUBS	lr,a3,a4
	EORCS	lr,lr,a3
	BICCSS	lr,a4,lr
	BNE	strcpy3_fin
	ORR	lr,ip,a3,LSL #24
	LDR	ip,[a2],#4
	STR	lr,[a1],#4
	MOV	a3,a3,LSR #8

	SUBS	lr,ip,a4
	EORCS	lr,lr,ip
	BICCSS	lr,a4,lr
	BNE	strcpy3_ip
	ORR	lr,a3,ip,LSL #24
	LDR	a3,[a2],#4
	STR	lr,[a1],#4
	MOV	ip,ip,LSR #8
	B	strcpy3_lp

strcpy3_ip:
	MOV	lr,a3
	MOV	a3,ip
	MOV	ip,lr

strcpy3_fin:
	@a3 contains the last loaded word which contains a 0 byte
	@ip contains (in its 3 LSBs) the 3 next bytes to be written to dest

	ORR	lr,ip,a3,LSL #24
	STR	lr,[a1],#4
	TST	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1
	TSTNE	a3,#0xFF
	MOVNE	a3,a3,LSR #8
	STRNEB	a3,[a1],#1

	@finished

	LDR	a1,[sp,#4]
	LDR	pc,[sp],#8

	DECLARE_FUNCTION strcpy

	.end
