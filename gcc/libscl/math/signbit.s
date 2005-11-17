	AREA	|C$$code|, CODE, READONLY

	EXPORT	signbit
	EXPORT	__signbitf
	EXPORT	__signbitd
signbit
__signbitf
__signbitd
	AND	a1, a1, #&80000000
	MOV	pc, lr

	END
