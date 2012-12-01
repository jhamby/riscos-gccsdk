; Tests DCFS, DCFH, DCFHU.
; RUNOPT: -FPU=VFPv3-SP_FP16

	AREA	Data, DATA

	GBLL	TestSF
	GBLL	TestHF
	GBLL	DoOut
TestSF	SETL	{TRUE}
TestHF	SETL	{TRUE}

	[ :LNOT: REFERENCE
	MACRO
	DataType $dir

	; Zero:
	$dir	+0.
	$dir	-0.

	; Single-precision subnormal values:
	$dir	0x0.8p-126
	$dir	0x0.fp-126
	$dir	0x0.ffp-126
	$dir	0x0.ffffp-126
	$dir	0x0.fffffe0p-126
	$dir	0x0.fffffe8p-126
	$dir	0x0.ffffff0p-126
	$dir	0x0.ffffff8p-126
	$dir	0x1.0000000p-126	; Single-precision normalized.

	; Half-precision subnormal values:
	; Minimum:
	$dir	0x0.6p-24
	$dir	0x0.8p-24
	$dir	0x0.ap-24
	$dir	0x0.cp-24
	$dir	0x0.ep-24
	$dir	0x1.0p-24	; Minimum half-precision denormalized value.
	$dir	0x1.6p-24
	$dir	0x1.8p-24
	$dir	0x1.ap-24
	$dir	0x1.cp-24
	$dir	0x1.ep-24
	$dir	0x2.0p-24
	$dir	0x2.6p-24
	$dir	0x2.8p-24
	$dir	0x2.ap-24
	; Mid:
	$dir	0x1.00p-18
	$dir	0x1.01p-18
	$dir	0x1.02p-18
	$dir	0x1.03p-18
	$dir	0x1.04p-18
	$dir	0x1.05p-18
	$dir	0x1.06p-18
	$dir	0x1.07p-18
	; Maximum:
	$dir	0x0.8p-14
	$dir	0x0.ap-14
	$dir	0x0.cp-14

	$dir	0x0.ff0p-14
	$dir	0x0.ff1p-14
	$dir	0x0.ff2p-14
	$dir	0x0.ff3p-14
	$dir	0x0.ff4p-14
	$dir	0x0.ff5p-14
	$dir	0x0.ff6p-14
	$dir	0x0.ff7p-14
	$dir	0x0.ff8p-14
	$dir	0x0.ff9p-14
	$dir	0x0.ffap-14
	$dir	0x0.ffbp-14
	$dir	0x0.ffcp-14
	$dir	0x0.ffdp-14
	$dir	0x0.ffep-14
	$dir	0x0.fffp-14
	$dir	0x1.0p-14	; vvv Half-precision normalized.
	$dir	0x1.4p-14
	$dir	0x1.8p-14
	$dir	0x1.cp-14
	$dir	0x1.0p-13	; ^^^ Half-precision normalized.

	; Normalized values:
	$dir	0x1.ffc0p5
	$dir	0x1.ffc8p5
	$dir	0x1.ffd0p5
	$dir	0x1.ffd8p5
	$dir	0x1.ffe0p5
	$dir	0x1.ffe8p5
	$dir	0x1.fff0p5
	$dir	0x1.fff8p5
	$dir	0x1.000p6

	; Half-precision maximum values:
	; - IEEE:
	$dir	0x1.ffc0p15
	$dir	0x1.ffc8p15
	$dir	0x1.ffd0p15
	$dir	0x1.ffd8p15
	$dir	0x1.ffe0p15
	$dir	0x1.ffe8p15
	$dir	0x1.fff0p15
	$dir	0x1.fff8p15
	$dir	0x1.0000p16
	; - ARM alternative:
	$dir	0x1.ffc0p16
	$dir	0x1.ffc8p16
	$dir	0x1.ffd0p16
	$dir	0x1.ffd8p16
	$dir	0x1.ffe0p16
	$dir	0x1.ffe8p16
	$dir	0x1.fff0p16
	$dir	0x1.fff8p16
	$dir	0x1.0000p17

	; +/-Inf:
	$dir	0f_7f800000	; +Inf
	$dir	0f_ff800000	; -Inf

	; sNan:
	$dir	0f_7f880000	;  sNaN(0x80000)
	$dir	0f_7f808000	;  sNaN(0x08000)
	$dir	0f_ff880000	; -sNaN(0x80000)
	$dir	0f_ff808000	; -sNaN(0x08000)

	; qNan:
	$dir	0f_7fc80000	;  qNaN(0x80000)
	$dir	0f_7fc08000	;  qNaN(0x08000)
	$dir	0f_ffc80000	; -qNaN(0x80000)
	$dir	0f_ffc08000	; -qNaN(0x08000)

	MEND

	[ TestSF
	DataType DCFS
	]
	[ TestHF
	DataType DCFH
	]

	|

	[ TestSF
	; *** OUTPUT SINGLE-PRECISION:
	; Zero
	DCFS	0f_00000000	; +0.
	DCFS	0f_80000000	; -0.

	; Single-precision subnormal values:
	DCFS	0f_00400000	; 0x0.8p-126, exact
	DCFS	0f_00780000	; 0x0.fp-126, exact
	DCFS	0f_007f8000	; 0x0.ffp-126, exact
	DCFS	0f_007fff80	; 0x0.ffffp-126, exact
	DCFS	0f_007fffff	; 0x0.fffffe0p-126, exact
	DCFS	0f_007fffff	; 0x0.fffffe8p-126, rounded down to 0x0.fffffe8p-126
	DCFS	0f_00800000	; 0x0.ffffff0p-126, rounded up to 0x1p-126, became normalized
	DCFS	0f_00800000	; 0x0.ffffff8p-126, rounded up to 0x1p-126, became normalized
	DCFS	0f_00800000	; 0x1.0000000p-126, exact, became normalized

	; Half-precision subnormal values:
	; Minimum:
	DCFS	0f_32C00000
	DCFS	0f_33000000
	DCFS	0f_33200000
	DCFS	0f_33400000
	DCFS	0f_33600000
	DCFS	0f_33800000
	DCFS	0f_33B00000
	DCFS	0f_33C00000
	DCFS	0f_33D00000
	DCFS	0f_33E00000
	DCFS	0f_33F00000
	DCFS	0f_34000000
	DCFS	0f_34180000
	DCFS	0f_34200000
	DCFS	0f_34280000
	; Mid:
	DCFS	0f_36800000
	DCFS	0f_36808000
	DCFS	0f_36810000
	DCFS	0f_36818000
	DCFS	0f_36820000
	DCFS	0f_36828000
	DCFS	0f_36830000
	DCFS	0f_36838000
	; Maximum:
	DCFS	0f_38000000
	DCFS	0f_38200000
	DCFS	0f_38400000
	DCFS	0f_387F0000
	DCFS	0f_387F1000
	DCFS	0f_387F2000
	DCFS	0f_387F3000
	DCFS	0f_387F4000
	DCFS	0f_387F5000
	DCFS	0f_387F6000
	DCFS	0f_387F7000
	DCFS	0f_387F8000
	DCFS	0f_387F9000
	DCFS	0f_387FA000
	DCFS	0f_387FB000
	DCFS	0f_387FC000
	DCFS	0f_387FD000
	DCFS	0f_387FE000
	DCFS	0f_387FF000
	DCFS	0f_38800000
	DCFS	0f_38A00000
	DCFS	0f_38C00000
	DCFS	0f_38E00000
	DCFS	0f_39000000

	; Normalized values:
	DCFS	0f_427FE000
	DCFS	0f_427FE400
	DCFS	0f_427FE800
	DCFS	0f_427FEC00
	DCFS	0f_427FF000
	DCFS	0f_427FF400
	DCFS	0f_427FF800
	DCFS	0f_427FFC00
	DCFS	0f_42800000

	; Half-precision maximum values:
	; - IEEE:
	DCFS	0f_477FE000
	DCFS	0f_477FE400
	DCFS	0f_477FE800
	DCFS	0f_477FEC00
	DCFS	0f_477FF000
	DCFS	0f_477FF400
	DCFS	0f_477FF800
	DCFS	0f_477FFC00
	DCFS	0f_47800000
	; - ARM alternative:
	DCFS	0f_47FFE000
	DCFS	0f_47FFE400
	DCFS	0f_47FFE800
	DCFS	0f_47FFEC00
	DCFS	0f_47FFF000
	DCFS	0f_47FFF400
	DCFS	0f_47FFF800
	DCFS	0f_47FFFC00
	DCFS	0f_48000000

	; +/-Inf:
	DCD	&7f800000
	DCD	&ff800000

	; sNan:
	[ {FALSE} ; FIXME: should be {TRUE}, sNaN gets wrongly turned into qNaN
	DCD	&7f880000
	DCD	&7f808000
	DCD	&ff880000
	DCD	&ff808000
	|
	DCD	&7fc80000
	DCD	&7fc08000
	DCD	&ffc80000
	DCD	&ffc08000
	]

	; qNan:
	DCD	&7fc80000
	DCD	&7fc08000
	DCD	&ffc80000
	DCD	&ffc08000
	]

	[ TestHF
	; *** OUTPUT HALF-PRECISION (IEEE):
	; Zero:
	DCW	&0000	; +0.
	DCW	&8000	; -0.

	; Single-precision subnormal values:
	; Minimum:
	DCW	&0000	; 0x0.8p-126, rounded down to +0.
	DCW	&0000	; 0x0.fp-126, rounded down to +0.
	DCW	&0000	; 0x0.ffp-126, rounded down to +0.
	DCW	&0000	; 0x0.ffffp-126, rounded down to +0.
	DCW	&0000	; 0x0.fffffe0p-126, rounded down to +0.
	DCW	&0000	; 0x0.fffffe8p-126, rounded down to +0.
	DCW	&0000	; 0x0.ffffff0p-126, rounded down to +0.
	DCW	&0000	; 0x0.ffffff8p-126, rounded down to +0.
	DCW	&0000	; 0x1.0000000p-126, rounded down to +0.

	; Half-precision subnormal values:
	; Minimum:
	DCW	&0000	; 0x0.6p-24, rounded down to +0.
	DCW	&0000	; 0x0.8p-24, rounded down to +0.
	DCW	&0001	; 0x0.ap-24, rounded up to 0x1.p-24
	DCW	&0001	; 0x0.cp-24, rounded up to 0x1.p-24
	DCW	&0001	; 0x0.ep-24, rounded up to 0x1.p-24
	DCW	&0001	; 0x1.0p-24, exact
	DCW	&0001	; 0x1.6p-24, rounded down to 0x1.p-24
	DCW	&0002	; 0x1.8p-24, rounded up to 0x1.p-25
	DCW	&0002	; 0x1.ap-24, rounded up to 0x1.p-25
	DCW	&0002	; 0x1.cp-24, rounded up to 0x1.p-25
	DCW	&0002	; 0x1.ep-24, rounded up to 0x1.p-25
	DCW	&0002	; 0x2.0p-24, exact
	DCW	&0002	; 0x2.6p-24, rounded down to 0x1.p-25
	DCW	&0002	; 0x2.8p-24, rounded down to 0x1.p-25
	DCW	&0003	; 0x2.ap-24, rounded up to 0x3.p-25
	; Mid:
	DCW	&0040	; 0x1.00p-18, exact
	DCW	&0040	; 0x1.01p-18, rounded down to 0x1.p-18
	DCW	&0040	; 0x1.02p-18, rounded down to 0x1.p-18
	DCW	&0041	; 0x1.03p-18, rounded up to 0x1.04p-18
	DCW	&0041	; 0x1.04p-18, exact
	DCW	&0041	; 0x1.05p-18, rounded down to 0x1.04p18
	DCW	&0042	; 0x1.06p-18, rounded up to 0x1.08p18
	DCW	&0042	; 0x1.07p-18, rounded up to 0x1.08p18
	; Maximum:
	DCW	&0200	; 0x0.8p-14, exact
	DCW	&0280	; 0x0.ap-14, exact
	DCW	&0300	; 0x0.cp-14, exact

	DCW	&03FC	; 0x0.ff0p-14, exact
	DCW	&03FC	; 0x0.ff1p-14, rounded down to 0x0.ff0p-14
	DCW	&03FC	; 0x0.ff2p-14, rounded down to 0x0.ff0p-14
	DCW	&03FD	; 0x0.ff3p-14, rounded up to 0x0.ff4p-14
	DCW	&03FD	; 0x0.ff4p-14, exact
	DCW	&03FD	; 0x0.ff5p-14, rounded down to 0x0.ff4p-14
	DCW	&03FE	; 0x0.ff6p-14, rounded up to 0x0.ff8p-14
	DCW	&03FE	; 0x0.ff7p-14, rounded up to 0x0.ff8p-14
	DCW	&03FE	; 0x0.ff8p-14, exact
	DCW	&03FE	; 0x0.ff9p-14, rounded down to 0x0.ff8p-14
	DCW	&03FE	; 0x0.ffap-14, rounded down to 0x0.ff8p-14
	DCW	&03FF	; 0x0.ffbp-14, rounded up to 0x0.ff9p-14
	DCW	&03FF	; 0x0.ffcp-14, exact
	DCW	&03FF	; 0x0.ffdp-14, rounded down to 0x0.ffcp-14
	DCW	&0400	; 0x0.ffep-14, rounded up to 0x1.p-14
	DCW	&0400	; 0x0.fffp-14, rounded up to 0x1.p-14
	DCW	&0400	; 0x1.0p-14, exact, vvv Half-precision normalized.
	DCW	&0500	; 0x1.4p-14, exact
	DCW	&0600	; 0x1.8p-14, exact
	DCW	&0700	; 0x1.cp-14, exact
	DCW	&0800	; 0x1.0p-13, exact, ^^^ Half-precision normalized.

	; Normalized values:
	DCW	&53FF	; 0x1.ffc0p5, exact
	DCW	&53FF	; 0x1.ffc8p5, rounded down to 0x1.ffcp5
	DCW	&53FF	; 0x1.ffd0p5, rounded down to 0x1.ffcp5
	DCW	&53FF	; 0x1.ffd8p5, rounded down to 0x1.ffcp5
	DCW	&5400	; 0x1.ffe0p5, rounded up to 0x1.p6
	DCW	&5400	; 0x1.ffe8p5, rounded up to 0x1.p6
	DCW	&5400	; 0x1.fff0p5, rounded up to 0x1.p6
	DCW	&5400	; 0x1.fff8p5, rounded up to 0x1.p6
	DCW	&5400	; 0x1.000p6, exact

	; Half-precision maximum values:
	; - IEEE:
	DCW	&7BFF	; 0x1.ffc0p15, exact
	DCW	&7BFF	; 0x1.ffc8p15, rounded down to 0x1.ffcp15
	DCW	&7BFF	; 0x1.ffd0p15, rounded down to 0x1.ffcp15
	DCW	&7BFF	; 0x1.ffd8p15, rounded down to 0x1.ffcp15
	DCW	&7C00	; 0x1.ffe0p15, rounded up to +Inf
	DCW	&7C00	; 0x1.ffe8p15, rounded up to +Inf
	DCW	&7C00	; 0x1.fff0p15, rounded up to +Inf
	DCW	&7C00	; 0x1.fff8p15, rounded up to +Inf
	DCW	&7C00	; 0x1.0000p16, overflow, is +Inf
	; - ARM alternative:
	DCW	&7C00	; 0x1.ffc0p16, overflow, is +Inf
	DCW	&7C00	; 0x1.ffc8p16, overflow, is +Inf
	DCW	&7C00	; 0x1.ffd0p16, overflow, is +Inf
	DCW	&7C00	; 0x1.ffd8p16, overflow, is +Inf
	DCW	&7C00	; 0x1.ffe0p16, overflow, is +Inf
	DCW	&7C00	; 0x1.ffe8p16, overflow, is +Inf
	DCW	&7C00	; 0x1.fff0p16, overflow, is +Inf
	DCW	&7C00	; 0x1.fff8p16, overflow, is +Inf
	DCW	&7C00	; 0x1.0000p17, overflow, is +Inf
	]

	; +/-Inf:
	DCW	&7c00	; +Inf
	DCW	&fc00	; -Inf

	; sNan:
	[ {FALSE} ; FIXME: should be {TRUE}, sNaN gets wrongly turned into qNaN
	DCW	&7c40	;  qNan(0x040)
	DCW	&7c04	;  qNan(0x004)
	DCW	&fc40	; -qNaN(0x040)
	DCW	&fc04	; -qNan(0x004)
	|
	DCW	&7e40	;  sNan(0x040)
	DCW	&7e04	;  sNan(0x004)
	DCW	&fe40	; -sNaN(0x040)
	DCW	&fe04	; -sNan(0x004)
	]

	; sNan:
	DCW	&7e40	;  sNan(0x040)
	DCW	&7e04	;  sNan(0x004)
	DCW	&fe40	; -sNaN(0x040)
	DCW	&fe04	; -sNan(0x004)

	]

	; Specific tests:
	; - Check if half-precision value is obtained from a double-precision
	;   value by an intermediate single-precision cast.
	[ :LNOT: REFERENCE
	DCFH	0x1.0020008p0
	|
	DCFH	0x1.p0
	]

	AREA	DataUnaligned, DATA
	[ :LNOT: REFERENCE
	DCB	42
	DCFHU	0x1.ffc0p15
	DCB	41
	|
	DCB	42
	DCB	&FF, &7B
	DCB	41
	]

	END
