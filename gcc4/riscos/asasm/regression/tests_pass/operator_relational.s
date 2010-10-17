	AREA Data, DATA, READONLY

	[ :LNOT:REFERENCE

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

	[ {TRUE}	; Extension
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

	[ {TRUE}	; Extension
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

	= "|"
	= :STR:(3>5)
	= :STR:(5>3)
	= :STR:(-1>1)
	= :STR:(-1>&FFFFFFFF)
	= :STR:("ABC">"DEF")
	= :STR:("ABC">"ABC")
	= :STR:("ABC">"ABCD")
	= :STR:("ABCD">"ABC")

	= "|"
	= :STR:(3>=5)
	= :STR:(5>=3)
	= :STR:(-1>=1)
	= :STR:(-1>=&FFFFFFFF)
	= :STR:("ABC">="DEF")
	= :STR:("ABC">="ABC")
	= :STR:("ABC">="ABCD")
	= :STR:("ABCD">="ABC")

	= "|"
	= :STR:(3<5)
	= :STR:(5<3)
	= :STR:(-1<1)
	= :STR:(-1<&FFFFFFFF)
	= :STR:("ABC"<"DEF")
	= :STR:("ABC"<"ABC")
	= :STR:("ABC"<"ABCD")
	= :STR:("ABCD"<"ABC")

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
	[ {TRUE}	; Extension
	= "|FFFTFTFFTFFT"	; ==
	]
	= "|TTTFTFTTTFFT"	; <>
	[ {TRUE}	; Extension
	= "|TTTFTFTTTFFT"	; !=
	]
	= "|TTTFTFTTTFFT"	; /=
	= "|FTTFFFFT"		; >
	= "|FTTTFTFT"		; >=
	= "|TFFFTFTF"		; <
	= "|TFFTTTTF"		; <=

	]

	END
