	GET	config.h

	AREA	Code, CODE, READONLY

		^	0, r12
foo		#	4
bar		#	4
sizeof_map	*	@

	[ :LNOT: REFERENCE
		LDR	r0, bar
		MOV	r1, #sizeof_map
;		LDR	r2, baz
	|
		LDR	r0,[r12,#4]
		MOV	r1,#8
;		LDR	r2,[r9]
	]

;		^	0, r9
;baz		#	4

