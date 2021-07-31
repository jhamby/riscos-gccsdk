; Tests MUL, MLA, MLS.
; RUNOPT: -CPU=7-A

		AREA Code, CODE, READONLY

	[ :LNOT: REFERENCE
		MUL	R1,R2,R3
		MULS	 R1 ,   R2 , R3

		MLA	R1,R2,   R3  ,  R4
		MLAS	R1,R2,R3,R4

		MLS	R1,R2,R3,R4	; Does not have 'S' version.

		; MUL (with condition codes):
		MULEQ	R1,R2,R3
		MULNE	R1,R2,R3
		MULCS	R1,R2,R3
		MULHS	R1,R2,R3
		MULCC	R1,R2,R3
		MULLO	R1,R2,R3
		MULMI	R1,R2,R3
		MULPL	R1,R2,R3
		MULVS	R1,R2,R3
		MULVC	R1,R2,R3
		MULHI	R1,R2,R3
		MULLS	R1,R2,R3
		MULGE	R1,R2,R3
		MULLT	R1,R2,R3
		MULGT	R1,R2,R3
		MULLE	R1,R2,R3
		MULAL	R1,R2,R3
		MUL	R1,R2,R3

		; MUL (pre-UAL):
		MULEQS	R1,R2,R3
		MULNES	R1,R2,R3
		MULCSS	R1,R2,R3
		MULHSS	R1,R2,R3
		MULCCS	R1,R2,R3
		MULLOS	R1,R2,R3
		MULMIS	R1,R2,R3
		MULPLS	R1,R2,R3
		MULVSS	R1,R2,R3
		MULVCS	R1,R2,R3
		MULHIS	R1,R2,R3
		MULLSS	R1,R2,R3
		MULGES	R1,R2,R3
		MULLTS	R1,R2,R3
		MULGTS	R1,R2,R3
		MULLES	R1,R2,R3
		MULALS	R1,R2,R3
		MULS	R1,R2,R3

		; MUL (UAL):
		MULSEQ	R1,R2,R3
		MULSNE	R1,R2,R3
		MULSCS	R1,R2,R3
		MULSHS	R1,R2,R3
		MULSCC	R1,R2,R3
		MULSLO	R1,R2,R3
		MULSMI	R1,R2,R3
		MULSPL	R1,R2,R3
		MULSVS	R1,R2,R3
		MULSVC	R1,R2,R3
		MULSHI	R1,R2,R3
		MULSLS	R1,R2,R3
		MULSGE	R1,R2,R3
		MULSLT	R1,R2,R3
		MULSGT	R1,R2,R3
		MULSLE	R1,R2,R3
		MULSAL	R1,R2,R3
		MULS	R1,R2,R3

		; MLA (with condition codes):
		MLAEQ	R1,R2,R3,R4
		MLANE	R1,R2,R3,R4
		MLACS	R1,R2,R3,R4
		MLAHS	R1,R2,R3,R4
		MLACC	R1,R2,R3,R4
		MLALO	R1,R2,R3,R4
		MLAMI	R1,R2,R3,R4
		MLAPL	R1,R2,R3,R4
		MLAVS	R1,R2,R3,R4
		MLAVC	R1,R2,R3,R4
		MLAHI	R1,R2,R3,R4
		MLALS	R1,R2,R3,R4
		MLAGE	R1,R2,R3,R4
		MLALT	R1,R2,R3,R4
		MLAGT	R1,R2,R3,R4
		MLALE	R1,R2,R3,R4
		MLAAL	R1,R2,R3,R4
		MLA	R1,R2,R3,R4

		; MLA (pre-UAL):
		MLAEQS	R1,R2,R3,R4
		MLANES	R1,R2,R3,R4
		MLACSS	R1,R2,R3,R4
		MLAHSS	R1,R2,R3,R4
		MLACCS	R1,R2,R3,R4
		MLALOS	R1,R2,R3,R4
		MLAMIS	R1,R2,R3,R4
		MLAPLS	R1,R2,R3,R4
		MLAVSS	R1,R2,R3,R4
		MLAVCS	R1,R2,R3,R4
		MLAHIS	R1,R2,R3,R4
		MLALSS	R1,R2,R3,R4
		MLAGES	R1,R2,R3,R4
		MLALTS	R1,R2,R3,R4
		MLAGTS	R1,R2,R3,R4
		MLALES	R1,R2,R3,R4
		MLAALS	R1,R2,R3,R4
		MLAS	R1,R2,R3,R4

		; MLA (UAL):
		MLASEQ	R1,R2,R3,R4
		MLASNE	R1,R2,R3,R4
		MLASCS	R1,R2,R3,R4
		MLASHS	R1,R2,R3,R4
		MLASCC	R1,R2,R3,R4
		MLASLO	R1,R2,R3,R4
		MLASMI	R1,R2,R3,R4
		MLASPL	R1,R2,R3,R4
		MLASVS	R1,R2,R3,R4
		MLASVC	R1,R2,R3,R4
		MLASHI	R1,R2,R3,R4
		MLASLS	R1,R2,R3,R4
		MLASGE	R1,R2,R3,R4
		MLASLT	R1,R2,R3,R4
		MLASGT	R1,R2,R3,R4
		MLASLE	R1,R2,R3,R4
		MLASAL	R1,R2,R3,R4
		MLAS	R1,R2,R3,R4

		; MLS (with condition codes):
		MLSEQ	R1,R2,R3,R4
		MLSNE	R1,R2,R3,R4
		MLSCS	R1,R2,R3,R4
		MLSHS	R1,R2,R3,R4
		MLSCC	R1,R2,R3,R4
		MLSLO	R1,R2,R3,R4
		MLSMI	R1,R2,R3,R4
		MLSPL	R1,R2,R3,R4
		MLSVS	R1,R2,R3,R4
		MLSVC	R1,R2,R3,R4
		MLSHI	R1,R2,R3,R4
		MLSLS	R1,R2,R3,R4
		MLSGE	R1,R2,R3,R4
		MLSLT	R1,R2,R3,R4
		MLSGT	R1,R2,R3,R4
		MLSLE	R1,R2,R3,R4
		MLSAL	R1,R2,R3,R4
		MLS	R1,R2,R3,R4
	|
		DCI &e0010392 	; mul	r1, r2, r3
		DCI &e0110392 	; muls	r1, r2, r3

		DCI &e0214392 	; mla	r1, r2, r3, r4
		DCI &e0314392 	; mlas	r1, r2, r3, r4

		DCI &e0614392 	; mls	r1, r2, r3, r4

		DCI &00010392 	; muleq	r1, r2, r3
		DCI &10010392 	; mulne	r1, r2, r3
		DCI &20010392 	; mulcs	r1, r2, r3
		DCI &20010392 	; mulcs	r1, r2, r3
		DCI &30010392 	; mulcc	r1, r2, r3
		DCI &30010392 	; mulcc	r1, r2, r3
		DCI &40010392 	; mulmi	r1, r2, r3
		DCI &50010392 	; mulpl	r1, r2, r3
		DCI &60010392 	; mulvs	r1, r2, r3
		DCI &70010392 	; mulvc	r1, r2, r3
		DCI &80010392 	; mulhi	r1, r2, r3
		DCI &90010392 	; mulls	r1, r2, r3
		DCI &a0010392 	; mulge	r1, r2, r3
		DCI &b0010392 	; mullt	r1, r2, r3
		DCI &c0010392 	; mulgt	r1, r2, r3
		DCI &d0010392 	; mulle	r1, r2, r3
		DCI &e0010392 	; mul	r1, r2, r3
		DCI &e0010392 	; mul	r1, r2, r3

		DCI &00110392 	; mulseq	r1, r2, r3
		DCI &10110392 	; mulsne	r1, r2, r3
		DCI &20110392 	; mulscs	r1, r2, r3
		DCI &20110392 	; mulscs	r1, r2, r3
		DCI &30110392 	; mulscc	r1, r2, r3
		DCI &30110392 	; mulscc	r1, r2, r3
		DCI &40110392 	; mulsmi	r1, r2, r3
		DCI &50110392 	; mulspl	r1, r2, r3
		DCI &60110392 	; mulsvs	r1, r2, r3
		DCI &70110392 	; mulsvc	r1, r2, r3
		DCI &80110392 	; mulshi	r1, r2, r3
		DCI &90110392 	; mulsls	r1, r2, r3
		DCI &a0110392 	; mulsge	r1, r2, r3
		DCI &b0110392 	; mulslt	r1, r2, r3
		DCI &c0110392 	; mulsgt	r1, r2, r3
		DCI &d0110392 	; mulsle	r1, r2, r3
		DCI &e0110392 	; muls	r1, r2, r3
		DCI &e0110392 	; muls	r1, r2, r3

		DCI &00110392 	; mulseq	r1, r2, r3
		DCI &10110392 	; mulsne	r1, r2, r3
		DCI &20110392 	; mulscs	r1, r2, r3
		DCI &20110392 	; mulscs	r1, r2, r3
		DCI &30110392 	; mulscc	r1, r2, r3
		DCI &30110392 	; mulscc	r1, r2, r3
		DCI &40110392 	; mulsmi	r1, r2, r3
		DCI &50110392 	; mulspl	r1, r2, r3
		DCI &60110392 	; mulsvs	r1, r2, r3
		DCI &70110392 	; mulsvc	r1, r2, r3
		DCI &80110392 	; mulshi	r1, r2, r3
		DCI &90110392 	; mulsls	r1, r2, r3
		DCI &a0110392 	; mulsge	r1, r2, r3
		DCI &b0110392 	; mulslt	r1, r2, r3
		DCI &c0110392 	; mulsgt	r1, r2, r3
		DCI &d0110392 	; mulsle	r1, r2, r3
		DCI &e0110392 	; muls	r1, r2, r3
		DCI &e0110392 	; muls	r1, r2, r3

		DCI &00214392 	; mlaeq	r1, r2, r3, r4
		DCI &10214392 	; mlane	r1, r2, r3, r4
		DCI &20214392 	; mlacs	r1, r2, r3, r4
		DCI &20214392 	; mlacs	r1, r2, r3, r4
		DCI &30214392 	; mlacc	r1, r2, r3, r4
		DCI &30214392 	; mlacc	r1, r2, r3, r4
		DCI &40214392 	; mlami	r1, r2, r3, r4
		DCI &50214392 	; mlapl	r1, r2, r3, r4
		DCI &60214392 	; mlavs	r1, r2, r3, r4
		DCI &70214392 	; mlavc	r1, r2, r3, r4
		DCI &80214392 	; mlahi	r1, r2, r3, r4
		DCI &90214392 	; mlals	r1, r2, r3, r4
		DCI &a0214392 	; mlage	r1, r2, r3, r4
		DCI &b0214392 	; mlalt	r1, r2, r3, r4
		DCI &c0214392 	; mlagt	r1, r2, r3, r4
		DCI &d0214392 	; mlale	r1, r2, r3, r4
		DCI &e0214392 	; mla	r1, r2, r3, r4
		DCI &e0214392 	; mla	r1, r2, r3, r4

		DCI &00314392 	; mlaseq	r1, r2, r3, r4
		DCI &10314392 	; mlasne	r1, r2, r3, r4
		DCI &20314392 	; mlascs	r1, r2, r3, r4
		DCI &20314392 	; mlascs	r1, r2, r3, r4
		DCI &30314392 	; mlascc	r1, r2, r3, r4
		DCI &30314392 	; mlascc	r1, r2, r3, r4
		DCI &40314392 	; mlasmi	r1, r2, r3, r4
		DCI &50314392 	; mlaspl	r1, r2, r3, r4
		DCI &60314392 	; mlasvs	r1, r2, r3, r4
		DCI &70314392 	; mlasvc	r1, r2, r3, r4
		DCI &80314392 	; mlashi	r1, r2, r3, r4
		DCI &90314392 	; mlasls	r1, r2, r3, r4
		DCI &a0314392 	; mlasge	r1, r2, r3, r4
		DCI &b0314392 	; mlaslt	r1, r2, r3, r4
		DCI &c0314392 	; mlasgt	r1, r2, r3, r4
		DCI &d0314392 	; mlasle	r1, r2, r3, r4
		DCI &e0314392 	; mlas	r1, r2, r3, r4
		DCI &e0314392 	; mlas	r1, r2, r3, r4

		DCI &00314392 	; mlaseq	r1, r2, r3, r4
		DCI &10314392 	; mlasne	r1, r2, r3, r4
		DCI &20314392 	; mlascs	r1, r2, r3, r4
		DCI &20314392 	; mlascs	r1, r2, r3, r4
		DCI &30314392 	; mlascc	r1, r2, r3, r4
		DCI &30314392 	; mlascc	r1, r2, r3, r4
		DCI &40314392 	; mlasmi	r1, r2, r3, r4
		DCI &50314392 	; mlaspl	r1, r2, r3, r4
		DCI &60314392 	; mlasvs	r1, r2, r3, r4
		DCI &70314392 	; mlasvc	r1, r2, r3, r4
		DCI &80314392 	; mlashi	r1, r2, r3, r4
		DCI &90314392 	; mlasls	r1, r2, r3, r4
		DCI &a0314392 	; mlasge	r1, r2, r3, r4
		DCI &b0314392 	; mlaslt	r1, r2, r3, r4
		DCI &c0314392 	; mlasgt	r1, r2, r3, r4
		DCI &d0314392 	; mlasle	r1, r2, r3, r4
		DCI &e0314392 	; mlas	r1, r2, r3, r4
		DCI &e0314392 	; mlas	r1, r2, r3, r4

		DCI &00614392 	; mlseq	r1, r2, r3, r4
		DCI &10614392 	; mlsne	r1, r2, r3, r4
		DCI &20614392 	; mlscs	r1, r2, r3, r4
		DCI &20614392 	; mlscs	r1, r2, r3, r4
		DCI &30614392 	; mlscc	r1, r2, r3, r4
		DCI &30614392 	; mlscc	r1, r2, r3, r4
		DCI &40614392 	; mlsmi	r1, r2, r3, r4
		DCI &50614392 	; mlspl	r1, r2, r3, r4
		DCI &60614392 	; mlsvs	r1, r2, r3, r4
		DCI &70614392 	; mlsvc	r1, r2, r3, r4
		DCI &80614392 	; mlshi	r1, r2, r3, r4
		DCI &90614392 	; mlsls	r1, r2, r3, r4
		DCI &a0614392 	; mlsge	r1, r2, r3, r4
		DCI &b0614392 	; mlslt	r1, r2, r3, r4
		DCI &c0614392 	; mlsgt	r1, r2, r3, r4
		DCI &d0614392 	; mlsle	r1, r2, r3, r4
		DCI &e0614392 	; mls	r1, r2, r3, r4
		DCI &e0614392 	; mls	r1, r2, r3, r4
	]

		END
