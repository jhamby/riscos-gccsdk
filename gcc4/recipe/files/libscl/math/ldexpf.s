	AREA	|C$$code|, CODE, READONLY

	; scalb is the BSD name for ldexp
	EXPORT	ldexpf
ldexpf
	; return x*pow(2, y)
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	FLTS	f1, a2
	MVFS	f2, #2
	POWS	f3, f2, f1
	MUFS	f0, f0, f3
	MOV	pc, lr

	END
