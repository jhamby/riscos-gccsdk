;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/_hostbyadd,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
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
