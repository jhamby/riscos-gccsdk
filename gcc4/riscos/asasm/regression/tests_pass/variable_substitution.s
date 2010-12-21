; Tests variable substitution.

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE
	
	MACRO
	SimpleTCall
	= "T"
	MEND
	
	MACRO
	SimpleFCall
	= "F"
	MEND

	GBLL	ATrue
ATrue	SETL	{TRUE}
	GBLL	AFalse
AFalse	SETL	{FALSE}

	Simple$ATrue.Call
	Simple$AFalse.Call
	= ">$ATrue.<"
	= "$ATrue"
	= ">$AFalse.<"
	= "$AFalse"
	ALIGN
	CMP	r0, #'$ATrue'

	GBLA	AnInt
AnInt	SETA	100
	= ">$AnInt.<"

	GBLS	AString
AString	SETS	"Str"
	= ">$AString.<"

	GBLS	AString2
AString2 SETS	"222"
	GBLS	AString3
AString3 SETS	"[$AString2.]"
	= ">$AString3.<"
	GBLS	AString4
AString4 SETS	"[" :CC: :CHR:&24 :CC: "AString2.]"
	= ">$AString4.<"

	|
	
	=	"T"
	=	"F"
	=	">T<"
	=	"T"
	=	">F<"
	=	"F"
	ALIGN
	CMP	r0, #'T'

	=	">00000064<"

	=	">Str<"

	=	">[222]<"
	=	">[$$AString2.]<"

	]


	[ :LNOT: REFERENCE
	GBLS	var1
var1	SETS	"vl1"

	GBLS	var2
var2	SETS	"arg3"

	GBLS	var3
var3	SETS	"$$arg3"

	MACRO
	Test	$arg1, $arg2, $arg3
	=	"tst1 $bla\n"
	=	"tst2 $$bla\n"
	=	"tst3 $$$bla\n"
	=	"tst4 $var1\n"
	=	"tst5 $$var1\n"
	=	"tst6 $$$var1\n"
	=	"tst7 $var2\n"
	=	"tst8 $$var2\n"
	=	"tst9 $$$var2\n"
	=	"tst10 $var3\n"
	=	"tst11 $$var3\n"
	=	"tst12 $$$var3\n"
	=	"tst13 $arg1\n"
	=	"tst14 $$arg1\n"
	=	"tst15 $$$arg1\n"
	=	"tst16 $arg2\n"
	=	"tst17 $$arg2\n"
	=	"tst18 $$$arg2\n"
	MEND

	Test	"mcr1", "var2", "mcr3"
	|
	=	"tst1 $bla\n"
	=	"tst2 $bla\n"
	=	"tst3 $bla\n"
	=	"tst4 vl1\n"
	=	"tst5 vl1\n"
	=	"tst6 $var1\n"
	=	"tst7 arg3\n"
	=	"tst8 arg3\n"
	=	"tst9 $var2\n"
	=	"tst10 $arg3\n"
	=	"tst11 $arg3\n"
	=	"tst12 $var3\n"
	=	"tst13 mcr1\n"
	=	"tst14 $arg1\n"
	=	"tst15 $mcr1\n"
	=	"tst16 var2\n"
	=	"tst17 $arg2\n"
	=	"tst18 arg3\n"
	]

	END
