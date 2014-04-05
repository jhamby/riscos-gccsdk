; Tests RN, FN, CP and CN.

	[ :LNOT: REFERENCE
		AREA	Data, DATA
		; Emtpy area.

		AREA	Code, CODE
		MOV	r1, #1
		FLTD	f2,r1
		STC	p3,c4,[r1,#4]

		MOV	r1, #1
		FLTD	f2,r1
		STC	p3,c4,[r1,#4]
	|
		AREA	Data, DATA
		; Define on purpose registers in a different area.  That
		; shouldn't make a difference.
MyReg1a		RN	1
MyFReg2a	FN	2
MyPReg3a	CP	3
MyCReg4a	CN	4

		; Define registers based on a previous register definition.
MyReg1b		RN	MyReg1a
MyFReg2b	FN	MyFReg2a
MyPReg3b	CP	MyPReg3a
MyCReg4b	CN	MyCReg4a

		; Redefine register with the same value.  That should work.
MyReg1b		RN	1
MyFReg2b	FN	2
MyPReg3b	CP	3
MyCReg4b	CN	4

		AREA	Code, CODE
		MOV	MyReg1a, #1
		FLTD	MyFReg2a,MyReg1a
		STC	MyPReg3a,MyCReg4a,[MyReg1a,#4]

		MOV	MyReg1b, #1
		FLTD	MyFReg2b,MyReg1b
		STC	MyPReg3b,MyCReg4b,[MyReg1b,#4]
	]

	END
