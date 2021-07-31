; RUNOPT: --apcs=/vfp
	AREA	CodeVFP, CODE
	DCFD	123456789.

	; APCS option /vfp will make /fpregargs default.
	GET include/export_qualifiers.template
	DoExport FPREGARGS
	DoExport NOFPREGARGS

	GET include/import_qualifiers.template
	DoImport FPREGARGS
	DoImport NOFPREGARGS

	END
