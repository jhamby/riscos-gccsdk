;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_sendto.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error|

	EXPORT	|_sendto|
	NAME	_sendto
|_sendto|
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	NetSWI	XSocket_Sendto
	LDMFD	sp!, {r4-r5, pc}

	END
