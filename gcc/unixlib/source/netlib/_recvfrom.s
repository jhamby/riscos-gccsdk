;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/_recvfrom,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error|

	EXPORT	|_recvfrom|
|_recvfrom|
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	NetSWI	XSocket_Recvfrom
	stackreturn	AL, "r4-r5, pc"

	END
