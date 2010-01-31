	AREA	|C$$code|, CODE, READONLY

	; return sqrt (x*x + y*y)
	EXPORT	hypot
hypot
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	MUFD	f0, f0, f0
	MUFD	f1, f1, f1
	ADFD	f0, f0, f1
	SQTD	f0, f0
	MOV	pc, lr

	END
