; Tests BLX <register>, BX, BXJ.
; -RUNOPT: -CPU=7-A

		AREA Code5, CODE
	[ :LNOT: REFERENCE
	MACRO
	Inject	$i
	;INFO 0, "\t$i"
	$i
	MEND

	; <base>
	; <base><q1> (when <q1> is non-empty and we're not in ARM mode)
	; <base><q2>
	; <base><cond code>
	; <base><cond code><q1> (when <q1> is non-empty and we're not in ARM mode)
	; <base><cond code><q2>
	MACRO
	Invoke $base, $q1, $q2
	Inject	"$base R9"
	[ "$q1" <> "" :LAND: {CODESIZE} <> 32
	Inject	"$base.$q1 R9"
	]
	[ "$q2" <> ""
	Inject	"$base.$q2 R9"
	]

	LCLS	instr
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
	Inject	"$instr R9"
	[ "$q1" <> "" :LAND: {CODESIZE} <> 32
	Inject	"$instr.$q1 R9"
	]
	[ "$q2" <> ""
	Inject	"$instr.$q2 R9"
	]

cnt	SETA	cnt + 1
	WEND
	MEND

	ARM
	Invoke BXJ,, .W
	THUMB
	Invoke BXJ,, .W

	ARM
	Invoke BX,, .W
	THUMB
	Invoke BX, .N

	ARM
	Invoke BLX,,.W
	THUMB
	Invoke BLX,.N
	|
	; BXJ
	ARM
	DCI &e12fff29	; bxj	r9
	DCI &e12fff29	; bxj	r9
	DCI &012fff29	; bxjeq	r9
	DCI &012fff29	; bxjeq	r9
	DCI &112fff29	; bxjne	r9
	DCI &112fff29	; bxjne	r9
	DCI &212fff29	; bxjcs	r9
	DCI &212fff29	; bxjcs	r9
	DCI &312fff29	; bxjcc	r9
	DCI &312fff29	; bxjcc	r9
	DCI &412fff29	; bxjmi	r9
	DCI &412fff29	; bxjmi	r9
	DCI &512fff29	; bxjpl	r9
	DCI &512fff29	; bxjpl	r9
	DCI &612fff29	; bxjvs	r9
	DCI &612fff29	; bxjvs	r9
	DCI &712fff29	; bxjvc	r9
	DCI &712fff29	; bxjvc	r9
	DCI &812fff29	; bxjhi	r9
	DCI &812fff29	; bxjhi	r9
	DCI &912fff29	; bxjls	r9
	DCI &912fff29	; bxjls	r9
	DCI &a12fff29	; bxjge	r9
	DCI &a12fff29	; bxjge	r9
	DCI &b12fff29	; bxjlt	r9
	DCI &b12fff29	; bxjlt	r9
	DCI &c12fff29	; bxjgt	r9
	DCI &c12fff29	; bxjgt	r9
	DCI &d12fff29	; bxjle	r9
	DCI &d12fff29	; bxjle	r9
	DCI &e12fff29	; bxj	r9
	DCI &e12fff29	; bxj	r9
	DCI &212fff29	; bxjcs	r9
	DCI &212fff29	; bxjcs	r9
	DCI &312fff29	; bxjcc	r9
	DCI &312fff29	; bxjcc	r9

	THUMB
	DCI.W &f3c98f00	; bxj	r9
	DCI.W &f3c98f00	; bxj	r9
	DCI.N &bf08	; it	eq
	DCI.W &f3c98f00	; bxjeq	r9
	DCI.N &bf08	; it	eq
	DCI.W &f3c98f00	; bxjeq	r9
	DCI.N &bf18	; it	ne
	DCI.W &f3c98f00	; bxjne	r9
	DCI.N &bf18	; it	ne
	DCI.W &f3c98f00	; bxjne	r9
	DCI.N &bf28	; it	cs
	DCI.W &f3c98f00	; bxjcs	r9
	DCI.N &bf28	; it	cs
	DCI.W &f3c98f00	; bxjcs	r9
	DCI.N &bf38	; it	cc
	DCI.W &f3c98f00	; bxjcc	r9
	DCI.N &bf38	; it	cc
	DCI.W &f3c98f00	; bxjcc	r9
	DCI.N &bf48	; it	mi
	DCI.W &f3c98f00	; bxjmi	r9
	DCI.N &bf48	; it	mi
	DCI.W &f3c98f00	; bxjmi	r9
	DCI.N &bf58	; it	pl
	DCI.W &f3c98f00	; bxjpl	r9
	DCI.N &bf58	; it	pl
	DCI.W &f3c98f00	; bxjpl	r9
	DCI.N &bf68	; it	vs
	DCI.W &f3c98f00	; bxjvs	r9
	DCI.N &bf68	; it	vs
	DCI.W &f3c98f00	; bxjvs	r9
	DCI.N &bf78	; it	vc
	DCI.W &f3c98f00	; bxjvc	r9
	DCI.N &bf78	; it	vc
	DCI.W &f3c98f00	; bxjvc	r9
	DCI.N &bf88	; it	hi
	DCI.W &f3c98f00	; bxjhi	r9
	DCI.N &bf88	; it	hi
	DCI.W &f3c98f00	; bxjhi	r9
	DCI.N &bf98	; it	ls
	DCI.W &f3c98f00	; bxjls	r9
	DCI.N &bf98	; it	ls
	DCI.W &f3c98f00	; bxjls	r9
	DCI.N &bfa8	; it	ge
	DCI.W &f3c98f00	; bxjge	r9
	DCI.N &bfa8	; it	ge
	DCI.W &f3c98f00	; bxjge	r9
	DCI.N &bfb8	; it	lt
	DCI.W &f3c98f00	; bxjlt	r9
	DCI.N &bfb8	; it	lt
	DCI.W &f3c98f00	; bxjlt	r9
	DCI.N &bfc8	; it	gt
	DCI.W &f3c98f00	; bxjgt	r9
	DCI.N &bfc8	; it	gt
	DCI.W &f3c98f00	; bxjgt	r9
	DCI.N &bfd8	; it	le
	DCI.W &f3c98f00	; bxjle	r9
	DCI.N &bfd8	; it	le
	DCI.W &f3c98f00	; bxjle	r9
	DCI.W &f3c98f00	; bxj	r9
	DCI.W &f3c98f00	; bxj	r9
	DCI.N &bf28	; it	cs
	DCI.W &f3c98f00	; bxjcs	r9
	DCI.N &bf28	; it	cs
	DCI.W &f3c98f00	; bxjcs	r9
	DCI.N &bf38	; it	cc
	DCI.W &f3c98f00	; bxjcc	r9
	DCI.N &bf38	; it	cc
	DCI.W &f3c98f00	; bxjcc	r9

	; BX
	ARM
	DCI &e12fff19	; bx	r9
	DCI &e12fff19	; bx	r9
	DCI &012fff19	; bxeq	r9
	DCI &012fff19	; bxeq	r9
	DCI &112fff19	; bxne	r9
	DCI &112fff19	; bxne	r9
	DCI &212fff19	; bxcs	r9
	DCI &212fff19	; bxcs	r9
	DCI &312fff19	; bxcc	r9
	DCI &312fff19	; bxcc	r9
	DCI &412fff19	; bxmi	r9
	DCI &412fff19	; bxmi	r9
	DCI &512fff19	; bxpl	r9
	DCI &512fff19	; bxpl	r9
	DCI &612fff19	; bxvs	r9
	DCI &612fff19	; bxvs	r9
	DCI &712fff19	; bxvc	r9
	DCI &712fff19	; bxvc	r9
	DCI &812fff19	; bxhi	r9
	DCI &812fff19	; bxhi	r9
	DCI &912fff19	; bxls	r9
	DCI &912fff19	; bxls	r9
	DCI &a12fff19	; bxge	r9
	DCI &a12fff19	; bxge	r9
	DCI &b12fff19	; bxlt	r9
	DCI &b12fff19	; bxlt	r9
	DCI &c12fff19	; bxgt	r9
	DCI &c12fff19	; bxgt	r9
	DCI &d12fff19	; bxle	r9
	DCI &d12fff19	; bxle	r9
	DCI &e12fff19	; bx	r9
	DCI &e12fff19	; bx	r9
	DCI &212fff19	; bxcs	r9
	DCI &212fff19	; bxcs	r9
	DCI &312fff19	; bxcc	r9
	DCI &312fff19	; bxcc	r9

	THUMB
	DCI.N &4748	; bx	r9
	DCI.N &4748	; bx	r9
	DCI.N &bf08	; it	eq
	DCI.N &4748	; bxeq	r9
	DCI.N &bf08	; it	eq
	DCI.N &4748	; bxeq	r9
	DCI.N &bf18	; it	ne
	DCI.N &4748	; bxne	r9
	DCI.N &bf18	; it	ne
	DCI.N &4748	; bxne	r9
	DCI.N &bf28	; it	cs
	DCI.N &4748	; bxcs	r9
	DCI.N &bf28	; it	cs
	DCI.N &4748	; bxcs	r9
	DCI.N &bf38	; it	cc
	DCI.N &4748	; bxcc	r9
	DCI.N &bf38	; it	cc
	DCI.N &4748	; bxcc	r9
	DCI.N &bf48	; it	mi
	DCI.N &4748	; bxmi	r9
	DCI.N &bf48	; it	mi
	DCI.N &4748	; bxmi	r9
	DCI.N &bf58	; it	pl
	DCI.N &4748	; bxpl	r9
	DCI.N &bf58	; it	pl
	DCI.N &4748	; bxpl	r9
	DCI.N &bf68	; it	vs
	DCI.N &4748	; bxvs	r9
	DCI.N &bf68	; it	vs
	DCI.N &4748	; bxvs	r9
	DCI.N &bf78	; it	vc
	DCI.N &4748	; bxvc	r9
	DCI.N &bf78	; it	vc
	DCI.N &4748	; bxvc	r9
	DCI.N &bf88	; it	hi
	DCI.N &4748	; bxhi	r9
	DCI.N &bf88	; it	hi
	DCI.N &4748	; bxhi	r9
	DCI.N &bf98	; it	ls
	DCI.N &4748	; bxls	r9
	DCI.N &bf98	; it	ls
	DCI.N &4748	; bxls	r9
	DCI.N &bfa8	; it	ge
	DCI.N &4748	; bxge	r9
	DCI.N &bfa8	; it	ge
	DCI.N &4748	; bxge	r9
	DCI.N &bfb8	; it	lt
	DCI.N &4748	; bxlt	r9
	DCI.N &bfb8	; it	lt
	DCI.N &4748	; bxlt	r9
	DCI.N &bfc8	; it	gt
	DCI.N &4748	; bxgt	r9
	DCI.N &bfc8	; it	gt
	DCI.N &4748	; bxgt	r9
	DCI.N &bfd8	; it	le
	DCI.N &4748	; bxle	r9
	DCI.N &bfd8	; it	le
	DCI.N &4748	; bxle	r9
	DCI.N &4748	; bx	r9
	DCI.N &4748	; bx	r9
	DCI.N &bf28	; it	cs
	DCI.N &4748	; bxcs	r9
	DCI.N &bf28	; it	cs
	DCI.N &4748	; bxcs	r9
	DCI.N &bf38	; it	cc
	DCI.N &4748	; bxcc	r9
	DCI.N &bf38	; it	cc
	DCI.N &4748	; bxcc	r9

	; BLX
	ARM
	DCI &e12fff39	; blx	r9
	DCI &e12fff39	; blx	r9
	DCI &012fff39	; blxeq	r9
	DCI &012fff39	; blxeq	r9
	DCI &112fff39	; blxne	r9
	DCI &112fff39	; blxne	r9
	DCI &212fff39	; blxcs	r9
	DCI &212fff39	; blxcs	r9
	DCI &312fff39	; blxcc	r9
	DCI &312fff39	; blxcc	r9
	DCI &412fff39	; blxmi	r9
	DCI &412fff39	; blxmi	r9
	DCI &512fff39	; blxpl	r9
	DCI &512fff39	; blxpl	r9
	DCI &612fff39	; blxvs	r9
	DCI &612fff39	; blxvs	r9
	DCI &712fff39	; blxvc	r9
	DCI &712fff39	; blxvc	r9
	DCI &812fff39	; blxhi	r9
	DCI &812fff39	; blxhi	r9
	DCI &912fff39	; blxls	r9
	DCI &912fff39	; blxls	r9
	DCI &a12fff39	; blxge	r9
	DCI &a12fff39	; blxge	r9
	DCI &b12fff39	; blxlt	r9
	DCI &b12fff39	; blxlt	r9
	DCI &c12fff39	; blxgt	r9
	DCI &c12fff39	; blxgt	r9
	DCI &d12fff39	; blxle	r9
	DCI &d12fff39	; blxle	r9
	DCI &e12fff39	; blx	r9
	DCI &e12fff39	; blx	r9
	DCI &212fff39	; blxcs	r9
	DCI &212fff39	; blxcs	r9
	DCI &312fff39	; blxcc	r9
	DCI &312fff39	; blxcc	r9

	THUMB
	DCI.N &47c8	; blx	r9
	DCI.N &47c8	; blx	r9
	DCI.N &bf08	; it	eq
	DCI.N &47c8	; blxeq	r9
	DCI.N &bf08	; it	eq
	DCI.N &47c8	; blxeq	r9
	DCI.N &bf18	; it	ne
	DCI.N &47c8	; blxne	r9
	DCI.N &bf18	; it	ne
	DCI.N &47c8	; blxne	r9
	DCI.N &bf28	; it	cs
	DCI.N &47c8	; blxcs	r9
	DCI.N &bf28	; it	cs
	DCI.N &47c8	; blxcs	r9
	DCI.N &bf38	; it	cc
	DCI.N &47c8	; blxcc	r9
	DCI.N &bf38	; it	cc
	DCI.N &47c8	; blxcc	r9
	DCI.N &bf48	; it	mi
	DCI.N &47c8	; blxmi	r9
	DCI.N &bf48	; it	mi
	DCI.N &47c8	; blxmi	r9
	DCI.N &bf58	; it	pl
	DCI.N &47c8	; blxpl	r9
	DCI.N &bf58	; it	pl
	DCI.N &47c8	; blxpl	r9
	DCI.N &bf68	; it	vs
	DCI.N &47c8	; blxvs	r9
	DCI.N &bf68	; it	vs
	DCI.N &47c8	; blxvs	r9
	DCI.N &bf78	; it	vc
	DCI.N &47c8	; blxvc	r9
	DCI.N &bf78	; it	vc
	DCI.N &47c8	; blxvc	r9
	DCI.N &bf88	; it	hi
	DCI.N &47c8	; blxhi	r9
	DCI.N &bf88	; it	hi
	DCI.N &47c8	; blxhi	r9
	DCI.N &bf98	; it	ls
	DCI.N &47c8	; blxls	r9
	DCI.N &bf98	; it	ls
	DCI.N &47c8	; blxls	r9
	DCI.N &bfa8	; it	ge
	DCI.N &47c8	; blxge	r9
	DCI.N &bfa8	; it	ge
	DCI.N &47c8	; blxge	r9
	DCI.N &bfb8	; it	lt
	DCI.N &47c8	; blxlt	r9
	DCI.N &bfb8	; it	lt
	DCI.N &47c8	; blxlt	r9
	DCI.N &bfc8	; it	gt
	DCI.N &47c8	; blxgt	r9
	DCI.N &bfc8	; it	gt
	DCI.N &47c8	; blxgt	r9
	DCI.N &bfd8	; it	le
	DCI.N &47c8	; blxle	r9
	DCI.N &bfd8	; it	le
	DCI.N &47c8	; blxle	r9
	DCI.N &47c8	; blx	r9
	DCI.N &47c8	; blx	r9
	DCI.N &bf28	; it	cs
	DCI.N &47c8	; blxcs	r9
	DCI.N &bf28	; it	cs
	DCI.N &47c8	; blxcs	r9
	DCI.N &bf38	; it	cc
	DCI.N &47c8	; blxcc	r9
	DCI.N &bf38	; it	cc
	DCI.N &47c8	; blxcc	r9
	]

		END
