; Test {PC} implementation

	; Test {PC} for a non-absolute area:
	AREA	Code1, CODE, READONLY
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	{PC}
	LDR	r1, {PC}
	LDR	r1, {PC} + 16
	LDR	r1, 16 + {PC}
	MOV	r2, #{PC} - {PC}
	|
	DCD	Code1 + 0
	DCD	Code1 + 4
	DCD	Code1 + 8
	LDR	r1, Code1 + 12
	LDR	r1, Code1 + 32
	LDR	r1, Code1 + 36
	MOV	r2, #0
	]

	; Test {PC} for an absolute area:
	AREA	Code2, CODE, ABS
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	{PC}
	|
	DCD	0
	DCD	4
	DCD	8
	]

	; Test {PC} for an area with non-zero base address:
	AREA	Code3, CODE, ABS
	ORG	&200
	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	{PC}
	|
	DCD	&200+0
	DCD	&200+4
	DCD	&200+8
	]

	END
