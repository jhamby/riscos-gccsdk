; Tests SEV, WFE, WFI and YIELD.
; RUNOPT: -CPU=7-A

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

	DCI &0320f004	; seveq
	DCI &1320f004	; sevne
	DCI &2320f004	; sevcs
	DCI &2320f004	; sevcs
	DCI &3320f004	; sevcc
	DCI &3320f004	; sevcc
	DCI &4320f004	; sevmi
	DCI &5320f004	; sevpl
	DCI &6320f004	; sevvs
	DCI &7320f004	; sevvc
	DCI &8320f004	; sevhi
	DCI &9320f004	; sevls
	DCI &a320f004	; sevge
	DCI &b320f004	; sevlt
	DCI &c320f004	; sevgt
	DCI &d320f004	; sevle
	DCI &e320f004	; sev
	DCI &e320f004	; sev

	DCI &0320f002	; wfeeq
	DCI &1320f002	; wfene
	DCI &2320f002	; wfecs
	DCI &2320f002	; wfecs
	DCI &3320f002	; wfecc
	DCI &3320f002	; wfecc
	DCI &4320f002	; wfemi
	DCI &5320f002	; wfepl
	DCI &6320f002	; wfevs
	DCI &7320f002	; wfevc
	DCI &8320f002	; wfehi
	DCI &9320f002	; wfels
	DCI &a320f002	; wfege
	DCI &b320f002	; wfelt
	DCI &c320f002	; wfegt
	DCI &d320f002	; wfele
	DCI &e320f002	; wfe
	DCI &e320f002	; wfe

	DCI &0320f003	; wfieq
	DCI &1320f003	; wfine
	DCI &2320f003	; wfics
	DCI &2320f003	; wfics
	DCI &3320f003	; wficc
	DCI &3320f003	; wficc
	DCI &4320f003	; wfimi
	DCI &5320f003	; wfipl
	DCI &6320f003	; wfivs
	DCI &7320f003	; wfivc
	DCI &8320f003	; wfihi
	DCI &9320f003	; wfils
	DCI &a320f003	; wfige
	DCI &b320f003	; wfilt
	DCI &c320f003	; wfigt
	DCI &d320f003	; wfile
	DCI &e320f003	; wfi
	DCI &e320f003	; wfi

	DCI &0320f001	; yieldeq
	DCI &1320f001	; yieldne
	DCI &2320f001	; yieldcs
	DCI &2320f001	; yieldcs
	DCI &3320f001	; yieldcc
	DCI &3320f001	; yieldcc
	DCI &4320f001	; yieldmi
	DCI &5320f001	; yieldpl
	DCI &6320f001	; yieldvs
	DCI &7320f001	; yieldvc
	DCI &8320f001	; yieldhi
	DCI &9320f001	; yieldls
	DCI &a320f001	; yieldge
	DCI &b320f001	; yieldlt
	DCI &c320f001	; yieldgt
	DCI &d320f001	; yieldle
	DCI &e320f001	; yield
	DCI &e320f001	; yield

	]

	END
