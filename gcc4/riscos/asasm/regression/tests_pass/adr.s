; ADR / ADRL tests:

	[ {FALSE}	; FIXME: results in relocations, how to test ?
	; ADR/ADRL simple label cases
	AREA	Code1, CODE, READONLY
	DCB	"Cde1"

	AREA	Code2, CODE, READONLY

	; First defined, then used.
c1lbl1	%	40
c1lbl2	%	8
	DCB	"Cde2"

	ADR	r1, c1lbl1
	ADR	r2, c1lbl2
	ADR	r3, c1lbl3
	ADR	r4, c1lbl4

	ADRL	r1, c1lbl1
	ADRL	r2, c1lbl2
	ADRL	r3, c1lbl3
	ADRL	r4, c1lbl4

	; ADR/ADRL to declared, current and not-yet declared AREAs
	ADR	r5, Code1
	ADR	r6, Code2
	ADR	r7, Code3
	ADRL	r8, Code1
	ADRL	r9, Code2
	ADRL	r10, Code3

	; First used, then defined.
c1lbl3	%	40
c1lbl4	%	8

	AREA	Code3, CODE, READONLY
	DCB	"Cde3"
	]

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
	[ EXTENSION
	; ADR/ADRL references to labels in register based AREA.
	; ObjAsm extension
	AREA	Data6, DATA, READONLY, BASED r3
dt6lbl1	%	8
dt6lbl2	%	32
dt6lbl3	%	4

	AREA	Code6, CODE, READONLY

	[ :LNOT: REFERENCE
	ADR	r1, dt6lbl2	; ObjAsm extension : ADD r1, r3, #dt6lbl2 - Data6
	|
	ADD	r1, r3, #8
	]
	]
	]

	[ {FALSE}	; FIXME: results in relocations, how to test ?
	; External label usage in ADR/ADRL.
	AREA	Code7, CODE, READONLY

	IMPORT	ExternLabel

	ADR	r3, ExternLabel
	ADRL	r4, ExternLabel
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
	MOV	r2, #0x23
	ADD	r2, r2, #0x100

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
	ADRL	r4, tstlbl2	; Generates warning, ADR is used. FIXME: no longer :( - twopass regression
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
	ADD r4, r3, #20	;ADRL -> ADR FIXME: no longer :( - twopass regression
	ADD r4, r4, #0		; FIXME: should go - twopass regression
	ADD r5, r7, #20	;ADR
	ADD r6, r7, #20	;ADRL
	ADD r6, r6, #0
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
	ADRL	r2, %f02		; ADD r2, pc, #... + ADD r2, r2, #0
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
	MOV	r1, #&210		; ADD r1, pc, #-8 - 4 would also be possible
	MOV	r2, #&220		; ADD r2, pc, #-8 + 8
	ADD	r2, r2, #0
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
	MOV	r1, #&10		; ADD r1, pc, #-8 - 4 would also be possible
	MOV	r2, #&1C		; ADD r2, pc, #-8 + 4 would also be possible
	DCD	2

	DCD	3
	%	&1100 - {PC}
	MOV	r3, #16 + 4*4
	MOV	r4, #&2200
	%	&1100 - 8
	DCD	4
	]
	]

	END
