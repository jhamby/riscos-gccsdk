@ Copyright (c) 2002-2013 UnixLib Developers

#include "internal/asm_dec.s"

	.weak	__feature_imagefs_is_file

	.text

	@ int __get_feature_imagefs_is_file (void)
	.global	__get_feature_imagefs_is_file
	NAME	__get_feature_imagefs_is_file
__get_feature_imagefs_is_file:
 PICEQ "LDR	a1, =__GOTT_BASE__"
 PICEQ "LDR	a1, [a1, #0]"
 PICEQ "LDR	a2, [a1, #__GOTT_INDEX__]"	@ a2 = GOT ptr

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
	DECLARE_FUNCTION __get_feature_imagefs_is_file

	.data

__feature_imagefs_is_file_internal:
	.word	1
	DECLARE_OBJECT __feature_imagefs_is_file_internal

	.end
