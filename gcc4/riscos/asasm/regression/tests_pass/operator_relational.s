; Tests =, ==, !=, <>, /= for integers, strings, boolean, addr and registers.
; Tests >, >=, <, <= for integer and string.

	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	[ :LNOT:REFERENCE

FooR3	RN	r3

	; =
	= "Test =\n"
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
	= :STR:(98="a")
	= :STR:(98="b")
	= :STR:("a"=98)
	= :STR:("b"=98)
	= :STR:({FALSE}={FALSE})
	= :STR:({FALSE}={TRUE})
	= :STR:({TRUE}={FALSE})
	= :STR:({TRUE}={TRUE})
LblEq
	= :STR:(.=LblEq)
	= :STR:(.=LblEq)
	= :STR:(FooR3=r3)
	= :STR:(FooR3=c0)
	= "\n"

	; ==
	[ EXTENSION
	= "Test ==\n"
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
	= :STR:(98=="a")
	= :STR:(98=="b")
	= :STR:("a"==98)
	= :STR:("b"==98)
	= :STR:({FALSE}=={FALSE})
	= :STR:({FALSE}=={TRUE})
	= :STR:({TRUE}=={FALSE})
	= :STR:({TRUE}=={TRUE})
LblEqEq
	= :STR:(.==LblEqEq)
	= :STR:(.==LblEqEq)
	= :STR:(FooR3==r3)
	= :STR:(FooR3==c0)
	= "\n"
	]

	; <>
	= "Test <>\n"
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
	= :STR:(98<>"a")
	= :STR:(98<>"b")
	= :STR:("a"<>98)
	= :STR:("b"<>98)
	= :STR:({FALSE}<>{FALSE})
	= :STR:({FALSE}<>{TRUE})
	= :STR:({TRUE}<>{FALSE})
	= :STR:({TRUE}<>{TRUE})
LblNEq1
	= :STR:(.<>LblNEq1)
	= :STR:(.<>LblNEq1)
	= :STR:(FooR3<>r3)
	= :STR:(FooR3<>c0)
	= "\n"

	; !=
	[ EXTENSION
	= "Test !=\n"
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
	= :STR:(98!="a")
	= :STR:(98!="b")
	= :STR:("a"!=98)
	= :STR:("b"!=98)
	= :STR:({FALSE}!={FALSE})
	= :STR:({FALSE}!={TRUE})
	= :STR:({TRUE}!={FALSE})
	= :STR:({TRUE}!={TRUE})
LblNEq2
	= :STR:(.!=LblNEq2)
	= :STR:(.!=LblNEq2)
	= :STR:(FooR3!=r3)
	= :STR:(FooR3!=c0)
	= "\n"
	]

	; /=
	= "Test /=\n"
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
	= :STR:(98/="a")
	= :STR:(98/="b")
	= :STR:("a"/=98)
	= :STR:("b"/=98)
	= :STR:({FALSE}/={FALSE})
	= :STR:({FALSE}/={TRUE})
	= :STR:({TRUE}/={FALSE})
	= :STR:({TRUE}/={TRUE})
LblNEq3
	= :STR:(./=LblNEq3)
	= :STR:(./=LblNEq3)
	= :STR:(FooR3/=r3)
	= :STR:(FooR3/=c0)
	= "\n"

	; >
	= "Test >\n"
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
	= :STR:(98>"a")
	= :STR:(98>"b")
	= :STR:("a">98)
	= :STR:("b">98)
	= "\n"

	; >=
	= "Test >=\n"
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
	= :STR:(98>="a")
	= :STR:(98>="b")
	= :STR:("a">=98)
	= :STR:("b">=98)
	= "\n"

	; <
	= "Test <\n"
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
	= :STR:(98<"a")
	= :STR:(98<"b")
	= :STR:("a"<98)
	= :STR:("b"<98)
	= "\n"

	; <=
	= "Test <=\n"
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
	= :STR:(98<="a")
	= :STR:(98<="b")
	= :STR:("a"<=98)
	= :STR:("b"<=98)
	= "\n"

	|

	= "Test =\nFFFTFTFTFFFTFTTFFTTFTF\n"	; =
	[ EXTENSION
	= "Test ==\nFFFTFTFTFFFTFTTFFTTFTF\n"	; ==
	]
	= "Test <>\nTTTFTFTFTTTFTFFTTFFTFT\n"	; <>
	[ EXTENSION
	= "Test !=\nTTTFTFTFTTTFTFFTTFFTFT\n"	; !=
	]
	= "Test /=\nTTTFTFTFTTTFTFFTTFFTFT\n"	; /=
	= "Test >\nFTTFTFFFFTTFFF\n"		; >
	= "Test >=\nFTTTTTFTFTTTFT\n"		; >=
	= "Test <\nTFFFFFTFTFFFTF\n"		; <
	= "Test <=\nTFFTFTTTTFFTTT\n"		; <=

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
