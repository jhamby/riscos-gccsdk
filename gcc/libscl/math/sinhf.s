	AREA	|C$$code|, CODE, READONLY

	; return (exp(x) - exp(-x)) / 2
	EXPORT	sinhf
sinhf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1
	SUFS	f0, f0, f1
	DVFS	f0, f0, #2
	MOV	pc, lr

	END
