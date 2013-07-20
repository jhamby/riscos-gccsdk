; Basic relocation test for ADR and ADRL (before ARMv6T2, no
; MOVW/MOV32 possible).
; RUNOPT: -CPU=3

	GET	include/reloc_adr.s

	END