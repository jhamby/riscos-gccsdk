	AREA	|C$$code|,CODE,READONLY

	; int finite (double x)
	; return 1 if x is finite, else 0
	EXPORT  finite
finite
	BIC	a1, a1, #&80000000
	ADD	a1, a1, #&100000
	ADD	a1, a1, #&80000000
	MOV	a1, a1, ASR #31
	MOV	pc, lr

	END
