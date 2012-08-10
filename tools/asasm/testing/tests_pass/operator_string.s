; Tests :LEFT:, :RIGHT:, :CC:, :LEN:, :STR:, :LOWERCASE:, :UPPERCASE:

	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	[ :LNOT: REFERENCE

	= "*** TEST :LEFT: ***"
	= "ABC":LEFT:0
	= "DEF" :LEFT:1
	= "GHI":LEFT: 2
	= "JKL" :LEFT: 3

	= "*** TEST :RIGHT: ***"
	= "ABC":RIGHT:0
	= "DEF" :RIGHT:1
	= "GHI":RIGHT: 2
	= "JKL" :RIGHT: 3

	= "*** TEST :CC: ***"
	= "":CC:""
	= "A" :CC:""
	= "":CC: "B"
	= "C" :CC: "D"

	= "*** TEST :LEN: ***"
	ALIGN
	DCD :LEN:""
	DCD :LEN:"ABC"
	DCD :LEN:"AB\0C"
	DCD :LEN:("ABC" :CC: "DEF")
	DCD :LEN: "ABC" - 1

	= "*** TEST :CHR: ***"
	= ">", :CHR:0, "<"
	= ">", :CHR:-1, "<"
	= ">", :CHR:(256 + 65), "<"
	= ">", :CHR:(:NOT:-66), "<"

	= "*** TEST :STR: ***"
	= ">", :STR:5, "<"
	= ">", :STR:-1, "<"
	= ">", :STR:(4/3), "<"
	= ">", :STR:{FALSE}, "<"
	= ">", :STR:{TRUE}, "<"
	[ EXTENSION
	= ">", :STR:(1./3.), "<"
	]

	= "*** TEST :LOWERCASE: ***"
	= ">", :LOWERCASE:"#$ABCabc()", "<"

	= "*** TEST :UPPERCASE: ***"
	= ">", :UPPERCASE:"#$ABCabc()", "<"

	|

	= "*** TEST :LEFT: ***"
	= ""
	= "D"
	= "GH"
	= "JKL"

	= "*** TEST :RIGHT: ***"
	= ""
	= "F"
	= "HI"
	= "JKL"

	= "*** TEST :CC: ***"
	= ""
	= "A"
	= "B"
	= "CD"

	= "*** TEST :LEN: ***"
	ALIGN
	DCD 0
	DCD 3
	DCD 4
	DCD 6
	DCD 2

	= "*** TEST :CHR: ***"
	= ">\0<"
	= ">\377<"
	= ">A<"
	= ">A<"

	= "*** TEST :STR: ***"
	= ">00000005<"
	= ">FFFFFFFF<"
	= ">00000001<"
	= ">F<"
	= ">T<"
	[ EXTENSION
	= ">0.333333<"
	]

	= "*** TEST :LOWERCASE: ***"
	= ">", :LOWERCASE:"#$abcabc()", "<"

	= "*** TEST :UPPERCASE: ***"
	= ">", :UPPERCASE:"#$ABCABC()", "<"

	]

	; :STR: should work on . and {PC} for ABS areas.
	AREA	Code2, CODE
	ORG	&8000
	[ :LNOT: REFERENCE
	=	:STR:.
Snap	*	{PC}
Snap2	*	2*Snap + &10
	=	:STR:{PC}
	=	:STR:Snap
	=	:STR:Snap2
	|
	=	"00008000"
	=	"00008008"
	=	"00008008"
	=	"00010020"
	]

	END
