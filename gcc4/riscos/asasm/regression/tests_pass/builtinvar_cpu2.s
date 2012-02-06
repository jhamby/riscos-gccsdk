; Tests built-in variable {CPU} (when -CPU option is specifying an architecture)
; -RUNOPT: -CPU=3

	ASSERT {CPU} = "Generic ARM"
	
	END
