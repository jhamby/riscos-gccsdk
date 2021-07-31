; Tests CPS
; RUNOPT: -CPU 6T2

	AREA	Code1, CODE
	[ :LNOT: REFERENCE
	MACRO
	CPS$ext $var1, $var2
	NOP
	MEND

	ARM
	CPSCC	ia	; Calls macro CPS*
	CPSCC	#9	; Calls macro CPS*
	CPSCC.W	ia, #9	; Calls macro CPS*

	CPS	#17
	CPSID	aI ,  #17
	CPSIE.W	f
	CPSIE.W	if ,#17
	CPSID.W		; UNPREDICTABLE
	CPSIE.W		; UNPREDICTABLE
	CPSID	, #17	; UNPREDICTABLE
	CPSIE.W	, # 17	; UNPREDICTABLE

	THUMB
	CPSCC	ia	; Calls macro CPS*
	CPSCC	#9	; Calls macro CPS*
	CPSCC.W	ia, #9	; Calls macro CPS*

	; 16-bit Thumb
	CPSIE	a
	CPSID	iA
	CPSIE	FaI
	CPSID		; UNPREDICTABLE
	CPSIE		; UNPREDICTABLE

	; 32-bit Thumb
	CPS	#17
	CPSID	aI ,  #17
	CPSIE.W	f
	CPSIE.W	if ,#17
	CPSID.W		; UNPREDICTABLE
	CPSIE.W		; UNPREDICTABLE
	CPSID	, #17	; UNPREDICTABLE
	CPSIE.W	, # 17	; UNPREDICTABLE

	; Following should generate two IT blocks:
	DBGCC	#9
	CPSID	i
	DBGCS	#9

	|

	ARM
	NOP
	NOP
	NOP

	DCI.W &f1020011	; cps	#17
	DCI.W &f10e0191	; cpsid	ai,#17
	DCI.W &f1080040	; cpsie	f
	DCI.W &f10a00d1	; cpsie	if,#17
	DCI.W &f10c0000	; cpsid
	DCI.W &f1080000	; cpsie
	DCI.W &f10e0011	; cpsid	,#17
	DCI.W &f10a0011	; cpsie	,#17

	THUMB
	NOP
	NOP
	NOP

	; 16-bit Thumb
	DCI.N &b664	; cpsie	a
	DCI.N &b676	; cpsid	ai
	DCI.N &b667	; cpsie	aif
	DCI.N &b670	; cpsid
	DCI.N &b660	; cpsie

	; 32-bit Thumb
	DCI.W &f3af8111	; cps	#17
	DCI.W &f3af87d1	; cpsid	ai, #17
	DCI.W &f3af8420	; cpsie.w	f
	DCI.W &f3af8571	; cpsie	if, #17
	DCI.W &f3af8600	; cpsid.w
	DCI.W &f3af8400	; cpsie.w
	DCI.W &f3af8711	; cpsid	, #17
	DCI.W &f3af8511	; cpsie	, #17

	; Following should generate two IT blocks:
	DCI.N &bf38	; it	cc
	DCI.W &f3af80f9	; dbgcc	#9
	DCI.N &b672	; cpsid	i
	DCI.N &bf28	; it	cs
	DCI.W &f3af80f9	; dbgcs	#9

	]

	END
