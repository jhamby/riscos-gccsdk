; RUNOPTACT: -uppercase
; Tests the -UpperCase option.  When set, only uppercase mnemonics are
; recognised.

	AREA	Code, CODE
	
	[ :LNOT: REFERENCE
	MACRO
	mov	$x, $y
	MOV	r0, #0
	MEND

	MOV	r1, #1
	mov	r2, #2
	|
	MOV	r1, #1
	MOV	r0, #0
	]
	
	END
