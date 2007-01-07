	AREA	|C$$code|, CODE, READONLY

	EXPORT  fabs
fabs
	BIC	a1, a1, #&80000000
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MOV	pc, lr

	END
