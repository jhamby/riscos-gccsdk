	AREA Code, CODE, READONLY

	[ :LNOT:REFERENCE

	; Unary +
	DCD	+(5-3)		; Unary+ <int>
	DCFS	+(5.3)		; Unary+ <float>

	DCD	+"A"
Lbl1	DCD	+Lbl1

	; Unary -
	DCD	-(5-3)		; Unary- <int>
	DCFS	-(5.3)		; Unary- <float>

	DCD	-"A"

	; Unary :NOT:
	DCD	:NOT:3		; :NOT:
	DCD	:NOT:"A"

	|

	DCD	2
	DCFS	5.3

	DCD	0x41		; +"A"
Lbl1	DCD	Lbl1

	DCD	-2
	DCFS	-5.3

	DCD	-0x41		; -"A"

	; Unary :NOT:
	DCD	0xFFFFFFFC
	DCD	0xFFFFFFBE
	]
	
	END
