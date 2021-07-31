; A label can have two types, either relative reference to the start of
; the area, either an absolute integer value.  Here we're testing the
; cases which should work (note subtraction of two relative labels result
; in a plain integer which is assignable to an arithmetic variable).

	[ {TRUE}
	; Absolute area: labels are integers
	AREA	CodeA, CODE, ABS

	DCI	0
LabelA
	DCI	0
	DCI	0

	GBLA	VarA1
	GBLA	VarA2
	GBLA	VarA3
VarA1	SETA	{PC}+8-LabelA	; Result is an integer.
VarA2	SETA	{PC}+8		; Result is an integer.
VarA3	SETA	LabelA		; Result is an integer.
	]

	[ {TRUE}
	; Area marked with ORG: absolute area.
	AREA	CodeB, CODE
	ORG	4

LabelB
	DCI	0
	DCI	0

	GBLA	VarB1
	GBLA	VarB2
	GBLA	VarB3
VarB1	SETA	{PC}+8-LabelB	; Result is an integer.
VarB2	SETA	{PC}+8		; Result is an integer.
VarB3	SETA	LabelB		; Result is an integer.
	]

	[ {TRUE}
	; Non-absolute area, labels are relative.
	AREA	CodeC, CODE

	DCI	0
LabelC
	DCI	0
	DCI	0

	GBLA	VarC1
	GBLA	VarC2
	GBLA	VarC3
VarC1	SETA	{PC}+8-LabelC	; Result is an integer.
;;VarC2	SETA	{PC}+8		; Result is NOT an integer.  FIXME: when enabled, this should give an error but we can't test that yet.
;;VarC3	SETA	LabelC		; Result is NOT an integer.  FIXME: when enabled, this should give an error but we can't test that yet.
	]

	END
