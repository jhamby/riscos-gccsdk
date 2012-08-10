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
			DCI &0320f000 	; nopeq
			DCI &1320f000 	; nopne
			DCI &2320f000 	; nopcs
			DCI &2320f000 	; nopcs
			DCI &3320f000 	; nopcc
			DCI &3320f000 	; nopcc
			DCI &4320f000 	; nopmi
			DCI &5320f000 	; noppl
			DCI &6320f000 	; nopvs
			DCI &7320f000 	; nopvc
			DCI &8320f000 	; nophi
			DCI &9320f000 	; nopls
			DCI &a320f000 	; nopge
			DCI &b320f000 	; noplt
			DCI &c320f000 	; nopgt
			DCI &d320f000 	; nople
			DCI &e320f000 	; nop
			DCI &e320f000 	; nop
		ELSE
			DCI &E1A00000
		ENDIF
	]

	END
