; Basic relocation test for MOV32.
; RUNOPT: -CPU 6T2

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
	% 0x7F30
DBlbl2	DCD 4

	MACRO
	TestCase$sfx $type
	AREA Code$sfx.$type, CODE, READONLY
	[ "$sfx" = "ABS"
	ORG 0xFF40
	]

	; --------------------------------
	; First defined, then used.
Clbl1$sfx.$type	%	40
Clbl2$sfx.$type	%	8

	; Other relocatable area (before):
	MOV32 r1, CodeBefore
	MOV32 r2, CBlbl1

	; Other absolute area (before) - case 1:
	MOV32 r1, CodeABS1Before
	MOV32 r2, CBA1lbl1

	; Other absolute area (before) - case 2:
	MOV32 r1, CodeABS2Before
	MOV32 r2, CBA2lbl1

	; Other absolute area (before) - case 3:
	MOV32 r1, CodeABS3Before
	MOV32 r2, CBA3lbl1

	; This area (before):
	MOV32 r1, Code$sfx.$type
	MOV32 r2, Clbl1$sfx.$type
	MOV32 r3, Clbl2$sfx.$type

	; Define + use in same line:
Clbl3$sfx.$type	MOV32 r1, Clbl3$sfx.$type

	; This area (after):
	MOV32 r1, Clbl5$sfx.$type
	MOV32 r2, Clbl6$sfx.$type

	; Other relocatable area (after):
	MOV32 r1, CodeAfter
	MOV32 r2, CAlbl1

	; Other absolute area (after) - case 1:
	MOV32 r1, CodeABS1After
	MOV32 r2, CAA1lbl1

	; Other absolute area (after) - case 2:
	MOV32 r1, CodeABS2After
	MOV32 r2, CAA2lbl1

	; Other absolute area (after) - case 3:
	MOV32 r1, CodeABS3After
	MOV32 r2, CAA3lbl1

	; First used, then defined.
Clbl5$sfx.$type	%	40
Clbl6$sfx.$type	%	8
	; --------------------------------

	; --------------------------------
	IMPORT ImpLblB1
	MOV32 r1, ImpLblB1
	MOV32 r2, ImpLblA1
	IMPORT ImpLblA1
	MOV32 r3, ImpLblNotDeclared

	IMPORT ImpLblB2 [BASED r10]
	MOV32 r1, ImpLblB2
	MOV32 r2, ImpLblA2
	IMPORT ImpLblA2 [BASED r10]

	IMPORT ImpLblB3 [COMMON=40]
	MOV32 r1, ImpLblB3
	MOV32 r2, ImpLblA3
	IMPORT ImpLblA3 [COMMON=0x10]
	; --------------------------------

	; --------------------------------
	; Map (before):
	MOV32 r1, MBlbl1
	MOV32 r2, MBlbl2

	; Map (after):
	MOV32 r1, MAlbl1
	MOV32 r2, MAlbl2
	; --------------------------------

	; --------------------------------
	; Based area (before):
	MOV32 r1, DataBefore
	MOV32 r2, DBlbl1
	MOV32 r3, DBlbl2
	MOV32 r4, DataBefore - &7812

	; Based area (after):
	MOV32 r1, DataAfter
	MOV32 r2, DAlbl1
	MOV32 r3, DAlbl2
	MOV32 r4, DataAfter - &7812
	; --------------------------------
	MEND

	MAP 4, r12
	# 16
	# 4
MBlbl1  # 0xFF30
MBlbl2	# 4

	ARM
	TestCaseNonABS ARM
	TestCaseABS ARM

	THUMB
	TestCaseNonABS THUMB
	TestCaseABS THUMB

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
	% 0x7F30
DAlbl2	DCD 4

	END
