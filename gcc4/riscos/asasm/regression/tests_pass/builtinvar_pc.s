; Tests {PC}

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE
	DCD	{PC}
	DCD	{PC}
	DCD	{PC}
	LDR	r1, {PC}
	LDR	r1, {PC} + 16
	LDR	r1, 16 + {PC}
	MOV	r2, #{PC} - {PC}
	|
	DCD	Code + 0
	DCD	Code + 4
	DCD	Code + 8
	LDR	r1, Code + 12
	LDR	r1, Code + 32
	LDR	r1, Code + 36
	MOV	r2, #0
	]

	END

