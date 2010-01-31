	AREA	|C$$code|, CODE, READONLY

	EXPORT	fegetround
fegetround
	; The default rounding mode cannot be set on the ARM FPU without
	; handcoding the rounding mode into the instruction. We therefore
	; always default to round-to-nearest.
	MOV	a1, #0
	MOV	pc, lr

	END
