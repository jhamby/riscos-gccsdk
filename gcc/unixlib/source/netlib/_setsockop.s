;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_setsockop.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error|

	EXPORT	|_setsockopt|
	NAME	_setsockopt
|_setsockopt|
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI0	XSocket_Setsockopt
	stackreturn	AL, "r4, pc"

	END
