; FPE (FPA10/FPA11/FPASC) memory access : LDF/STF/LFM/SFM
; Option autocast is specified as we want to test the integer -> float
; conversions.
; RUNOPT: -autocast

		AREA |C$$code|, CODE, READONLY

		; Literal test : simple case
	[ :LNOT: REFERENCE
		LDFS	F1,=1234.
		LDFD	F1,=1234.
		LDFE	F1,=1234.
		LDFP	F1,=1234.
		LTORG

		; Same constants (even as integers) will be re-used.
		LDFS	F1,=1234
		LDFD	F1,=1234
		LDFE	F1,=1234
		LDFP	F1,=1234
		LTORG
	|
		LDFS	F1, dump1_single
		LDFD	F1, dump1_double
		LDFD	F1, dump1_double
		LDFD	F1, dump1_double
dump1_single	DCFS	1234.
dump1_double	DCFD	1234.

		LDFS	F1, dump1_single
		LDFD	F1, dump1_double
		LDFD	F1, dump1_double
		LDFD	F1, dump1_double
	]

		; Literal test : LDF -> MVF/MNF
	[ :LNOT: REFERENCE
		LDFS	F1, =1.
		LDFD	F2, =2.
		LDFE	F3, =4.

		LDFS	F1, =-1.
		LDFD	F2, =2.-4.
		LDFE	F3, =-1.*4.
	|
		MVFS	F1, #1.
		MVFD	F2, #2.
		MVFD	F3, #4.

		MNFS	F1, #1.
		MNFD	F2, #2.
		MNFD	F3, #4.
	]

		END
