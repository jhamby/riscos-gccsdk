	[ :LNOT:REFERENCE

	ORG	&1000
	AREA	Code1, CODE, READONLY
	DCD	Code1
	DCD	2*Code1		; When we have an absolute area, this should work.

	AREA	Code2, CODE, READONLY
	ORG	&2000
	DCD	Code2
	DCD	2*Code2		; When we have an absolute area, this should work.

	|

	AREA	Code1, CODE, READONLY
	ORG	&1000
	DCD	&1000
	DCD	&2000

	AREA	Code2, CODE, READONLY
	ORG	&2000
	DCD	&2000
	DCD	&4000

	]

	END
