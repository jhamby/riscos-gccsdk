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

			MACRO
			NopTest	$suffix
			NOPEQ$suffix
			NOPNE$suffix
			NOPCS$suffix
			NOPHS$suffix
			NOPCC$suffix
			NOPLO$suffix
			NOPMI$suffix
			NOPPL$suffix
			NOPVS$suffix
			NOPVC$suffix
			NOPHI$suffix
			NOPLS$suffix
			NOPGE$suffix
			NOPLT$suffix
			NOPGT$suffix
			NOPLE$suffix
			NOPAL$suffix
			NOP$suffix
			MEND

			NopTest		; ARM
			NopTest	.W	; ARM
			THUMB
			NopTest		; T
			NopTest	.N	; T
			NopTest	.W	; T2
		ELSE
			INFO 0, "Testing old style NOP encoding"

			NOP
			THUMB
			NOP
		ENDIF
	|
		IF NewEnc
			; ARM
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
			DCI &e320f000 	; nopal
			DCI &e320f000 	; nop

			; ARM
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
			DCI &e320f000 	; nopal
			DCI &e320f000 	; nop

			THUMB
			; Thumb
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00

			; Thumb
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00
			DCI.N &bf00

			; Thumb-2
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
			DCI.W &f3af8000
		ELSE
			DCI &E1A00000

			THUMB
			DCI.N &46c0
		ENDIF
	]

	END
