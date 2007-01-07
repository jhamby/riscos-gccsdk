	AREA	|C$$code|, CODE, READONLY

	EXPORT	log10l
log10l
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	LOGD	f0, f0
	MOV	pc, lr

	END
