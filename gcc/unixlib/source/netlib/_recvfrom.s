;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_recvfrom.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_recvfrom|
	NAME	_recvfrom
|_recvfrom|
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWI	XSocket_Recvfrom
	|
	NetSWI	XSocket_Recvfrom_1
	]
	LDMFD	sp!, {r4-r5, pc}

	END
