; Tests SEV, WFE, WFI and YIELD.
; RUNOPT: -CPU=7-A

	AREA	Code, CODE

	[ :LNOT: REFERENCE
	MACRO
	Inject	$i
	;INFO 0, "\t$i"
	$i
	MEND

	; <base>["" | ".N" | ".W"]
	; <base> <cond code>["" | ".N" | ".W"]
	MACRO
	Invoke $base
	Inject	"$base"
	[ {UAL}
	Inject	"$base..N"
	]
	[ {CODESIZE}=32 :LOR: {UAL}
	Inject	"$base..W"
	]

	LCLS	instr
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
	Inject	"$instr"
	[ {UAL}
	Inject	"$instr..N"
	]
	[ {CODESIZE}=32 :LOR: {UAL}
	Inject	"$instr..W"
	]
cnt	SETA	cnt + 1
	WEND
	MEND

	ARM
	Invoke SEV
	CODE16
	Invoke SEV
	THUMB
	Invoke SEV

	ARM
	Invoke WFE
	CODE16
	Invoke WFE
	THUMB
	Invoke WFE

	ARM
	Invoke WFI
	CODE16
	Invoke WFI
	THUMB
	Invoke WFI

	ARM
	Invoke YIELD
	CODE16
	Invoke YIELD
	THUMB
	Invoke YIELD

	|

	; SEV
	ARM
	DCI &e320f004	; sev
	DCI &e320f004	; sev
	DCI &0320f004	; seveq
	DCI &0320f004	; seveq
	DCI &1320f004	; sevne
	DCI &1320f004	; sevne
	DCI &2320f004	; sevcs
	DCI &2320f004	; sevcs
	DCI &3320f004	; sevcc
	DCI &3320f004	; sevcc
	DCI &4320f004	; sevmi
	DCI &4320f004	; sevmi
	DCI &5320f004	; sevpl
	DCI &5320f004	; sevpl
	DCI &6320f004	; sevvs
	DCI &6320f004	; sevvs
	DCI &7320f004	; sevvc
	DCI &7320f004	; sevvc
	DCI &8320f004	; sevhi
	DCI &8320f004	; sevhi
	DCI &9320f004	; sevls
	DCI &9320f004	; sevls
	DCI &a320f004	; sevge
	DCI &a320f004	; sevge
	DCI &b320f004	; sevlt
	DCI &b320f004	; sevlt
	DCI &c320f004	; sevgt
	DCI &c320f004	; sevgt
	DCI &d320f004	; sevle
	DCI &d320f004	; sevle
	DCI &e320f004	; sev
	DCI &e320f004	; sev
	DCI &2320f004	; sevcs
	DCI &2320f004	; sevcs
	DCI &3320f004	; sevcc
	DCI &3320f004	; sevcc

	CODE16
	DCI &bf40	; sev
	DCI &bf0c	; ite	eq
	DCI &bf40	; seveq
	DCI &bf40	; sevne
	DCI &bf2c	; ite	cs
	DCI &bf40	; sevcs
	DCI &bf40	; sevcc
	DCI &bf4c	; ite	mi
	DCI &bf40	; sevmi
	DCI &bf40	; sevpl
	DCI &bf6c	; ite	vs
	DCI &bf40	; sevvs
	DCI &bf40	; sevvc
	DCI &bf8c	; ite	hi
	DCI &bf40	; sevhi
	DCI &bf40	; sevls
	DCI &bfac	; ite	ge
	DCI &bf40	; sevge
	DCI &bf40	; sevlt
	DCI &bfcc	; ite	gt
	DCI &bf40	; sevgt
	DCI &bf40	; sevle
	DCI &bf40	; sev
	DCI &bf2c	; ite	cs
	DCI &bf40	; sevcs
	DCI &bf40	; sevcc

	THUMB
	DCI.N &bf40	; sev
	DCI.N &bf40	; sev
	DCI.W &f3af8004	; sev.w
	DCI.N &bf03	; ittte	eq
	DCI.N &bf40	; seveq
	DCI.N &bf40	; seveq
	DCI.W &f3af8004	; seveq.w
	DCI.N &bf40	; sevne
	DCI.N &bf1c	; itt	ne
	DCI.N &bf40	; sevne
	DCI.W &f3af8004	; sevne.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf40	; sevcs
	DCI.N &bf40	; sevcs
	DCI.W &f3af8004	; sevcs.w
	DCI.N &bf40	; sevcc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf40	; sevcc
	DCI.W &f3af8004	; sevcc.w
	DCI.N &bf43	; ittte	mi
	DCI.N &bf40	; sevmi
	DCI.N &bf40	; sevmi
	DCI.W &f3af8004	; sevmi.w
	DCI.N &bf40	; sevpl
	DCI.N &bf5c	; itt	pl
	DCI.N &bf40	; sevpl
	DCI.W &f3af8004	; sevpl.w
	DCI.N &bf63	; ittte	vs
	DCI.N &bf40	; sevvs
	DCI.N &bf40	; sevvs
	DCI.W &f3af8004	; sevvs.w
	DCI.N &bf40	; sevvc
	DCI.N &bf7c	; itt	vc
	DCI.N &bf40	; sevvc
	DCI.W &f3af8004	; sevvc.w
	DCI.N &bf83	; ittte	hi
	DCI.N &bf40	; sevhi
	DCI.N &bf40	; sevhi
	DCI.W &f3af8004	; sevhi.w
	DCI.N &bf40	; sevls
	DCI.N &bf9c	; itt	ls
	DCI.N &bf40	; sevls
	DCI.W &f3af8004	; sevls.w
	DCI.N &bfa3	; ittte	ge
	DCI.N &bf40	; sevge
	DCI.N &bf40	; sevge
	DCI.W &f3af8004	; sevge.w
	DCI.N &bf40	; sevlt
	DCI.N &bfbc	; itt	lt
	DCI.N &bf40	; sevlt
	DCI.W &f3af8004	; sevlt.w
	DCI.N &bfc3	; ittte	gt
	DCI.N &bf40	; sevgt
	DCI.N &bf40	; sevgt
	DCI.W &f3af8004	; sevgt.w
	DCI.N &bf40	; sevle
	DCI.N &bfdc	; itt	le
	DCI.N &bf40	; sevle
	DCI.W &f3af8004	; sevle.w
	DCI.N &bf40	; sev
	DCI.N &bf40	; sev
	DCI.W &f3af8004	; sev.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf40	; sevcs
	DCI.N &bf40	; sevcs
	DCI.W &f3af8004	; sevcs.w
	DCI.N &bf40	; sevcc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf40	; sevcc
	DCI.W &f3af8004	; sevcc.w

	; WFE
	ARM
	DCI &e320f002	; wfe
	DCI &e320f002	; wfe
	DCI &0320f002	; wfeeq
	DCI &0320f002	; wfeeq
	DCI &1320f002	; wfene
	DCI &1320f002	; wfene
	DCI &2320f002	; wfecs
	DCI &2320f002	; wfecs
	DCI &3320f002	; wfecc
	DCI &3320f002	; wfecc
	DCI &4320f002	; wfemi
	DCI &4320f002	; wfemi
	DCI &5320f002	; wfepl
	DCI &5320f002	; wfepl
	DCI &6320f002	; wfevs
	DCI &6320f002	; wfevs
	DCI &7320f002	; wfevc
	DCI &7320f002	; wfevc
	DCI &8320f002	; wfehi
	DCI &8320f002	; wfehi
	DCI &9320f002	; wfels
	DCI &9320f002	; wfels
	DCI &a320f002	; wfege
	DCI &a320f002	; wfege
	DCI &b320f002	; wfelt
	DCI &b320f002	; wfelt
	DCI &c320f002	; wfegt
	DCI &c320f002	; wfegt
	DCI &d320f002	; wfele
	DCI &d320f002	; wfele
	DCI &e320f002	; wfe
	DCI &e320f002	; wfe
	DCI &2320f002	; wfecs
	DCI &2320f002	; wfecs
	DCI &3320f002	; wfecc
	DCI &3320f002	; wfecc

	CODE16
	DCI &bf20	; wfe
	DCI &bf0c	; ite	eq
	DCI &bf20	; wfeeq
	DCI &bf20	; wfene
	DCI &bf2c	; ite	cs
	DCI &bf20	; wfecs
	DCI &bf20	; wfecc
	DCI &bf4c	; ite	mi
	DCI &bf20	; wfemi
	DCI &bf20	; wfepl
	DCI &bf6c	; ite	vs
	DCI &bf20	; wfevs
	DCI &bf20	; wfevc
	DCI &bf8c	; ite	hi
	DCI &bf20	; wfehi
	DCI &bf20	; wfels
	DCI &bfac	; ite	ge
	DCI &bf20	; wfege
	DCI &bf20	; wfelt
	DCI &bfcc	; ite	gt
	DCI &bf20	; wfegt
	DCI &bf20	; wfele
	DCI &bf20	; wfe
	DCI &bf2c	; ite	cs
	DCI &bf20	; wfecs
	DCI &bf20	; wfecc

	THUMB
	DCI.N &bf20	; wfe
	DCI.N &bf20	; wfe
	DCI.W &f3af8002	; wfe.w
	DCI.N &bf03	; ittte	eq
	DCI.N &bf20	; wfeeq
	DCI.N &bf20	; wfeeq
	DCI.W &f3af8002	; wfeeq.w
	DCI.N &bf20	; wfene
	DCI.N &bf1c	; itt	ne
	DCI.N &bf20	; wfene
	DCI.W &f3af8002	; wfene.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf20	; wfecs
	DCI.N &bf20	; wfecs
	DCI.W &f3af8002	; wfecs.w
	DCI.N &bf20	; wfecc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf20	; wfecc
	DCI.W &f3af8002	; wfecc.w
	DCI.N &bf43	; ittte	mi
	DCI.N &bf20	; wfemi
	DCI.N &bf20	; wfemi
	DCI.W &f3af8002	; wfemi.w
	DCI.N &bf20	; wfepl
	DCI.N &bf5c	; itt	pl
	DCI.N &bf20	; wfepl
	DCI.W &f3af8002	; wfepl.w
	DCI.N &bf63	; ittte	vs
	DCI.N &bf20	; wfevs
	DCI.N &bf20	; wfevs
	DCI.W &f3af8002	; wfevs.w
	DCI.N &bf20	; wfevc
	DCI.N &bf7c	; itt	vc
	DCI.N &bf20	; wfevc
	DCI.W &f3af8002	; wfevc.w
	DCI.N &bf83	; ittte	hi
	DCI.N &bf20	; wfehi
	DCI.N &bf20	; wfehi
	DCI.W &f3af8002	; wfehi.w
	DCI.N &bf20	; wfels
	DCI.N &bf9c	; itt	ls
	DCI.N &bf20	; wfels
	DCI.W &f3af8002	; wfels.w
	DCI.N &bfa3	; ittte	ge
	DCI.N &bf20	; wfege
	DCI.N &bf20	; wfege
	DCI.W &f3af8002	; wfege.w
	DCI.N &bf20	; wfelt
	DCI.N &bfbc	; itt	lt
	DCI.N &bf20	; wfelt
	DCI.W &f3af8002	; wfelt.w
	DCI.N &bfc3	; ittte	gt
	DCI.N &bf20	; wfegt
	DCI.N &bf20	; wfegt
	DCI.W &f3af8002	; wfegt.w
	DCI.N &bf20	; wfele
	DCI.N &bfdc	; itt	le
	DCI.N &bf20	; wfele
	DCI.W &f3af8002	; wfele.w
	DCI.N &bf20	; wfe
	DCI.N &bf20	; wfe
	DCI.W &f3af8002	; wfe.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf20	; wfecs
	DCI.N &bf20	; wfecs
	DCI.W &f3af8002	; wfecs.w
	DCI.N &bf20	; wfecc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf20	; wfecc
	DCI.W &f3af8002	; wfecc.w

	; WFI
	ARM
	DCI &e320f003	; wfi
	DCI &e320f003	; wfi
	DCI &0320f003	; wfieq
	DCI &0320f003	; wfieq
	DCI &1320f003	; wfine
	DCI &1320f003	; wfine
	DCI &2320f003	; wfics
	DCI &2320f003	; wfics
	DCI &3320f003	; wficc
	DCI &3320f003	; wficc
	DCI &4320f003	; wfimi
	DCI &4320f003	; wfimi
	DCI &5320f003	; wfipl
	DCI &5320f003	; wfipl
	DCI &6320f003	; wfivs
	DCI &6320f003	; wfivs
	DCI &7320f003	; wfivc
	DCI &7320f003	; wfivc
	DCI &8320f003	; wfihi
	DCI &8320f003	; wfihi
	DCI &9320f003	; wfils
	DCI &9320f003	; wfils
	DCI &a320f003	; wfige
	DCI &a320f003	; wfige
	DCI &b320f003	; wfilt
	DCI &b320f003	; wfilt
	DCI &c320f003	; wfigt
	DCI &c320f003	; wfigt
	DCI &d320f003	; wfile
	DCI &d320f003	; wfile
	DCI &e320f003	; wfi
	DCI &e320f003	; wfi
	DCI &2320f003	; wfics
	DCI &2320f003	; wfics
	DCI &3320f003	; wficc
	DCI &3320f003	; wficc

	CODE16
	DCI &bf30	; wfi
	DCI &bf0c	; ite	eq
	DCI &bf30	; wfieq
	DCI &bf30	; wfine
	DCI &bf2c	; ite	cs
	DCI &bf30	; wfics
	DCI &bf30	; wficc
	DCI &bf4c	; ite	mi
	DCI &bf30	; wfimi
	DCI &bf30	; wfipl
	DCI &bf6c	; ite	vs
	DCI &bf30	; wfivs
	DCI &bf30	; wfivc
	DCI &bf8c	; ite	hi
	DCI &bf30	; wfihi
	DCI &bf30	; wfils
	DCI &bfac	; ite	ge
	DCI &bf30	; wfige
	DCI &bf30	; wfilt
	DCI &bfcc	; ite	gt
	DCI &bf30	; wfigt
	DCI &bf30	; wfile
	DCI &bf30	; wfi
	DCI &bf2c	; ite	cs
	DCI &bf30	; wfics
	DCI &bf30	; wficc

	THUMB
	DCI.N &bf30	; wfi
	DCI.N &bf30	; wfi
	DCI.W &f3af8003	; wfi.w
	DCI.N &bf03	; ittte	eq
	DCI.N &bf30	; wfieq
	DCI.N &bf30	; wfieq
	DCI.W &f3af8003	; wfieq.w
	DCI.N &bf30	; wfine
	DCI.N &bf1c	; itt	ne
	DCI.N &bf30	; wfine
	DCI.W &f3af8003	; wfine.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf30	; wfics
	DCI.N &bf30	; wfics
	DCI.W &f3af8003	; wfics.w
	DCI.N &bf30	; wficc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf30	; wficc
	DCI.W &f3af8003	; wficc.w
	DCI.N &bf43	; ittte	mi
	DCI.N &bf30	; wfimi
	DCI.N &bf30	; wfimi
	DCI.W &f3af8003	; wfimi.w
	DCI.N &bf30	; wfipl
	DCI.N &bf5c	; itt	pl
	DCI.N &bf30	; wfipl
	DCI.W &f3af8003	; wfipl.w
	DCI.N &bf63	; ittte	vs
	DCI.N &bf30	; wfivs
	DCI.N &bf30	; wfivs
	DCI.W &f3af8003	; wfivs.w
	DCI.N &bf30	; wfivc
	DCI.N &bf7c	; itt	vc
	DCI.N &bf30	; wfivc
	DCI.W &f3af8003	; wfivc.w
	DCI.N &bf83	; ittte	hi
	DCI.N &bf30	; wfihi
	DCI.N &bf30	; wfihi
	DCI.W &f3af8003	; wfihi.w
	DCI.N &bf30	; wfils
	DCI.N &bf9c	; itt	ls
	DCI.N &bf30	; wfils
	DCI.W &f3af8003	; wfils.w
	DCI.N &bfa3	; ittte	ge
	DCI.N &bf30	; wfige
	DCI.N &bf30	; wfige
	DCI.W &f3af8003	; wfige.w
	DCI.N &bf30	; wfilt
	DCI.N &bfbc	; itt	lt
	DCI.N &bf30	; wfilt
	DCI.W &f3af8003	; wfilt.w
	DCI.N &bfc3	; ittte	gt
	DCI.N &bf30	; wfigt
	DCI.N &bf30	; wfigt
	DCI.W &f3af8003	; wfigt.w
	DCI.N &bf30	; wfile
	DCI.N &bfdc	; itt	le
	DCI.N &bf30	; wfile
	DCI.W &f3af8003	; wfile.w
	DCI.N &bf30	; wfi
	DCI.N &bf30	; wfi
	DCI.W &f3af8003	; wfi.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf30	; wfics
	DCI.N &bf30	; wfics
	DCI.W &f3af8003	; wfics.w
	DCI.N &bf30	; wficc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf30	; wficc
	DCI.W &f3af8003	; wficc.w

	; YIELD
	ARM
	DCI &e320f001	; yield
	DCI &e320f001	; yield
	DCI &0320f001	; yieldeq
	DCI &0320f001	; yieldeq
	DCI &1320f001	; yieldne
	DCI &1320f001	; yieldne
	DCI &2320f001	; yieldcs
	DCI &2320f001	; yieldcs
	DCI &3320f001	; yieldcc
	DCI &3320f001	; yieldcc
	DCI &4320f001	; yieldmi
	DCI &4320f001	; yieldmi
	DCI &5320f001	; yieldpl
	DCI &5320f001	; yieldpl
	DCI &6320f001	; yieldvs
	DCI &6320f001	; yieldvs
	DCI &7320f001	; yieldvc
	DCI &7320f001	; yieldvc
	DCI &8320f001	; yieldhi
	DCI &8320f001	; yieldhi
	DCI &9320f001	; yieldls
	DCI &9320f001	; yieldls
	DCI &a320f001	; yieldge
	DCI &a320f001	; yieldge
	DCI &b320f001	; yieldlt
	DCI &b320f001	; yieldlt
	DCI &c320f001	; yieldgt
	DCI &c320f001	; yieldgt
	DCI &d320f001	; yieldle
	DCI &d320f001	; yieldle
	DCI &e320f001	; yield
	DCI &e320f001	; yield
	DCI &2320f001	; yieldcs
	DCI &2320f001	; yieldcs
	DCI &3320f001	; yieldcc
	DCI &3320f001	; yieldcc

	CODE16
	DCI &bf10	; yield
	DCI &bf0c	; ite	eq
	DCI &bf10	; yieldeq
	DCI &bf10	; yieldne
	DCI &bf2c	; ite	cs
	DCI &bf10	; yieldcs
	DCI &bf10	; yieldcc
	DCI &bf4c	; ite	mi
	DCI &bf10	; yieldmi
	DCI &bf10	; yieldpl
	DCI &bf6c	; ite	vs
	DCI &bf10	; yieldvs
	DCI &bf10	; yieldvc
	DCI &bf8c	; ite	hi
	DCI &bf10	; yieldhi
	DCI &bf10	; yieldls
	DCI &bfac	; ite	ge
	DCI &bf10	; yieldge
	DCI &bf10	; yieldlt
	DCI &bfcc	; ite	gt
	DCI &bf10	; yieldgt
	DCI &bf10	; yieldle
	DCI &bf10	; yield
	DCI &bf2c	; ite	cs
	DCI &bf10	; yieldcs
	DCI &bf10	; yieldcc

	THUMB
	DCI.N &bf10	; yield
	DCI.N &bf10	; yield
	DCI.W &f3af8001	; yield.w
	DCI.N &bf03	; ittte	eq
	DCI.N &bf10	; yieldeq
	DCI.N &bf10	; yieldeq
	DCI.W &f3af8001	; yieldeq.w
	DCI.N &bf10	; yieldne
	DCI.N &bf1c	; itt	ne
	DCI.N &bf10	; yieldne
	DCI.W &f3af8001	; yieldne.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf10	; yieldcs
	DCI.N &bf10	; yieldcs
	DCI.W &f3af8001	; yieldcs.w
	DCI.N &bf10	; yieldcc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf10	; yieldcc
	DCI.W &f3af8001	; yieldcc.w
	DCI.N &bf43	; ittte	mi
	DCI.N &bf10	; yieldmi
	DCI.N &bf10	; yieldmi
	DCI.W &f3af8001	; yieldmi.w
	DCI.N &bf10	; yieldpl
	DCI.N &bf5c	; itt	pl
	DCI.N &bf10	; yieldpl
	DCI.W &f3af8001	; yieldpl.w
	DCI.N &bf63	; ittte	vs
	DCI.N &bf10	; yieldvs
	DCI.N &bf10	; yieldvs
	DCI.W &f3af8001	; yieldvs.w
	DCI.N &bf10	; yieldvc
	DCI.N &bf7c	; itt	vc
	DCI.N &bf10	; yieldvc
	DCI.W &f3af8001	; yieldvc.w
	DCI.N &bf83	; ittte	hi
	DCI.N &bf10	; yieldhi
	DCI.N &bf10	; yieldhi
	DCI.W &f3af8001	; yieldhi.w
	DCI.N &bf10	; yieldls
	DCI.N &bf9c	; itt	ls
	DCI.N &bf10	; yieldls
	DCI.W &f3af8001	; yieldls.w
	DCI.N &bfa3	; ittte	ge
	DCI.N &bf10	; yieldge
	DCI.N &bf10	; yieldge
	DCI.W &f3af8001	; yieldge.w
	DCI.N &bf10	; yieldlt
	DCI.N &bfbc	; itt	lt
	DCI.N &bf10	; yieldlt
	DCI.W &f3af8001	; yieldlt.w
	DCI.N &bfc3	; ittte	gt
	DCI.N &bf10	; yieldgt
	DCI.N &bf10	; yieldgt
	DCI.W &f3af8001	; yieldgt.w
	DCI.N &bf10	; yieldle
	DCI.N &bfdc	; itt	le
	DCI.N &bf10	; yieldle
	DCI.W &f3af8001	; yieldle.w
	DCI.N &bf10	; yield
	DCI.N &bf10	; yield
	DCI.W &f3af8001	; yield.w
	DCI.N &bf23	; ittte	cs
	DCI.N &bf10	; yieldcs
	DCI.N &bf10	; yieldcs
	DCI.W &f3af8001	; yieldcs.w
	DCI.N &bf10	; yieldcc
	DCI.N &bf3c	; itt	cc
	DCI.N &bf10	; yieldcc
	DCI.W &f3af8001	; yieldcc.w
	]

	END
