; Test the label size operator '?'.

	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

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

	^	&1000
RecSp1	#	16
RecSp2	#	8
RecSp3	#	4

Equ1	*	5
Equ2	*	10

	AREA Data2, DATA, READONLY

	%	4
DataSp1	%	8
	%	12
DataSp2	%	16

	AREA	Code2, CODE, READONLY

	[ :LNOT: REFERENCE

	DCD	?RecSp1
	DCD	?RecSp2
	DCD	?RecSp3
	DCD	?Equ1
	DCD	?Equ2
	DCD	?DataSp1
	DCD	?DataSp2

	LDR	r0, =?RecSp1
	MOV	r0, #?RecSp2
	MOV	r0, r1, LSL#?RecSp3
	MOV	r0, #?DataSp1

Foo	; Nothing here.
	DCD	?Foo
Bar	DCD	?Bar
	[ EXTENSION
FltLbl	DCFS	?FltLbl - 0.		; Not supported by objasm
DblLbl	DCFD	?DblLbl	- 0.	; Not supported by objasm
	]

	; String
StrLen	=	"8 bytes", 0
	DCD	?StrLen

	; Align
	[ EXTENSION
	=	"X"
AlgnLbl	ALIGN			; Not supported by objasm
	DCD	?AlgnLbl
	]

	|

	DCD	16
	DCD	8
	DCD	4
	DCD	0
	DCD	0
	DCD	8
	DCD	16

	MOV	r0, #16
	MOV	r0, #8
	MOV	r0, r1, LSL#4
	MOV	r0, #8

Foo
	DCD	0		; Label defined without output.
Bar	DCD	4		; Label defined and used in one line
	[ EXTENSION
	DCFS	4.		; Label defined and its size is used as floating point constant
	DCFD	8.		; Label defined and its size is used as floating point constant
	]

	; String:
StrLen	=	"8 bytes", 0
	DCD	8

	; Align
	[ EXTENSION
	=	"X"
AlgnLbl	ALIGN
	DCD	3
	]

	]

	; Test 3
	AREA	CodeTest3, CODE
	[ :LNOT: REFERENCE
	^ 0
Val1	# 5
Val2	# &110 - @
	LDR	r3, =?Val2
	|
	LDR	r3, =&110 - 5
	]

	; Test 4 : late label
	AREA	CodeTest4, CODE
	[ :LNOT: REFERENCE
	DCD	?latelbl
latelbl	%	8
	|
	DCD	8
	DCD	0
	DCD	0
	]

	END
