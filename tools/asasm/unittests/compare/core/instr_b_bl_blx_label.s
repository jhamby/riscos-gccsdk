; Tests B, BL, BLX <label>
; -RUNOPT: -CPU=7-A

	KEEP

	MACRO
	Inject $what
	;INFO 0, "\t$what"
	$what
	MEND

	; ---------------------------------------------------------------
	; B, BL, BLX

		AREA	Code_B_BL_BLX_01, CODE, READONLY

		; Branch instructions
	[ :LNOT: REFERENCE
		; Label defined upfront, label at pos 0.
label11
		B	label11
		B	label11 + 48
		BL	label11
		BL	label11 + 64

		BLX	label11
		BLX	label11 + 2

		; Label defined upfront, label at pos non-0.
label12
		B	label12
		B	label12 + 48
		BL	label12
		BL	label12 + 64

		BLX	label12
		BLX	label12 + 4

		; Label defined afterwards.
		B	label13
		B	label13 + 48
		BL	label13
		BL	label13 + 64

		BLX	label13
		BLX	label13 + 4

		DCI	0
		DCI	0
label13
	|
label11
		DCI	&eafffffe	; B        Code1 / label11
		DCI	&ea000009	; B        0x30 / label11 + 48
		DCI	&ebfffffc	; BL       Code1 / label11
		DCI	&eb00000b	; BL       0x40 / label11 + 64

		DCI	&fafffffa	; BLX      Code1 / label11
		DCI	&fbfffff9	; BLX      0x2 / label11 + 2

label12
		DCI	&eafffffe	; B        0x18
		DCI	&ea000009	; B        0x48
		DCI	&ebfffffc	; BL       0x18
		DCI	&eb00000b	; BL       0x58

		DCI	&fafffffa	; BLX      0x18
		DCI	&fafffffa	; BLX      0x1c

		DCI	&ea000006	; B        0x50
		DCI	&ea000011	; B        0x80
		DCI	&eb000004	; BL       0x50
		DCI	&eb000013	; BL       0x90

		DCI	&fa000002	; BLX      0x50
		DCI	&fa000002	; BLX      0x54

		DCI	0
		DCI	0
label13
	]

		; Absolute area:
		AREA	Code_B_BL_BLX_02, CODE, READONLY
		ORG	&500

		; Branch instructions
	[ :LNOT: REFERENCE
		; Label defined upfront, label at pos 0.
label21
		B	label21
		B	label21 + 48
		BL	label21
		BL	label21 + 64

		BLX	label21
		BLX	label21 + 2

		; Label defined upfront, label at pos non-0.
label22
		B	label22
		B	label22 + 48
		BL	label22
		BL	label22 + 64

		BLX	label22
		BLX	label22 + 4

		; Label defined afterwards.
		B	label23
		B	label23 + 48
		BL	label23
		BL	label23 + 64

		BLX	label23
		BLX	label23 + 4

		DCI	0
		DCI	0
label23
	|
label21
		DCI	&eafffffe	; B        Code2 / label21
		DCI	&ea000009	; B        0x30 / label21 + 48
		DCI	&ebfffffc	; BL       Code2 / label21
		DCI	&eb00000b	; BL       0x40 / label21 + 64

		DCI	&fafffffa	; BLX      Code2 / label21
		DCI	&fbfffff9	; BLX      0x2 / label21 + 2

label22
		DCI	&eafffffe	; B        0x18
		DCI	&ea000009	; B        0x48
		DCI	&ebfffffc	; BL       0x18
		DCI	&eb00000b	; BL       0x58

		DCI	&fafffffa	; BLX      0x18
		DCI	&fafffffa	; BLX      0x1c

		DCI	&ea000006	; B        0x50
		DCI	&ea000011	; B        0x80
		DCI	&eb000004	; BL       0x50
		DCI	&eb000013	; BL       0x90

		DCI	&fa000002	; BLX      0x50
		DCI	&fa000002	; BLX      0x54

		DCI	0
		DCI	0
label23
	]

	; ---------------------------------------------------------------
	; Tests the full branch range for B, BL and BLX <label>.

	AREA Code_RangeTesting, CODE

	[ :LNOT: REFERENCE
	; Generate branches fully exercising the branch target range which you
	; can cover with $bmax bits (and $binit bits granularity).
	MACRO
	GenBranch $base, $binit, $bmax
	LCLA i
	LCLA boffset
	[ {CODESIZE} = 32
boffset	SETA 8
	|
boffset	SETA 4
	]
	LCLA target

	; Positive branch targets:
i	SETA $binit
target	SETA 0
	Inject "$base . + 0x$boffset + 0x$target"
	WHILE i < $bmax - 1
target	SETA target + 1:SHL:i
	Inject "$base . + 0x$boffset + 0x$target"
i	SETA i + 1
	WEND

	; Negative branch targets:
i	SETA $binit
target	SETA - 1:SHL:i
	Inject "$base . + 0x$boffset + 0x$target"
	WHILE i < $bmax - 1
target	SETA target - 1:SHL:i
	Inject "$base . + 0x$boffset + 0x$target"
i	SETA i + 1
	WEND
	MEND
	]

	ARM
	[ :LNOT: REFERENCE
	GenBranch B, 2, 26
	|
	DCI &ea000000
	DCI &ea000001
	DCI &ea000003
	DCI &ea000007
	DCI &ea00000f
	DCI &ea00001f
	DCI &ea00003f
	DCI &ea00007f
	DCI &ea0000ff
	DCI &ea0001ff
	DCI &ea0003ff
	DCI &ea0007ff
	DCI &ea000fff
	DCI &ea001fff
	DCI &ea003fff
	DCI &ea007fff
	DCI &ea00ffff
	DCI &ea01ffff
	DCI &ea03ffff
	DCI &ea07ffff
	DCI &ea0fffff
	DCI &ea1fffff
	DCI &ea3fffff
	DCI &ea7fffff
	DCI &eaffffff
	DCI &eafffffe
	DCI &eafffffc
	DCI &eafffff8
	DCI &eafffff0
	DCI &eaffffe0
	DCI &eaffffc0
	DCI &eaffff80
	DCI &eaffff00
	DCI &eafffe00
	DCI &eafffc00
	DCI &eafff800
	DCI &eafff000
	DCI &eaffe000
	DCI &eaffc000
	DCI &eaff8000
	DCI &eaff0000
	DCI &eafe0000
	DCI &eafc0000
	DCI &eaf80000
	DCI &eaf00000
	DCI &eae00000
	DCI &eac00000
	DCI &ea800000
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch BNE, 1, 9
	|
	DCI.N &d100
	DCI.N &d101
	DCI.N &d103
	DCI.N &d107
	DCI.N &d10f
	DCI.N &d11f
	DCI.N &d13f
	DCI.N &d17f
	DCI.N &d1ff
	DCI.N &d1fe
	DCI.N &d1fc
	DCI.N &d1f8
	DCI.N &d1f0
	DCI.N &d1e0
	DCI.N &d1c0
	DCI.N &d180
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch B, 1, 12
	|
	DCI.N &e000
	DCI.N &e001
	DCI.N &e003
	DCI.N &e007
	DCI.N &e00f
	DCI.N &e01f
	DCI.N &e03f
	DCI.N &e07f
	DCI.N &e0ff
	DCI.N &e1ff
	DCI.N &e3ff
	DCI.N &e7ff
	DCI.N &e7fe
	DCI.N &e7fc
	DCI.N &e7f8
	DCI.N &e7f0
	DCI.N &e7e0
	DCI.N &e7c0
	DCI.N &e780
	DCI.N &e700
	DCI.N &e600
	DCI.N &e400
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch BNE.W, 1, 21
	|
	DCI.W &f0408000
	DCI.W &f0408001
	DCI.W &f0408003
	DCI.W &f0408007
	DCI.W &f040800f
	DCI.W &f040801f
	DCI.W &f040803f
	DCI.W &f040807f
	DCI.W &f04080ff
	DCI.W &f04081ff
	DCI.W &f04083ff
	DCI.W &f04087ff
	DCI.W &f04187ff
	DCI.W &f04387ff
	DCI.W &f04787ff
	DCI.W &f04f87ff
	DCI.W &f05f87ff
	DCI.W &f07f87ff
	DCI.W &f07fa7ff
	DCI.W &f07fafff
	DCI.W &f47fafff
	DCI.W &f47faffe
	DCI.W &f47faffc
	DCI.W &f47faff8
	DCI.W &f47faff0
	DCI.W &f47fafe0
	DCI.W &f47fafc0
	DCI.W &f47faf80
	DCI.W &f47faf00
	DCI.W &f47fae00
	DCI.W &f47fac00
	DCI.W &f47fa800
	DCI.W &f47ea800
	DCI.W &f47ca800
	DCI.W &f478a800
	DCI.W &f470a800
	DCI.W &f460a800
	DCI.W &f440a800
	DCI.W &f4408800
	DCI.W &f4408000
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch B.W, 1, 25
	|
	DCI.W &f000b800
	DCI.W &f000b801
	DCI.W &f000b803
	DCI.W &f000b807
	DCI.W &f000b80f
	DCI.W &f000b81f
	DCI.W &f000b83f
	DCI.W &f000b87f
	DCI.W &f000b8ff
	DCI.W &f000b9ff
	DCI.W &f000bbff
	DCI.W &f000bfff
	DCI.W &f001bfff
	DCI.W &f003bfff
	DCI.W &f007bfff
	DCI.W &f00fbfff
	DCI.W &f01fbfff
	DCI.W &f03fbfff
	DCI.W &f07fbfff
	DCI.W &f0ffbfff
	DCI.W &f1ffbfff
	DCI.W &f3ffbfff
	DCI.W &f3ffb7ff
	DCI.W &f3ff97ff
	DCI.W &f7ffbfff
	DCI.W &f7ffbffe
	DCI.W &f7ffbffc
	DCI.W &f7ffbff8
	DCI.W &f7ffbff0
	DCI.W &f7ffbfe0
	DCI.W &f7ffbfc0
	DCI.W &f7ffbf80
	DCI.W &f7ffbf00
	DCI.W &f7ffbe00
	DCI.W &f7ffbc00
	DCI.W &f7ffb800
	DCI.W &f7feb800
	DCI.W &f7fcb800
	DCI.W &f7f8b800
	DCI.W &f7f0b800
	DCI.W &f7e0b800
	DCI.W &f7c0b800
	DCI.W &f780b800
	DCI.W &f700b800
	DCI.W &f600b800
	DCI.W &f400b800
	DCI.W &f400b000
	DCI.W &f4009000
	]

	ARM
	[ :LNOT: REFERENCE
	GenBranch BL, 2, 26
	|
	DCI &eb000000
	DCI &eb000001
	DCI &eb000003
	DCI &eb000007
	DCI &eb00000f
	DCI &eb00001f
	DCI &eb00003f
	DCI &eb00007f
	DCI &eb0000ff
	DCI &eb0001ff
	DCI &eb0003ff
	DCI &eb0007ff
	DCI &eb000fff
	DCI &eb001fff
	DCI &eb003fff
	DCI &eb007fff
	DCI &eb00ffff
	DCI &eb01ffff
	DCI &eb03ffff
	DCI &eb07ffff
	DCI &eb0fffff
	DCI &eb1fffff
	DCI &eb3fffff
	DCI &eb7fffff
	DCI &ebffffff
	DCI &ebfffffe
	DCI &ebfffffc
	DCI &ebfffff8
	DCI &ebfffff0
	DCI &ebffffe0
	DCI &ebffffc0
	DCI &ebffff80
	DCI &ebffff00
	DCI &ebfffe00
	DCI &ebfffc00
	DCI &ebfff800
	DCI &ebfff000
	DCI &ebffe000
	DCI &ebffc000
	DCI &ebff8000
	DCI &ebff0000
	DCI &ebfe0000
	DCI &ebfc0000
	DCI &ebf80000
	DCI &ebf00000
	DCI &ebe00000
	DCI &ebc00000
	DCI &eb800000
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch BL, 1, 25
	|
	DCI.W &f000f800
	DCI.W &f000f801
	DCI.W &f000f803
	DCI.W &f000f807
	DCI.W &f000f80f
	DCI.W &f000f81f
	DCI.W &f000f83f
	DCI.W &f000f87f
	DCI.W &f000f8ff
	DCI.W &f000f9ff
	DCI.W &f000fbff
	DCI.W &f000ffff
	DCI.W &f001ffff
	DCI.W &f003ffff
	DCI.W &f007ffff
	DCI.W &f00fffff
	DCI.W &f01fffff
	DCI.W &f03fffff
	DCI.W &f07fffff
	DCI.W &f0ffffff
	DCI.W &f1ffffff
	DCI.W &f3ffffff
	DCI.W &f3fff7ff
	DCI.W &f3ffd7ff
	DCI.W &f7ffffff
	DCI.W &f7fffffe
	DCI.W &f7fffffc
	DCI.W &f7fffff8
	DCI.W &f7fffff0
	DCI.W &f7ffffe0
	DCI.W &f7ffffc0
	DCI.W &f7ffff80
	DCI.W &f7ffff00
	DCI.W &f7fffe00
	DCI.W &f7fffc00
	DCI.W &f7fff800
	DCI.W &f7fef800
	DCI.W &f7fcf800
	DCI.W &f7f8f800
	DCI.W &f7f0f800
	DCI.W &f7e0f800
	DCI.W &f7c0f800
	DCI.W &f780f800
	DCI.W &f700f800
	DCI.W &f600f800
	DCI.W &f400f800
	DCI.W &f400f000
	DCI.W &f400d000
	]

	ARM
	[ :LNOT: REFERENCE
	GenBranch BLX, 1, 26
	|
	DCI &fa000000
	DCI &fb000000
	DCI &fb000001
	DCI &fb000003
	DCI &fb000007
	DCI &fb00000f
	DCI &fb00001f
	DCI &fb00003f
	DCI &fb00007f
	DCI &fb0000ff
	DCI &fb0001ff
	DCI &fb0003ff
	DCI &fb0007ff
	DCI &fb000fff
	DCI &fb001fff
	DCI &fb003fff
	DCI &fb007fff
	DCI &fb00ffff
	DCI &fb01ffff
	DCI &fb03ffff
	DCI &fb07ffff
	DCI &fb0fffff
	DCI &fb1fffff
	DCI &fb3fffff
	DCI &fb7fffff
	DCI &fbffffff
	DCI &faffffff
	DCI &fafffffe
	DCI &fafffffc
	DCI &fafffff8
	DCI &fafffff0
	DCI &faffffe0
	DCI &faffffc0
	DCI &faffff80
	DCI &faffff00
	DCI &fafffe00
	DCI &fafffc00
	DCI &fafff800
	DCI &fafff000
	DCI &faffe000
	DCI &faffc000
	DCI &faff8000
	DCI &faff0000
	DCI &fafe0000
	DCI &fafc0000
	DCI &faf80000
	DCI &faf00000
	DCI &fae00000
	DCI &fac00000
	DCI &fa800000
	]

	THUMB
	[ :LNOT: REFERENCE
	GenBranch BLX, 2, 25
	|
	DCI.W &f000e800
	DCI.W &f000e802
	DCI.W &f000e806
	DCI.W &f000e80e
	DCI.W &f000e81e
	DCI.W &f000e83e
	DCI.W &f000e87e
	DCI.W &f000e8fe
	DCI.W &f000e9fe
	DCI.W &f000ebfe
	DCI.W &f000effe
	DCI.W &f001effe
	DCI.W &f003effe
	DCI.W &f007effe
	DCI.W &f00feffe
	DCI.W &f01feffe
	DCI.W &f03feffe
	DCI.W &f07feffe
	DCI.W &f0ffeffe
	DCI.W &f1ffeffe
	DCI.W &f3ffeffe
	DCI.W &f3ffe7fe
	DCI.W &f3ffc7fe
	DCI.W &f7ffeffe
	DCI.W &f7ffeffc
	DCI.W &f7ffeff8
	DCI.W &f7ffeff0
	DCI.W &f7ffefe0
	DCI.W &f7ffefc0
	DCI.W &f7ffef80
	DCI.W &f7ffef00
	DCI.W &f7ffee00
	DCI.W &f7ffec00
	DCI.W &f7ffe800
	DCI.W &f7fee800
	DCI.W &f7fce800
	DCI.W &f7f8e800
	DCI.W &f7f0e800
	DCI.W &f7e0e800
	DCI.W &f7c0e800
	DCI.W &f780e800
	DCI.W &f700e800
	DCI.W &f600e800
	DCI.W &f400e800
	DCI.W &f400e000
	DCI.W &f400c000
	]


	; ---------------------------------------------------------------
	; Specific B testing:

	AREA Code_SpecificB_01, CODE

	THUMB
	[ :LNOT: REFERENCE
	; <base> <cond code>
	; <base> <cond code> ".N"
	; <base> <cond code> ".W"
	MACRO
	Invoke $base
	LCLS	instr
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
	Inject	"$instr . + 4 + 16"
	Inject	"$instr..N . + 4 + 16"
	Inject	"$instr..W . + 4 + 16"
cnt	SETA	cnt + 1
	WEND
	MEND

	Invoke B
	|
	DCI.N &d008	; beq.n	. + 4 + 16
	DCI.N &d008	; beq.n	. + 4 + 16
	DCI.W &f0008008	; beq.w	. + 4 + 16
	DCI.N &d108	; bne.n	. + 4 + 16
	DCI.N &d108	; bne.n	. + 4 + 16
	DCI.W &f0408008	; bne.w	. + 4 + 16
	DCI.N &d208	; bcs.n	. + 4 + 16
	DCI.N &d208	; bcs.n	. + 4 + 16
	DCI.W &f0808008	; bcs.w	. + 4 + 16
	DCI.N &d308	; bcc.n	. + 4 + 16
	DCI.N &d308	; bcc.n	. + 4 + 16
	DCI.W &f0c08008	; bcc.w	. + 4 + 16
	DCI.N &d408	; bmi.n	. + 4 + 16
	DCI.N &d408	; bmi.n	. + 4 + 16
	DCI.W &f1008008	; bmi.w	. + 4 + 16
	DCI.N &d508	; bpl.n	. + 4 + 16
	DCI.N &d508	; bpl.n	. + 4 + 16
	DCI.W &f1408008	; bpl.w	. + 4 + 16
	DCI.N &d608	; bvs.n	. + 4 + 16
	DCI.N &d608	; bvs.n	. + 4 + 16
	DCI.W &f1808008	; bvs.w	. + 4 + 16
	DCI.N &d708	; bvc.n	. + 4 + 16
	DCI.N &d708	; bvc.n	. + 4 + 16
	DCI.W &f1c08008	; bvc.w	. + 4 + 16
	DCI.N &d808	; bhi.n	. + 4 + 16
	DCI.N &d808	; bhi.n	. + 4 + 16
	DCI.W &f2008008	; bhi.w	. + 4 + 16
	DCI.N &d908	; bls.n	. + 4 + 16
	DCI.N &d908	; bls.n	. + 4 + 16
	DCI.W &f2408008	; bls.w	. + 4 + 16
	DCI.N &da08	; bge.n	. + 4 + 16
	DCI.N &da08	; bge.n	. + 4 + 16
	DCI.W &f2808008	; bge.w	. + 4 + 16
	DCI.N &db08	; blt.n	. + 4 + 16
	DCI.N &db08	; blt.n	. + 4 + 16
	DCI.W &f2c08008	; blt.w	. + 4 + 16
	DCI.N &dc08	; bgt.n	. + 4 + 16
	DCI.N &dc08	; bgt.n	. + 4 + 16
	DCI.W &f3008008	; bgt.w	. + 4 + 16
	DCI.N &dd08	; ble.n	. + 4 + 16
	DCI.N &dd08	; ble.n	. + 4 + 16
	DCI.W &f3408008	; ble.w	. + 4 + 16
	DCI.N &e008	; b.n	. + 4 + 16
	DCI.N &e008	; b.n	. + 4 + 16
	DCI.W &f000b808	; b.w	. + 4 + 16
	DCI.N &d208	; bcs.n	. + 4 + 16
	DCI.N &d208	; bcs.n	. + 4 + 16
	DCI.W &f0808008	; bcs.w	. + 4 + 16
	DCI.N &d308	; bcc.n	. + 4 + 16
	DCI.N &d308	; bcc.n	. + 4 + 16
	DCI.W &f0c08008	; bcc.w	. + 4 + 16
	]

	; Branch + condition should be the last instruction in IT block.
	THUMB
	[ :LNOT: REFERENCE
	SWICC 0		; Force implicit IT block
	BCC .
	SWICC 0

	ITT MI		; Explicit IT block
	SWIMI 0
	BMI .
	SWIMI 0

	SWINE 0
	BNE.W .
	SWINE 0
	|
	DCI.N &bf3c	; itt	cc
	DCI.N &df00	; svccc	0
	DCI.N &e7fe	; bcc.n	.
	DCI.N &bf38	; it	cc
	DCI.N &df00	; svccc	0

	DCI.N &bf44	; itt	mi
	DCI.N &df00	; svcmi	0
	DCI.N &e7fe	; bmi.n	.
	DCI.N &bf48	; it	mi
	DCI.N &df00	; svcmi	0

	DCI.N &bf1c	; itt	ne
	DCI.N &df00	; svcne	0
	DCI.W &f7ffbffe	; bne.w	.
	DCI.N &bf18	; it	ne
	DCI.N &df00	; svcne	0
	]

	; Select IT when necessary:
	AREA Code_SpecificB_02, CODE
	THUMB
	[ :LNOT: REFERENCE
	; Forward, condition code, implicit narrow small range:
	BCC . + 4 + 2*(1<<7 - 2)
	BCC . + 4 + 2*(1<<7 - 1)
	BCC . + 4 + 2*(1<<7 + 0)	; Should generate IT + narrow long range branch

	BCC . + 4 + 2*(1<<7 - 2)
	BCC . + 4 + 2*(1<<7 - 1)
	IT CC
	BCC . + 4 + 2*(1<<7 + 0) - 2	; Should generate IT + narrow long range branch

	; Forward, condition code, explicit narrow small range:
	BCC.N . + 4 + 2*(1<<7 - 2)
	BCC.N . + 4 + 2*(1<<7 - 1)
	BCC.N . + 4 + 2*(1<<7 + 0)	; Should generate IT + narrow long range branch

	BCC.N . + 4 + 2*(1<<7 - 2)
	BCC.N . + 4 + 2*(1<<7 - 1)
	IT CC
	BCC.N . + 4 + 2*(1<<7 + 0) - 2	; Should generate IT + narrow long range branch

	; Forward, condition code, implicit narrow long range:
	BCC . + 4 + 2*(1<<10 - 2)
	BCC . + 4 + 2*(1<<10 - 1)
	BCC . + 4 + 2*(1<<10 + 0)	; One would think this should result in
		; a wide branch + short range but it actually generates an IT
		; + narrow long branch which is possible as the branch target
		; gets 2 bytes closer.
	BCC . + 4 + 2*(1<<10 + 1)	; Should generate short wide branch.

	IT CC
	BCC . + 4 + 2*(1<<10 - 2) - 2
	IT CC
	BCC . + 4 + 2*(1<<10 - 1) - 2
	IT CC
	BCC . + 4 + 2*(1<<10 + 0) - 2
	BCC . + 4 + 2*(1<<10 + 1)	; Should generate short wide branch.

	; Forward, condition code, explicit narrow long range:
	BCC.N . + 4 + 2*(1<<10 - 2)
	BCC.N . + 4 + 2*(1<<10 - 1)
	BCC.N . + 4 + 2*(1<<10 + 0)	; One would think this should result in
		; a wide branch + short range but it actually generates an IT
		; + narrow long branch which is possible as the branch target
		; gets 2 bytes closer.
	BCC.W . + 4 + 2*(1<<10 + 1)	; Should generate short wide branch.

	IT CC
	BCC.N . + 4 + 2*(1<<10 - 2) - 2
	IT CC
	BCC.N . + 4 + 2*(1<<10 - 1) - 2
	IT CC
	BCC.N . + 4 + 2*(1<<10 + 0) - 2
	BCC.W . + 4 + 2*(1<<10 + 1)	; Should generate short wide branch.

	; Forward, condition code, implicit wide, small range:
	BCC . + 4 + 2*(1<<19 - 2)
	BCC . + 4 + 2*(1<<19 - 1)
	BCC . + 4 + 2*(1<<19 + 0)	; Should generate IT + wide long range branch
	BCC . + 4 + 2*(1<<19 + 1)	; Should generate IT + wide long range branch

	BCC . + 4 + 2*(1<<19 - 2)
	BCC . + 4 + 2*(1<<19 - 1)
	IT CC
	BCC . + 4 + 2*(1<<19 + 0) - 2
	IT CC
	BCC . + 4 + 2*(1<<19 + 1) - 2

	; Forward, condition code, explicit wide, small range:
	BCC.W . + 4 + 2*(1<<19 - 2)
	BCC.W . + 4 + 2*(1<<19 - 1)
	BCC.W . + 4 + 2*(1<<19 + 0)	; Should generate IT + wide long range branch
	BCC.W . + 4 + 2*(1<<19 + 1)	; Should generate IT + wide long range branch

	BCC.W . + 4 + 2*(1<<19 - 2)
	BCC.W . + 4 + 2*(1<<19 - 1)
	IT CC
	BCC.W . + 4 + 2*(1<<19 + 0) - 2
	IT CC
	BCC.W . + 4 + 2*(1<<19 + 1) - 2

	; Forward, condition code, implicit wide, long range:
	BCC . + 4 + 2*(1<<23 - 2)
	BCC . + 4 + 2*(1<<23 - 1)
	BCC . + 4 + 2*(1<<23 + 0)	; One would think this should result in
		; an error but it actually generates an IT + wide long branch
		; which is possible as the branch target gets 2 bytes closer.
	; BCC . + 4 + 2*(1<<23 + 1)	; Too far...

	IT CC
	BCC . + 4 + 2*(1<<23 - 2) - 2
	IT CC
	BCC . + 4 + 2*(1<<23 - 1) - 2
	IT CC
	BCC . + 4 + 2*(1<<23 + 0) - 2
	; BCC . + 4 + 2*(1<<23 + 1)	; Too far...

	; Forward, condition code, explicit wide, long range:
	BCC.W . + 4 + 2*(1<<23 - 2)
	BCC.W . + 4 + 2*(1<<23 - 1)
	BCC.W . + 4 + 2*(1<<23 + 0)	; One would think this should result in
		; an error but it actually generates an IT + wide long branch
		; which is possible as the branch target gets 2 bytes closer.
	; BCC.W . + 4 + 2*(1<<23 + 1)	; Too far...

	IT CC
	BCC.W . + 4 + 2*(1<<23 - 2) - 2
	IT CC
	BCC.W . + 4 + 2*(1<<23 - 1) - 2
	IT CC
	BCC.W . + 4 + 2*(1<<23 + 0) - 2
	; BCC.W . + 4 + 2*(1<<23 + 1)	; Too far...

	; Forward, no condition code:
	B . + 4 + 2*(0)
	B . + 4 + 2*(1<<10 - 1)
	B . + 4 + 2*(1<<10 + 0)
	B . + 4 + 2*(1<<10 + 1)
	B . + 4 + 2*(1<<23 - 1)

	; Backward, condition code, implicit narrow small range:
	BCC . + 4 - 2*(1<<7 - 1)
	BCC . + 4 - 2*(1<<7 + 0)
	BCC . + 4 - 2*(1<<7 + 1)	; Should generate IT + narrow long range branch

	BCC . + 4 - 2*(1<<7 - 1)
	BCC . + 4 - 2*(1<<7 + 0)
	IT CC
	BCC . + 4 - 2*(1<<7 + 1) - 2	; Narrow long range branch

	; Backward, condition code, explicit narrow small range:
	BCC.N . + 4 - 2*(1<<7 - 1)
	BCC.N . + 4 - 2*(1<<7 + 0)
	BCC.N . + 4 - 2*(1<<7 + 1)	; Should generate IT + narrow long range branch

	BCC.N . + 4 - 2*(1<<7 - 1)
	BCC.N . + 4 - 2*(1<<7 + 0)
	IT CC
	BCC.N . + 4 - 2*(1<<7 + 1) - 2	; Narrow long range branch

	; Backward, condition code, implicit narrow long range:
	BCC . + 4 - 2*(1<<10 - 2)
	BCC . + 4 - 2*(1<<10 - 1)
	BCC . + 4 - 2*(1<<10 + 0)	; Should generate short wide branch as
		; we need to inject an extra IT instruction which pushes the
		; branch target 2 bytes further away.
	BCC . + 4 - 2*(1<<10 + 1)

	IT CC
	BCC . + 4 - 2*(1<<10 - 2) - 2
	IT CC
	BCC . + 4 - 2*(1<<10 - 1) - 2
	BCC . + 4 - 2*(1<<10 + 0)
	BCC . + 4 - 2*(1<<10 + 1)

	; Backward, condition code, explicit narrow long range:
	BCC.N . + 4 - 2*(1<<10 - 2)
	BCC.N . + 4 - 2*(1<<10 - 1)
	BCC.W . + 4 - 2*(1<<10 + 0)	; Should generate short wide branch as
		; we need to inject an extra IT instruction which pushes the
		; branch target 2 bytes further away.
	BCC.W . + 4 - 2*(1<<10 + 1)

	IT CC
	BCC.N . + 4 - 2*(1<<10 - 2) - 2
	IT CC
	BCC.N . + 4 - 2*(1<<10 - 1) - 2
	BCC.W . + 4 - 2*(1<<10 + 0)
	BCC.W . + 4 - 2*(1<<10 + 1)

	; Backward, condition code, implicit wide small range:
	BCC . + 4 - 2*(1<<19 - 1)
	BCC . + 4 - 2*(1<<19 + 0)
	BCC . + 4 - 2*(1<<19 + 1)	; Should generate IT + wide long range branch

	BCC . + 4 - 2*(1<<19 - 1)
	BCC . + 4 - 2*(1<<19 + 0)
	IT CC
	BCC . + 4 - 2*(1<<19 + 1) - 2	; Wide long range branch

	; Backward, condition code, explicit wide small range:
	BCC.W . + 4 - 2*(1<<19 - 1)
	BCC.W . + 4 - 2*(1<<19 + 0)
	BCC.W . + 4 - 2*(1<<19 + 1)	; Should generate IT + wide long range branch

	BCC.W . + 4 - 2*(1<<19 - 1)
	BCC.W . + 4 - 2*(1<<19 + 0)
	IT CC
	BCC.W . + 4 - 2*(1<<19 + 1) - 2	; Wide long range branch

	; Backward, condition code, implicit wide long range:
	BCC . + 4 - 2*(1<<23 - 3)
	BCC . + 4 - 2*(1<<23 - 2)
	BCC . + 4 - 2*(1<<23 - 1)
	; BCC . + 4 - 2*(1<<23 + 0)	; Too far (as an IT needed)...

	IT CC
	BCC . + 4 - 2*(1<<23 - 3) - 2
	IT CC
	BCC . + 4 - 2*(1<<23 - 2) - 2
	IT CC
	BCC . + 4 - 2*(1<<23 - 1) - 2
	; IT CC
	; BCC . + 4 - 2*(1<<23 + 0) - 2	; Too far...

	; Backward, condition code, explicit wide long range:
	BCC.W . + 4 - 2*(1<<23 - 3)
	BCC.W . + 4 - 2*(1<<23 - 2)
	BCC.W . + 4 - 2*(1<<23 - 1)
	; BCC.W . + 4 - 2*(1<<23 + 0)	; Too far (as an IT needed)...

	IT CC
	BCC.W . + 4 - 2*(1<<23 - 3) - 2
	IT CC
	BCC.W . + 4 - 2*(1<<23 - 2) - 2
	IT CC
	BCC.W . + 4 - 2*(1<<23 - 1) - 2
	; IT CC
	; BCC.W . + 4 - 2*(1<<23 + 0) - 2	; Too far...

	; Backward, no condition code:
	B . + 4 - 2*(1)
	B . + 4 - 2*(1<<10 - 1)
	B . + 4 - 2*(1<<10 + 0)
	B . + 4 - 2*(1<<10 + 1)
	B . + 4 - 2*(1<<23 - 1)
	B . + 4 - 2*(1<<23 + 0)

	|

	; Forward, condition code, implicit narrow small range:
	DCI.N &d37e	; bcc.n	. + 4 + 2*(1<<7 - 2)
	DCI.N &d37f	; bcc.n	. + 4 + 2*(1<<7 - 1)
	DCI.N &bf38	; it	cc
	DCI.N &e07f	; bcc.n	. + 4 + 2*(1<<7 + 0) - 2

	DCI.N &d37e	; bcc.n	. + 4 + 2*(1<<7 - 2)
	DCI.N &d37f	; bcc.n	. + 4 + 2*(1<<7 - 1)
	DCI.N &bf38	; it	cc
	DCI.N &e07f	; bcc.n	. + 4 + 2*(1<<7 + 0) - 2

	; Forward, condition code, explicit narrow small range:
	DCI.N &d37e	; bcc.n	. + 4 + 2*(1<<7 - 2)
	DCI.N &d37f	; bcc.n	. + 4 + 2*(1<<7 - 1)
	DCI.N &bf38	; it	cc
	DCI.N &e07f	; bcc.n	. + 4 + 2*(1<<7 + 0) - 2

	DCI.N &d37e	; bcc.n	. + 4 + 2*(1<<7 - 2)
	DCI.N &d37f	; bcc.n	. + 4 + 2*(1<<7 - 1)
	DCI.N &bf38	; it	cc
	DCI.N &e07f	; bcc.n	. + 4 + 2*(1<<7 + 0) - 2

	; Forward, condition code, implicit narrow long range:
	DCI.N &bf38	; it	cc
	DCI.N &e3fd	; bcc.n	. + 4 + 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3fe	; bcc.n	. + 4 + 2*(1<<10 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3ff	; bcc.n	. + 4 + 2*(1<<10 + 0) - 2
	DCI.W &f0c08401	; bcc.w	. + 4 + 2*(1<<10 + 1)

	DCI.N &bf38	; it	cc
	DCI.N &e3fd	; bcc.n	. + 4 + 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3fe	; bcc.n	. + 4 + 2*(1<<10 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3ff	; bcc.n	. + 4 + 2*(1<<10 + 0) - 2
	DCI.W &f0c08401	; bcc.w	. + 4 + 2*(1<<10 + 1)

	; Forward, condition code, explicit narrow long range:
	DCI.N &bf38	; it	cc
	DCI.N &e3fd	; bcc.n	. + 4 + 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3fe	; bcc.n	. + 4 + 2*(1<<10 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3ff	; bcc.n	. + 4 + 2*(1<<10 + 0) - 2
	DCI.W &f0c08401	; bcc.w	. + 4 + 2*(1<<10 + 1)

	DCI.N &bf38	; it	cc
	DCI.N &e3fd	; bcc.n	. + 4 + 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3fe	; bcc.n	. + 4 + 2*(1<<10 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e3ff	; bcc.n	. + 4 + 2*(1<<10 + 0) - 2
	DCI.W &f0c08401	; bcc.w	. + 4 + 2*(1<<10 + 1)

	; Forward, condition code, implicit wide, small range:
	DCI.W &f0ffaffe	; bcc.w	. + 4 + 2*(1<<19 - 2)
	DCI.W &f0ffafff	; bcc.w	. + 4 + 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f0ffbfff	; bcc.w	. + 4 + 2*(1<<19 + 0) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f100b800	; bcc.w	. + 4 + 2*(1<<19 + 1) - 2

	DCI.W &f0ffaffe	; bcc.w	. + 4 + 2*(1<<19 - 2)
	DCI.W &f0ffafff	; bcc.w	. + 4 + 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f0ffbfff	; bcc.w	. + 4 + 2*(1<<19 + 0) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f100b800	; bcc.w	. + 4 + 2*(1<<19 + 1) - 2

	; Forward, condition code, explicit wide, small range:
	DCI.W &f0ffaffe	; bcc.w	. + 4 + 2*(1<<19 - 2)
	DCI.W &f0ffafff	; bcc.w	. + 4 + 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f0ffbfff	; bcc.w	. + 4 + 2*(1<<19 + 0) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f100b800	; bcc.w	. + 4 + 2*(1<<19 + 1) - 2

	DCI.W &f0ffaffe	; bcc.w	. + 4 + 2*(1<<19 - 2)
	DCI.W &f0ffafff	; bcc.w	. + 4 + 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f0ffbfff	; bcc.w	. + 4 + 2*(1<<19 + 0) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f100b800	; bcc.w	. + 4 + 2*(1<<19 + 1) - 2

	; Forward, condition code, implicit wide, long range:
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fd	; bcc.w	. + 4 + 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fe	; bcc.w	. + 4 + 2*(1<<23 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97ff	; bcc.w	. + 4 + 2*(1<<23 + 0) - 2

	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fd	; bcc.w	. + 4 + 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fe	; bcc.w	. + 4 + 2*(1<<23 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97ff	; bcc.w	. + 4 + 2*(1<<23 + 0) - 2

	; Forward, condition code, explicit wide, long range:
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fd	; bcc.w	. + 4 + 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fe	; bcc.w	. + 4 + 2*(1<<23 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97ff	; bcc.w	. + 4 + 2*(1<<23 + 0) - 2

	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fd	; bcc.w	. + 4 + 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97fe	; bcc.w	. + 4 + 2*(1<<23 - 1) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f3ff97ff	; bcc.w	. + 4 + 2*(1<<23 + 0) - 2

	; Forward, no condition code:
	DCI.N &e000	; b.n	. + 4 + 2*(0)
	DCI.N &e3ff	; b.n	. + 4 + 2*(1<<10 - 1)
	DCI.W &f000bc00	; b.w	. + 4 + 2*(1<<10 + 0)
	DCI.W &f000bc01	; b.w	. + 4 + 2*(1<<10 + 1)
	DCI.W &f3ff97ff	; b.w	. + 4 + 2*(1<<23 - 1)

	; Backward, condition code, implicit narrow small range:
	DCI.N &d381	; bcc.n	. + 4 - 2*(1<<7 - 1)
	DCI.N &d380	; bcc.n	. + 4 - 2*(1<<7 + 0)
	DCI.N &bf38	; it	cc
	DCI.N &e77e	; bcc.n	. + 4 - 2*(1<<7 + 1) - 2

	DCI.N &d381	; bcc.n	. + 4 - 2*(1<<7 - 1)
	DCI.N &d380	; bcc.n	. + 4 - 2*(1<<7 + 0)
	DCI.N &bf38	; it	cc
	DCI.N &e77e	; bcc.n	. + 4 - 2*(1<<7 + 1) - 2

	; Backward, condition code, explicit narrow small range:
	DCI.N &d381	; bcc.n	. + 4 - 2*(1<<7 - 1)
	DCI.N &d380	; bcc.n	. + 4 - 2*(1<<7 + 0)
	DCI.N &bf38	; it	cc
	DCI.N &e77e	; bcc.n	. + 4 - 2*(1<<7 + 1) - 2

	DCI.N &d381	; bcc.n	. + 4 - 2*(1<<7 - 1)
	DCI.N &d380	; bcc.n	. + 4 - 2*(1<<7 + 0)
	DCI.N &bf38	; it	cc
	DCI.N &e77e	; bcc.n	. + 4 - 2*(1<<7 + 1) - 2

	; Backward, condition code, implicit narrow long range:
	DCI.N &bf38	; it	cc
	DCI.N &e401	; bcc.n	. + 4 - 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e400	; bcc.n	. + 4 - 2*(1<<10 - 1) - 2
	DCI.W &f4ffac00	; bcc.w	. + 4 - 2*(1<<10 + 0)
	DCI.W &f4ffabff	; bcc.w	. + 4 - 2*(1<<10 + 1)

	DCI.N &bf38	; it	cc
	DCI.N &e401	; bcc.n	. + 4 - 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e400	; bcc.n	. + 4 - 2*(1<<10 - 1) - 2
	DCI.W &f4ffac00	; bcc.w	. + 4 - 2*(1<<10 + 0)
	DCI.W &f4ffabff	; bcc.w	. + 4 - 2*(1<<10 + 1)

	; Backward, condition code, explicit narrow long range:
	DCI.N &bf38	; it	cc
	DCI.N &e401	; bcc.n	. + 4 - 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e400	; bcc.n	. + 4 - 2*(1<<10 - 1) - 2
	DCI.W &f4ffac00	; bcc.w	. + 4 - 2*(1<<10 + 0)
	DCI.W &f4ffabff	; bcc.w	. + 4 - 2*(1<<10 + 1)

	DCI.N &bf38	; it	cc
	DCI.N &e401	; bcc.n	. + 4 - 2*(1<<10 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.N &e400	; bcc.n	. + 4 - 2*(1<<10 - 1) - 2
	DCI.W &f4ffac00	; bcc.w	. + 4 - 2*(1<<10 + 0)
	DCI.W &f4ffabff	; bcc.w	. + 4 - 2*(1<<10 + 1)

	; Backward, condition code, implicit wide small range:
	DCI.W &f4c08001	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.W &f4c08000	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f6ffbffe	; bcc.w	. + 4 - 2*(1<<19 - 1) - 2

	DCI.W &f4c08001	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.W &f4c08000	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f6ffbffe	; bcc.w	. + 4 - 2*(1<<19 - 1) - 2

	; Backward, condition code, explicit wide small range:
	DCI.W &f4c08001	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.W &f4c08000	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f6ffbffe	; bcc.w	. + 4 - 2*(1<<19 - 1) - 2

	DCI.W &f4c08001	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.W &f4c08000	; bcc.w	. + 4 - 2*(1<<19 - 1)
	DCI.N &bf38	; it	cc
	DCI.W &f6ffbffe	; bcc.w	. + 4 - 2*(1<<19 - 1) - 2

	; Backward, condition code, implicit wide long range:
	DCI.N &bf38	; it	cc
	DCI.W &f4009002	; bcc.w	. + 4 - 2*(1<<23 - 3) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009001	; bcc.w	. + 4 - 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009000	; bcc.w	. + 4 - 2*(1<<23 - 1) - 2

	DCI.N &bf38	; it	cc
	DCI.W &f4009002	; bcc.w	. + 4 - 2*(1<<23 - 3) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009001	; bcc.w	. + 4 - 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009000	; bcc.w	. + 4 - 2*(1<<23 - 1) - 2

	; Backward, condition code, explicit wide long range:
	DCI.N &bf38	; it	cc
	DCI.W &f4009002	; bcc.w	. + 4 - 2*(1<<23 - 3) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009001	; bcc.w	. + 4 - 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009000	; bcc.w	. + 4 - 2*(1<<23 - 1) - 2

	DCI.N &bf38	; it	cc
	DCI.W &f4009002	; bcc.w	. + 4 - 2*(1<<23 - 3) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009001	; bcc.w	. + 4 - 2*(1<<23 - 2) - 2
	DCI.N &bf38	; it	cc
	DCI.W &f4009000	; bcc.w	. + 4 - 2*(1<<23 - 1) - 2

	; Backward, no condition code:
	DCI.N &e7ff	; b.n	. + 4 - 2*(1)
	DCI.N &e401	; b.n	. + 4 - 2*(1<<10 - 1)
	DCI.N &e400	; b.n	. + 4 - 2*(1<<10 + 0)
	DCI.W &f7ffbbff	; b.w	. + 4 - 2*(1<<10 + 1)
	DCI.W &f4009001	; b.w	. + 4 - 2*(1<<23 - 1)
	DCI.W &f4009000	; b.w	. + 4 - 2*(1<<23 + 0)
	]

	; Labels which do not resolve during pass 1 result in narrow
	; (or wide when explicit .W qualifier is used) long range branches.
	AREA Code_SpecificB_03, CODE
	THUMB
	[ :LNOT: REFERENCE
	; Forward:
	BCC bt101
bt101	* . + 4 + 2*(0) - 2

	BCC.N bt151
bt151	* . + 4 + 2*(0) - 2

	BCC bt201
bt201	* . + 4 + 2*(1<<7 - 2) - 2
	BCC bt202
bt202	* . + 4 + 2*(1<<7 - 1) - 2
	BCC bt203
bt203	* . + 4 + 2*(1<<7 + 0) - 2
	BCC bt204
bt204	* . + 4 + 2*(1<<7 + 1) - 2

	BCC.N bt251
bt251	* . + 4 + 2*(1<<7 - 2) - 2
	BCC.N bt252
bt252	* . + 4 + 2*(1<<7 - 1) - 2
	BCC.N bt253
bt253	* . + 4 + 2*(1<<7 + 0) - 2
	BCC.N bt254
bt254	* . + 4 + 2*(1<<7 + 1) - 2

	BCC bt301
bt301	* . + 4 + 2*(1<<10 - 2) - 2
	BCC bt302
bt302	* . + 4 + 2*(1<<10 - 1) - 2

	BCC.N bt351
bt351	* . + 4 + 2*(1<<10 - 2) - 2
	BCC.N bt352
bt352	* . + 4 + 2*(1<<10 - 1) - 2
	BCC.W bt353			; We need to specify wide.
bt353	* . + 4 + 2*(1<<10 + 0) - 4
	BCC.W bt354
bt354	* . + 4 + 2*(1<<10 + 1) - 4
	BCC.W bt355
bt355	* . + 4 + 2*(1<<10 + 2) - 4

	BCC.W bt451
bt451	* . + 4 + 2*(1<<23 - 2) - 4
	BCC.W bt452
bt452	* . + 4 + 2*(1<<23 - 1) - 4

	; Backward:
	BCC bt501
bt501	* . + 4 - 2*(1) - 2

	BCC.N bt551
bt551	* . + 4 - 2*(1) - 2

	BCC bt601
bt601	* . + 4 - 2*(1<<7 - 2) - 2
	BCC bt602
bt602	* . + 4 - 2*(1<<7 - 1) - 2
	BCC bt603
bt603	* . + 4 - 2*(1<<7 + 0) - 2
	BCC bt604
bt604	* . + 4 - 2*(1<<7 + 1) - 2

	BCC.N bt651
bt651	* . + 4 - 2*(1<<7 - 2) - 2
	BCC.N bt652
bt652	* . + 4 - 2*(1<<7 - 1) - 2
	BCC.N bt653
bt653	* . + 4 - 2*(1<<7 + 0) - 2
	BCC.N bt654
bt654	* . + 4 - 2*(1<<7 + 1) - 2

	BCC bt701
bt701	* . + 4 - 2*(1<<10 - 2) - 2
	BCC bt702
bt702	* . + 4 - 2*(1<<10 - 1) - 2

	BCC.N bt751
bt751	* . + 4 - 2*(1<<10 - 2) - 2
	BCC.N bt752
bt752	* . + 4 - 2*(1<<10 - 1) - 2
	BCC.N bt753
bt753	* . + 4 - 2*(1<<10 + 0) - 2
	BCC.W bt754			; We need to specify wide.
bt754	* . + 4 - 2*(1<<10 + 1) - 4
	BCC.W bt755
bt755	* . + 4 - 2*(1<<10 + 2) - 4

	BCC.W bt851
bt851	* . + 4 - 2*(1<<23 - 2) - 4
	BCC.W bt852
bt852	* . + 4 - 2*(1<<23 - 1) - 4

	; Forward:
	B bu101
bu101	* . + 4 + 2*(0) - 2

	B.N bu151
bu151	* . + 4 + 2*(0) - 2

	B bu201
bu201	* . + 4 + 2*(1<<7 - 2) - 2
	B bu202
bu202	* . + 4 + 2*(1<<7 - 1) - 2
	B bu203
bu203	* . + 4 + 2*(1<<7 + 0) - 2
	B bu204
bu204	* . + 4 + 2*(1<<7 + 1) - 2

	B.N bu251
bu251	* . + 4 + 2*(1<<7 - 2) - 2
	B.N bu252
bu252	* . + 4 + 2*(1<<7 - 1) - 2
	B.N bu253
bu253	* . + 4 + 2*(1<<7 + 0) - 2
	B.N bu254
bu254	* . + 4 + 2*(1<<7 + 1) - 2

	B bu301
bu301	* . + 4 + 2*(1<<10 - 2) - 2
	B bu302
bu302	* . + 4 + 2*(1<<10 - 1) - 2

	B.N bu351
bu351	* . + 4 + 2*(1<<10 - 2) - 2
	B.N bu352
bu352	* . + 4 + 2*(1<<10 - 1) - 2
	B.W bu353			; We need to specify wide.
bu353	* . + 4 + 2*(1<<10 + 0) - 4
	B.W bu354
bu354	* . + 4 + 2*(1<<10 + 1) - 4
	B.W bu355
bu355	* . + 4 + 2*(1<<10 + 2) - 4

	B.W bu451
bu451	* . + 4 + 2*(1<<23 - 2) - 4
	B.W bu452
bu452	* . + 4 + 2*(1<<23 - 1) - 4

	; Backward:
	B bu501
bu501	* . + 4 - 2*(1) - 2

	B.N bu551
bu551	* . + 4 - 2*(1) - 2

	B bu601
bu601	* . + 4 - 2*(1<<7 - 2) - 2
	B bu602
bu602	* . + 4 - 2*(1<<7 - 1) - 2
	B bu603
bu603	* . + 4 - 2*(1<<7 + 0) - 2
	B bu604
bu604	* . + 4 - 2*(1<<7 + 1) - 2

	B.N bu651
bu651	* . + 4 - 2*(1<<7 - 2) - 2
	B.N bu652
bu652	* . + 4 - 2*(1<<7 - 1) - 2
	B.N bu653
bu653	* . + 4 - 2*(1<<7 + 0) - 2
	B.N bu654
bu654	* . + 4 - 2*(1<<7 + 1) - 2

	B bu701
bu701	* . + 4 - 2*(1<<10 - 2) - 2
	B bu702
bu702	* . + 4 - 2*(1<<10 - 1) - 2

	B.N bu751
bu751	* . + 4 - 2*(1<<10 - 2) - 2
	B.N bu752
bu752	* . + 4 - 2*(1<<10 - 1) - 2
	B.N bu753
bu753	* . + 4 - 2*(1<<10 + 0) - 2
	B.W bu754			; We need to specify wide.
bu754	* . + 4 - 2*(1<<10 + 1) - 4
	B.W bu755
bu755	* . + 4 - 2*(1<<10 + 2) - 4

	B.W bu851
bu851	* . + 4 - 2*(1<<23 - 2) - 4
	B.W bu852
bu852	* . + 4 - 2*(1<<23 - 1) - 4

	|

	; Forward:
	IT CC
	BCC.N . + 4 + 2*(0)

	IT CC
	BCC.N . + 4 + 2*(0)

	IT CC
	BCC.N . + 4 + 2*(1<<7 - 2)
	IT CC
	BCC.N . + 4 + 2*(1<<7 - 1)
	IT CC
	BCC.N . + 4 + 2*(1<<7 + 0)
	IT CC
	BCC.N . + 4 + 2*(1<<7 + 1)

	IT CC
	BCC.N . + 4 + 2*(1<<7 - 2)
	IT CC
	BCC.N . + 4 + 2*(1<<7 - 1)
	IT CC
	BCC.N . + 4 + 2*(1<<7 + 0)
	IT CC
	BCC.N . + 4 + 2*(1<<7 + 1)

	IT CC
	BCC.N . + 4 + 2*(1<<10 - 2)
	IT CC
	BCC.N . + 4 + 2*(1<<10 - 1)

	IT CC
	BCC.N . + 4 + 2*(1<<10 - 2)
	IT CC
	BCC.N . + 4 + 2*(1<<10 - 1)
	IT CC
	BCC.W . + 4 + 2*(1<<10 + 0)
	IT CC
	BCC.W . + 4 + 2*(1<<10 + 1)
	IT CC
	BCC.W . + 4 + 2*(1<<10 + 2)

	IT CC
	BCC.W . + 4 + 2*(1<<23 - 2)
	IT CC
	BCC.W . + 4 + 2*(1<<23 - 1)

	; Backward:
	IT CC
	BCC.N . + 4 - 2*(1)

	IT CC
	BCC.N . + 4 - 2*(1)

	IT CC
	BCC.N . + 4 - 2*(1<<7 - 2)
	IT CC
	BCC.N . + 4 - 2*(1<<7 - 1)
	IT CC
	BCC.N . + 4 - 2*(1<<7 + 0)
	IT CC
	BCC.N . + 4 - 2*(1<<7 + 1)

	IT CC
	BCC.N . + 4 - 2*(1<<7 - 2)
	IT CC
	BCC.N . + 4 - 2*(1<<7 - 1)
	IT CC
	BCC.N . + 4 - 2*(1<<7 + 0)
	IT CC
	BCC.N . + 4 - 2*(1<<7 + 1)

	IT CC
	BCC.N . + 4 - 2*(1<<10 - 2)
	IT CC
	BCC.N . + 4 - 2*(1<<10 - 1)

	IT CC
	BCC.N . + 4 - 2*(1<<10 - 2)
	IT CC
	BCC.N . + 4 - 2*(1<<10 - 1)
	IT CC
	BCC.N . + 4 - 2*(1<<10 + 0)
	IT CC
	BCC.W . + 4 - 2*(1<<10 + 1)
	IT CC
	BCC.W . + 4 - 2*(1<<10 + 2)

	IT CC
	BCC.W . + 4 - 2*(1<<23 - 2)
	IT CC
	BCC.W . + 4 - 2*(1<<23 - 1)

	; Forward:
	B.N . + 4 + 2*(0)

	B.N . + 4 + 2*(0)

	B.N . + 4 + 2*(1<<7 - 2)
	B.N . + 4 + 2*(1<<7 - 1)
	B.N . + 4 + 2*(1<<7 + 0)
	B.N . + 4 + 2*(1<<7 + 1)

	B.N . + 4 + 2*(1<<7 - 2)
	B.N . + 4 + 2*(1<<7 - 1)
	B.N . + 4 + 2*(1<<7 + 0)
	B.N . + 4 + 2*(1<<7 + 1)

	B.N . + 4 + 2*(1<<10 - 2)
	B.N . + 4 + 2*(1<<10 - 1)

	B.N . + 4 + 2*(1<<10 - 2)
	B.N . + 4 + 2*(1<<10 - 1)
	B.W . + 4 + 2*(1<<10 + 0)
	B.W . + 4 + 2*(1<<10 + 1)
	B.W . + 4 + 2*(1<<10 + 2)

	B.W . + 4 + 2*(1<<23 - 2)
	B.W . + 4 + 2*(1<<23 - 1)

	; Backward:
	B.N . + 4 - 2*(1)

	B.N . + 4 - 2*(1)

	B.N . + 4 - 2*(1<<7 - 2)
	B.N . + 4 - 2*(1<<7 - 1)
	B.N . + 4 - 2*(1<<7 + 0)
	B.N . + 4 - 2*(1<<7 + 1)

	B.N . + 4 - 2*(1<<7 - 2)
	B.N . + 4 - 2*(1<<7 - 1)
	B.N . + 4 - 2*(1<<7 + 0)
	B.N . + 4 - 2*(1<<7 + 1)

	B.N . + 4 - 2*(1<<10 - 2)
	B.N . + 4 - 2*(1<<10 - 1)

	B.N . + 4 - 2*(1<<10 - 2)
	B.N . + 4 - 2*(1<<10 - 1)
	B.N . + 4 - 2*(1<<10 + 0)
	B.W . + 4 - 2*(1<<10 + 1)
	B.W . + 4 - 2*(1<<10 + 2)

	B.W . + 4 - 2*(1<<23 - 2)
	B.W . + 4 - 2*(1<<23 - 1)
	]

	; Test on IT block <cc> mismatch:
	AREA Code_SpecificB_04, CODE
	[ :LNOT: REFERENCE
	SWINE 0
	BCC . + 4

	SWINE 0
	BCC c21_01
c21_01	* . + 4 - 2
	|
	DCI.N &bf18	; it	ne
	DCI.N &df00	; svcne	0
	DCI.N &d300	; bcc.n	. + 4

	DCI.N &bf18	; it	ne
	DCI.N &df00	; svcne	0
	DCI.N &bf38	; it	cc
	DCI.N &e000	; bcc.n	. + 4
	]

	; ---------------------------------------------------------------
	; Specific BLX tests:

	AREA Code_SpecificBLX_01, CODE
	[ :LNOT: REFERENCE

	; THUMB BLX calling ARM code.
	ARM
	nop
lblb2	nop
lblb4	nop

	THUMB
	blx lblb2
	nop.n
	blx lblb2
	nop.n
	blx lblb2

	blx lblb4
	nop.n
	blx lblb4
	nop.n
	blx lblb4

	blx lblf2
	nop.n
	blx lblf2
	nop.n
	blx lblf2

	blx lblf4
	nop.n
	blx lblf4
	nop.n
	blx lblf4

	ARM
lblf2	nop
lblf4	nop

	; ARM BLX calling THUMB code
	THUMB
lblb8	nop.n
lblb10	nop.n

	ARM
	blx lblb8
	blx lblb10
	blx lblf8
	blx lblf10

	THUMB
lblf8	nop.n
lblf10	nop.n
	|

	; THUMB BLX calling ARM code.
	ARM
	DCI &e320f000	; nop	{0}
lblb2	DCI &e320f000	; nop	{0}
lblb4	DCI &e320f000	; nop	{0}

	THUMB
	DCI.W &f7ffeffa	; blx	4 <lblb2>
	DCI.N &bf00	; nop
	DCI.W &f7ffeff8	; blx	4 <lblb2>
	DCI.N &bf00	; nop
	DCI.W &f7ffeff4	; blx	4 <lblb2>
	DCI.W &f7ffeff4	; blx	8 <lblb4>
	DCI.N &bf00	; nop
	DCI.W &f7ffeff2	; blx	8 <lblb4>
	DCI.N &bf00	; nop
	DCI.W &f7ffefee	; blx	8 <lblb4>
	DCI.W &f000e80e	; blx	4c <lblf2>
	DCI.N &bf00	; nop
	DCI.W &f000e80c	; blx	4c <lblf2>
	DCI.N &bf00	; nop
	DCI.W &f000e808	; blx	4c <lblf2>
	DCI.W &f000e808	; blx	50 <lblf4>
	DCI.N &bf00	; nop
	DCI.W &f000e806	; blx	50 <lblf4>
	DCI.N &bf00	; nop
	DCI.W &f000e802	; blx	50 <lblf4>

	ARM
lblf2	DCI &e320f000	; nop	{0}
lblf4	DCI &e320f000	; nop	{0}

	; ARM BLX calling THUMB code
	THUMB
lblb8	DCI.N &bf00	; nop
lblb10	DCI.N &bf00	; nop

	ARM
	DCI.W &fafffffd	; blx	54 <lblb8>
	DCI.W &fbfffffc	; blx	56 <lblb10>
	DCI.W &fa000000	; blx	68 <lblf8>
	DCI.W &fbffffff	; blx	6a <lblf10>

	THUMB
lblf8	DCI.N &bf00	; nop
lblf10	DCI.N &bf00	; nop
	]

	; Ensure implicit IT block gets terminated.
	AREA Code_SpecificBLX_02, CODE
	[ :LNOT:REFERENCE
	THUMB
	blxcc %f10
	dbgcc 5
	ARM
10
	|
	DCI.N &bf38	; it	cc
	DCI.W &f000e804	; blxcc c <Code2+0xc>
	DCI.N &bf38	; it	cc
	DCI.W &f3af80f5	; dbgcc	#5
	]

	END
