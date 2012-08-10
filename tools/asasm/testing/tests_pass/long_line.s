	AREA	Data, DATA, READONLY

	[ :LNOT: REFERENCE
	= "This is a long " \
:CC: "line\n"

	MACRO
	Test
	= "Also a long "\
:CC: "line\n"
	MEND
	Test
	|
	= "This is a long line\n"
	= "Also a long line\n"
	]

	END
