; Tests :LAND:, &&, :LOR:, ||, :LNOT:, ! and :LEOR:

	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	[ :LNOT:REFERENCE

	; :LAND:
	= "Test :LAND:\n"
	= :STR:({FALSE} :LAND: {FALSE})
	= :STR:({FALSE} :LAND: {TRUE})
	= :STR:({TRUE} :LAND: {FALSE})
	= :STR:({TRUE} :LAND: {TRUE})
	= "\n"

	; &&
	[ EXTENSION
	= "Test &&\n"
	= :STR:({FALSE} && {FALSE})
	= :STR:({FALSE}&&{TRUE})
	= :STR:({TRUE}&&{FALSE})
	= :STR:({TRUE} && {TRUE})
	= "\n"
	]

	; :LOR:
	= "Test :LOR:\n"
	= :STR:({FALSE} :LOR: {FALSE})
	= :STR:({FALSE} :LOR: {TRUE})
	= :STR:({TRUE} :LOR: {FALSE})
	= :STR:({TRUE} :LOR: {TRUE})
	= "\n"

	; ||
	[ EXTENSION
	= "Test ||\n"
	= :STR:({FALSE} || {FALSE})
	= :STR:({FALSE}||{TRUE})
	= :STR:({TRUE} || {FALSE})
	= :STR:({TRUE} || {TRUE})
	= "\n"
	]

	; :LNOT:
	= "Test :LNOT:\n"
	= :STR:(:LNOT:{FALSE})
	= :STR:(:LNOT:{TRUE})
	= "\n"

	[ EXTENSION
	; !
	= "Test !\n"
	= :STR:(!{FALSE})
	= :STR:(!{TRUE})
	= "\n"
	]

	; :LEOR:
	= "Test :LEOR:\n"
	= :STR:({FALSE} :LEOR: {FALSE})
	= :STR:({FALSE}:LEOR:{TRUE})
	= :STR:({TRUE}:LEOR:{FALSE})
	= :STR:({TRUE}:LEOR:{TRUE})
	= "\n"

	|

	= "Test :LAND:\nFFFT\n"	; :LAND:
	[ EXTENSION
	= "Test &&\nFFFT\n"	; &&
	]
	= "Test :LOR:\nFTTT\n"	; :LOR:
	[ EXTENSION
	= "Test ||\nFTTT\n"	; ||
	]
	= "Test :LNOT:\nTF\n"	; :LNOT:
	[ EXTENSION
	= "Test !\nTF\n"	; !
	]
	= "Test :LEOR:\nFTTF\n"	; :LEOR:

	]

	END
