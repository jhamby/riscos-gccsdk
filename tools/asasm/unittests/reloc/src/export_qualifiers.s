	GET export_qualifiers.template

	DoExport FPREGARGS
	DoExport NOFPREGARGS
	DoExport SOFTFP
	DoExport HARDFP
	DoExport DATA
	DoExport CODE
	DoExport ARM
	DoExport THUMB
	DoExport LEAF
	DoExport NOLEAF
	DoExport USESSB
	DoExport NOUSESSB

	; Empty qualifier list:
	EXPORT ExpEmptyList []
ExpEmptyList

	END
