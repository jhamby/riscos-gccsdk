; Tests &, =, DCB, DCW, DCWU, DCD, DCDU and DCI.

	AREA	Data, DATA, READONLY

	[ :LNOT: REFERENCE

	; One byte:
	DCB	"t", "est"

	DCB	4,3,2,1
	DCB	8 , 0x7 , &6 , 5
	DCB	8+4, 12-1, 5*2, 3*3

	=	116, "es", 116

	; Two bytes:
	DCW	4,3,2,1
	DCW	8 , 7 , 6 , 5
	DCW	8+4, 12-1, 5*2, 3*3
	DCW	"A", "B", "C", "D"

	; Four bytes:
	DCD	4,3,2,1
	DCD	8 , 7 , 6 , 5
	DCD	8+4, 12-1, 5*2, 3*3
	DCD	"A", "B", "C"

	&	4,3,2,1
	&	8 , 7 , 6 , 5
	&	8+4, 12-1, 5*2, 3*3
	&	"A", "B", "C"

	; Late expressions :
	DCB	DExpr4,DExpr3,DExpr2,DExpr1
	DCW	DExpr4,DExpr3,DExpr2,DExpr1
	DCD	DExpr4,DExpr3,DExpr2,DExpr1
DExpr4	*	4
DExpr3	*	3
DExpr2	*	2
DExpr1	*	1

	=	DExpr5, DExpr6
DExpr5	*	"1"
DExpr6	*	"2"

	; Unaligned storage:
	DCB	&11
	DCWU	&3322
	DCDU	&77665544
	ALIGN

	; Four bytes (representing an instruction):
	DCI	2, &22, &FF00EE00

	; IMPORT case:
	IMPORT	ExternalLabel1
	DCD	ExternalLabel1		; Works only for DCD (not DCB,DCW)

	|

	; One byte:
	=	"te"
	DCB	115,116

	DCB	4,3,2,1
	DCB	8,7,6,5
	DCB	12,11,10,9

	=	"test"

	; Two bytes:
	DCB	4,0,3,0
	DCW	0x0002, 0x0001
	DCB	8,0,7,0,6,0,5,0
	DCB	12,0,11,0,10,0,9,0
	DCB	0x41, 0, 0x42, 0, 0x43, 0, 0x44, 0

	; Four bytes:
	DCW	4,0,3,0
	DCW	2,0,1,0
	DCB	8,0,0,0,7,0,0,0,6,0,0,0,5,0,0,0
	DCB	12,0,0,0,11,0,0,0,10,0,0,0,9,0,0,0
	DCB	0x41,0,0,0,0x42,0,0,0,0x43,0,0,0

	DCW	4,0,3,0
	DCW	2,0,1,0
	DCB	8,0,0,0,7,0,0,0,6,0,0,0,5,0,0,0
	DCB	12,0,0,0,11,0,0,0,10,0,0,0,9,0,0,0
	DCB	0x41,0,0,0,0x42,0,0,0,0x43,0,0,0

	; Late expressions:
	DCB	4,3,2,1
	DCW	4,3,2,1
	DCD	4,3,2,1
	=	"12"

	; Unaligned storage:
	DCB	&11, &22, &33, &44, &55, &66, &77
	DCB	0

	; Four bytes (representing an instruction):
	DCD	2, &22, &FF00EE00

	; IMPORT case:
	IMPORT	ExternalLabel1
	DCD	ExternalLabel1		; Works only for DCD (not DCB,DCW)

	]

	END
