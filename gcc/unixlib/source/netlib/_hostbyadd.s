;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_hostbyadd.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_gethostbyaddr|
	NAME	_gethostbyaddr
|_gethostbyaddr|
	STMFD	sp!, {lr}
	MOV	r3, r2
	MOV	r2, r1
	MOV	r1, r0
	MOV	r0, #0
	SWI	XInternet_GetHostByAddr
	MOVVC	r0, r1
	MOVVS	r0, #0
	stackreturn	AL, "pc"

	END
