; This tests the RELOC directive.

	AREA	Code, CODE

	IMPORT	ImpSym
	LDR	r1, [pc, #-8]
	RELOC	4, ImpSym

	DCD	0
	RELOC	2, ImpSym

	DCD	0, 1, 2, 3, 4	; The last word is relocated R_ARM_TARGET1
	RELOC	38, ImpSym2

	DCD	ImpSym
	RELOC	R_ARM_TARGET1

	END