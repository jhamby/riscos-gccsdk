; Tests IT
; RUNOPT: -cpu=7-A

	AREA	Code1, CODE
	[ :LNOT: REFERENCE

	MACRO
	Expand $numThenElse
	LCLA	thenElseIndex
	LCLS	cc
	LCLS	revcc
	WHILE	thenElseIndex < 1 :SHL: $numThenElse

	LCLS	itArg
	LCLA	itArgIndex
	WHILE	itArgIndex < $numThenElse
	[ thenElseIndex :AND: (1:SHL:itArgIndex) <> 0
itArg	SETS	itArg :CC: "E"
	|
itArg	SETS	itArg :CC: "T"
	]
itArgIndex SETA	itArgIndex + 1
	WEND

	LCLA	ccIndex
	WHILE	ccIndex < 16
	; 16 condition codes to be tested:
	;   - Condition codes NV and AL are left out from this test.
	;   - HS and LO are equivalents for CS and CC.
cc	SETS	("EQNECSCCMIPLVSVCHILSGELTGTLEHSLO" :RIGHT: 2*(16 - ccIndex)) :LEFT: 2
revcc	SETS	:REVERSE_CC: cc

	;INFO 0, "IT$itArg $cc"
	IT$itArg $cc
	;INFO 0, "NOP$cc"
	NOP$cc		; Then

	LCLA	itArgIndex
	WHILE	itArgIndex < $numThenElse
	[ thenElseIndex :AND: (1:SHL:itArgIndex) <> 0
	;INFO 0, "NOP$revcc"
	NOP$revcc	; Else
	|
	;INFO 0, "NOP$cc"
	NOP$cc		; Then
	]
itArgIndex SETA	itArgIndex + 1
	WEND

ccIndex	SETA	ccIndex + 1
	WEND	; ccIndex
thenElseIndex SETA thenElseIndex + 1
	WEND	; thenElseIndex
	MEND

	THUMB
	Expand 0	; IT <cc>
	Expand 1	; ITx <cc> (with x = T or E)
	Expand 2	; ITxx <cc>
	Expand 3	; ITxxx <cc>

	|

	THUMB
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
	DCI.N &bf28	; it	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf38	; it	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf04	; itt	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf1c	; itt	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf24	; itt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3c	; itt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf44	; itt	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf5c	; itt	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf64	; itt	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf7c	; itt	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf84	; itt	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf9c	; itt	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bfa4	; itt	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfbc	; itt	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfc4	; itt	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bfdc	; itt	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf24	; itt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3c	; itt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf0c	; ite	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf14	; ite	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf2c	; ite	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf34	; ite	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf4c	; ite	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf54	; ite	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf6c	; ite	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf74	; ite	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf8c	; ite	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf94	; ite	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bfac	; ite	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfb4	; ite	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfcc	; ite	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bfd4	; ite	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf2c	; ite	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf34	; ite	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf02	; ittt	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf1e	; ittt	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf22	; ittt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3e	; ittt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf42	; ittt	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf5e	; ittt	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf62	; ittt	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf7e	; ittt	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf82	; ittt	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf9e	; ittt	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bfa2	; ittt	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfbe	; ittt	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfc2	; ittt	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bfde	; ittt	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf22	; ittt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3e	; ittt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf0a	; itet	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf16	; itet	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf2a	; itet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf36	; itet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf4a	; itet	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf56	; itet	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf6a	; itet	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf76	; itet	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf8a	; itet	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf96	; itet	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bfaa	; itet	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfb6	; itet	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfca	; itet	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bfd6	; itet	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf2a	; itet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf36	; itet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf06	; itte	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf1a	; itte	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf26	; itte	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf3a	; itte	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf46	; itte	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf5a	; itte	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf66	; itte	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf7a	; itte	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf86	; itte	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf9a	; itte	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bfa6	; itte	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfba	; itte	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfc6	; itte	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bfda	; itte	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf26	; itte	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf3a	; itte	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf0e	; itee	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf12	; itee	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf2e	; itee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf32	; itee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf4e	; itee	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf52	; itee	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf6e	; itee	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf72	; itee	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf8e	; itee	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf92	; itee	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bfae	; itee	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfb2	; itee	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfce	; itee	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bfd2	; itee	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf2e	; itee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf32	; itee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf01	; itttt	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf1f	; itttt	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf21	; itttt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3f	; itttt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf41	; itttt	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf5f	; itttt	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf61	; itttt	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf7f	; itttt	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf81	; itttt	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf9f	; itttt	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bfa1	; itttt	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfbf	; itttt	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfc1	; itttt	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bfdf	; itttt	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf21	; itttt	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf3f	; itttt	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf09	; itett	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf17	; itett	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf29	; itett	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf37	; itett	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf49	; itett	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf57	; itett	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf69	; itett	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf77	; itett	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf89	; itett	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf97	; itett	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bfa9	; itett	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfb7	; itett	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfc9	; itett	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bfd7	; itett	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf29	; itett	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf37	; itett	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf05	; ittet	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf1b	; ittet	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf25	; ittet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf3b	; ittet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf45	; ittet	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf5b	; ittet	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf65	; ittet	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf7b	; ittet	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf85	; ittet	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf9b	; ittet	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bfa5	; ittet	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfbb	; ittet	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfc5	; ittet	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bfdb	; ittet	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf25	; ittet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf3b	; ittet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf0d	; iteet	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf13	; iteet	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf2d	; iteet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf33	; iteet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf4d	; iteet	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf53	; iteet	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf6d	; iteet	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf73	; iteet	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf8d	; iteet	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf93	; iteet	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bfad	; iteet	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfb3	; iteet	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfcd	; iteet	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bfd3	; iteet	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf2d	; iteet	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf33	; iteet	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf03	; ittte	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf1d	; ittte	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf23	; ittte	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf3d	; ittte	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf43	; ittte	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf5d	; ittte	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf63	; ittte	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf7d	; ittte	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf83	; ittte	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf9d	; ittte	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bfa3	; ittte	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfbd	; ittte	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfc3	; ittte	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bfdd	; ittte	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf23	; ittte	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf3d	; ittte	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf0b	; itete	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf15	; itete	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf2b	; itete	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf35	; itete	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf4b	; itete	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf55	; itete	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf6b	; itete	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf75	; itete	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf8b	; itete	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf95	; itete	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bfab	; itete	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bfb5	; itete	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bfcb	; itete	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bfd5	; itete	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf2b	; itete	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf35	; itete	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf07	; ittee	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf19	; ittee	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf27	; ittee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf39	; ittee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf47	; ittee	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf59	; ittee	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf67	; ittee	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf79	; ittee	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf87	; ittee	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf99	; ittee	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bfa7	; ittee	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfb9	; ittee	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfc7	; ittee	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bfd9	; ittee	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf27	; ittee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf39	; ittee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf0f	; iteee	eq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopne
	DCI.N &bf11	; iteee	ne
	DCI.N &bf00	; nopne
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf00	; nopeq
	DCI.N &bf2f	; iteee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf31	; iteee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf4f	; iteee	mi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; noppl
	DCI.N &bf51	; iteee	pl
	DCI.N &bf00	; noppl
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf00	; nopmi
	DCI.N &bf6f	; iteee	vs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvc
	DCI.N &bf71	; iteee	vc
	DCI.N &bf00	; nopvc
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf00	; nopvs
	DCI.N &bf8f	; iteee	hi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nopls
	DCI.N &bf91	; iteee	ls
	DCI.N &bf00	; nopls
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bf00	; nophi
	DCI.N &bfaf	; iteee	ge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; noplt
	DCI.N &bfb1	; iteee	lt
	DCI.N &bf00	; noplt
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bf00	; nopge
	DCI.N &bfcf	; iteee	gt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bf00	; nople
	DCI.N &bfd1	; iteee	le
	DCI.N &bf00	; nople
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf00	; nopgt
	DCI.N &bf2f	; iteee	cs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcc
	DCI.N &bf31	; iteee	cc
	DCI.N &bf00	; nopcc
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs
	DCI.N &bf00	; nopcs

	]

	; Check implicit IT block generation.
	AREA	Code2, CODE
	THUMB
	[ :LNOT: REFERENCE
	NOPEQ
	NOPNE
	NOPEQ
	NOPEQ
	NOPNE
	NOPNE
	NOPEQ
	NOPCC
	NOPVS
	NOP
	|
	ITETT	EQ
	NOPEQ
	NOPNE
	NOPEQ
	NOPEQ
	ITTE	NE
	NOPNE
	NOPNE
	NOPEQ
	IT	CC
	NOPCC
	IT	VS
	NOPVS
	NOP
	]

	END
