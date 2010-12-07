; Test the label size operator '?'.

	[ :LNOT: REFERENCE

	^ &1000
Lbl1	# 16

	AREA	Code, CODE, READONLY

	DCD	Lbl1
	DCD	?Lbl1

Lbl2	DCD	?Lbl2
	DCD	?Lbl3
Lbl3	DCD	0

Lbl4	DCB	"abc", ?Lbl4

	|

	AREA	Code, CODE, READONLY

	DCD	0x1000
	DCD	16

	DCD	4
	DCD	4
	DCD	0

	DCB	"abc", 4

	]

	END
