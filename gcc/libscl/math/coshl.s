	AREA	|C$$code|, CODE, READONLY
	EXPORT	coshl
coshl
	; return (exp(x) + exp(-x)) / 2
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MNFD	f1, f0
	EXPD	f0, f0
	EXPD	f1, f1
	ADFD	f0, f0, f1
	DVFD	f0, f0, #2
	MOV	pc, lr

	END
