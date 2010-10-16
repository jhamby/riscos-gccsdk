	GET	config.h

	AREA	Code, CODE, READONLY

	^	0, r12
baz	#	4
marray	#	256

	[ :LNOT: REFERENCE
		; Late label in same area
		; (=> PC relative expression)
		LDR	r0, foo
		LDR	r0, array + 10
		; Late label in different area based on r9
		; (=> R9 relative expression)
		LDR	r0, bar
		LDR	r0, barray + 10
		; Late label in different area
		; (=> PC-relative expression)
		LDR	r0, bat
		LDR	r0, oarray + 10
		; Label in map based on R12
		; (=> R12 relative expression)
		LDR	r0, baz
		LDR	r0, marray + 10
	|
		LDR	r0, [pc,#32 - 8]
		LDR	r0, [pc,#32 + 10 - 8]

		LDR	r0, [r9]
		LDR	r0, [r9,#14]

		LDR	r0, [pc,#-24]
		LDR	r0, [pc,#-14]

		LDR	r0, [r12]
		LDR	r0, [r12,#14]
	]

foo	DCD	0x55

array	%	256

	AREA	Data, DATA, BASED r9

bar	DCD	0xaa

barray	%	256

	AREA	Data2, DATA

bat	DCD	0xf8

oarray	%	256

	END
