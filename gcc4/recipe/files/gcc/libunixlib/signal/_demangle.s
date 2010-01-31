@ Demangle C++ symbols using libstc++ '__cxa_demangle' implementation when
@ libstc++ is available during link time.
@ Copyright (c) 2006-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.weak	__cxa_demangle

	@char *__unixlib_cxa_demangle(const char *__mangled_name,
	@			char *__output_buffer, size_t *__length,
	@			int *__status);
	.global	__unixlib_cxa_demangle
	NAME	__unixlib_cxa_demangele
__unixlib_cxa_demangle:
 PICEQ "LDR	v5, .L0+4"
.LPIC0:
 PICEQ "ADD	v5, pc, v5"		@ v5 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v5, {v5, ip}"		@ v5 = Object index, ip = GOT ptr array location
 PICEQ "LDR	ip, [ip, #0]"		@ ip = GOT ptr array
 PICEQ "LDR	v5, [ip, v5, LSL#4]"	@ v5 = GOT ptr

	LDR	ip, .L0			@=__cxa_demangle
 PICEQ "LDR	ip, [v5, ip]"
	TEQ	ip, #0
	MOVNE	PC, ip
	MOV	a1, #-1		@ Write -1 = "A memory allocation failure occurred"
	STR	a1, [a4]
	MOV	a1, #0
	MOV	PC, R14
.L0:
	WORD	__cxa_demangle
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION __unixlib_cxa_demangle

	.end
