; Tests {ENDIAN}

		GET config.h

		AREA	TestData, DATA, READONLY

	[ :LNOT: REFERENCE
		= {ENDIAN}, 3
	|
		= "little", 3
	]

		END
