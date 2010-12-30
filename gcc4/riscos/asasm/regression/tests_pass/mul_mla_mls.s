; Tests MUL, MLA, MLS.

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
		DCD &e0010392 	; mul	r1, r2, r3
		DCD &e0110392 	; muls	r1, r2, r3

		DCD &e0214392 	; mla	r1, r2, r3, r4
		DCD &e0314392 	; mlas	r1, r2, r3, r4

		DCD &e0614392 	; mls	r1, r2, r3, r4

		DCD &00010392 	; muleq	r1, r2, r3
		DCD &10010392 	; mulne	r1, r2, r3
		DCD &20010392 	; mulcs	r1, r2, r3
		DCD &20010392 	; mulcs	r1, r2, r3
		DCD &30010392 	; mulcc	r1, r2, r3
		DCD &30010392 	; mulcc	r1, r2, r3
		DCD &40010392 	; mulmi	r1, r2, r3
		DCD &50010392 	; mulpl	r1, r2, r3
		DCD &60010392 	; mulvs	r1, r2, r3
		DCD &70010392 	; mulvc	r1, r2, r3
		DCD &80010392 	; mulhi	r1, r2, r3
		DCD &90010392 	; mulls	r1, r2, r3
		DCD &a0010392 	; mulge	r1, r2, r3
		DCD &b0010392 	; mullt	r1, r2, r3
		DCD &c0010392 	; mulgt	r1, r2, r3
		DCD &d0010392 	; mulle	r1, r2, r3
		DCD &e0010392 	; mul	r1, r2, r3
		DCD &e0010392 	; mul	r1, r2, r3

		DCD &00110392 	; mulseq	r1, r2, r3
		DCD &10110392 	; mulsne	r1, r2, r3
		DCD &20110392 	; mulscs	r1, r2, r3
		DCD &20110392 	; mulscs	r1, r2, r3
		DCD &30110392 	; mulscc	r1, r2, r3
		DCD &30110392 	; mulscc	r1, r2, r3
		DCD &40110392 	; mulsmi	r1, r2, r3
		DCD &50110392 	; mulspl	r1, r2, r3
		DCD &60110392 	; mulsvs	r1, r2, r3
		DCD &70110392 	; mulsvc	r1, r2, r3
		DCD &80110392 	; mulshi	r1, r2, r3
		DCD &90110392 	; mulsls	r1, r2, r3
		DCD &a0110392 	; mulsge	r1, r2, r3
		DCD &b0110392 	; mulslt	r1, r2, r3
		DCD &c0110392 	; mulsgt	r1, r2, r3
		DCD &d0110392 	; mulsle	r1, r2, r3
		DCD &e0110392 	; muls	r1, r2, r3
		DCD &e0110392 	; muls	r1, r2, r3

		DCD &00110392 	; mulseq	r1, r2, r3
		DCD &10110392 	; mulsne	r1, r2, r3
		DCD &20110392 	; mulscs	r1, r2, r3
		DCD &20110392 	; mulscs	r1, r2, r3
		DCD &30110392 	; mulscc	r1, r2, r3
		DCD &30110392 	; mulscc	r1, r2, r3
		DCD &40110392 	; mulsmi	r1, r2, r3
		DCD &50110392 	; mulspl	r1, r2, r3
		DCD &60110392 	; mulsvs	r1, r2, r3
		DCD &70110392 	; mulsvc	r1, r2, r3
		DCD &80110392 	; mulshi	r1, r2, r3
		DCD &90110392 	; mulsls	r1, r2, r3
		DCD &a0110392 	; mulsge	r1, r2, r3
		DCD &b0110392 	; mulslt	r1, r2, r3
		DCD &c0110392 	; mulsgt	r1, r2, r3
		DCD &d0110392 	; mulsle	r1, r2, r3
		DCD &e0110392 	; muls	r1, r2, r3
		DCD &e0110392 	; muls	r1, r2, r3

		DCD &00214392 	; mlaeq	r1, r2, r3, r4
		DCD &10214392 	; mlane	r1, r2, r3, r4
		DCD &20214392 	; mlacs	r1, r2, r3, r4
		DCD &20214392 	; mlacs	r1, r2, r3, r4
		DCD &30214392 	; mlacc	r1, r2, r3, r4
		DCD &30214392 	; mlacc	r1, r2, r3, r4
		DCD &40214392 	; mlami	r1, r2, r3, r4
		DCD &50214392 	; mlapl	r1, r2, r3, r4
		DCD &60214392 	; mlavs	r1, r2, r3, r4
		DCD &70214392 	; mlavc	r1, r2, r3, r4
		DCD &80214392 	; mlahi	r1, r2, r3, r4
		DCD &90214392 	; mlals	r1, r2, r3, r4
		DCD &a0214392 	; mlage	r1, r2, r3, r4
		DCD &b0214392 	; mlalt	r1, r2, r3, r4
		DCD &c0214392 	; mlagt	r1, r2, r3, r4
		DCD &d0214392 	; mlale	r1, r2, r3, r4
		DCD &e0214392 	; mla	r1, r2, r3, r4
		DCD &e0214392 	; mla	r1, r2, r3, r4

		DCD &00314392 	; mlaseq	r1, r2, r3, r4
		DCD &10314392 	; mlasne	r1, r2, r3, r4
		DCD &20314392 	; mlascs	r1, r2, r3, r4
		DCD &20314392 	; mlascs	r1, r2, r3, r4
		DCD &30314392 	; mlascc	r1, r2, r3, r4
		DCD &30314392 	; mlascc	r1, r2, r3, r4
		DCD &40314392 	; mlasmi	r1, r2, r3, r4
		DCD &50314392 	; mlaspl	r1, r2, r3, r4
		DCD &60314392 	; mlasvs	r1, r2, r3, r4
		DCD &70314392 	; mlasvc	r1, r2, r3, r4
		DCD &80314392 	; mlashi	r1, r2, r3, r4
		DCD &90314392 	; mlasls	r1, r2, r3, r4
		DCD &a0314392 	; mlasge	r1, r2, r3, r4
		DCD &b0314392 	; mlaslt	r1, r2, r3, r4
		DCD &c0314392 	; mlasgt	r1, r2, r3, r4
		DCD &d0314392 	; mlasle	r1, r2, r3, r4
		DCD &e0314392 	; mlas	r1, r2, r3, r4
		DCD &e0314392 	; mlas	r1, r2, r3, r4

		DCD &00314392 	; mlaseq	r1, r2, r3, r4
		DCD &10314392 	; mlasne	r1, r2, r3, r4
		DCD &20314392 	; mlascs	r1, r2, r3, r4
		DCD &20314392 	; mlascs	r1, r2, r3, r4
		DCD &30314392 	; mlascc	r1, r2, r3, r4
		DCD &30314392 	; mlascc	r1, r2, r3, r4
		DCD &40314392 	; mlasmi	r1, r2, r3, r4
		DCD &50314392 	; mlaspl	r1, r2, r3, r4
		DCD &60314392 	; mlasvs	r1, r2, r3, r4
		DCD &70314392 	; mlasvc	r1, r2, r3, r4
		DCD &80314392 	; mlashi	r1, r2, r3, r4
		DCD &90314392 	; mlasls	r1, r2, r3, r4
		DCD &a0314392 	; mlasge	r1, r2, r3, r4
		DCD &b0314392 	; mlaslt	r1, r2, r3, r4
		DCD &c0314392 	; mlasgt	r1, r2, r3, r4
		DCD &d0314392 	; mlasle	r1, r2, r3, r4
		DCD &e0314392 	; mlas	r1, r2, r3, r4
		DCD &e0314392 	; mlas	r1, r2, r3, r4

		DCD &00614392 	; mlseq	r1, r2, r3, r4
		DCD &10614392 	; mlsne	r1, r2, r3, r4
		DCD &20614392 	; mlscs	r1, r2, r3, r4
		DCD &20614392 	; mlscs	r1, r2, r3, r4
		DCD &30614392 	; mlscc	r1, r2, r3, r4
		DCD &30614392 	; mlscc	r1, r2, r3, r4
		DCD &40614392 	; mlsmi	r1, r2, r3, r4
		DCD &50614392 	; mlspl	r1, r2, r3, r4
		DCD &60614392 	; mlsvs	r1, r2, r3, r4
		DCD &70614392 	; mlsvc	r1, r2, r3, r4
		DCD &80614392 	; mlshi	r1, r2, r3, r4
		DCD &90614392 	; mlsls	r1, r2, r3, r4
		DCD &a0614392 	; mlsge	r1, r2, r3, r4
		DCD &b0614392 	; mlslt	r1, r2, r3, r4
		DCD &c0614392 	; mlsgt	r1, r2, r3, r4
		DCD &d0614392 	; mlsle	r1, r2, r3, r4
		DCD &e0614392 	; mls	r1, r2, r3, r4
		DCD &e0614392 	; mls	r1, r2, r3, r4
	]

		END
