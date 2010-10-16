		AREA	Code, CODE, READONLY

		^	0
foo		#	4
bar		#	4

	[ :LNOT: REFERENCE
		LDR	r0,[r1,#foo]
		STR	r0,[r1,#bar]
	|
		LDR	r0,[r1]
		STR	r0,[r1,#4]
	]

		END

