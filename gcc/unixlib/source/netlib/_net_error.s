;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_net_error.s,v $
; $Date: 2000/07/15 14:52:26 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

; Utility function to record a networking (tcp/ip) swi error. a1 is
; a (kernel_oserror*) and errno is extracted from this. The last kernel
; oserror is updated.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|errno|
	IMPORT	|__seterr|

; Entry point from NetSWIsimple
; B to here in SVC mode with return address (flags and mode) in ip
	EXPORT	|__net_error_simple_entry|
|__net_error_simple_entry|
	MOV	lr, ip		; restore normality
;
; continue through into standard net_error code
; standard net_error entered in SVC with BLVS immediately after SWI
; hence in this case ip contains lr from caller (ie return address and mode)
; and hence must be preserved
;
	EXPORT	|__net_error|
|__net_error|
	STMFD	sp!, {a1, ip, lr}	; preserve ip (for when called as subroutine
				; by NetSWI)
	BL	|__seterr|	; save error for last_oserror
	LDMFD	sp!, {a1, ip, lr}
	LDR	a1, [a1, #0]		; get real errno from oserror
	__set_errno	a1, a2
	return	AL, pc, lr

	END
