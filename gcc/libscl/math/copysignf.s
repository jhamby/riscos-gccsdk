	AREA	|C$$code|,CODE,READONLY

	; float copysign (float x, float y)
	; returns a value with the magnitude of x and with
	; the sign bit of y.
        EXPORT  copysignf
copysignf
	TST	a2, #&80000000
	ORRNE	a1, a1, #&80000000
	BICEQ	a1, a1, #&80000000
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MOV	pc, lr

	END
