; Tests &, =, DCB, DCW, DCWU, DCD, DCDU, DCQ, DCQU and DCI.
; RUNOPT: -CPU 3
; We select ARMv3 as otherwise "SWINV" (see below) is not a recognised instruction.

	AREA	Data1, DATA, READONLY

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

	; Eight bytes:
	DCQ	0xFEDCBA9876543210
	DCQ	-0xFEDCBA9876543210
	DCQ	18446744073709551615
	DCQ	18446744073709551616	; Give 64 bit overflow warning and becomes 0
	DCQ	"A", "B"

	; Late expressions :
	DCB	DExpr4,DExpr3,DExpr2,DExpr1
	DCW	DExpr4,DExpr3,DExpr2,DExpr1
	DCD	DExpr4,DExpr3,DExpr2,DExpr1
	; DCQ : not supported
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
	DCQU	&FFEEDDCCBBAA9988

	; Four bytes (representing an instruction):
	DCI	2, &22, &FF00EE00	; Gives unaligned instruction warning + code generated in data area warning

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

	; Eight bytes:
	DCD	&76543210, &FEDCBA98
	DCD	&89abcdf0, &01234567
	DCD	&ffffffff, &ffffffff
	DCD	0, 0
	DCD	0x41,0,0x42,0

	; Late expressions:
	DCB	4,3,2,1
	DCW	4,3,2,1
	DCD	4,3,2,1
	=	"12"

	; Unaligned storage:
	DCB	&11, &22, &33, &44, &55, &66, &77
	DCB	&88, &99, &AA, &BB, &CC, &DD, &EE, &FF

	; Four bytes (representing an instruction):
	;DCD	2, &22, &FF00EE00
	ANDEQ   R0,R0,R2
	ANDEQ   R0,R0,R2,LSR #32
	SWINV   &00EE00

	; IMPORT case:
	IMPORT	ExternalLabel1
	DCD	ExternalLabel1		; Works only for DCD (not DCB,DCW)

	]

	AREA	Data2, DATA, READONLY

	[ :LNOT: REFERENCE
	DCD	1, 2, 3
	DCD	0x1, 0xA, 0xC
	DCD	&1, &A, &F
	DCD	2_0, 2_1, 2_10, 2_100, -2_101
	DCD	3_0, 3_2, 3_210, 3_1202, -3_2210
	DCD	7_0, 7_623352, 7_1234, -7_2346
	DCD	9_0, 9_81526, -9_62321

	DCD	'A', 'a', '\'', 'b'
	DCD	'ab', 'abc', 'abcd'

	|

	DCD	0x00000001, 0x00000002, 0x00000003
	DCD	0x00000001, 0x0000000a, 0x0000000c
	DCD	0x00000001, 0x0000000a, 0x0000000f
	DCD	0x00000000, 0x00000001, 0x00000002, 0x00000004, 0xfffffffb
	DCD	0x00000000, 0x00000002, 0x00000015, 0x0000002f, 0xffffffb5
	DCD	0x00000000, 0x0001a169, 0x000001d2, 0xfffffc9d
	DCD	0x00000000, 0x0000d18e, 0xffff5f82

	DCD	0x41, 0x61, 0x27, 0x62
	DCD	0x6162, 0x616263, 0x61626364

	]

	; DCI test for ARM/CODE16 and THUMB syntax mode.
	AREA	Code, CODE
	[ :LNOT:REFERENCE
	ARM
	DCI	&e3a03004	; Generates 2 bytes

	CODE16
	DCI	&c938		; Generates 2 bytes

	THUMB
	DCI	&c938		; Generates 2 bytes
	DCI.N	&c938		; Generates 2 bytes
	DCI.W	&e8b10238	; Generates 4 bytes
	|
	ARM
	MOV	r3, #4

	CODE16
	LDMIA	r1!, {r3,r4,r5}

	THUMB
	LDM	r1!, {r3,r4,r5}
	LDM	r1!, {r3,r4,r5}
	LDM	r1!, {r3,r4,r5,r9}
	]

	END
