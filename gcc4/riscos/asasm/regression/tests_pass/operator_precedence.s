; Tests precedence of unary & binary operators

	AREA	Data, DATA, READONLY
	
	[ :LNOT: REFERENCE
	DCD 3 :OR: 1:SHL:26 :OR: 1:SHL:27 :AND::NOT: (3 + 1:SHL:26 + 1:SHL:27)
	|
	DCD 0
	]
	
	END
