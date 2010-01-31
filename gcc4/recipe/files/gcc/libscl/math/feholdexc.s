	AREA	|C$$code|, CODE, READONLY

	EXPORT	feholdexcept
feholdexcept
	RFS	ip
	STR	ip, [a1, #0]
	; ip &= ~(FE_ALL_EXCEPT << FE_EXCEPT_SHIFT)
	BIC	ip, ip, #&F0000
	WFS	ip
	MOV	a1, #1
	MOV	pc, lr
	END
