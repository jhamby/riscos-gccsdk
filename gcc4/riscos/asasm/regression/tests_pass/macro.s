; Test basic macro support when using IF/WHILE

	AREA	Data, DATA
	
	[ :LNOT: REFERENCE
		MACRO
		Test	$stop1, $stop2
		LCLA	loopOut
		WHILE	{TRUE}
			IF	loopOut >= $stop1
				MEXIT
			ENDIF
			LCLA	loopIn
			WHILE	loopIn	< $stop2
				DCB	65 + loopIn
loopIn				SETA	1 + loopIn
			WEND
loopOut			SETA	loopOut + 1
		WEND
		MEND
		
		= "Test1\n  "
		Test	2, 3
		= "\nTest2\n  "
		Test	3, 2
		= "\nTest3\n  "
		Test	1, 0
		= "\nTest4\n  "
		Test	0, 1
		= "\n"
	|
		= "Test1\n"
		= "  ABCABC\n"
		= "Test2\n"
		= "  ABABAB\n"
		= "Test3\n"
		= "  \n"
		= "Test4\n"
		= "  \n"
	]

; Tests if the presence of a 'comment' doesn't confuse the macro implementation
; (including the local variable handling)

	[ :LNOT: REFERENCE
	MACRO				; comment
$label	TWOTIMES	$val		; comment
	LCLA		tt		; comment
tt	SETA		$val<<1		; comment
$label	&		tt		; comment
	MEND				; comment

twenty	TWOTIMES	10		; comment
	|
twenty	&	20
	]

	END
