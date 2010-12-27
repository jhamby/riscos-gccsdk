; Tests ASR, LSL, LSR, ROR and RRX.

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; ASR
	ASR	r1, r2, #0	; Becomes MOV r1, r2
	ASR	r1, r2, #1
	ASR	r1, r2, #31
	ASR	r2, #0		; Becomes MOV r2, r2
	ASR	r2, #1
	ASR	r2, #31

	ASR	r1, r2, r3
	ASR	r2, r3

	; LSL
	LSL	r1, r2, #0	; Becomes MOV r1, r2
	LSL	r1, r2, #1
	LSL	r1, r2, #31
	LSL	r2, #0		; Becomes MOV r2, r2
	LSL	r2, #1
	LSL	r2, #31

	LSL	r1, r2, r3
	LSL	r2, r3

	; LSR
	LSR	r1, r2, #0	; Becomes MOV r1, r2
	LSR	r1, r2, #1
	LSR	r1, r2, #31
	LSR	r2, #0		; Becomes MOV r2, r2
	LSR	r2, #1
	LSR	r2, #31

	LSR	r1, r2, r3
	LSR	r2, r3

	; ROR
	ROR	r1, r2, #0	; Becomes MOV r1, r2
	ROR	r1, r2, #1
	ROR	r1, r2, #31
	ROR	r2, #0		; Becomes MOV r2, r2
	ROR	r2, #1
	ROR	r2, #31

	ROR	r1, r2, r3
	ROR	r2, r3

	; RRX
	RRX	r1, r2
	RRX	r2

	|
	
	DCD	&e1a01002 	; mov	r1, r2
	DCD	&e1a010c2 	; asr	r1, r2, #1
	DCD	&e1a01fc2 	; asr	r1, r2, #31
	DCD	&e1a02002 	; mov	r2, r2
	DCD	&e1a020c2 	; asr	r2, r2, #1
	DCD	&e1a02fc2 	; asr	r2, r2, #31
	DCD	&e1a01352 	; asr	r1, r2, r3
	DCD	&e1a02352 	; asr	r2, r2, r3

	DCD	&e1a01002 	; mov	r1, r2
	DCD	&e1a01082 	; lsl	r1, r2, #1
	DCD	&e1a01f82 	; lsl	r1, r2, #31
	DCD	&e1a02002 	; mov	r2, r2
	DCD	&e1a02082 	; lsl	r2, r2, #1
	DCD	&e1a02f82 	; lsl	r2, r2, #31
	DCD	&e1a01312 	; lsl	r1, r2, r3
	DCD	&e1a02312 	; lsl	r2, r2, r3

	DCD	&e1a01002 	; mov	r1, r2
	DCD	&e1a010a2 	; lsr	r1, r2, #1
	DCD	&e1a01fa2 	; lsr	r1, r2, #31
	DCD	&e1a02002 	; mov	r2, r2
	DCD	&e1a020a2 	; lsr	r2, r2, #1
	DCD	&e1a02fa2 	; lsr	r2, r2, #31
	DCD	&e1a01332 	; lsr	r1, r2, r3
	DCD	&e1a02332 	; lsr	r2, r2, r3

	DCD	&e1a01002 	; mov	r1, r2
	DCD	&e1a010e2 	; ror	r1, r2, #1
	DCD	&e1a01fe2 	; ror	r1, r2, #31
	DCD	&e1a02002 	; mov	r2, r2
	DCD	&e1a020e2 	; ror	r2, r2, #1
	DCD	&e1a02fe2 	; ror	r2, r2, #31
	DCD	&e1a01372 	; ror	r1, r2, r3
	DCD	&e1a02372 	; ror	r2, r2, r3

	DCD	&e1a01062 	; rrx	r1, r2
	DCD	&e1a02062 	; rrx	r2, r2

	]

	[ :LNOT: REFERENCE

	; We don't test NV condition.
	LSLEQ	r1, r2
	LSLNE	r1, r2
	LSLCS	r1, r2
	LSLHS	r1, r2
	LSLCC	r1, r2
	LSLLO	r1, r2
	LSLMI	r1, r2
	LSLPL	r1, r2
	LSLVS	r1, r2
	LSLVC	r1, r2
	LSLHI	r1, r2
	LSLLS	r1, r2
	LSLGE	r1, r2
	LSLLT	r1, r2
	LSLGT	r1, r2
	LSLLE	r1, r2
	LSLAL	r1, r2
	LSL	r1, r2

	LSLSEQ	r1, r2
	LSLSNE	r1, r2
	LSLSCS	r1, r2
	LSLSHS	r1, r2
	LSLSCC	r1, r2
	LSLSLO	r1, r2
	LSLSMI	r1, r2
	LSLSPL	r1, r2
	LSLSVS	r1, r2
	LSLSVC	r1, r2
	LSLSHI	r1, r2
	LSLSLS	r1, r2
	LSLSGE	r1, r2
	LSLSLT	r1, r2
	LSLSGT	r1, r2
	LSLSLE	r1, r2
	LSLSAL	r1, r2
	LSLS	r1, r2

	|

	DCD &01a01211 	; lsleq	r1, r1, r2
	DCD &11a01211 	; lslne	r1, r1, r2
	DCD &21a01211 	; lslcs	r1, r1, r2
	DCD &21a01211 	; lslcs	r1, r1, r2
	DCD &31a01211 	; lslcc	r1, r1, r2
	DCD &31a01211 	; lslcc	r1, r1, r2
	DCD &41a01211 	; lslmi	r1, r1, r2
	DCD &51a01211 	; lslpl	r1, r1, r2
	DCD &61a01211 	; lslvs	r1, r1, r2
	DCD &71a01211 	; lslvc	r1, r1, r2
	DCD &81a01211 	; lslhi	r1, r1, r2
	DCD &91a01211 	; lslls	r1, r1, r2
	DCD &a1a01211 	; lslge	r1, r1, r2
	DCD &b1a01211 	; lsllt	r1, r1, r2
	DCD &c1a01211 	; lslgt	r1, r1, r2
	DCD &d1a01211 	; lslle	r1, r1, r2
	DCD &e1a01211 	; lsl	r1, r1, r2
	DCD &e1a01211 	; lsl	r1, r1, r2

	DCD &01b01211 	; lslseq	r1, r1, r2
	DCD &11b01211 	; lslsne	r1, r1, r2
	DCD &21b01211 	; lslscs	r1, r1, r2
	DCD &21b01211 	; lslscs	r1, r1, r2
	DCD &31b01211 	; lslscc	r1, r1, r2
	DCD &31b01211 	; lslscc	r1, r1, r2
	DCD &41b01211 	; lslsmi	r1, r1, r2
	DCD &51b01211 	; lslspl	r1, r1, r2
	DCD &61b01211 	; lslsvs	r1, r1, r2
	DCD &71b01211 	; lslsvc	r1, r1, r2
	DCD &81b01211 	; lslshi	r1, r1, r2
	DCD &91b01211 	; lslsls	r1, r1, r2
	DCD &a1b01211 	; lslsge	r1, r1, r2
	DCD &b1b01211 	; lslslt	r1, r1, r2
	DCD &c1b01211 	; lslsgt	r1, r1, r2
	DCD &d1b01211 	; lslsle	r1, r1, r2
	DCD &e1b01211 	; lsls	r1, r1, r2
	DCD &e1b01211 	; lsls	r1, r1, r2

	]

	END