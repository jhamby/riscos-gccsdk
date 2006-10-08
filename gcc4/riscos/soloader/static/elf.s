	.include	"../utils/elf-consts.h"
	.include	"../utils/swis.h"
	.include	"../utils/macros.h"
	.include	"../asm_config.h"
	.include	"error.h"

	.text

.set	BUFFER_SIZE,	256

@;* entry:
@;*  r0 = pointer to store file handle at
@;*  r1 = pointer to filename
@;*  r2 = buffer to hold header
@;* exit:
@;*  r0 = 0 if successful otherwise pointer to error block
	.global	open_ELF_file
open_ELF_file:
	STMFD	sp!,{r4-r6,lr}

	MOV	r5, r0
	MOV	r6, r2

	.if USE_SYMLINKS == 1

	SUB	sp, sp, #BUFFER_SIZE
	MOV	r0, r1
	MOV	r1, sp
	MOV	r2, #BUFFER_SIZE
	BL	resolve_links
	MOV	r1, sp

	.endif
	MOV	r0, #0x4F
	SWI	XOS_Find
	.if USE_SYMLINKS == 1
	ADD	sp, sp, #BUFFER_SIZE
	.endif
	BVS	99f

	STR	r0, [r5, #0]

	MOV	r1, r0
	MOV	r0, #3
	MOV	r2, r6
	MOV	r3, #E_HDRSIZE
	MOV	r4, #0
	SWI	XOS_GBPB
	BVS	99f

	LDRB	r1, [r6, #E_IDENT + EI_MAG0]
	TEQ	r1, #0x7F
	LDREQB	r1, [r6, #E_IDENT + EI_MAG1]
	TEQEQ	r1, #'E'
	LDREQB	r1, [r6, #E_IDENT + EI_MAG2]
	TEQEQ	r1, #'L'
	LDREQB	r1, [r6, #E_IDENT + EI_MAG3]
	TEQEQ	r1, #'F'
	MOVNE	r0, #ERROR_NOT_ELF
	BNE	98f

	LDRB	r1, [r6, #E_IDENT + EI_CLASS]
	TEQ	r1, #ELFCLASS32
	MOVNE	r0, #ERROR_NOT_32BIT
	BNE	98f

	LDRB	r1, [r6, #E_IDENT + EI_DATA]
	TEQ	r1, #ELFDATA2LSB
	MOVNE	r0, #ERROR_NOT_LITTLE
	BNE	98f

	LDRB	r1, [r6, #E_IDENT + EI_VERSION]
	TEQ	r1, #EV_CURRENT
	MOVNE	r0, #ERROR_NO_MATCH
	BNE	98f

	LDR	r1, [r6, #E_MACHINE]
	MOV	r1, r1, LSL#16
	MOV	r1, r1, ASR#16
	TEQ	r1, #EM_ARM
	MOVNE	r0, #ERROR_NOT_ARM
	BNE	98f
	MOV	r0, #0
99:
	LDMFD	sp!, {r4-r6,pc}

98:
	BL	error_blk_from_index
	LDMFD	sp!, {r4-r6,pc}

@;* entry:
@;*  r0 = pointer to ELF header
@;*  r1 = pointer to 2 word block to store program header size and
@;*       number of program headers
@;* exit:
@;*  r0 = size of buffer required to hold program headers
	.global	read_program_headers
read_program_headers:

	LDR	r2, [r0, #E_PHENTSIZE]
	LDR	r3, [r0, #E_PHNUM]
	MOV	r2, r2, LSL#16
	MOV	r2, r2, ASR#16
	STR	r2, [r1, #4]
	MOV	r3, r3, LSL#16
	MOV	r3, r3, ASR#16
	STR	r3, [r1, #0]
	MUL	r0, r2, r3	@;* program header array size = program header entry size * num program headers

	MOV	pc, lr

@;* entry:
@;*  r0 = ELF header
@;*  r1 = file handle
@;*  r2 = buffer to hold program headers
@;*  r3 = size of program headers
@;* exit:
@;*  r0 = 0 for success or pointer to error block
@;	.global	load_program_headers
@;load_program_headers:
@;	STMFD	sp!, {r4,lr}
@;
@;	LDR	r4, [r0,#E_PHOFF]
@;	MOV	r0, #3
@;	SWI	XOS_GBPB
@;	MOVVC	r0, #0
@;
@;	LDMFD	sp!, {r4,pc}

	.end
