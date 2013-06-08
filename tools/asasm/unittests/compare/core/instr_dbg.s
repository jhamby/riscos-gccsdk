; Tests DBG
; RUNOPT: -CPU=7-A

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
	Invoke	DBG
	THUMB
	Invoke	DBG
	|
	ARM
	DCI &e320f0f9	; dbg	#9
	DCI &e320f0f9	; dbg	#9
	DCI &0320f0f9	; dbgeq	#9
	DCI &0320f0f9	; dbgeq	#9
	DCI &1320f0f9	; dbgne	#9
	DCI &1320f0f9	; dbgne	#9
	DCI &2320f0f9	; dbgcs	#9
	DCI &2320f0f9	; dbgcs	#9
	DCI &3320f0f9	; dbgcc	#9
	DCI &3320f0f9	; dbgcc	#9
	DCI &4320f0f9	; dbgmi	#9
	DCI &4320f0f9	; dbgmi	#9
	DCI &5320f0f9	; dbgpl	#9
	DCI &5320f0f9	; dbgpl	#9
	DCI &6320f0f9	; dbgvs	#9
	DCI &6320f0f9	; dbgvs	#9
	DCI &7320f0f9	; dbgvc	#9
	DCI &7320f0f9	; dbgvc	#9
	DCI &8320f0f9	; dbghi	#9
	DCI &8320f0f9	; dbghi	#9
	DCI &9320f0f9	; dbgls	#9
	DCI &9320f0f9	; dbgls	#9
	DCI &a320f0f9	; dbgge	#9
	DCI &a320f0f9	; dbgge	#9
	DCI &b320f0f9	; dbglt	#9
	DCI &b320f0f9	; dbglt	#9
	DCI &c320f0f9	; dbggt	#9
	DCI &c320f0f9	; dbggt	#9
	DCI &d320f0f9	; dbgle	#9
	DCI &d320f0f9	; dbgle	#9
	DCI &e320f0f9	; dbg	#9
	DCI &e320f0f9	; dbg	#9
	DCI &2320f0f9	; dbgcs	#9
	DCI &2320f0f9	; dbgcs	#9
	DCI &3320f0f9	; dbgcc	#9
	DCI &3320f0f9	; dbgcc	#9

	THUMB
	DCI.W &f3af80f9	; dbg	#9
	DCI.W &f3af80f9	; dbg	#9
	DCI.N &bf07	; ittee	eq
	DCI.W &f3af80f9	; dbgeq	#9
	DCI.W &f3af80f9	; dbgeq	#9
	DCI.W &f3af80f9	; dbgne	#9
	DCI.W &f3af80f9	; dbgne	#9
	DCI.N &bf27	; ittee	cs
	DCI.W &f3af80f9	; dbgcs	#9
	DCI.W &f3af80f9	; dbgcs	#9
	DCI.W &f3af80f9	; dbgcc	#9
	DCI.W &f3af80f9	; dbgcc	#9
	DCI.N &bf47	; ittee	mi
	DCI.W &f3af80f9	; dbgmi	#9
	DCI.W &f3af80f9	; dbgmi	#9
	DCI.W &f3af80f9	; dbgpl	#9
	DCI.W &f3af80f9	; dbgpl	#9
	DCI.N &bf67	; ittee	vs
	DCI.W &f3af80f9	; dbgvs	#9
	DCI.W &f3af80f9	; dbgvs	#9
	DCI.W &f3af80f9	; dbgvc	#9
	DCI.W &f3af80f9	; dbgvc	#9
	DCI.N &bf87	; ittee	hi
	DCI.W &f3af80f9	; dbghi	#9
	DCI.W &f3af80f9	; dbghi	#9
	DCI.W &f3af80f9	; dbgls	#9
	DCI.W &f3af80f9	; dbgls	#9
	DCI.N &bfa7	; ittee	ge
	DCI.W &f3af80f9	; dbgge	#9
	DCI.W &f3af80f9	; dbgge	#9
	DCI.W &f3af80f9	; dbglt	#9
	DCI.W &f3af80f9	; dbglt	#9
	DCI.N &bfc7	; ittee	gt
	DCI.W &f3af80f9	; dbggt	#9
	DCI.W &f3af80f9	; dbggt	#9
	DCI.W &f3af80f9	; dbgle	#9
	DCI.W &f3af80f9	; dbgle	#9
	DCI.W &f3af80f9	; dbg	#9
	DCI.W &f3af80f9	; dbg	#9
	DCI.N &bf27	; ittee	cs
	DCI.W &f3af80f9	; dbgcs	#9
	DCI.W &f3af80f9	; dbgcs	#9
	DCI.W &f3af80f9	; dbgcc	#9
	DCI.W &f3af80f9	; dbgcc	#9
	]

	;Check DBG has # optional.
	AREA	Code5, CODE
	ARM
	[ :LNOT: REFERENCE
		DBG	#9
		DBGNE	#15

		DBG	9
		DBGNE	15
	|
		DCI	0xe320f0f9
		DCI	0x1320f0ff

		DCI	0xe320f0f9
		DCI	0x1320f0ff
	]
	
	END
