	AREA	|C$$code|, CODE, READONLY

	EXPORT  isinf
isinf
	BIC	a1, a1, #&80000000
	EOR	a1, a1, #&70000000
	EOR	a1, a1, #&0ff00000
	ORRS	a2, a1, a2
	MOVNE	a1, #0
	MOVEQ	a1, #1
	MOV	pc, lr

	END
