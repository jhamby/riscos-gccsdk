; Test KEEP without an argument, i.e. try to keep all symbols in the output.

	; Variables should never end up in output.
	GBLL	IgnBool
IgnBool	SETL	{TRUE}
	GBLA	IgnInt
IgnInt	SETA	42
	GBLS	IgnStr
IgnStr	SETS	"Foo"

		; Map variables neither.
		^	20
		#	4
IgnMapOff1	#	4
IgnMapOff2	#	8

		^	4, r12
		#	4
IgnRegOff1	#	8
IgnRegOff2	#	3

		AREA	AreaData, DATA

		DCD	0
AreaData1	DCD	1
AreaData2	DCD	2
IgnAreaData3	EQU	AreaData2

		AREA	AreaCode, CODE

		MOV	r0, #0
AreaCode1	DCD	1
AreaCode2	DCD	2
IgnAreaCode3	EQU	AreaCode2

		AREA	AreaBased, DATA, BASED r6

		DCD	1
		MOV	r2, #0
AreaBased1	MOV	r3, #0
AreaBased2	DCD	5
IgnAreaBased3	EQU	AreaBased2	; NOT kept in output

		AREA	AreaABS, CODE, ABS

		DCD	1
		MOV	r2, #0
AreaABS1	MOV	r3, #0
AreaABS2	DCD	5
IgnAreaABS3	EQU	AreaABS2	; NOT kept in output, FIXME: this is however happening.

		; Local labels should never be exported.
		AREA	AreaCode, CODE
01
		BEQ	%b01
		BNE	%f01
01

	END
