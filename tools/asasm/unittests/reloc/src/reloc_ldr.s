; Basic relocation test for LDR.

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
	LDR r1, CodeBefore
	LDR r3, CBlbl1

	; Other absolute area (before) - case 1:
	LDR r1, CodeABS1Before
	LDR r3, CBA1lbl1

	; Other absolute area (before) - case 2:
	LDR r1, CodeABS2Before
	LDR r3, CBA2lbl1

	; Other absolute area (before) - case 3:
	;LDR r1, CodeABS3Before
	;LDR r3, CBA3lbl1

	; This area (before):
	LDR r1, Code$sfx
	LDR r3, Clbl1$sfx

	; Define + use in same line:
Clbl3$sfx	LDR r5, Clbl3$sfx

	; This area (after):
	LDR r1, Clbl5$sfx

	; Other relocatable area (after):
	LDR r1, CodeAfter
	LDR r3, CAlbl1

	; Other absolute area (after) - case 1:
	LDR r1, CodeABS1After
	LDR r3, CAA1lbl1

	; Other absolute area (after) - case 2:
	LDR r1, CodeABS2After
	LDR r3, CAA2lbl1

	; Other absolute area (after) - case 3:
	LDR r1, CodeABS3After
	LDR r3, CAA3lbl1

	; First used, then defined.
Clbl5$sfx	%	40
Clbl6$sfx	%	8
	; --------------------------------

	; --------------------------------
	IMPORT ImpLblB1
	LDR r1, ImpLblB1
	LDR r3, ImpLblA1
	IMPORT ImpLblA1
	LDR r5, ImpLblNotDeclared

	IMPORT ImpLblB2 [BASED r10]
	LDR r1, ImpLblB2
	LDR r3, ImpLblA2
	IMPORT ImpLblA2 [BASED r10]

	IMPORT ImpLblB3 [COMMON=40]
	LDR r5, ImpLblB3
	LDR r7, ImpLblA3
	IMPORT ImpLblA3 [COMMON=0x10]
	; --------------------------------

	; --------------------------------
	; Map (before):
	LDR r1, MBlbl1
	;LDR r3, MBlbl2

	; Map (after):
	LDR r1, MAlbl1
	;LDR r3, MAlbl2
	; --------------------------------

	; --------------------------------
	; Based area (before):
	LDR r1, DataBefore
	LDR r3, DBlbl1
	;LDR r5, DBlbl2

	; Based area (after):
	LDR r1, DataAfter
	LDR r3, DAlbl1
	;LDR r5, DAlbl2
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
