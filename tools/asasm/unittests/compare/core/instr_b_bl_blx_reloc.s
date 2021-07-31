; Tests ELF relocations for B, BL, BLX <label>
; -RUNOPT: -CPU=7-A

	[ OUTPUTTYPE == "ELF"

	; IMPORT extSymbol_AB01
	IMPORT extSymbol_AB02
	; IMPORT extSymbol_TB01
	IMPORT extSymbol_TB02

	; IMPORT extSymbol_ABL01
	IMPORT extSymbol_ABL02
	; IMPORT extSymbol_TBL01
	IMPORT extSymbol_ABL02

	; IMPORT extSymbol_ABLX01
	IMPORT extSymbol_ABLX02
	; IMPORT extSymbol_TBLX01
	IMPORT extSymbol_ABLX02

	AREA CodeELFReloc, CODE
	[ :LNOT: REFERENCE
	; B <label> reloc:
	ARM
	B extSymbol_AB01 + 8
	B extSymbol_AB02 - 20

	; Long range branches are always taken for unresolved symbols:
	THUMB
	B extSymbol_TB01 + 6
	B extSymbol_TB02 - 6
	B.N extSymbol_TB01 + 6
	B.N extSymbol_TB02 - 6
	B.W extSymbol_TB01 + 6
	B.W extSymbol_TB02 - 6

	; BL <label> reloc:
	ARM
	BL extSymbol_ABL01 + 40
	BL extSymbol_ABL02 - 40

	THUMB
	BL extSymbol_TBL01 + 40
	BL extSymbol_TBL02 - 40
	BL.W extSymbol_TBL01 + 40
	BL.W extSymbol_TBL02 - 40

	; BLX <label> reloc:
	ARM
	BLX extSymbol_ABLX01 + 40
	BLX extSymbol_ABLX02 - 40

	THUMB
	BLX extSymbol_TBLX01 + 40
	BLX extSymbol_TBLX02 - 40
	BLX.W extSymbol_TBLX01 + 40
	BLX.W extSymbol_TBLX02 - 40

	|

	; B <label> reloc:
	ARM
	DCI &ea000000	; b	extSymbol_AB01 + 8
	RELOC R_ARM_JUMP24, extSymbol_AB01
	DCI &eafffff9	; b	extSymbol_AB02 - 20
	RELOC R_ARM_JUMP24, extSymbol_AB02

	THUMB
	DCI.N &e001	; b.n	extSymbol_TB01 + 6
	RELOC R_ARM_THM_JUMP11, extSymbol_TB01
	DCI.N &e7fb	; b.n	extSymbol_TB02 - 6
	RELOC R_ARM_THM_JUMP11, extSymbol_TB02
	DCI.N &e001	; b.n	extSymbol_TB01 + 6
	RELOC R_ARM_THM_JUMP11, extSymbol_TB01
	DCI.N &e7fb	; b.n	extSymbol_TB02 - 6
	RELOC R_ARM_THM_JUMP11, extSymbol_TB02
	DCI.W &f000b801	; b.w	extSymbol_TB01 + 6
	RELOC R_ARM_THM_JUMP24, extSymbol_TB01
	DCI.W &f7ffbffb	; b.w	extSymbol_TB02 - 6
	RELOC R_ARM_THM_JUMP24, extSymbol_TB02

	; BL <label> reloc:
	ARM
	DCI &eb000008	; bl	extSymbol_ABL01 + 40
	RELOC R_ARM_CALL, extSymbol_ABL01
	DCI &ebfffff4	; bl	extSymbol_ABL02 - 40
	RELOC R_ARM_CALL, extSymbol_ABL02

	THUMB
	DCI.W &f000f812	; bl	extSymbol_TBL01 + 40
	RELOC R_ARM_THM_CALL, extSymbol_TBL01
	DCI.W &f7ffffea	; bl	extSymbol_TBL02 - 40
	RELOC R_ARM_THM_CALL, extSymbol_TBL02
	DCI.W &f000f812	; bl	extSymbol_TBL01 + 40
	RELOC R_ARM_THM_CALL, extSymbol_TBL01
	DCI.W &f7ffffea	; bl	extSymbol_TBL02 - 40
	RELOC R_ARM_THM_CALL, extSymbol_TBL02

	; BLX <label> reloc:
	ARM
	DCI &fa000008	; blx	extSymbol_ABLX01 + 40
	RELOC R_ARM_CALL, extSymbol_ABLX01
	DCI &fafffff4	; blx	extSymbol_ABLX02 - 40
	RELOC R_ARM_CALL, extSymbol_ABLX02

	THUMB
	DCI.W &f000e812	; blx	extSymbol_TBLX01 + 40
	RELOC R_ARM_THM_CALL, extSymbol_TBLX01
	DCI.W &f7ffefea	; blx	extSymbol_TABLX02 - 40
	RELOC R_ARM_THM_CALL, extSymbol_TBLX02
	DCI.W &f000e812	; blx	extSymbol_TABLX01 + 40
	RELOC R_ARM_THM_CALL, extSymbol_TBLX01
	DCI.W &f7ffefea	; blx	extSymbol_TABLX02 - 40
	RELOC R_ARM_THM_CALL, extSymbol_TBLX02

	]

	]

	END
