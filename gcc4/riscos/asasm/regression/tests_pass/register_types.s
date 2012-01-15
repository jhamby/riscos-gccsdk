	[ :LNOT: REFERENCE
		AREA	Data, DATA

		AREA	Code, CODE

		MOV	r1, #1
		FLTD	f2,r1
		STC	p3,c4,[r1,#4]
	|
		AREA	Data, DATA
		; Define on purpose registers in a different area.  That
		; shouldn't make a difference.
MyReg1		RN	1
MyFReg2		FN	2
MyPReg3		CP	3
MyCReg4		CN	4

		AREA	Code, CODE
		MOV	MyReg1, #1
		FLTD	MyFReg2,MyReg1
		STC	MyPReg3,MyCReg4,[MyReg1,#4]
	]
	END