; Tests the :DEF: operand

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	MACRO
	TestDef $var
	[ :DEF: $var
	= "$var is defined\n"
	|
	= "$var is not defined\n"
	]
	MEND
	
	TestDef Foo	; Not defined
	IMPORT	Bar
	TestDef Bar	; Is defined
	TestDef Code	; Is defined
	GBLA	Bar2
	TestDef Bar2	; Is defined

	|

	= "Foo is not defined\n"
	= "Bar is defined\n"
	= "Code is defined\n"
	= "Bar2 is defined\n"

	]
	
	END
