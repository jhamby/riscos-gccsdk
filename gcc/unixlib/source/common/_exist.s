;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/_exist.s,v $
; $Date: 2003/08/18 22:35:36 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	IMPORT	|__feature_imagefs_is_file|, WEAK

	AREA	|C$$code|, CODE, READONLY

	; int __isdir_raw (const char *dir)
	; Return nonzero if DIR is a directory.
	; DIR is a RISC OS file specification.

	EXPORT	|__isdir_raw|
	NAME	__isdir_raw
|__isdir_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	v1, #0		; Error, so assume not dir
	MOVVC	v1, a1
	BL	|__get_feature_imagefs_is_file|
	TEQ	a1, #0
	ANDEQ	a1, v1, #2	; feature imagefs not set, so bit 1 set for dir or image
	TEQNE	v1, #2		; feature imagefs set, so only real directories
	MOVNE	a1, #0
	LDMFD	sp!, {v1, v2, pc}

	; int __object_exists_raw (const char *object)
	; Return nonzero if OBJECT exists (as file, dir or image).
	; OBJECT is a RISC OS file specification.

	EXPORT	|__object_exists_raw|
	NAME	__object_exists_raw
|__object_exists_raw|
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		; Error, so assume not there
	LDMFD	sp!, {v1, v2, pc}

	; int __get_feature_imagefs_is_file (void)
	EXPORT	|__get_feature_imagefs_is_file|
	NAME	"__get_feature_imagefs_is_file"
|__get_feature_imagefs_is_file|
	LDR	a1, =|__feature_imagefs_is_file|
	TEQ	a1, #0
	LDREQ	a1, =|__feature_imagefs_is_file_internal|
	LDR	a1, [a1, #0]
	MOV	pc, lr

	; At the moment, we don't need to manipulate the
	; __set_feature_imagefs_is_file feature directly inside UnixLib, so
	; following routine is commented out until we need it.

;	; void __set_feature_imagefs_is_file(int __feature_imagefs_is_file_value)
;	EXPORT	|__set_feature_imagefs_is_file|
;	NAME	"__set_feature_imagefs_is_file"
;|__set_feature_imagefs_is_file|
;	LDR	a2, =|__feature_imagefs_is_file|
;	TEQ	a2, #0
;	LDREQ	a2, =|__feature_imagefs_is_file_internal|
;	STR	a1, [a2, #0]
;	return	AL, pc, lr

	AREA	|C$$data|, DATA

|__feature_imagefs_is_file_internal|
	DCD	0

	END
