; Tests SEV, WFE, WFI and YIELD.

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; We don't test NV condition.
	SEVEQ
	SEVNE
	SEVCS
	SEVHS
	SEVCC
	SEVLO
	SEVMI
	SEVPL
	SEVVS
	SEVVC
	SEVHI
	SEVLS
	SEVGE
	SEVLT
	SEVGT
	SEVLE
	SEVAL
	SEV

	; We don't test NV condition.
	WFEEQ
	WFENE
	WFECS
	WFEHS
	WFECC
	WFELO
	WFEMI
	WFEPL
	WFEVS
	WFEVC
	WFEHI
	WFELS
	WFEGE
	WFELT
	WFEGT
	WFELE
	WFEAL
	WFE

	; We don't test NV condition.
	WFIEQ
	WFINE
	WFICS
	WFIHS
	WFICC
	WFILO
	WFIMI
	WFIPL
	WFIVS
	WFIVC
	WFIHI
	WFILS
	WFIGE
	WFILT
	WFIGT
	WFILE
	WFIAL
	WFI

	; We don't test NV condition.
	YIELDEQ
	YIELDNE
	YIELDCS
	YIELDHS
	YIELDCC
	YIELDLO
	YIELDMI
	YIELDPL
	YIELDVS
	YIELDVC
	YIELDHI
	YIELDLS
	YIELDGE
	YIELDLT
	YIELDGT
	YIELDLE
	YIELDAL
	YIELD

	|

	DCD &0320f004 	; seveq
	DCD &1320f004 	; sevne
	DCD &2320f004 	; sevcs
	DCD &2320f004 	; sevcs
	DCD &3320f004 	; sevcc
	DCD &3320f004 	; sevcc
	DCD &4320f004 	; sevmi
	DCD &5320f004 	; sevpl
	DCD &6320f004 	; sevvs
	DCD &7320f004 	; sevvc
	DCD &8320f004 	; sevhi
	DCD &9320f004 	; sevls
	DCD &a320f004 	; sevge
	DCD &b320f004 	; sevlt
	DCD &c320f004 	; sevgt
	DCD &d320f004 	; sevle
	DCD &e320f004 	; sev
	DCD &e320f004 	; sev

	DCD &0320f002 	; wfeeq
	DCD &1320f002 	; wfene
	DCD &2320f002 	; wfecs
	DCD &2320f002 	; wfecs
	DCD &3320f002 	; wfecc
	DCD &3320f002 	; wfecc
	DCD &4320f002 	; wfemi
	DCD &5320f002 	; wfepl
	DCD &6320f002 	; wfevs
	DCD &7320f002 	; wfevc
	DCD &8320f002 	; wfehi
	DCD &9320f002 	; wfels
	DCD &a320f002 	; wfege
	DCD &b320f002 	; wfelt
	DCD &c320f002 	; wfegt
	DCD &d320f002 	; wfele
	DCD &e320f002 	; wfe
	DCD &e320f002 	; wfe

	DCD &0320f003 	; wfieq
	DCD &1320f003 	; wfine
	DCD &2320f003 	; wfics
	DCD &2320f003 	; wfics
	DCD &3320f003 	; wficc
	DCD &3320f003 	; wficc
	DCD &4320f003 	; wfimi
	DCD &5320f003 	; wfipl
	DCD &6320f003 	; wfivs
	DCD &7320f003 	; wfivc
	DCD &8320f003 	; wfihi
	DCD &9320f003 	; wfils
	DCD &a320f003 	; wfige
	DCD &b320f003 	; wfilt
	DCD &c320f003 	; wfigt
	DCD &d320f003 	; wfile
	DCD &e320f003 	; wfi
	DCD &e320f003 	; wfi

	DCD &0320f001 	; yieldeq
	DCD &1320f001 	; yieldne
	DCD &2320f001 	; yieldcs
	DCD &2320f001 	; yieldcs
	DCD &3320f001 	; yieldcc
	DCD &3320f001 	; yieldcc
	DCD &4320f001 	; yieldmi
	DCD &5320f001 	; yieldpl
	DCD &6320f001 	; yieldvs
	DCD &7320f001 	; yieldvc
	DCD &8320f001 	; yieldhi
	DCD &9320f001 	; yieldls
	DCD &a320f001 	; yieldge
	DCD &b320f001 	; yieldlt
	DCD &c320f001 	; yieldgt
	DCD &d320f001 	; yieldle
	DCD &e320f001 	; yield
	DCD &e320f001 	; yield

	]
	
	END