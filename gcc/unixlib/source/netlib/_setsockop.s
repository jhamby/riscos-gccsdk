;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_setsockop.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
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
	LDMFD	sp!, {r4, pc}

	END
