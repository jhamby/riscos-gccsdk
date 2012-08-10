; Tests local & global variables for regressions.

		AREA Data1, DATA, READONLY

	[ :LNOT: REFERENCE
	; *** TESTCASE:

; Macro / local variables:

		; Simple invocation of a macro.
TestMacro0
		MACRO
		MTestMacro
		LCLA	TestMacro0Var
TestMacro0Var	SETA	42
		DCD	TestMacro0Var
		MEND
		MTestMacro

TestMacro1
		MACRO
		MTestMacro1
		; Default value should be 0.
		LCLA	TestMacro1Var
		=	"TestMacro1a: "
		[ TestMacro1Var = 0
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Setting a local variable should update its value:
		=	"TestMacro1b: "
TestMacro1Var	SETA	11
		[ TestMacro1Var = 11
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Call another macro which also defines & uses the same
		; TestMacro1Var local variable.
		MTestMacro2
		; We expect that our local variable still has its same
		; value:
		=	"TestMacro1c: "
		[ TestMacro1Var = 11
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		MEND

		MACRO
		MTestMacro2
		; Default value should be 0.
		LCLA	TestMacro1Var
		=	"TestMacro2a: "
		[ TestMacro1Var = 0
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Setting variable should update its value:
		=	"TestMacro2b: "
TestMacro1Var	SETA	22
		[ TestMacro1Var = 22
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		MEND

		; Invoke test:
		MTestMacro1

; Arithmetic --------------------------------------------------------

TestArith1
		; Default value should be 0.
		=	"TestArith1: "
		GBLA	TestArith1GlobalVar
		[ :DEF:TestArith1GlobalVar :LAND: TestArith1GlobalVar = 0
		=	"ok"
		|
		=	"not ok"
		]
		=	"\n"

TestArith2
		; Set non default value:
		=	"TestArith2a: "
		GBLA	TestArith2GlobalVar
TestArith2GlobalVar	SETA	40 + 2
		[ :DEF:TestArith2GlobalVar :LAND: TestArith2GlobalVar = 44 - 2
		=	"ok"
		|
		=	"not ok"
		]
		=	"\n"
		; Redefine same global variable should reset it:
		=	"TestArith2b: "
		GBLA	TestArith2GlobalVar
		[ :DEF:TestArith2GlobalVar :LAND: TestArith2GlobalVar = 0
		=	"ok"
		|
		=	"not ok"
		]
		=	"\n"

TestArith3
		; Test local variable and its interaction with
		; global variables:
		GBLA	TestArith3GlobalVar1
TestArith3GlobalVar1	SETA	3

		MACRO
		MTestArith1
		; Test default value of local variable:
		=	"MTestArith1a: "
		LCLA	TestArith3LocalVar
		[ TestArith3LocalVar = 0
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Note that :DEF: only tests {TRUE} for *global* variables:
		=	"MTestArith1b: "
		[ :LNOT: :DEF:TestArith3LocalVar
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		;Test setting value local variable:
		=	"MTestArith1c: "
TestArith3LocalVar SETA 22 + 20
		[ TestArith3LocalVar = 42
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Another test to see if :DEF: doesn't return {TRUE}
		; after having assigned a non default value to local
		; variable:
		=	"MTestArith1d: "
		[ :LNOT: :DEF:TestArith3LocalVar
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Test to see if we can access global variable:
		=	"MTestArith1e: "
		[ :DEF:TestArith3GlobalVar1 :LAND: TestArith3GlobalVar1 = 3
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
TestArith3GlobalVar1	SETA	TestArith3GlobalVar1 + 1
		; Define new global variable:
		GBLA	TestArith3GlobalVar1b
TestArith3GlobalVar1b	SETA	33
		MEND

		MTestArith1
		; Test if the global variable update in our macro worked:
		=	"TestArith3GlobalVar1a: "
		[ :DEF:TestArith3GlobalVar1 :LAND: TestArith3GlobalVar1 = 4
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; Test the global variable we've made in our macro:
		=	"TestArith3GlobalVar1b: "
		[ :DEF:TestArith3GlobalVar1b :LAND: TestArith3GlobalVar1b = 33
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"

		; Define global variable which is going to be used
		; in MTestArith2 macro as local variable.
		GBLA	TestArith3Var2
TestArith3Var2	SETA	333

		MACRO
		MTestArith2
		; We're going to define TestArith3Var2 as a local
		; variable.
		; Test default value of local variable.  Defining a variable
		; will always reset its value.
		=	"MTestArith2a: "
		LCLA	TestArith3Var2
		[ TestArith3Var2 = 0
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; :DEF: only tests {TRUE} for *global* variables but as
		; TestArith3Var2 is also a global one, :DEF: will return
		; {TRUE}.
		=	"MTestArith2b: "
		[ :DEF:TestArith3Var2
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		;Test setting value local variable:
		=	"MTestArith2c: "
TestArith3Var2	SETA 22 + 20
		[ TestArith3Var2 = 42
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		; TestArith3Var2 remains defined as global variable
		; so :DEF: should still return {TRUE}.
		=	"MTestArith2d: "
		[ :DEF:TestArith3Var2
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"
		MEND

		MTestArith2
		; We have TestArith3Var2 as global variable and just have
		; had it as local variable too which value we've change.
		; The global variable should still be the same:
		=	"TestArith3GlobalVar1c: "
		[ :DEF:TestArith3Var2 :LAND: TestArith3Var2 = 333
		=	"ok"
		|
		=	"nok"
		]
		=	"\n"

; Logical --------------------------------------------------------

TestLog1
		; Default value should be {FALSE}.
		=	"TestLog1: "
		GBLL	TestLogVar1
		[ :DEF:TestLogVar1 :LAND: :LNOT: TestLogVar1
		=	"ok"
		|
		=	"not ok"
		]
		=	"\n"

; String --------------------------------------------------------

TestStr1
		; Default value should be "".
		=	"TestStr1: "
		GBLS	TestStrVar1
		[ :DEF:TestStrVar1 :LAND: TestStrVar1 = ""
		=	"ok"
		|
		=	"not ok"
		]
		=	"\n"

	|

	; *** REFERENCE:
TestMacro0
		DCD 42
TestMacro1
		=	"TestMacro1a: ok\n"
		=	"TestMacro1b: ok\n"
		=	"TestMacro2a: ok\n"
		=	"TestMacro2b: ok\n"
		=	"TestMacro1c: ok\n"
TestArith1
		=	"TestArith1: ok\n"
TestArith2
		=	"TestArith2a: ok\n"
		=	"TestArith2b: ok\n"
TestArith3
		=	"MTestArith1a: ok\n"
		=	"MTestArith1b: ok\n"
		=	"MTestArith1c: ok\n"
		=	"MTestArith1d: ok\n"
		=	"MTestArith1e: ok\n"
		=	"TestArith3GlobalVar1a: ok\n"
		=	"TestArith3GlobalVar1b: ok\n"
		=	"MTestArith2a: ok\n"
		=	"MTestArith2b: ok\n"
		=	"MTestArith2c: ok\n"
		=	"MTestArith2d: ok\n"
		=	"TestArith3GlobalVar1c: ok\n"
TestLog1
		=	"TestLog1: ok\n"
TestStr1
		=	"TestStr1: ok\n"

	]

; A local variable can have a different type than a global one.
	AREA	Data2, DATA
	[ :LNOT: REFERENCE
AVar	RN	9
	GBLS	AVarStr
AVarStr	SETS	"Doh"

	MACRO
	TestMacro
	LCLL	AVar
AVar	SETL	{TRUE}
	= "\nTestMacro: AVar: " :CC: :STR: :DEF: AVar

	LCLL	AVarStr
AVarStr	SETL	{TRUE}
	= "\nTestMacro: AVarStr: " :CC: :STR: :DEF: AVarStr
	MEND

	TestMacro

	= "\nTestMain: AVar: " :CC: :STR: :DEF: AVar
	= "\nTestMain: AVarStr: " :CC: :STR: :DEF: AVarStr
	|
	= "\nTestMacro: AVar: T"
	= "\nTestMacro: AVarStr: T"
	= "\nTestMain: AVar: T"
	= "\nTestMain: AVarStr: T"
	]

; Test that registers and coprocessor numbers can be assigned to
; arithmetic variables
	AREA	Data3, DATA
	[ :LNOT:REFERENCE
		GBLA	VarRegR3
VarRegR3	SETA	r3
		DCD	VarRegR3

		GBLA	VarRegF4
VarRegF4	SETA	f4
		DCD	VarRegF4

		GBLA	VarRegP5
VarRegP5	SETA	p5
		DCD	VarRegP5

		GBLA	VarRegC6
VarRegC6	SETA	c6
		DCD	VarRegC6

		; FIXME: add tests for q, d and s VFP registers
;		GBLA	VarRegQ7
;VarRegQ7	SETA	q7
;		DCD	VarRegQ7

;		GBLA	VarRegD8
;VarRegD8	SETA	d8
;		DCD	VarRegD8

;		GBLA	VarRegS9
;VarRegS9	SETA	s9
;		DCD	VarRegS9
	|
		DCD	3
		DCD	4
		DCD	5
		DCD	6
	]

; Test that all variables are reset and become undefined at start of pass 2.
	AREA	Data4, DATA
	[ :LNOT: REFERENCE

	; *** String
	MACRO
	StrFirst
	=	"StrFirst\n"
	MEND

	MACRO
	StrSecond
	=	"StrSecond\n"
	MEND

	[ :LNOT: :DEF: IterS
	GBLS	IterS
IterS	SETS	"StrFirst"
	]

	$IterS

IterS	SETS	"StrSecond"

	$IterS

	; *** Boolean
	MACRO
	BoolT
	=	"BoolFirst\n"
	MEND

	MACRO
	BoolF
	=	"BoolSecond\n"
	MEND

	[ :LNOT: :DEF: IterL
	GBLL	IterL
IterL	SETL	{TRUE}
	]

	GBLS	ResL
ResL	SETS	"Bool" :CC: :STR: IterL
	$ResL

IterL	SETL	{FALSE}

ResL	SETS	"Bool" :CC: :STR: IterL
	$ResL

	; *** Integer
	MACRO
	Int00000011
	=	"IntFirst\n"
	MEND

	MACRO
	Int00000022
	=	"IntSecond\n"
	MEND

	[ :LNOT: :DEF: IterA
	GBLA	IterA
IterA	SETA	&11
	]

	GBLS	ResA
ResA	SETS	"Int" :CC: :STR: IterA
	$ResA

IterA	SETA	&22

ResA	SETS	"Int" :CC: :STR: IterA
	$ResA

	|

	= "StrFirst\n"
	= "StrSecond\n"
	= "BoolFirst\n"
	= "BoolSecond\n"
	= "IntFirst\n"
	= "IntSecond\n"

	]

	END
