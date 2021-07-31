; Load literal in ABS area + KEEP.

	ORG	&80000
	AREA	Code, CODE
	KEEP
	[ :LNOT: REFERENCE
	LDR	r1, =&123456
	|
	LDR	r1, [pc, #-4]
	DCD	&123456
	]

	END
