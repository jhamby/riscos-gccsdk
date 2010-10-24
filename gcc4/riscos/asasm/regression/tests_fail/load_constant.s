	[ :LNOT: REFERENCE

		AREA	Data, DATA, READONLY

		^	20
rcrd1f1		#	12
rcrd1f2		#	8
rcrd1f3		#	4

		^	4
rcrd2f1		#	4
rcrd2f2		#	2048
rcrd2f3		#	8

		AREA	Code, CODE, READONLY

		LDR	r0, =rcrd1f2
		LDR	r1, =rcrd2f2
		LDR	r2, =rcrd2f3

	|

		AREA	Data, DATA, READONLY

		AREA	Code, CODE, READONLY

		MOV	r0, #32
		MOV	r1, #8
		LDR	r2, [pc, #-4]
		DCD	2048 + 8

	]

	END
