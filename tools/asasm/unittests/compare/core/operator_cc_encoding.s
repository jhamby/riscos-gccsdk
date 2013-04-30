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

	DCD 0<<24, 1<<24, 2<<24, 3<<24, 4<<24, 5<<24, 6<<24, 7<<24, 8<<24, 9<<24, 10<<24, 11<<24, 12<<24, 13<<24, 14<<24, 15<<24, 2<<24, 3<<24
	DCD 0<<24, 1<<24, 2<<24, 3<<24, 4<<24, 5<<24, 6<<24, 7<<24, 8<<24, 9<<24, 10<<24, 11<<24, 12<<24, 13<<24, 14<<24, 15<<24, 2<<24, 3<<24
	DCD 14<<24

	]

	END