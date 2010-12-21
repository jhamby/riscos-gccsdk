
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
; FIXME		LDR	r1, [r2, #tst1_a]
; FIXME		STR	r1, [r2, #tst2_a]

		; Offset containing base register:
		LDR	r2, tst4_b
		STR	r2, tst5_b
		LDR	r2, tst4_a
		STR	r2, tst5_a

		; Offset in an area marked with base register:
		LDR	r3, tst7_b
		STR	r3, tst8_b
		LDR	r3, tst7_a
		STR	r3, tst8_a
	|
		LDR	r1, [r2]
		STR	r1, [r2, #4]
; FIXME		LDR	r1, [r2]
; FIXME		STR	r1, [r2, #4]

		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]
		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]

		LDR	r3, [r11, #0x20]
		STR	r3, [r11, #0x28]
		LDR	r3, [r10, #0x20]
		STR	r3, [r10, #0x28]
	]

		AREA	Code2, CODE, READONLY
		ORG	&500
	[ :LNOT: REFERENCE
		; Normal offset:
		LDR	r1, [r2, #tst1_b]
		STR	r1, [r2, #tst2_b]
; FIXME		LDR	r1, [r2, #tst1_a]
; FIXME		STR	r1, [r2, #tst2_a]

		; Offset containing base register:
		LDR	r2, tst4_b
		STR	r2, tst5_b
		LDR	r2, tst4_a
		STR	r2, tst5_a

		; Offset in an area marked with base register:
		LDR	r3, tst7_b
		STR	r3, tst8_b
		LDR	r3, tst7_a
		STR	r3, tst8_a
	|
		LDR	r1, [r2]
		STR	r1, [r2, #4]
; FIXME		LDR	r1, [r2]
; FIXME		STR	r1, [r2, #4]

		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]
		LDR	r2, [r9, #0x14]
		STR	r2, [r9, #0x18]

		LDR	r3, [r11, #0x20]
		STR	r3, [r11, #0x28]
		LDR	r3, [r10, #0x20]
		STR	r3, [r10, #0x28]
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

		END
