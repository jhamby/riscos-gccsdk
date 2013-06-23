; Tests ELF relocation for BX <register> when assembled for targets up to
; ARMv4T and ARMv5 (in ARM mode, not thumb).
; RUNOPT: -CPU=4T

	[ OUTPUTELF

	AREA CodeELFReloc, CODE

	[ :LNOT: REFERENCE
	THUMB
	BX r3	; Shouldn't generate R_ARM_V4BX reloc.
	ARM
	BX r3	; Should generate R_ARM_V4BX reloc.
	|
	THUMB
	DCI.N &4718	; bx	r3
	DCI.N &0000	; movs	r0, r0
	ARM
	DCI &e12fff13	; bx	r3
	RELOC	R_ARM_V4BX
	]

	] ; OUTPUTELF

	END
