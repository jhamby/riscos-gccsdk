; Test KEEP without an argument, i.e. try to keep all symbols in the output.

	; Variables should never end up in output.
	GBLL	IgnBool
IgnBool	SETL	{TRUE}
	GBLA	IgnInt
IgnInt	SETA	42
	GBLS	IgnStr
IgnStr	SETS	"Foo"

	AREA	AreaData, DATA

	DCD	0
AreaData1	DCD	1
AreaData2	DCD	2

	AREA	AreaCode, CODE

	MOV	r0, #0
AreaCode1	DCD	1
AreaCode2	DCD	2

	AREA	AreaBased, DATA, BASED r6

	DCD	1
	MOV	r2, #0
AreaBased1	MOV	r3, #0
AreaBased2	DCD	5

	AREA	AreaABS, CODE, ABS

	DCD	1
	MOV	r2, #0
AreaABS1	MOV	r3, #0
AreaABS2	DCD	5

	END
