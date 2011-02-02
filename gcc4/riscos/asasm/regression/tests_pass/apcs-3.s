; RUNOPT: -APCS 3

	AREA	Data, DATA

	[ :LNOT: REFERENCE
	GET	tests_pass/include/predef_reg_test.s
	|
	= "r0 T\n"
	= "R0 T\n"
	= "r13 T\n"
	= "r14 T\n"
	= "r15 T\n"
	= "sp T\n"
	= "lr T\n"
	= "pc T\n"
	= "LR T\n"
	= "PC T\n"
	= "p0 T\n"
	= "p15 T\n"
	= "c0 T\n"
	= "c15 T\n"
	= "f0 T\n"
	= "F0 T\n"
	= "f7 T\n"
	= "F7 T\n"
	= "a1 T\n"
	= "v1 T\n"
	= "v5 T\n"
	= "v6 T\n"
	= "sb F\n"
	= "v7 F\n"
	= "sl T\n"
	= "fp T\n"
	= "ip T\n"
	= "SP F\n"
	= "P0 F\n"
	= "P15 F\n"
	= "C0 F\n"
	= "C15 F\n"
	= "A1 F\n"
	= "V1 F\n"
	= "SB F\n"
	= "SL F\n"
	= "FP F\n"
	= "IP F\n"
	]

	END
