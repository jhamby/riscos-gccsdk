@ Fast memcpy and memmove
@ Copyright (c) 2005-2008 UnixLib Devlopers
@ Written by Adrian Lees

	.text

#include "internal/asm_dec.s"

	@ Always select ARM v6 architecture as this avoids the assembler error
	@ "selected processor does not support `PLD xxx`" when there is an
	@ explicit -march/cpu specified which predates armv6.  Here we're only
	@ using PLD which doesn't do anything on pre-armv6 architectures.
.arch	armv6

	.global	memcpy
	.global	memmove
	.global	bcopy

	NAME	bcopy
bcopy:
	MOV	a4,a1
	MOV	a1,a2
	MOV	a2,a4

@ Copy/move a block of memory
@
@ entry	a1 -> destination
@	a2 -> source
@	a3 = size in bytes
@ exit	a1 -> destination

memcpy:
memmove:
	STR	lr,[sp,#-4]!
	STR	v1,[sp,#-4]!
	STR	v2,[sp,#-4]!

	@are we copying/moving up/down in memory?

	CMP	a1,a2
	BLO	down

	ADD	a2,a2,a3
	ADD	a4,a1,a3

	@are the source and destination both word-aligned?

	ANDS	ip,a2,#3
	TSTEQ	a4,#3
	BNE	up_unaligned

up_aligned:
	@copying up in memory, we need to run backwards
	@  in case the two regions overlap (allowed by memmove)

	CMP	a3,#32
	BLO	ucp4
	CMP	a3,#128
	BLO	ucp16lp

ucp32lp:
	SUBS	a3,a3,#32
	PLD	[a2,#-36]
	LDRHS	v1,[a2,#-4]!
	LDRHS	v2,[a2,#-4]!
	LDRHS	ip,[a2,#-4]!
	LDRHS	lr,[a2,#-4]!
	STRHS	v1,[a4,#-4]!
	STRHS	v2,[a4,#-4]!
	STRHS	ip,[a4,#-4]!
	STRHS	lr,[a4,#-4]!
	LDRHS	v1,[a2,#-4]!
	LDRHS	v2,[a2,#-4]!
	LDRHS	ip,[a2,#-4]!
	LDRHS	lr,[a2,#-4]!
	STRHS	v1,[a4,#-4]!
	STRHS	v2,[a4,#-4]!
	STRHS	ip,[a4,#-4]!
	STRHS	lr,[a4,#-4]!
	BHI	ucp32lp
	ADDLO	a3,a3,#32

ucp16lp:
	SUBS	a3,a3,#16
	LDRHS	v1,[a2,#-4]!
	LDRHS	v2,[a2,#-4]!
	LDRHS	ip,[a2,#-4]!
	LDRHS	lr,[a2,#-4]!
	STRHS	v1,[a4,#-4]!
	STRHS	v2,[a4,#-4]!
	STRHS	ip,[a4,#-4]!
	STRHS	lr,[a4,#-4]!
	BHI	ucp16lp
	ADDLO	a3,a3,#16

ucp4:	SUBS	a3,a3,#4
	LDRHS	ip,[a2,#-4]!
ucp4lp:
	STRHS	ip,[a4,#-4]!
	SUBHSS	a3,a3,#4
	LDRHS	ip,[a2,#-4]!
	BHS	ucp4lp
	ADD	a3,a3,#4

ucp1:	SUBS	a3,a3,#1
	LDRHSB	ip,[a2,#-1]!
ucp1lp:
	STRHSB	ip,[a4,#-1]!
	SUBHSS	a3,a3,#1
	LDRHSB	ip,[a2,#-1]!
	BHS	ucp1lp

	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

up_unaligned:
	@source and/or destination unaligned
	@
	@copying up in memory, we need to run backwards
	@align source to next word boundary
	@
	@ip = nof bytes til aligned

	RSB	v1,ip,#3	@jump table index
	SUBS	a3,a3,ip	@check length not equalled/exceeded
	ADDLS	a3,a3,ip	@restore
	ADDHI	pc,pc,v1,LSL #3
	B	ucp1
	LDRB	v1,[a2,#-1]!
	STRB	v1,[a4,#-1]!
	LDRB	v1,[a2,#-1]!
	STRB	v1,[a4,#-1]!
	LDRB	v1,[a2,#-1]!
	STRB	v1,[a4,#-1]!

	ANDS	ip,a4,#3
	BEQ	up_aligned

	LDR	lr,[a4,-ip]!	@=> use post-dec when storing
	STR	v3,[sp,#-4]!	@free another register

	CMP	ip,#2
	BHI	ucp_sh3
	BEQ	ucp_sh2

ucp_sh1:
	SUBS	a3,a3,#32
	BIC	lr,lr,#0xFF
	BLO	ucp32sh1fin

ucp32sh1lp:
	PLD	[a2,#-36]
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #24
	MOV	v1,v1,LSL #8
	ORR	v1,v1,v2,LSR #24
	MOV	v2,v2,LSL #8
	ORR	v2,v2,v3,LSR #24
	MOV	v3,v3,LSL #8
	ORR	v3,v3,ip,LSR #24
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #8
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #24
	MOV	v1,v1,LSL #8
	ORR	v1,v1,v2,LSR #24
	MOV	v2,v2,LSL #8
	ORR	v2,v2,v3,LSR #24
	MOV	v3,v3,LSL #8
	ORR	v3,v3,ip,LSR #24
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #8
	SUBS	a3,a3,#32
	BHS	ucp32sh1lp
ucp32sh1fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2,#-4]!
	BLO	ucp4sh1fin
ucp4sh1lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSR #24
	STR	lr,[a4],#-4
	MOV	lr,v1,LSL #8
	LDRHS	v1,[a2,#-4]!
	BHS	ucp4sh1lp
ucp4sh1fin:
	ADD	a3,a3,#4

ucp1sh1:
	@we arrive here with 3 bytes still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRHSB	v1,[a2,#-1]!
	LDRLOB	v1,[a4]
	LDRHIB	v2,[a2,#-1]!
	CMP	a3,#2
	LDRHIB	v3,[a2,#-1]!
	ORR	lr,lr,v1
	STR	lr,[a4],#-4
	STRHSB	v2,[a4,#3]
	STRHIB	v3,[a4,#2]

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

ucp_sh2:
	SUBS	a3,a3,#32
	BIC	lr,lr,#0xFF00
	BIC	lr,lr,#0x00FF
	BLO	ucp32sh2fin
ucp32sh2lp:
	PLD	[a2,#-36]
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #16
	MOV	v1,v1,LSL #16
	ORR	v1,v1,v2,LSR #16
	MOV	v2,v2,LSL #16
	ORR	v2,v2,v3,LSR #16
	MOV	v3,v3,LSL #16
	ORR	v3,v3,ip,LSR #16
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #16
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #16
	MOV	v1,v1,LSL #16
	ORR	v1,v1,v2,LSR #16
	MOV	v2,v2,LSL #16
	ORR	v2,v2,v3,LSR #16
	MOV	v3,v3,LSL #16
	ORR	v3,v3,ip,LSR #16
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #16
	SUBS	a3,a3,#32
	BHS	ucp32sh2lp
ucp32sh2fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2,#-4]!
	BLO	ucp4sh2fin
ucp4sh2lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSR #16
	STR	lr,[a4],#-4
	MOV	lr,v1,LSL #16
	LDRHS	v1,[a2,#-4]!
	BHS	ucp4sh2lp
ucp4sh2fin:
	ADD	a3,a3,#4

ucp1sh2:
	@we arrive here with 2 bytes still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRHSB	v1,[a2,#-1]!
	LDRLOB	v1,[a4,#1]
	LDRHIB	v2,[a2,#-1]!
	LDRLSB	v2,[a4]
	CMP	a3,#2
	LDRHIB	v3,[a2,#-1]!
	ORR	lr,lr,v1,LSL #8
	ORR	lr,lr,v2
	STR	lr,[a4],#-4
	STRHIB	v3,[a4,#3]

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

ucp_sh3:
	SUBS	a3,a3,#32
	AND	lr,lr,#0xFF000000
	BLO	ucp32sh3fin
ucp32sh3lp:
	PLD	[a2,#-36]
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #8
	MOV	v1,v1,LSL #24
	ORR	v1,v1,v2,LSR #8
	MOV	v2,v2,LSL #24
	ORR	v2,v2,v3,LSR #8
	MOV	v3,v3,LSL #24
	ORR	v3,v3,ip,LSR #8
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #24
	LDR	v1,[a2,#-4]!
	LDR	v2,[a2,#-4]!
	LDR	v3,[a2,#-4]!
	LDR	ip,[a2,#-4]!
	ORR	lr,lr,v1,LSR #8
	MOV	v1,v1,LSL #24
	ORR	v1,v1,v2,LSR #8
	MOV	v2,v2,LSL #24
	ORR	v2,v2,v3,LSR #8
	MOV	v3,v3,LSL #24
	ORR	v3,v3,ip,LSR #8
	STR	lr,[a4],#-4
	STR	v1,[a4],#-4
	STR	v2,[a4],#-4
	STR	v3,[a4],#-4
	MOV	lr,ip,LSL #24
	SUBS	a3,a3,#32
	BHS	ucp32sh3lp
ucp32sh3fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2,#-4]!
	BLO	ucp4sh3fin
ucp4sh3lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSR #8
	STR	lr,[a4],#-4
	MOV	lr,v1,LSL #24
	LDRHS	v1,[a2,#-4]!
	BHS	ucp4sh3lp
ucp4sh3fin:
	ADD	a3,a3,#4

ucp1sh3:
	@we arrive here with 1 byte still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRHSB	v1,[a2,#-1]!
	LDRLOB	v1,[a4,#2]
	LDRHIB	v2,[a2,#-1]!
	LDRLSB	v2,[a4,#1]
	CMP	a3,#2
	LDRHIB	v3,[a2,#-1]!
	LDRLSB	v3,[a4]
	ORR	lr,lr,v1,LSL #16
	ORR	lr,lr,v2,LSL #8
	ORR	lr,lr,v3
	STR	lr,[a4]

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4


down:	@copying down in memory
	@ are the source and destination both word-aligned?

	MOV	a4,a1
	ANDS	ip,a2,#3
	TSTEQ	a1,#3
	BNE	down_unaligned

down_aligned:
	CMP	a3,#32
	BLO	dcp4
	CMP	a3,#128
	BLO	dcp16lp

dcp32lp:
	SUBS	a3,a3,#32
	PLD	[a2,#32]
	LDRHS	v1,[a2],#4
	LDRHS	v2,[a2],#4
	LDRHS	ip,[a2],#4
	LDRHS	lr,[a2],#4
	STRHS	v1,[a4],#4
	STRHS	v2,[a4],#4
	STRHS	ip,[a4],#4
	STRHS	lr,[a4],#4
	LDRHS	v1,[a2],#4
	LDRHS	v2,[a2],#4
	LDRHS	ip,[a2],#4
	LDRHS	lr,[a2],#4
	STRHS	v1,[a4],#4
	STRHS	v2,[a4],#4
	STRHS	ip,[a4],#4
	STRHS	lr,[a4],#4
	BHI	dcp32lp
	ADDLO	a3,a3,#32

dcp16lp:
	SUBS	a3,a3,#16
	LDRHS	v1,[a2],#4
	LDRHS	v2,[a2],#4
	LDRHS	ip,[a2],#4
	LDRHS	lr,[a2],#4
	STRHS	v1,[a4],#4
	STRHS	v2,[a4],#4
	STRHS	ip,[a4],#4
	STRHS	lr,[a4],#4
	BHI	dcp16lp
	ADDLO	a3,a3,#16

dcp4:
	SUBS	a3,a3,#4
	LDRHS	ip,[a2],#4
dcp4lp:
	STRHS	ip,[a4],#4
	SUBHSS	a3,a3,#4
	LDRHS	ip,[a2],#4
	BHS	dcp4lp
	ADD	a3,a3,#4

dcp1:
	SUBS	a3,a3,#1
	LDRHSB	ip,[a2],#1
dcp1lp:
	STRHSB	ip,[a4],#1
	SUBHSS	a3,a3,#1
	LDRHSB	ip,[a2],#1
	BHS	dcp1lp

	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

down_unaligned:
	@align source to next word boundary

	RSB	v1,ip,#4	@nof bytes til src aligned
	SUBS	a3,a3,v1	@check length not equalled/exceeded
	ADDLS	a3,a3,v1	@restore
	ADDHI	pc,pc,ip,LSL #3
	B	dcp1
	ADD	a3,a3,#4	@offset = 0, restore
	B	down_chkdest
	LDRB	v1,[a2],#1	@offset = 1
	STRB	v1,[a4],#1
	LDRB	v1,[a2],#1	@offset = 2
	STRB	v1,[a4],#1
	LDRB	v1,[a2],#1	@offset = 3
	STRB	v1,[a4],#1

down_chkdest:
	@is the destination also word-aligned now?

	ANDS	ip,a4,#3
	BEQ	down_aligned
	LDR	lr,[a4,-ip]!
	STR	v3,[sp,#-4]!	@free another register

	CMP	ip,#2
	BHI	dcp_sh3
	BEQ	dcp_sh2

dcp_sh1:
	SUBS	a3,a3,#32
	AND	lr,lr,#0xFF
	BLO	dcp32sh1fin
dcp32sh1lp:
	PLD	[a2,#32]
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #8
	MOV	v1,v1,LSR #24
	ORR	v1,v1,v2,LSL #8
	MOV	v2,v2,LSR #24
	ORR	v2,v2,v3,LSL #8
	MOV	v3,v3,LSR #24
	ORR	v3,v3,ip,LSL #8
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #24
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #8
	MOV	v1,v1,LSR #24
	ORR	v1,v1,v2,LSL #8
	MOV	v2,v2,LSR #24
	ORR	v2,v2,v3,LSL #8
	MOV	v3,v3,LSR #24
	ORR	v3,v3,ip,LSL #8
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #24
	SUBS	a3,a3,#32
	BHS	dcp32sh1lp
dcp32sh1fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2],#4
	BLO	dcp4sh1fin
dcp4sh1lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSL #8
	STR	lr,[a4],#4
	MOV	lr,v1,LSR #24
	LDRHS	v1,[a2],#4
	BHS	dcp4sh1lp
dcp4sh1fin:
	ADD	a3,a3,#4

dcp1sh1:
	@we arrive here with 1 byte still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRHSB	v1,[a2],#1
	LDRLOB	v1,[a4,#1]
	LDRHIB	v2,[a2],#1
	LDRLSB	v2,[a4,#2]
	CMP	a3,#2
	LDRHIB	v3,[a2],#1
	LDRLSB	v3,[a4,#3]
	ORR	lr,lr,v1,LSL #8
	ORR	lr,lr,v2,LSL #16
	ORR	lr,lr,v3,LSL #24
	STR	lr,[a4]

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

dcp_sh2:
	SUBS	a3,a3,#32
	BIC	lr,lr,#0xFF000000
	BIC	lr,lr,#0x00FF0000
	BLO	dcp32sh2fin
dcp32sh2lp:
	PLD	[a2,#32]
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #16
	MOV	v1,v1,LSR #16
	ORR	v1,v1,v2,LSL #16
	MOV	v2,v2,LSR #16
	ORR	v2,v2,v3,LSL #16
	MOV	v3,v3,LSR #16
	ORR	v3,v3,ip,LSL #16
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #16
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #16
	MOV	v1,v1,LSR #16
	ORR	v1,v1,v2,LSL #16
	MOV	v2,v2,LSR #16
	ORR	v2,v2,v3,LSL #16
	MOV	v3,v3,LSR #16
	ORR	v3,v3,ip,LSL #16
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #16
	SUBS	a3,a3,#32
	BHS	dcp32sh2lp
dcp32sh2fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2],#4
	BLO	dcp4sh2fin
dcp4sh2lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSL #16
	STR	lr,[a4],#4
	MOV	lr,v1,LSR #16
	LDRHS	v1,[a2],#4
	BHS	dcp4sh2lp
dcp4sh2fin:
	ADD	a3,a3,#4

dcp1sh2:
	@we arrive here with 2 bytes still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRLOB	v1,[a4,#2]
	LDRHSB	v1,[a2],#1
	LDRLSB	v2,[a4,#3]
	LDRHIB	v2,[a2],#1
	CMP	a3,#2
	LDRHIB	v3,[a2],#1
	ORR	lr,lr,v1,LSL #16
	ORR	lr,lr,v2,LSL #24
	STR	lr,[a4],#4
	STRHIB	v3,[a4],#1

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

dcp_sh3:
	SUBS	a3,a3,#32
	BIC	lr,lr,#0xFF000000
	BLO	dcp32sh3fin

dcp32sh3lp:
	PLD	[a2,#32]
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #24
	MOV	v1,v1,LSR #8
	ORR	v1,v1,v2,LSL #24
	MOV	v2,v2,LSR #8
	ORR	v2,v2,v3,LSL #24
	MOV	v3,v3,LSR #8
	ORR	v3,v3,ip,LSL #24
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #8
	LDR	v1,[a2],#4
	LDR	v2,[a2],#4
	LDR	v3,[a2],#4
	LDR	ip,[a2],#4
	ORR	lr,lr,v1,LSL #24
	MOV	v1,v1,LSR #8
	ORR	v1,v1,v2,LSL #24
	MOV	v2,v2,LSR #8
	ORR	v2,v2,v3,LSL #24
	MOV	v3,v3,LSR #8
	ORR	v3,v3,ip,LSL #24
	STR	lr,[a4],#4
	STR	v1,[a4],#4
	STR	v2,[a4],#4
	STR	v3,[a4],#4
	MOV	lr,ip,LSR #8
	SUBS	a3,a3,#32
	BHS	dcp32sh3lp
dcp32sh3fin:
	ADD	a3,a3,#32

	SUBS	a3,a3,#4
	LDRHS	v1,[a2],#4
	BLO	dcp4sh3fin
dcp4sh3lp:
	SUBS	a3,a3,#4
	ORR	lr,lr,v1,LSL #24
	STR	lr,[a4],#4
	MOV	lr,v1,LSR #8
	LDRHS	v1,[a2],#4
	BHS	dcp4sh3lp
dcp4sh3fin:
	ADD	a3,a3,#4

dcp1sh3:
	@we arrive here with 3 bytes still in lr
	@  and a further 0-3 bytes to copy

	CMP	a3,#1
	LDRHSB	v1,[a2],#1
	LDRLOB	v1,[a4,#3]	@read 4th from dest buffer
	LDRHIB	v2,[a2],#1
	CMP	a3,#2
	LDRHIB	v3,[a2],#1
	ORR	lr,lr,v1,LSL #24
	STR	lr,[a4],#4
	STRHSB	v2,[a4]
	STRHIB	v3,[a4,#1]

	LDR	v3,[sp],#4
	LDR	v2,[sp],#4
	LDR	v1,[sp],#4
	LDR	pc,[sp],#4

	DECLARE_FUNCTION bcopy
	DECLARE_FUNCTION memcpy
	DECLARE_FUNCTION memmove

	.weak	memcpy
	.weak	memmove
	.weak	bcopy

	.end
