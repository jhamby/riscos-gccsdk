; UnixLib GCC 4 alloca function interface.
; These functions tie-in directly with both UnixLib and the GCC compiler.
; Change one, change the other.
; Copyright (c) 2005 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY
	EXPORT	|__gcc_alloca_free|
|__gcc_alloca_free|
	; We must save any call-clobbered registers that are likely to be
	; corrupted by the following function.  Otherwise we risk corrupting
	; the return values from functions.
	stmfd	sp!, {a1, a2, a3, a4, lr}
	; If we pass the frame pointer in here, then we don't need to
	; provide specific code.
	mov	a1, fp
	; Return from this function will hold the real return address
	; or zero.  The latter case might happen where a call to
	; __gcc_alloca_restore has occurred before function exit.
	bl	__gcc_alloca_free_1
	movs	ip, a1
	ldmfd	sp!, {a1, a2, a3, a4, lr}
	movne	lr, ip		; if non-zero, then it is real return address

	teq	pc, pc
	movnes	pc, lr		; 26-bit mode, restore flags
	mov	pc, lr		; 32-bit mode, scrap flags
