	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	EXPORT	tanhf
tanhf
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1

	SUFS	f2, f0, f1
	ADFS	f3, f0, f1
	DVFS	f0, f2, f3
	MOV	pc, lr

	END
