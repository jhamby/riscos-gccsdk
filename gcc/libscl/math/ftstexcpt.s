	AREA	|C$$code|, CODE, READONLY

	EXPORT	fetestexcept
fetestexcept
	RFS	ip
	AND	ip, ip, a1
	AND	a1, ip, #15
	MOV	pc, lr

	END
