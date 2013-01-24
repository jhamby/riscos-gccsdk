; Tests built-in variables {CONFIG}, {CODESIZE} and {UAL} 
; RUNOPT: -APCS 3/32bit

	MACRO
	Expect $val, $ual
	ASSERT {CONFIG} = $val
	ASSERT {CODESIZE} = $val
	ASSERT {UAL} = $ual
	MEND

	; Because of the -APCS 3/32bit commandline option
	Expect 32, {FALSE}

	CODE16
	Expect 16, {FALSE}

	CODE32
	Expect 32, {FALSE}

	THUMB
	Expect 16, {TRUE}

	ARM
	Expect 32, {FALSE}

	THUMBX
	Expect 16, {TRUE}

	END