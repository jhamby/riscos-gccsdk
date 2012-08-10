; Test BIN / INCBIN

	AREA Data, DATA

	[ :LNOT: REFERENCE

	= "abc"
	BIN tests_pass/include/def.txt
	= "ghi", 10

	= "abc"
	INCBIN tests_pass/include/def.txt
	= "ghi", 10

	|
	
	= "abcdefghi", 10
	= "abcdefghi", 10

	]

	END