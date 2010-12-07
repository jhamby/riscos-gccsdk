	AREA	Code, CODE, READONLY

		^	0, r12
foo		#	4
bar		#	4
sizeof_map	*	@

	[ :LNOT: REFERENCE
		LDR	r0, bar
		MOV	r1, #sizeof_map
		MOV	r1, #@
		LDR	r2, baz
		ADR	r3, bar
		ADRL	r4, bar		; Will be detected ADR is sufficent.
	|
		LDR	r0,[r12,#4]
		MOV	r1,#8
		MOV	r1,#8
		LDR	r2,[r9]
		ADD	r3,r12,#4
		ADD	r4,r12,#4
	]

		^	0, r9
baz		#	4

	END
