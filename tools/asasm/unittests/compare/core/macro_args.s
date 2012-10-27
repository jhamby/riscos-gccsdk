; Test the macro argument support.

	AREA	Code, CODE, READONLY

	; Test basic support using a well known macro:
	[ :LNOT: REFERENCE
	MACRO
$Lab	DivMod $Div,$Top,$Bot,$Temp
	ASSERT $Top <> $Bot
	ASSERT $Top <> $Temp
	ASSERT $Bot <> $Temp
	IF	"$Div" <> ""
		ASSERT $Div <> $Top
		ASSERT $Div <> $Bot
		ASSERT $Div <> $Temp
	ENDIF
$Lab
	MOV	$Temp, $Bot
	CMP	$Temp, $Top, LSR #1
90	MOVLS	$Temp, $Temp, LSL #1
	CMP	$Temp, $Top, LSR #1
	BLS	%b90
	IF	"$Div" <> ""
		MOV	$Div, #0
	ENDIF
91	CMP	$Top, $Temp
	SUBCS	$Top, $Top, $Temp
	IF	"$Div" <> ""
		ADC	$Div, $Div, $Div
	ENDIF
	MOV	$Temp, $Temp, LSR #1
	CMP	$Temp, $Bot
	BHS	%b91
	MEND

ratio	DivMod	r0,r5,r4,r2	; Invoke, DivMod, macro (comma's are part of the test)

	|

ratio	MOV	r2, r4
	CMP	r2, r5, LSR #1
90	MOVLS	r2, r2, LSL #1
	CMP	r2, r5, LSR #1
	BLS	%b90
	MOV	r0, #0
91	CMP	r5, r2
	SUBCS	r5, r5, r2
	ADC	r0, r0, r0
	MOV	r2, r2, LSR #1
	CMP	r2, r4
	BHS	%b91

	]

	; Test '.' support in macro argument expansion.
	[ :LNOT: REFERENCE
	MACRO
$Lbl	Test1
$Lbl	DCD	1
	B	Tst1
	MEND

Tst1	Test1

	MACRO
$Lbl	Test2
$Lbl.X	DCD	2
	B	Tst2X
	MEND

Tst2	Test2

	|

	DCD	1
	B	{PC} - 4
	DCD	2
	B	{PC} - 4

	]

	; Test '|' support for macro arguments.
	[ :LNOT: REFERENCE
        MACRO
$label  Test3	$reg, $what, $iswhat, $reg2
        AND	$reg2, $reg, #Test3_$|what|_Mask
        TEQ	$reg2, #Test3_$|what|_$iswhat
        MEND

Test3_Foo_Mask	*	3 :SHL: 0
Test3_Foo_Val0	*	0 :SHL: 0
Test3_Foo_Val1	*	1 :SHL: 0
Test3_Foo_Val2	*	2 :SHL: 0

	Test3 r0, Foo, Val1, r14
	|
	AND	r14, r0, #3
	TEQ	r14, #1
	]

	; Test the macro default argument value:
	[ :LNOT: REFERENCE

	; Note there are two spaces after "def3" which make that the 3rd
	; argument default value is " def3  "
	MACRO
	Test4	$Arg1  , $Arg2="def2",$Arg3  = def3  
	$Arg1
	DCB	"$Arg2", 1
	DCB	"$Arg3", 2
	ALIGN
	MEND ; End of macro

	Test4
	Test4	,
	Test4	,,
	Test4	DCB "t1arg1",t1arg2  ,    t1arg3
	Test4	DCB "t2arg1","t2arg2" ,   "t2arg3"
	Test4	,|  ,   "t3arg3"
	Test4	,  "t4arg2"  ,   |

	|

	DCD	&0201
	DCD	&0201
	DCD	&0201
	DCB	"t1arg1", "t1arg2", 1, "t1arg3", 2
	ALIGN
	DCB	"t2arg1", "t2arg2", 1, "t2arg3", 2
	ALIGN
	DCB	"", "def2", 1, "t3arg3", 2
	ALIGN
	DCB	"", "t4arg2", 1, " def3  ", 2
	ALIGN

	]

	; Test macro label and suffix argument
	AREA	LblSfxData, DATA
	[ :LNOT: REFERENCE
	MACRO
	LSTest1$sfx
	= "LSTest1, sfx is '$sfx'\n"
	MEND

	MACRO
$lbl	LSTest2$sfx
$lbl	= "LSTest2, sfx is '$sfx', lbl is '$lbl'"
	[ "$lbl" <> ""
	= " (", :STR:($lbl - LblSfxData), ")"
	]
	= "\n"
	MEND

	MACRO
$lbl	LSTest3$sfx $arg1, $arg2
$lbl	= "LSTest3, sfx is '$sfx', lbl is '$lbl'"
	[ "$lbl" <> ""
	= " (", :STR:($lbl - LblSfxData), ")"
	]
	= ", arg1 is '$arg1', arg2 is '$arg2'\n"
	MEND

	LSTest1
	LSTest1SFX1
lbl1	LSTest1SFX2

	LSTest2
	LSTest2SFX1
lbl2	LSTest2SFX2

	LSTest3 ARG1, ARG2
	LSTest3SFX1 ARG1, ARG2
lbl3	LSTest3SFX2 ARG1, ARG2
	|
	= "LSTest1, sfx is ''\n"
	= "LSTest1, sfx is 'SFX1'\n"
	= "LSTest1, sfx is 'SFX2'\n"
	= "LSTest2, sfx is '', lbl is ''\n"
	= "LSTest2, sfx is 'SFX1', lbl is ''\n"
	= "LSTest2, sfx is 'SFX2', lbl is 'lbl2' (00000081)\n"
	= "LSTest3, sfx is '', lbl is '', arg1 is 'ARG1', arg2 is 'ARG2'\n"
	= "LSTest3, sfx is 'SFX1', lbl is '', arg1 is 'ARG1', arg2 is 'ARG2'\n"
	= "LSTest3, sfx is 'SFX2', lbl is 'lbl3' (00000132), arg1 is 'ARG1', arg2 is 'ARG2'\n"
	]

	END
