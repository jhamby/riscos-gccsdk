; Tests FPE (FPA10/FPA11/FPASC) immediate support

	AREA	Code, CODE

	[ :LNOT:REFERENCE
	; Standard float values:
	MVFS	f1, #0.0
	MVFS	f1, #1.0
	MVFS	f1, #2.0
	MVFS	f1, #3.0
	MVFS	f1, #4.0
	MVFS	f1, #5.0
	MVFS	f1, #0.5
	MVFS	f1, #10.0

	; Without zero digit:
	MVFS	f1, #0.
	MVFS	f1, #1.
	MVFS	f1, #2.
	MVFS	f1, #3.
	MVFS	f1, #4.
	MVFS	f1, #5.
	MVFS	f1, #.5
	MVFS	f1, #10.

	; As integers:
	MVFS	f1, #0
	MVFS	f1, #1
	MVFS	f1, #2
	MVFS	f1, #3
	MVFS	f1, #4
	MVFS	f1, #5
	; MVFS	r1, #.5
	MVFS	f1, #10
	|
	; Standard float values:
	DCI	&EE009108	; MVFS    F1,#0.0
	DCI	&EE009109	; MVFS    F1,#1.0
	DCI	&EE00910A	; MVFS    F1,#2.0
	DCI	&EE00910B	; MVFS    F1,#3.0
	DCI	&EE00910C	; MVFS    F1,#4.0
	DCI	&EE00910D	; MVFS    F1,#5.0
	DCI	&EE00910E	; MVFS    F1,#0.5
	DCI	&EE00910F	; MVFS    F1,#10.0

	; Without zero digit:
	DCI	&EE009108	; MVFS    F1,#0.0
	DCI	&EE009109	; MVFS    F1,#1.0
	DCI	&EE00910A	; MVFS    F1,#2.0
	DCI	&EE00910B	; MVFS    F1,#3.0
	DCI	&EE00910C	; MVFS    F1,#4.0
	DCI	&EE00910D	; MVFS    F1,#5.0
	DCI	&EE00910E	; MVFS    F1,#0.5
	DCI	&EE00910F	; MVFS    F1,#10.0

	; As integers:
	DCI	&EE009108	; MVFS    F1,#0.0
	DCI	&EE009109	; MVFS    F1,#1.0
	DCI	&EE00910A	; MVFS    F1,#2.0
	DCI	&EE00910B	; MVFS    F1,#3.0
	DCI	&EE00910C	; MVFS    F1,#4.0
	DCI	&EE00910D	; MVFS    F1,#5.0
	; DCI	&EE00910E	; MVFS    F1,#0.5
	DCI	&EE00910F	; MVFS    F1,#10.0
	]

	; Test postive/negative immediate support:
	[ :LNOT: REFERENCE
	ADFS	f1, f2, #-4
	SUFS	f1, f2, #-4

	MVFS	f1, #-4
	MNFS	f1, #-4

	CMF	f1, #-4
	CNF	f1, #-4

	CMFE	f1, #-4
	CNFE	f1, #-4

	ABSS	f1, #-4
	|
	SUFS	f1, f2, #4
	ADFS	f1, f2, #4

	MNFS	f1, #4
	MVFS	f1, #4

	CNF	f1, #4
	CMF	f1, #4

	CNFE	f1, #4
	CMFE	f1, #4

	ABSS	f1, #4
	]

	END
