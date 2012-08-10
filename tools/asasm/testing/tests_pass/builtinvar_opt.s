; Tests built-in variable {OPT} and OPT directive implementation.

	AREA	Data, DATA
	MACRO
	ExpectOpt $val
	LCLA	opt
opt	SETA	{OPT}
	; INFO 0, "Expect {OPT} to be " :CC: :STR:($val) :CC: " and is " :CC: :STR:opt
	[ :LNOT: REFERENCE
	DCD	{OPT}
	|
	DCD	$val
	]
	MEND

	GBLA optSt
optSt	SETA	{OPT}

	OPT 0
	ExpectOpt optSt

	; b1-0
	[ (optSt :AND: (1:SHL:0)) /= 0
	OPT 1:SHL:1
	ExpectOpt optSt :AND: :NOT:(1:SHL:0) :OR: (1:SHL:1)
	OPT 1:SHL:0
	ExpectOpt optSt :OR: (1:SHL:0)
	|
	OPT 1:SHL:0
	ExpectOpt optSt :OR: (1:SHL:0)
	OPT 1:SHL:1
	ExpectOpt optSt :AND: :NOT:(1:SHL:0) :OR: (1:SHL:1)
	]

	; b2 (not sticky)
	OPT 1:SHL:2
	ExpectOpt optSt

	; b3 (not sticky)
	;OPT 1:SHL:3
	;ExpectOpt optSt

	; b4-5
	[ (optSt :AND: (1:SHL:4)) /= 0
	OPT 1:SHL:5
	ExpectOpt optSt :AND: :NOT:(1:SHL:4) :OR: (1:SHL:5)
	OPT 1:SHL:4
	ExpectOpt optSt :OR: (1:SHL:4)
	|
	OPT 1:SHL:4
	ExpectOpt optSt :OR: (1:SHL:4)
	OPT 1:SHL:5
	ExpectOpt optSt :AND: :NOT:(1:SHL:4) :OR: (1:SHL:5)
	]

	; b6-7
	[ (optSt :AND: (1:SHL:6)) /= 0
	OPT 1:SHL:7
	ExpectOpt (optSt :AND: :NOT:(1:SHL:6) :OR: (1:SHL:7)) :AND: :NOT:3 :OR: 2
	OPT 1:SHL:6
	ExpectOpt optSt :OR: (1:SHL:6)
	|
	OPT 1:SHL:6
	ExpectOpt optSt :OR: (1:SHL:6)
	OPT 1:SHL:7
	ExpectOpt (optSt :AND: :NOT:(1:SHL:6) :OR: (1:SHL:7)) :AND: :NOT:3 :OR: 2
	]

	; b8-9
	[ (optSt :AND: (1:SHL:8)) /= 0
	OPT 1:SHL:9
	ExpectOpt optSt :AND: :NOT:(1:SHL:8) :OR: (1:SHL:9)
	OPT 1:SHL:8
	ExpectOpt optSt :OR: (1:SHL:8)
	|
	OPT 1:SHL:8
	ExpectOpt optSt :OR: (1:SHL:8)
	OPT 1:SHL:9
	ExpectOpt optSt :AND: :NOT:(1:SHL:8) :OR: (1:SHL:9)
	]

	; b10-11 (not sticky)
	OPT 1:SHL:10
	ExpectOpt optSt
	OPT 1:SHL:11
	ExpectOpt optSt
	
	; b12-13
	[ (optSt :AND: (1:SHL:12)) /= 0
	OPT 1:SHL:13
	ExpectOpt optSt :AND: :NOT:(1:SHL:12) :OR: (1:SHL:13)
	OPT 1:SHL:12
	ExpectOpt optSt :OR: (1:SHL:12)
	|
	OPT 1:SHL:12
	ExpectOpt optSt :OR: (1:SHL:12)
	OPT 1:SHL:13
	ExpectOpt optSt :AND: :NOT:(1:SHL:12) :OR: (1:SHL:13)
	]
	
	; b14-15 (not sticky)
	OPT 1:SHL:14
	ExpectOpt optSt
	OPT 1:SHL:15
	ExpectOpt optSt

	; b16-31 (not sticky)
	GBLA cnt
cnt	SETA 16
	WHILE cnt < 32
	OPT 1:SHL:cnt
	ExpectOpt optSt
cnt	SETA cnt + 1
	WEND

	; On & off set at the same time.
	OPT optSt + (optSt:SHL:1)
	ExpectOpt optSt
	
	END
