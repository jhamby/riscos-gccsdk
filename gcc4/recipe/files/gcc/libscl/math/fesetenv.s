	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetenv
fesetenv
	CMN	a1, #1 ; if envp == FE_DFL_ENV
	; set FPSR to FPU_DEFAULT
	MOVEQ	ip, #0
	LDRNE	ip, [a1, #0]
	WFS	ip
	MOV	pc, lr

	END
