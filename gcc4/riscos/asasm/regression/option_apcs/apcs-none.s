; RUNOPT: -APCS none

	AREA Data, DATA

	[ :LNOT: REFERENCE
	GET tests_pass/include/predef_reg_test.s
	|
	= "r0 T 00000000\n"
	= "R0 T 00000000\n"
	= "r13 T 0000000D\n"
	= "r14 T 0000000E\n"
	= "r15 T 0000000F\n"
	= "sp T 0000000D\n" ; ObjAsm 3.xx : "sp F\n"
	= "SP T 0000000D\n" ; ObjAsm 3.xx : "SP F\n"
	= "lr T 0000000E\n"
	= "LR T 0000000E\n"
	= "pc T 0000000F\n"
	= "PC T 0000000F\n"
	= "p0 T 00000000\n"
	= "P0 F\n"
	= "p15 T 0000000F\n"
	= "P15 F\n"
	= "c0 T 00000000\n"
	= "C0 F\n"
	= "c15 T 0000000F\n"
	= "C15 F\n"
	= "f0 T 00000000\n"
	= "F0 T 00000000\n"
	= "f7 T 00000007\n"
	= "F7 T 00000007\n"
	= "a1 F\n"
	= "A1 F\n"
	= "v1 F\n"
	= "V1 F\n"
	= "v5 F\n"
	= "ip F\n"
	= "IP F\n"
	= "v6 F\n"
	= "sb F\n"
	= "SB F\n"
	= "v7 F\n"
	= "sl F\n"
	= "SL F\n"
	= "v8 F\n"
	= "fp F\n"
	= "FP F\n"
	]

	END
