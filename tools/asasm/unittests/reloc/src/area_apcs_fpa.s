; RUNOPT: --apcs=/fpa
	AREA	CodeFPA, CODE
	DCFD	123456789.

	; APCS option /fpa will make /nofpregargs default.
	GET include/export_qualifiers.template
	DoExport FPREGARGS
	DoExport NOFPREGARGS

	GET include/import_qualifiers.template
	DoImport FPREGARGS
	DoImport NOFPREGARGS

	END
