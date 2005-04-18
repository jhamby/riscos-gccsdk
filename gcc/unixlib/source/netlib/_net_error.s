;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_net_error.s,v $
; $Date: 2004/11/28 21:31:34 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; Function to record a networking (TCP/IP) SWI error.
	; a1 is a (kernel_oserror *) RISC OS error block and errno is
	; extracted from this.

	EXPORT	|__net_error|
	NAME	__net_error
|__net_error|
	STMFD	sp!, {a1, lr}
	MOV	a2, #0
	BL	|__ul_seterr|	   ; Make a copy of RISC OS error block
	LDMFD	sp!, {a1, lr}
	LDR	a1, [a1, #0]       ; Get real errno from RISC OS error number
	AND     a1, a1, #0xff      ; RISC OS 5 correctly returns error
	                           ; numbers in Internet error block.
	__set_errno	a1, a2
	MOV	pc, lr

	END
