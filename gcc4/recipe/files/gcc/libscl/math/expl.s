	AREA	|C$$code|, CODE, READONLY
	EXPORT	expl
expl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	EXPD	f0, f0
	MOV	pc, lr

	END
