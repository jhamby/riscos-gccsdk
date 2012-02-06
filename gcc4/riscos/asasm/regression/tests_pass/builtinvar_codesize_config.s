; Tests built-in variables {CONFIG} and {CODESIZE} 
; RUNOPT: -APCS 3/32bit

	MACRO
	Expect $val
	ASSERT {CONFIG} = $val
	ASSERT {CODESIZE} = $val
	MEND

	; Because of the -APCS 3/32bit commandline option
	Expect 32

	CODE16
	Expect 16

	CODE32
	Expect 32

	THUMB
	Expect 16

	ARM
	Expect 32
	
	THUMBX
	Expect 16
	
	END