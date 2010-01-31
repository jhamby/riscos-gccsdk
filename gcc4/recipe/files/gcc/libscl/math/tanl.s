	AREA	|C$$code|, CODE, READONLY

	EXPORT	tanl
tanl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	TAND	f0, f0
	MOV	pc, lr
	END
