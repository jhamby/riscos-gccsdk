;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_hostbynam.s,v $
; $Date: 2000/07/15 14:52:26 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_gethostbyname|
|_gethostbyname|
	STMFD	sp!, {lr}
	MOV	r1, r0
	MOV	r0, #0
	SWI	XInternet_GetHostByName
	MOVVC	r0, r1
	MOVVS	r0, #0
	stackreturn	AL, "pc"

	END
