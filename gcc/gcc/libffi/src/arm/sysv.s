FFI_TYPE_INT EQU 1
FFI_TYPE_FLOAT EQU 2
FFI_TYPE_DOUBLE EQU 3
		
	AREA	|C$$code|, CODE, READONLY

	; a1:   ffi_prep_args
	; a2:   &ecif
	; a3:   cif->bytes
	; a4:   fig->flags
	; sp+0: ecif.rvalue
	; sp+4: fn

	EXPORT	ffi_call_SYSV
ffi_call_SYSV
	; Save registers
        stmfd sp!, {a1-a4, fp, lr}
	mov   fp, sp

	; Make room for all of the new args.
	sub   sp, fp, a3

	; Place all of the ffi_prep_args in position
	mov   ip, a1
	mov   a1, sp
	;     a2 already set

	; And call
	mov   lr, pc
	mov   pc, ip

	; move first 4 parameters in registers
	ldr   a1, [sp, #0]
	ldr   a2, [sp, #4]
	ldr   a3, [sp, #8]
        ldr   a4, [sp, #12]

	; and adjust stack
	ldr   ip, [fp, #8]
        cmp   ip, #16
	movge ip, #16
        add   sp, sp, ip

	; call function
	mov   lr, pc
	ldr   pc, [fp, #28]

	; Remove the space we pushed for the args
	mov   sp, fp

	; Load a3 with the pointer to storage for the return value
	ldr   a3, [sp, #24]

	; Load a4 with the return type code 
	ldr   a4, [sp, #12]

	; If the return value pointer is NULL, assume no return value.
	cmp   a3, #0
	beq   epilogue

	; return INT
	cmp   a4, #FFI_TYPE_INT
	streq a1, [a3]
	beq   epilogue

	; return FLOAT
	cmp     a4, #FFI_TYPE_FLOAT
	stfeqs  f0, [a3]
	beq     epilogue

	; return DOUBLE or LONGDOUBLE
	cmp     a4, #FFI_TYPE_DOUBLE
	stfeqd  f0, [a3]

epilogue
        ldmfd sp!, {a1-a4, fp, pc}
