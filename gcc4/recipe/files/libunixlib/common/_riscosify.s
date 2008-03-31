@ __get_riscosify_control, __set_riscosify_control
@ Copyright (c) 2002-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.weak	__riscosify_control

	@int __get_riscosify_control (void)
	.global	__get_riscosify_control
	NAME	__get_riscosify_control
__get_riscosify_control:
 PICEQ "LDR	a1, .L0+8"
.LPIC0:
 PICEQ "ADD	a1, pc, a1"		@ a1 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	a1, {a1, a2}"		@ a1 = Object index, a2 = GOT ptr array location
 PICEQ "LDR	a2, [a2, #0]"		@ a2 = GOT ptr array
 PICEQ "LDR	a2, [a2, a1, LSL#4]"	@ a2 = GOT ptr

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
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION __get_riscosify_control

	@void __set_riscosify_control(int __riscosify_flags)
	.global	__set_riscosify_control
	NAME	__set_riscosify_control
__set_riscosify_control:
 PICEQ "LDR	a2, .L1"
.LPIC1:
 PICEQ "ADD	a2, pc, a2"		@ a2 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	a2, {a2, a3}"		@ a2 = Object index, a3 = GOT ptr array location
 PICEQ "LDR	a3, [a3, #0]"		@ a3 = GOT ptr array
 PICEQ "LDR	a3, [a3, a2, LSL#4]"	@ a3 = GOT ptr

	LDR	a2, .L0+4		@=__riscosify_control
 PICEQ "LDR	a2, [a3, a2]"
	TEQ	a2, #0
	LDREQ	a2, .L0			@=__riscosify_control_internal
 PICEQ "LDREQ	a2, [a3, a2]"
	STR	a1, [a2, #0]
	MOV	pc, lr
.L1:
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+4)"
	DECLARE_FUNCTION __set_riscosify_control

	.data

__riscosify_control_internal:
	.word	0
	DECLARE_OBJECT __riscosify_control_internal

	.end
