	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	EXPORT	tanhl
tanhl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MNFD	f1, f0
	EXPD	f0, f0
	EXPD	f1, f1

	SUFD	f2, f0, f1
	ADFD	f3, f0, f1
	DVFD	f0, f2, f3
	MOV	pc, lr

	END
