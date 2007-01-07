	AREA	|C$$code|, CODE, READONLY

	EXPORT	logf
logf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	LGNS	f0, f0
	MOV	pc, lr

	END
