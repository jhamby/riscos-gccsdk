; RUNOPT: --apcs=/softfp

	GET export_qualifiers.template
	DoExport SOFTFP
	DoExport HARDFP

	GET import_qualifiers.template
	DoImport SOFTFP
	DoImport HARDFP

	END
