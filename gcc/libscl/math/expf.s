	AREA	|C$$code|, CODE, READONLY
	EXPORT	expf
expf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	EXPS	f0, f0
	MOV	pc, lr

	END
