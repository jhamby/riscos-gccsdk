;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getpagesiz.s,v $
; $Date: 2001/01/29 15:10:22 $
; $Revision: 1.2 $
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
	return	AL, pc, lr

	END
