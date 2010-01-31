	AREA	|C$$code|, CODE, READONLY

	EXPORT	feclearexcept
feclearexcept
	RFS	ip
	AND	a1, a1, #15
	EOR	a1, a1, #15
	AND	ip, ip, a1
	WFS	ip
	MOV	pc, lr

	END
