	AREA	|C$$code|, CODE, READONLY
	EXPORT	atan1
atan1
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	ATND	f0, f0
	MOV	pc, lr

	END
