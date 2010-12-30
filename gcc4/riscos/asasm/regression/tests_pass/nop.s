; Tests NOP

	AREA	Code, CODE

; NOP encodes differenctly when the chosen architecture is ARMv6K,
; ARMv6TE and ARMv7.  Also, for those architectures, we can use condition
; codes.
NewEnc	EQU	{ARCHITECTURE} = "6K" \
			:LOR: {ARCHITECTURE} = "6TE" \
			:LOR: {ARCHITECTURE} = "7-A" \
			:LOR: {ARCHITECTURE} = "7-R" \
			:LOR: {ARCHITECTURE} = "7E-M" \
			:LOR: {ARCHITECTURE} = "7-M"

	[ :LNOT: REFERENCE
		IF NewEnc
			INFO 0, "Testing new style NOP encoding"

			NOPEQ
			NOPNE
			NOPCS
			NOPHS
			NOPCC
			NOPLO
			NOPMI
			NOPPL
			NOPVS
			NOPVC
			NOPHI
			NOPLS
			NOPGE
			NOPLT
			NOPGT
			NOPLE
			NOPAL
			NOP
		ELSE
			INFO 0, "Testing old style NOP encoding"

			NOP
		ENDIF
	|
		IF NewEnc
			DCD &0320f000 	; nopeq
			DCD &1320f000 	; nopne
			DCD &2320f000 	; nopcs
			DCD &2320f000 	; nopcs
			DCD &3320f000 	; nopcc
			DCD &3320f000 	; nopcc
			DCD &4320f000 	; nopmi
			DCD &5320f000 	; noppl
			DCD &6320f000 	; nopvs
			DCD &7320f000 	; nopvc
			DCD &8320f000 	; nophi
			DCD &9320f000 	; nopls
			DCD &a320f000 	; nopge
			DCD &b320f000 	; noplt
			DCD &c320f000 	; nopgt
			DCD &d320f000 	; nople
			DCD &e320f000 	; nop
			DCD &e320f000 	; nop
		ELSE
			DCD &E1A00000
		ENDIF
	]

	END