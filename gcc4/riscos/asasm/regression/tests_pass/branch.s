; Tests B, BL, BLX, BX, BXJ.
; -RUNOPT: -CPU=7-A

		AREA	Code1, CODE, READONLY

		; Branch instructions
	[ :LNOT: REFERENCE
		; Label defined upfront, label at pos 0.
label11
		B	label11
		B	label11 + 48
		BL	label11
		BL	label11 + 64

		BLX	label11
		BLX	label11 + 2

		; Label defined upfront, label at pos non-0.
label12
		B	label12
		B	label12 + 48
		BL	label12
		BL	label12 + 64

		BLX	label12
		BLX	label12 + 4

		; Label defined afterwards.
		B	label13
		B	label13 + 48
		BL	label13
		BL	label13 + 64

		BLX	label13
		BLX	label13 + 4

		DCI	0
		DCI	0

label13
	|
		DCI	&eafffffe	; B        Code1 / label11
		DCI	&ea000009	; B        0x30 / label11 + 48
		DCI	&ebfffffc	; BL       Code1 / label11
		DCI	&eb00000b	; BL       0x40 / label11 + 64

		DCI	&fafffffa	; BLX      Code1 / label11
		DCI	&fbfffff9	; BLX      0x2 / label11 + 2

		DCI	&eafffffe	; B        0x18
		DCI	&ea000009	; B        0x48
		DCI	&ebfffffc	; BL       0x18
		DCI	&eb00000b	; BL       0x58

		DCI	&fafffffa	; BLX      0x18
		DCI	&fafffffa	; BLX      0x1c

		DCI	&ea000006	; B        0x50
		DCI	&ea000011	; B        0x80
		DCI	&eb000004	; BL       0x50
		DCI	&eb000013	; BL       0x90

		DCI	&fa000002	; BLX      0x50
		DCI	&fa000002	; BLX      0x54

		DCI	0
		DCI	0
	]

		; Absolute area:
		AREA	Code2, CODE, READONLY
		ORG	&500

		; Branch instructions
	[ :LNOT: REFERENCE
		; Label defined upfront, label at pos 0.
label21
		B	label21
		B	label21 + 48
		BL	label21
		BL	label21 + 64

		BLX	label21
		BLX	label21 + 2

		; Label defined upfront, label at pos non-0.
label22
		B	label22
		B	label22 + 48
		BL	label22
		BL	label22 + 64

		BLX	label22
		BLX	label22 + 4

		; Label defined afterwards.
		B	label23
		B	label23 + 48
		BL	label23
		BL	label23 + 64

		BLX	label23
		BLX	label23 + 4

		DCI	0
		DCI	0
label23
	|
		DCI	&eafffffe	; B        Code2 / label21
		DCI	&ea000009	; B        0x30 / label21 + 48
		DCI	&ebfffffc	; BL       Code2 / label21
		DCI	&eb00000b	; BL       0x40 / label21 + 64

		DCI	&fafffffa	; BLX      Code2 / label21
		DCI	&fbfffff9	; BLX      0x2 / label21 + 2

		DCI	&eafffffe	; B        0x18
		DCI	&ea000009	; B        0x48
		DCI	&ebfffffc	; BL       0x18
		DCI	&eb00000b	; BL       0x58

		DCI	&fafffffa	; BLX      0x18
		DCI	&fafffffa	; BLX      0x1c

		DCI	&ea000006	; B        0x50
		DCI	&ea000011	; B        0x80
		DCI	&eb000004	; BL       0x50
		DCI	&eb000013	; BL       0x90

		DCI	&fa000002	; BLX      0x50
		DCI	&fa000002	; BLX      0x54

		DCI	0
		DCI	0
	]

		; BLX, BX, BXJ + register
		AREA	Code3, CODE

	[ :LNOT: REFERENCE
		BX	R3
		BXJ	R4
		BLX	R5

		BXEQ	R3
		BXNE	R3
		BXCS	R3
		BXHS	R3
		BXCC	R3
		BXLO	R3
		BXMI	R3
		BXPL	R3
		BXVS	R3
		BXVC	R3
		BXHI	R3
		BXLS	R3
		BXGE	R3
		BXLT	R3
		BXGT	R3
		BXLE	R3
		BXAL	R3

		BXJEQ	R4
		BXJNE	R4
		BXJCS	R4
		BXJHS	R4
		BXJCC	R4
		BXJLO	R4
		BXJMI	R4
		BXJPL	R4
		BXJVS	R4
		BXJVC	R4
		BXJHI	R4
		BXJLS	R4
		BXJGE	R4
		BXJLT	R4
		BXJGT	R4
		BXJLE	R4
		BXJAL	R4

		BLXEQ	R5
		BLXNE	R5
		BLXCS	R5
		BLXHS	R5
		BLXCC	R5
		BLXLO	R5
		BLXMI	R5
		BLXPL	R5
		BLXVS	R5
		BLXVC	R5
		BLXHI	R5
		BLXLS	R5
		BLXGE	R5
		BLXLT	R5
		BLXGT	R5
		BLXLE	R5
		BLXAL	R5

	|
		DCI	&e12fff13	; BX	r3
		DCI	&e12fff24	; BXJ	r4
		DCI	&e12fff35	; BLX	r5

		DCI	&012fff13	; BXEQ	R3
		DCI	&112fff13	; BXNE	R3
		DCI	&212fff13	; BXCS	R3
		DCI	&212fff13	; BXHS	R3
		DCI	&312fff13	; BXCC	R3
		DCI	&312fff13	; BXLO	R3
		DCI	&412fff13	; BXMI	R3
		DCI	&512fff13	; BXPL	R3
		DCI	&612fff13	; BXVS	R3
		DCI	&712fff13	; BXVC	R3
		DCI	&812fff13	; BXHI	R3
		DCI	&912fff13	; BXLS	R3
		DCI	&a12fff13	; BXGE	R3
		DCI	&b12fff13	; BXLT	R3
		DCI	&c12fff13	; BXGT	R3
		DCI	&d12fff13	; BXLE	R3
		DCI	&e12fff13	; BXAL	R3

		DCI	&012fff24	; BXJEQ	R4
		DCI	&112fff24	; BXJNE	R4
		DCI	&212fff24	; BXJCS	R4
		DCI	&212fff24	; BXJHS	R4
		DCI	&312fff24	; BXJCC	R4
		DCI	&312fff24	; BXJLO	R4
		DCI	&412fff24	; BXJMI	R4
		DCI	&512fff24	; BXJPL	R4
		DCI	&612fff24	; BXJVS	R4
		DCI	&712fff24	; BXJVC	R4
		DCI	&812fff24	; BXJHI	R4
		DCI	&912fff24	; BXJLS	R4
		DCI	&a12fff24	; BXJGE	R4
		DCI	&b12fff24	; BXJLT	R4
		DCI	&c12fff24	; BXJGT	R4
		DCI	&d12fff24	; BXJLE	R4
		DCI	&e12fff24	; BXJAL	R4

		DCI	&012fff35	; BLXEQ	R5
		DCI	&112fff35	; BLXNE	R5
		DCI	&212fff35	; BLXCS	R5
		DCI	&212fff35	; BLXHS	R5
		DCI	&312fff35	; BLXCC	R5
		DCI	&312fff35	; BLXLO	R5
		DCI	&412fff35	; BLXMI	R5
		DCI	&512fff35	; BLXPL	R5
		DCI	&612fff35	; BLXVS	R5
		DCI	&712fff35	; BLXVC	R5
		DCI	&812fff35	; BLXHI	R5
		DCI	&912fff35	; BLXLS	R5
		DCI	&a12fff35	; BLXGE	R5
		DCI	&b12fff35	; BLXLT	R5
		DCI	&c12fff35	; BLXGT	R5
		DCI	&d12fff35	; BLXLE	R5
		DCI	&e12fff35	; BLXAL	R5

	]

		END
