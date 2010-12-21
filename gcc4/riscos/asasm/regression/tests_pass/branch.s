; Tests B, BL, BLX and BX.

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

		BLX	R0
		BX	R0

label13
	|
		DCD	&eafffffe	; B        Code1 / label11
		DCD	&ea000009	; B        0x30 / label11 + 48
		DCD	&ebfffffc	; BL       Code1 / label11
		DCD	&eb00000b	; BL       0x40 / label11 + 64

		DCD	&fafffffa	; BLX      Code1 / label11
		DCD	&fbfffff9	; BLX      0x2 / label11 + 2

		DCD	&eafffffe	; B        0x18
		DCD	&ea000009	; B        0x48
		DCD	&ebfffffc	; BL       0x18
		DCD	&eb00000b	; BL       0x58
		DCD	&fafffffa	; BLX      0x18
		DCD	&fafffffa	; BLX      0x1c

		DCD	&ea000006	; B        0x50
		DCD	&ea000011	; B        0x80
		DCD	&eb000004	; BL       0x50
		DCD	&eb000013	; BL       0x90
		DCD	&fa000002	; BLX      0x50
		DCD	&fa000002	; BLX      0x54

		DCD	&e12fff30	; BLX      r0
		DCD	&e12fff10	; BX       r0
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

		BLX	R0
		BX	R0

label23
	|
		DCD	&eafffffe	; B        Code2 / label21
		DCD	&ea000009	; B        0x30 / label21 + 48
		DCD	&ebfffffc	; BL       Code2 / label21
		DCD	&eb00000b	; BL       0x40 / label21 + 64

		DCD	&fafffffa	; BLX      Code2 / label21
		DCD	&fbfffff9	; BLX      0x2 / label21 + 2

		DCD	&eafffffe	; B        0x18
		DCD	&ea000009	; B        0x48
		DCD	&ebfffffc	; BL       0x18
		DCD	&eb00000b	; BL       0x58
		DCD	&fafffffa	; BLX      0x18
		DCD	&fafffffa	; BLX      0x1c

		DCD	&ea000006	; B        0x50
		DCD	&ea000011	; B        0x80
		DCD	&eb000004	; BL       0x50
		DCD	&eb000013	; BL       0x90
		DCD	&fa000002	; BLX      0x50
		DCD	&fa000002	; BLX      0x54

		DCD	&e12fff30	; BLX      r0
		DCD	&e12fff10	; BX       r0
	]

		END
