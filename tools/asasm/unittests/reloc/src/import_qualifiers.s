	GET include/import_qualifiers.template

	DoImport FPREGARGS
	DoImport NOFPREGARGS
	DoImport SOFTFP
	DoImport HARDFP
	DoImport DATA
	DoImport CODE
	DoImport WEAK
	DoImport NOWEAK
	DoImport READONLY
	DoImport READWRITE
	DoImport NOCASE
	DoImport CASE

	IMPORT	ImpBASED [ BASED r9 ]
	LDR	r0, ImBASED

	IMPORT	ImpCOMMON [ COMMON =  3*4  ]
	DCD	ImpCOMMON
	ASSERT	?ImpCOMMON = 12

	; Empty qualifier list:
	IMPORT ImpEmptyList []
	BL	ImpEmptyList

	IMPORT ImpNoListFPREGARGS ,  FPREGARGS
	BL	ImpNoListFPREGARGS

	IMPORT ImpNoListFPREGARGS_WEAK  ,  WEAK  , FPREGARGS
	BL	ImpNoListFPREGARGS_WEAK

	IMPORT ImpNoListWEAK  ,  WEAK
	BL	ImpNoListWEAK

	END
