;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getpagesiz.s,v $
; $Date: 2000/07/15 14:52:46 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------
;
; In assembler because it's so simple and small
; Written by Nick Burrett, 13 October 1996.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; size_t getpagesize (void)
	EXPORT	getpagesize
getpagesize
	SWI	XOS_ReadMemMapInfo
	return	AL, pc, lr

	END
