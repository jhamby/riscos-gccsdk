;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getpagesiz.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------
;
; In assembler because it's so simple and small
; Written by Nick Burrett, 13 October 1996.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; size_t getpagesize (void)
	EXPORT	getpagesize
	NAME	getpagesize
getpagesize
	SWI	XOS_ReadMemMapInfo
	MOV	pc, lr

	END
