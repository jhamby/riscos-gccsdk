	AREA	|C$$code|, CODE, READONLY
	EXPORT	cosl
cosl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	COSD	f0, f0
	MOV	pc, lr

	END
