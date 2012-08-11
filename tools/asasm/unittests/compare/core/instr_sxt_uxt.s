; Tests SXT/SXTA/UXT/UXTA

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	MACRO
	TestXT $f,$w
	LCLL Thumb16
Thumb16	SETL {CODESIZE} = 16 :LAND: "$w" = ".N"
	; No 16 bit Thumb support for SXTA/UXTA, B16 flavours or non 0 ROR
	; values.

	$f.XTB$w	r1, r2
	$f.XTB$w	r3
	$f.XTBCC$w	r1, r2, ROR#0
	[ :LNOT: Thumb16
	$f.XTBHI$w	r1, r2, ROR#24
	$f.XTBCS$w	r2, ROR#16
	]

	[ :LNOT: Thumb16
	$f.XTAB$w	r1, r2, r4
	$f.XTAB$w	r3, r4
	$f.XTABCC$w	r1, r2, r4, ROR#0
	$f.XTABHI$w	r1, r2, r4, ROR#24
	$f.XTABCS$w	r2, r4, ROR#16

	$f.XTB16$w	r1, r2
	$f.XTB16$w	r3
	$f.XTB16CC$w	r1, r2, ROR#0
	$f.XTB16HI$w	r1, r2, ROR#24
	$f.XTB16CS$w	r2, ROR#16

	$f.XTAB16$w	r1, r2, r4
	$f.XTAB16$w	r3, r4
	$f.XTAB16CC$w	r1, r2, r4, ROR#0
	$f.XTAB16HI$w	r1, r2, r4, ROR#24
	$f.XTAB16CS$w	r2, r4, ROR#16
	]

	$f.XTH$w	r1, r2
	$f.XTH$w	r3
	$f.XTHCC$w	r1, r2, ROR#0
	[ :LNOT: Thumb16
	$f.XTHHI$w	r1, r2, ROR#24
	$f.XTHCS$w	r2, ROR#16
	]

	[ :LNOT: Thumb16
	$f.XTAH$w	r1, r2, r4
	$f.XTAH$w	r3, r4
	$f.XTAHCC$w	r1, r2, r4, ROR#0
	$f.XTAHHI$w	r1, r2, r4, ROR#24
	$f.XTAHCS$w	r2, r4, ROR#16
	]
	MEND

	; SXT/SXTA
	ARM
	DCI	0
	TestXT S,
	TestXT S,.W

	THUMB
	DCI	0
	TestXT S,
	TestXT S,.W
	TestXT S,.N

	ALIGN

	; UXT/UXTA
	ARM
	DCI	0
	TestXT U,
	TestXT U,.W

	THUMB
	DCI	0
	TestXT U,
	TestXT U,.W
	TestXT U,.N

	ALIGN

	|

	; SXT/SXTA
	ARM
	DCI &00000000 	; andeq	r0, r0, r0
	; B
	DCI &e6af1072 	; sxtb	r1, r2
	DCI &e6af3073 	; sxtb	r3, r3
	DCI &36af1072 	; sxtbcc	r1, r2
	DCI &86af1c72 	; sxtbhi	r1, r2, ror #24
	DCI &26af2872 	; sxtbcs	r2, r2, ror #16

	DCI &e6a21074 	; sxtab	r1, r2, r4
	DCI &e6a33074 	; sxtab	r3, r3, r4
	DCI &36a21074 	; sxtabcc	r1, r2, r4
	DCI &86a21c74 	; sxtabhi	r1, r2, r4, ror #24
	DCI &26a22874 	; sxtabcs	r2, r2, r4, ror #16

	; B16
	DCI &e68f1072 	; sxtb16	r1, r2
	DCI &e68f3073 	; sxtb16	r3, r3
	DCI &368f1072 	; sxtb16cc	r1, r2
	DCI &868f1c72 	; sxtb16hi	r1, r2, ror #24
	DCI &268f2872 	; sxtb16cs	r2, r2, ror #16

	DCI &e6821074 	; sxtab16	r1, r2, r4
	DCI &e6833074 	; sxtab16	r3, r3, r4
	DCI &36821074 	; sxtab16cc	r1, r2, r4
	DCI &86821c74 	; sxtab16hi	r1, r2, r4, ror #24
	DCI &26822874 	; sxtab16cs	r2, r2, r4, ror #16

	; H
	DCI &e6bf1072 	; sxth	r1, r2
	DCI &e6bf3073 	; sxth	r3, r3
	DCI &36bf1072 	; sxthcc	r1, r2
	DCI &86bf1c72 	; sxthhi	r1, r2, ror #24
	DCI &26bf2872 	; sxthcs	r2, r2, ror #16

	DCI &e6b21074 	; sxtah	r1, r2, r4
	DCI &e6b33074 	; sxtah	r3, r3, r4
	DCI &36b21074 	; sxtahcc	r1, r2, r4
	DCI &86b21c74 	; sxtahhi	r1, r2, r4, ror #24
	DCI &26b22874 	; sxtahcs	r2, r2, r4, ror #16

	; B (with .W)
	DCI &e6af1072 	; sxtb	r1, r2
	DCI &e6af3073 	; sxtb	r3, r3
	DCI &36af1072 	; sxtbcc	r1, r2
	DCI &86af1c72 	; sxtbhi	r1, r2, ror #24
	DCI &26af2872 	; sxtbcs	r2, r2, ror #16

	DCI &e6a21074 	; sxtab	r1, r2, r4
	DCI &e6a33074 	; sxtab	r3, r3, r4
	DCI &36a21074 	; sxtabcc	r1, r2, r4
	DCI &86a21c74 	; sxtabhi	r1, r2, r4, ror #24
	DCI &26a22874 	; sxtabcs	r2, r2, r4, ror #16

	; B16 (with .W)
	DCI &e68f1072 	; sxtb16	r1, r2
	DCI &e68f3073 	; sxtb16	r3, r3
	DCI &368f1072 	; sxtb16cc	r1, r2
	DCI &868f1c72 	; sxtb16hi	r1, r2, ror #24
	DCI &268f2872 	; sxtb16cs	r2, r2, ror #16

	DCI &e6821074 	; sxtab16	r1, r2, r4
	DCI &e6833074 	; sxtab16	r3, r3, r4
	DCI &36821074 	; sxtab16cc	r1, r2, r4
	DCI &86821c74 	; sxtab16hi	r1, r2, r4, ror #24
	DCI &26822874 	; sxtab16cs	r2, r2, r4, ror #16

	; H (with .W)
	DCI &e6bf1072 	; sxth	r1, r2
	DCI &e6bf3073 	; sxth	r3, r3
	DCI &36bf1072 	; sxthcc	r1, r2
	DCI &86bf1c72 	; sxthhi	r1, r2, ror #24
	DCI &26bf2872 	; sxthcs	r2, r2, ror #16

	DCI &e6b21074 	; sxtah	r1, r2, r4
	DCI &e6b33074 	; sxtah	r3, r3, r4
	DCI &36b21074 	; sxtahcc	r1, r2, r4
	DCI &86b21c74 	; sxtahhi	r1, r2, r4, ror #24
	DCI &26b22874 	; sxtahcs	r2, r2, r4, ror #16

	THUMB
	DCI.N &0000      ; 	movs	r0, r0
	; B
	DCI.N &b251      ; 	sxtb	r1, r2
	DCI.N &b25b      ; 	sxtb	r3, r3
	DCI.N &b251      ; 	sxtb	r1, r2  FIXME: IT support missing
	DCI.W &fa4ff1b2 ; 	sxtb.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa4ff2a2 ; 	sxtb.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa42f184 ; 	sxtab	r1, r2, r4
	DCI.W &fa43f384 ; 	sxtab	r3, r3, r4
	DCI.W &fa42f184 ; 	sxtab	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa42f1b4 ; 	sxtab	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa42f2a4 ; 	sxtab	r2, r2, r4, ror #16  FIXME: IT support missing

	; B16
	DCI.W &fa2ff182 ; 	sxtb16	r1, r2
	DCI.W &fa2ff383 ; 	sxtb16	r3, r3
	DCI.W &fa2ff182 ; 	sxtb16	r1, r2  FIXME: IT support missing
	DCI.W &fa2ff1b2 ; 	sxtb16	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa2ff2a2 ; 	sxtb16	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa22f184 ; 	sxtab16	r1, r2, r4
	DCI.W &fa23f384 ; 	sxtab16	r3, r3, r4
	DCI.W &fa22f184 ; 	sxtab16	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa22f1b4 ; 	sxtab16	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa22f2a4 ; 	sxtab16	r2, r2, r4, ror #16  FIXME: IT support missing

	; H
	DCI.N &b211      ; 	sxth	r1, r2
	DCI.N &b21b      ; 	sxth	r3, r3
	DCI.N &b211      ; 	sxth	r1, r2  FIXME: IT support missing
	DCI.W &fa0ff1b2 ; 	sxth.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa0ff2a2 ; 	sxth.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa02f184 ; 	sxtah	r1, r2, r4
	DCI.W &fa03f384 ; 	sxtah	r3, r3, r4
	DCI.W &fa02f184 ; 	sxtah	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa02f1b4 ; 	sxtah	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa02f2a4 ; 	sxtah	r2, r2, r4, ror #16  FIXME: IT support missing

	; B (with .W)
	DCI.W &fa4ff182 ; 	sxtb.w	r1, r2
	DCI.W &fa4ff383 ; 	sxtb.w	r3, r3
	DCI.W &fa4ff182 ; 	sxtb.w	r1, r2  FIXME: IT support missing
	DCI.W &fa4ff1b2 ; 	sxtb.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa4ff2a2 ; 	sxtb.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa42f184 ; 	sxtab	r1, r2, r4
	DCI.W &fa43f384 ; 	sxtab	r3, r3, r4
	DCI.W &fa42f184 ; 	sxtab	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa42f1b4 ; 	sxtab	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa42f2a4 ; 	sxtab	r2, r2, r4, ror #16  FIXME: IT support missing

	; B16 (with .W)
	DCI.W &fa2ff182 ; 	sxtb16	r1, r2
	DCI.W &fa2ff383 ; 	sxtb16	r3, r3
	DCI.W &fa2ff182 ; 	sxtb16	r1, r2  FIXME: IT support missing
	DCI.W &fa2ff1b2 ; 	sxtb16	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa2ff2a2 ; 	sxtb16	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa22f184 ; 	sxtab16	r1, r2, r4
	DCI.W &fa23f384 ; 	sxtab16	r3, r3, r4
	DCI.W &fa22f184 ; 	sxtab16	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa22f1b4 ; 	sxtab16	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa22f2a4 ; 	sxtab16	r2, r2, r4, ror #16

	; H (with .W)
	DCI.W &fa0ff182 ; 	sxth.w	r1, r2
	DCI.W &fa0ff383 ; 	sxth.w	r3, r3
	DCI.W &fa0ff182 ; 	sxth.w	r1, r2  FIXME: IT support missing
	DCI.W &fa0ff1b2 ; 	sxth.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa0ff2a2 ; 	sxth.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa02f184 ; 	sxtah	r1, r2, r4
	DCI.W &fa03f384 ; 	sxtah	r3, r3, r4
	DCI.W &fa02f184 ; 	sxtah	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa02f1b4 ; 	sxtah	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa02f2a4 ; 	sxtah	r2, r2, r4, ror #16  FIXME: IT support missing

	; B (with .N)
	DCI.N &b251      ; 	sxtb	r1, r2
	DCI.N &b25b      ; 	sxtb	r3, r3
	DCI.N &b251      ; 	sxtb	r1, r2  FIXME: IT support missing

	; B16 (with .N)

	; H (with .H)
	DCI.N &b211      ; 	sxth	r1, r2
	DCI.N &b21b      ; 	sxth	r3, r3
	DCI.N &b211      ; 	sxth	r1, r2  FIXME: IT support missing

	DCI.N &0000      ; 	movs	r0, r0

	; LXT/LXTA
	ARM
	DCI &00000000 	; andeq	r0, r0, r0
	; B
	DCI &e6ef1072 	; uxtb	r1, r2
	DCI &e6ef3073 	; uxtb	r3, r3
	DCI &36ef1072 	; uxtbcc	r1, r2
	DCI &86ef1c72 	; uxtbhi	r1, r2, ror #24
	DCI &26ef2872 	; uxtbcs	r2, r2, ror #16

	DCI &e6e21074 	; uxtab	r1, r2, r4
	DCI &e6e33074 	; uxtab	r3, r3, r4
	DCI &36e21074 	; uxtabcc	r1, r2, r4
	DCI &86e21c74 	; uxtabhi	r1, r2, r4, ror #24
	DCI &26e22874 	; uxtabcs	r2, r2, r4, ror #16

	; B16
	DCI &e6cf1072 	; uxtb16	r1, r2
	DCI &e6cf3073 	; uxtb16	r3, r3
	DCI &36cf1072 	; uxtb16cc	r1, r2
	DCI &86cf1c72 	; uxtb16hi	r1, r2, ror #24
	DCI &26cf2872 	; uxtb16cs	r2, r2, ror #16

	DCI &e6c21074 	; uxtab16	r1, r2, r4
	DCI &e6c33074 	; uxtab16	r3, r3, r4
	DCI &36c21074 	; uxtab16cc	r1, r2, r4
	DCI &86c21c74 	; uxtab16hi	r1, r2, r4, ROR #24
	DCI &26c22874 	; uxtab16cs	r2, r2, r4, ror #16

	; H
	DCI &e6ff1072 	; uxth	r1, r2
	DCI &e6ff3073 	; uxth	r3, r3
	DCI &36ff1072 	; uxthcc	r1, r2
	DCI &86ff1c72 	; uxthhi	r1, r2, ror #24
	DCI &26ff2872 	; uxthcs	r2, r2, ror #16

	DCI &e6f21074 	; uxtah	r1, r2, r4
	DCI &e6f33074 	; uxtah	r3, r3, r4
	DCI &36f21074 	; uxtahcc	r1, r2, r4
	DCI &86f21c74 	; uxtahhi	r1, r2, r4, ror #24
	DCI &26f22874 	; uxtahcs	r2, r2, r4, ror #16

	; B (with .W)
	DCI &e6ef1072 	; uxtb	r1, r2
	DCI &e6ef3073 	; uxtb	r3, r3
	DCI &36ef1072 	; uxtbcc	r1, r2
	DCI &86ef1c72 	; uxtbhi	r1, r2, ror #24
	DCI &26ef2872 	; uxtbcs	r2, r2, ror #16

	DCI &e6e21074 	; uxtab	r1, r2, r4
	DCI &e6e33074 	; uxtab	r3, r3, r4
	DCI &36e21074 	; uxtabcc	r1, r2, r4
	DCI &86e21c74 	; uxtabhi	r1, r2, r4, ror #24
	DCI &26e22874 	; uxtabcs	r2, r2, r4, ror #16

	; B16 (with .W)
	DCI &e6cf1072 	; uxtb16	r1, r2
	DCI &e6cf3073 	; uxtb16	r3, r3
	DCI &36cf1072 	; uxtb16cc	r1, r2
	DCI &86cf1c72 	; uxtb16hi	r1, r2, ror #24
	DCI &26cf2872 	; uxtb16cs	r2, r2, ror #16

	DCI &e6c21074 	; uxtab16	r1, r2, r4
	DCI &e6c33074 	; uxtab16	r3, r3, r4
	DCI &36c21074 	; uxtab16cc	r1, r2, r4
	DCI &86c21c74 	; uxtab16hi	r1, r2, r4, ROR #24
	DCI &26c22874 	; uxtab16cs	r2, r2, r4, ror #16

	; H (with .W)
	DCI &e6ff1072 	; uxth	r1, r2
	DCI &e6ff3073 	; uxth	r3, r3
	DCI &36ff1072 	; uxthcc	r1, r2
	DCI &86ff1c72 	; uxthhi	r1, r2, ror #24
	DCI &26ff2872 	; uxthcs	r2, r2, ror #16

	DCI &e6f21074 	; uxtah	r1, r2, r4
	DCI &e6f33074 	; uxtah	r3, r3, r4
	DCI &36f21074 	; uxtahcc	r1, r2, r4
	DCI &86f21c74 	; uxtahhi	r1, r2, r4, ror #24
	DCI &26f22874 	; uxtahcs	r2, r2, r4, ror #16

	THUMB
	DCI.N &0000      ; 	movs	r0, r0
	; B
	DCI.N &b2d1      ; 	uxtb	r1, r2
	DCI.N &b2db      ; 	uxtb	r3, r3
	DCI.N &b2d1      ; 	uxtb	r1, r2  FIXME: IT support missing
	DCI.W &fa5ff1b2 ; 	uxtb.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa5ff2a2 ; 	uxtb.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa52f184 ; 	uxtab	r1, r2, r4
	DCI.W &fa53f384 ; 	uxtab	r3, r3, r4
	DCI.W &fa52f184 ; 	uxtab	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa52f1b4 ; 	uxtab	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa52f2a4 ; 	uxtab	r2, r2, r4, ror #16  FIXME: IT support missing

	; B16
	DCI.W &fa3ff182 ; 	uxtb16	r1, r2
	DCI.W &fa3ff383 ; 	uxtb16	r3, r3
	DCI.W &fa3ff182 ; 	uxtb16	r1, r2  FIXME: IT support missing
	DCI.W &fa3ff1b2 ; 	uxtb16	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa3ff2a2 ; 	uxtb16	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa32f184 ; 	uxtab16	r1, r2, r4
	DCI.W &fa33f384 ; 	uxtab16	r3, r3, r4
	DCI.W &fa32f184 ; 	uxtab16	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa32f1b4 ; 	uxtab16	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa32f2a4 ; 	uxtab16	r2, r2, r4, ror #16  FIXME: IT support missing

	; H
	DCI.N &b291      ; 	uxth	r1, r2
	DCI.N &b29b      ; 	uxth	r3, r3
	DCI.N &b291      ; 	uxth	r1, r2  FIXME: IT support missing
	DCI.W &fa1ff1b2 ; 	uxth.w	r1, r2, ror #24  FIXME: IT support missingc
	DCI.W &fa1ff2a2 ; 	uxth.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa12f184 ; 	uxtah	r1, r2, r4
	DCI.W &fa13f384 ; 	uxtah	r3, r3, r4
	DCI.W &fa12f184 ; 	uxtah	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa12f1b4 ; 	uxtah	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa12f2a4 ; 	uxtah	r2, r2, r4, ror #16  FIXME: IT support missing

	; B (with .W)
	DCI.W &fa5ff182 ; 	uxtb.w	r1, r2
	DCI.W &fa5ff383 ; 	uxtb.w	r3, r3
	DCI.W &fa5ff182 ; 	uxtb.w	r1, r2  FIXME: IT support missing
	DCI.W &fa5ff1b2 ; 	uxtb.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa5ff2a2 ; 	uxtb.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa52f184 ; 	uxtab	r1, r2, r4
	DCI.W &fa53f384 ; 	uxtab	r3, r3, r4
	DCI.W &fa52f184 ; 	uxtab	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa52f1b4 ; 	uxtab	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa52f2a4 ; 	uxtab	r2, r2, r4, ror #16  FIXME: IT support missing

	; B16 (with .W)
	DCI.W &fa3ff182 ; 	uxtb16	r1, r2
	DCI.W &fa3ff383 ; 	uxtb16	r3, r3
	DCI.W &fa3ff182 ; 	uxtb16	r1, r2  FIXME: IT support missing
	DCI.W &fa3ff1b2 ; 	uxtb16	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa3ff2a2 ; 	uxtb16	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa32f184 ; 	uxtab16	r1, r2, r4
	DCI.W &fa33f384 ; 	uxtab16	r3, r3, r4
	DCI.W &fa32f184 ; 	uxtab16	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa32f1b4 ; 	uxtab16	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa32f2a4 ; 	uxtab16	r2, r2, r4, ror #16  FIXME: IT support missing

	; H (with .W)
	DCI.W &fa1ff182 ; 	uxth.w	r1, r2
	DCI.W &fa1ff383 ; 	uxth.w	r3, r3
	DCI.W &fa1ff182 ; 	uxth.w	r1, r2  FIXME: IT support missing
	DCI.W &fa1ff1b2 ; 	uxth.w	r1, r2, ror #24  FIXME: IT support missing
	DCI.W &fa1ff2a2 ; 	uxth.w	r2, r2, ror #16  FIXME: IT support missing

	DCI.W &fa12f184 ; 	uxtah	r1, r2, r4
	DCI.W &fa13f384 ; 	uxtah	r3, r3, r4
	DCI.W &fa12f184 ; 	uxtah	r1, r2, r4  FIXME: IT support missing
	DCI.W &fa12f1b4 ; 	uxtah	r1, r2, r4, ror #24  FIXME: IT support missing
	DCI.W &fa12f2a4 ; 	uxtah	r2, r2, r4, ror #16  FIXME: IT support missing

	; B (with .N)
	DCI.N &b2d1      ; 	uxtb	r1, r2
	DCI.N &b2db      ; 	uxtb	r3, r3
	DCI.N &b2d1      ; 	uxtb	r1, r2  FIXME: IT support missing

	; B16 (with .N)

	; H (with .N)
	DCI.N &b291      ; 	uxth	r1, r2
	DCI.N &b29b      ; 	uxth	r3, r3
	DCI.N &b291      ; 	uxth	r1, r2  FIXME: IT support missing

	DCI.N &0000      ; 	movs	r0, r0

	]

	END
