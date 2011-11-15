	; Specific mnemonic decode tests.

	; EXPORT is a keyword, but EXPORTGLOBAL should be treated
	; as a macro.
	MACRO
	EXPORTGLOBAL
	MEND
	EXPORTGLOBAL

	AREA	Data, DATA
	[ :LNOT: REFERENCE
	; Label symbols used for macros which share same prefix as existing
	; mnemonics (in this case 'SET*').
	MACRO
$lbl	SETD
	MEND

NonExistingSymbol SETD
	ASSERT :LNOT: :DEF: NonExistingSymbol

	GBLL ExistingSymbol
	ASSERT :DEF: ExistingSymbol
	= "1:" :CC: :STR:ExistingSymbol :CC: "\n"
ExistingSymbol SETD
	ASSERT :DEF: ExistingSymbol
	= "2:" :CC: :STR:ExistingSymbol :CC: "\n"
	|
	= "1:F\n"
	= "2:F\n"
	]

	; Reset local macro variables properly.
	AREA	Code, CODE
        ^       0,R12
temp	# 4
	MACRO
	Test
	LCLA	temp
temp	SETA	4
	MEND
	Test

	END
