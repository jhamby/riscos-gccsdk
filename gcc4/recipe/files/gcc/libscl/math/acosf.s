	AREA	|C$$code|, CODE, READONLY
	EXPORT	acosf
acosf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	ACSS	f0, f0
	MOV	pc, lr

	END
