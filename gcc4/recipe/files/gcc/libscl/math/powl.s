	AREA	|C$$code|, CODE, READONLY

	EXPORT	powl
powl
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	POWD	f0, f0, f1
	MOV	pc, lr

	END
