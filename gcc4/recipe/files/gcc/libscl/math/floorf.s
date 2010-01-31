	AREA	|C$$code|, CODE, READONLY

	EXPORT	floorf
floorf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	RNDSM	f0, f0
	MOV	pc, lr

	END
