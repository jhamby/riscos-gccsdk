; Tests the existence of predefined register symbols (which
; depend on chosen APCS flavour).

	MACRO
	RegTest	$reg
	[ :DEF:$reg
	=	"$reg ", :STR::DEF:$reg, " ", :STR:$reg, "\n"
	|
	=	"$reg ", :STR::DEF:$reg, "\n"
	]
	MEND

	; Main CPU registers:
	RegTest	r0
	RegTest	R0
	RegTest	r13
	RegTest	r14
	RegTest	r15
	RegTest	sp
	RegTest	SP	; Never recognized (!!!)
	RegTest	lr
	RegTest	LR
	RegTest	pc
	RegTest	PC

	; Coprocessor numbers:
	RegTest	p0	; P0 .. P15 are never recognized
	RegTest	P0	; Never recognized
	RegTest	p15
	RegTest	P15	; Never recognized

	; Coprocessor registers:
	RegTest	c0	; C0 .. C15 are never recognized
	RegTest	C0	; Never recognized
	RegTest	c15
	RegTest	C15	; Never recognized

	; FPA registers:
	RegTest	f0
	RegTest	F0
	RegTest	f7
	RegTest	F7

	; Base APCS registers:
	RegTest	a1
	RegTest	A1	; Never recognized
	RegTest	v1
	RegTest	V1	; Never recognized
	RegTest	v5
	RegTest	ip
	RegTest	IP	; Never recognized

	; Optional APCS registers:
	RegTest	v6
	RegTest	sb
	RegTest	SB	; Never recognized
	RegTest	v7
	RegTest	sl
	RegTest	SL	; Never recognized
	RegTest v8
	RegTest	fp
	RegTest	FP	; Never recognized

	[ {FALSE} ; FIXME
	= "\n"

	MACRO
	TestBltIn $bltin
	= "{$bltin} : " :CC: :STR: {$bltin} :CC: "\n"
	MEND

	TestBltIn ROPI
	TestBltIn REENTRANT
	TestBltin RWPI
	]

	END
