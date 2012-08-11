; Tests :RCONST:

	AREA	Data, DATA

	[ :LNOT: REFERENCE

	DCB	:RCONST: r3
	DCB	:RCONST: pc
	DCB	:RCONST: a3
	DCB	:RCONST: v2

	DCB	:RCONST: p5
	DCB	:RCONST: c4
	DCB	:RCONST: f3

fooR2	RN	r2
fooC4	CN	c4
fooF3	FN	f3
	DCB	:RCONST: fooR2
	DCB	:RCONST: fooC4
	DCB	:RCONST: fooF3

	; FIXME: Add tests on Neon/VFP q, d and s registers.

	|

	DCB	3, 15, 2, 5
	DCB	5, 4, 3
	DCB	2, 4, 3

	]

	END