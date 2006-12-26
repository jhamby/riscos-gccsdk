; UnixLib GCC 4 alloca function interface.
; These functions tie-in directly with both UnixLib and the GCC compiler.
; Change one, change the other.
; Copyright (c) 2005, 2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	[ __UNIXLIB_ELF > 0
	@ BEWARE: this looks like GAS syntax but isn't, the labels don't
	@ have a colon.
	.text

	.global	__gcc_alloca_free
__gcc_alloca_free
	@ We must save any call-clobbered registers that are likely to be
	@ corrupted by the following function.  Otherwise we risk corrupting
	@ the return values in a1 & a2 from functions.
	@ No need to save lr as we won't use it to return.
	stmfd	sp!, {a1, a2}
	@ Return from this function will hold the real return address.
	bl	__gcc_alloca_free_1
	mov	ip, a1
	ldmfd	sp!, {a1, a2}
	mov	pc, ip

	]

	END
