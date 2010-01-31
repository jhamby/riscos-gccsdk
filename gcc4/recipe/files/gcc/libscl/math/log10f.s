	AREA	|C$$code|, CODE, READONLY

	EXPORT	log10f
log10f
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	LOGS	f0, f0
	MOV	pc, lr

	END
