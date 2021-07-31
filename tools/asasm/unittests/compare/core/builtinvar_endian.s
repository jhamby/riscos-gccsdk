; Tests built-in variable {ENDIAN}

		AREA	TestData, DATA, READONLY

	[ :LNOT: REFERENCE
		= {ENDIAN}, 3
	|
		= "little", 3
	]

		END
