; Tests built-in variable {AREANAME}
; -RUNOPT: -ABS

	; Implicit areaname for AAsm compatibility mode is "ABS$$BLOCK" but
	; $ needs escaping. 
	ASSERT {AREANAME} = "ABS$$$$BLOCK"

	END