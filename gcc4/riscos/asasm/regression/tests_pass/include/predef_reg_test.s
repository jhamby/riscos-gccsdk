; Tests the existence of predefined register symbols (which
; depend on chosen APCS flavour).

	MACRO
	RegTest	$reg
	=	"$reg ", :STR::DEF:$reg, "\n"
	MEND

	RegTest	r0
	RegTest	R0
	RegTest	r13
	RegTest	r14
	RegTest	r15
	RegTest	sp
	RegTest	lr
	RegTest	pc
	RegTest	LR
	RegTest	PC

	RegTest	p0
	RegTest	p15

	RegTest	c0
	RegTest	c15

	RegTest	f0
	RegTest	F0
	RegTest	f7
	RegTest	F7

	RegTest	a1
	RegTest	v1
	RegTest	v5
	RegTest	v6
	RegTest	sb
	RegTest	v7
	RegTest	sl
	RegTest	fp
	RegTest	ip

	RegTest	SP	; Never recognized (!!!)
	RegTest	P0	; Never recognized
	RegTest	P15	; Never recognized
	RegTest	C0	; Never recognized
	RegTest	C15	; Never recognized
	RegTest	A1	; Never recognized
	RegTest	V1	; Never recognized
	RegTest	SB	; Never recognized
	RegTest	SL	; Never recognized
	RegTest	FP	; Never recognized
	RegTest	IP	; Never recognized

	END
