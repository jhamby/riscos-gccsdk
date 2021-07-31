; Tests SMC/SMI
; RUNOPT: -CPU=7-A.security


	AREA	Code3, CODE
	[ :LNOT: REFERENCE
	MACRO
	Inject	$i
	;INFO 0, "\t$i"
	$i
	MEND

	; <base>
	; <base> <cond code>
	MACRO
	Invoke $base
	Inject	"$base #9"
	Inject	"$base..W #9"

	LCLS	instr
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
	Inject	"$instr #9"
	Inject	"$instr..W #9"
cnt	SETA	cnt + 1
	WEND
	MEND

	ARM
	Invoke	SMI
	Invoke	SMC
	THUMB
	Invoke	SMC
	|
	ARM
	DCI &e1600079	; smc	9
	DCI &e1600079	; smc	9
	DCI &01600079	; smceq	9
	DCI &01600079	; smceq	9
	DCI &11600079	; smcne	9
	DCI &11600079	; smcne	9
	DCI &21600079	; smccs	9
	DCI &21600079	; smccs	9
	DCI &31600079	; smccc	9
	DCI &31600079	; smccc	9
	DCI &41600079	; smcmi	9
	DCI &41600079	; smcmi	9
	DCI &51600079	; smcpl	9
	DCI &51600079	; smcpl	9
	DCI &61600079	; smcvs	9
	DCI &61600079	; smcvs	9
	DCI &71600079	; smcvc	9
	DCI &71600079	; smcvc	9
	DCI &81600079	; smchi	9
	DCI &81600079	; smchi	9
	DCI &91600079	; smcls	9
	DCI &91600079	; smcls	9
	DCI &a1600079	; smcge	9
	DCI &a1600079	; smcge	9
	DCI &b1600079	; smclt	9
	DCI &b1600079	; smclt	9
	DCI &c1600079	; smcgt	9
	DCI &c1600079	; smcgt	9
	DCI &d1600079	; smcle	9
	DCI &d1600079	; smcle	9
	DCI &e1600079	; smc	9
	DCI &e1600079	; smc	9
	DCI &21600079	; smccs	9
	DCI &21600079	; smccs	9
	DCI &31600079	; smccc	9
	DCI &31600079	; smccc	9

	DCI &e1600079	; smc	9
	DCI &e1600079	; smc	9
	DCI &01600079	; smceq	9
	DCI &01600079	; smceq	9
	DCI &11600079	; smcne	9
	DCI &11600079	; smcne	9
	DCI &21600079	; smccs	9
	DCI &21600079	; smccs	9
	DCI &31600079	; smccc	9
	DCI &31600079	; smccc	9
	DCI &41600079	; smcmi	9
	DCI &41600079	; smcmi	9
	DCI &51600079	; smcpl	9
	DCI &51600079	; smcpl	9
	DCI &61600079	; smcvs	9
	DCI &61600079	; smcvs	9
	DCI &71600079	; smcvc	9
	DCI &71600079	; smcvc	9
	DCI &81600079	; smchi	9
	DCI &81600079	; smchi	9
	DCI &91600079	; smcls	9
	DCI &91600079	; smcls	9
	DCI &a1600079	; smcge	9
	DCI &a1600079	; smcge	9
	DCI &b1600079	; smclt	9
	DCI &b1600079	; smclt	9
	DCI &c1600079	; smcgt	9
	DCI &c1600079	; smcgt	9
	DCI &d1600079	; smcle	9
	DCI &d1600079	; smcle	9
	DCI &e1600079	; smc	9
	DCI &e1600079	; smc	9
	DCI &21600079	; smccs	9
	DCI &21600079	; smccs	9
	DCI &31600079	; smccc	9
	DCI &31600079	; smccc	9

	THUMB
	DCI.W &f7f98000	; smc	#9
	DCI.W &f7f98000	; smc	#9
	DCI.N &bf08	; it	eq
	DCI.W &f7f98000	; smceq	#9
	DCI.N &bf08	; it	eq
	DCI.W &f7f98000	; smceq	#9
	DCI.N &bf18	; it	ne
	DCI.W &f7f98000	; smcne	#9
	DCI.N &bf18	; it	ne
	DCI.W &f7f98000	; smcne	#9
	DCI.N &bf28	; it	cs
	DCI.W &f7f98000	; smccs	#9
	DCI.N &bf28	; it	cs
	DCI.W &f7f98000	; smccs	#9
	DCI.N &bf38	; it	cc
	DCI.W &f7f98000	; smccc	#9
	DCI.N &bf38	; it	cc
	DCI.W &f7f98000	; smccc	#9
	DCI.N &bf48	; it	mi
	DCI.W &f7f98000	; smcmi	#9
	DCI.N &bf48	; it	mi
	DCI.W &f7f98000	; smcmi	#9
	DCI.N &bf58	; it	pl
	DCI.W &f7f98000	; smcpl	#9
	DCI.N &bf58	; it	pl
	DCI.W &f7f98000	; smcpl	#9
	DCI.N &bf68	; it	vs
	DCI.W &f7f98000	; smcvs	#9
	DCI.N &bf68	; it	vs
	DCI.W &f7f98000	; smcvs	#9
	DCI.N &bf78	; it	vc
	DCI.W &f7f98000	; smcvc	#9
	DCI.N &bf78	; it	vc
	DCI.W &f7f98000	; smcvc	#9
	DCI.N &bf88	; it	hi
	DCI.W &f7f98000	; smchi	#9
	DCI.N &bf88	; it	hi
	DCI.W &f7f98000	; smchi	#9
	DCI.N &bf98	; it	ls
	DCI.W &f7f98000	; smcls	#9
	DCI.N &bf98	; it	ls
	DCI.W &f7f98000	; smcls	#9
	DCI.N &bfa8	; it	ge
	DCI.W &f7f98000	; smcge	#9
	DCI.N &bfa8	; it	ge
	DCI.W &f7f98000	; smcge	#9
	DCI.N &bfb8	; it	lt
	DCI.W &f7f98000	; smclt	#9
	DCI.N &bfb8	; it	lt
	DCI.W &f7f98000	; smclt	#9
	DCI.N &bfc8	; it	gt
	DCI.W &f7f98000	; smcgt	#9
	DCI.N &bfc8	; it	gt
	DCI.W &f7f98000	; smcgt	#9
	DCI.N &bfd8	; it	le
	DCI.W &f7f98000	; smcle	#9
	DCI.N &bfd8	; it	le
	DCI.W &f7f98000	; smcle	#9
	DCI.W &f7f98000	; smc	#9
	DCI.W &f7f98000	; smc	#9
	DCI.N &bf28	; it	cs
	DCI.W &f7f98000	; smccs	#9
	DCI.N &bf28	; it	cs
	DCI.W &f7f98000	; smccs	#9
	DCI.N &bf38	; it	cc
	DCI.W &f7f98000	; smccc	#9
	DCI.N &bf38	; it	cc
	DCI.W &f7f98000	; smccc	#9
	]

	; Check SMC/SMI have # optional.
	AREA	Code4, CODE
	ARM
	[ :LNOT: REFERENCE
		SMC	#5
		SMCNE	#15
		SMI	#1
		SMINE	#14

		SMC	5
		SMCNE	15
		SMI	1
		SMINE	14
	|
		DCI	0xe1600075
		DCI	0x1160007f
		DCI	0xe1600071
		DCI	0x1160007e

		DCI	0xe1600075
		DCI	0x1160007f
		DCI	0xe1600071
		DCI	0x1160007e
	]

	; Check SMC is unrecognized in pre-UAL only mode.
	; Check SMI is unrecognized in UAL only mode.
	AREA	Code5, CODE
	[ :LNOT: REFERENCE
	MACRO
	SMC$cc  $val
	NOP
	MEND

	MACRO
	SMI$cc  $val
	NOP
	MEND

	CODE16
	SMC	#5
	SMCCC	5
	SMI	6

	THUMB
	SMI	#5
	SMICC	5
	SMC	6
	|
	CODE16
	NOP
	NOP
	DCI &f7f6
	DCI &8000
	THUMB
	NOP
	NOP
	DCI.W &f7f68000
	]

	AREA Code6, CODE
	THUMB
	[ :LNOT: REFERENCE
	SMCCC #5	; Forces the end of the implicit IT block.
	SWICC #5
	|
	DCI.N &bf38	; it	cc
	DCI.W &f7f58000	; smccc	#5
	DCI.N &bf38	; it	cc
	DCI.N &df05	; svccc	5
	]

	END