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

	[ :LNOT: REFERENCE

	MACRO
	Test2	$lbl
	DCB	"$lbl", 10
	MEND

	Test2	Begin

	Test2	$doh		; Give warning.
	Test2	x$doh		; Give warning.

	Test2	"$doh"		; Give warning.
	Test2	"x$doh"		; Give warning.

	GBLS	foo
foo	SETS	"bar"
	Test2	$foo		; foo -> bar
	Test2	x$foo		; foo -> bar

	Test2	"$foo"		; $foo -> bar
	Test2	"x$foo"		; x$foo -> bar

	Test2	|$foo|		; No substitution
	Test2	|x$foo|		; No substitution

	Test2	"$$foo"		; $$foo -> bar (!)
	Test2	"x$$foo"	; x$$foo -> xbar (!)

	Test2	End

	|

	=	"Begin", 10

	=	0x24, "doh", 10
	=	"x", 0x24, "doh", 10

	=	0x24, "doh", 10
	=	"x", 0x24, "doh", 10

	=	"bar", 10
	=	"xbar", 10

	=	"bar", 10
	=	"xbar", 10

	=	"|", 0x24, "foo|", 10
	=	"|x", 0x24, "foo|", 10

	=	"bar", 10
	=	"xbar", 10

	=	"End", 10

	]

	; Don't go crazy on empty variable values.
	[ :LNOT: REFERENCE
	GBLS	Green
Green	SETS	""
	=	"x"
	$Green
	=	"y"
	|
	=	"xy"
	]

	; Do only substituion for GBLL, GBLS and GBLA.
	AREA	Data, DATA
	ORG	&8000
	[ :LNOT: REFERENCE
Var1	RN	0
Var2	*	42
	GBLL	Var3
Var3	SETL	{TRUE}
	GBLA	Var4
Var4	SETA	3322
	GBLS	Var5
Var5	SETS	"foo"

	DCB	"1: $Var1\n"
	DCB	"2: $Var2\n"
	DCB	"3: $Var3\n"
	DCB	"4: $Var4\n"
	DCB	"5: $Var5\n"
	DCB	"6: $Data\n"
Label
	DCB	"7: $Label\n"
	|
	DCB	"1: $Var1\n"
	DCB	"2: $Var2\n"
	DCB	"3: T\n"
	DCB	"4: 00000CFA\n"
	DCB	"5: foo\n"
	DCB	"6: $Data\n"
	DCB	"7: $Label\n"
	]

	; Check vertical bar don't prevent variable substitution.
	AREA	Test10, DATA
	[ :LNOT: REFERENCE
	GBLA	Test10Var1
Test10Var1	SETA	1
|Test10Lbl1|	%	&$Test10Var1*4
	|
	DCD	0
	]

	END
