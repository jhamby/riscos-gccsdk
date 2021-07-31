; RUNOPT: --apcs=/softfp

	GET include/export_qualifiers.template
	DoExport SOFTFP
	DoExport HARDFP

	GET include/import_qualifiers.template
	DoImport SOFTFP
	DoImport HARDFP

	END
