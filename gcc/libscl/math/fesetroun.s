	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetround
fesetround
	; Only FE_TONEAREST is supported.
	CMP	a1, #0
	MOVNE	a1, #0
	MOVEQ	a1, #1
	MOV	pc, lr

	END
