;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_address.s,v $
; $Date: 2000/07/15 14:52:33 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$code|, CODE, READONLY

	EXPORT	|__address_valid|
|__address_valid|

	MOV	ip, lr
	SWI	XOS_ValidateAddress
	MOVCC	r0, #TRUE
	MOVCS	r0, #FALSE
	MOVVS	r0, #FALSE
	return	AL, pc, ip

	END
