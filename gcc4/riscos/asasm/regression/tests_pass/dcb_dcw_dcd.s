	AREA	Data, DATA, READONLY

	[ :LNOT: REFERENCE

	DCB	"t", "est"

	DCB	4,3,2,1
	DCB	8 , 0x7 , &6 , 5
	DCB	8+4, 12-1, 5*2, 3*3

	DCW	4,3,2,1
	DCW	8 , 7 , 6 , 5
	DCW	8+4, 12-1, 5*2, 3*3

	DCD	4,3,2,1
	DCD	8 , 7 , 6 , 5
	DCD	8+4, 12-1, 5*2, 3*3

	DCB	DExpr4,DExpr3,DExpr2,DExpr1
	DCW	DExpr4,DExpr3,DExpr2,DExpr1
	DCD	DExpr4,DExpr3,DExpr2,DExpr1
DExpr4	*	4
DExpr3	*	3
DExpr2	*	2
DExpr1	*	1

	IMPORT	ExternalLabel1
	DCD	ExternalLabel1		; Works only for DCD (not DCB,DCW)

	|

	=	"te"
	DCB	115,116

	DCB	4,3,2,1
	DCB	8,7,6,5
	DCB	12,11,10,9

	DCB	4,0,3,0
	DCW	0x0002, 0x0001
	DCB	8,0,7,0,6,0,5,0
	DCB	12,0,11,0,10,0,9,0

	DCW	4,0,3,0
	DCW	2,0,1,0
	DCB	8,0,0,0,7,0,0,0,6,0,0,0,5,0,0,0
	DCB	12,0,0,0,11,0,0,0,10,0,0,0,9,0,0,0

	DCB	4,3,2,1
	DCW	4,3,2,1
	DCD	4,3,2,1

	IMPORT	ExternalLabel1
	DCD	ExternalLabel1		; Works only for DCD (not DCB,DCW)

	]

	END
