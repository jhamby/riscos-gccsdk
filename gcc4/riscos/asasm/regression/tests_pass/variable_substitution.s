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
	END
