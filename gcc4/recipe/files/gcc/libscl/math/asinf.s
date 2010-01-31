	AREA	|C$$code|, CODE, READONLY

	EXPORT	asinf
asinf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	ASNS	f0, f0
	MOV	pc, lr

	END
