	AREA	|C$$code|, CODE, READONLY

	EXPORT	feraiseexcept
feraiseexcept
	RFS	ip
	AND	a1, a1, #15
	ORR	ip, ip, a1
	WFS	ip
	MOV	pc, lr
	END
