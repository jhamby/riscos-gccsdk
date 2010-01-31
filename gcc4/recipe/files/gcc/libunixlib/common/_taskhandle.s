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
 PICEQ "LDR	a3, .L0+4"
.LPIC0:
 PICEQ "ADD	a3, pc, a3"		@ a3 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	a3, {a3, a4}"		@ a3 = Object index, a4 = GOT array location
 PICEQ "LDR	a4, [a4, #0]"		@ a4 = GOT array
 PICEQ "LDR	a3, [a4, a3, LSL#4]"	@ a3 = GOT (private)

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
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION __get_taskhandle

	.end
