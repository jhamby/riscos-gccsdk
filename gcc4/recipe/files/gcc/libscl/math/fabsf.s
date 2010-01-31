	AREA	|C$$code|, CODE, READONLY

	EXPORT  fabsf
fabsf
	BIC	a1, a1, #&80000000
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MOV	pc, lr

	END
