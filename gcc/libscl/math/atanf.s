	AREA	|C$$code|, CODE, READONLY

	EXPORT	atanf
atanf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	ATNS	f0, f0
	MOV	pc, lr

	END
