; Basic relocation test for ADR and ADRL.

		GBLL	CanUseMOVW
CanUseMOVW	SETL	{TARGET_ARCH_6T2} :LOR: {TARGET_ARCH_7} 

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
	ADR r1, CodeBefore
	ADRL r2, CodeBefore
	ADR r3, CBlbl1
	ADRL r4, CBlbl1

	; Other absolute area (before) - case 1:
	ADR r1, CodeABS1Before
	ADRL r2, CodeABS1Before
	ADR r3, CBA1lbl1
	ADRL r4, CBA1lbl1

	; Other absolute area (before) - case 2:
	[ CanUseMOVW
	ADR r1, CodeABS2Before
	]
	ADRL r2, CodeABS2Before
	[ CanUseMOVW
	ADR r3, CBA2lbl1
	]
	ADRL r4, CBA2lbl1

	; Other absolute area (before) - case 3:
	;ADR r1, CodeABS3Before
	ADRL r2, CodeABS3Before
	;ADR r3, CBA3lbl1
	ADRL r4, CBA3lbl1

	; This area (before):
	ADR r1, Code$sfx
	ADRL r2, Code$sfx
	ADR r3, Clbl1$sfx
	ADRL r4, Clbl2$sfx

	; Define + use in same line:
Clbl3$sfx	ADR r5, Clbl3$sfx
Clbl4$sfx	ADRL r6, Clbl4$sfx

	; This area (after):
	ADR r1, Clbl5$sfx
	ADRL r2, Clbl6$sfx

	; Other relocatable area (after):
	ADR r1, CodeAfter
	ADRL r2, CodeAfter
	ADR r3, CAlbl1
	ADRL r4, CAlbl1

	; Other absolute area (after) - case 1:
	ADR r1, CodeABS1After
	ADRL r2, CodeABS1After
	ADR r3, CAA1lbl1
	ADRL r4, CAA1lbl1

	; Other absolute area (after) - case 2:
	[ CanUseMOVW
	ADR r1, CodeABS2After
	]
	ADRL r2, CodeABS2After
	[ CanUseMOVW
	ADR r3, CAA2lbl1
	]
	ADRL r4, CAA2lbl1

	; Other absolute area (after) - case 3:
	;ADR r1, CodeABS3After
	ADRL r2, CodeABS3After
	;ADR r3, CAA3lbl1
	ADRL r4, CAA3lbl1

	; First used, then defined.
Clbl5$sfx	%	40
Clbl6$sfx	%	8
	; --------------------------------

	; --------------------------------
	IMPORT ImpLblB1
	ADR r1, ImpLblB1
	ADRL r2, ImpLblB1
	ADR r3, ImpLblA1
	ADRL r4, ImpLblA1
	IMPORT ImpLblA1
	ADR r5, ImpLblNotDeclared
	ADRL r6, ImpLblNotDeclared

	IMPORT ImpLblB2 [BASED r10]
	ADR r1, ImpLblB2
	ADRL r2, ImpLblB2
	ADR r3, ImpLblA2
	ADRL r4, ImpLblA2
	IMPORT ImpLblA2 [BASED r10]

	IMPORT ImpLblB3 [COMMON=40]
	ADR r5, ImpLblB3
	ADRL r6, ImpLblB3
	ADR r7, ImpLblA3
	ADRL r8, ImpLblA3
	IMPORT ImpLblA3 [COMMON=0x10]
	; --------------------------------

	; --------------------------------
	; Map (before):
	ADR r1, MBlbl1
	ADRL r2, MBlbl1
	;ADR r3, MBlbl2
	ADRL r4, MBlbl2

	; Map (after):
	ADR r1, MAlbl1
	ADRL r2, MAlbl1
	;ADR r3, MAlbl2
	ADRL r4, MAlbl2
	; --------------------------------

	; --------------------------------
	; Based area (before):
	ADR r1, DataBefore
	ADRL r2, DataBefore
	ADR r3, DBlbl1
	ADRL r4, DBlbl1
	;ADR r5, DBlbl2
	ADRL r6, DBlbl2

	; Based area (after):
	ADR r1, DataAfter
	ADRL r2, DataAfter
	ADR r3, DAlbl1
	ADRL r4, DAlbl1
	;ADR r5, DAlbl2
	ADRL r6, DAlbl2
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
