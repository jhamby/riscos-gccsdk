	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	[ :LNOT:REFERENCE

	; =
	= "|"
	= :STR:(3=5)
	= :STR:(5=3)
	= :STR:(-1=1)
	= :STR:(-1=&FFFFFFFF)
	= :STR:("ABC"="DEF")
	= :STR:("ABC"="ABC")
	= :STR:("ABC"="ABCD")
	= :STR:("ABCD"="ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})

	; ==
	[ EXTENSION
	= "|"
	= :STR:(3==5)
	= :STR:(5==3)
	= :STR:(-1==1)
	= :STR:(-1==&FFFFFFFF)
	= :STR:("ABC"=="DEF")
	= :STR:("ABC"=="ABC")
	= :STR:("ABC"=="ABCD")
	= :STR:("ABCD"=="ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})
	]

	; <>
	= "|"
	= :STR:(3<>5)
	= :STR:(5<>3)
	= :STR:(-1<>1)
	= :STR:(-1<>&FFFFFFFF)
	= :STR:("ABC"<>"DEF")
	= :STR:("ABC"<>"ABC")
	= :STR:("ABC"<>"ABCD")
	= :STR:("ABCD"<>"ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})

	; !=
	[ EXTENSION
	= "|"
	= :STR:(3!=5)
	= :STR:(5!=3)
	= :STR:(-1!=1)
	= :STR:(-1!=&FFFFFFFF)
	= :STR:("ABC"!="DEF")
	= :STR:("ABC"!="ABC")
	= :STR:("ABC"!="ABCD")
	= :STR:("ABCD"!="ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})
	]

	; /=
	= "|"
	= :STR:(3/=5)
	= :STR:(5/=3)
	= :STR:(-1/=1)
	= :STR:(-1/=&FFFFFFFF)
	= :STR:("ABC"/="DEF")
	= :STR:("ABC"/="ABC")
	= :STR:("ABC"/="ABCD")
	= :STR:("ABCD"/="ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})

	; :LEOR:
	= "|"
	= :STR:({FALSE} :LEOR: {FALSE})
	= :STR:({FALSE}:LEOR:{TRUE})
	= :STR:({TRUE}:LEOR:{FALSE})
	= :STR:({TRUE}:LEOR:{TRUE})

	; >
	= "|"
	= :STR:(3>5)
	= :STR:(5>3)
	= :STR:(-1>1)
	= :STR:(-1>&FFFFFFFF)
	= :STR:("ABC">"DEF")
	= :STR:("ABC">"ABC")
	= :STR:("ABC">"ABCD")
	= :STR:("ABCD">"ABC")

	; >=
	= "|"
	= :STR:(3>=5)
	= :STR:(5>=3)
	= :STR:(-1>=1)
	= :STR:(-1>=&FFFFFFFF)
	= :STR:("ABC">="DEF")
	= :STR:("ABC">="ABC")
	= :STR:("ABC">="ABCD")
	= :STR:("ABCD">="ABC")

	; <
	= "|"
	= :STR:(3<5)
	= :STR:(5<3)
	= :STR:(-1<1)
	= :STR:(-1<&FFFFFFFF)
	= :STR:("ABC"<"DEF")
	= :STR:("ABC"<"ABC")
	= :STR:("ABC"<"ABCD")
	= :STR:("ABCD"<"ABC")

	; <=
	= "|"
	= :STR:(3<=5)
	= :STR:(5<=3)
	= :STR:(-1<=1)
	= :STR:(-1<=&FFFFFFFF)
	= :STR:("ABC"<="DEF")
	= :STR:("ABC"<="ABC")
	= :STR:("ABC"<="ABCD")
	= :STR:("ABCD"<="ABC")

	|

	= "|FFFTFTFFTFFT"	; =
	[ EXTENSION
	= "|FFFTFTFFTFFT"	; ==
	]
	= "|TTTFTFTTTFFT"	; <>
	[ EXTENSION
	= "|TTTFTFTTTFFT"	; !=
	]
	= "|TTTFTFTTTFFT"	; /=
	= "|FTTF"		; :LEOR:
	= "|FTTFFFFT"		; >
	= "|FTTTFTFT"		; >=
	= "|TFFFTFTF"		; <
	= "|TFFTTTTF"		; <=

	]

	END
