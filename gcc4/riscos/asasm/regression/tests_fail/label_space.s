; Cases not work:
;   <lbl> <mnemonic> ?<lbl>  : I.e. use the size of the label in the same
;                              line where it is defined.
;   LDR r0, =?<lbl> -> MOV r0, #?<lbl> when possible.
	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	^	&1000
RecSp1	#	16
RecSp2	#	8
RecSp3	#	4

Equ1	*	5
Equ2	*	10

	AREA Data, DATA, READONLY

	%	4
DataSp1	%	8
	%	12
DataSp2	%	16

	AREA	Code, CODE, READONLY

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
FltLbl	DCFS	?FltLbl		; Not supported by objasm
DblLbl	DCFD	?DblLbl		; Not supported by objasm
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
	DCD	4		; Label defined and its size is used as floating point constant
	DCD	8		; Label defined and its size is used as floating point constant
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

	END
