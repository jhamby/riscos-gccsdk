;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	IMPORT	|__feature_imagefs_is_file|

	AREA	|C$$code|,CODE,READONLY

; int __isdir_raw (const char *dir)
; Return nonzero if DIR is a directory.

	EXPORT	|__isdir_raw|
	NAME	__isdir_raw
|__isdir_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		; Error, so assume not dir
	BVS	isdirreturn
	LDR	a2, =|__feature_imagefs_is_file|
	LDR	a2, [a2]
	CMP	a2, #0
	ANDEQ	a1, a1, #2	; feature imagefs not set, so bit 1 set for dir or image
	CMPNE	a1, #2		; feature imagefs set, so only real directories
	MOVNE	a1, #0
isdirreturn
	stackreturn	AL, "v1, v2, pc"

; int __object_exists_raw (const char *object)
; Return nonzero if object exists (object, dir or image).

	EXPORT	|__object_exists_raw|
	NAME	__object_exists_raw
|__object_exists_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		; Error, so assume not there
	stackreturn	AL, "v1, v2, pc"

	END
