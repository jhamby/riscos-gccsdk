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
 PICEQ "LDR	v5, =__GOTT_BASE__"
 PICEQ "LDR	v5, [v5, #0]"
 PICEQ "LDR	v5, [v5, #__GOTT_INDEX__]"

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
	DECLARE_FUNCTION __unixlib_cxa_demangle

	.end
