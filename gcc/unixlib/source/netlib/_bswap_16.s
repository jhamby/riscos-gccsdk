;----------------------------------------------------------------------------
;
; $Source: $
; $Date: $
; $Revision: $
; $State: $
; $Author: $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	__bswap_16
	NAME	__bswap_16
__bswap_16
	MOV	r2, #255
	AND	r1, r2, r0, LSR #8
	AND	r0, r0, r2
	ORR	r0, r1, r0, LSL #8
	return	AL, pc, lr

	END
