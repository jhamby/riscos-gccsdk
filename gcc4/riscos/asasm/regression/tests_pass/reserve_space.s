; Tests %, SPACE and FILL.

	AREA Data, DATA

	[ :LNOT: REFERENCE
		% 5
		FILL 3  , 42 + &300
		FILL 2
		SPACE 4
		FILL 5 ,  0x1234  , 2
	|
		DCB 0, 0, 0, 0, 0
		DCB 42, 42, 42
		DCB 0, 0
		DCB 0, 0, 0, 0
		DCW 0x1234, 0x1234, 0x1234, 0x1234, 0x1234 
	]

	END

