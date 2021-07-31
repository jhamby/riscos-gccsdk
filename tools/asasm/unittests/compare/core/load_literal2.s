	[ :LNOT: REFERENCE

	; Simple cases:
	[ {TRUE}
		AREA	Code, CODE, READONLY

		; Don't go crazy when they aren't any literals yet.
		LTORG

		; Simple, simple, word case:
ValBefore1	*	&12345679
		LDR	r1, =&12345678
		LDR	r2, =ValBefore1
		LDR	r3, =ValAfter1
ValAfter1	*	&1234567A

		; Delayed evaluation:
ValBefore2	*	ValueAfter + 3
		LDR	r4, =ValBefore2
		LDR	r5, =ValAfter2		; ObjAsm extension
ValAfter2	*	ValueAfter + 4

ValueAfter	*	&12345678
		LTORG

		; Simple, simple, byte case:
		LDRB	r1, =&808		; Gives warning

		LTORG

		; Reuse:
		LDR	r6, =&11223344
		LDR	r7, =&11223344
		LTORG
		LDR	r8, =&44332211
		LDR	r9, =&11223344
		LDR	r10, =&44332211

		LTORG
	]

	; Using maps:
	[ {TRUE}
		AREA	Data, DATA, READONLY

		^	20
rcrd1f1		#	12
rcrd1f2		#	8
rcrd1f3		#	4

		^	4
rcrd2f1		#	4
rcrd2f2		#	2048
rcrd2f3		#	8

		AREA	Code, CODE, READONLY

		LDR	r0, =rcrd1f2
		LDR	r1, =rcrd2f2
		LDR	r2, =rcrd2f3

		; No LTORG -> as this needs to happen at END
	]

	|

	; Simple cases:
	[ {TRUE}
		AREA	Code, CODE, READONLY

		; Simple, simple, word case:
		LDR	r1, lbl1
		LDR	r2, lbl2
		LDR	r3, lbl3
		LDR	r4, lbl4
		LDR	r5, lbl5

lbl1		DCD	&12345678
lbl2		DCD	&12345679
lbl3		DCD	&1234567a
lbl4		DCD	&1234567b
lbl5		DCD	&1234567c

		; Simple, simple, byte case:
		MOV	r1, #8

		; Reuse:
		LDR	r6, lbl6
		LDR	r7, lbl6
lbl6		DCD	&11223344
		LDR	r8, lbl7
		LDR	r9, lbl6
		LDR	r10, lbl7
lbl7		DCD	&44332211
	]

	; Using maps:
	[ {TRUE}
		AREA	Data, DATA, READONLY

		AREA	Code, CODE, READONLY

		MOV	r0, #32
		MOV	r1, #8
		LDR	r2, [pc, #-4]
		DCD	2048 + 8
	]

	]

	END
