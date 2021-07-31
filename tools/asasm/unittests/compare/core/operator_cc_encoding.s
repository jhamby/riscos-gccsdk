; Tests :CC_ENCODING:

	AREA	Data, DATA
	
	[ :LNOT: REFERENCE

	MACRO
	CCEnc $ccodes
	LCLA len
	LCLA i
len	SETA :LEN:"$ccodes"
i	SETA 0
	WHILE i < len
	DCD :CC_ENCODING: (("$ccodes" :RIGHT:(len - i)) :LEFT: 2)
i	SETA i + 2
	WEND
	MEND

	CCEnc "EQNECSCCMIPLVSVCHILSGELTGTLEALNVHSLO"
	CCEnc "eqnecsccmiplvsvchilsgeltgtlealnvhslo"
	DCD :CC_ENCODING: ""

	|

	DCD 0<<28, 1<<28, 2<<28, 3<<28, 4<<28, 5<<28, 6<<28, 7<<28, 8<<28, 9<<28, 10<<28, 11<<28, 12<<28, 13<<28, 14<<28, 15<<28, 2<<28, 3<<28
	DCD 0<<28, 1<<28, 2<<28, 3<<28, 4<<28, 5<<28, 6<<28, 7<<28, 8<<28, 9<<28, 10<<28, 11<<28, 12<<28, 13<<28, 14<<28, 15<<28, 2<<28, 3<<28
	DCD 14<<28

	]

	END
