	AREA	|C$$code|, CODE, READONLY
	EXPORT	coshf
coshf
	; return (exp(x) + exp(-x)) / 2
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1
	ADFS	f0, f0, f1
	DVFS	f0, f0, #2
	MOV	pc, lr

	END
