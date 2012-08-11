; Tests DBG
; RUNOPT: -CPU=7-A

	AREA	Code, CODE

	[ :LNOT: REFERENCE
		DBG	#5
		DBGNE	#15
	|
		DCI	0xe320f0f5
		DCI	0x1320f0ff
	]
	
	END
