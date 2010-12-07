		AREA	Code, CODE, READONLY

		^	0
foo_b		#	4
bar_b		#	4

	[ :LNOT: REFERENCE
		LDR	r1,[r2,#foo_b]
		STR	r1,[r2,#bar_b]
; FIXME		LDR	r1,[r2,#foo_a]
; FIXME		STR	r1,[r2,#bar_a]
	|
		LDR	r1,[r2]
		STR	r1,[r2,#4]
; FIXME		LDR	r1,[r2]
; FIXME		STR	r1,[r2,#4]
	]

		^	0
foo_a		#	4
bar_a		#	4

		END

