
	;Labels defined upfront:
	^	0
tst1_b	#	4
tst2_b	#	4

	^	12, r9
tst3_b	#	8
tst4_b	#	4
tst5_b	#	16

		AREA	Data1, DATA, READONLY, BASED r11

	%	20
tst6_b	%	12
tst7_b	%	8
tst8_b	%	4

		AREA	Code1, CODE, READONLY
	[ :LNOT: REFERENCE
		; Normal offset:
		LDR	r1, [r2, #tst1_b]
		STR	r1, [r2, #tst2_b]
		LDR	r1, [r2, #tst1_a]
		STR	r1, [r2, #tst2_a]

		; Offset containing base register:
		LDR	r2, tst4_b
		STR	r2, tst5_b
		LDR	r2, tst4_a
		STR	r2, tst5_a

		; Offset in an area marked with base register:
		[ {FALSE}		; Results in relocations.
		LDR	r3, tst7_b
		STR	r3, tst8_b
		LDR	r3, tst7_a
		STR	r3, tst8_a
		]
	|
		LDR	r1, [r2]
		STR	r1, [r2, #4]
		LDR	r1, [r2]
		STR	r1, [r2, #4]

		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]
		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]

		[ {FALSE}		; Results in relocations.
		LDR	r3, [r11, #0x20]
		STR	r3, [r11, #0x28]
		LDR	r3, [r10, #0x20]
		STR	r3, [r10, #0x28]
		]
	]

		AREA	Code2, CODE, READONLY
		ORG	&500
	[ :LNOT: REFERENCE
		; Normal offset:
		LDR	r1, [r2, #tst1_b]
		STR	r1, [r2, #tst2_b]
		LDR	r1, [r2, #tst1_a]
		STR	r1, [r2, #tst2_a]

		; Offset containing base register:
		LDR	r2, tst4_b
		STR	r2, tst5_b
		LDR	r2, tst4_a
		STR	r2, tst5_a

		; Offset in an area marked with base register:
		[ {FALSE}		; Results in relocations.
		LDR	r3, tst7_b
		STR	r3, tst8_b
		LDR	r3, tst7_a
		STR	r3, tst8_a
		]
	|
		LDR	r1, [r2]
		STR	r1, [r2, #4]
		LDR	r1, [r2]
		STR	r1, [r2, #4]

		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]
		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]

		[ {FALSE}		; Results in relocations.
		LDR	r3, [r11, #0x20]
		STR	r3, [r11, #0x28]
		LDR	r3, [r10, #0x20]
		STR	r3, [r10, #0x28]
		]
	]

	; Labels defined afterwards:
	^	0
tst1_a	#	4
tst2_a	#	4

	^	12, r9
tst3_a	#	8
tst4_a	#	4
tst5_a	#	16

		AREA	Data2, DATA, READONLY, BASED r10

	%	20
tst6_a	%	12
tst7_a	%	8
tst8_a	%	4


	; Load done in ABS AREA:
		AREA	CodeABS, CODE
		ORG	&20

		%	&10
	[ :LNOT: REFERENCE

Offset	*	&8
Start1
		LDR	r0, Start1
		LDR	r1, Start1 + Offset
		LDR	r2, Start2
		LDR	r3, Start2 + Offset
Start2
	|
		DCI	&e51f0008	; LDR      r0,0x30
		DCI	&e51f1004	; LDR      r1,0x38
		DCI	&e59f2000	; LDR      r2,0x40
		DCI	&e59f3004	; LDR      r3,0x48
	]

		END
