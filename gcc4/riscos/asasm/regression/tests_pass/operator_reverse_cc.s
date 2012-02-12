; Tests :REVERSE_CC:

	AREA	Data, DATA
	
	[ :LNOT: REFERENCE

	MACRO
	RevCC $ccodes
	LCLA len
	LCLA i
len	SETA :LEN:"$ccodes"
i	SETA 0
	WHILE i < len
	= :REVERSE_CC: (("$ccodes" :RIGHT:(len - i)) :LEFT: 2)
i	SETA i + 2
	WEND
	MEND

	= "Test 1: Uppercase\n"
	RevCC "EQNECSCCMIPLVSVCHILSGELTGTLEALNVHSLO"
	= "\n"

	= "Test 2: Lowercase\n"
	RevCC "eqnecsccmiplvsvchilsgeltgtlealnvhslo"
	= "\n"

	= "Test 3: Empty string\n"
	= :REVERSE_CC: ""
	= "\n"

	|

	= "Test 1: Uppercase\n"
	= "NEEQCCCSPLMIVCVSLSHILTGELEGTNVALLOHS"
	= "\n"

	= "Test 2: Lowercase\n"
	= "neeqcccsplmivcvslshiltgelegtnvallohs"
	= "\n"

	= "Test 3: Empty string\n"
	= "NV"				; The uppercase NV is a bit arbitrary.
	= "\n"

	]

	END