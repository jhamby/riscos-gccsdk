; Tests built-in variables {LINENUM}, {LINENUMUP} and {LINENUMUPPER}.

; Watch out add/removing lines in this test ;-)

	MACRO
	Top
	DCD	0x0123		; Line 1
	DCD	{LINENUM}	; Line 2
	DCD	{LINENUMUP}	; Line 3
	DCD	{LINENUMUPPER}	; Line 4
	Bottom			; Line 5
	MEND

	MACRO
	Bottom
	DCD	0x0123		; Line 1
	DCD	0x4567		; Line 2
	DCD	0x89ab		; Line 3
	DCD	0xcdef		; Line 4
	DCD	{LINENUM}	; Line 5
	DCD	{LINENUMUP}	; Line 6
	DCD	{LINENUMUPPER}	; Line 7
	MEND

	AREA Data, DATA
	[ :LNOT: REFERENCE
		DCD {LINENUM}		; Line 27
		DCD {LINENUMUP}
		DCD {LINENUMUPPER}
		
		Top			; Line 31
	|
		DCD 27
		DCD 28
		DCD 29
		
		DCD 0x0123
		DCD 31
		DCD 3
		DCD 4
		
		DCD 0x0123
		DCD 0x4567
		DCD 0x89ab
		DCD 0xcdef
		DCD 31
		DCD 6
		DCD 5
	]

	END
