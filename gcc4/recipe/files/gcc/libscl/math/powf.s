	AREA	|C$$code|, CODE, READONLY
	EXPORT	powf
powf
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	POWS	f0, f0, f1
	MOV	pc, lr

	END
