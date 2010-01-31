	AREA	|C$$code|,CODE,READONLY

	EXPORT	sinl
sinl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	SIND	f0, f0
	MOV	pc, lr
	END
