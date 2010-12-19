	AREA	Code, CODE, READONLY

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
	LDRB	r8, =datab
	LDRSB	r8, =datab	; Reuse.
	LDRH	r9, =dataw
	LDRSH	r9, =dataw	; Reuse.
	LDR	r10, =datad
	LDR	r10, =datad	; Reuse.

	LDRB	r8, =databp + 16
	LDRSB	r8, =databp + 16	; Reuse.
	LDRH	r9, =datawp + 16
	LDRSH	r9, =datawp + 16	; Reuse.
	LDR	r10, =datadp + 16
	LDR	r10, =datadp + 16	; Reuse.

databp	%	2
datawp	%	2
datadp	%	12

datab	DCB	0x11
	ALIGN	2
dataw	DCW	0x3322
datad	DCD	0x77665544

	|

	MOV	r1, #0x22
	MOV	r2, #0x33
	LDRH	r3, lbl1
	LDR	r5, lbl2
	LDRSH	r4, lbl3

	; Convertable to MOV/MVN:
	MOV	r3, #0x88
	MOV	r3, #0x8800
	MOV	r3, #0x880
	MOV	r4, #0x99
	LDRSH	r3, lbl4
	MOV	r5, #0x770000
	MOV	r5, #0x7700

	; Reuse tests:
	LDRSH	r5, lbl3
	LDRH	r6, lbl2 + 2
	LDRH	r7, lbl2

lbl1	DCD	0x00004455
lbl2	DCD	0x87654321
lbl3	DCW	0x6677
lbl4	DCW	0x9900

	; PC relative tests:
	LDRB	r8, databa1
	LDRSB	r8, databa1
	LDRH	r9, datawa1
	LDRSH	r9, datawa1
	LDR	r10, datada1
	LDR	r10, datada1

	LDRB	r8, databa2
	LDRSB	r8, databa2
	LDRH	r9, datawa2
	LDRSH	r9, datawa2
	LDR	r10, datada2
	LDR	r10, datada2

	%	16

datab	DCB	0x11
	ALIGN	2
dataw	DCW	0x3322
datad	DCD	0x77665544

	; Literal addresses:
	; FIXME: a future optimisation could merge data{b,w,d}a{1,2} together.
databa1	DCB	datab
	ALIGN	2
datawa1	DCW	dataw
datada1	DCD	datad

databa2	DCB	datab
	ALIGN	2
datawa2	DCW	dataw
datada2	DCD	datad

	]

	END
