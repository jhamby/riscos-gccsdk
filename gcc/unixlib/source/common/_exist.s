;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/_exist.s,v $
; $Date: 2000/07/15 14:52:18 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

; int __isdir_raw (const char *dir)
; Return nonzero if DIR is a directory.

	EXPORT	|__isdir_raw|
|__isdir_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		; Error, so assume not dir
	ANDVC	a1, a1, #2	; Bit 1 set for dir or image
	stackreturn	AL, "v1, v2, pc"

; int __object_exists_raw (const char *object)
; Return nonzero if object exists (object, dir or image).

	EXPORT	|__object_exists_raw|
|__object_exists_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		; Error, so assume not there
	stackreturn	AL, "v1, v2, pc"

	END
