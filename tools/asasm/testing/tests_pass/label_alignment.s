; Tests if a label in front of an ARM instruction gets aligned to a 4 byte
; boundary.

	AREA	Code, CODE

	[ :LNOT: REFERENCE
	; Test 1
	MOV	r0, #1
	=	"ab"
Lbl1	MOV	r0, #2		; Label Lbl1 will be aligned to next 4b boundary
	DCD	Lbl1

	; Test 2
	MOV	r0, #1
	=	"cd"
Lbl2				; Label Lbl2 will *not* be aligned to next 4b boundary
	MOV	r0, #2
	DCD	Lbl2

	; Test 3
	MACRO
$lbl	MOVNo
	=	"c"
$lbl	=	"d"
	MEND

	MOV	r1, #1
	=	"ab"
Lbl3	MOVNo
	DCD	Lbl3

	; Test 4 : label given to ROUT also aligns that label
	= "ab"
Lbl4	ROUT
	= "cd"
	DCD	Lbl4

	|
	; Test 1
	MOV	r0, #1
	DCD	&6261
Lbl1	MOV	r0, #2
	DCD	Lbl1

	; Test 2
	MOV	r0, #1
	DCW	&6463
Lbl2	DCW	0
	MOV	r0, #2
	DCD	Lbl2

	; Test 3
	MOV	r1, #1
	=	"abc"
Lbl3	=	"d"
	DCD	Lbl3

	; Test 4
	DCD	&6261
Lbl4	DCD	&6463
	DCD	Lbl4
	]

	END
