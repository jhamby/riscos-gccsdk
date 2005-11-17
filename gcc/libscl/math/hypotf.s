	AREA	|C$$code|, CODE, READONLY

	; return sqrt (x*x + y*y)
	EXPORT	hypotf
hypotf
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	MUFS	f0, f0, f0
	MUFS	f1, f1, f1
	ADFS	f0, f0, f1
	SQTS	f0, f0
	MOV	pc, lr

	END
