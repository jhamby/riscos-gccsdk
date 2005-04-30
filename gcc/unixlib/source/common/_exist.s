; Copyright (c) 2002, 2003, 2004, 2005 UnixLib Developers

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
	NAME	__get_feature_imagefs_is_file
|__get_feature_imagefs_is_file|
	LDR	a1, =|__feature_imagefs_is_file|
	TEQ	a1, #0
	LDREQ	a1, =|__feature_imagefs_is_file_internal|
	LDR	a1, [a1, #0]
	MOV	pc, lr

	AREA	|C$$data|, DATA

|__feature_imagefs_is_file_internal|
	DCD	0

	END
