; Tests MCRR, MCRR2, MRRC and MRRC2.
; -RUNOPT: -CPU=7-A

		AREA |C$$code|, CODE, READONLY

	[ :LNOT: REFERENCE
		MACRO
		Inject $in
		;INFO 0, "\t$in"
		$in
		MEND
	]

		; Enhanced DSP instructions

	[ :LNOT: REFERENCE
		MACRO
		SubInvoke $instr
		Inject	"$instr p9, # 5, r9, r12, c12"
		MEND

		MACRO
		Invoke $base
		SubInvoke $base
		SubInvoke $base..W
		LCLS	instr
		LCLA	cnt
cnt		SETA	0
		WHILE	cnt < 17
		; 17 condition codes to be tested:
		;   - Condition code NV is left out from this test.
		;   - HS and LO are equivalents for CS and CC.
instr		SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
		SubInvoke $instr
		SubInvoke $instr..W
cnt		SETA	cnt + 1
		WEND
		MEND

		ARM
		Invoke MRRC
		SubInvoke MRRC2
		SubInvoke MRRC2.W

		Invoke MCRR
		SubInvoke MCRR2
		SubInvoke MCRR2.W

		THUMB
		Invoke MRRC
		SubInvoke MRRC2
		SubInvoke MRRC2.W

		Invoke MCRR
		SubInvoke MCRR2
		SubInvoke MCRR2.W
	|
		ARM
		DCI &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI &0c5c995c	; mrrceq	9, 5, r9, ip, cr12
		DCI &0c5c995c	; mrrceq	9, 5, r9, ip, cr12
		DCI &1c5c995c	; mrrcne	9, 5, r9, ip, cr12
		DCI &1c5c995c	; mrrcne	9, 5, r9, ip, cr12
		DCI &2c5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI &2c5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI &3c5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI &3c5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI &4c5c995c	; mrrcmi	9, 5, r9, ip, cr12
		DCI &4c5c995c	; mrrcmi	9, 5, r9, ip, cr12
		DCI &5c5c995c	; mrrcpl	9, 5, r9, ip, cr12
		DCI &5c5c995c	; mrrcpl	9, 5, r9, ip, cr12
		DCI &6c5c995c	; mrrcvs	9, 5, r9, ip, cr12
		DCI &6c5c995c	; mrrcvs	9, 5, r9, ip, cr12
		DCI &7c5c995c	; mrrcvc	9, 5, r9, ip, cr12
		DCI &7c5c995c	; mrrcvc	9, 5, r9, ip, cr12
		DCI &8c5c995c	; mrrchi	9, 5, r9, ip, cr12
		DCI &8c5c995c	; mrrchi	9, 5, r9, ip, cr12
		DCI &9c5c995c	; mrrcls	9, 5, r9, ip, cr12
		DCI &9c5c995c	; mrrcls	9, 5, r9, ip, cr12
		DCI &ac5c995c	; mrrcge	9, 5, r9, ip, cr12
		DCI &ac5c995c	; mrrcge	9, 5, r9, ip, cr12
		DCI &bc5c995c	; mrrclt	9, 5, r9, ip, cr12
		DCI &bc5c995c	; mrrclt	9, 5, r9, ip, cr12
		DCI &cc5c995c	; mrrcgt	9, 5, r9, ip, cr12
		DCI &cc5c995c	; mrrcgt	9, 5, r9, ip, cr12
		DCI &dc5c995c	; mrrcle	9, 5, r9, ip, cr12
		DCI &dc5c995c	; mrrcle	9, 5, r9, ip, cr12
		DCI &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI &2c5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI &2c5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI &3c5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI &3c5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI &fc5c995c	; mrrc2	9, 5, r9, ip, cr12
		DCI &fc5c995c	; mrrc2	9, 5, r9, ip, cr12
		DCI &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI &0c4c995c	; mcrreq	9, 5, r9, ip, cr12
		DCI &0c4c995c	; mcrreq	9, 5, r9, ip, cr12
		DCI &1c4c995c	; mcrrne	9, 5, r9, ip, cr12
		DCI &1c4c995c	; mcrrne	9, 5, r9, ip, cr12
		DCI &2c4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI &2c4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI &3c4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI &3c4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI &4c4c995c	; mcrrmi	9, 5, r9, ip, cr12
		DCI &4c4c995c	; mcrrmi	9, 5, r9, ip, cr12
		DCI &5c4c995c	; mcrrpl	9, 5, r9, ip, cr12
		DCI &5c4c995c	; mcrrpl	9, 5, r9, ip, cr12
		DCI &6c4c995c	; mcrrvs	9, 5, r9, ip, cr12
		DCI &6c4c995c	; mcrrvs	9, 5, r9, ip, cr12
		DCI &7c4c995c	; mcrrvc	9, 5, r9, ip, cr12
		DCI &7c4c995c	; mcrrvc	9, 5, r9, ip, cr12
		DCI &8c4c995c	; mcrrhi	9, 5, r9, ip, cr12
		DCI &8c4c995c	; mcrrhi	9, 5, r9, ip, cr12
		DCI &9c4c995c	; mcrrls	9, 5, r9, ip, cr12
		DCI &9c4c995c	; mcrrls	9, 5, r9, ip, cr12
		DCI &ac4c995c	; mcrrge	9, 5, r9, ip, cr12
		DCI &ac4c995c	; mcrrge	9, 5, r9, ip, cr12
		DCI &bc4c995c	; mcrrlt	9, 5, r9, ip, cr12
		DCI &bc4c995c	; mcrrlt	9, 5, r9, ip, cr12
		DCI &cc4c995c	; mcrrgt	9, 5, r9, ip, cr12
		DCI &cc4c995c	; mcrrgt	9, 5, r9, ip, cr12
		DCI &dc4c995c	; mcrrle	9, 5, r9, ip, cr12
		DCI &dc4c995c	; mcrrle	9, 5, r9, ip, cr12
		DCI &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI &2c4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI &2c4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI &3c4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI &3c4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI &fc4c995c	; mcrr2	9, 5, r9, ip, cr12
		DCI &fc4c995c	; mcrr2	9, 5, r9, ip, cr12

		THUMB
		DCI.W &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI.N &bf07	; ittee	eq
		DCI.W &ec5c995c	; mrrceq	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrceq	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcne	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcne	9, 5, r9, ip, cr12
		DCI.N &bf27	; ittee	cs
		DCI.W &ec5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI.N &bf47	; ittee	mi
		DCI.W &ec5c995c	; mrrcmi	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcmi	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcpl	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcpl	9, 5, r9, ip, cr12
		DCI.N &bf67	; ittee	vs
		DCI.W &ec5c995c	; mrrcvs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcvs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcvc	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcvc	9, 5, r9, ip, cr12
		DCI.N &bf87	; ittee	hi
		DCI.W &ec5c995c	; mrrchi	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrchi	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcls	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcls	9, 5, r9, ip, cr12
		DCI.N &bfa7	; ittee	ge
		DCI.W &ec5c995c	; mrrcge	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcge	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrclt	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrclt	9, 5, r9, ip, cr12
		DCI.N &bfc7	; ittee	gt
		DCI.W &ec5c995c	; mrrcgt	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcgt	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcle	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrcle	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrc	9, 5, r9, ip, cr12
		DCI.N &bf27	; ittee	cs
		DCI.W &ec5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccs	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI.W &ec5c995c	; mrrccc	9, 5, r9, ip, cr12
		DCI.W &fc5c995c	; mrrc2	9, 5, r9, ip, cr12
		DCI.W &fc5c995c	; mrrc2	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI.N &bf07	; ittee	eq
		DCI.W &ec4c995c	; mcrreq	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrreq	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrne	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrne	9, 5, r9, ip, cr12
		DCI.N &bf27	; ittee	cs
		DCI.W &ec4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI.N &bf47	; ittee	mi
		DCI.W &ec4c995c	; mcrrmi	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrmi	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrpl	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrpl	9, 5, r9, ip, cr12
		DCI.N &bf67	; ittee	vs
		DCI.W &ec4c995c	; mcrrvs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrvs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrvc	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrvc	9, 5, r9, ip, cr12
		DCI.N &bf87	; ittee	hi
		DCI.W &ec4c995c	; mcrrhi	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrhi	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrls	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrls	9, 5, r9, ip, cr12
		DCI.N &bfa7	; ittee	ge
		DCI.W &ec4c995c	; mcrrge	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrge	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrlt	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrlt	9, 5, r9, ip, cr12
		DCI.N &bfc7	; ittee	gt
		DCI.W &ec4c995c	; mcrrgt	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrgt	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrle	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrle	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrr	9, 5, r9, ip, cr12
		DCI.N &bf27	; ittee	cs
		DCI.W &ec4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcs	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI.W &ec4c995c	; mcrrcc	9, 5, r9, ip, cr12
		DCI.W &fc4c995c	; mcrr2	9, 5, r9, ip, cr12
		DCI.W &fc4c995c	; mcrr2	9, 5, r9, ip, cr12
	]

		END
