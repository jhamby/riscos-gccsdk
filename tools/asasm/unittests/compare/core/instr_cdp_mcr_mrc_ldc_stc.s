; Tests CDP, CDP2, MCR, MCR2, MRC, MRC2, LDC, STC.
; -RUNOPT: -CPU=7-A

		AREA |C$$code|, CODE, READONLY

	[ :LNOT: REFERENCE
		MACRO
		Inject $in
		;INFO 0, "\t$in"
		$in
		MEND
	]

		; Coprocessor instructions

	[ :LNOT: REFERENCE
		MACRO
		SubInvoke $instr, $dst
		Inject	"$instr p8, 5, $dst, c4, c12"
		Inject	"$instr p8 , 5 , $dst , c4 , c12 , 4"
		Inject	"$instr p8 ,5 ,$dst ,c4 ,c12 , # 7"
		MEND

		MACRO
		Invoke $base, $dst
		SubInvoke $base, $dst
		SubInvoke $base..W, $dst
		LCLS	instr
		LCLA	cnt
cnt		SETA	0
		WHILE	cnt < 17
		; 17 condition codes to be tested:
		;   - Condition code NV is left out from this test.
		;   - HS and LO are equivalents for CS and CC.
instr		SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
		SubInvoke $instr, $dst
		SubInvoke $instr..W, $dst
cnt		SETA	cnt + 1
		WEND
		MEND

		ARM
		Invoke CDP, c3
		SubInvoke CDP2, c3
		SubInvoke CDP2.W, c3

		Invoke MCR, r3
		SubInvoke MCR2, r3
		SubInvoke MCR2.W, r3

		Invoke MRC, r3
		SubInvoke MRC2, r3
		SubInvoke MRC2.W, r3

		THUMB
		Invoke CDP, c3
		SubInvoke CDP2, c3
		SubInvoke CDP2.W, c3

		Invoke MCR, r3
		SubInvoke MCR2, r3
		SubInvoke MCR2.W, r3

		Invoke MRC, r3
		SubInvoke MRC2, r3
		SubInvoke MRC2.W, r3
	|
		ARM
		DCI.W &ee54380c	; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c	; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec	; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c	; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c	; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec	; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &0e54380c	; cdpeq	8, 5, cr3, cr4, cr12, {0}
		DCI.W &0e54388c	; cdpeq	8, 5, cr3, cr4, cr12, {4}
		DCI.W &0e5438ec	; cdpeq	8, 5, cr3, cr4, cr12, {7}
		DCI.W &0e54380c	; cdpeq	8, 5, cr3, cr4, cr12, {0}
		DCI.W &0e54388c	; cdpeq	8, 5, cr3, cr4, cr12, {4}
		DCI.W &0e5438ec	; cdpeq	8, 5, cr3, cr4, cr12, {7}
		DCI.W &1e54380c	; cdpne	8, 5, cr3, cr4, cr12, {0}
		DCI.W &1e54388c	; cdpne	8, 5, cr3, cr4, cr12, {4}
		DCI.W &1e5438ec	; cdpne	8, 5, cr3, cr4, cr12, {7}
		DCI.W &1e54380c	; cdpne	8, 5, cr3, cr4, cr12, {0}
		DCI.W &1e54388c	; cdpne	8, 5, cr3, cr4, cr12, {4}
		DCI.W &1e5438ec	; cdpne	8, 5, cr3, cr4, cr12, {7}
		DCI.W &2e54380c	; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &2e54388c	; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &2e5438ec	; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &2e54380c	; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &2e54388c	; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &2e5438ec	; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &3e54380c	; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &3e54388c	; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &3e5438ec	; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &3e54380c	; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &3e54388c	; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &3e5438ec	; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &4e54380c	; cdpmi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &4e54388c	; cdpmi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &4e5438ec	; cdpmi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &4e54380c	; cdpmi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &4e54388c	; cdpmi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &4e5438ec	; cdpmi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &5e54380c	; cdppl	8, 5, cr3, cr4, cr12, {0}
		DCI.W &5e54388c	; cdppl	8, 5, cr3, cr4, cr12, {4}
		DCI.W &5e5438ec	; cdppl	8, 5, cr3, cr4, cr12, {7}
		DCI.W &5e54380c	; cdppl	8, 5, cr3, cr4, cr12, {0}
		DCI.W &5e54388c	; cdppl	8, 5, cr3, cr4, cr12, {4}
		DCI.W &5e5438ec	; cdppl	8, 5, cr3, cr4, cr12, {7}
		DCI.W &6e54380c	; cdpvs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &6e54388c	; cdpvs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &6e5438ec	; cdpvs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &6e54380c	; cdpvs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &6e54388c	; cdpvs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &6e5438ec	; cdpvs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &7e54380c	; cdpvc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &7e54388c	; cdpvc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &7e5438ec	; cdpvc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &7e54380c	; cdpvc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &7e54388c	; cdpvc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &7e5438ec	; cdpvc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &8e54380c	; cdphi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &8e54388c	; cdphi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &8e5438ec	; cdphi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &8e54380c	; cdphi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &8e54388c	; cdphi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &8e5438ec	; cdphi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &9e54380c	; cdpls	8, 5, cr3, cr4, cr12, {0}
		DCI.W &9e54388c	; cdpls	8, 5, cr3, cr4, cr12, {4}
		DCI.W &9e5438ec	; cdpls	8, 5, cr3, cr4, cr12, {7}
		DCI.W &9e54380c	; cdpls	8, 5, cr3, cr4, cr12, {0}
		DCI.W &9e54388c	; cdpls	8, 5, cr3, cr4, cr12, {4}
		DCI.W &9e5438ec	; cdpls	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ae54380c	; cdpge	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ae54388c	; cdpge	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ae5438ec	; cdpge	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ae54380c	; cdpge	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ae54388c	; cdpge	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ae5438ec	; cdpge	8, 5, cr3, cr4, cr12, {7}
		DCI.W &be54380c	; cdplt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &be54388c	; cdplt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &be5438ec	; cdplt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &be54380c	; cdplt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &be54388c	; cdplt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &be5438ec	; cdplt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ce54380c	; cdpgt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ce54388c	; cdpgt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ce5438ec	; cdpgt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ce54380c	; cdpgt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ce54388c	; cdpgt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ce5438ec	; cdpgt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &de54380c	; cdple	8, 5, cr3, cr4, cr12, {0}
		DCI.W &de54388c	; cdple	8, 5, cr3, cr4, cr12, {4}
		DCI.W &de5438ec	; cdple	8, 5, cr3, cr4, cr12, {7}
		DCI.W &de54380c	; cdple	8, 5, cr3, cr4, cr12, {0}
		DCI.W &de54388c	; cdple	8, 5, cr3, cr4, cr12, {4}
		DCI.W &de5438ec	; cdple	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c	; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c	; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec	; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c	; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c	; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec	; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &2e54380c	; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &2e54388c	; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &2e5438ec	; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &2e54380c	; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &2e54388c	; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &2e5438ec	; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &3e54380c	; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &3e54388c	; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &3e5438ec	; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &3e54380c	; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &3e54388c	; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &3e5438ec	; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &fe54380c	; cdp2	8, 5, cr3, cr4, cr12, {0}
		DCI.W &fe54388c	; cdp2	8, 5, cr3, cr4, cr12, {4}
		DCI.W &fe5438ec	; cdp2	8, 5, cr3, cr4, cr12, {7}
		DCI.W &fe54380c	; cdp2	8, 5, cr3, cr4, cr12, {0}
		DCI.W &fe54388c	; cdp2	8, 5, cr3, cr4, cr12, {4}
		DCI.W &fe5438ec	; cdp2	8, 5, cr3, cr4, cr12, {7}
		DCI.W &eea4381c	; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c	; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc	; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c	; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c	; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc	; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &0ea4381c	; mcreq	8, 5, r3, cr4, cr12, {0}
		DCI.W &0ea4389c	; mcreq	8, 5, r3, cr4, cr12, {4}
		DCI.W &0ea438fc	; mcreq	8, 5, r3, cr4, cr12, {7}
		DCI.W &0ea4381c	; mcreq	8, 5, r3, cr4, cr12, {0}
		DCI.W &0ea4389c	; mcreq	8, 5, r3, cr4, cr12, {4}
		DCI.W &0ea438fc	; mcreq	8, 5, r3, cr4, cr12, {7}
		DCI.W &1ea4381c	; mcrne	8, 5, r3, cr4, cr12, {0}
		DCI.W &1ea4389c	; mcrne	8, 5, r3, cr4, cr12, {4}
		DCI.W &1ea438fc	; mcrne	8, 5, r3, cr4, cr12, {7}
		DCI.W &1ea4381c	; mcrne	8, 5, r3, cr4, cr12, {0}
		DCI.W &1ea4389c	; mcrne	8, 5, r3, cr4, cr12, {4}
		DCI.W &1ea438fc	; mcrne	8, 5, r3, cr4, cr12, {7}
		DCI.W &2ea4381c	; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2ea4389c	; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2ea438fc	; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &2ea4381c	; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2ea4389c	; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2ea438fc	; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &3ea4381c	; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3ea4389c	; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3ea438fc	; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &3ea4381c	; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3ea4389c	; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3ea438fc	; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &4ea4381c	; mcrmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &4ea4389c	; mcrmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &4ea438fc	; mcrmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &4ea4381c	; mcrmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &4ea4389c	; mcrmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &4ea438fc	; mcrmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &5ea4381c	; mcrpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &5ea4389c	; mcrpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &5ea438fc	; mcrpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &5ea4381c	; mcrpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &5ea4389c	; mcrpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &5ea438fc	; mcrpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &6ea4381c	; mcrvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &6ea4389c	; mcrvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &6ea438fc	; mcrvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &6ea4381c	; mcrvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &6ea4389c	; mcrvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &6ea438fc	; mcrvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &7ea4381c	; mcrvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &7ea4389c	; mcrvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &7ea438fc	; mcrvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &7ea4381c	; mcrvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &7ea4389c	; mcrvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &7ea438fc	; mcrvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &8ea4381c	; mcrhi	8, 5, r3, cr4, cr12, {0}
		DCI.W &8ea4389c	; mcrhi	8, 5, r3, cr4, cr12, {4}
		DCI.W &8ea438fc	; mcrhi	8, 5, r3, cr4, cr12, {7}
		DCI.W &8ea4381c	; mcrhi	8, 5, r3, cr4, cr12, {0}
		DCI.W &8ea4389c	; mcrhi	8, 5, r3, cr4, cr12, {4}
		DCI.W &8ea438fc	; mcrhi	8, 5, r3, cr4, cr12, {7}
		DCI.W &9ea4381c	; mcrls	8, 5, r3, cr4, cr12, {0}
		DCI.W &9ea4389c	; mcrls	8, 5, r3, cr4, cr12, {4}
		DCI.W &9ea438fc	; mcrls	8, 5, r3, cr4, cr12, {7}
		DCI.W &9ea4381c	; mcrls	8, 5, r3, cr4, cr12, {0}
		DCI.W &9ea4389c	; mcrls	8, 5, r3, cr4, cr12, {4}
		DCI.W &9ea438fc	; mcrls	8, 5, r3, cr4, cr12, {7}
		DCI.W &aea4381c	; mcrge	8, 5, r3, cr4, cr12, {0}
		DCI.W &aea4389c	; mcrge	8, 5, r3, cr4, cr12, {4}
		DCI.W &aea438fc	; mcrge	8, 5, r3, cr4, cr12, {7}
		DCI.W &aea4381c	; mcrge	8, 5, r3, cr4, cr12, {0}
		DCI.W &aea4389c	; mcrge	8, 5, r3, cr4, cr12, {4}
		DCI.W &aea438fc	; mcrge	8, 5, r3, cr4, cr12, {7}
		DCI.W &bea4381c	; mcrlt	8, 5, r3, cr4, cr12, {0}
		DCI.W &bea4389c	; mcrlt	8, 5, r3, cr4, cr12, {4}
		DCI.W &bea438fc	; mcrlt	8, 5, r3, cr4, cr12, {7}
		DCI.W &bea4381c	; mcrlt	8, 5, r3, cr4, cr12, {0}
		DCI.W &bea4389c	; mcrlt	8, 5, r3, cr4, cr12, {4}
		DCI.W &bea438fc	; mcrlt	8, 5, r3, cr4, cr12, {7}
		DCI.W &cea4381c	; mcrgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &cea4389c	; mcrgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &cea438fc	; mcrgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &cea4381c	; mcrgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &cea4389c	; mcrgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &cea438fc	; mcrgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &dea4381c	; mcrle	8, 5, r3, cr4, cr12, {0}
		DCI.W &dea4389c	; mcrle	8, 5, r3, cr4, cr12, {4}
		DCI.W &dea438fc	; mcrle	8, 5, r3, cr4, cr12, {7}
		DCI.W &dea4381c	; mcrle	8, 5, r3, cr4, cr12, {0}
		DCI.W &dea4389c	; mcrle	8, 5, r3, cr4, cr12, {4}
		DCI.W &dea438fc	; mcrle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c	; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c	; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc	; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c	; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c	; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc	; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &2ea4381c	; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2ea4389c	; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2ea438fc	; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &2ea4381c	; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2ea4389c	; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2ea438fc	; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &3ea4381c	; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3ea4389c	; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3ea438fc	; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &3ea4381c	; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3ea4389c	; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3ea438fc	; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &fea4381c	; mcr2	8, 5, r3, cr4, cr12, {0}
		DCI.W &fea4389c	; mcr2	8, 5, r3, cr4, cr12, {4}
		DCI.W &fea438fc	; mcr2	8, 5, r3, cr4, cr12, {7}
		DCI.W &fea4381c	; mcr2	8, 5, r3, cr4, cr12, {0}
		DCI.W &fea4389c	; mcr2	8, 5, r3, cr4, cr12, {4}
		DCI.W &fea438fc	; mcr2	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c	; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c	; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc	; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c	; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c	; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc	; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &0eb4381c	; mrceq	8, 5, r3, cr4, cr12, {0}
		DCI.W &0eb4389c	; mrceq	8, 5, r3, cr4, cr12, {4}
		DCI.W &0eb438fc	; mrceq	8, 5, r3, cr4, cr12, {7}
		DCI.W &0eb4381c	; mrceq	8, 5, r3, cr4, cr12, {0}
		DCI.W &0eb4389c	; mrceq	8, 5, r3, cr4, cr12, {4}
		DCI.W &0eb438fc	; mrceq	8, 5, r3, cr4, cr12, {7}
		DCI.W &1eb4381c	; mrcne	8, 5, r3, cr4, cr12, {0}
		DCI.W &1eb4389c	; mrcne	8, 5, r3, cr4, cr12, {4}
		DCI.W &1eb438fc	; mrcne	8, 5, r3, cr4, cr12, {7}
		DCI.W &1eb4381c	; mrcne	8, 5, r3, cr4, cr12, {0}
		DCI.W &1eb4389c	; mrcne	8, 5, r3, cr4, cr12, {4}
		DCI.W &1eb438fc	; mrcne	8, 5, r3, cr4, cr12, {7}
		DCI.W &2eb4381c	; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2eb4389c	; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2eb438fc	; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &2eb4381c	; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2eb4389c	; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2eb438fc	; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &3eb4381c	; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3eb4389c	; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3eb438fc	; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &3eb4381c	; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3eb4389c	; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3eb438fc	; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &4eb4381c	; mrcmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &4eb4389c	; mrcmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &4eb438fc	; mrcmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &4eb4381c	; mrcmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &4eb4389c	; mrcmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &4eb438fc	; mrcmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &5eb4381c	; mrcpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &5eb4389c	; mrcpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &5eb438fc	; mrcpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &5eb4381c	; mrcpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &5eb4389c	; mrcpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &5eb438fc	; mrcpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &6eb4381c	; mrcvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &6eb4389c	; mrcvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &6eb438fc	; mrcvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &6eb4381c	; mrcvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &6eb4389c	; mrcvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &6eb438fc	; mrcvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &7eb4381c	; mrcvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &7eb4389c	; mrcvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &7eb438fc	; mrcvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &7eb4381c	; mrcvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &7eb4389c	; mrcvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &7eb438fc	; mrcvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &8eb4381c	; mrchi	8, 5, r3, cr4, cr12, {0}
		DCI.W &8eb4389c	; mrchi	8, 5, r3, cr4, cr12, {4}
		DCI.W &8eb438fc	; mrchi	8, 5, r3, cr4, cr12, {7}
		DCI.W &8eb4381c	; mrchi	8, 5, r3, cr4, cr12, {0}
		DCI.W &8eb4389c	; mrchi	8, 5, r3, cr4, cr12, {4}
		DCI.W &8eb438fc	; mrchi	8, 5, r3, cr4, cr12, {7}
		DCI.W &9eb4381c	; mrcls	8, 5, r3, cr4, cr12, {0}
		DCI.W &9eb4389c	; mrcls	8, 5, r3, cr4, cr12, {4}
		DCI.W &9eb438fc	; mrcls	8, 5, r3, cr4, cr12, {7}
		DCI.W &9eb4381c	; mrcls	8, 5, r3, cr4, cr12, {0}
		DCI.W &9eb4389c	; mrcls	8, 5, r3, cr4, cr12, {4}
		DCI.W &9eb438fc	; mrcls	8, 5, r3, cr4, cr12, {7}
		DCI.W &aeb4381c	; mrcge	8, 5, r3, cr4, cr12, {0}
		DCI.W &aeb4389c	; mrcge	8, 5, r3, cr4, cr12, {4}
		DCI.W &aeb438fc	; mrcge	8, 5, r3, cr4, cr12, {7}
		DCI.W &aeb4381c	; mrcge	8, 5, r3, cr4, cr12, {0}
		DCI.W &aeb4389c	; mrcge	8, 5, r3, cr4, cr12, {4}
		DCI.W &aeb438fc	; mrcge	8, 5, r3, cr4, cr12, {7}
		DCI.W &beb4381c	; mrclt	8, 5, r3, cr4, cr12, {0}
		DCI.W &beb4389c	; mrclt	8, 5, r3, cr4, cr12, {4}
		DCI.W &beb438fc	; mrclt	8, 5, r3, cr4, cr12, {7}
		DCI.W &beb4381c	; mrclt	8, 5, r3, cr4, cr12, {0}
		DCI.W &beb4389c	; mrclt	8, 5, r3, cr4, cr12, {4}
		DCI.W &beb438fc	; mrclt	8, 5, r3, cr4, cr12, {7}
		DCI.W &ceb4381c	; mrcgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &ceb4389c	; mrcgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &ceb438fc	; mrcgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &ceb4381c	; mrcgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &ceb4389c	; mrcgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &ceb438fc	; mrcgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &deb4381c	; mrcle	8, 5, r3, cr4, cr12, {0}
		DCI.W &deb4389c	; mrcle	8, 5, r3, cr4, cr12, {4}
		DCI.W &deb438fc	; mrcle	8, 5, r3, cr4, cr12, {7}
		DCI.W &deb4381c	; mrcle	8, 5, r3, cr4, cr12, {0}
		DCI.W &deb4389c	; mrcle	8, 5, r3, cr4, cr12, {4}
		DCI.W &deb438fc	; mrcle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c	; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c	; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc	; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c	; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c	; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc	; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &2eb4381c	; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2eb4389c	; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2eb438fc	; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &2eb4381c	; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &2eb4389c	; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &2eb438fc	; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &3eb4381c	; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3eb4389c	; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3eb438fc	; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &3eb4381c	; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &3eb4389c	; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &3eb438fc	; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &feb4381c	; mrc2	8, 5, r3, cr4, cr12, {0}
		DCI.W &feb4389c	; mrc2	8, 5, r3, cr4, cr12, {4}
		DCI.W &feb438fc	; mrc2	8, 5, r3, cr4, cr12, {7}
		DCI.W &feb4381c	; mrc2	8, 5, r3, cr4, cr12, {0}
		DCI.W &feb4389c	; mrc2	8, 5, r3, cr4, cr12, {4}
		DCI.W &feb438fc	; mrc2	8, 5, r3, cr4, cr12, {7}

		THUMB
		DCI.W &ee54380c		; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf01		; itttt	eq
		DCI.W &ee54380c		; cdpeq	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpeq	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpeq	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpeq	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf07		; ittee	eq
		DCI.W &ee54388c		; cdpeq	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpeq	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpne	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpne	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf1f		; itttt	ne
		DCI.W &ee5438ec		; cdpne	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpne	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpne	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpne	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &ee54380c		; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &ee54388c		; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &ee5438ec		; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf41		; itttt	mi
		DCI.W &ee54380c		; cdpmi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpmi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpmi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpmi	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf47		; ittee	mi
		DCI.W &ee54388c		; cdpmi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpmi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdppl	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdppl	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf5f		; itttt	pl
		DCI.W &ee5438ec		; cdppl	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdppl	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdppl	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdppl	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf61		; itttt	vs
		DCI.W &ee54380c		; cdpvs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpvs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpvs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpvs	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf67		; ittee	vs
		DCI.W &ee54388c		; cdpvs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpvs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpvc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpvc	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf7f		; itttt	vc
		DCI.W &ee5438ec		; cdpvc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpvc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpvc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpvc	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf81		; itttt	hi
		DCI.W &ee54380c		; cdphi	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdphi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdphi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdphi	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf87		; ittee	hi
		DCI.W &ee54388c		; cdphi	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdphi	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpls	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpls	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf9f		; itttt	ls
		DCI.W &ee5438ec		; cdpls	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpls	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpls	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpls	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bfa1		; itttt	ge
		DCI.W &ee54380c		; cdpge	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpge	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpge	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpge	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bfa7		; ittee	ge
		DCI.W &ee54388c		; cdpge	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpge	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdplt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdplt	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bfbf		; itttt	lt
		DCI.W &ee5438ec		; cdplt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdplt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdplt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdplt	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bfc1		; itttt	gt
		DCI.W &ee54380c		; cdpgt	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpgt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpgt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpgt	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bfc7		; ittee	gt
		DCI.W &ee54388c		; cdpgt	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpgt	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdple	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdple	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bfdf		; itttt	le
		DCI.W &ee5438ec		; cdple	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdple	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdple	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdple	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdp	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdp	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdp	8, 5, cr3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &ee54380c		; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcs	8, 5, cr3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &ee54388c		; cdpcs	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcs	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &ee5438ec		; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &ee54380c		; cdpcc	8, 5, cr3, cr4, cr12, {0}
		DCI.W &ee54388c		; cdpcc	8, 5, cr3, cr4, cr12, {4}
		DCI.W &ee5438ec		; cdpcc	8, 5, cr3, cr4, cr12, {7}
		DCI.W &fe54380c		; cdp2	8, 5, cr3, cr4, cr12, {0}
		DCI.W &fe54388c		; cdp2	8, 5, cr3, cr4, cr12, {4}
		DCI.W &fe5438ec		; cdp2	8, 5, cr3, cr4, cr12, {7}
		DCI.W &fe54380c		; cdp2	8, 5, cr3, cr4, cr12, {0}
		DCI.W &fe54388c		; cdp2	8, 5, cr3, cr4, cr12, {4}
		DCI.W &fe5438ec		; cdp2	8, 5, cr3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf01		; itttt	eq
		DCI.W &eea4381c		; mcreq	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcreq	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcreq	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcreq	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf07		; ittee	eq
		DCI.W &eea4389c		; mcreq	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcreq	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrne	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrne	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf1f		; itttt	ne
		DCI.W &eea438fc		; mcrne	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrne	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrne	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrne	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &eea4381c		; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &eea4389c		; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &eea438fc		; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf41		; itttt	mi
		DCI.W &eea4381c		; mcrmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrmi	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf47		; ittee	mi
		DCI.W &eea4389c		; mcrmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrpl	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf5f		; itttt	pl
		DCI.W &eea438fc		; mcrpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrpl	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf61		; itttt	vs
		DCI.W &eea4381c		; mcrvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrvs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf67		; ittee	vs
		DCI.W &eea4389c		; mcrvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrvc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf7f		; itttt	vc
		DCI.W &eea438fc		; mcrvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrvc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf81		; itttt	hi
		DCI.W &eea4381c		; mcrhi	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrhi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrhi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrhi	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf87		; ittee	hi
		DCI.W &eea4389c		; mcrhi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrhi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrls	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrls	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf9f		; itttt	ls
		DCI.W &eea438fc		; mcrls	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrls	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrls	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrls	8, 5, r3, cr4, cr12, {7}
		DCI.N &bfa1		; itttt	ge
		DCI.W &eea4381c		; mcrge	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrge	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrge	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrge	8, 5, r3, cr4, cr12, {0}
		DCI.N &bfa7		; ittee	ge
		DCI.W &eea4389c		; mcrge	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrge	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrlt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrlt	8, 5, r3, cr4, cr12, {4}
		DCI.N &bfbf		; itttt	lt
		DCI.W &eea438fc		; mcrlt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrlt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrlt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrlt	8, 5, r3, cr4, cr12, {7}
		DCI.N &bfc1		; itttt	gt
		DCI.W &eea4381c		; mcrgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrgt	8, 5, r3, cr4, cr12, {0}
		DCI.N &bfc7		; ittee	gt
		DCI.W &eea4389c		; mcrgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrle	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrle	8, 5, r3, cr4, cr12, {4}
		DCI.N &bfdf		; itttt	le
		DCI.W &eea438fc		; mcrle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrle	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrle	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcr	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcr	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcr	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &eea4381c		; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &eea4389c		; mcrcs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &eea438fc		; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eea4381c		; mcrcc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eea4389c		; mcrcc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eea438fc		; mcrcc	8, 5, r3, cr4, cr12, {7}
		DCI.W &fea4381c		; mcr2	8, 5, r3, cr4, cr12, {0}
		DCI.W &fea4389c		; mcr2	8, 5, r3, cr4, cr12, {4}
		DCI.W &fea438fc		; mcr2	8, 5, r3, cr4, cr12, {7}
		DCI.W &fea4381c		; mcr2	8, 5, r3, cr4, cr12, {0}
		DCI.W &fea4389c		; mcr2	8, 5, r3, cr4, cr12, {4}
		DCI.W &fea438fc		; mcr2	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf01		; itttt	eq
		DCI.W &eeb4381c		; mrceq	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrceq	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrceq	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrceq	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf07		; ittee	eq
		DCI.W &eeb4389c		; mrceq	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrceq	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcne	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcne	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf1f		; itttt	ne
		DCI.W &eeb438fc		; mrcne	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcne	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcne	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcne	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &eeb4381c		; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &eeb4389c		; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &eeb438fc		; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf41		; itttt	mi
		DCI.W &eeb4381c		; mrcmi	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcmi	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf47		; ittee	mi
		DCI.W &eeb4389c		; mrcmi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcmi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcpl	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf5f		; itttt	pl
		DCI.W &eeb438fc		; mrcpl	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcpl	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcpl	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcpl	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf61		; itttt	vs
		DCI.W &eeb4381c		; mrcvs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcvs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf67		; ittee	vs
		DCI.W &eeb4389c		; mrcvs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcvs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcvc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf7f		; itttt	vc
		DCI.W &eeb438fc		; mrcvc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcvc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcvc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcvc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf81		; itttt	hi
		DCI.W &eeb4381c		; mrchi	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrchi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrchi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrchi	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf87		; ittee	hi
		DCI.W &eeb4389c		; mrchi	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrchi	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcls	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcls	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf9f		; itttt	ls
		DCI.W &eeb438fc		; mrcls	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcls	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcls	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcls	8, 5, r3, cr4, cr12, {7}
		DCI.N &bfa1		; itttt	ge
		DCI.W &eeb4381c		; mrcge	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcge	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcge	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcge	8, 5, r3, cr4, cr12, {0}
		DCI.N &bfa7		; ittee	ge
		DCI.W &eeb4389c		; mrcge	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcge	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrclt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrclt	8, 5, r3, cr4, cr12, {4}
		DCI.N &bfbf		; itttt	lt
		DCI.W &eeb438fc		; mrclt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrclt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrclt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrclt	8, 5, r3, cr4, cr12, {7}
		DCI.N &bfc1		; itttt	gt
		DCI.W &eeb4381c		; mrcgt	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcgt	8, 5, r3, cr4, cr12, {0}
		DCI.N &bfc7		; ittee	gt
		DCI.W &eeb4389c		; mrcgt	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcgt	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcle	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcle	8, 5, r3, cr4, cr12, {4}
		DCI.N &bfdf		; itttt	le
		DCI.W &eeb438fc		; mrcle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrcle	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrcle	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrcle	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrc	8, 5, r3, cr4, cr12, {7}
		DCI.N &bf21		; itttt	cs
		DCI.W &eeb4381c		; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccs	8, 5, r3, cr4, cr12, {0}
		DCI.N &bf27		; ittee	cs
		DCI.W &eeb4389c		; mrccs	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccs	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.N &bf3f		; itttt	cc
		DCI.W &eeb438fc		; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &eeb4381c		; mrccc	8, 5, r3, cr4, cr12, {0}
		DCI.W &eeb4389c		; mrccc	8, 5, r3, cr4, cr12, {4}
		DCI.W &eeb438fc		; mrccc	8, 5, r3, cr4, cr12, {7}
		DCI.W &feb4381c		; mrc2	8, 5, r3, cr4, cr12, {0}
		DCI.W &feb4389c		; mrc2	8, 5, r3, cr4, cr12, {4}
		DCI.W &feb438fc		; mrc2	8, 5, r3, cr4, cr12, {7}
		DCI.W &feb4381c		; mrc2	8, 5, r3, cr4, cr12, {0}
		DCI.W &feb4389c		; mrc2	8, 5, r3, cr4, cr12, {4}
		DCI.W &feb438fc		; mrc2	8, 5, r3, cr4, cr12, {7}
	]

		; Addressing mode 5 - Load/Store Coprocessor
	[ :LNOT: REFERENCE
		; Called for pre-UAL version in THUMB mode or UAL version
		; in CODE16 mode.
		MACRO
		LDC$sfx $a, $b, $c, $d, $e, $f
		[ {UAL}
		NOP.N
		|
		; Force IT block to be ended
		THUMB
		NOP.N
		CODE16
		]
		MEND

		; Called for pre-UAL version in THUMB mode or UAL version
		; in CODE16 mode.
		MACRO
		STC$sfx $a, $b, $c, $d, $e, $f
		[ {UAL}
		NOP.N
		|
		; Force IT block to be ended
		THUMB
		NOP.N
		CODE16
		]
		MEND

		; Invokes all LDC/LDC2/STC/STRC2 addressing mode variants.
		MACRO
		SubInvokeDC $instr
		; 1
		Inject "$instr p6 , c3 , [ r2 , # 516]"
		Inject "$instr..W p6 , c3 , [ r2 , # 516]"
		Inject "$instr p6 , c3 , [ r2 , # - 516]"
		Inject "$instr..W p6 , c3 , [ r2 , # - 516]"
		; 2
		Inject "$instr p6 , c3 , [ r2 , # 516]!"
		Inject "$instr..W p6 , c3 , [ r2 , # 516]!"
		Inject "$instr p6 , c3 , [ r2 , # - 516]!"
		Inject "$instr..W p6 , c3 , [ r2 , # - 516]!"
		; 3
		Inject "$instr p6 , c3 , [ r2 ] , # 516"
		Inject "$instr..W p6 , c3 , [ r2 ] , # 516"
		Inject "$instr p6 , c3 , [ r2 ] , # - 516"
		Inject "$instr..W p6 , c3 , [ r2 ] , # - 516"
		; 4
		Inject "$instr p6 , c3 , [ r2 ] , { 33 }"
		Inject "$instr..W p6 , c3 , [ r2 ] , { 33 }"
		MEND

		; Invokes {L|S}DC with and without L, and with and without
		; condition codes.
		MACRO
		InvokeDC $base
		SubInvokeDC $base
		SubInvokeDC $base.L
		LCLS	instr0
		LCLS	instr1
		LCLS	instr2
		LCLA	cnt
cnt		SETA	0
		WHILE	cnt < 17
		; 17 condition codes to be tested:
		;   - Condition code NV is left out from this test.
		;   - HS and LO are equivalents for CS and CC.
instr0		SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
instr1		SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "L"	; Pre-UAL
instr2		SETS	"$base.L" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)		; UAL
		SubInvokeDC $instr0
		SubInvokeDC $instr1
		SubInvokeDC $instr2
cnt		SETA	cnt + 1
		WEND
		MEND

		ARM			; Pre-UAL and UAL
		InvokeDC LDC
		SubInvokeDC LDC2
		InvokeDC STC
		SubInvokeDC STC2

		THUMB			; UAL only
		InvokeDC LDC
		SubInvokeDC LDC2
		InvokeDC STC
		SubInvokeDC STC2

		CODE16			; Pre-UAL only
		InvokeDC LDC
		SubInvokeDC LDC2
		InvokeDC STC
		SubInvokeDC STC2
	|
		ARM
		DCI &ed923681		; ldc	6, cr3, [r2, #516]
		DCI &ed923681		; ldc	6, cr3, [r2, #516]
		DCI &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI &ecb23681		; ldc	6, cr3, [r2], #516
		DCI &ecb23681		; ldc	6, cr3, [r2], #516
		DCI &ec323681		; ldc	6, cr3, [r2], #-516
		DCI &ec323681		; ldc	6, cr3, [r2], #-516
		DCI &ec923621		; ldc	6, cr3, [r2], {33}
		DCI &ec923621		; ldc	6, cr3, [r2], {33}
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &0d923681		; ldceq	6, cr3, [r2, #516]
		DCI &0d923681		; ldceq	6, cr3, [r2, #516]
		DCI &0d123681		; ldceq	6, cr3, [r2, #-516]
		DCI &0d123681		; ldceq	6, cr3, [r2, #-516]
		DCI &0db23681		; ldceq	6, cr3, [r2, #516]!
		DCI &0db23681		; ldceq	6, cr3, [r2, #516]!
		DCI &0d323681		; ldceq	6, cr3, [r2, #-516]!
		DCI &0d323681		; ldceq	6, cr3, [r2, #-516]!
		DCI &0cb23681		; ldceq	6, cr3, [r2], #516
		DCI &0cb23681		; ldceq	6, cr3, [r2], #516
		DCI &0c323681		; ldceq	6, cr3, [r2], #-516
		DCI &0c323681		; ldceq	6, cr3, [r2], #-516
		DCI &0c923621		; ldceq	6, cr3, [r2], {33}
		DCI &0c923621		; ldceq	6, cr3, [r2], {33}
		DCI &0dd23681		; ldcleq	6, cr3, [r2, #516]
		DCI &0dd23681		; ldcleq	6, cr3, [r2, #516]
		DCI &0d523681		; ldcleq	6, cr3, [r2, #-516]
		DCI &0d523681		; ldcleq	6, cr3, [r2, #-516]
		DCI &0df23681		; ldcleq	6, cr3, [r2, #516]!
		DCI &0df23681		; ldcleq	6, cr3, [r2, #516]!
		DCI &0d723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI &0d723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI &0cf23681		; ldcleq	6, cr3, [r2], #516
		DCI &0cf23681		; ldcleq	6, cr3, [r2], #516
		DCI &0c723681		; ldcleq	6, cr3, [r2], #-516
		DCI &0c723681		; ldcleq	6, cr3, [r2], #-516
		DCI &0cd23621		; ldcleq	6, cr3, [r2], {33}
		DCI &0cd23621		; ldcleq	6, cr3, [r2], {33}
		DCI &0dd23681		; ldcleq	6, cr3, [r2, #516]
		DCI &0dd23681		; ldcleq	6, cr3, [r2, #516]
		DCI &0d523681		; ldcleq	6, cr3, [r2, #-516]
		DCI &0d523681		; ldcleq	6, cr3, [r2, #-516]
		DCI &0df23681		; ldcleq	6, cr3, [r2, #516]!
		DCI &0df23681		; ldcleq	6, cr3, [r2, #516]!
		DCI &0d723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI &0d723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI &0cf23681		; ldcleq	6, cr3, [r2], #516
		DCI &0cf23681		; ldcleq	6, cr3, [r2], #516
		DCI &0c723681		; ldcleq	6, cr3, [r2], #-516
		DCI &0c723681		; ldcleq	6, cr3, [r2], #-516
		DCI &0cd23621		; ldcleq	6, cr3, [r2], {33}
		DCI &0cd23621		; ldcleq	6, cr3, [r2], {33}
		DCI &1d923681		; ldcne	6, cr3, [r2, #516]
		DCI &1d923681		; ldcne	6, cr3, [r2, #516]
		DCI &1d123681		; ldcne	6, cr3, [r2, #-516]
		DCI &1d123681		; ldcne	6, cr3, [r2, #-516]
		DCI &1db23681		; ldcne	6, cr3, [r2, #516]!
		DCI &1db23681		; ldcne	6, cr3, [r2, #516]!
		DCI &1d323681		; ldcne	6, cr3, [r2, #-516]!
		DCI &1d323681		; ldcne	6, cr3, [r2, #-516]!
		DCI &1cb23681		; ldcne	6, cr3, [r2], #516
		DCI &1cb23681		; ldcne	6, cr3, [r2], #516
		DCI &1c323681		; ldcne	6, cr3, [r2], #-516
		DCI &1c323681		; ldcne	6, cr3, [r2], #-516
		DCI &1c923621		; ldcne	6, cr3, [r2], {33}
		DCI &1c923621		; ldcne	6, cr3, [r2], {33}
		DCI &1dd23681		; ldclne	6, cr3, [r2, #516]
		DCI &1dd23681		; ldclne	6, cr3, [r2, #516]
		DCI &1d523681		; ldclne	6, cr3, [r2, #-516]
		DCI &1d523681		; ldclne	6, cr3, [r2, #-516]
		DCI &1df23681		; ldclne	6, cr3, [r2, #516]!
		DCI &1df23681		; ldclne	6, cr3, [r2, #516]!
		DCI &1d723681		; ldclne	6, cr3, [r2, #-516]!
		DCI &1d723681		; ldclne	6, cr3, [r2, #-516]!
		DCI &1cf23681		; ldclne	6, cr3, [r2], #516
		DCI &1cf23681		; ldclne	6, cr3, [r2], #516
		DCI &1c723681		; ldclne	6, cr3, [r2], #-516
		DCI &1c723681		; ldclne	6, cr3, [r2], #-516
		DCI &1cd23621		; ldclne	6, cr3, [r2], {33}
		DCI &1cd23621		; ldclne	6, cr3, [r2], {33}
		DCI &1dd23681		; ldclne	6, cr3, [r2, #516]
		DCI &1dd23681		; ldclne	6, cr3, [r2, #516]
		DCI &1d523681		; ldclne	6, cr3, [r2, #-516]
		DCI &1d523681		; ldclne	6, cr3, [r2, #-516]
		DCI &1df23681		; ldclne	6, cr3, [r2, #516]!
		DCI &1df23681		; ldclne	6, cr3, [r2, #516]!
		DCI &1d723681		; ldclne	6, cr3, [r2, #-516]!
		DCI &1d723681		; ldclne	6, cr3, [r2, #-516]!
		DCI &1cf23681		; ldclne	6, cr3, [r2], #516
		DCI &1cf23681		; ldclne	6, cr3, [r2], #516
		DCI &1c723681		; ldclne	6, cr3, [r2], #-516
		DCI &1c723681		; ldclne	6, cr3, [r2], #-516
		DCI &1cd23621		; ldclne	6, cr3, [r2], {33}
		DCI &1cd23621		; ldclne	6, cr3, [r2], {33}
		DCI &2d923681		; ldccs	6, cr3, [r2, #516]
		DCI &2d923681		; ldccs	6, cr3, [r2, #516]
		DCI &2d123681		; ldccs	6, cr3, [r2, #-516]
		DCI &2d123681		; ldccs	6, cr3, [r2, #-516]
		DCI &2db23681		; ldccs	6, cr3, [r2, #516]!
		DCI &2db23681		; ldccs	6, cr3, [r2, #516]!
		DCI &2d323681		; ldccs	6, cr3, [r2, #-516]!
		DCI &2d323681		; ldccs	6, cr3, [r2, #-516]!
		DCI &2cb23681		; ldccs	6, cr3, [r2], #516
		DCI &2cb23681		; ldccs	6, cr3, [r2], #516
		DCI &2c323681		; ldccs	6, cr3, [r2], #-516
		DCI &2c323681		; ldccs	6, cr3, [r2], #-516
		DCI &2c923621		; ldccs	6, cr3, [r2], {33}
		DCI &2c923621		; ldccs	6, cr3, [r2], {33}
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &3d923681		; ldccc	6, cr3, [r2, #516]
		DCI &3d923681		; ldccc	6, cr3, [r2, #516]
		DCI &3d123681		; ldccc	6, cr3, [r2, #-516]
		DCI &3d123681		; ldccc	6, cr3, [r2, #-516]
		DCI &3db23681		; ldccc	6, cr3, [r2, #516]!
		DCI &3db23681		; ldccc	6, cr3, [r2, #516]!
		DCI &3d323681		; ldccc	6, cr3, [r2, #-516]!
		DCI &3d323681		; ldccc	6, cr3, [r2, #-516]!
		DCI &3cb23681		; ldccc	6, cr3, [r2], #516
		DCI &3cb23681		; ldccc	6, cr3, [r2], #516
		DCI &3c323681		; ldccc	6, cr3, [r2], #-516
		DCI &3c323681		; ldccc	6, cr3, [r2], #-516
		DCI &3c923621		; ldccc	6, cr3, [r2], {33}
		DCI &3c923621		; ldccc	6, cr3, [r2], {33}
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &4d923681		; ldcmi	6, cr3, [r2, #516]
		DCI &4d923681		; ldcmi	6, cr3, [r2, #516]
		DCI &4d123681		; ldcmi	6, cr3, [r2, #-516]
		DCI &4d123681		; ldcmi	6, cr3, [r2, #-516]
		DCI &4db23681		; ldcmi	6, cr3, [r2, #516]!
		DCI &4db23681		; ldcmi	6, cr3, [r2, #516]!
		DCI &4d323681		; ldcmi	6, cr3, [r2, #-516]!
		DCI &4d323681		; ldcmi	6, cr3, [r2, #-516]!
		DCI &4cb23681		; ldcmi	6, cr3, [r2], #516
		DCI &4cb23681		; ldcmi	6, cr3, [r2], #516
		DCI &4c323681		; ldcmi	6, cr3, [r2], #-516
		DCI &4c323681		; ldcmi	6, cr3, [r2], #-516
		DCI &4c923621		; ldcmi	6, cr3, [r2], {33}
		DCI &4c923621		; ldcmi	6, cr3, [r2], {33}
		DCI &4dd23681		; ldclmi	6, cr3, [r2, #516]
		DCI &4dd23681		; ldclmi	6, cr3, [r2, #516]
		DCI &4d523681		; ldclmi	6, cr3, [r2, #-516]
		DCI &4d523681		; ldclmi	6, cr3, [r2, #-516]
		DCI &4df23681		; ldclmi	6, cr3, [r2, #516]!
		DCI &4df23681		; ldclmi	6, cr3, [r2, #516]!
		DCI &4d723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI &4d723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI &4cf23681		; ldclmi	6, cr3, [r2], #516
		DCI &4cf23681		; ldclmi	6, cr3, [r2], #516
		DCI &4c723681		; ldclmi	6, cr3, [r2], #-516
		DCI &4c723681		; ldclmi	6, cr3, [r2], #-516
		DCI &4cd23621		; ldclmi	6, cr3, [r2], {33}
		DCI &4cd23621		; ldclmi	6, cr3, [r2], {33}
		DCI &4dd23681		; ldclmi	6, cr3, [r2, #516]
		DCI &4dd23681		; ldclmi	6, cr3, [r2, #516]
		DCI &4d523681		; ldclmi	6, cr3, [r2, #-516]
		DCI &4d523681		; ldclmi	6, cr3, [r2, #-516]
		DCI &4df23681		; ldclmi	6, cr3, [r2, #516]!
		DCI &4df23681		; ldclmi	6, cr3, [r2, #516]!
		DCI &4d723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI &4d723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI &4cf23681		; ldclmi	6, cr3, [r2], #516
		DCI &4cf23681		; ldclmi	6, cr3, [r2], #516
		DCI &4c723681		; ldclmi	6, cr3, [r2], #-516
		DCI &4c723681		; ldclmi	6, cr3, [r2], #-516
		DCI &4cd23621		; ldclmi	6, cr3, [r2], {33}
		DCI &4cd23621		; ldclmi	6, cr3, [r2], {33}
		DCI &5d923681		; ldcpl	6, cr3, [r2, #516]
		DCI &5d923681		; ldcpl	6, cr3, [r2, #516]
		DCI &5d123681		; ldcpl	6, cr3, [r2, #-516]
		DCI &5d123681		; ldcpl	6, cr3, [r2, #-516]
		DCI &5db23681		; ldcpl	6, cr3, [r2, #516]!
		DCI &5db23681		; ldcpl	6, cr3, [r2, #516]!
		DCI &5d323681		; ldcpl	6, cr3, [r2, #-516]!
		DCI &5d323681		; ldcpl	6, cr3, [r2, #-516]!
		DCI &5cb23681		; ldcpl	6, cr3, [r2], #516
		DCI &5cb23681		; ldcpl	6, cr3, [r2], #516
		DCI &5c323681		; ldcpl	6, cr3, [r2], #-516
		DCI &5c323681		; ldcpl	6, cr3, [r2], #-516
		DCI &5c923621		; ldcpl	6, cr3, [r2], {33}
		DCI &5c923621		; ldcpl	6, cr3, [r2], {33}
		DCI &5dd23681		; ldclpl	6, cr3, [r2, #516]
		DCI &5dd23681		; ldclpl	6, cr3, [r2, #516]
		DCI &5d523681		; ldclpl	6, cr3, [r2, #-516]
		DCI &5d523681		; ldclpl	6, cr3, [r2, #-516]
		DCI &5df23681		; ldclpl	6, cr3, [r2, #516]!
		DCI &5df23681		; ldclpl	6, cr3, [r2, #516]!
		DCI &5d723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI &5d723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI &5cf23681		; ldclpl	6, cr3, [r2], #516
		DCI &5cf23681		; ldclpl	6, cr3, [r2], #516
		DCI &5c723681		; ldclpl	6, cr3, [r2], #-516
		DCI &5c723681		; ldclpl	6, cr3, [r2], #-516
		DCI &5cd23621		; ldclpl	6, cr3, [r2], {33}
		DCI &5cd23621		; ldclpl	6, cr3, [r2], {33}
		DCI &5dd23681		; ldclpl	6, cr3, [r2, #516]
		DCI &5dd23681		; ldclpl	6, cr3, [r2, #516]
		DCI &5d523681		; ldclpl	6, cr3, [r2, #-516]
		DCI &5d523681		; ldclpl	6, cr3, [r2, #-516]
		DCI &5df23681		; ldclpl	6, cr3, [r2, #516]!
		DCI &5df23681		; ldclpl	6, cr3, [r2, #516]!
		DCI &5d723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI &5d723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI &5cf23681		; ldclpl	6, cr3, [r2], #516
		DCI &5cf23681		; ldclpl	6, cr3, [r2], #516
		DCI &5c723681		; ldclpl	6, cr3, [r2], #-516
		DCI &5c723681		; ldclpl	6, cr3, [r2], #-516
		DCI &5cd23621		; ldclpl	6, cr3, [r2], {33}
		DCI &5cd23621		; ldclpl	6, cr3, [r2], {33}
		DCI &6d923681		; ldcvs	6, cr3, [r2, #516]
		DCI &6d923681		; ldcvs	6, cr3, [r2, #516]
		DCI &6d123681		; ldcvs	6, cr3, [r2, #-516]
		DCI &6d123681		; ldcvs	6, cr3, [r2, #-516]
		DCI &6db23681		; ldcvs	6, cr3, [r2, #516]!
		DCI &6db23681		; ldcvs	6, cr3, [r2, #516]!
		DCI &6d323681		; ldcvs	6, cr3, [r2, #-516]!
		DCI &6d323681		; ldcvs	6, cr3, [r2, #-516]!
		DCI &6cb23681		; ldcvs	6, cr3, [r2], #516
		DCI &6cb23681		; ldcvs	6, cr3, [r2], #516
		DCI &6c323681		; ldcvs	6, cr3, [r2], #-516
		DCI &6c323681		; ldcvs	6, cr3, [r2], #-516
		DCI &6c923621		; ldcvs	6, cr3, [r2], {33}
		DCI &6c923621		; ldcvs	6, cr3, [r2], {33}
		DCI &6dd23681		; ldclvs	6, cr3, [r2, #516]
		DCI &6dd23681		; ldclvs	6, cr3, [r2, #516]
		DCI &6d523681		; ldclvs	6, cr3, [r2, #-516]
		DCI &6d523681		; ldclvs	6, cr3, [r2, #-516]
		DCI &6df23681		; ldclvs	6, cr3, [r2, #516]!
		DCI &6df23681		; ldclvs	6, cr3, [r2, #516]!
		DCI &6d723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI &6d723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI &6cf23681		; ldclvs	6, cr3, [r2], #516
		DCI &6cf23681		; ldclvs	6, cr3, [r2], #516
		DCI &6c723681		; ldclvs	6, cr3, [r2], #-516
		DCI &6c723681		; ldclvs	6, cr3, [r2], #-516
		DCI &6cd23621		; ldclvs	6, cr3, [r2], {33}
		DCI &6cd23621		; ldclvs	6, cr3, [r2], {33}
		DCI &6dd23681		; ldclvs	6, cr3, [r2, #516]
		DCI &6dd23681		; ldclvs	6, cr3, [r2, #516]
		DCI &6d523681		; ldclvs	6, cr3, [r2, #-516]
		DCI &6d523681		; ldclvs	6, cr3, [r2, #-516]
		DCI &6df23681		; ldclvs	6, cr3, [r2, #516]!
		DCI &6df23681		; ldclvs	6, cr3, [r2, #516]!
		DCI &6d723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI &6d723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI &6cf23681		; ldclvs	6, cr3, [r2], #516
		DCI &6cf23681		; ldclvs	6, cr3, [r2], #516
		DCI &6c723681		; ldclvs	6, cr3, [r2], #-516
		DCI &6c723681		; ldclvs	6, cr3, [r2], #-516
		DCI &6cd23621		; ldclvs	6, cr3, [r2], {33}
		DCI &6cd23621		; ldclvs	6, cr3, [r2], {33}
		DCI &7d923681		; ldcvc	6, cr3, [r2, #516]
		DCI &7d923681		; ldcvc	6, cr3, [r2, #516]
		DCI &7d123681		; ldcvc	6, cr3, [r2, #-516]
		DCI &7d123681		; ldcvc	6, cr3, [r2, #-516]
		DCI &7db23681		; ldcvc	6, cr3, [r2, #516]!
		DCI &7db23681		; ldcvc	6, cr3, [r2, #516]!
		DCI &7d323681		; ldcvc	6, cr3, [r2, #-516]!
		DCI &7d323681		; ldcvc	6, cr3, [r2, #-516]!
		DCI &7cb23681		; ldcvc	6, cr3, [r2], #516
		DCI &7cb23681		; ldcvc	6, cr3, [r2], #516
		DCI &7c323681		; ldcvc	6, cr3, [r2], #-516
		DCI &7c323681		; ldcvc	6, cr3, [r2], #-516
		DCI &7c923621		; ldcvc	6, cr3, [r2], {33}
		DCI &7c923621		; ldcvc	6, cr3, [r2], {33}
		DCI &7dd23681		; ldclvc	6, cr3, [r2, #516]
		DCI &7dd23681		; ldclvc	6, cr3, [r2, #516]
		DCI &7d523681		; ldclvc	6, cr3, [r2, #-516]
		DCI &7d523681		; ldclvc	6, cr3, [r2, #-516]
		DCI &7df23681		; ldclvc	6, cr3, [r2, #516]!
		DCI &7df23681		; ldclvc	6, cr3, [r2, #516]!
		DCI &7d723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI &7d723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI &7cf23681		; ldclvc	6, cr3, [r2], #516
		DCI &7cf23681		; ldclvc	6, cr3, [r2], #516
		DCI &7c723681		; ldclvc	6, cr3, [r2], #-516
		DCI &7c723681		; ldclvc	6, cr3, [r2], #-516
		DCI &7cd23621		; ldclvc	6, cr3, [r2], {33}
		DCI &7cd23621		; ldclvc	6, cr3, [r2], {33}
		DCI &7dd23681		; ldclvc	6, cr3, [r2, #516]
		DCI &7dd23681		; ldclvc	6, cr3, [r2, #516]
		DCI &7d523681		; ldclvc	6, cr3, [r2, #-516]
		DCI &7d523681		; ldclvc	6, cr3, [r2, #-516]
		DCI &7df23681		; ldclvc	6, cr3, [r2, #516]!
		DCI &7df23681		; ldclvc	6, cr3, [r2, #516]!
		DCI &7d723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI &7d723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI &7cf23681		; ldclvc	6, cr3, [r2], #516
		DCI &7cf23681		; ldclvc	6, cr3, [r2], #516
		DCI &7c723681		; ldclvc	6, cr3, [r2], #-516
		DCI &7c723681		; ldclvc	6, cr3, [r2], #-516
		DCI &7cd23621		; ldclvc	6, cr3, [r2], {33}
		DCI &7cd23621		; ldclvc	6, cr3, [r2], {33}
		DCI &8d923681		; ldchi	6, cr3, [r2, #516]
		DCI &8d923681		; ldchi	6, cr3, [r2, #516]
		DCI &8d123681		; ldchi	6, cr3, [r2, #-516]
		DCI &8d123681		; ldchi	6, cr3, [r2, #-516]
		DCI &8db23681		; ldchi	6, cr3, [r2, #516]!
		DCI &8db23681		; ldchi	6, cr3, [r2, #516]!
		DCI &8d323681		; ldchi	6, cr3, [r2, #-516]!
		DCI &8d323681		; ldchi	6, cr3, [r2, #-516]!
		DCI &8cb23681		; ldchi	6, cr3, [r2], #516
		DCI &8cb23681		; ldchi	6, cr3, [r2], #516
		DCI &8c323681		; ldchi	6, cr3, [r2], #-516
		DCI &8c323681		; ldchi	6, cr3, [r2], #-516
		DCI &8c923621		; ldchi	6, cr3, [r2], {33}
		DCI &8c923621		; ldchi	6, cr3, [r2], {33}
		DCI &8dd23681		; ldclhi	6, cr3, [r2, #516]
		DCI &8dd23681		; ldclhi	6, cr3, [r2, #516]
		DCI &8d523681		; ldclhi	6, cr3, [r2, #-516]
		DCI &8d523681		; ldclhi	6, cr3, [r2, #-516]
		DCI &8df23681		; ldclhi	6, cr3, [r2, #516]!
		DCI &8df23681		; ldclhi	6, cr3, [r2, #516]!
		DCI &8d723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI &8d723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI &8cf23681		; ldclhi	6, cr3, [r2], #516
		DCI &8cf23681		; ldclhi	6, cr3, [r2], #516
		DCI &8c723681		; ldclhi	6, cr3, [r2], #-516
		DCI &8c723681		; ldclhi	6, cr3, [r2], #-516
		DCI &8cd23621		; ldclhi	6, cr3, [r2], {33}
		DCI &8cd23621		; ldclhi	6, cr3, [r2], {33}
		DCI &8dd23681		; ldclhi	6, cr3, [r2, #516]
		DCI &8dd23681		; ldclhi	6, cr3, [r2, #516]
		DCI &8d523681		; ldclhi	6, cr3, [r2, #-516]
		DCI &8d523681		; ldclhi	6, cr3, [r2, #-516]
		DCI &8df23681		; ldclhi	6, cr3, [r2, #516]!
		DCI &8df23681		; ldclhi	6, cr3, [r2, #516]!
		DCI &8d723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI &8d723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI &8cf23681		; ldclhi	6, cr3, [r2], #516
		DCI &8cf23681		; ldclhi	6, cr3, [r2], #516
		DCI &8c723681		; ldclhi	6, cr3, [r2], #-516
		DCI &8c723681		; ldclhi	6, cr3, [r2], #-516
		DCI &8cd23621		; ldclhi	6, cr3, [r2], {33}
		DCI &8cd23621		; ldclhi	6, cr3, [r2], {33}
		DCI &9d923681		; ldcls	6, cr3, [r2, #516]
		DCI &9d923681		; ldcls	6, cr3, [r2, #516]
		DCI &9d123681		; ldcls	6, cr3, [r2, #-516]
		DCI &9d123681		; ldcls	6, cr3, [r2, #-516]
		DCI &9db23681		; ldcls	6, cr3, [r2, #516]!
		DCI &9db23681		; ldcls	6, cr3, [r2, #516]!
		DCI &9d323681		; ldcls	6, cr3, [r2, #-516]!
		DCI &9d323681		; ldcls	6, cr3, [r2, #-516]!
		DCI &9cb23681		; ldcls	6, cr3, [r2], #516
		DCI &9cb23681		; ldcls	6, cr3, [r2], #516
		DCI &9c323681		; ldcls	6, cr3, [r2], #-516
		DCI &9c323681		; ldcls	6, cr3, [r2], #-516
		DCI &9c923621		; ldcls	6, cr3, [r2], {33}
		DCI &9c923621		; ldcls	6, cr3, [r2], {33}
		DCI &9dd23681		; ldclls	6, cr3, [r2, #516]
		DCI &9dd23681		; ldclls	6, cr3, [r2, #516]
		DCI &9d523681		; ldclls	6, cr3, [r2, #-516]
		DCI &9d523681		; ldclls	6, cr3, [r2, #-516]
		DCI &9df23681		; ldclls	6, cr3, [r2, #516]!
		DCI &9df23681		; ldclls	6, cr3, [r2, #516]!
		DCI &9d723681		; ldclls	6, cr3, [r2, #-516]!
		DCI &9d723681		; ldclls	6, cr3, [r2, #-516]!
		DCI &9cf23681		; ldclls	6, cr3, [r2], #516
		DCI &9cf23681		; ldclls	6, cr3, [r2], #516
		DCI &9c723681		; ldclls	6, cr3, [r2], #-516
		DCI &9c723681		; ldclls	6, cr3, [r2], #-516
		DCI &9cd23621		; ldclls	6, cr3, [r2], {33}
		DCI &9cd23621		; ldclls	6, cr3, [r2], {33}
		DCI &9dd23681		; ldclls	6, cr3, [r2, #516]
		DCI &9dd23681		; ldclls	6, cr3, [r2, #516]
		DCI &9d523681		; ldclls	6, cr3, [r2, #-516]
		DCI &9d523681		; ldclls	6, cr3, [r2, #-516]
		DCI &9df23681		; ldclls	6, cr3, [r2, #516]!
		DCI &9df23681		; ldclls	6, cr3, [r2, #516]!
		DCI &9d723681		; ldclls	6, cr3, [r2, #-516]!
		DCI &9d723681		; ldclls	6, cr3, [r2, #-516]!
		DCI &9cf23681		; ldclls	6, cr3, [r2], #516
		DCI &9cf23681		; ldclls	6, cr3, [r2], #516
		DCI &9c723681		; ldclls	6, cr3, [r2], #-516
		DCI &9c723681		; ldclls	6, cr3, [r2], #-516
		DCI &9cd23621		; ldclls	6, cr3, [r2], {33}
		DCI &9cd23621		; ldclls	6, cr3, [r2], {33}
		DCI &ad923681		; ldcge	6, cr3, [r2, #516]
		DCI &ad923681		; ldcge	6, cr3, [r2, #516]
		DCI &ad123681		; ldcge	6, cr3, [r2, #-516]
		DCI &ad123681		; ldcge	6, cr3, [r2, #-516]
		DCI &adb23681		; ldcge	6, cr3, [r2, #516]!
		DCI &adb23681		; ldcge	6, cr3, [r2, #516]!
		DCI &ad323681		; ldcge	6, cr3, [r2, #-516]!
		DCI &ad323681		; ldcge	6, cr3, [r2, #-516]!
		DCI &acb23681		; ldcge	6, cr3, [r2], #516
		DCI &acb23681		; ldcge	6, cr3, [r2], #516
		DCI &ac323681		; ldcge	6, cr3, [r2], #-516
		DCI &ac323681		; ldcge	6, cr3, [r2], #-516
		DCI &ac923621		; ldcge	6, cr3, [r2], {33}
		DCI &ac923621		; ldcge	6, cr3, [r2], {33}
		DCI &add23681		; ldclge	6, cr3, [r2, #516]
		DCI &add23681		; ldclge	6, cr3, [r2, #516]
		DCI &ad523681		; ldclge	6, cr3, [r2, #-516]
		DCI &ad523681		; ldclge	6, cr3, [r2, #-516]
		DCI &adf23681		; ldclge	6, cr3, [r2, #516]!
		DCI &adf23681		; ldclge	6, cr3, [r2, #516]!
		DCI &ad723681		; ldclge	6, cr3, [r2, #-516]!
		DCI &ad723681		; ldclge	6, cr3, [r2, #-516]!
		DCI &acf23681		; ldclge	6, cr3, [r2], #516
		DCI &acf23681		; ldclge	6, cr3, [r2], #516
		DCI &ac723681		; ldclge	6, cr3, [r2], #-516
		DCI &ac723681		; ldclge	6, cr3, [r2], #-516
		DCI &acd23621		; ldclge	6, cr3, [r2], {33}
		DCI &acd23621		; ldclge	6, cr3, [r2], {33}
		DCI &add23681		; ldclge	6, cr3, [r2, #516]
		DCI &add23681		; ldclge	6, cr3, [r2, #516]
		DCI &ad523681		; ldclge	6, cr3, [r2, #-516]
		DCI &ad523681		; ldclge	6, cr3, [r2, #-516]
		DCI &adf23681		; ldclge	6, cr3, [r2, #516]!
		DCI &adf23681		; ldclge	6, cr3, [r2, #516]!
		DCI &ad723681		; ldclge	6, cr3, [r2, #-516]!
		DCI &ad723681		; ldclge	6, cr3, [r2, #-516]!
		DCI &acf23681		; ldclge	6, cr3, [r2], #516
		DCI &acf23681		; ldclge	6, cr3, [r2], #516
		DCI &ac723681		; ldclge	6, cr3, [r2], #-516
		DCI &ac723681		; ldclge	6, cr3, [r2], #-516
		DCI &acd23621		; ldclge	6, cr3, [r2], {33}
		DCI &acd23621		; ldclge	6, cr3, [r2], {33}
		DCI &bd923681		; ldclt	6, cr3, [r2, #516]
		DCI &bd923681		; ldclt	6, cr3, [r2, #516]
		DCI &bd123681		; ldclt	6, cr3, [r2, #-516]
		DCI &bd123681		; ldclt	6, cr3, [r2, #-516]
		DCI &bdb23681		; ldclt	6, cr3, [r2, #516]!
		DCI &bdb23681		; ldclt	6, cr3, [r2, #516]!
		DCI &bd323681		; ldclt	6, cr3, [r2, #-516]!
		DCI &bd323681		; ldclt	6, cr3, [r2, #-516]!
		DCI &bcb23681		; ldclt	6, cr3, [r2], #516
		DCI &bcb23681		; ldclt	6, cr3, [r2], #516
		DCI &bc323681		; ldclt	6, cr3, [r2], #-516
		DCI &bc323681		; ldclt	6, cr3, [r2], #-516
		DCI &bc923621		; ldclt	6, cr3, [r2], {33}
		DCI &bc923621		; ldclt	6, cr3, [r2], {33}
		DCI &bdd23681		; ldcllt	6, cr3, [r2, #516]
		DCI &bdd23681		; ldcllt	6, cr3, [r2, #516]
		DCI &bd523681		; ldcllt	6, cr3, [r2, #-516]
		DCI &bd523681		; ldcllt	6, cr3, [r2, #-516]
		DCI &bdf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI &bdf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI &bd723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI &bd723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI &bcf23681		; ldcllt	6, cr3, [r2], #516
		DCI &bcf23681		; ldcllt	6, cr3, [r2], #516
		DCI &bc723681		; ldcllt	6, cr3, [r2], #-516
		DCI &bc723681		; ldcllt	6, cr3, [r2], #-516
		DCI &bcd23621		; ldcllt	6, cr3, [r2], {33}
		DCI &bcd23621		; ldcllt	6, cr3, [r2], {33}
		DCI &bdd23681		; ldcllt	6, cr3, [r2, #516]
		DCI &bdd23681		; ldcllt	6, cr3, [r2, #516]
		DCI &bd523681		; ldcllt	6, cr3, [r2, #-516]
		DCI &bd523681		; ldcllt	6, cr3, [r2, #-516]
		DCI &bdf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI &bdf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI &bd723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI &bd723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI &bcf23681		; ldcllt	6, cr3, [r2], #516
		DCI &bcf23681		; ldcllt	6, cr3, [r2], #516
		DCI &bc723681		; ldcllt	6, cr3, [r2], #-516
		DCI &bc723681		; ldcllt	6, cr3, [r2], #-516
		DCI &bcd23621		; ldcllt	6, cr3, [r2], {33}
		DCI &bcd23621		; ldcllt	6, cr3, [r2], {33}
		DCI &cd923681		; ldcgt	6, cr3, [r2, #516]
		DCI &cd923681		; ldcgt	6, cr3, [r2, #516]
		DCI &cd123681		; ldcgt	6, cr3, [r2, #-516]
		DCI &cd123681		; ldcgt	6, cr3, [r2, #-516]
		DCI &cdb23681		; ldcgt	6, cr3, [r2, #516]!
		DCI &cdb23681		; ldcgt	6, cr3, [r2, #516]!
		DCI &cd323681		; ldcgt	6, cr3, [r2, #-516]!
		DCI &cd323681		; ldcgt	6, cr3, [r2, #-516]!
		DCI &ccb23681		; ldcgt	6, cr3, [r2], #516
		DCI &ccb23681		; ldcgt	6, cr3, [r2], #516
		DCI &cc323681		; ldcgt	6, cr3, [r2], #-516
		DCI &cc323681		; ldcgt	6, cr3, [r2], #-516
		DCI &cc923621		; ldcgt	6, cr3, [r2], {33}
		DCI &cc923621		; ldcgt	6, cr3, [r2], {33}
		DCI &cdd23681		; ldclgt	6, cr3, [r2, #516]
		DCI &cdd23681		; ldclgt	6, cr3, [r2, #516]
		DCI &cd523681		; ldclgt	6, cr3, [r2, #-516]
		DCI &cd523681		; ldclgt	6, cr3, [r2, #-516]
		DCI &cdf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI &cdf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI &cd723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI &cd723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI &ccf23681		; ldclgt	6, cr3, [r2], #516
		DCI &ccf23681		; ldclgt	6, cr3, [r2], #516
		DCI &cc723681		; ldclgt	6, cr3, [r2], #-516
		DCI &cc723681		; ldclgt	6, cr3, [r2], #-516
		DCI &ccd23621		; ldclgt	6, cr3, [r2], {33}
		DCI &ccd23621		; ldclgt	6, cr3, [r2], {33}
		DCI &cdd23681		; ldclgt	6, cr3, [r2, #516]
		DCI &cdd23681		; ldclgt	6, cr3, [r2, #516]
		DCI &cd523681		; ldclgt	6, cr3, [r2, #-516]
		DCI &cd523681		; ldclgt	6, cr3, [r2, #-516]
		DCI &cdf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI &cdf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI &cd723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI &cd723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI &ccf23681		; ldclgt	6, cr3, [r2], #516
		DCI &ccf23681		; ldclgt	6, cr3, [r2], #516
		DCI &cc723681		; ldclgt	6, cr3, [r2], #-516
		DCI &cc723681		; ldclgt	6, cr3, [r2], #-516
		DCI &ccd23621		; ldclgt	6, cr3, [r2], {33}
		DCI &ccd23621		; ldclgt	6, cr3, [r2], {33}
		DCI &dd923681		; ldcle	6, cr3, [r2, #516]
		DCI &dd923681		; ldcle	6, cr3, [r2, #516]
		DCI &dd123681		; ldcle	6, cr3, [r2, #-516]
		DCI &dd123681		; ldcle	6, cr3, [r2, #-516]
		DCI &ddb23681		; ldcle	6, cr3, [r2, #516]!
		DCI &ddb23681		; ldcle	6, cr3, [r2, #516]!
		DCI &dd323681		; ldcle	6, cr3, [r2, #-516]!
		DCI &dd323681		; ldcle	6, cr3, [r2, #-516]!
		DCI &dcb23681		; ldcle	6, cr3, [r2], #516
		DCI &dcb23681		; ldcle	6, cr3, [r2], #516
		DCI &dc323681		; ldcle	6, cr3, [r2], #-516
		DCI &dc323681		; ldcle	6, cr3, [r2], #-516
		DCI &dc923621		; ldcle	6, cr3, [r2], {33}
		DCI &dc923621		; ldcle	6, cr3, [r2], {33}
		DCI &ddd23681		; ldclle	6, cr3, [r2, #516]
		DCI &ddd23681		; ldclle	6, cr3, [r2, #516]
		DCI &dd523681		; ldclle	6, cr3, [r2, #-516]
		DCI &dd523681		; ldclle	6, cr3, [r2, #-516]
		DCI &ddf23681		; ldclle	6, cr3, [r2, #516]!
		DCI &ddf23681		; ldclle	6, cr3, [r2, #516]!
		DCI &dd723681		; ldclle	6, cr3, [r2, #-516]!
		DCI &dd723681		; ldclle	6, cr3, [r2, #-516]!
		DCI &dcf23681		; ldclle	6, cr3, [r2], #516
		DCI &dcf23681		; ldclle	6, cr3, [r2], #516
		DCI &dc723681		; ldclle	6, cr3, [r2], #-516
		DCI &dc723681		; ldclle	6, cr3, [r2], #-516
		DCI &dcd23621		; ldclle	6, cr3, [r2], {33}
		DCI &dcd23621		; ldclle	6, cr3, [r2], {33}
		DCI &ddd23681		; ldclle	6, cr3, [r2, #516]
		DCI &ddd23681		; ldclle	6, cr3, [r2, #516]
		DCI &dd523681		; ldclle	6, cr3, [r2, #-516]
		DCI &dd523681		; ldclle	6, cr3, [r2, #-516]
		DCI &ddf23681		; ldclle	6, cr3, [r2, #516]!
		DCI &ddf23681		; ldclle	6, cr3, [r2, #516]!
		DCI &dd723681		; ldclle	6, cr3, [r2, #-516]!
		DCI &dd723681		; ldclle	6, cr3, [r2, #-516]!
		DCI &dcf23681		; ldclle	6, cr3, [r2], #516
		DCI &dcf23681		; ldclle	6, cr3, [r2], #516
		DCI &dc723681		; ldclle	6, cr3, [r2], #-516
		DCI &dc723681		; ldclle	6, cr3, [r2], #-516
		DCI &dcd23621		; ldclle	6, cr3, [r2], {33}
		DCI &dcd23621		; ldclle	6, cr3, [r2], {33}
		DCI &ed923681		; ldc	6, cr3, [r2, #516]
		DCI &ed923681		; ldc	6, cr3, [r2, #516]
		DCI &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI &ecb23681		; ldc	6, cr3, [r2], #516
		DCI &ecb23681		; ldc	6, cr3, [r2], #516
		DCI &ec323681		; ldc	6, cr3, [r2], #-516
		DCI &ec323681		; ldc	6, cr3, [r2], #-516
		DCI &ec923621		; ldc	6, cr3, [r2], {33}
		DCI &ec923621		; ldc	6, cr3, [r2], {33}
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI &2d923681		; ldccs	6, cr3, [r2, #516]
		DCI &2d923681		; ldccs	6, cr3, [r2, #516]
		DCI &2d123681		; ldccs	6, cr3, [r2, #-516]
		DCI &2d123681		; ldccs	6, cr3, [r2, #-516]
		DCI &2db23681		; ldccs	6, cr3, [r2, #516]!
		DCI &2db23681		; ldccs	6, cr3, [r2, #516]!
		DCI &2d323681		; ldccs	6, cr3, [r2, #-516]!
		DCI &2d323681		; ldccs	6, cr3, [r2, #-516]!
		DCI &2cb23681		; ldccs	6, cr3, [r2], #516
		DCI &2cb23681		; ldccs	6, cr3, [r2], #516
		DCI &2c323681		; ldccs	6, cr3, [r2], #-516
		DCI &2c323681		; ldccs	6, cr3, [r2], #-516
		DCI &2c923621		; ldccs	6, cr3, [r2], {33}
		DCI &2c923621		; ldccs	6, cr3, [r2], {33}
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2dd23681		; ldclcs	6, cr3, [r2, #516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2d523681		; ldclcs	6, cr3, [r2, #-516]
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2df23681		; ldclcs	6, cr3, [r2, #516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2d723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2cf23681		; ldclcs	6, cr3, [r2], #516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2c723681		; ldclcs	6, cr3, [r2], #-516
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &2cd23621		; ldclcs	6, cr3, [r2], {33}
		DCI &3d923681		; ldccc	6, cr3, [r2, #516]
		DCI &3d923681		; ldccc	6, cr3, [r2, #516]
		DCI &3d123681		; ldccc	6, cr3, [r2, #-516]
		DCI &3d123681		; ldccc	6, cr3, [r2, #-516]
		DCI &3db23681		; ldccc	6, cr3, [r2, #516]!
		DCI &3db23681		; ldccc	6, cr3, [r2, #516]!
		DCI &3d323681		; ldccc	6, cr3, [r2, #-516]!
		DCI &3d323681		; ldccc	6, cr3, [r2, #-516]!
		DCI &3cb23681		; ldccc	6, cr3, [r2], #516
		DCI &3cb23681		; ldccc	6, cr3, [r2], #516
		DCI &3c323681		; ldccc	6, cr3, [r2], #-516
		DCI &3c323681		; ldccc	6, cr3, [r2], #-516
		DCI &3c923621		; ldccc	6, cr3, [r2], {33}
		DCI &3c923621		; ldccc	6, cr3, [r2], {33}
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3dd23681		; ldclcc	6, cr3, [r2, #516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3d523681		; ldclcc	6, cr3, [r2, #-516]
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3df23681		; ldclcc	6, cr3, [r2, #516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3d723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3cf23681		; ldclcc	6, cr3, [r2], #516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3c723681		; ldclcc	6, cr3, [r2], #-516
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &3cd23621		; ldclcc	6, cr3, [r2], {33}
		DCI &fd923681		; ldc2	6, cr3, [r2, #516]
		DCI &fd923681		; ldc2	6, cr3, [r2, #516]
		DCI &fd123681		; ldc2	6, cr3, [r2, #-516]
		DCI &fd123681		; ldc2	6, cr3, [r2, #-516]
		DCI &fdb23681		; ldc2	6, cr3, [r2, #516]!
		DCI &fdb23681		; ldc2	6, cr3, [r2, #516]!
		DCI &fd323681		; ldc2	6, cr3, [r2, #-516]!
		DCI &fd323681		; ldc2	6, cr3, [r2, #-516]!
		DCI &fcb23681		; ldc2	6, cr3, [r2], #516
		DCI &fcb23681		; ldc2	6, cr3, [r2], #516
		DCI &fc323681		; ldc2	6, cr3, [r2], #-516
		DCI &fc323681		; ldc2	6, cr3, [r2], #-516
		DCI &fc923621		; ldc2	6, cr3, [r2], {33}
		DCI &fc923621		; ldc2	6, cr3, [r2], {33}
		DCI &ed823681		; stc	6, cr3, [r2, #516]
		DCI &ed823681		; stc	6, cr3, [r2, #516]
		DCI &ed023681		; stc	6, cr3, [r2, #-516]
		DCI &ed023681		; stc	6, cr3, [r2, #-516]
		DCI &eda23681		; stc	6, cr3, [r2, #516]!
		DCI &eda23681		; stc	6, cr3, [r2, #516]!
		DCI &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI &eca23681		; stc	6, cr3, [r2], #516
		DCI &eca23681		; stc	6, cr3, [r2], #516
		DCI &ec223681		; stc	6, cr3, [r2], #-516
		DCI &ec223681		; stc	6, cr3, [r2], #-516
		DCI &ec823621		; stc	6, cr3, [r2], {33}
		DCI &ec823621		; stc	6, cr3, [r2], {33}
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &0d823681		; stceq	6, cr3, [r2, #516]
		DCI &0d823681		; stceq	6, cr3, [r2, #516]
		DCI &0d023681		; stceq	6, cr3, [r2, #-516]
		DCI &0d023681		; stceq	6, cr3, [r2, #-516]
		DCI &0da23681		; stceq	6, cr3, [r2, #516]!
		DCI &0da23681		; stceq	6, cr3, [r2, #516]!
		DCI &0d223681		; stceq	6, cr3, [r2, #-516]!
		DCI &0d223681		; stceq	6, cr3, [r2, #-516]!
		DCI &0ca23681		; stceq	6, cr3, [r2], #516
		DCI &0ca23681		; stceq	6, cr3, [r2], #516
		DCI &0c223681		; stceq	6, cr3, [r2], #-516
		DCI &0c223681		; stceq	6, cr3, [r2], #-516
		DCI &0c823621		; stceq	6, cr3, [r2], {33}
		DCI &0c823621		; stceq	6, cr3, [r2], {33}
		DCI &0dc23681		; stcleq	6, cr3, [r2, #516]
		DCI &0dc23681		; stcleq	6, cr3, [r2, #516]
		DCI &0d423681		; stcleq	6, cr3, [r2, #-516]
		DCI &0d423681		; stcleq	6, cr3, [r2, #-516]
		DCI &0de23681		; stcleq	6, cr3, [r2, #516]!
		DCI &0de23681		; stcleq	6, cr3, [r2, #516]!
		DCI &0d623681		; stcleq	6, cr3, [r2, #-516]!
		DCI &0d623681		; stcleq	6, cr3, [r2, #-516]!
		DCI &0ce23681		; stcleq	6, cr3, [r2], #516
		DCI &0ce23681		; stcleq	6, cr3, [r2], #516
		DCI &0c623681		; stcleq	6, cr3, [r2], #-516
		DCI &0c623681		; stcleq	6, cr3, [r2], #-516
		DCI &0cc23621		; stcleq	6, cr3, [r2], {33}
		DCI &0cc23621		; stcleq	6, cr3, [r2], {33}
		DCI &0dc23681		; stcleq	6, cr3, [r2, #516]
		DCI &0dc23681		; stcleq	6, cr3, [r2, #516]
		DCI &0d423681		; stcleq	6, cr3, [r2, #-516]
		DCI &0d423681		; stcleq	6, cr3, [r2, #-516]
		DCI &0de23681		; stcleq	6, cr3, [r2, #516]!
		DCI &0de23681		; stcleq	6, cr3, [r2, #516]!
		DCI &0d623681		; stcleq	6, cr3, [r2, #-516]!
		DCI &0d623681		; stcleq	6, cr3, [r2, #-516]!
		DCI &0ce23681		; stcleq	6, cr3, [r2], #516
		DCI &0ce23681		; stcleq	6, cr3, [r2], #516
		DCI &0c623681		; stcleq	6, cr3, [r2], #-516
		DCI &0c623681		; stcleq	6, cr3, [r2], #-516
		DCI &0cc23621		; stcleq	6, cr3, [r2], {33}
		DCI &0cc23621		; stcleq	6, cr3, [r2], {33}
		DCI &1d823681		; stcne	6, cr3, [r2, #516]
		DCI &1d823681		; stcne	6, cr3, [r2, #516]
		DCI &1d023681		; stcne	6, cr3, [r2, #-516]
		DCI &1d023681		; stcne	6, cr3, [r2, #-516]
		DCI &1da23681		; stcne	6, cr3, [r2, #516]!
		DCI &1da23681		; stcne	6, cr3, [r2, #516]!
		DCI &1d223681		; stcne	6, cr3, [r2, #-516]!
		DCI &1d223681		; stcne	6, cr3, [r2, #-516]!
		DCI &1ca23681		; stcne	6, cr3, [r2], #516
		DCI &1ca23681		; stcne	6, cr3, [r2], #516
		DCI &1c223681		; stcne	6, cr3, [r2], #-516
		DCI &1c223681		; stcne	6, cr3, [r2], #-516
		DCI &1c823621		; stcne	6, cr3, [r2], {33}
		DCI &1c823621		; stcne	6, cr3, [r2], {33}
		DCI &1dc23681		; stclne	6, cr3, [r2, #516]
		DCI &1dc23681		; stclne	6, cr3, [r2, #516]
		DCI &1d423681		; stclne	6, cr3, [r2, #-516]
		DCI &1d423681		; stclne	6, cr3, [r2, #-516]
		DCI &1de23681		; stclne	6, cr3, [r2, #516]!
		DCI &1de23681		; stclne	6, cr3, [r2, #516]!
		DCI &1d623681		; stclne	6, cr3, [r2, #-516]!
		DCI &1d623681		; stclne	6, cr3, [r2, #-516]!
		DCI &1ce23681		; stclne	6, cr3, [r2], #516
		DCI &1ce23681		; stclne	6, cr3, [r2], #516
		DCI &1c623681		; stclne	6, cr3, [r2], #-516
		DCI &1c623681		; stclne	6, cr3, [r2], #-516
		DCI &1cc23621		; stclne	6, cr3, [r2], {33}
		DCI &1cc23621		; stclne	6, cr3, [r2], {33}
		DCI &1dc23681		; stclne	6, cr3, [r2, #516]
		DCI &1dc23681		; stclne	6, cr3, [r2, #516]
		DCI &1d423681		; stclne	6, cr3, [r2, #-516]
		DCI &1d423681		; stclne	6, cr3, [r2, #-516]
		DCI &1de23681		; stclne	6, cr3, [r2, #516]!
		DCI &1de23681		; stclne	6, cr3, [r2, #516]!
		DCI &1d623681		; stclne	6, cr3, [r2, #-516]!
		DCI &1d623681		; stclne	6, cr3, [r2, #-516]!
		DCI &1ce23681		; stclne	6, cr3, [r2], #516
		DCI &1ce23681		; stclne	6, cr3, [r2], #516
		DCI &1c623681		; stclne	6, cr3, [r2], #-516
		DCI &1c623681		; stclne	6, cr3, [r2], #-516
		DCI &1cc23621		; stclne	6, cr3, [r2], {33}
		DCI &1cc23621		; stclne	6, cr3, [r2], {33}
		DCI &2d823681		; stccs	6, cr3, [r2, #516]
		DCI &2d823681		; stccs	6, cr3, [r2, #516]
		DCI &2d023681		; stccs	6, cr3, [r2, #-516]
		DCI &2d023681		; stccs	6, cr3, [r2, #-516]
		DCI &2da23681		; stccs	6, cr3, [r2, #516]!
		DCI &2da23681		; stccs	6, cr3, [r2, #516]!
		DCI &2d223681		; stccs	6, cr3, [r2, #-516]!
		DCI &2d223681		; stccs	6, cr3, [r2, #-516]!
		DCI &2ca23681		; stccs	6, cr3, [r2], #516
		DCI &2ca23681		; stccs	6, cr3, [r2], #516
		DCI &2c223681		; stccs	6, cr3, [r2], #-516
		DCI &2c223681		; stccs	6, cr3, [r2], #-516
		DCI &2c823621		; stccs	6, cr3, [r2], {33}
		DCI &2c823621		; stccs	6, cr3, [r2], {33}
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &3d823681		; stccc	6, cr3, [r2, #516]
		DCI &3d823681		; stccc	6, cr3, [r2, #516]
		DCI &3d023681		; stccc	6, cr3, [r2, #-516]
		DCI &3d023681		; stccc	6, cr3, [r2, #-516]
		DCI &3da23681		; stccc	6, cr3, [r2, #516]!
		DCI &3da23681		; stccc	6, cr3, [r2, #516]!
		DCI &3d223681		; stccc	6, cr3, [r2, #-516]!
		DCI &3d223681		; stccc	6, cr3, [r2, #-516]!
		DCI &3ca23681		; stccc	6, cr3, [r2], #516
		DCI &3ca23681		; stccc	6, cr3, [r2], #516
		DCI &3c223681		; stccc	6, cr3, [r2], #-516
		DCI &3c223681		; stccc	6, cr3, [r2], #-516
		DCI &3c823621		; stccc	6, cr3, [r2], {33}
		DCI &3c823621		; stccc	6, cr3, [r2], {33}
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &4d823681		; stcmi	6, cr3, [r2, #516]
		DCI &4d823681		; stcmi	6, cr3, [r2, #516]
		DCI &4d023681		; stcmi	6, cr3, [r2, #-516]
		DCI &4d023681		; stcmi	6, cr3, [r2, #-516]
		DCI &4da23681		; stcmi	6, cr3, [r2, #516]!
		DCI &4da23681		; stcmi	6, cr3, [r2, #516]!
		DCI &4d223681		; stcmi	6, cr3, [r2, #-516]!
		DCI &4d223681		; stcmi	6, cr3, [r2, #-516]!
		DCI &4ca23681		; stcmi	6, cr3, [r2], #516
		DCI &4ca23681		; stcmi	6, cr3, [r2], #516
		DCI &4c223681		; stcmi	6, cr3, [r2], #-516
		DCI &4c223681		; stcmi	6, cr3, [r2], #-516
		DCI &4c823621		; stcmi	6, cr3, [r2], {33}
		DCI &4c823621		; stcmi	6, cr3, [r2], {33}
		DCI &4dc23681		; stclmi	6, cr3, [r2, #516]
		DCI &4dc23681		; stclmi	6, cr3, [r2, #516]
		DCI &4d423681		; stclmi	6, cr3, [r2, #-516]
		DCI &4d423681		; stclmi	6, cr3, [r2, #-516]
		DCI &4de23681		; stclmi	6, cr3, [r2, #516]!
		DCI &4de23681		; stclmi	6, cr3, [r2, #516]!
		DCI &4d623681		; stclmi	6, cr3, [r2, #-516]!
		DCI &4d623681		; stclmi	6, cr3, [r2, #-516]!
		DCI &4ce23681		; stclmi	6, cr3, [r2], #516
		DCI &4ce23681		; stclmi	6, cr3, [r2], #516
		DCI &4c623681		; stclmi	6, cr3, [r2], #-516
		DCI &4c623681		; stclmi	6, cr3, [r2], #-516
		DCI &4cc23621		; stclmi	6, cr3, [r2], {33}
		DCI &4cc23621		; stclmi	6, cr3, [r2], {33}
		DCI &4dc23681		; stclmi	6, cr3, [r2, #516]
		DCI &4dc23681		; stclmi	6, cr3, [r2, #516]
		DCI &4d423681		; stclmi	6, cr3, [r2, #-516]
		DCI &4d423681		; stclmi	6, cr3, [r2, #-516]
		DCI &4de23681		; stclmi	6, cr3, [r2, #516]!
		DCI &4de23681		; stclmi	6, cr3, [r2, #516]!
		DCI &4d623681		; stclmi	6, cr3, [r2, #-516]!
		DCI &4d623681		; stclmi	6, cr3, [r2, #-516]!
		DCI &4ce23681		; stclmi	6, cr3, [r2], #516
		DCI &4ce23681		; stclmi	6, cr3, [r2], #516
		DCI &4c623681		; stclmi	6, cr3, [r2], #-516
		DCI &4c623681		; stclmi	6, cr3, [r2], #-516
		DCI &4cc23621		; stclmi	6, cr3, [r2], {33}
		DCI &4cc23621		; stclmi	6, cr3, [r2], {33}
		DCI &5d823681		; stcpl	6, cr3, [r2, #516]
		DCI &5d823681		; stcpl	6, cr3, [r2, #516]
		DCI &5d023681		; stcpl	6, cr3, [r2, #-516]
		DCI &5d023681		; stcpl	6, cr3, [r2, #-516]
		DCI &5da23681		; stcpl	6, cr3, [r2, #516]!
		DCI &5da23681		; stcpl	6, cr3, [r2, #516]!
		DCI &5d223681		; stcpl	6, cr3, [r2, #-516]!
		DCI &5d223681		; stcpl	6, cr3, [r2, #-516]!
		DCI &5ca23681		; stcpl	6, cr3, [r2], #516
		DCI &5ca23681		; stcpl	6, cr3, [r2], #516
		DCI &5c223681		; stcpl	6, cr3, [r2], #-516
		DCI &5c223681		; stcpl	6, cr3, [r2], #-516
		DCI &5c823621		; stcpl	6, cr3, [r2], {33}
		DCI &5c823621		; stcpl	6, cr3, [r2], {33}
		DCI &5dc23681		; stclpl	6, cr3, [r2, #516]
		DCI &5dc23681		; stclpl	6, cr3, [r2, #516]
		DCI &5d423681		; stclpl	6, cr3, [r2, #-516]
		DCI &5d423681		; stclpl	6, cr3, [r2, #-516]
		DCI &5de23681		; stclpl	6, cr3, [r2, #516]!
		DCI &5de23681		; stclpl	6, cr3, [r2, #516]!
		DCI &5d623681		; stclpl	6, cr3, [r2, #-516]!
		DCI &5d623681		; stclpl	6, cr3, [r2, #-516]!
		DCI &5ce23681		; stclpl	6, cr3, [r2], #516
		DCI &5ce23681		; stclpl	6, cr3, [r2], #516
		DCI &5c623681		; stclpl	6, cr3, [r2], #-516
		DCI &5c623681		; stclpl	6, cr3, [r2], #-516
		DCI &5cc23621		; stclpl	6, cr3, [r2], {33}
		DCI &5cc23621		; stclpl	6, cr3, [r2], {33}
		DCI &5dc23681		; stclpl	6, cr3, [r2, #516]
		DCI &5dc23681		; stclpl	6, cr3, [r2, #516]
		DCI &5d423681		; stclpl	6, cr3, [r2, #-516]
		DCI &5d423681		; stclpl	6, cr3, [r2, #-516]
		DCI &5de23681		; stclpl	6, cr3, [r2, #516]!
		DCI &5de23681		; stclpl	6, cr3, [r2, #516]!
		DCI &5d623681		; stclpl	6, cr3, [r2, #-516]!
		DCI &5d623681		; stclpl	6, cr3, [r2, #-516]!
		DCI &5ce23681		; stclpl	6, cr3, [r2], #516
		DCI &5ce23681		; stclpl	6, cr3, [r2], #516
		DCI &5c623681		; stclpl	6, cr3, [r2], #-516
		DCI &5c623681		; stclpl	6, cr3, [r2], #-516
		DCI &5cc23621		; stclpl	6, cr3, [r2], {33}
		DCI &5cc23621		; stclpl	6, cr3, [r2], {33}
		DCI &6d823681		; stcvs	6, cr3, [r2, #516]
		DCI &6d823681		; stcvs	6, cr3, [r2, #516]
		DCI &6d023681		; stcvs	6, cr3, [r2, #-516]
		DCI &6d023681		; stcvs	6, cr3, [r2, #-516]
		DCI &6da23681		; stcvs	6, cr3, [r2, #516]!
		DCI &6da23681		; stcvs	6, cr3, [r2, #516]!
		DCI &6d223681		; stcvs	6, cr3, [r2, #-516]!
		DCI &6d223681		; stcvs	6, cr3, [r2, #-516]!
		DCI &6ca23681		; stcvs	6, cr3, [r2], #516
		DCI &6ca23681		; stcvs	6, cr3, [r2], #516
		DCI &6c223681		; stcvs	6, cr3, [r2], #-516
		DCI &6c223681		; stcvs	6, cr3, [r2], #-516
		DCI &6c823621		; stcvs	6, cr3, [r2], {33}
		DCI &6c823621		; stcvs	6, cr3, [r2], {33}
		DCI &6dc23681		; stclvs	6, cr3, [r2, #516]
		DCI &6dc23681		; stclvs	6, cr3, [r2, #516]
		DCI &6d423681		; stclvs	6, cr3, [r2, #-516]
		DCI &6d423681		; stclvs	6, cr3, [r2, #-516]
		DCI &6de23681		; stclvs	6, cr3, [r2, #516]!
		DCI &6de23681		; stclvs	6, cr3, [r2, #516]!
		DCI &6d623681		; stclvs	6, cr3, [r2, #-516]!
		DCI &6d623681		; stclvs	6, cr3, [r2, #-516]!
		DCI &6ce23681		; stclvs	6, cr3, [r2], #516
		DCI &6ce23681		; stclvs	6, cr3, [r2], #516
		DCI &6c623681		; stclvs	6, cr3, [r2], #-516
		DCI &6c623681		; stclvs	6, cr3, [r2], #-516
		DCI &6cc23621		; stclvs	6, cr3, [r2], {33}
		DCI &6cc23621		; stclvs	6, cr3, [r2], {33}
		DCI &6dc23681		; stclvs	6, cr3, [r2, #516]
		DCI &6dc23681		; stclvs	6, cr3, [r2, #516]
		DCI &6d423681		; stclvs	6, cr3, [r2, #-516]
		DCI &6d423681		; stclvs	6, cr3, [r2, #-516]
		DCI &6de23681		; stclvs	6, cr3, [r2, #516]!
		DCI &6de23681		; stclvs	6, cr3, [r2, #516]!
		DCI &6d623681		; stclvs	6, cr3, [r2, #-516]!
		DCI &6d623681		; stclvs	6, cr3, [r2, #-516]!
		DCI &6ce23681		; stclvs	6, cr3, [r2], #516
		DCI &6ce23681		; stclvs	6, cr3, [r2], #516
		DCI &6c623681		; stclvs	6, cr3, [r2], #-516
		DCI &6c623681		; stclvs	6, cr3, [r2], #-516
		DCI &6cc23621		; stclvs	6, cr3, [r2], {33}
		DCI &6cc23621		; stclvs	6, cr3, [r2], {33}
		DCI &7d823681		; stcvc	6, cr3, [r2, #516]
		DCI &7d823681		; stcvc	6, cr3, [r2, #516]
		DCI &7d023681		; stcvc	6, cr3, [r2, #-516]
		DCI &7d023681		; stcvc	6, cr3, [r2, #-516]
		DCI &7da23681		; stcvc	6, cr3, [r2, #516]!
		DCI &7da23681		; stcvc	6, cr3, [r2, #516]!
		DCI &7d223681		; stcvc	6, cr3, [r2, #-516]!
		DCI &7d223681		; stcvc	6, cr3, [r2, #-516]!
		DCI &7ca23681		; stcvc	6, cr3, [r2], #516
		DCI &7ca23681		; stcvc	6, cr3, [r2], #516
		DCI &7c223681		; stcvc	6, cr3, [r2], #-516
		DCI &7c223681		; stcvc	6, cr3, [r2], #-516
		DCI &7c823621		; stcvc	6, cr3, [r2], {33}
		DCI &7c823621		; stcvc	6, cr3, [r2], {33}
		DCI &7dc23681		; stclvc	6, cr3, [r2, #516]
		DCI &7dc23681		; stclvc	6, cr3, [r2, #516]
		DCI &7d423681		; stclvc	6, cr3, [r2, #-516]
		DCI &7d423681		; stclvc	6, cr3, [r2, #-516]
		DCI &7de23681		; stclvc	6, cr3, [r2, #516]!
		DCI &7de23681		; stclvc	6, cr3, [r2, #516]!
		DCI &7d623681		; stclvc	6, cr3, [r2, #-516]!
		DCI &7d623681		; stclvc	6, cr3, [r2, #-516]!
		DCI &7ce23681		; stclvc	6, cr3, [r2], #516
		DCI &7ce23681		; stclvc	6, cr3, [r2], #516
		DCI &7c623681		; stclvc	6, cr3, [r2], #-516
		DCI &7c623681		; stclvc	6, cr3, [r2], #-516
		DCI &7cc23621		; stclvc	6, cr3, [r2], {33}
		DCI &7cc23621		; stclvc	6, cr3, [r2], {33}
		DCI &7dc23681		; stclvc	6, cr3, [r2, #516]
		DCI &7dc23681		; stclvc	6, cr3, [r2, #516]
		DCI &7d423681		; stclvc	6, cr3, [r2, #-516]
		DCI &7d423681		; stclvc	6, cr3, [r2, #-516]
		DCI &7de23681		; stclvc	6, cr3, [r2, #516]!
		DCI &7de23681		; stclvc	6, cr3, [r2, #516]!
		DCI &7d623681		; stclvc	6, cr3, [r2, #-516]!
		DCI &7d623681		; stclvc	6, cr3, [r2, #-516]!
		DCI &7ce23681		; stclvc	6, cr3, [r2], #516
		DCI &7ce23681		; stclvc	6, cr3, [r2], #516
		DCI &7c623681		; stclvc	6, cr3, [r2], #-516
		DCI &7c623681		; stclvc	6, cr3, [r2], #-516
		DCI &7cc23621		; stclvc	6, cr3, [r2], {33}
		DCI &7cc23621		; stclvc	6, cr3, [r2], {33}
		DCI &8d823681		; stchi	6, cr3, [r2, #516]
		DCI &8d823681		; stchi	6, cr3, [r2, #516]
		DCI &8d023681		; stchi	6, cr3, [r2, #-516]
		DCI &8d023681		; stchi	6, cr3, [r2, #-516]
		DCI &8da23681		; stchi	6, cr3, [r2, #516]!
		DCI &8da23681		; stchi	6, cr3, [r2, #516]!
		DCI &8d223681		; stchi	6, cr3, [r2, #-516]!
		DCI &8d223681		; stchi	6, cr3, [r2, #-516]!
		DCI &8ca23681		; stchi	6, cr3, [r2], #516
		DCI &8ca23681		; stchi	6, cr3, [r2], #516
		DCI &8c223681		; stchi	6, cr3, [r2], #-516
		DCI &8c223681		; stchi	6, cr3, [r2], #-516
		DCI &8c823621		; stchi	6, cr3, [r2], {33}
		DCI &8c823621		; stchi	6, cr3, [r2], {33}
		DCI &8dc23681		; stclhi	6, cr3, [r2, #516]
		DCI &8dc23681		; stclhi	6, cr3, [r2, #516]
		DCI &8d423681		; stclhi	6, cr3, [r2, #-516]
		DCI &8d423681		; stclhi	6, cr3, [r2, #-516]
		DCI &8de23681		; stclhi	6, cr3, [r2, #516]!
		DCI &8de23681		; stclhi	6, cr3, [r2, #516]!
		DCI &8d623681		; stclhi	6, cr3, [r2, #-516]!
		DCI &8d623681		; stclhi	6, cr3, [r2, #-516]!
		DCI &8ce23681		; stclhi	6, cr3, [r2], #516
		DCI &8ce23681		; stclhi	6, cr3, [r2], #516
		DCI &8c623681		; stclhi	6, cr3, [r2], #-516
		DCI &8c623681		; stclhi	6, cr3, [r2], #-516
		DCI &8cc23621		; stclhi	6, cr3, [r2], {33}
		DCI &8cc23621		; stclhi	6, cr3, [r2], {33}
		DCI &8dc23681		; stclhi	6, cr3, [r2, #516]
		DCI &8dc23681		; stclhi	6, cr3, [r2, #516]
		DCI &8d423681		; stclhi	6, cr3, [r2, #-516]
		DCI &8d423681		; stclhi	6, cr3, [r2, #-516]
		DCI &8de23681		; stclhi	6, cr3, [r2, #516]!
		DCI &8de23681		; stclhi	6, cr3, [r2, #516]!
		DCI &8d623681		; stclhi	6, cr3, [r2, #-516]!
		DCI &8d623681		; stclhi	6, cr3, [r2, #-516]!
		DCI &8ce23681		; stclhi	6, cr3, [r2], #516
		DCI &8ce23681		; stclhi	6, cr3, [r2], #516
		DCI &8c623681		; stclhi	6, cr3, [r2], #-516
		DCI &8c623681		; stclhi	6, cr3, [r2], #-516
		DCI &8cc23621		; stclhi	6, cr3, [r2], {33}
		DCI &8cc23621		; stclhi	6, cr3, [r2], {33}
		DCI &9d823681		; stcls	6, cr3, [r2, #516]
		DCI &9d823681		; stcls	6, cr3, [r2, #516]
		DCI &9d023681		; stcls	6, cr3, [r2, #-516]
		DCI &9d023681		; stcls	6, cr3, [r2, #-516]
		DCI &9da23681		; stcls	6, cr3, [r2, #516]!
		DCI &9da23681		; stcls	6, cr3, [r2, #516]!
		DCI &9d223681		; stcls	6, cr3, [r2, #-516]!
		DCI &9d223681		; stcls	6, cr3, [r2, #-516]!
		DCI &9ca23681		; stcls	6, cr3, [r2], #516
		DCI &9ca23681		; stcls	6, cr3, [r2], #516
		DCI &9c223681		; stcls	6, cr3, [r2], #-516
		DCI &9c223681		; stcls	6, cr3, [r2], #-516
		DCI &9c823621		; stcls	6, cr3, [r2], {33}
		DCI &9c823621		; stcls	6, cr3, [r2], {33}
		DCI &9dc23681		; stclls	6, cr3, [r2, #516]
		DCI &9dc23681		; stclls	6, cr3, [r2, #516]
		DCI &9d423681		; stclls	6, cr3, [r2, #-516]
		DCI &9d423681		; stclls	6, cr3, [r2, #-516]
		DCI &9de23681		; stclls	6, cr3, [r2, #516]!
		DCI &9de23681		; stclls	6, cr3, [r2, #516]!
		DCI &9d623681		; stclls	6, cr3, [r2, #-516]!
		DCI &9d623681		; stclls	6, cr3, [r2, #-516]!
		DCI &9ce23681		; stclls	6, cr3, [r2], #516
		DCI &9ce23681		; stclls	6, cr3, [r2], #516
		DCI &9c623681		; stclls	6, cr3, [r2], #-516
		DCI &9c623681		; stclls	6, cr3, [r2], #-516
		DCI &9cc23621		; stclls	6, cr3, [r2], {33}
		DCI &9cc23621		; stclls	6, cr3, [r2], {33}
		DCI &9dc23681		; stclls	6, cr3, [r2, #516]
		DCI &9dc23681		; stclls	6, cr3, [r2, #516]
		DCI &9d423681		; stclls	6, cr3, [r2, #-516]
		DCI &9d423681		; stclls	6, cr3, [r2, #-516]
		DCI &9de23681		; stclls	6, cr3, [r2, #516]!
		DCI &9de23681		; stclls	6, cr3, [r2, #516]!
		DCI &9d623681		; stclls	6, cr3, [r2, #-516]!
		DCI &9d623681		; stclls	6, cr3, [r2, #-516]!
		DCI &9ce23681		; stclls	6, cr3, [r2], #516
		DCI &9ce23681		; stclls	6, cr3, [r2], #516
		DCI &9c623681		; stclls	6, cr3, [r2], #-516
		DCI &9c623681		; stclls	6, cr3, [r2], #-516
		DCI &9cc23621		; stclls	6, cr3, [r2], {33}
		DCI &9cc23621		; stclls	6, cr3, [r2], {33}
		DCI &ad823681		; stcge	6, cr3, [r2, #516]
		DCI &ad823681		; stcge	6, cr3, [r2, #516]
		DCI &ad023681		; stcge	6, cr3, [r2, #-516]
		DCI &ad023681		; stcge	6, cr3, [r2, #-516]
		DCI &ada23681		; stcge	6, cr3, [r2, #516]!
		DCI &ada23681		; stcge	6, cr3, [r2, #516]!
		DCI &ad223681		; stcge	6, cr3, [r2, #-516]!
		DCI &ad223681		; stcge	6, cr3, [r2, #-516]!
		DCI &aca23681		; stcge	6, cr3, [r2], #516
		DCI &aca23681		; stcge	6, cr3, [r2], #516
		DCI &ac223681		; stcge	6, cr3, [r2], #-516
		DCI &ac223681		; stcge	6, cr3, [r2], #-516
		DCI &ac823621		; stcge	6, cr3, [r2], {33}
		DCI &ac823621		; stcge	6, cr3, [r2], {33}
		DCI &adc23681		; stclge	6, cr3, [r2, #516]
		DCI &adc23681		; stclge	6, cr3, [r2, #516]
		DCI &ad423681		; stclge	6, cr3, [r2, #-516]
		DCI &ad423681		; stclge	6, cr3, [r2, #-516]
		DCI &ade23681		; stclge	6, cr3, [r2, #516]!
		DCI &ade23681		; stclge	6, cr3, [r2, #516]!
		DCI &ad623681		; stclge	6, cr3, [r2, #-516]!
		DCI &ad623681		; stclge	6, cr3, [r2, #-516]!
		DCI &ace23681		; stclge	6, cr3, [r2], #516
		DCI &ace23681		; stclge	6, cr3, [r2], #516
		DCI &ac623681		; stclge	6, cr3, [r2], #-516
		DCI &ac623681		; stclge	6, cr3, [r2], #-516
		DCI &acc23621		; stclge	6, cr3, [r2], {33}
		DCI &acc23621		; stclge	6, cr3, [r2], {33}
		DCI &adc23681		; stclge	6, cr3, [r2, #516]
		DCI &adc23681		; stclge	6, cr3, [r2, #516]
		DCI &ad423681		; stclge	6, cr3, [r2, #-516]
		DCI &ad423681		; stclge	6, cr3, [r2, #-516]
		DCI &ade23681		; stclge	6, cr3, [r2, #516]!
		DCI &ade23681		; stclge	6, cr3, [r2, #516]!
		DCI &ad623681		; stclge	6, cr3, [r2, #-516]!
		DCI &ad623681		; stclge	6, cr3, [r2, #-516]!
		DCI &ace23681		; stclge	6, cr3, [r2], #516
		DCI &ace23681		; stclge	6, cr3, [r2], #516
		DCI &ac623681		; stclge	6, cr3, [r2], #-516
		DCI &ac623681		; stclge	6, cr3, [r2], #-516
		DCI &acc23621		; stclge	6, cr3, [r2], {33}
		DCI &acc23621		; stclge	6, cr3, [r2], {33}
		DCI &bd823681		; stclt	6, cr3, [r2, #516]
		DCI &bd823681		; stclt	6, cr3, [r2, #516]
		DCI &bd023681		; stclt	6, cr3, [r2, #-516]
		DCI &bd023681		; stclt	6, cr3, [r2, #-516]
		DCI &bda23681		; stclt	6, cr3, [r2, #516]!
		DCI &bda23681		; stclt	6, cr3, [r2, #516]!
		DCI &bd223681		; stclt	6, cr3, [r2, #-516]!
		DCI &bd223681		; stclt	6, cr3, [r2, #-516]!
		DCI &bca23681		; stclt	6, cr3, [r2], #516
		DCI &bca23681		; stclt	6, cr3, [r2], #516
		DCI &bc223681		; stclt	6, cr3, [r2], #-516
		DCI &bc223681		; stclt	6, cr3, [r2], #-516
		DCI &bc823621		; stclt	6, cr3, [r2], {33}
		DCI &bc823621		; stclt	6, cr3, [r2], {33}
		DCI &bdc23681		; stcllt	6, cr3, [r2, #516]
		DCI &bdc23681		; stcllt	6, cr3, [r2, #516]
		DCI &bd423681		; stcllt	6, cr3, [r2, #-516]
		DCI &bd423681		; stcllt	6, cr3, [r2, #-516]
		DCI &bde23681		; stcllt	6, cr3, [r2, #516]!
		DCI &bde23681		; stcllt	6, cr3, [r2, #516]!
		DCI &bd623681		; stcllt	6, cr3, [r2, #-516]!
		DCI &bd623681		; stcllt	6, cr3, [r2, #-516]!
		DCI &bce23681		; stcllt	6, cr3, [r2], #516
		DCI &bce23681		; stcllt	6, cr3, [r2], #516
		DCI &bc623681		; stcllt	6, cr3, [r2], #-516
		DCI &bc623681		; stcllt	6, cr3, [r2], #-516
		DCI &bcc23621		; stcllt	6, cr3, [r2], {33}
		DCI &bcc23621		; stcllt	6, cr3, [r2], {33}
		DCI &bdc23681		; stcllt	6, cr3, [r2, #516]
		DCI &bdc23681		; stcllt	6, cr3, [r2, #516]
		DCI &bd423681		; stcllt	6, cr3, [r2, #-516]
		DCI &bd423681		; stcllt	6, cr3, [r2, #-516]
		DCI &bde23681		; stcllt	6, cr3, [r2, #516]!
		DCI &bde23681		; stcllt	6, cr3, [r2, #516]!
		DCI &bd623681		; stcllt	6, cr3, [r2, #-516]!
		DCI &bd623681		; stcllt	6, cr3, [r2, #-516]!
		DCI &bce23681		; stcllt	6, cr3, [r2], #516
		DCI &bce23681		; stcllt	6, cr3, [r2], #516
		DCI &bc623681		; stcllt	6, cr3, [r2], #-516
		DCI &bc623681		; stcllt	6, cr3, [r2], #-516
		DCI &bcc23621		; stcllt	6, cr3, [r2], {33}
		DCI &bcc23621		; stcllt	6, cr3, [r2], {33}
		DCI &cd823681		; stcgt	6, cr3, [r2, #516]
		DCI &cd823681		; stcgt	6, cr3, [r2, #516]
		DCI &cd023681		; stcgt	6, cr3, [r2, #-516]
		DCI &cd023681		; stcgt	6, cr3, [r2, #-516]
		DCI &cda23681		; stcgt	6, cr3, [r2, #516]!
		DCI &cda23681		; stcgt	6, cr3, [r2, #516]!
		DCI &cd223681		; stcgt	6, cr3, [r2, #-516]!
		DCI &cd223681		; stcgt	6, cr3, [r2, #-516]!
		DCI &cca23681		; stcgt	6, cr3, [r2], #516
		DCI &cca23681		; stcgt	6, cr3, [r2], #516
		DCI &cc223681		; stcgt	6, cr3, [r2], #-516
		DCI &cc223681		; stcgt	6, cr3, [r2], #-516
		DCI &cc823621		; stcgt	6, cr3, [r2], {33}
		DCI &cc823621		; stcgt	6, cr3, [r2], {33}
		DCI &cdc23681		; stclgt	6, cr3, [r2, #516]
		DCI &cdc23681		; stclgt	6, cr3, [r2, #516]
		DCI &cd423681		; stclgt	6, cr3, [r2, #-516]
		DCI &cd423681		; stclgt	6, cr3, [r2, #-516]
		DCI &cde23681		; stclgt	6, cr3, [r2, #516]!
		DCI &cde23681		; stclgt	6, cr3, [r2, #516]!
		DCI &cd623681		; stclgt	6, cr3, [r2, #-516]!
		DCI &cd623681		; stclgt	6, cr3, [r2, #-516]!
		DCI &cce23681		; stclgt	6, cr3, [r2], #516
		DCI &cce23681		; stclgt	6, cr3, [r2], #516
		DCI &cc623681		; stclgt	6, cr3, [r2], #-516
		DCI &cc623681		; stclgt	6, cr3, [r2], #-516
		DCI &ccc23621		; stclgt	6, cr3, [r2], {33}
		DCI &ccc23621		; stclgt	6, cr3, [r2], {33}
		DCI &cdc23681		; stclgt	6, cr3, [r2, #516]
		DCI &cdc23681		; stclgt	6, cr3, [r2, #516]
		DCI &cd423681		; stclgt	6, cr3, [r2, #-516]
		DCI &cd423681		; stclgt	6, cr3, [r2, #-516]
		DCI &cde23681		; stclgt	6, cr3, [r2, #516]!
		DCI &cde23681		; stclgt	6, cr3, [r2, #516]!
		DCI &cd623681		; stclgt	6, cr3, [r2, #-516]!
		DCI &cd623681		; stclgt	6, cr3, [r2, #-516]!
		DCI &cce23681		; stclgt	6, cr3, [r2], #516
		DCI &cce23681		; stclgt	6, cr3, [r2], #516
		DCI &cc623681		; stclgt	6, cr3, [r2], #-516
		DCI &cc623681		; stclgt	6, cr3, [r2], #-516
		DCI &ccc23621		; stclgt	6, cr3, [r2], {33}
		DCI &ccc23621		; stclgt	6, cr3, [r2], {33}
		DCI &dd823681		; stcle	6, cr3, [r2, #516]
		DCI &dd823681		; stcle	6, cr3, [r2, #516]
		DCI &dd023681		; stcle	6, cr3, [r2, #-516]
		DCI &dd023681		; stcle	6, cr3, [r2, #-516]
		DCI &dda23681		; stcle	6, cr3, [r2, #516]!
		DCI &dda23681		; stcle	6, cr3, [r2, #516]!
		DCI &dd223681		; stcle	6, cr3, [r2, #-516]!
		DCI &dd223681		; stcle	6, cr3, [r2, #-516]!
		DCI &dca23681		; stcle	6, cr3, [r2], #516
		DCI &dca23681		; stcle	6, cr3, [r2], #516
		DCI &dc223681		; stcle	6, cr3, [r2], #-516
		DCI &dc223681		; stcle	6, cr3, [r2], #-516
		DCI &dc823621		; stcle	6, cr3, [r2], {33}
		DCI &dc823621		; stcle	6, cr3, [r2], {33}
		DCI &ddc23681		; stclle	6, cr3, [r2, #516]
		DCI &ddc23681		; stclle	6, cr3, [r2, #516]
		DCI &dd423681		; stclle	6, cr3, [r2, #-516]
		DCI &dd423681		; stclle	6, cr3, [r2, #-516]
		DCI &dde23681		; stclle	6, cr3, [r2, #516]!
		DCI &dde23681		; stclle	6, cr3, [r2, #516]!
		DCI &dd623681		; stclle	6, cr3, [r2, #-516]!
		DCI &dd623681		; stclle	6, cr3, [r2, #-516]!
		DCI &dce23681		; stclle	6, cr3, [r2], #516
		DCI &dce23681		; stclle	6, cr3, [r2], #516
		DCI &dc623681		; stclle	6, cr3, [r2], #-516
		DCI &dc623681		; stclle	6, cr3, [r2], #-516
		DCI &dcc23621		; stclle	6, cr3, [r2], {33}
		DCI &dcc23621		; stclle	6, cr3, [r2], {33}
		DCI &ddc23681		; stclle	6, cr3, [r2, #516]
		DCI &ddc23681		; stclle	6, cr3, [r2, #516]
		DCI &dd423681		; stclle	6, cr3, [r2, #-516]
		DCI &dd423681		; stclle	6, cr3, [r2, #-516]
		DCI &dde23681		; stclle	6, cr3, [r2, #516]!
		DCI &dde23681		; stclle	6, cr3, [r2, #516]!
		DCI &dd623681		; stclle	6, cr3, [r2, #-516]!
		DCI &dd623681		; stclle	6, cr3, [r2, #-516]!
		DCI &dce23681		; stclle	6, cr3, [r2], #516
		DCI &dce23681		; stclle	6, cr3, [r2], #516
		DCI &dc623681		; stclle	6, cr3, [r2], #-516
		DCI &dc623681		; stclle	6, cr3, [r2], #-516
		DCI &dcc23621		; stclle	6, cr3, [r2], {33}
		DCI &dcc23621		; stclle	6, cr3, [r2], {33}
		DCI &ed823681		; stc	6, cr3, [r2, #516]
		DCI &ed823681		; stc	6, cr3, [r2, #516]
		DCI &ed023681		; stc	6, cr3, [r2, #-516]
		DCI &ed023681		; stc	6, cr3, [r2, #-516]
		DCI &eda23681		; stc	6, cr3, [r2, #516]!
		DCI &eda23681		; stc	6, cr3, [r2, #516]!
		DCI &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI &eca23681		; stc	6, cr3, [r2], #516
		DCI &eca23681		; stc	6, cr3, [r2], #516
		DCI &ec223681		; stc	6, cr3, [r2], #-516
		DCI &ec223681		; stc	6, cr3, [r2], #-516
		DCI &ec823621		; stc	6, cr3, [r2], {33}
		DCI &ec823621		; stc	6, cr3, [r2], {33}
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &edc23681		; stcl	6, cr3, [r2, #516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ece23681		; stcl	6, cr3, [r2], #516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ec623681		; stcl	6, cr3, [r2], #-516
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI &2d823681		; stccs	6, cr3, [r2, #516]
		DCI &2d823681		; stccs	6, cr3, [r2, #516]
		DCI &2d023681		; stccs	6, cr3, [r2, #-516]
		DCI &2d023681		; stccs	6, cr3, [r2, #-516]
		DCI &2da23681		; stccs	6, cr3, [r2, #516]!
		DCI &2da23681		; stccs	6, cr3, [r2, #516]!
		DCI &2d223681		; stccs	6, cr3, [r2, #-516]!
		DCI &2d223681		; stccs	6, cr3, [r2, #-516]!
		DCI &2ca23681		; stccs	6, cr3, [r2], #516
		DCI &2ca23681		; stccs	6, cr3, [r2], #516
		DCI &2c223681		; stccs	6, cr3, [r2], #-516
		DCI &2c223681		; stccs	6, cr3, [r2], #-516
		DCI &2c823621		; stccs	6, cr3, [r2], {33}
		DCI &2c823621		; stccs	6, cr3, [r2], {33}
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2dc23681		; stclcs	6, cr3, [r2, #516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2d423681		; stclcs	6, cr3, [r2, #-516]
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2de23681		; stclcs	6, cr3, [r2, #516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2d623681		; stclcs	6, cr3, [r2, #-516]!
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2ce23681		; stclcs	6, cr3, [r2], #516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2c623681		; stclcs	6, cr3, [r2], #-516
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &2cc23621		; stclcs	6, cr3, [r2], {33}
		DCI &3d823681		; stccc	6, cr3, [r2, #516]
		DCI &3d823681		; stccc	6, cr3, [r2, #516]
		DCI &3d023681		; stccc	6, cr3, [r2, #-516]
		DCI &3d023681		; stccc	6, cr3, [r2, #-516]
		DCI &3da23681		; stccc	6, cr3, [r2, #516]!
		DCI &3da23681		; stccc	6, cr3, [r2, #516]!
		DCI &3d223681		; stccc	6, cr3, [r2, #-516]!
		DCI &3d223681		; stccc	6, cr3, [r2, #-516]!
		DCI &3ca23681		; stccc	6, cr3, [r2], #516
		DCI &3ca23681		; stccc	6, cr3, [r2], #516
		DCI &3c223681		; stccc	6, cr3, [r2], #-516
		DCI &3c223681		; stccc	6, cr3, [r2], #-516
		DCI &3c823621		; stccc	6, cr3, [r2], {33}
		DCI &3c823621		; stccc	6, cr3, [r2], {33}
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3dc23681		; stclcc	6, cr3, [r2, #516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3d423681		; stclcc	6, cr3, [r2, #-516]
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3de23681		; stclcc	6, cr3, [r2, #516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3d623681		; stclcc	6, cr3, [r2, #-516]!
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3ce23681		; stclcc	6, cr3, [r2], #516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3c623681		; stclcc	6, cr3, [r2], #-516
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &3cc23621		; stclcc	6, cr3, [r2], {33}
		DCI &fd823681		; stc2	6, cr3, [r2, #516]
		DCI &fd823681		; stc2	6, cr3, [r2, #516]
		DCI &fd023681		; stc2	6, cr3, [r2, #-516]
		DCI &fd023681		; stc2	6, cr3, [r2, #-516]
		DCI &fda23681		; stc2	6, cr3, [r2, #516]!
		DCI &fda23681		; stc2	6, cr3, [r2, #516]!
		DCI &fd223681		; stc2	6, cr3, [r2, #-516]!
		DCI &fd223681		; stc2	6, cr3, [r2, #-516]!
		DCI &fca23681		; stc2	6, cr3, [r2], #516
		DCI &fca23681		; stc2	6, cr3, [r2], #516
		DCI &fc223681		; stc2	6, cr3, [r2], #-516
		DCI &fc223681		; stc2	6, cr3, [r2], #-516
		DCI &fc823621		; stc2	6, cr3, [r2], {33}
		DCI &fc823621		; stc2	6, cr3, [r2], {33}

		THUMB
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.N &bf01		; itttt	eq
		DCI.W &ed923681		; ldceq	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldceq	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldceq	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldceq	6, cr3, [r2, #-516]
		DCI.N &bf01		; itttt	eq
		DCI.W &edb23681		; ldceq	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldceq	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldceq	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldceq	6, cr3, [r2, #-516]!
		DCI.N &bf01		; itttt	eq
		DCI.W &ecb23681		; ldceq	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldceq	6, cr3, [r2], #516
		DCI.W &ec323681		; ldceq	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldceq	6, cr3, [r2], #-516
		DCI.N &bf04		; itt	eq
		DCI.W &ec923621		; ldceq	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldceq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf01		; itttt	eq
		DCI.W &edd23681		; ldcleq	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldcleq	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldcleq	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldcleq	6, cr3, [r2, #-516]
		DCI.N &bf01		; itttt	eq
		DCI.W &edf23681		; ldcleq	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldcleq	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI.N &bf01		; itttt	eq
		DCI.W &ecf23681		; ldcleq	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldcleq	6, cr3, [r2], #516
		DCI.W &ec723681		; ldcleq	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldcleq	6, cr3, [r2], #-516
		DCI.N &bf07		; ittee	eq
		DCI.W &ecd23621		; ldcleq	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldcleq	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldcne	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcne	6, cr3, [r2, #516]
		DCI.N &bf1f		; itttt	ne
		DCI.W &ed123681		; ldcne	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcne	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldcne	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcne	6, cr3, [r2, #516]!
		DCI.N &bf1f		; itttt	ne
		DCI.W &ed323681		; ldcne	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcne	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldcne	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcne	6, cr3, [r2], #516
		DCI.N &bf1f		; itttt	ne
		DCI.W &ec323681		; ldcne	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcne	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldcne	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf1f		; itttt	ne
		DCI.W &edd23681		; ldclne	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclne	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclne	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclne	6, cr3, [r2, #-516]
		DCI.N &bf1f		; itttt	ne
		DCI.W &edf23681		; ldclne	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclne	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclne	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclne	6, cr3, [r2, #-516]!
		DCI.N &bf1f		; itttt	ne
		DCI.W &ecf23681		; ldclne	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclne	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclne	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclne	6, cr3, [r2], #-516
		DCI.N &bf1c		; itt	ne
		DCI.W &ecd23621		; ldclne	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclne	6, cr3, [r2], {33}
		DCI.N &bf21		; itttt	cs
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.N &bf24		; itt	cs
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf21		; itttt	cs
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.N &bf27		; ittee	cs
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.N &bf3f		; itttt	cc
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf3f		; itttt	cc
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.N &bf3c		; itt	cc
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.N &bf41		; itttt	mi
		DCI.W &ed923681		; ldcmi	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcmi	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldcmi	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcmi	6, cr3, [r2, #-516]
		DCI.N &bf41		; itttt	mi
		DCI.W &edb23681		; ldcmi	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcmi	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldcmi	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcmi	6, cr3, [r2, #-516]!
		DCI.N &bf41		; itttt	mi
		DCI.W &ecb23681		; ldcmi	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcmi	6, cr3, [r2], #516
		DCI.W &ec323681		; ldcmi	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcmi	6, cr3, [r2], #-516
		DCI.N &bf44		; itt	mi
		DCI.W &ec923621		; ldcmi	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf41		; itttt	mi
		DCI.W &edd23681		; ldclmi	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclmi	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclmi	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclmi	6, cr3, [r2, #-516]
		DCI.N &bf41		; itttt	mi
		DCI.W &edf23681		; ldclmi	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclmi	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI.N &bf41		; itttt	mi
		DCI.W &ecf23681		; ldclmi	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclmi	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclmi	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclmi	6, cr3, [r2], #-516
		DCI.N &bf47		; ittee	mi
		DCI.W &ecd23621		; ldclmi	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclmi	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldcpl	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcpl	6, cr3, [r2, #516]
		DCI.N &bf5f		; itttt	pl
		DCI.W &ed123681		; ldcpl	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcpl	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldcpl	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcpl	6, cr3, [r2, #516]!
		DCI.N &bf5f		; itttt	pl
		DCI.W &ed323681		; ldcpl	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcpl	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldcpl	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcpl	6, cr3, [r2], #516
		DCI.N &bf5f		; itttt	pl
		DCI.W &ec323681		; ldcpl	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcpl	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldcpl	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf5f		; itttt	pl
		DCI.W &edd23681		; ldclpl	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclpl	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclpl	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclpl	6, cr3, [r2, #-516]
		DCI.N &bf5f		; itttt	pl
		DCI.W &edf23681		; ldclpl	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclpl	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI.N &bf5f		; itttt	pl
		DCI.W &ecf23681		; ldclpl	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclpl	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclpl	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclpl	6, cr3, [r2], #-516
		DCI.N &bf5c		; itt	pl
		DCI.W &ecd23621		; ldclpl	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclpl	6, cr3, [r2], {33}
		DCI.N &bf61		; itttt	vs
		DCI.W &ed923681		; ldcvs	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcvs	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldcvs	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcvs	6, cr3, [r2, #-516]
		DCI.N &bf61		; itttt	vs
		DCI.W &edb23681		; ldcvs	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcvs	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldcvs	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcvs	6, cr3, [r2, #-516]!
		DCI.N &bf61		; itttt	vs
		DCI.W &ecb23681		; ldcvs	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcvs	6, cr3, [r2], #516
		DCI.W &ec323681		; ldcvs	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcvs	6, cr3, [r2], #-516
		DCI.N &bf64		; itt	vs
		DCI.W &ec923621		; ldcvs	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf61		; itttt	vs
		DCI.W &edd23681		; ldclvs	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclvs	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclvs	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclvs	6, cr3, [r2, #-516]
		DCI.N &bf61		; itttt	vs
		DCI.W &edf23681		; ldclvs	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclvs	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI.N &bf61		; itttt	vs
		DCI.W &ecf23681		; ldclvs	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclvs	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclvs	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclvs	6, cr3, [r2], #-516
		DCI.N &bf67		; ittee	vs
		DCI.W &ecd23621		; ldclvs	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclvs	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldcvc	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcvc	6, cr3, [r2, #516]
		DCI.N &bf7f		; itttt	vc
		DCI.W &ed123681		; ldcvc	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcvc	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldcvc	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcvc	6, cr3, [r2, #516]!
		DCI.N &bf7f		; itttt	vc
		DCI.W &ed323681		; ldcvc	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcvc	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldcvc	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcvc	6, cr3, [r2], #516
		DCI.N &bf7f		; itttt	vc
		DCI.W &ec323681		; ldcvc	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcvc	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldcvc	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf7f		; itttt	vc
		DCI.W &edd23681		; ldclvc	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclvc	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclvc	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclvc	6, cr3, [r2, #-516]
		DCI.N &bf7f		; itttt	vc
		DCI.W &edf23681		; ldclvc	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclvc	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI.N &bf7f		; itttt	vc
		DCI.W &ecf23681		; ldclvc	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclvc	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclvc	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclvc	6, cr3, [r2], #-516
		DCI.N &bf7c		; itt	vc
		DCI.W &ecd23621		; ldclvc	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclvc	6, cr3, [r2], {33}
		DCI.N &bf81		; itttt	hi
		DCI.W &ed923681		; ldchi	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldchi	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldchi	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldchi	6, cr3, [r2, #-516]
		DCI.N &bf81		; itttt	hi
		DCI.W &edb23681		; ldchi	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldchi	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldchi	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldchi	6, cr3, [r2, #-516]!
		DCI.N &bf81		; itttt	hi
		DCI.W &ecb23681		; ldchi	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldchi	6, cr3, [r2], #516
		DCI.W &ec323681		; ldchi	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldchi	6, cr3, [r2], #-516
		DCI.N &bf84		; itt	hi
		DCI.W &ec923621		; ldchi	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldchi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf81		; itttt	hi
		DCI.W &edd23681		; ldclhi	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclhi	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclhi	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclhi	6, cr3, [r2, #-516]
		DCI.N &bf81		; itttt	hi
		DCI.W &edf23681		; ldclhi	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclhi	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI.N &bf81		; itttt	hi
		DCI.W &ecf23681		; ldclhi	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclhi	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclhi	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclhi	6, cr3, [r2], #-516
		DCI.N &bf87		; ittee	hi
		DCI.W &ecd23621		; ldclhi	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclhi	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldcls	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcls	6, cr3, [r2, #516]
		DCI.N &bf9f		; itttt	ls
		DCI.W &ed123681		; ldcls	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcls	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldcls	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcls	6, cr3, [r2, #516]!
		DCI.N &bf9f		; itttt	ls
		DCI.W &ed323681		; ldcls	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcls	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldcls	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcls	6, cr3, [r2], #516
		DCI.N &bf9f		; itttt	ls
		DCI.W &ec323681		; ldcls	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcls	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldcls	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf9f		; itttt	ls
		DCI.W &edd23681		; ldclls	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclls	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclls	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclls	6, cr3, [r2, #-516]
		DCI.N &bf9f		; itttt	ls
		DCI.W &edf23681		; ldclls	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclls	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclls	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclls	6, cr3, [r2, #-516]!
		DCI.N &bf9f		; itttt	ls
		DCI.W &ecf23681		; ldclls	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclls	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclls	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclls	6, cr3, [r2], #-516
		DCI.N &bf9c		; itt	ls
		DCI.W &ecd23621		; ldclls	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclls	6, cr3, [r2], {33}
		DCI.N &bfa1		; itttt	ge
		DCI.W &ed923681		; ldcge	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcge	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldcge	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcge	6, cr3, [r2, #-516]
		DCI.N &bfa1		; itttt	ge
		DCI.W &edb23681		; ldcge	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcge	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldcge	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcge	6, cr3, [r2, #-516]!
		DCI.N &bfa1		; itttt	ge
		DCI.W &ecb23681		; ldcge	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcge	6, cr3, [r2], #516
		DCI.W &ec323681		; ldcge	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcge	6, cr3, [r2], #-516
		DCI.N &bfa4		; itt	ge
		DCI.W &ec923621		; ldcge	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfa1		; itttt	ge
		DCI.W &edd23681		; ldclge	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclge	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclge	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclge	6, cr3, [r2, #-516]
		DCI.N &bfa1		; itttt	ge
		DCI.W &edf23681		; ldclge	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclge	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclge	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclge	6, cr3, [r2, #-516]!
		DCI.N &bfa1		; itttt	ge
		DCI.W &ecf23681		; ldclge	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclge	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclge	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclge	6, cr3, [r2], #-516
		DCI.N &bfa7		; ittee	ge
		DCI.W &ecd23621		; ldclge	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclge	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldclt	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldclt	6, cr3, [r2, #516]
		DCI.N &bfbf		; itttt	lt
		DCI.W &ed123681		; ldclt	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldclt	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldclt	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldclt	6, cr3, [r2, #516]!
		DCI.N &bfbf		; itttt	lt
		DCI.W &ed323681		; ldclt	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldclt	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldclt	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldclt	6, cr3, [r2], #516
		DCI.N &bfbf		; itttt	lt
		DCI.W &ec323681		; ldclt	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldclt	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldclt	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldclt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfbf		; itttt	lt
		DCI.W &edd23681		; ldcllt	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldcllt	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldcllt	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldcllt	6, cr3, [r2, #-516]
		DCI.N &bfbf		; itttt	lt
		DCI.W &edf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI.N &bfbf		; itttt	lt
		DCI.W &ecf23681		; ldcllt	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldcllt	6, cr3, [r2], #516
		DCI.W &ec723681		; ldcllt	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldcllt	6, cr3, [r2], #-516
		DCI.N &bfbc		; itt	lt
		DCI.W &ecd23621		; ldcllt	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldcllt	6, cr3, [r2], {33}
		DCI.N &bfc1		; itttt	gt
		DCI.W &ed923681		; ldcgt	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcgt	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldcgt	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcgt	6, cr3, [r2, #-516]
		DCI.N &bfc1		; itttt	gt
		DCI.W &edb23681		; ldcgt	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcgt	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldcgt	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcgt	6, cr3, [r2, #-516]!
		DCI.N &bfc1		; itttt	gt
		DCI.W &ecb23681		; ldcgt	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcgt	6, cr3, [r2], #516
		DCI.W &ec323681		; ldcgt	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcgt	6, cr3, [r2], #-516
		DCI.N &bfc4		; itt	gt
		DCI.W &ec923621		; ldcgt	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfc1		; itttt	gt
		DCI.W &edd23681		; ldclgt	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclgt	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclgt	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclgt	6, cr3, [r2, #-516]
		DCI.N &bfc1		; itttt	gt
		DCI.W &edf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI.N &bfc1		; itttt	gt
		DCI.W &ecf23681		; ldclgt	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclgt	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclgt	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclgt	6, cr3, [r2], #-516
		DCI.N &bfc7		; ittee	gt
		DCI.W &ecd23621		; ldclgt	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclgt	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldcle	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldcle	6, cr3, [r2, #516]
		DCI.N &bfdf		; itttt	le
		DCI.W &ed123681		; ldcle	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldcle	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldcle	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldcle	6, cr3, [r2, #516]!
		DCI.N &bfdf		; itttt	le
		DCI.W &ed323681		; ldcle	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldcle	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldcle	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldcle	6, cr3, [r2], #516
		DCI.N &bfdf		; itttt	le
		DCI.W &ec323681		; ldcle	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldcle	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldcle	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldcle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfdf		; itttt	le
		DCI.W &edd23681		; ldclle	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclle	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclle	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclle	6, cr3, [r2, #-516]
		DCI.N &bfdf		; itttt	le
		DCI.W &edf23681		; ldclle	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclle	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclle	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclle	6, cr3, [r2, #-516]!
		DCI.N &bfdf		; itttt	le
		DCI.W &ecf23681		; ldclle	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclle	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclle	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclle	6, cr3, [r2], #-516
		DCI.N &bfdc		; itt	le
		DCI.W &ecd23621		; ldclle	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclle	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.N &bf21		; itttt	cs
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.N &bf24		; itt	cs
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf21		; itttt	cs
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.N &bf27		; ittee	cs
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.N &bf3f		; itttt	cc
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf3f		; itttt	cc
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.N &bf3c		; itt	cc
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.W &fd923681		; ldc2	6, cr3, [r2, #516]
		DCI.W &fd923681		; ldc2	6, cr3, [r2, #516]
		DCI.W &fd123681		; ldc2	6, cr3, [r2, #-516]
		DCI.W &fd123681		; ldc2	6, cr3, [r2, #-516]
		DCI.W &fdb23681		; ldc2	6, cr3, [r2, #516]!
		DCI.W &fdb23681		; ldc2	6, cr3, [r2, #516]!
		DCI.W &fd323681		; ldc2	6, cr3, [r2, #-516]!
		DCI.W &fd323681		; ldc2	6, cr3, [r2, #-516]!
		DCI.W &fcb23681		; ldc2	6, cr3, [r2], #516
		DCI.W &fcb23681		; ldc2	6, cr3, [r2], #516
		DCI.W &fc323681		; ldc2	6, cr3, [r2], #-516
		DCI.W &fc323681		; ldc2	6, cr3, [r2], #-516
		DCI.W &fc923621		; ldc2	6, cr3, [r2], {33}
		DCI.W &fc923621		; ldc2	6, cr3, [r2], {33}
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.N &bf01		; itttt	eq
		DCI.W &ed823681		; stceq	6, cr3, [r2, #516]
		DCI.W &ed823681		; stceq	6, cr3, [r2, #516]
		DCI.W &ed023681		; stceq	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stceq	6, cr3, [r2, #-516]
		DCI.N &bf01		; itttt	eq
		DCI.W &eda23681		; stceq	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stceq	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stceq	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stceq	6, cr3, [r2, #-516]!
		DCI.N &bf01		; itttt	eq
		DCI.W &eca23681		; stceq	6, cr3, [r2], #516
		DCI.W &eca23681		; stceq	6, cr3, [r2], #516
		DCI.W &ec223681		; stceq	6, cr3, [r2], #-516
		DCI.W &ec223681		; stceq	6, cr3, [r2], #-516
		DCI.N &bf04		; itt	eq
		DCI.W &ec823621		; stceq	6, cr3, [r2], {33}
		DCI.W &ec823621		; stceq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf01		; itttt	eq
		DCI.W &edc23681		; stcleq	6, cr3, [r2, #516]
		DCI.W &edc23681		; stcleq	6, cr3, [r2, #516]
		DCI.W &ed423681		; stcleq	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stcleq	6, cr3, [r2, #-516]
		DCI.N &bf01		; itttt	eq
		DCI.W &ede23681		; stcleq	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stcleq	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stcleq	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stcleq	6, cr3, [r2, #-516]!
		DCI.N &bf01		; itttt	eq
		DCI.W &ece23681		; stcleq	6, cr3, [r2], #516
		DCI.W &ece23681		; stcleq	6, cr3, [r2], #516
		DCI.W &ec623681		; stcleq	6, cr3, [r2], #-516
		DCI.W &ec623681		; stcleq	6, cr3, [r2], #-516
		DCI.N &bf07		; ittee	eq
		DCI.W &ecc23621		; stcleq	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stcleq	6, cr3, [r2], {33}
		DCI.W &ed823681		; stcne	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcne	6, cr3, [r2, #516]
		DCI.N &bf1f		; itttt	ne
		DCI.W &ed023681		; stcne	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcne	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stcne	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcne	6, cr3, [r2, #516]!
		DCI.N &bf1f		; itttt	ne
		DCI.W &ed223681		; stcne	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcne	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stcne	6, cr3, [r2], #516
		DCI.W &eca23681		; stcne	6, cr3, [r2], #516
		DCI.N &bf1f		; itttt	ne
		DCI.W &ec223681		; stcne	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcne	6, cr3, [r2], #-516
		DCI.W &ec823621		; stcne	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf1f		; itttt	ne
		DCI.W &edc23681		; stclne	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclne	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclne	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclne	6, cr3, [r2, #-516]
		DCI.N &bf1f		; itttt	ne
		DCI.W &ede23681		; stclne	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclne	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclne	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclne	6, cr3, [r2, #-516]!
		DCI.N &bf1f		; itttt	ne
		DCI.W &ece23681		; stclne	6, cr3, [r2], #516
		DCI.W &ece23681		; stclne	6, cr3, [r2], #516
		DCI.W &ec623681		; stclne	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclne	6, cr3, [r2], #-516
		DCI.N &bf1c		; itt	ne
		DCI.W &ecc23621		; stclne	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclne	6, cr3, [r2], {33}
		DCI.N &bf21		; itttt	cs
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.N &bf24		; itt	cs
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf21		; itttt	cs
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.N &bf27		; ittee	cs
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.N &bf3f		; itttt	cc
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf3f		; itttt	cc
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.N &bf3c		; itt	cc
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.N &bf41		; itttt	mi
		DCI.W &ed823681		; stcmi	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcmi	6, cr3, [r2, #516]
		DCI.W &ed023681		; stcmi	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcmi	6, cr3, [r2, #-516]
		DCI.N &bf41		; itttt	mi
		DCI.W &eda23681		; stcmi	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcmi	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stcmi	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcmi	6, cr3, [r2, #-516]!
		DCI.N &bf41		; itttt	mi
		DCI.W &eca23681		; stcmi	6, cr3, [r2], #516
		DCI.W &eca23681		; stcmi	6, cr3, [r2], #516
		DCI.W &ec223681		; stcmi	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcmi	6, cr3, [r2], #-516
		DCI.N &bf44		; itt	mi
		DCI.W &ec823621		; stcmi	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf41		; itttt	mi
		DCI.W &edc23681		; stclmi	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclmi	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclmi	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclmi	6, cr3, [r2, #-516]
		DCI.N &bf41		; itttt	mi
		DCI.W &ede23681		; stclmi	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclmi	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclmi	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclmi	6, cr3, [r2, #-516]!
		DCI.N &bf41		; itttt	mi
		DCI.W &ece23681		; stclmi	6, cr3, [r2], #516
		DCI.W &ece23681		; stclmi	6, cr3, [r2], #516
		DCI.W &ec623681		; stclmi	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclmi	6, cr3, [r2], #-516
		DCI.N &bf47		; ittee	mi
		DCI.W &ecc23621		; stclmi	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclmi	6, cr3, [r2], {33}
		DCI.W &ed823681		; stcpl	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcpl	6, cr3, [r2, #516]
		DCI.N &bf5f		; itttt	pl
		DCI.W &ed023681		; stcpl	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcpl	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stcpl	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcpl	6, cr3, [r2, #516]!
		DCI.N &bf5f		; itttt	pl
		DCI.W &ed223681		; stcpl	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcpl	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stcpl	6, cr3, [r2], #516
		DCI.W &eca23681		; stcpl	6, cr3, [r2], #516
		DCI.N &bf5f		; itttt	pl
		DCI.W &ec223681		; stcpl	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcpl	6, cr3, [r2], #-516
		DCI.W &ec823621		; stcpl	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf5f		; itttt	pl
		DCI.W &edc23681		; stclpl	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclpl	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclpl	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclpl	6, cr3, [r2, #-516]
		DCI.N &bf5f		; itttt	pl
		DCI.W &ede23681		; stclpl	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclpl	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclpl	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclpl	6, cr3, [r2, #-516]!
		DCI.N &bf5f		; itttt	pl
		DCI.W &ece23681		; stclpl	6, cr3, [r2], #516
		DCI.W &ece23681		; stclpl	6, cr3, [r2], #516
		DCI.W &ec623681		; stclpl	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclpl	6, cr3, [r2], #-516
		DCI.N &bf5c		; itt	pl
		DCI.W &ecc23621		; stclpl	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclpl	6, cr3, [r2], {33}
		DCI.N &bf61		; itttt	vs
		DCI.W &ed823681		; stcvs	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcvs	6, cr3, [r2, #516]
		DCI.W &ed023681		; stcvs	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcvs	6, cr3, [r2, #-516]
		DCI.N &bf61		; itttt	vs
		DCI.W &eda23681		; stcvs	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcvs	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stcvs	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcvs	6, cr3, [r2, #-516]!
		DCI.N &bf61		; itttt	vs
		DCI.W &eca23681		; stcvs	6, cr3, [r2], #516
		DCI.W &eca23681		; stcvs	6, cr3, [r2], #516
		DCI.W &ec223681		; stcvs	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcvs	6, cr3, [r2], #-516
		DCI.N &bf64		; itt	vs
		DCI.W &ec823621		; stcvs	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf61		; itttt	vs
		DCI.W &edc23681		; stclvs	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclvs	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclvs	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclvs	6, cr3, [r2, #-516]
		DCI.N &bf61		; itttt	vs
		DCI.W &ede23681		; stclvs	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclvs	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclvs	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclvs	6, cr3, [r2, #-516]!
		DCI.N &bf61		; itttt	vs
		DCI.W &ece23681		; stclvs	6, cr3, [r2], #516
		DCI.W &ece23681		; stclvs	6, cr3, [r2], #516
		DCI.W &ec623681		; stclvs	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclvs	6, cr3, [r2], #-516
		DCI.N &bf67		; ittee	vs
		DCI.W &ecc23621		; stclvs	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclvs	6, cr3, [r2], {33}
		DCI.W &ed823681		; stcvc	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcvc	6, cr3, [r2, #516]
		DCI.N &bf7f		; itttt	vc
		DCI.W &ed023681		; stcvc	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcvc	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stcvc	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcvc	6, cr3, [r2, #516]!
		DCI.N &bf7f		; itttt	vc
		DCI.W &ed223681		; stcvc	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcvc	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stcvc	6, cr3, [r2], #516
		DCI.W &eca23681		; stcvc	6, cr3, [r2], #516
		DCI.N &bf7f		; itttt	vc
		DCI.W &ec223681		; stcvc	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcvc	6, cr3, [r2], #-516
		DCI.W &ec823621		; stcvc	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf7f		; itttt	vc
		DCI.W &edc23681		; stclvc	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclvc	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclvc	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclvc	6, cr3, [r2, #-516]
		DCI.N &bf7f		; itttt	vc
		DCI.W &ede23681		; stclvc	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclvc	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclvc	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclvc	6, cr3, [r2, #-516]!
		DCI.N &bf7f		; itttt	vc
		DCI.W &ece23681		; stclvc	6, cr3, [r2], #516
		DCI.W &ece23681		; stclvc	6, cr3, [r2], #516
		DCI.W &ec623681		; stclvc	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclvc	6, cr3, [r2], #-516
		DCI.N &bf7c		; itt	vc
		DCI.W &ecc23621		; stclvc	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclvc	6, cr3, [r2], {33}
		DCI.N &bf81		; itttt	hi
		DCI.W &ed823681		; stchi	6, cr3, [r2, #516]
		DCI.W &ed823681		; stchi	6, cr3, [r2, #516]
		DCI.W &ed023681		; stchi	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stchi	6, cr3, [r2, #-516]
		DCI.N &bf81		; itttt	hi
		DCI.W &eda23681		; stchi	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stchi	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stchi	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stchi	6, cr3, [r2, #-516]!
		DCI.N &bf81		; itttt	hi
		DCI.W &eca23681		; stchi	6, cr3, [r2], #516
		DCI.W &eca23681		; stchi	6, cr3, [r2], #516
		DCI.W &ec223681		; stchi	6, cr3, [r2], #-516
		DCI.W &ec223681		; stchi	6, cr3, [r2], #-516
		DCI.N &bf84		; itt	hi
		DCI.W &ec823621		; stchi	6, cr3, [r2], {33}
		DCI.W &ec823621		; stchi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf81		; itttt	hi
		DCI.W &edc23681		; stclhi	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclhi	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclhi	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclhi	6, cr3, [r2, #-516]
		DCI.N &bf81		; itttt	hi
		DCI.W &ede23681		; stclhi	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclhi	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclhi	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclhi	6, cr3, [r2, #-516]!
		DCI.N &bf81		; itttt	hi
		DCI.W &ece23681		; stclhi	6, cr3, [r2], #516
		DCI.W &ece23681		; stclhi	6, cr3, [r2], #516
		DCI.W &ec623681		; stclhi	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclhi	6, cr3, [r2], #-516
		DCI.N &bf87		; ittee	hi
		DCI.W &ecc23621		; stclhi	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclhi	6, cr3, [r2], {33}
		DCI.W &ed823681		; stcls	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcls	6, cr3, [r2, #516]
		DCI.N &bf9f		; itttt	ls
		DCI.W &ed023681		; stcls	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcls	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stcls	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcls	6, cr3, [r2, #516]!
		DCI.N &bf9f		; itttt	ls
		DCI.W &ed223681		; stcls	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcls	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stcls	6, cr3, [r2], #516
		DCI.W &eca23681		; stcls	6, cr3, [r2], #516
		DCI.N &bf9f		; itttt	ls
		DCI.W &ec223681		; stcls	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcls	6, cr3, [r2], #-516
		DCI.W &ec823621		; stcls	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf9f		; itttt	ls
		DCI.W &edc23681		; stclls	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclls	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclls	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclls	6, cr3, [r2, #-516]
		DCI.N &bf9f		; itttt	ls
		DCI.W &ede23681		; stclls	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclls	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclls	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclls	6, cr3, [r2, #-516]!
		DCI.N &bf9f		; itttt	ls
		DCI.W &ece23681		; stclls	6, cr3, [r2], #516
		DCI.W &ece23681		; stclls	6, cr3, [r2], #516
		DCI.W &ec623681		; stclls	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclls	6, cr3, [r2], #-516
		DCI.N &bf9c		; itt	ls
		DCI.W &ecc23621		; stclls	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclls	6, cr3, [r2], {33}
		DCI.N &bfa1		; itttt	ge
		DCI.W &ed823681		; stcge	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcge	6, cr3, [r2, #516]
		DCI.W &ed023681		; stcge	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcge	6, cr3, [r2, #-516]
		DCI.N &bfa1		; itttt	ge
		DCI.W &eda23681		; stcge	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcge	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stcge	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcge	6, cr3, [r2, #-516]!
		DCI.N &bfa1		; itttt	ge
		DCI.W &eca23681		; stcge	6, cr3, [r2], #516
		DCI.W &eca23681		; stcge	6, cr3, [r2], #516
		DCI.W &ec223681		; stcge	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcge	6, cr3, [r2], #-516
		DCI.N &bfa4		; itt	ge
		DCI.W &ec823621		; stcge	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfa1		; itttt	ge
		DCI.W &edc23681		; stclge	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclge	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclge	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclge	6, cr3, [r2, #-516]
		DCI.N &bfa1		; itttt	ge
		DCI.W &ede23681		; stclge	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclge	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclge	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclge	6, cr3, [r2, #-516]!
		DCI.N &bfa1		; itttt	ge
		DCI.W &ece23681		; stclge	6, cr3, [r2], #516
		DCI.W &ece23681		; stclge	6, cr3, [r2], #516
		DCI.W &ec623681		; stclge	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclge	6, cr3, [r2], #-516
		DCI.N &bfa7		; ittee	ge
		DCI.W &ecc23621		; stclge	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclge	6, cr3, [r2], {33}
		DCI.W &ed823681		; stclt	6, cr3, [r2, #516]
		DCI.W &ed823681		; stclt	6, cr3, [r2, #516]
		DCI.N &bfbf		; itttt	lt
		DCI.W &ed023681		; stclt	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stclt	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stclt	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stclt	6, cr3, [r2, #516]!
		DCI.N &bfbf		; itttt	lt
		DCI.W &ed223681		; stclt	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stclt	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stclt	6, cr3, [r2], #516
		DCI.W &eca23681		; stclt	6, cr3, [r2], #516
		DCI.N &bfbf		; itttt	lt
		DCI.W &ec223681		; stclt	6, cr3, [r2], #-516
		DCI.W &ec223681		; stclt	6, cr3, [r2], #-516
		DCI.W &ec823621		; stclt	6, cr3, [r2], {33}
		DCI.W &ec823621		; stclt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfbf		; itttt	lt
		DCI.W &edc23681		; stcllt	6, cr3, [r2, #516]
		DCI.W &edc23681		; stcllt	6, cr3, [r2, #516]
		DCI.W &ed423681		; stcllt	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stcllt	6, cr3, [r2, #-516]
		DCI.N &bfbf		; itttt	lt
		DCI.W &ede23681		; stcllt	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stcllt	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stcllt	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stcllt	6, cr3, [r2, #-516]!
		DCI.N &bfbf		; itttt	lt
		DCI.W &ece23681		; stcllt	6, cr3, [r2], #516
		DCI.W &ece23681		; stcllt	6, cr3, [r2], #516
		DCI.W &ec623681		; stcllt	6, cr3, [r2], #-516
		DCI.W &ec623681		; stcllt	6, cr3, [r2], #-516
		DCI.N &bfbc		; itt	lt
		DCI.W &ecc23621		; stcllt	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stcllt	6, cr3, [r2], {33}
		DCI.N &bfc1		; itttt	gt
		DCI.W &ed823681		; stcgt	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcgt	6, cr3, [r2, #516]
		DCI.W &ed023681		; stcgt	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcgt	6, cr3, [r2, #-516]
		DCI.N &bfc1		; itttt	gt
		DCI.W &eda23681		; stcgt	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcgt	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stcgt	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcgt	6, cr3, [r2, #-516]!
		DCI.N &bfc1		; itttt	gt
		DCI.W &eca23681		; stcgt	6, cr3, [r2], #516
		DCI.W &eca23681		; stcgt	6, cr3, [r2], #516
		DCI.W &ec223681		; stcgt	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcgt	6, cr3, [r2], #-516
		DCI.N &bfc4		; itt	gt
		DCI.W &ec823621		; stcgt	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfc1		; itttt	gt
		DCI.W &edc23681		; stclgt	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclgt	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclgt	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclgt	6, cr3, [r2, #-516]
		DCI.N &bfc1		; itttt	gt
		DCI.W &ede23681		; stclgt	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclgt	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclgt	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclgt	6, cr3, [r2, #-516]!
		DCI.N &bfc1		; itttt	gt
		DCI.W &ece23681		; stclgt	6, cr3, [r2], #516
		DCI.W &ece23681		; stclgt	6, cr3, [r2], #516
		DCI.W &ec623681		; stclgt	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclgt	6, cr3, [r2], #-516
		DCI.N &bfc7		; ittee	gt
		DCI.W &ecc23621		; stclgt	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclgt	6, cr3, [r2], {33}
		DCI.W &ed823681		; stcle	6, cr3, [r2, #516]
		DCI.W &ed823681		; stcle	6, cr3, [r2, #516]
		DCI.N &bfdf		; itttt	le
		DCI.W &ed023681		; stcle	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stcle	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stcle	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stcle	6, cr3, [r2, #516]!
		DCI.N &bfdf		; itttt	le
		DCI.W &ed223681		; stcle	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stcle	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stcle	6, cr3, [r2], #516
		DCI.W &eca23681		; stcle	6, cr3, [r2], #516
		DCI.N &bfdf		; itttt	le
		DCI.W &ec223681		; stcle	6, cr3, [r2], #-516
		DCI.W &ec223681		; stcle	6, cr3, [r2], #-516
		DCI.W &ec823621		; stcle	6, cr3, [r2], {33}
		DCI.W &ec823621		; stcle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bfdf		; itttt	le
		DCI.W &edc23681		; stclle	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclle	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclle	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclle	6, cr3, [r2, #-516]
		DCI.N &bfdf		; itttt	le
		DCI.W &ede23681		; stclle	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclle	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclle	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclle	6, cr3, [r2, #-516]!
		DCI.N &bfdf		; itttt	le
		DCI.W &ece23681		; stclle	6, cr3, [r2], #516
		DCI.W &ece23681		; stclle	6, cr3, [r2], #516
		DCI.W &ec623681		; stclle	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclle	6, cr3, [r2], #-516
		DCI.N &bfdc		; itt	le
		DCI.W &ecc23621		; stclle	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclle	6, cr3, [r2], {33}
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.N &bf21		; itttt	cs
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.N &bf24		; itt	cs
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf21		; itttt	cs
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.N &bf21		; itttt	cs
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.N &bf21		; itttt	cs
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.N &bf27		; ittee	cs
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.N &bf3f		; itttt	cc
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf00		; nop.n
		DCI.N &bf3f		; itttt	cc
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.N &bf3f		; itttt	cc
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.N &bf3f		; itttt	cc
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.N &bf3c		; itt	cc
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.W &fd823681		; stc2	6, cr3, [r2, #516]
		DCI.W &fd823681		; stc2	6, cr3, [r2, #516]
		DCI.W &fd023681		; stc2	6, cr3, [r2, #-516]
		DCI.W &fd023681		; stc2	6, cr3, [r2, #-516]
		DCI.W &fda23681		; stc2	6, cr3, [r2, #516]!
		DCI.W &fda23681		; stc2	6, cr3, [r2, #516]!
		DCI.W &fd223681		; stc2	6, cr3, [r2, #-516]!
		DCI.W &fd223681		; stc2	6, cr3, [r2, #-516]!
		DCI.W &fca23681		; stc2	6, cr3, [r2], #516
		DCI.W &fca23681		; stc2	6, cr3, [r2], #516
		DCI.W &fc223681		; stc2	6, cr3, [r2], #-516
		DCI.W &fc223681		; stc2	6, cr3, [r2], #-516
		DCI.W &fc823621		; stc2	6, cr3, [r2], {33}
		DCI.W &fc823621		; stc2	6, cr3, [r2], {33}

		THUMB
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed923681		; ldceq	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed123681		; ldceq	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &edb23681		; ldceq	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed323681		; ldceq	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ecb23681		; ldceq	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec323681		; ldceq	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec923621		; ldceq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &edd23681		; ldcleq	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed523681		; ldcleq	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &edf23681		; ldcleq	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed723681		; ldcleq	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ecf23681		; ldcleq	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec723681		; ldcleq	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ecd23621		; ldcleq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed923681		; ldcne	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed123681		; ldcne	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &edb23681		; ldcne	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed323681		; ldcne	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ecb23681		; ldcne	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec323681		; ldcne	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec923621		; ldcne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &edd23681		; ldclne	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed523681		; ldclne	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &edf23681		; ldclne	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed723681		; ldclne	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ecf23681		; ldclne	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec723681		; ldclne	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ecd23621		; ldclne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed923681		; ldcmi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed123681		; ldcmi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &edb23681		; ldcmi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed323681		; ldcmi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ecb23681		; ldcmi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec323681		; ldcmi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec923621		; ldcmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &edd23681		; ldclmi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed523681		; ldclmi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &edf23681		; ldclmi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed723681		; ldclmi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ecf23681		; ldclmi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec723681		; ldclmi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ecd23621		; ldclmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed923681		; ldcpl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed123681		; ldcpl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &edb23681		; ldcpl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed323681		; ldcpl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ecb23681		; ldcpl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec323681		; ldcpl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec923621		; ldcpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &edd23681		; ldclpl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed523681		; ldclpl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &edf23681		; ldclpl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed723681		; ldclpl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ecf23681		; ldclpl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec723681		; ldclpl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ecd23621		; ldclpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed923681		; ldcvs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed123681		; ldcvs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &edb23681		; ldcvs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed323681		; ldcvs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ecb23681		; ldcvs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec323681		; ldcvs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec923621		; ldcvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &edd23681		; ldclvs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed523681		; ldclvs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &edf23681		; ldclvs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed723681		; ldclvs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ecf23681		; ldclvs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec723681		; ldclvs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ecd23621		; ldclvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed923681		; ldcvc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed123681		; ldcvc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &edb23681		; ldcvc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed323681		; ldcvc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ecb23681		; ldcvc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec323681		; ldcvc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec923621		; ldcvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &edd23681		; ldclvc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed523681		; ldclvc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &edf23681		; ldclvc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed723681		; ldclvc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ecf23681		; ldclvc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec723681		; ldclvc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ecd23621		; ldclvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed923681		; ldchi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed123681		; ldchi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &edb23681		; ldchi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed323681		; ldchi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ecb23681		; ldchi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec323681		; ldchi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec923621		; ldchi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &edd23681		; ldclhi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed523681		; ldclhi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &edf23681		; ldclhi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed723681		; ldclhi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ecf23681		; ldclhi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec723681		; ldclhi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ecd23621		; ldclhi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed923681		; ldcls	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed123681		; ldcls	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &edb23681		; ldcls	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed323681		; ldcls	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ecb23681		; ldcls	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec323681		; ldcls	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec923621		; ldcls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &edd23681		; ldclls	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed523681		; ldclls	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &edf23681		; ldclls	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed723681		; ldclls	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ecf23681		; ldclls	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec723681		; ldclls	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ecd23621		; ldclls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed923681		; ldcge	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed123681		; ldcge	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &edb23681		; ldcge	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed323681		; ldcge	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ecb23681		; ldcge	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec323681		; ldcge	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec923621		; ldcge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &edd23681		; ldclge	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed523681		; ldclge	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &edf23681		; ldclge	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed723681		; ldclge	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ecf23681		; ldclge	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec723681		; ldclge	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ecd23621		; ldclge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed923681		; ldclt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed123681		; ldclt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &edb23681		; ldclt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed323681		; ldclt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ecb23681		; ldclt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec323681		; ldclt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec923621		; ldclt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &edd23681		; ldcllt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed523681		; ldcllt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &edf23681		; ldcllt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed723681		; ldcllt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ecf23681		; ldcllt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec723681		; ldcllt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ecd23621		; ldcllt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed923681		; ldcgt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed123681		; ldcgt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &edb23681		; ldcgt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed323681		; ldcgt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ecb23681		; ldcgt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec323681		; ldcgt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec923621		; ldcgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &edd23681		; ldclgt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed523681		; ldclgt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &edf23681		; ldclgt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed723681		; ldclgt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ecf23681		; ldclgt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec723681		; ldclgt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ecd23621		; ldclgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed923681		; ldcle	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed123681		; ldcle	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &edb23681		; ldcle	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed323681		; ldcle	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ecb23681		; ldcle	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec323681		; ldcle	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec923621		; ldcle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &edd23681		; ldclle	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed523681		; ldclle	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &edf23681		; ldclle	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed723681		; ldclle	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ecf23681		; ldclle	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec723681		; ldclle	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ecd23621		; ldclle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.W &ed923681		; ldc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed123681		; ldc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &edb23681		; ldc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed323681		; ldc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ecb23681		; ldc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec323681		; ldc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ec923621		; ldc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.W &edd23681		; ldcl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed523681		; ldcl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &edf23681		; ldcl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed723681		; ldcl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ecf23681		; ldcl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec723681		; ldcl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ecd23621		; ldcl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed923681		; ldccs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed123681		; ldccs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edb23681		; ldccs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed323681		; ldccs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecb23681		; ldccs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec323681		; ldccs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec923621		; ldccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edd23681		; ldclcs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed523681		; ldclcs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edf23681		; ldclcs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed723681		; ldclcs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecf23681		; ldclcs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec723681		; ldclcs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecd23621		; ldclcs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed923681		; ldccc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed123681		; ldccc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edb23681		; ldccc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed323681		; ldccc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecb23681		; ldccc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec323681		; ldccc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec923621		; ldccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edd23681		; ldclcc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed523681		; ldclcc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edf23681		; ldclcc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed723681		; ldclcc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecf23681		; ldclcc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec723681		; ldclcc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecd23621		; ldclcc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.W &fd923681		; ldc2	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &fd123681		; ldc2	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &fdb23681		; ldc2	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &fd323681		; ldc2	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &fcb23681		; ldc2	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &fc323681		; ldc2	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &fc923621		; ldc2	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed823681		; stceq	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed023681		; stceq	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &eda23681		; stceq	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed223681		; stceq	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &eca23681		; stceq	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec223681		; stceq	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec823621		; stceq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &edc23681		; stcleq	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed423681		; stcleq	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ede23681		; stcleq	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ed623681		; stcleq	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ece23681		; stcleq	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ec623681		; stcleq	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf08		; it	eq
		DCI.W &ecc23621		; stcleq	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed823681		; stcne	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed023681		; stcne	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &eda23681		; stcne	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed223681		; stcne	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &eca23681		; stcne	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec223681		; stcne	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec823621		; stcne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &edc23681		; stclne	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed423681		; stclne	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ede23681		; stclne	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ed623681		; stclne	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ece23681		; stclne	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ec623681		; stclne	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf18		; it	ne
		DCI.W &ecc23621		; stclne	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed823681		; stcmi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed023681		; stcmi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &eda23681		; stcmi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed223681		; stcmi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &eca23681		; stcmi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec223681		; stcmi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec823621		; stcmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &edc23681		; stclmi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed423681		; stclmi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ede23681		; stclmi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ed623681		; stclmi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ece23681		; stclmi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ec623681		; stclmi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf48		; it	mi
		DCI.W &ecc23621		; stclmi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed823681		; stcpl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed023681		; stcpl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &eda23681		; stcpl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed223681		; stcpl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &eca23681		; stcpl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec223681		; stcpl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec823621		; stcpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &edc23681		; stclpl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed423681		; stclpl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ede23681		; stclpl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ed623681		; stclpl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ece23681		; stclpl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ec623681		; stclpl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf58		; it	pl
		DCI.W &ecc23621		; stclpl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed823681		; stcvs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed023681		; stcvs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &eda23681		; stcvs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed223681		; stcvs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &eca23681		; stcvs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec223681		; stcvs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec823621		; stcvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &edc23681		; stclvs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed423681		; stclvs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ede23681		; stclvs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ed623681		; stclvs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ece23681		; stclvs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ec623681		; stclvs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf68		; it	vs
		DCI.W &ecc23621		; stclvs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed823681		; stcvc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed023681		; stcvc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &eda23681		; stcvc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed223681		; stcvc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &eca23681		; stcvc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec223681		; stcvc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec823621		; stcvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &edc23681		; stclvc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed423681		; stclvc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ede23681		; stclvc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ed623681		; stclvc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ece23681		; stclvc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ec623681		; stclvc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf78		; it	vc
		DCI.W &ecc23621		; stclvc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed823681		; stchi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed023681		; stchi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &eda23681		; stchi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed223681		; stchi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &eca23681		; stchi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec223681		; stchi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec823621		; stchi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &edc23681		; stclhi	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed423681		; stclhi	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ede23681		; stclhi	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ed623681		; stclhi	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ece23681		; stclhi	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ec623681		; stclhi	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf88		; it	hi
		DCI.W &ecc23621		; stclhi	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed823681		; stcls	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed023681		; stcls	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &eda23681		; stcls	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed223681		; stcls	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &eca23681		; stcls	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec223681		; stcls	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec823621		; stcls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &edc23681		; stclls	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed423681		; stclls	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ede23681		; stclls	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ed623681		; stclls	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ece23681		; stclls	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ec623681		; stclls	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf98		; it	ls
		DCI.W &ecc23621		; stclls	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed823681		; stcge	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed023681		; stcge	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &eda23681		; stcge	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed223681		; stcge	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &eca23681		; stcge	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec223681		; stcge	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec823621		; stcge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &edc23681		; stclge	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed423681		; stclge	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ede23681		; stclge	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ed623681		; stclge	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ece23681		; stclge	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ec623681		; stclge	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfa8		; it	ge
		DCI.W &ecc23621		; stclge	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed823681		; stclt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed023681		; stclt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &eda23681		; stclt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed223681		; stclt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &eca23681		; stclt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec223681		; stclt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec823621		; stclt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &edc23681		; stcllt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed423681		; stcllt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ede23681		; stcllt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ed623681		; stcllt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ece23681		; stcllt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ec623681		; stcllt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfb8		; it	lt
		DCI.W &ecc23621		; stcllt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed823681		; stcgt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed023681		; stcgt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &eda23681		; stcgt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed223681		; stcgt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &eca23681		; stcgt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec223681		; stcgt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec823621		; stcgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &edc23681		; stclgt	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed423681		; stclgt	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ede23681		; stclgt	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ed623681		; stclgt	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ece23681		; stclgt	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ec623681		; stclgt	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfc8		; it	gt
		DCI.W &ecc23621		; stclgt	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed823681		; stcle	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed023681		; stcle	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &eda23681		; stcle	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed223681		; stcle	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &eca23681		; stcle	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec223681		; stcle	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec823621		; stcle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &edc23681		; stclle	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed423681		; stclle	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ede23681		; stclle	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ed623681		; stclle	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ece23681		; stclle	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ec623681		; stclle	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bfd8		; it	le
		DCI.W &ecc23621		; stclle	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.W &ed823681		; stc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed023681		; stc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &eda23681		; stc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed223681		; stc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &eca23681		; stc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec223681		; stc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ec823621		; stc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.W &edc23681		; stcl	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &ed423681		; stcl	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &ede23681		; stcl	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &ed623681		; stcl	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &ece23681		; stcl	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &ec623681		; stcl	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &ecc23621		; stcl	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed823681		; stccs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed023681		; stccs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &eda23681		; stccs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed223681		; stccs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &eca23681		; stccs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec223681		; stccs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec823621		; stccs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &edc23681		; stclcs	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed423681		; stclcs	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ede23681		; stclcs	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ed623681		; stclcs	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ece23681		; stclcs	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ec623681		; stclcs	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf28		; it	cs
		DCI.W &ecc23621		; stclcs	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed823681		; stccc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed023681		; stccc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &eda23681		; stccc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed223681		; stccc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &eca23681		; stccc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec223681		; stccc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec823621		; stccc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &edc23681		; stclcc	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed423681		; stclcc	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ede23681		; stclcc	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ed623681		; stclcc	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ece23681		; stclcc	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ec623681		; stclcc	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.N &bf38		; it	cc
		DCI.W &ecc23621		; stclcc	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.N &bf00		; nop
		DCI.W &fd823681		; stc2	6, cr3, [r2, #516]
		DCI.N &bf00		; nop
		DCI.W &fd023681		; stc2	6, cr3, [r2, #-516]
		DCI.N &bf00		; nop
		DCI.W &fda23681		; stc2	6, cr3, [r2, #516]!
		DCI.N &bf00		; nop
		DCI.W &fd223681		; stc2	6, cr3, [r2, #-516]!
		DCI.N &bf00		; nop
		DCI.W &fca23681		; stc2	6, cr3, [r2], #516
		DCI.N &bf00		; nop
		DCI.W &fc223681		; stc2	6, cr3, [r2], #-516
		DCI.N &bf00		; nop
		DCI.W &fc823621		; stc2	6, cr3, [r2], {33}
		DCI.N &bf00		; nop
	]

		END
