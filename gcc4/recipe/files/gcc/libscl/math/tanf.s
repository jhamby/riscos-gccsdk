	AREA	|C$$code|, CODE, READONLY

	EXPORT	tanf
tanf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	TANS	f0, f0
	MOV	pc, lr

	END
