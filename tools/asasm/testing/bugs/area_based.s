; I don't think it is possible to turn this into a REFERENCE based test
; but nevertheless, this shows a bug: the reloc table for Code1 is
; "At 0 : Byte [04] displaced by base of area Code1" is wrong.  We expect
; to see something like "At 0 : base-relative to base of area Data1".

	[ :LNOT: REFERENCE
	AREA	Data1, DATA, READONLY, BASED R3
	|
	AREA	Data1, DATA, READONLY
	]

	DCD	0
lbl1	DCD	40

	AREA	Code1, CODE, READONLY

	[ :LNOT: REFERENCE

	LDR	r1, lbl1

	|

	LDR	r1, [r3, #lbl1]

	]

	END
