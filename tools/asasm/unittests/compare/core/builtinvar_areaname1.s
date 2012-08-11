; Tests built-in variable {AREANAME}

	; Implicit areaname is "$$$$$$$" but $ needs escaping. 
	ASSERT {AREANAME} = "$$$$$$$$$$$$$$"
	AREA Code1, CODE
	ASSERT {AREANAME} = "Code1"
	AREA Code2, CODE
	ASSERT {AREANAME} = "Code2"
	AREA Code3, CODE
	ASSERT {AREANAME} = "Code3"

	END