; ADR / ADRL tests:

		GBLL	CanUseMOVW
CanUseMOVW	SETL	{TARGET_ARCH_6T2} :LOR: {TARGET_ARCH_7} 

	[ {TRUE}
	AREA	Code4, CODE, READONLY
	; Excercise the address range of ADR/ADRL.

	[ :LNOT: REFERENCE
CurPC	*	{PC} + 8
	ADR	r1, CurPC + &FFFF0000

	ADRL	r1, {PC} + 8 + &400002e4

	ADRL	r1, lbl1 + &400002e4
lbl1

lbl2	*	lbl3
	ADRL	r1, lbl2 + &400002e4
lbl3
	|
	SUB	r1, pc, #0x10000

	ADD	r1, pc, #0x2e4		; The immediate value can be split
	ADD	r1, r1, #0x40000000	; differently.

	ADD	r1, pc, #0x2e4		; The immediate value can be split
	ADD	r1, r1, #0x40000000	; differently.

	ADD	r1, pc, #0x2e4		; The immediate value can be split
	ADD	r1, r1, #0x40000000	; differently.
	]
	]

	[ {TRUE}
	AREA	Code5, CODE, READONLY

	[ :LNOT: REFERENCE
	GBLA	SymAbs
SymAbs	SETA	32

	ADR	r0, SymAbs	; MOV r0, #32, note this is a bit odd.
	|
	MOV	r0, #32
	]
	]

	[ {TRUE}
	; Absolute value usage in ADR/ADRL.
	; Results in MOV/MVN.
	AREA	Code8, CODE, READONLY

	[ :LNOT: REFERENCE
	ADR	r2, 0		; MOV r2, #0
	ADR	r2, 42		; MOV r2, #42
	ADRL	r2, &123

	ADR	r3, -42		; MVN r3, #41
	ADRL	r3, -&123
	|
	MOV	r2, #0
	MOV	r2, #0x2a
	[ CanUseMOVW
	MOVW	r2, #&123	; Generates "Using ADR instead of ADRL" warning
	|
	MOV	r2, #0x23
	ADD	r2, r2, #0x100
	]

	MVN	r3, #0x29
	MVN	r3, #0x22
	SUB	r3, r3, #0x100
	]
	]

	; Wacko case also resulting in absolute value usage in ADR/ADRL.
	; Results in MOV/MVN.
	; ObjAsm extension
	[ {FALSE}	; FIXME: Is disabled as it is too wacko.
	[ EXTENSION
	AREA	Code9, CODE, READONLY
	[ :LNOT: REFERENCE
	ADR	r2, 42 - 8 - {PC}
	ADR	r2, 42 - {PC} - 8
	ADR	r2, - {PC} + 42 - 8
	ADRL	r2, 42 - 8 - {PC}	; Becomes ADR. FIXME: no longer :( - twopass regression
	ADRL	r2, 42 - {PC} - 8	; Becomes ADR. FIXME: no longer :( - twopass regression
	ADRL	r2, - {PC} + 42 - 8	; Becomes ADR. FIXME: no longer :( - twopass regression
	|
	ADD	r2, pc, #&1a
	ADD	r2, pc, #&1a
	ADD	r2, pc, #&1a
	ADD	r2, pc, #&1a
	ADD	r2, r2, #0		; FIXME: should go - twopass regression
	ADD	r2, pc, #&1a
	ADD	r2, r2, #0		; FIXME: should go - twopass regression
	ADD	r2, pc, #&1a
	ADD	r2, r2, #0		; FIXME: should go - twopass regression
	]
	]
	]

	; Storage map + base register usage in ADR/ADRL.
	[ {TRUE}
	[ :LNOT: REFERENCE
	; First defined, then used.
	^	16, r3
tstlbl1	#	4
tstlbl2	#	8
tstlbl3	#	32

	AREA	Code10, CODE, READONLY
	ADR	r2, tstlbl2
	ADRL	r4, tstlbl2	; Generates warning, ADR is used.
	ADR	r5, tstlbl5
	ADRL	r6, tstlbl5	; Generates warning, ADRL is still used

	; First used, then defined.
	^	16, r7
tstlbl4	#	4
tstlbl5	#	8
tstlbl6	#	32
	|
	AREA	Code10, CODE, READONLY
	ADD r2, r3, #20	;ADR
	ADD r4, r3, #20	;ADR
	ADD r5, r7, #20	;ADR
	ADD r6, r7, #20	;ADRL
	NOP
	]
	]

	; Absolute area.
	[ {TRUE}
	[ :LNOT: REFERENCE
	; Explicitly setting 'ORG':
	AREA	Code11, CODE
	ORG	&200

	%	16
01
	DCD	1
	ADR	r1, %b01		; SUB r1, pc, #...
	ADRL	r2, %f02		; ADD r2, pc, #... + NOP, generates "ADR instead of ADRL can be used" alike warning"
02
	DCD	2

03
	DCD	3
	%	&1100 - {PC}
	ADR	r3, %b03		; MOV r3, #&200 + 16 + 4*4 + 4
	ADR	r4, %f04		; MOV r4, #&2200
	%	&1100 - 8
04
	DCD	4

	; Implicit 'ORG 0' by setting the ABS AREA attribute:
	AREA	Code12, CODE, ABS

	%	16
01
	DCD	1
	ADR	r1, %b01		; SUB r1, pc, #...
	ADR	r2, %f02		; ADD r2, pc, #...
02
	DCD	2

03
	DCD	3
	%	&1100 - {PC}
	ADR	r3, %b03		; MOV r3, #16 + 4*4
	ADR	r4, %f04		; MOV r4, #&2200
	%	&1100 - 8
04
	DCD	4

	|

	AREA	Code11, CODE
	ORG	&200
	%	16
	DCD	1
	ADD r1, pc, #-8 - 4		; MOV r1, #&210 would also be possible but less preferred.
	ADD r2, pc, #-8 + 8		; MOV r2, #&220 would also be possible but less preferred.
	NOP
	DCD	2

	DCD	3
	%	&1100 - {PC}
	MOV	r3, #&200 + 16 + 4*4 + 4
	MOV	r4, #&2200
	%	&1100 - 8
	DCD	4

	AREA	Code12, CODE, ABS
	%	16
	DCD	1
	ADD r1, pc, #-8 - 4		; MOV r1, #&10 would also be possible but less preferred.
	ADD r2, pc, #-8 + 4		; MOV r2, #&1C would also be possible but less preferred.
	DCD	2

	DCD	3
	%	&1100 - {PC}
	MOV	r3, #16 + 4*4
	MOV	r4, #&2200
	%	&1100 - 8
	DCD	4
	]

	AREA	Code13, CODE
	ORG	&100000
	DCD	1,2,3,4
	[ :LNOT: REFERENCE
Code13lbl
	ADR	r2, &120		; Can only be done with MOV.
	ADR	r3, Code13lbl		; SUB r3, pc, #&c
	ADR	r4, &100000		; SUB r4, pc, #&20
	[ CanUseMOVW
	ADR	r5, &1234		; Can only be done with MOVW.
	ADRL	r6, &12345678		; Can only be done with MOV32.
	]
	|
	MOV	r2, #&120
	SUB	r3, pc, #&c
	SUB	r4, pc, #&20
	[ CanUseMOVW
	MOVW	r5, #&1234
	MOVW	r6, #&5678
	MOVT	r6, #&1234
	]
	]

	] ; Absolute area


	; Test ADR(L) + condition code.
	[ {TRUE}
	AREA	Code14, CODE
	[ :LNOT: REFERENCE
	MACRO
	Inject	$i
	;INFO 0, "\t$i"
	$i
	MEND

	; <base>			; no conditional code (pre-UAL, UAL)
	; <base> "L"			; no conditional code (pre-UAL, UAL)
	; <base> <cond code>		: no L variant (pre-UAL, UAL)
	; <base> <cond code> "L"	; pre-UAL only
	; <base> "L" <cond code>	; UAL only
	MACRO
	Invoke
	Inject	"ADR r1, %f10 - 4"
10
	Inject	"ADRL r2, %f20 - 8"
20

	LCLS	instr0
	LCLS	instr1
	LCLS	instr2
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr0	SETS	"ADR" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
instr1	SETS	"ADR" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "L"	; Pre-UAL
instr2	SETS	"ADRL" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)	; UAL
	Inject	"$instr0 r3, %f30 - 4"
30
	Inject	"$instr1 r4, %f40 - 8"
40
	Inject	"$instr2 r5, %f50 - 8"
50
cnt	SETA	cnt + 1
	WEND
	MEND

	ARM
	Invoke
	|
	ARM
	DCI &e24f1008	; sub	r1, pc, #8
	DCI &e24f2008	; sub	r2, pc, #8
	NOP
	DCI &024f3008	; subeq	r3, pc, #8
	DCI &024f4008	; subeq	r4, pc, #8
	NOP
	DCI &024f5008	; subeq	r5, pc, #8
	NOP
	DCI &124f3008	; subne	r3, pc, #8
	DCI &124f4008	; subne	r4, pc, #8
	NOP
	DCI &124f5008	; subne	r5, pc, #8
	NOP
	DCI &224f3008	; subcs	r3, pc, #8
	DCI &224f4008	; subcs	r4, pc, #8
	NOP
	DCI &224f5008	; subcs	r5, pc, #8
	NOP
	DCI &324f3008	; subcc	r3, pc, #8
	DCI &324f4008	; subcc	r4, pc, #8
	NOP
	DCI &324f5008	; subcc	r5, pc, #8
	NOP
	DCI &424f3008	; submi	r3, pc, #8
	DCI &424f4008	; submi	r4, pc, #8
	NOP
	DCI &424f5008	; submi	r5, pc, #8
	NOP
	DCI &524f3008	; subpl	r3, pc, #8
	DCI &524f4008	; subpl	r4, pc, #8
	NOP
	DCI &524f5008	; subpl	r5, pc, #8
	NOP
	DCI &624f3008	; subvs	r3, pc, #8
	DCI &624f4008	; subvs	r4, pc, #8
	NOP
	DCI &624f5008	; subvs	r5, pc, #8
	NOP
	DCI &724f3008	; subvc	r3, pc, #8
	DCI &724f4008	; subvc	r4, pc, #8
	NOP
	DCI &724f5008	; subvc	r5, pc, #8
	NOP
	DCI &824f3008	; subhi	r3, pc, #8
	DCI &824f4008	; subhi	r4, pc, #8
	NOP
	DCI &824f5008	; subhi	r5, pc, #8
	NOP
	DCI &924f3008	; subls	r3, pc, #8
	DCI &924f4008	; subls	r4, pc, #8
	NOP
	DCI &924f5008	; subls	r5, pc, #8
	NOP
	DCI &a24f3008	; subge	r3, pc, #8
	DCI &a24f4008	; subge	r4, pc, #8
	NOP
	DCI &a24f5008	; subge	r5, pc, #8
	NOP
	DCI &b24f3008	; sublt	r3, pc, #8
	DCI &b24f4008	; sublt	r4, pc, #8
	NOP
	DCI &b24f5008	; sublt	r5, pc, #8
	NOP
	DCI &c24f3008	; subgt	r3, pc, #8
	DCI &c24f4008	; subgt	r4, pc, #8
	NOP
	DCI &c24f5008	; subgt	r5, pc, #8
	NOP
	DCI &d24f3008	; suble	r3, pc, #8
	DCI &d24f4008	; suble	r4, pc, #8
	NOP
	DCI &d24f5008	; suble	r5, pc, #8
	NOP
	DCI &e24f3008	; sub	r3, pc, #8
	DCI &e24f4008	; sub	r4, pc, #8
	NOP
	DCI &e24f5008	; sub	r5, pc, #8
	NOP
	DCI &224f3008	; subcs	r3, pc, #8
	DCI &224f4008	; subcs	r4, pc, #8
	NOP
	DCI &224f5008	; subcs	r5, pc, #8
	NOP
	DCI &324f3008	; subcc	r3, pc, #8
	DCI &324f4008	; subcc	r4, pc, #8
	NOP
	DCI &324f5008	; subcc	r5, pc, #8
	NOP
	]
	]

	END
