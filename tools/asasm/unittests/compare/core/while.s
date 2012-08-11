; Tests WHILE / WEND

	AREA	Data, DATA

	[ :LNOT: REFERENCE
		GBLA	cntOut
cntOut		SETA	3
		WHILE	cntOut > 0
			GBLA	cntIn
cntIn			SETA	5
			WHILE	cntIn > 0
				DCB	64 + cntIn
cntIn				SETA	cntIn - 1
			WEND
			ALIGN
cntOut			SETA	cntOut - 1
		WEND

		WHILE	{FALSE}
			INFO	1, "This may not happen"
		WEND
	|
		= "EDCBA"
		ALIGN
		= "EDCBA"
		ALIGN
		= "EDCBA"
		ALIGN
	]

	[ :LNOT: REFERENCE
		GBLS	EndWhile
EndWhile	SETS	"WEND"

	WHILE {FALSE}
	$EndWhile

	|
	]

	; Variable substitution needs to be redone for each WHILE evaluation.
	[ :LNOT: REFERENCE
        GBLA    count
count   SETA    8
        WHILE   $count > 0
	DCD	$count
count   SETA    count - 4
        WEND
	|
	DCD	8
	DCD	4
	]

	; Test if WHILE rewind happens correctly when variable substitution
	; needs to be done.
	[ :LNOT: REFERENCE
	; File
		GBLA	CountFile
CountFile	SETA	3
		WHILE	CountFile > 0
		DCD	CountFile
CountFile	SETA	CountFile - 1
		WEND

		MACRO
		RewindTest $MaxCountMacro
		LCLA	CountMacro
		WHILE	CountMacro < $MaxCountMacro
		DCD	CountMacro
CountMacro	SETA	CountMacro + 1
		WEND
		MEND
		RewindTest "3            "
	|
		DCD	3, 2, 1
		DCD	0, 1, 2
	]

	END
