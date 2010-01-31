	AREA	|C$$code|, CODE, READONLY
	EXPORT	fegetenv
fegetenv
	RFS	ip
	STR	ip, [a1, #0]
	MOV	pc, lr

	END
