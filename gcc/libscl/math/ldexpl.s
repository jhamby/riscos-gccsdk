	AREA	|C$$code|, CODE, READONLY

	; scalb is the BSD name for ldexp
	EXPORT	scalb
	EXPORT	ldexpl
scalb
ldexpl
	; return x*pow(2, y)
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	FLTD	f1, a3
	MVFD	f2, #2
	POWD	f3, f2, f1
	MUFD	f0, f0, f3
	MOV	pc, lr

	END
