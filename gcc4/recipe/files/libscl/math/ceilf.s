	AREA	|C$$code|, CODE, READONLY
	EXPORT	ceilf
ceilf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	RNDSP	f0, f0
	MOV	pc, lr

	END
