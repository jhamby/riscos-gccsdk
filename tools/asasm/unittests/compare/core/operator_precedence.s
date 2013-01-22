; Tests precedence of unary & binary operators

	AREA	Data, DATA, READONLY
	
	[ :LNOT: REFERENCE
	DCD 3 :OR: 1:SHL:26 :OR: 1:SHL:27 :AND::NOT: (3 + 1:SHL:26 + 1:SHL:27)
	|
	DCD 0
	]

	; Expression evaluator and tester.
	MACRO
	Eval $expr, $val
	LCLA val
val	SETA $expr
	ASSERT val = $val
	MEND

	Eval ~-2 + ~-3, 3
	Eval 1 + 9 * 8 / 6 % 5, 3
	Eval (1 + (((9 * 8) / 6) % 5)), 3
	Eval 1 + (9 * (8 / (6 % 5))), 73

	Eval 1 + "abc":LEFT:1, 98
	Eval 0 + "abc":LEFT:1 + 1, 98
	Eval 0 + "abc":LEFT:2/2 + 1, 98
	Eval 0 + "abc":LEFT:9:MOD:8 + 1, 98

	Eval 0 + "a":CC:"bc":LEFT:1 :SHL:8, &6100
	
	END
