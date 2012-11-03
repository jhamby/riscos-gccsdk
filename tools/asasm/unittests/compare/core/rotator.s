; Tests the rotator syntax for immediate values.

	AREA	Code1, CODE
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

	AREA	Code2, CODE
	[ :LNOT: REFERENCE
	MOV	r1, #&81,0
	MOV	r2, #&81,2
	MOV	r3, #&81,8
	MOV	r4, #&81,16
	MOV	r5, #&81,24
	MOV	r6, #&81,30
	|
	MOV	r1, #&81
	MOV	r2, #&40000020
	MOV	r3, #&81000000
	MOV	r4, #&810000
	MOV	r5, #&8100
	MOV	r6, #&204
	]

	END