; Tests the :DEF: operand

	AREA	Data, DATA

	[ :LNOT: REFERENCE

	MACRO
	TestDef $var
	[ :DEF: $var
	= "$var is defined\n"
	|
	= "$var is not defined\n"
	]
	MEND

	TestDef Foo		; Not defined
	IMPORT	SymImp
	TestDef SymImp		; Is defined
	TestDef Data		; Is defined
	EXPORT SymExp
	TestDef SymExp
SymExp
	GBLL	VarLog
	TestDef VarLog		; Is defined
	GBLS	VarStr
	TestDef VarStr		; Is defined
	GBLA	VarArith
	TestDef VarArith	; Is defined
	[ {FALSE} ; FIXME: we should be able to enable this
	TestDef |$$$$$$$|	; Not defined
	]

	|

	= "Foo is not defined\n"
	= "SymImp is defined\n"
	= "Data is defined\n"
	EXPORT SymExp
	= "SymExp is defined\n"
SymExp
	= "VarLog is defined\n"
	= "VarStr is defined\n"
	= "VarArith is defined\n"
	[ {FALSE} ; FIXME: we should be able to enable this
	= "|$$$$$$$| is not defined\n"
	]

	]

	END
