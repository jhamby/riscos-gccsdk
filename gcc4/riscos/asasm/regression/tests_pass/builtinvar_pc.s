; Tests built-in variable {PC} and .

	; Test {PC} and . for a non-absolute area:
	AREA	Code1, CODE, READONLY
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	.
	LDR	r1, {PC}
	LDR	r1, {PC} + 16
	LDR	r1, 16 + {PC}
	MOV	r2, #{PC} - {PC}
	LDR	r1, .
	LDR	r1, . + 16
	LDR	r1, 16 + .
	MOV	r2, #. - .
	|
	DCD	Code1 + 0
	DCD	Code1 + 4
	DCD	Code1 + 8
	LDR	r1, Code1 + 12
	LDR	r1, Code1 + 32
	LDR	r1, Code1 + 36
	MOV	r2, #0
	LDR	r1, Code1 + 28
	LDR	r1, Code1 + 48
	LDR	r1, Code1 + 52
	MOV	r2, #0
	]

	; Test {PC} and . for an absolute area:
	AREA	Code2, CODE, ABS
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	.
	|
	DCD	0
	DCD	4
	DCD	8
	]

	; Test {PC} and . for an area with non-zero base address:
	AREA	Code3, CODE, ABS
	ORG	&200
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	.
	|
	DCD	&200+0
	DCD	&200+4
	DCD	&200+8
	]

	; Test {PC} and . via variables (relative area):
	AREA	Code3, CODE

	[ :LNOT: REFERENCE
	MOV	r0, #1
	MOV	r1, #2

Code3Var1	*	{PC} + 20
Code3Var2	*	{PC}

	MOV	r0, #1
	MOV	r1, #2

Code3Var3	*	. + 20
Code3Var4	*	.

	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2

	LDR	r0, Code3Var1
	LDR	r1, Code3Var2
	LDR	r0, Code3Var1
	LDR	r1, Code3Var2
	LDR	r2, Code3Var3
	LDR	r3, Code3Var4
	LDR	r2, Code3Var3
	LDR	r3, Code3Var4
	|
	MOV	r0, #1
	MOV	r1, #2
Code3Var2
	MOV	r0, #1
	MOV	r1, #2
Code3Var4
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
Code3Var1
	MOV	r1, #2
	MOV	r0, #1
Code3Var3
	MOV	r1, #2

	LDR	r0, Code3Var1
	LDR	r1, Code3Var2
	LDR	r0, Code3Var1
	LDR	r1, Code3Var2
	LDR	r2, Code3Var3
	LDR	r3, Code3Var4
	LDR	r2, Code3Var3
	LDR	r3, Code3Var4
	]

	; Test {PC} and . via variables (absolute area):
	AREA	Code4, CODE
	ORG	&1000

	[ :LNOT: REFERENCE
	MOV	r0, #1
	MOV	r1, #2

Code4Var1	*	{PC} + 20
Code4Var2	*	{PC}

	MOV	r0, #1		; <= Code4Var2 (&1008)
	MOV	r1, #2

Code4Var3	*	. + 20
Code4Var4	*	.

	MOV	r0, #1		; <= Code4Var4 (&1010)
	MOV	r1, #2

	MOV	r0, #1
	MOV	r1, #2		; <= Code4Var1 (&101c)
	MOV	r0, #1
	MOV	r1, #2		; <= Code4Var3 (&1024)

	LDR	r0, Code4Var1
	LDR	r1, Code4Var2
	LDR	r0, Code4Var1
	LDR	r1, Code4Var2
	LDR	r2, Code4Var3
	LDR	r3, Code4Var4
	LDR	r2, Code4Var3
	LDR	r3, Code4Var4
	|
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2
	MOV	r0, #1
	MOV	r1, #2

	LDR	r0, &101c
	LDR	r1, &1008
	LDR	r0, &101c
	LDR	r1, &1008
	LDR	r2, &1024
	LDR	r3, &1010
	LDR	r2, &1024
	LDR	r3, &1010
	]

	END
