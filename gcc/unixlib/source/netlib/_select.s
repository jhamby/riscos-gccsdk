;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_select.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error|

	EXPORT	|_select|
	NAME	_select
|_select|
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI	XSocket_Select
	stackreturn	AL, "r4, pc"

	END
