; Test BIN / INCBIN

	AREA Data, DATA

	[ :LNOT: REFERENCE

	= "abc"
	BIN include/def.txt
	= "ghi", 10

	= "abc"
	INCBIN include/def.txt
	= "ghi", 10

	|
	
	= "abcdefghi", 10
	= "abcdefghi", 10

	]

	END
