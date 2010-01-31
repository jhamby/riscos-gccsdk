@ Copyright (c) 2002, 2003, 2004, 2005, 2008 UnixLib Developers

#include "internal/asm_dec.s"

	.weak	__feature_imagefs_is_file

	.text

	@ int __isdir_raw (const char *dir)
	@ Return nonzero if DIR is a directory.
	@ DIR is a RISC OS file specification.

	.global	__isdir_raw
	NAME	__isdir_raw
__isdir_raw:
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	v1, #0		@ Error, so assume not dir
	MOVVC	v1, a1
	BL	__get_feature_imagefs_is_file
	TEQ	a1, #0
	ANDEQ	a1, v1, #2	@ feature imagefs not set, so bit 1 set for dir or image
	TEQNE	v1, #2		@ feature imagefs set, so only real directories
	MOVNE	a1, #0
	LDMFD	sp!, {v1, v2, pc}
	DECLARE_FUNCTION __isdir_raw

	@ int __object_exists_raw (const char *object)
	@ Return nonzero if OBJECT exists (as file, dir or image).
	@ OBJECT is a RISC OS file specification.

	.global	__object_exists_raw
	NAME	__object_exists_raw
__object_exists_raw:
	STMFD	sp!, {v1, v2, lr}
	MOV	a2, a1
	MOV	a1, #17
	SWI	XOS_File
	MOVVS	a1, #0		@ Error, so assume not there
	LDMFD	sp!, {v1, v2, pc}
	DECLARE_FUNCTION __object_exists_raw

	@ int __get_feature_imagefs_is_file (void)
	.global	__get_feature_imagefs_is_file
	NAME	__get_feature_imagefs_is_file
__get_feature_imagefs_is_file:
 PICEQ "LDR	a1, .L0+8"
.LPIC0:
 PICEQ "ADD	a1, pc, a1"		@ a1 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	a1, {a1, a2}"		@ a1 = Object index, a2 = GOT ptr array location
 PICEQ "LDR	a2, [a2, #0]"		@ a2 = GOT ptr array
 PICEQ "LDR	a2, [a2, a1, LSL#4]"	@ a2 = GOT ptr

	LDR	a1, .L0			@=__feature_imagefs_is_file
 PICEQ "LDR	a1, [a2, a1]"
	TEQ	a1, #0
	LDREQ	a1, .L0+4		@=__feature_imagefs_is_file_internal
 PICEQ "LDREQ	a1, [a2, a1]"
	LDR	a1, [a1, #0]
	MOV	pc, lr
.L0:
	WORD	__feature_imagefs_is_file
	WORD	__feature_imagefs_is_file_internal
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION __get_feature_imagefs_is_file

	.data

__feature_imagefs_is_file_internal:
	.word	1
	DECLARE_OBJECT __feature_imagefs_is_file_internal

	.end
