;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/unix/s/getpagesiz,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.5 $
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
getpagesize
	SWI	XOS_ReadMemMapInfo
	return	AL, pc, lr

	END
