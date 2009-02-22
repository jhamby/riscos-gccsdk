	GET	config.h

	AREA	|foo|, CODE, READONLY

	^	0, r12
foo		#	4
bar		#	4
sizeof_map	*	@

	[ :LNOT: REFERENCE
	LDR	r0, bar
	MOV	r1, #sizeof_map
	|
	LDR	r0,[r12,#4]
	MOV	r1,#8
	]

