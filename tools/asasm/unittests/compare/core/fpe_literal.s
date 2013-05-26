; Tests FPE (FPA10/FPA11/FPASC) LDF literal support

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

		; Literals and symbols:
	[ :LNOT: REFERENCE
		LDFD	F1, =AFloat
		LDFD	F2, =AnInt
AFloat		*	3.14
AnInt		*	42
		LTORG
	|
		LDFD	F1, lit1
		LDFD	F2, lit2
lit1		DCFD	3.14
lit2		DCFD	42.
	]

		; Literals via LDC{2}{L} : coprocessor 1 is FPA
	[ :LNOT: REFERENCE
		LDC	p1, c1, =14.56	; Single precision
		LDC	p1, c10, =24.56	; Double precision
		LDCL	p1, c3, =34.56	; Extended precision
		LDCL	p1, c12, =44.56	; Packed precision
	|
		LDFS	F1, =14.56
		LDFD	F2, =24.56
		LDFE	F3, =34.56
		LDFP	F4, =44.56
	]

		END
