;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_hostbyadd.s,v $
; $Date: 2000/07/15 14:52:26 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_gethostbyaddr|
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
