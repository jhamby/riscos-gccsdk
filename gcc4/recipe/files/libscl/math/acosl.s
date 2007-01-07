
	AREA	|C$$code|, CODE, READONLY

	EXPORT	acosl
acosl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	ACSD	f0, f0
	MOV	pc, lr

	END
