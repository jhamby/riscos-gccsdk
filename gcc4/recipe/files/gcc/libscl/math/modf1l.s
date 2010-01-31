	AREA	|C$$code|,CODE,READONLY

	EXPORT	modfl
modfl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDZ	f1, f0
	STFD	f1, [a3]
	SUFD	f0, f0, f1
	MOV	pc, lr

	END
