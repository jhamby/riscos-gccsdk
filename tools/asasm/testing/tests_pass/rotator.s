; Tests the rotator syntax for immediate values.

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; $base needs :
	;   - to be between 0 and 256 (excl),
	;   - bits 7 or 6 needs to be set,
	;   - bits 1 or 0 needs to be set.
	; otherwise the test is not reliable as the assembler has freedom
	; to generate a different solution.
	; $rot needs to be between 0 and 32 (excl) and even.
	MACRO
	TestRot	$base, $rot
	MOV	r3, #$base, $rot
	MOV	r3, #($base >> $rot) :OR: ($base << (32 - $rot))
	MEND

	GBLA	cnt
cnt	SETA	0
	WHILE	cnt < 32
	TestRot	&42, cnt
cnt	SETA	cnt + 2
	WEND
	
	|

	GBLA	cnt
cnt	SETA	0
	WHILE	cnt < 32
	DCI	&e3a03042 + cnt<<7
	DCI	&e3a03042 + cnt<<7
cnt	SETA	cnt + 2
	WEND

	]

	END
