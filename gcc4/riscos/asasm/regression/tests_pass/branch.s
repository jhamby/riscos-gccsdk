		AREA |C$$code|, CODE, READONLY

		; Branch instructions
	[ :LNOT: REFERENCE
		; Label defined upfront, label at pos 0.
label1
		B	label1
		B	label1 + 48
		BL	label1
		BL	label1 + 64

		BLX	label1
		BLX	label1 + 2

		; Label defined upfront, label at pos non-0.
label2
		B	label2
		B	label2 + 48
		BL	label2
		BL	label2 + 64

		BLX	label2
		BLX	label2 + 4

		; Label defined afterwards.
		B	label3
		B	label3 + 48
		BL	label3
		BL	label3 + 64

		BLX	label3
		BLX	label3 + 4

		BLX	R0
		BX	R0

label3
	|
		DCD	&eafffffe	; B        C$$code / label1
		DCD	&ea000009	; B        0x30 / label1 + 48
		DCD	&ebfffffc	; BL       C$$code / label1
		DCD	&eb00000b	; BL       0x40 / label1 + 64

		DCD	&fafffffa	; BLX      C$$code / label1
		DCD	&fbfffff9	; BLX      0x2 / label1 + 2

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
