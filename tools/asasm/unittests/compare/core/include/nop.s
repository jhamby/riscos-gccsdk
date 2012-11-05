; Tests NOP

	AREA	Code, CODE

; NOP encodes differenctly when the chosen architecture is ARMv6K,
; ARMv6TE or ARMv7.  Also, for those architectures, we can use condition
; codes.
NewEnc	EQU	{ARCHITECTURE} = "6K" \
			:LOR: {ARCHITECTURE} = "6TE" \
			:LOR: {ARCHITECTURE} = "7-A" \
			:LOR: {ARCHITECTURE} = "7-R" \
			:LOR: {ARCHITECTURE} = "7E-M" \
			:LOR: {ARCHITECTURE} = "7-M"

	[ :LNOT: REFERENCE
		IF NewEnc
			INFO 0, "Testing new style NOP encoding"

			MACRO
			NopTest	$suffix

			LCLS	cc
			LCLA	ccIndex
			WHILE	ccIndex < 17
			; 17 condition codes to be tested:
			;   - Condition code NV is left out from this test.
			;   - HS and LO are equivalents for CS and CC.
cc			SETS	("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: 2*(17 - ccIndex)) :LEFT: 2
			IT $cc
			NOP$cc.$suffix
ccIndex			SETA	ccIndex + 1
			WEND
			IT AL
			NOP$suffix
			MEND

			NopTest		; ARM
			NopTest	.W	; ARM
			THUMB
			NopTest		; T
			NopTest	.N	; T
			NopTest	.W	; T2
		ELSE
			INFO 0, "Testing old style NOP encoding"

			NOP
			THUMB
			NOP
		ENDIF
	|
		IF NewEnc
			; ARM
			DCI &0320f000 	; nopeq
			DCI &1320f000 	; nopne
			DCI &2320f000 	; nopcs
			DCI &3320f000 	; nopcc
			DCI &4320f000 	; nopmi
			DCI &5320f000 	; noppl
			DCI &6320f000 	; nopvs
			DCI &7320f000 	; nopvc
			DCI &8320f000 	; nophi
			DCI &9320f000 	; nopls
			DCI &a320f000 	; nopge
			DCI &b320f000 	; noplt
			DCI &c320f000 	; nopgt
			DCI &d320f000 	; nople
			DCI &e320f000 	; nopal
			DCI &2320f000 	; nophs
			DCI &3320f000 	; noplo
			DCI &e320f000 	; nop

			; ARM
			DCI &0320f000 	; nopeq
			DCI &1320f000 	; nopne
			DCI &2320f000 	; nopcs
			DCI &3320f000 	; nopcc
			DCI &4320f000 	; nopmi
			DCI &5320f000 	; noppl
			DCI &6320f000 	; nopvs
			DCI &7320f000 	; nopvc
			DCI &8320f000 	; nophi
			DCI &9320f000 	; nopls
			DCI &a320f000 	; nopge
			DCI &b320f000 	; noplt
			DCI &c320f000 	; nopgt
			DCI &d320f000 	; nople
			DCI &e320f000 	; nopal
			DCI &2320f000 	; nophs
			DCI &3320f000 	; noplo
			DCI &e320f000 	; nop

			THUMB
			; Thumb
			DCI.N &bf08	; it	eq
			DCI.N &bf00	; nopeq
			DCI.N &bf18	; it	ne
			DCI.N &bf00	; nopne
			DCI.N &bf28	; it	cs
			DCI.N &bf00	; nopcs
			DCI.N &bf38	; it	cc
			DCI.N &bf00	; nopcc
			DCI.N &bf48	; it	mi
			DCI.N &bf00	; nopmi
			DCI.N &bf58	; it	pl
			DCI.N &bf00	; noppl
			DCI.N &bf68	; it	vs
			DCI.N &bf00	; nopvs
			DCI.N &bf78	; it	vc
			DCI.N &bf00	; nopvc
			DCI.N &bf88	; it	hi
			DCI.N &bf00	; nophi
			DCI.N &bf98	; it	ls
			DCI.N &bf00	; nopls
			DCI.N &bfa8	; it	ge
			DCI.N &bf00	; nopge
			DCI.N &bfb8	; it	lt
			DCI.N &bf00	; noplt
			DCI.N &bfc8	; it	gt
			DCI.N &bf00	; nopgt
			DCI.N &bfd8	; it	le
			DCI.N &bf00	; nople
			DCI.N &bfe8	; it	al
			DCI.N &bf00	; nop
			DCI.N &bf28	; it	hs
			DCI.N &bf00	; nophs
			DCI.N &bf38	; it	ls
			DCI.N &bf00	; nopls
			DCI.N &bfe8	; it	al
			DCI.N &bf00	; nop

			; Thumb
			DCI.N &bf08	; it	eq
			DCI.N &bf00	; nopeq
			DCI.N &bf18	; it	ne
			DCI.N &bf00	; nopne
			DCI.N &bf28	; it	cs
			DCI.N &bf00	; nopcs
			DCI.N &bf38	; it	cc
			DCI.N &bf00	; nopcc
			DCI.N &bf48	; it	mi
			DCI.N &bf00	; nopmi
			DCI.N &bf58	; it	pl
			DCI.N &bf00	; noppl
			DCI.N &bf68	; it	vs
			DCI.N &bf00	; nopvs
			DCI.N &bf78	; it	vc
			DCI.N &bf00	; nopvc
			DCI.N &bf88	; it	hi
			DCI.N &bf00	; nophi
			DCI.N &bf98	; it	ls
			DCI.N &bf00	; nopls
			DCI.N &bfa8	; it	ge
			DCI.N &bf00	; nopge
			DCI.N &bfb8	; it	lt
			DCI.N &bf00	; noplt
			DCI.N &bfc8	; it	gt
			DCI.N &bf00	; nopgt
			DCI.N &bfd8	; it	le
			DCI.N &bf00	; nople
			DCI.N &bfe8	; it	al
			DCI.N &bf00	; nop
			DCI.N &bf28	; it	hs
			DCI.N &bf00	; nophs
			DCI.N &bf38	; it	ls
			DCI.N &bf00	; nopls
			DCI.N &bfe8	; it	al
			DCI.N &bf00	; nop

			; Thumb-2
			DCI.N &bf08	; it	eq
			DCI.W &f3af8000	; nopeq
			DCI.N &bf18	; it	ne
			DCI.W &f3af8000	; nopne
			DCI.N &bf28	; it	cs
			DCI.W &f3af8000	; nopcs
			DCI.N &bf38	; it	cc
			DCI.W &f3af8000	; nopcc
			DCI.N &bf48	; it	mi
			DCI.W &f3af8000	; nopmi
			DCI.N &bf58	; it	pl
			DCI.W &f3af8000	; noppl
			DCI.N &bf68	; it	vs
			DCI.W &f3af8000	; nopvs
			DCI.N &bf78	; it	vc
			DCI.W &f3af8000	; nopvc
			DCI.N &bf88	; it	hi
			DCI.W &f3af8000	; nophi
			DCI.N &bf98	; it	ls
			DCI.W &f3af8000	; nopls
			DCI.N &bfa8	; it	ge
			DCI.W &f3af8000	; nopge
			DCI.N &bfb8	; it	lt
			DCI.W &f3af8000	; noplt
			DCI.N &bfc8	; it	gt
			DCI.W &f3af8000	; nopgt
			DCI.N &bfd8	; it	le
			DCI.W &f3af8000	; nople
			DCI.N &bfe8	; it	al
			DCI.W &f3af8000	; nop
			DCI.N &bf28	; it	hs
			DCI.W &f3af8000	; nophs
			DCI.N &bf38	; it	ls
			DCI.W &f3af8000	; nopls
			DCI.N &bfe8	; it	al
			DCI.W &f3af8000	; nop
		ELSE
			DCI &E1A00000

			THUMB
			DCI.N &46c0
		ENDIF
	]

	END
