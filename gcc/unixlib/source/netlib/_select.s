;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/_select,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error|

	EXPORT	|_select|
|_select|
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI	XSocket_Select
	stackreturn	AL, "r4, pc"

	END
