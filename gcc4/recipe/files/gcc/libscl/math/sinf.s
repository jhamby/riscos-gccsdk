	AREA	|C$$code|, CODE, READONLY

	EXPORT	sinf
sinf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	SINS	f0, f0
	MOV	pc, lr

	END
