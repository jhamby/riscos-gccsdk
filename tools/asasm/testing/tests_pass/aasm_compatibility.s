; Tests when the AAsm compatibility is selected whether the AOF/AOUT/AREA/IMPORT/EXPORT
; STRONG/ENTRY/KEEP directives are disabled (resulting in evoking similar named macros).
; RUNOPTACT: -ABS

	[ :LNOT: REFERENCE

	MACRO
	AOF
	= "AOF macro\n"
	MEND

	MACRO
	AOUT
	= "AOUT macro\n"
	MEND

	MACRO
	AREA $a, $b
	= "AREA macro\n"
	MEND

	MACRO
	IMPORT $x
	= "IMPORT macro\n"
	MEND

	MACRO
	EXPORT $x
	= "EXPORT macro\n"
	MEND

	MACRO
	STRONG $x
	= "STRONG macro\n"
	MEND

	MACRO
	ENTRY
	= "ENTRY macro\n"
	MEND

	MACRO
	KEEP
	= "KEEP macro\n"
	MEND

	AOF
	AOUT

	AREA	Data, DATA

	IMPORT	a
	EXPORT	b
	STRONG	c
	ENTRY
	KEEP

	|

	AREA	|ABS$$BLOCK|, CODE

	= "AOF macro\n"
	= "AOUT macro\n"
	= "AREA macro\n"
	= "IMPORT macro\n"
	= "EXPORT macro\n"
	= "STRONG macro\n"
	= "ENTRY macro\n"
	= "KEEP macro\n"

	]

	END
