; Test KEEP with symbol attribute.

	AREA	Data, DATA

	; KEEP SymB# symbols upfront.
	KEEP	SymB1
	KEEP	SymB2

	DCD	00
SymB0_NoExport
	DCD	01
SymB1	DCD	02
SymB2	EQU	&8000

	DCD	10
SymA0_NoExport
	DCD	11
SymA1	DCD	12
SymA2	EQU	&8000

	; KEEP SymA# symbols afterwards.
	KEEP	SymA1
	KEEP	SymA2

	END
