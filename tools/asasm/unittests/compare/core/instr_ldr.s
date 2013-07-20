	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE
		; Late label in same area
		; (=> PC relative expression)
		LDR	r1, foo
		LDR	r1, array + 10

		[ {FALSE}		; Results in relocations.
		; Late label in different area based on r9
		; (=> R9 relative expression)
		LDR	r2, bar
		LDR	r2, barray + 10
		|
		NOP
		NOP
		]

		[ {FALSE}		; Results in relocations.
		; Late label in different area
		; (=> PC-relative expression)
		LDR	r3, bat
		LDR	r3, oarray + 10
		|
		NOP
		NOP
		]

		; Label in map based on R12
		; (=> R12 relative expression)
		LDR	r4, baz
		LDR	r4, marray + 10
	|
		LDR	r1, [pc,#32 - 8]
		LDR	r1, [pc,#32 + 10 - 8]

		[ {FALSE}
		LDR	r2, [r9]
		LDR	r2, [r9,#14]
		|
		NOP
		NOP
		]

		[ {FALSE}
		LDR	r3, [pc,#-24]
		LDR	r3, [pc,#-14]
		|
		NOP
		NOP
		]

		LDR	r4, [r12]
		LDR	r4, [r12,#14]
	]

foo	DCD	0x55

array	%	16

	AREA	Data, DATA, BASED r9

bar	DCD	0xaa

barray	%	16

	AREA	Data2, DATA

bat	DCD	0xf8

oarray	%	16

	^	0, r12
baz	#	4
marray	#	16

	END
