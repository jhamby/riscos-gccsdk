	AREA	|C$$code|,CODE,READONLY

	; double copysign (double x, double y)
	; returns a value with the magnitude of x and with
	; the sign bit of y.
        EXPORT  copysign
copysign
	TST	a3, #&80000000
	ORRNE	a1, a1, #&80000000
	BICEQ	a1, a1, #&80000000
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MOV	pc, lr

	END
