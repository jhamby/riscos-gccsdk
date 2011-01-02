; Tests WHILE / WEND

	AREA	Data, DATA

	[ :LNOT: REFERENCE
		GBLA	cntOut
cntOut		SETA	3
		WHILE	cntOut > 0
			GBLA	cntIn
cntIn			SETA	5
			WHILE	cntIn > 0
				DCB	64 + cntIn
cntIn				SETA	cntIn - 1
			WEND
			ALIGN
cntOut			SETA	cntOut - 1
		WEND

		WHILE	{FALSE}
			INFO	1, "This may not happen"
		WEND
	|
		= "EDCBA"
		ALIGN
		= "EDCBA"
		ALIGN
		= "EDCBA"
		ALIGN
	]

	END
