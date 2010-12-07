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
	]

	END
