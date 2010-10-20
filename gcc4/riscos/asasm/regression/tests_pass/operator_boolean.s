	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	; :LEOR: is tested in operator_relational.s.

	[ :LNOT:REFERENCE

	; :LAND:
	= "|"
	= :STR:({FALSE} :LAND: {FALSE})
	= :STR:({FALSE} :LAND: {TRUE})
	= :STR:({TRUE} :LAND: {FALSE})
	= :STR:({TRUE} :LAND: {TRUE})

	; &&
	[ EXTENSION
	= "|"
	= :STR:({FALSE} && {FALSE})
	= :STR:({FALSE}&&{TRUE})
	= :STR:({TRUE}&&{FALSE})
	= :STR:({TRUE} && {TRUE})
	]

	; :LOR:
	= "|"
	= :STR:({FALSE} :LOR: {FALSE})
	= :STR:({FALSE} :LOR: {TRUE})
	= :STR:({TRUE} :LOR: {FALSE})
	= :STR:({TRUE} :LOR: {TRUE})

	; ||
	[ EXTENSION
	= "|"
	= :STR:({FALSE} || {FALSE})
	= :STR:({FALSE}||{TRUE})
	= :STR:({TRUE} || {FALSE})
	= :STR:({TRUE} || {TRUE})
	]

	; :LNOT:
	= "|"
	= :STR:(:LNOT:{FALSE})
	= :STR:(:LNOT:{TRUE})

	[ EXTENSION
	; !
	= "|"
	= :STR:(!{FALSE})
	= :STR:(!{TRUE})
	]

	|

	= "|FFFT"	; :LAND:
	[ EXTENSION
	= "|FFFT"	; &&
	]
	= "|FTTT"	; :LOR:
	[ EXTENSION
	= "|FTTT"	; ||
	]
	= "|TF"		; :LNOT:
	[ EXTENSION
	= "|TF"		; !
	]

	]

	END
