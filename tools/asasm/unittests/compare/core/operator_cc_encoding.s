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
	DCB :CC_ENCODING: (("$ccodes" :RIGHT:(len - i)) :LEFT: 2)
i	SETA i + 2
	WEND
	MEND

	CCEnc "EQNECSCCMIPLVSVCHILSGELTGTLEALNVHSLO"
	CCEnc "eqnecsccmiplvsvchilsgeltgtlealnvhslo"
	DCB :CC_ENCODING: ""

	|

	DCB 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 2, 3
	DCB 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 2, 3
	DCB 14

	]

	END