	AREA	|C$$code|,CODE,READONLY

	EXPORT	atan2l
atan2l
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	CMF	f0, #0
	CMFEQ	f1, #0
	POLNED	f0, f1, f0
	MOV	pc, lr

	END
