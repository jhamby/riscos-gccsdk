@ __get_riscosify_control, __set_riscosify_control
@ Copyright (c) 2002-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.weak	__riscosify_control

	@int __get_riscosify_control (void)
	.global	__get_riscosify_control
	NAME	__get_riscosify_control
__get_riscosify_control:
 PICEQ "LDR	a2, =__GOTT_BASE__"
 PICEQ "LDR	a2, [a2, #0]"
 PICEQ "LDR	a2, [a2, #__GOTT_INDEX__]"	@ a2 = GOT ptr

	LDR	a1, .L0+4		@=__riscosify_control
 PICEQ "LDR	a1, [a2, a1]"
	TEQ	a1, #0
	LDREQ	a1, .L0			@=__riscosify_control_internal
 PICEQ "LDREQ	a1, [a2, a1]"
	LDR	a1, [a1, #0]
	MOV	pc, lr
.L0:
	WORD	__riscosify_control_internal
	WORD	__riscosify_control
	DECLARE_FUNCTION __get_riscosify_control

	@void __set_riscosify_control(int __riscosify_flags)
	.global	__set_riscosify_control
	NAME	__set_riscosify_control
__set_riscosify_control:
 PICEQ "LDR	a3, =__GOTT_BASE__"
 PICEQ "LDR	a3, [a3, #0]"
 PICEQ "LDR	a3, [a3, #__GOTT_INDEX__]"	@ a3 = GOT ptr

	LDR	a2, .L0+4		@=__riscosify_control
 PICEQ "LDR	a2, [a3, a2]"
	TEQ	a2, #0
	LDREQ	a2, .L0			@=__riscosify_control_internal
 PICEQ "LDREQ	a2, [a3, a2]"
	STR	a1, [a2, #0]
	MOV	pc, lr
	DECLARE_FUNCTION __set_riscosify_control

	.data

__riscosify_control_internal:
	.word	0
	DECLARE_OBJECT __riscosify_control_internal

	.end
