	AREA	|C$$code|,CODE,READONLY

	EXPORT	modff
modff
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	RNDSZ	f1, f0
	STFS	f1, [a2, #0]
	SUFS	f0, f0, f1
	MOV	pc, lr

	END
