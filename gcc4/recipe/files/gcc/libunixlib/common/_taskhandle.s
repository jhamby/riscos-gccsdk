@ __get_taskhandle
@ Copyright (c) 2007-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int __get_taskhandle (void)
	@
	@ Returns __ul_global.taskhandle.  This is a separate routine as at
	@ runtime we can become a WIMP task so our check at startup can become
	@ outdated.  Note that once we've found out that we're a task we
	@ continue to claim that.
	.global	__get_taskhandle
	NAME	__get_taskhandle
__get_taskhandle:
 PICEQ "LDR	a3, =__GOTT_BASE__"
 PICEQ "LDR	a3, [a3, #0]"
 PICEQ "LDR	a3, [a3, #__GOTT_INDEX__]"	@ a3 = GOT ptr

	LDR	ip, .L0			@ ip = __ul_global
 PICEQ "LDR	ip, [a3, ip]"

	@ Non-zero taskhandle ? Yes, we know enough and return.
	LDR	a1, [ip, #GBL_TASKHANDLE]
	TEQ	a1, #0
	MOVNE	pc, lr

	MOV	a1, #3			@ In desktop?
	SWI	XWimp_ReadSysInfo
	MOVVS	a1, #0
	TEQ	a1, #0
	MOVNE	a1, #5			@ Read taskhandle, iff in desktop
	SWINE	XWimp_ReadSysInfo
	MOVVS	a1, #0

	STR	a1, [ip, #GBL_TASKHANDLE]

	MOV	pc, lr
.L0:
	WORD	__ul_global
	DECLARE_FUNCTION __get_taskhandle

	.end
