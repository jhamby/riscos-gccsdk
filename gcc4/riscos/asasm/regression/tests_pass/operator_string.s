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
	[ {TRUE}
	= ">", :STR:(1./3.), "<"
	]

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
	[ {TRUE}
	= ">0.333333<"
	]

	]

	END
