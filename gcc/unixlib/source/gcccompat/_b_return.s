; __builtin_return_address
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	; The AOF version of this routine lives in gcc/gcc/libgcc/lib1aof.s

	AREA	|C$$code|, CODE, READONLY

	[ __UNIXLIB_ELF > 0

	IMPORT	|__free_stack_chunk|

	; void * __builtin_return_adress (unsigned int level)

	; This is an implementation of a GCC internal function.  It appears
	; here because we need to cope with the function return address
	; being modified in the frame by calls to stack extension code
	; or calls to alloca.

	; Return the return address of the current function (level == 0)
	; or of one of its callers.  Return 0 if at the top of the stack
	; or the address is unobtainable
	EXPORT	|__builtin_return_address|
|__builtin_return_address|
	; A non-zero level is only required for debugging.  For the time
	; being, do not support it. FIXME
	CMP	a1, #0
	MOVNE	a1, #0
	MOVNE	pc, lr

	; Traditionally the return address is held at fp[-4], but this
	; may change if a stack extension has occurred.
	LDR	a1, [fp, #-4]
	TEQ	pc, pc	; <= FIXME: only works when we know that there is at least one non-zero processor flag bit. Is that the case?
	BICNE	a1, a1, #0xfc000003	; Drop flags in 26-bit mode
	LDR	a2, |.L1|
	; If the return address in the frame points to the '__free_stack_chunk'
	; function, then the real return address can be found
	; at sl[CHUNK_RETURN]
	CMP	a1, a2
	LDREQ	a1, [sl, #CHUNK_RETURN]

	TEQ	pc, pc	; <= FIXME: only works when we know that there is at least one non-zero processor flag bit. Is that the case?
	BICNE	a1, a1, #0xfc000003	; Drop flags in 26-bit mode

	; XXX FIXME: Implement alloca() return address check.
	MOV	pc, lr
	DECLARE_FUNCTION __builtin_return_address

|.L1|
	WORD	__free_stack_chunk
	]

	END
