@;* main.c
@;* ELF loader
@;* by Lee Noar
@;* 28th June, 2004

	.include	"../utils/swis.h"

.equ	STATIC_ERROR_BASE,	0x0081dc10

	.text

@; entry:
@;  r0 = error index
@; Does not return
	.global	exit_with_error
exit_with_error:
	ADR	r1, error_array
	LDR	r0, [r1, r0, LSL#2]
	ADD	r0, r1, r0
	SWI	OS_GenerateError		@; Never returns

@; entry:
@;  r0 = error index
@; exit:
@;  r0 = ptr to error block
	.global	error_blk_from_index
error_blk_from_index:
	ADR	r1, error_array
	LDR	r0, [r1, r0, LSL#2]
	ADD	r0, r1, r0
	MOV	pc, lr

error_array:
	.word	error_bad_file1 - error_array
	.word	error_bad_file2 - error_array
	.word	error_bad_file3 - error_array
	.word	error_bad_file4 - error_array
	.word	error_bad_file5 - error_array
	.word	error_no_module - error_array
	.word	error_no_memory - error_array
	.word	error_not_exec - error_array
	.word	error_no_got - error_array
	.word	error_no_dl - error_array

error_bad_file1:
	.word	STATIC_ERROR_BASE + 0
	.asciz	"Not an ELF file"
	.align

error_bad_file2:
	.word	STATIC_ERROR_BASE + 1
	.asciz	"Not a 32 bit ELF file"
	.align

error_bad_file3:
	.word	STATIC_ERROR_BASE + 2
	.asciz	"Not a little endian ELF file"
	.align

error_bad_file4:
	.word	STATIC_ERROR_BASE + 3
	.asciz	"ELF version does not match loader"
	.align

error_bad_file5:
	.word	STATIC_ERROR_BASE + 4
	.asciz	"Not an ARM ELF program"
	.align

error_no_module:
	.word	STATIC_ERROR_BASE + 5
	.asciz	"Shared Object Manager module is not loaded"
	.align

error_no_memory:
	.word	STATIC_ERROR_BASE + 6
	.asciz	"Wimpslot not big enough, stack would be overwritten."
	.align

error_not_exec:
	.word	STATIC_ERROR_BASE + 7
	.asciz	"Not an ELF executable program"
	.align

error_no_got:
	.word	STATIC_ERROR_BASE + 8
	.asciz	"Unable to locate GOT of dynamic loader"
	.align

error_no_dl:
	.word	STATIC_ERROR_BASE + 9
	.asciz	"Unable to find requested Dynamic Loader."
	.align

	.end
