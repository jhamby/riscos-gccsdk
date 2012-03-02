; Test NOP for ARMv6K, ARMv6TE and ARMv7 architectures.
; -RUNOPT: -CPU=Cortex-A8

	GET	tests_pass/include/nop.s

	; Test the Thumb variants.
	THUMB
	[ :LNOT: REFERENCE

	NOP
	NOP.W

	|

	DCI 0xBF00
	DCI.W 0xf3af8000

	]

	END
