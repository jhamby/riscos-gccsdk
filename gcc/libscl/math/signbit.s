	AREA	|C$$code|, CODE, READONLY

	EXPORT	signbit
signbit
	AND	a1, a1, #&80000000
	MOV	pc, lr

	END
