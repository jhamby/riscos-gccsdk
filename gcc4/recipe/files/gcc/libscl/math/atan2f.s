	AREA	|C$$code|, CODE, READONLY

	EXPORT	atan2f
atan2f
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	CMF	f0, #0
	CMFEQ	f1, #0
	POLNES	f0, f1, f0
	MOV	pc, lr

	END
