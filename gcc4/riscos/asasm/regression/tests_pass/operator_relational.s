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
	= :STR:(:CHR:235=5)
	= :STR:(:CHR:235=235)
	= :STR:("ABC"="DEF")
	= :STR:("ABC"="ABC")
	= :STR:("ABC"="ABCD")
	= :STR:("ABCD"="ABC")
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})
LblEq
	= :STR:(.=LblEq)
	= :STR:(.=LblEq)

	; ==
	[ EXTENSION
	= "|"
	= :STR:(3==5)
	= :STR:(5==3)
	= :STR:(-1==1)
	= :STR:(-1==&FFFFFFFF)
	= :STR:(:CHR:235==5)
	= :STR:(:CHR:235==235)
	= :STR:("ABC"=="DEF")
	= :STR:("ABC"=="ABC")
	= :STR:("ABC"=="ABCD")
	= :STR:("ABCD"=="ABC")
	= :STR:({FALSE}=={FALSE})
	= :STR:({FALSE}=={TRUE})
	= :STR:({TRUE}=={FALSE})
	= :STR:({TRUE}=={TRUE})
LblEqEq
	= :STR:(.=LblEqEq)
	= :STR:(.=LblEqEq)
	]

	; <>
	= "|"
	= :STR:(3<>5)
	= :STR:(5<>3)
	= :STR:(-1<>1)
	= :STR:(-1<>&FFFFFFFF)
	= :STR:(:CHR:235<>5)
	= :STR:(:CHR:235<>235)
	= :STR:("ABC"<>"DEF")
	= :STR:("ABC"<>"ABC")
	= :STR:("ABC"<>"ABCD")
	= :STR:("ABCD"<>"ABC")
	= :STR:({FALSE}<>{FALSE})
	= :STR:({FALSE}<>{TRUE})
	= :STR:({TRUE}<>{FALSE})
	= :STR:({TRUE}<>{TRUE})
LblNEq1
	= :STR:(.<>LblNEq1)
	= :STR:(.<>LblNEq1)

	; !=
	[ EXTENSION
	= "|"
	= :STR:(3!=5)
	= :STR:(5!=3)
	= :STR:(-1!=1)
	= :STR:(-1!=&FFFFFFFF)
	= :STR:(:CHR:235!=5)
	= :STR:(:CHR:235!=235)
	= :STR:("ABC"!="DEF")
	= :STR:("ABC"!="ABC")
	= :STR:("ABC"!="ABCD")
	= :STR:("ABCD"!="ABC")
	= :STR:({FALSE}!={FALSE})
	= :STR:({FALSE}!={TRUE})
	= :STR:({TRUE}!={FALSE})
	= :STR:({TRUE}!={TRUE})
LblNEq2
	= :STR:(.<>LblNEq2)
	= :STR:(.<>LblNEq2)
	]

	; /=
	= "|"
	= :STR:(3/=5)
	= :STR:(5/=3)
	= :STR:(-1/=1)
	= :STR:(-1/=&FFFFFFFF)
	= :STR:(:CHR:235/=5)
	= :STR:(:CHR:235/=235)
	= :STR:("ABC"/="DEF")
	= :STR:("ABC"/="ABC")
	= :STR:("ABC"/="ABCD")
	= :STR:("ABCD"/="ABC")
	= :STR:({FALSE}/={FALSE})
	= :STR:({FALSE}/={TRUE})
	= :STR:({TRUE}/={FALSE})
	= :STR:({TRUE}/={TRUE})
LblNEq3
	= :STR:(.<>LblNEq3)
	= :STR:(.<>LblNEq3)

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
	= :STR:(:CHR:235>5)
	= :STR:(:CHR:235>235)
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
	= :STR:(:CHR:235>=5)
	= :STR:(:CHR:235>=235)
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
	= :STR:(:CHR:235<5)
	= :STR:(:CHR:235<235)
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
	= :STR:(:CHR:235<=5)
	= :STR:(:CHR:235<=235)
	= :STR:("ABC"<="DEF")
	= :STR:("ABC"<="ABC")
	= :STR:("ABC"<="ABCD")
	= :STR:("ABCD"<="ABC")

	|

	= "|FFFTFTFTFFTFFTTF"	; =
	[ EXTENSION
	= "|FFFTFTFTFFTFFTTF"	; ==
	]
	= "|TTTFTFTFTTFTTFFT"	; <>
	[ EXTENSION
	= "|TTTFTFTFTTFTTFFT"	; !=
	]
	= "|TTTFTFTFTTFTTFFT"	; /=
	= "|FTTF"		; :LEOR:
	= "|FTTFTFFFFT"		; >
	= "|FTTTTTFTFT"		; >=
	= "|TFFFFFTFTF"		; <
	= "|TFFTFTTTTF"		; <=

	]

	AREA	Data2, DATA, READONLY
	[ :LNOT: REFERENCE
Lbl2	; Needs to be at start of area.
	= :STR:(.=Lbl2)
	|
	= "T"
	]

	AREA	Data3, DATA, READONLY
	[ :LNOT: REFERENCE
Lbl3	; Needs to be at start of area.
	= :STR:(.<>Lbl3)
	|
	= "F"
	]

	END
