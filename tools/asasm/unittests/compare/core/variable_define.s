; Test variable definition + redefinition.

	AREA Data, DATA

	[ :LNOT: REFERENCE	

	GBLA Int
Int	SETA 42
	GBLL Bool
Bool	SETL {TRUE}
	GBLS String
String	SETS "Hi"

	= "Test 1 : '", String, :STR:Int, :STR:Bool, "'\n"

	; Redefining variable will reset its value.
	GBLA Int
	GBLL Bool
	GBLS String

	= "Test 2 : '", String, :STR:Int, :STR:Bool, "'\n"

	|

	= "Test 1 : 'Hi0000002AT'\n"
	= "Test 2 : '00000000F'\n"

	]
	
	END
