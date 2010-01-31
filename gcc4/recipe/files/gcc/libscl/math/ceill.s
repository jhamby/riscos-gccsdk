	AREA	|C$$code|, CODE, READONLY
	EXPORT	ceill
ceill
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDP	f0, f0
	MOV	pc, lr

	END
