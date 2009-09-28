	GET	config.h

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE
		LDR	r2, baz
	|
		LDR	r2,[r9]
	]

		^	0, r9
baz		#	4

