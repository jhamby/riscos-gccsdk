	AREA	|C$$code|, CODE, READONLY

	EXPORT	logl
logl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	LGND	f0, f0
	MOV	pc, lr

	END
