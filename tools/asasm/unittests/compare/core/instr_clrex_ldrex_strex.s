; Tests CLREX, LDREX, LDREXB, LDREXH, LDREXD, STREX, STREXB, STREXH, STREXD.
; -RUNOPT: -CPU=7-A

	AREA	Code, CODE, HALFWORD

	[ :LNOT: REFERENCE
	MACRO
	Invoke $base, $addr
	LCLS	instr
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr	SETS	"$base" :CC: (("EQNECSHSCCLOMIPLVSVCHILSGELTGTLEAL" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
	$instr	$addr
cnt	SETA	cnt + 1
	WEND
	$base	$addr
	MEND

	LDREX	r2, [r4]
	LDREXB	r2, [r4]
	LDREXH	r2, [r4]
	LDREXD	r2, r3, [r4]

	STREX	r1, r2, [r4]
	STREXB	r1, r2, [r4]
	STREXH	r1, r2, [r4]
	STREXD	r1, r2, r3, [r4]

	Invoke	LDREX, "r4, [r6]"
	Invoke	LDREXB, "r4, [r6]"
	Invoke	LDREXH, "r4, [r6]"
	Invoke	LDREXD, "r4, r5, [r6]"

	Invoke	STREX, "r1, r4, [r6]"
	Invoke	STREXB, "r1, r4, [r6]"
	Invoke	STREXH, "r1, r4, [r6]"
	Invoke	STREXD, "r1, r4, r5, [r6]"

	|

	DCI &e1942f9f 	; ldrex		r2, [r4]
	DCI &e1d42f9f 	; ldrexb	r2, [r4]
	DCI &e1f42f9f 	; ldrexh	r2, [r4]
	DCI &e1b42f9f 	; ldrexd	r2, [r4]

	DCI &e1841f92 	; strex		r1, r2, [r4]
	DCI &e1c41f92 	; strexb	r1, r2, [r4]
	DCI &e1e41f92 	; strexh	r1, r2, [r4]
	DCI &e1a41f92 	; strexd	r1, r2, [r4]

	DCI &01964f9f 	; ldrexeq	r4, [r6]
	DCI &11964f9f 	; ldrexne	r4, [r6]
	DCI &21964f9f 	; ldrexcs	r4, [r6]
	DCI &21964f9f 	; ldrexcs	r4, [r6]
	DCI &31964f9f 	; ldrexcc	r4, [r6]
	DCI &31964f9f 	; ldrexcc	r4, [r6]
	DCI &41964f9f 	; ldrexmi	r4, [r6]
	DCI &51964f9f 	; ldrexpl	r4, [r6]
	DCI &61964f9f 	; ldrexvs	r4, [r6]
	DCI &71964f9f 	; ldrexvc	r4, [r6]
	DCI &81964f9f 	; ldrexhi	r4, [r6]
	DCI &91964f9f 	; ldrexls	r4, [r6]
	DCI &a1964f9f 	; ldrexge	r4, [r6]
	DCI &b1964f9f 	; ldrexlt	r4, [r6]
	DCI &c1964f9f 	; ldrexgt	r4, [r6]
	DCI &d1964f9f 	; ldrexle	r4, [r6]
	DCI &e1964f9f 	; ldrex		r4, [r6]
	DCI &e1964f9f 	; ldrex		r4, [r6]

	DCI &01d64f9f 	; ldrexbeq	r4, [r6]
	DCI &11d64f9f 	; ldrexbne	r4, [r6]
	DCI &21d64f9f 	; ldrexbcs	r4, [r6]
	DCI &21d64f9f 	; ldrexbcs	r4, [r6]
	DCI &31d64f9f 	; ldrexbcc	r4, [r6]
	DCI &31d64f9f 	; ldrexbcc	r4, [r6]
	DCI &41d64f9f 	; ldrexbmi	r4, [r6]
	DCI &51d64f9f 	; ldrexbpl	r4, [r6]
	DCI &61d64f9f 	; ldrexbvs	r4, [r6]
	DCI &71d64f9f 	; ldrexbvc	r4, [r6]
	DCI &81d64f9f 	; ldrexbhi	r4, [r6]
	DCI &91d64f9f 	; ldrexbls	r4, [r6]
	DCI &a1d64f9f 	; ldrexbge	r4, [r6]
	DCI &b1d64f9f 	; ldrexblt	r4, [r6]
	DCI &c1d64f9f 	; ldrexbgt	r4, [r6]
	DCI &d1d64f9f 	; ldrexble	r4, [r6]
	DCI &e1d64f9f 	; ldrexb	r4, [r6]
	DCI &e1d64f9f 	; ldrexb	r4, [r6]

	DCI &01f64f9f 	; ldrexheq	r4, [r6]
	DCI &11f64f9f 	; ldrexhne	r4, [r6]
	DCI &21f64f9f 	; ldrexhcs	r4, [r6]
	DCI &21f64f9f 	; ldrexhcs	r4, [r6]
	DCI &31f64f9f 	; ldrexhcc	r4, [r6]
	DCI &31f64f9f 	; ldrexhcc	r4, [r6]
	DCI &41f64f9f 	; ldrexhmi	r4, [r6]
	DCI &51f64f9f 	; ldrexhpl	r4, [r6]
	DCI &61f64f9f 	; ldrexhvs	r4, [r6]
	DCI &71f64f9f 	; ldrexhvc	r4, [r6]
	DCI &81f64f9f 	; ldrexhhi	r4, [r6]
	DCI &91f64f9f 	; ldrexhls	r4, [r6]
	DCI &a1f64f9f 	; ldrexhge	r4, [r6]
	DCI &b1f64f9f 	; ldrexhlt	r4, [r6]
	DCI &c1f64f9f 	; ldrexhgt	r4, [r6]
	DCI &d1f64f9f 	; ldrexhle	r4, [r6]
	DCI &e1f64f9f 	; ldrexh	r4, [r6]
	DCI &e1f64f9f 	; ldrexh	r4, [r6]

	DCI &01b64f9f 	; ldrexdeq	r4, [r6]
	DCI &11b64f9f 	; ldrexdne	r4, [r6]
	DCI &21b64f9f 	; ldrexdcs	r4, [r6]
	DCI &21b64f9f 	; ldrexdcs	r4, [r6]
	DCI &31b64f9f 	; ldrexdcc	r4, [r6]
	DCI &31b64f9f 	; ldrexdcc	r4, [r6]
	DCI &41b64f9f 	; ldrexdmi	r4, [r6]
	DCI &51b64f9f 	; ldrexdpl	r4, [r6]
	DCI &61b64f9f 	; ldrexdvs	r4, [r6]
	DCI &71b64f9f 	; ldrexdvc	r4, [r6]
	DCI &81b64f9f 	; ldrexdhi	r4, [r6]
	DCI &91b64f9f 	; ldrexdls	r4, [r6]
	DCI &a1b64f9f 	; ldrexdge	r4, [r6]
	DCI &b1b64f9f 	; ldrexdlt	r4, [r6]
	DCI &c1b64f9f 	; ldrexdgt	r4, [r6]
	DCI &d1b64f9f 	; ldrexdle	r4, [r6]
	DCI &e1b64f9f 	; ldrexd	r4, [r6]
	DCI &e1b64f9f 	; ldrexd	r4, [r6]

	DCI &01861f94 	; strexeq	r1, r4, [r6]
	DCI &11861f94 	; strexne	r1, r4, [r6]
	DCI &21861f94 	; strexcs	r1, r4, [r6]
	DCI &21861f94 	; strexcs	r1, r4, [r6]
	DCI &31861f94 	; strexcc	r1, r4, [r6]
	DCI &31861f94 	; strexcc	r1, r4, [r6]
	DCI &41861f94 	; strexmi	r1, r4, [r6]
	DCI &51861f94 	; strexpl	r1, r4, [r6]
	DCI &61861f94 	; strexvs	r1, r4, [r6]
	DCI &71861f94 	; strexvc	r1, r4, [r6]
	DCI &81861f94 	; strexhi	r1, r4, [r6]
	DCI &91861f94 	; strexls	r1, r4, [r6]
	DCI &a1861f94 	; strexge	r1, r4, [r6]
	DCI &b1861f94 	; strexlt	r1, r4, [r6]
	DCI &c1861f94 	; strexgt	r1, r4, [r6]
	DCI &d1861f94 	; strexle	r1, r4, [r6]
	DCI &e1861f94 	; strex		r1, r4, [r6]
	DCI &e1861f94 	; strex		r1, r4, [r6]

	DCI &01c61f94 	; strexbeq	r1, r4, [r6]
	DCI &11c61f94 	; strexbne	r1, r4, [r6]
	DCI &21c61f94 	; strexbcs	r1, r4, [r6]
	DCI &21c61f94 	; strexbcs	r1, r4, [r6]
	DCI &31c61f94 	; strexbcc	r1, r4, [r6]
	DCI &31c61f94 	; strexbcc	r1, r4, [r6]
	DCI &41c61f94 	; strexbmi	r1, r4, [r6]
	DCI &51c61f94 	; strexbpl	r1, r4, [r6]
	DCI &61c61f94 	; strexbvs	r1, r4, [r6]
	DCI &71c61f94 	; strexbvc	r1, r4, [r6]
	DCI &81c61f94 	; strexbhi	r1, r4, [r6]
	DCI &91c61f94 	; strexbls	r1, r4, [r6]
	DCI &a1c61f94 	; strexbge	r1, r4, [r6]
	DCI &b1c61f94 	; strexblt	r1, r4, [r6]
	DCI &c1c61f94 	; strexbgt	r1, r4, [r6]
	DCI &d1c61f94 	; strexble	r1, r4, [r6]
	DCI &e1c61f94 	; strexb	r1, r4, [r6]
	DCI &e1c61f94 	; strexb	r1, r4, [r6]

	DCI &01e61f94 	; strexheq	r1, r4, [r6]
	DCI &11e61f94 	; strexhne	r1, r4, [r6]
	DCI &21e61f94 	; strexhcs	r1, r4, [r6]
	DCI &21e61f94 	; strexhcs	r1, r4, [r6]
	DCI &31e61f94 	; strexhcc	r1, r4, [r6]
	DCI &31e61f94 	; strexhcc	r1, r4, [r6]
	DCI &41e61f94 	; strexhmi	r1, r4, [r6]
	DCI &51e61f94 	; strexhpl	r1, r4, [r6]
	DCI &61e61f94 	; strexhvs	r1, r4, [r6]
	DCI &71e61f94 	; strexhvc	r1, r4, [r6]
	DCI &81e61f94 	; strexhhi	r1, r4, [r6]
	DCI &91e61f94 	; strexhls	r1, r4, [r6]
	DCI &a1e61f94 	; strexhge	r1, r4, [r6]
	DCI &b1e61f94 	; strexhlt	r1, r4, [r6]
	DCI &c1e61f94 	; strexhgt	r1, r4, [r6]
	DCI &d1e61f94 	; strexhle	r1, r4, [r6]
	DCI &e1e61f94 	; strexh	r1, r4, [r6]
	DCI &e1e61f94 	; strexh	r1, r4, [r6]

	DCI &01a61f94 	; strexdeq	r1, r4, [r6]
	DCI &11a61f94 	; strexdne	r1, r4, [r6]
	DCI &21a61f94 	; strexdcs	r1, r4, [r6]
	DCI &21a61f94 	; strexdcs	r1, r4, [r6]
	DCI &31a61f94 	; strexdcc	r1, r4, [r6]
	DCI &31a61f94 	; strexdcc	r1, r4, [r6]
	DCI &41a61f94 	; strexdmi	r1, r4, [r6]
	DCI &51a61f94 	; strexdpl	r1, r4, [r6]
	DCI &61a61f94 	; strexdvs	r1, r4, [r6]
	DCI &71a61f94 	; strexdvc	r1, r4, [r6]
	DCI &81a61f94 	; strexdhi	r1, r4, [r6]
	DCI &91a61f94 	; strexdls	r1, r4, [r6]
	DCI &a1a61f94 	; strexdge	r1, r4, [r6]
	DCI &b1a61f94 	; strexdlt	r1, r4, [r6]
	DCI &c1a61f94 	; strexdgt	r1, r4, [r6]
	DCI &d1a61f94 	; strexdle	r1, r4, [r6]
	DCI &e1a61f94 	; strexd	r1, r4, [r6]
	DCI &e1a61f94 	; strexd	r1, r4, [r6]

	]

	[ :LNOT: REFERENCE
	CLREX
	|
	DCI &f57ff01f
	]

	END
