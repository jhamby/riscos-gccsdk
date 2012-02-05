; -RUNOPT: -CPU=7-A

	MACRO
	LiteralTest $areaName, $areaOrg

	AREA	$areaName, CODE, READONLY
	$areaOrg

	[ :LNOT: REFERENCE

	LDRB	r1, =&22	; Converts to MOV/MVN
	LDRSB	r2, =&33	; Converts to MOV/MVN
	LDRH	r3, =&4455
	LDR	r5, =&87654321
	LDRSH	r4, =&6677

	; Convertable to MOV/MVN:
	LDRH	r3, =&88
	LDRH	r3, =&8800
	LDRH	r3, =&0880
	LDRSH	r4, =&99
	LDRSH	r3, =&9900	; Not possible to express with MOV/MVN
	LDR	r5, =&00770000
	LDR	r5, =&00007700

	; Reuse tests:
	LDRSH	r5, =&6677
	LDRH	r6, =&8765
	LDRH	r7, =&4321

	LTORG

	; PC relative tests:
	LDRB	r8, =datab$areaName
	LDRSB	r8, =datab$areaName	; Reuse.
	LDRH	r9, =dataw$areaName
	LDRSH	r9, =dataw$areaName	; Reuse.
	LDR	r10, =datad$areaName
	LDR	r10, =datad$areaName	; Reuse.

	LDRB	r8, =databp$areaName + 16
	LDRSB	r8, =databp$areaName + 16	; Reuse.
	LDRH	r9, =datawp$areaName + 16
	LDRSH	r9, =datawp$areaName + 16	; Reuse.
	LDR	r10, =datadp$areaName + 16
	LDR	r10, =datadp$areaName + 16	; Reuse.

databp$areaName	%	2
datawp$areaName	%	2
datadp$areaName	%	12

datab$areaName	DCB	0x11
	ALIGN	2
dataw$areaName	DCW	0x3322
datad$areaName	DCD	0x77665544

	; Implicit LTORG is happening here.
	|

	; Reference:
	MOV	r1, #0x22
	MOV	r2, #0x33
	LDRH	r3, lbl1$areaName
	LDR	r5, lbl2$areaName
	LDRSH	r4, lbl3$areaName

	; Convertable to MOV/MVN:
	MOV	r3, #0x88
	MOV	r3, #0x8800
	MOV	r3, #0x880
	MOV	r4, #0x99
	LDRSH	r3, lbl4$areaName
	MOV	r5, #0x770000
	MOV	r5, #0x7700

	; Reuse tests:
	LDRSH	r5, lbl3$areaName
	LDRH	r6, lbl2$areaName + 2
	LDRH	r7, lbl2$areaName

	; LTORG result:
lbl1$areaName	DCD	0x00004455
lbl2$areaName	DCD	0x87654321
lbl3$areaName	DCW	0x6677
lbl4$areaName	DCW	0x9900

	; PC relative tests:
	; This depends on type of AREA (i.e. absolute or not):
	[ "$areaName" == "CodeNonABS"
	; PC relative tests:
	LDRB	r8, databa1$areaName
	LDRSB	r8, databa1$areaName
	LDRH	r9, datawa1$areaName
	LDRSH	r9, datawa1$areaName
	LDR	r10, datada1$areaName
	LDR	r10, datada1$areaName

	LDRB	r8, databa1$areaName
	LDRSB	r8, databa1$areaName
	LDRH	r9, datawa1$areaName
	LDRSH	r9, datawa1$areaName
	LDR	r10, datada1$areaName
	LDR	r10, datada1$areaName

	%	16

datab$areaName	DCB	0x11
	ALIGN	2
dataw$areaName	DCW	0x3322
datad$areaName	DCD	0x77665544

	; Literal addresses (by implicit LTORG):
	; FIXME: a future optimisation could merge data{b,w,d}a{1,2} together.
databa1$areaName	DCB	datab$areaName
	ALIGN	2
datawa1$areaName	DCW	dataw$areaName
datada1$areaName	DCD	datad$areaName

databa2$areaName	DCB	datab$areaName	; FIXME: not used
	ALIGN	2
datawa2$areaName	DCW	dataw$areaName	; FIXME: not used
datada2$areaName	DCD	datad$areaName	; FIXME: not used
	|
	MOV	r8, #datab$areaName
	MOV	r8, #datab$areaName	; FIXME: wrong, no ? MVN ?
	MOV	r9, #dataw$areaName
	MOV	r9, #dataw$areaName
	MOV	r10, #datad$areaName
	MOV	r10, #datad$areaName

	MOV	r8, #datab$areaName
	MOV	r8, #datab$areaName	; FIXME: wrong, no ? MVN ?
	MOV	r9, #dataw$areaName
	MOV	r9, #dataw$areaName
	MOV	r10, #datad$areaName
	MOV	r10, #datad$areaName

	%	16

datab$areaName	DCB	0x11
	ALIGN	2
dataw$areaName	DCW	0x3322
datad$areaName	DCD	0x77665544
	]

	]
	MEND

	; Non-absolute area:
	LiteralTest	CodeNonABS
	[ {FALSE}	; FIXME: turn this into {TRUE}
	; Absolute area:
	LiteralTest	CodeABS, "ORG &50"
	]

	END
