; RUNOPT: -APCS 3/nonreent/swst/nofp

	AREA Data, DATA

	[ :LNOT: REFERENCE
	GET include/predef_reg_test.s
	|
	= "r0 T 00000000\n"
	= "R0 T 00000000\n"
	= "r13 T 0000000D\n"
	= "r14 T 0000000E\n"
	= "r15 T 0000000F\n"
	= "sp T 0000000D\n"
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
	= "a1 T 00000000\n"
	= "A1 F\n"
	= "v1 T 00000004\n"
	= "V1 F\n"
	= "v5 T 00000008\n"
	= "ip T 0000000C\n"
	= "IP F\n"
	= "v6 T 00000009\n"
	= "sb F\n"
	= "SB F\n"
	= "v7 F\n"
	= "sl T 0000000A\n"
	= "SL F\n"
	= "v8 T 0000000B\n"
	= "fp F\n" ; ObjAsm 3/4 : "fp T 0000000B\n"
	= "FP F\n"
	]

	END
