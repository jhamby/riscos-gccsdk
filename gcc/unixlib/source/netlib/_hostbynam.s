;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/_hostbynam,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
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
