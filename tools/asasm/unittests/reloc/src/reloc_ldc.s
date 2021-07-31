; Basic relocation test for LDC.

	AREA	CodeBefore, CODE, READONLY
	SWI 0
	SWI 1
CBlbl1  SWI 2

	AREA	CodeABS1Before, CODE, READONLY
	ORG &20
	SWI 0
	SWI 1
CBA1lbl1  SWI 2

	AREA	CodeABS2Before, CODE, READONLY
	ORG &FF20
	SWI 0
	SWI 1
CBA2lbl1  SWI 2

	AREA	CodeABS3Before, CODE, READONLY
	ORG &FFFF20
	SWI 0
	SWI 1
CBA3lbl1  SWI 2

	AREA DataBefore, DATA, BASED r10

	DCD 1
	DCD 2
DBlbl1	DCD 3
	% 0xFF30
DBlbl2	DCD 4

	MACRO
	TestCase$sfx
	AREA Code$sfx, CODE, READONLY
	[ "$sfx" = "ABS"
	ORG 0xFF80
	]

	; --------------------------------
	; First defined, then used.
Clbl1$sfx	%	40
Clbl2$sfx	%	8

	; Other relocatable area (before):
	LDC p6, c1, CodeBefore
	LDC p6, c3, CBlbl1

	; Other absolute area (before) - case 1:
	LDC p6, c1, CodeABS1Before
	LDC p6, c3, CBA1lbl1

	; Other absolute area (before) - case 2:
	LDC p6, c1, CodeABS2Before
	LDC p6, c3, CBA2lbl1

	; Other absolute area (before) - case 3:
	;LDC p6, c1, CodeABS3Before
	;LDC p6, c3, CBA3lbl1

	; This area (before):
	LDC p6, c1, Code$sfx
	LDC p6, c3, Clbl1$sfx

	; Define + use in same line:
Clbl3$sfx	LDC p6, c5, Clbl3$sfx

	; This area (after):
	LDC p6, c1, Clbl5$sfx

	; Other relocatable area (after):
	LDC p6, c1, CodeAfter
	LDC p6, c3, CAlbl1

	; Other absolute area (after) - case 1:
	LDC p6, c1, CodeABS1After
	LDC p6, c3, CAA1lbl1

	; Other absolute area (after) - case 2:
	LDC p6, c1, CodeABS2After
	LDC p6, c3, CAA2lbl1

	; Other absolute area (after) - case 3:
	LDC p6, c1, CodeABS3After
	LDC p6, c3, CAA3lbl1

	; First used, then defined.
Clbl5$sfx	%	40
Clbl6$sfx	%	8
	; --------------------------------

	; --------------------------------
	IMPORT ImpLblB1
	LDC p6, c1, ImpLblB1
	LDC p6, c3, ImpLblA1
	IMPORT ImpLblA1
	LDC p6, c5, ImpLblNotDeclared

	IMPORT ImpLblB2 [BASED r10]
	LDC p6, c1, ImpLblB2
	LDC p6, c3, ImpLblA2
	IMPORT ImpLblA2 [BASED r10]

	IMPORT ImpLblB3 [COMMON=40]
	LDC p6, c5, ImpLblB3
	LDC p6, c7, ImpLblA3
	IMPORT ImpLblA3 [COMMON=0x10]
	; --------------------------------

	; --------------------------------
	; Map (before):
	LDC p6, c1, MBlbl1
	;LDC p6, c3, MBlbl2

	; Map (after):
	LDC p6, c1, MAlbl1
	;LDC p6, c3, MAlbl2
	; --------------------------------

	; --------------------------------
	; Based area (before):
	LDC p6, c1, DataBefore
	LDC p6, c3, DBlbl1
	;LDC p6, c5, DBlbl2

	; Based area (after):
	LDC p6, c1, DataAfter
	LDC p6, c3, DAlbl1
	;LDC p6, c5, DAlbl2
	; --------------------------------
	MEND

	MAP 4, r12
	# 16
	# 4
MBlbl1  # 0xFF30
MBlbl2	# 4

	TestCaseNonABS
	TestCaseABS

	MAP 8, r10
	# 4
	# 8
MAlbl1	# 0xFF30
MAlbl2	# 4

	AREA CodeAfter, CODE

	SWI 3
	SWI 4
CAlbl1	SWI 5

	AREA	CodeABS1After, CODE, READONLY
	ORG &20
	SWI 0
	SWI 1
CAA1lbl1  SWI 2

	AREA	CodeABS2After, CODE, READONLY
	ORG &FF20
	SWI 0
	SWI 1
CAA2lbl1  SWI 2

	AREA	CodeABS3After, CODE, READONLY
	ORG &FFFF20
	SWI 0
	SWI 1
CAA3lbl1  SWI 2

	AREA DataAfter, DATA, BASED r10

	DCD 4
	DCD 5
DAlbl1	DCD 6
	% 0xFF30
DAlbl2	DCD 4

	END
