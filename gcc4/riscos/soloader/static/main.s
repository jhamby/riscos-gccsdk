@;* main.c
@;* ELF loader (with support for dynamic loading)
@;* by Lee Noar
@;* Started 27th June, 2004
@;* Rewitten May, 2005
@;* License: GPL v2

@; Uses DDEUtils to pass program arguments which are picked up by Unixlib

	.include	"main.h"
	.include	"error.h"
	.include	"../utils/macros.h"
	.include	"../utils/elf-consts.h"
	.include	"../utils/at-consts.h"
	.include	"../utils/swis.h"
	.include	"../structs.h"

	.global	exit

	.text

.equ	STACK_SIZE,			16 * 1024

	.global	_start
_start:
	@; Read environment parameters
	@; On exit:
	@;   r0 = pointer to environment string
	@;   r1 = permitted RAM limit (highest address available + 1)
	@;   r2 = pointer to real time the program was started (5 bytes)
	SWI	XOS_GetEnv

	MOV	r6, r0
	MOV	r7, r1
	MOV	r8, r2

	@; Place static loader variables at top of wimp slot.
	SUB	vars, r7, #VARS_SIZE
	@; Zero global variables
	MOV	r0, vars
	MOV	r1, #VARS_SIZE
	MOV	r2, #0
	BL	fillmem

	ADRL	r1, swi_test_name
	SWI	XOS_SWINumberFromString
	MOVVS	r0, #ERROR_NO_MODULE
	BVS	exit_program_error

	STR	r7, [vars, #VARS_ENV_MEM_TOP]
	STR	r8, [vars, #VARS_ENV_PROG_TIME]

	@; Place relocated static loader below static loader variables.
	LDR	r7, =Image$$RO$$Base		@; Address of start of static loader code
	LDR	r8, =Image$$RW$$Base		@; Address of end of static loader code
	SUB	r8, r8, #0x9C			@; Remove size of old ELF header & Program headers
	SUB	r3, r8, r7			@; Size of static loader
	SUB	r11, vars, r3			@; r11 = Address of start of static loader's new position

	MOV	sp, r11
	SUB	sl, sp, #STACK_SIZE		@; set up a temporary stack limit until Unixlib kicks in

	@; Make sure we're still in valid memory
	CMP	sl, #0x8000
	MOVLE	r0, #ERROR_NO_MEMORY
	BLE	exit_program_error

	@; Relocate static loader
	SUB	r4, r11, r7			@; how far it's moving
2:
	LDR	r3, [r7], #4
	STR	r3, [r11], #4
	CMP	r7, r8
	BLT	2b

	ADR	r0, reloc_main
	ADD	r0, r0, r4			@; add on the amount relocated by
	MOV	pc, r0

	@; after relocation, execution jumps to here within the relocated copy of the code
reloc_main:

	@;* command line is of the form:
	@;*
	@;* loader ELF-prog-name arg1 ... argN
	@;*
	@;* where args are optional

	@; make a copy of the command line, taking the DDEUtils CL into account
	MOV	r0, r6
	BL	strlen
	MOV	r4, r0			@; store the length of the OS command line
	SWI	XDDEUtils_GetCLSize
	MOVVC	r5, r0			@; store the length of the DDEUtils command line
	MOVVS	r5, #0

	CMP	r5, #0			@; check whether there is a DDEUtils command line
	ADDGT	r0, r4, r5
	ADDGT	r0, r0, #2		@; allow for terminator and a space to seperate the two
	ADDLE	r0, r4, #1		@; allow for terminator
	SWI	XSOM_ALLOC
	MOV	r1, r6
	BL	strcpy
	STR	r0, [vars, #VARS_CMD_LINE_START]
	MOV	r6, r0

	CMP	r5, #0
	ADDGT	r0, r0, r4		@; find end of OS CL
	MOVGT	r2, #' '
	STRGTB	r2, [r0], #1		@; tack on a space
	SWIGT	XDDEUtils_GetCL		@; add DDEUtils CL to end of OS CL

	@; break the command line up into 3 sections

	@; find start of ELF program name
0:
	LDRB	r1, [r6], #1
	TEQ	r1, #0
	BEQ	usage
	TEQNE	r1, #' '
	BNE	0b

	@; terminate loader name
	MOV	r0, #0
	STRB	r0, [r6, #-1]

	@; r6 -> ELF program name
	STR	r6, [vars, #VARS_CMD_LINE_ELF_PROG]

	@; find end of ELF program name
1:
	LDRB	r1, [r6], #1
	TEQ	r1, #0
	TEQNE	r1, #' '
	BNE	1b

	@; terminate ELF program name
	@; r6 -> ELF program args (if any - if ELF program name 0 terminated then there are no args)
	TEQ	r1, #0
	MOVNE	r0, #0					@; if ELF program name 0 terminated, then there's
	STRNEB	r0, [r6, #-1]				@; no point in terminating again
	MOVNE	r0, r6
	STR	r0, [vars, #VARS_CMD_LINE_ELF_ARGS]	@; store address of args or NULL if there are none

	ADD	r0, vars, #VARS_ELF_FILE_HANDLE
	LDR	r1, [vars, #VARS_CMD_LINE_ELF_PROG]

	SUB	sp, sp, #E_HDRSIZE
	STR	sp, [vars, #VARS_ELF_HEADER]
	MOV	r2, sp
	BL	open_ELF_file
	TEQ	r0, #0
	BNE	exit_os_error

	@;* make sure it's an executable
	LDR	r0, [sp, #E_TYPE]
	MOV	r0, r0, lsl#16
	MOV	r0, r0, asr#16
	TEQ	r0, #ET_EXEC
	MOVNE	r0, #ERROR_NOT_EXEC
	BNE	exit_program_error

	@; Start loading ELF program
	LDR	r0, [vars, #VARS_ELF_HEADER]
	ADD	r1, vars, #VARS_ELF_PHNUM
	BL	read_program_headers
	SUB	sp, sp, r0
	STR	sp, [vars, #VARS_ELF_PROGRAM_HEADERS]

	@; Load program headers
	MOV	r3, r0
	MOV	r0, #3
	LDR	r1, [vars, #VARS_ELF_FILE_HANDLE]
	MOV	r2, sp
	LDR	r4, [vars, #VARS_ELF_HEADER]
	LDR	r4, [r4, #E_PHOFF]
	SWI	XOS_GBPB
	BVS	exit_os_error

	BL	load_ELF_program

	@;* close ELF program file
	LDR	r1, [vars, #VARS_ELF_FILE_HANDLE]
	TEQ	r1, #0
	MOVNE	r0, #0
	STRNE	r0, [vars, #VARS_ELF_FILE_HANDLE]
	SWINE	XOS_Find

	@;* retrieve original environment values
	@;* The ELF program name is passed on using OS_WriteEnv. The arguments are passed on
	@;* using DDEUtils.
	LDR	r0, [vars, #VARS_CMD_LINE_ELF_PROG]	@; get pointer to ELF program name
	LDR	r1, [vars, #VARS_ENV_PROG_TIME]
	SWI	XOS_WriteEnv
	LDR	r0, [vars, #VARS_CMD_LINE_ELF_ARGS]
	TEQ	r0, #0
	SWIEQ	XDDEUtils_SetCLSize
	BEQ	8f
	BL	strlen
	ADD	r0, r0, #1
	SWI	XDDEUtils_SetCLSize
	LDR	r0, [vars, #VARS_CMD_LINE_ELF_ARGS]
	SWI	XDDEUtils_SetCL
8:
	LDR	r0, [vars, #VARS_ELF_DYNAMIC_PROG_HDR]
	TEQ	r0, #0
	BEQ	7f
	LDR	r0, [r0, #P_FILESZ]
	TEQ	r0, #0
	BNE	dynamic_loader
7:	@; We only get to here if the executable is statically linked
 	@; Free command line memory now that we've finished with it
	LDR	r0, [vars, #VARS_CMD_LINE_START]
	SWI	XSOM_FREE

	LDR	r3, [vars, #VARS_ELF_HEADER]
	MOV	fp, #0			@; zero the frame pointer
	LDR	pc, [r3, #E_ENTRY]	@; never returns

dynamic_loader:
	@; Check if the required loader is already loaded by the system
	LDR	r0, [vars, #VARS_ELF_INTERP_ADDR]
	SWI	XSOM_HANDLE_FROM_NAME	@; This is a search of the global list
	MOVS	r4, r0
	BEQ	3f		@; A zero handle means the loader isn't known to the system yet

	@; Loader is already in the system
	@; r0 already handle of dynamic loader
	SUB	r1, sp, #OBJINFO_STRUCT_SIZE
	MOV	r2, #0		@; Global list search
	SWI	XSOM_QUERY_OBJECT
	LDR	r0, [r1, #OBJINFO_BASE_ADDR]
	STR	r0, [vars, #VARS_DL_BASE_ADDR]

	@; Calculate entry point address
	LDR	r1, [r0, #E_ENTRY]
	ADD	r1, r0, r1
	STR	r1, [vars, #VARS_DL_ENTRY_ADDR]

	@; r0 = base address
	ADD	r1, vars, #VARS_DL_PHNUM
	BL	read_program_headers
	B	4f
3:	@; The loader isn't in the system yet, so we have to load it first
	BL	load_dynamic_loader
	MOV	r4, #0		@; There's no handle yet
4:
	BL	register_client

	MOV	r0, r4
	BL	register_dynamic_loader

	@; At this point, the stack should be flat and sp should point to
	@; base of relocated static loader

	@; Find size of LD env string
	ADRL	r0, env_array_name
	MOV	r1, #0
	MOV	r2, #-1
	MOV	r3, #0
	MOV	r4, #0
	SWI	XOS_ReadVarVal
	CMP	r2, #0
	MOVEQ	r2, #1
	RSBNE	r2, r2, #0

	@; Round to mulitple of 4
	ADD	r2, r2, #3
	BIC	r2, r2, #3

	@; Place env string below relocated static loader.
	SUB	sp, sp, r2
	MOV	r5, sp

	@; Load env string
	ADRL	r0, env_array_name
	MOV	r1, sp
	MOV	r3, #0
	MOV	r4, #0
	SWI	XOS_ReadVarVal
	MOV	r0, #0
	STRB	r0, [r1, r2]

	@; Place arg strings below env string
	@; First determine how much space is required, delay filling array
	@; until argv is allocated.
	BL	calc_arg_data_size
	SUB	sp, sp, r0
	STR	sp, [vars, #VARS_ARG_DATA]

	@; Place auxillary data array below arg strings
	SUB	sp, sp, #AUX_ARRAY_SIZE
	STR	sp, [vars, #VARS_AUX_DATA]

	MOV	r0, sp
	BL	setup_aux_data

	@; An empty string is a special case - all we need is a terminator
	LDRB	r0, [r1, #0]
	TEQ	r0, #0
	MOVEQ	r3, #1
	BEQ	6f

	@; Place env array below auxillary data array
	@; Count number of variables in the environment
	MOV	r2, r1
	MOV	r3, #2	@; Number of variables = number of spaces + 1 (also add 1 for terminating NULL pointer)
5:
	LDRB	r0, [r2], #1
	TEQ	r0, #32
	ADDEQ	r3, r3, #1
	TEQ	r0, #0
	BNE	5b
6:
	SUB	sp, sp, r3, ASL#2 @; Allocate space to env array (number of variables converted to bytes)

	MOV	r0, sp
	MOV	r1, r5
	BL	setup_env_array

	LDR	r1, [vars, #VARS_ARGC]
	SUB	sp, sp, r1, ASL#2	@; Allocate space for argv (number of args converted to bytes)
	SUB	sp, sp, #8		@; Allow for terminating NULL & argc
	ADD	r0, sp, #4		@; Find start of argv
	BL	setup_argv
	STR	r1, [sp, #0]		@; Finally, store argc

	@; Free command line memory now that we've finished with it
	LDR	r0, [vars, #VARS_CMD_LINE_START]
	SWI	XSOM_FREE

	MOV	fp, #0				@; zero the frame pointer
	MOV	lr, pc
	LDR	pc, [vars, #VARS_DL_ENTRY_ADDR]	@; Enter dynamic loader - should not return

	SWI	XOS_Exit

@; entry:
@;  r0 = ptr to error block
exit_os_error:
	MOV	r2, r0
	LDR	r1, [vars, #VARS_ELF_FILE_HANDLE]
	TEQ	r1, #0
	MOVNE	r0, #0
	SWINE	XOS_Find

	MOV	r0, r2
	SWI	OS_GenerateError 		@; Never returns


@; entry:
@;  r0 = error index
exit_program_error:
	MOV	r2, r0

	LDR	r1, [vars, #VARS_ELF_FILE_HANDLE]
	TEQ	r1, #0
	MOVNE	r0, #0
	SWINE	XOS_Find

	MOV	r0, r2
	B	exit_with_error

load_dynamic_loader:
	STMFD	sp!, {r4-r11,lr}

	SUB	sp, sp, #E_HDRSIZE
	MOV	r11, sp			@; r11 = buffer for ELF header

	LDR	r0, [vars, #VARS_ELF_INTERP_ADDR]
	BL	strlen
	MOV	r4, r0			@; r4 = length of name of requested loader

	ADRL	r0, loader_path
	BL	strlen
	MOV	r5, r0			@; r5 = length of path
	ADD	r4, r4, r0
	ADD	r0, r4, #1

	SWI	XSOM_ALLOC
	MOV	r6, r0			@; r6 = pointer to DL name. Memory local to this
					@; function and should be freed before exit
	MOV	r0, r6
	ADRL	r1, loader_path
	BL	strcpy

	ADD	r0, r6, r5
	LDR	r1, [vars, #VARS_ELF_INTERP_ADDR]
	STR	r1, [vars, #VARS_DL_NAME]
	BL	strcpy

	ADD	r0, vars, #VARS_DL_FILE_HANDLE
	MOV	r1, r6
	MOV	r2, r11
	BL	open_ELF_file
	MOV	r2, r0	@; Save result whilst we free DL name memory

	MOV	r0, r6
	SWI	XSOM_FREE	@; Free DL name memory

	TEQ	r2, #0
	BEQ	0f

	@; Failed to open requested loader, exit with error
	MOV	r0, #ERROR_NO_DL
	B	exit_program_error
0:
	MOV	r0, r11
	ADD	r1, vars, #VARS_DL_PHNUM
	BL	read_program_headers
	MOV	r7, r0				@; r7 = size of program headers for DL
	SUB	sp, sp, r7			@; Use the stack to store headers

	@; Load program headers
	MOV	r0, #3
	LDR	r1, [vars, #VARS_DL_FILE_HANDLE]
	MOV	r2, sp
	MOV	r3, r7
	LDR	r4, [r11, #E_PHOFF]
	SWI	XOS_GBPB
	BVS	exit_os_error

	@; Add up the sizes of the PT_LOAD segments
	@; sp = program headers
	MOV	r8, sp
	LDR	r9, [vars, #VARS_DL_PHNUM]
	LDR	r10, [vars, #VARS_DL_PHENTSIZE]
	MOV	r0, #0
1:
	LDR	r1, [r8, #P_TYPE]
	TEQ	r1, #PT_LOAD
	LDREQ	r1, [r8, #P_MEMSZ]
	ADDEQ	r0, r0, r1
	ADD	r8, r8, r10
	SUBS	r9, r9, #1
	BNE	1b

	@; r0 now contains the total size of the loadable segments
	SWI	XSOM_ALLOC_LIB		@; allocate enough memory to hold all loadable segments
	BVS	exit_os_error
	STR	r0, [vars, #VARS_DL_BASE_ADDR]

	@; Load all segments of the dynamic loader
	MOV	r8, sp
	LDR	r9, [vars, #VARS_DL_PHNUM]
2:
	LDR	r0, [r8, #P_TYPE]
	TEQ	r0, #PT_LOAD
	BNE	3f			@; not a loadable segment

	MOV	r0, #3
	LDR	r1, [vars, #VARS_DL_FILE_HANDLE]
	LDR	r2, [vars, #VARS_DL_BASE_ADDR]
	LDR	r3, [r8, #P_VADDR]
	ADD	r2, r2, r3
	LDR	r3, [r8, #P_FILESZ]
	LDR	r4, [r8, #P_OFFSET]
	SWI	XOS_GBPB
	BVS	exit_os_error
3:
	ADD	r8, r8, r10
	SUBS	r9, r9, #1
	BNE	2b

	LDR	r1, [vars, #VARS_DL_FILE_HANDLE]
	TEQ	r1, #0
	MOVNE	r0, #0
	STRNE	r0, [vars, #VARS_DL_FILE_HANDLE]
	SWINE	XOS_Find

	@; Record entry point address of dynamic loader...
	LDR	r0, [r11, #E_ENTRY]
	LDR	r1, [vars, #VARS_DL_BASE_ADDR]
	ADD	r0, r1, r0		@; ...it's relative to the base address
	STR	r0, [vars, #VARS_DL_ENTRY_ADDR]

	ADD	sp, sp, r7
	ADD	sp, sp, #E_HDRSIZE

	LDMFD	sp!, {r4-r11,pc}

@;* entry:
@;*  r0 = program header
load_prog_segment:
	STMFD	sp!, {r4,r5,lr}

	MOV	r5, r0

	LDR	r1, [vars, #VARS_ELF_FILE_HANDLE]
	LDR	r2, [r5, #P_VADDR]
	LDR	r3, [r5, #P_MEMSZ]
	LDR	r4, [r5, #P_OFFSET]

	@; Make sure that loading this segment will not overwrite the stack
	ADD	r0, r2, r3
	CMP	r0, sl
	MOVGT	r0, #ERROR_NO_MEMORY
	BGT	exit_program_error

	MOV	r0, #3
	SWI	XOS_GBPB
	BVS	exit_os_error

	LDR	r4, [r5, #P_MEMSZ]
	LDR	r3, [r5, #P_FILESZ]
	LDR	r2, [r5, #P_VADDR]
	ADD	r0, r2, r4
	STR	r0, [vars, #VARS_FREE_MEMORY]	@; Keep track of where free memory starts
	SUBS	r1, r4, r3		@; find difference between memory size and file size,
	BEQ	99f			@; this is then the size of the bss area

	ADD	r0, r2, r3		@; find where the bss area starts
	MOV	r2, #0
	BL	fillmem			@; zero the bss area
99:
	LDMFD	sp!, {r4,r5,pc}


load_ELF_program:
	STMFD	sp!,{r4-r6,lr}

	LDR	r4, [vars, #VARS_ELF_PROGRAM_HEADERS]
	LDR	r5, [vars, #VARS_ELF_PHNUM]
	LDR	r6, [vars, #VARS_ELF_PHENTSIZE]
0:
	LDR	r0, [r4, #P_TYPE]

	TEQ	r0, #PT_INTERP
	LDREQ	r0, [r4, #P_VADDR]
	STREQ	r0, [vars, #VARS_ELF_INTERP_ADDR]
	BEQ	1f

	TEQ	r0, #PT_DYNAMIC
	STREQ	r4, [vars, #VARS_ELF_DYNAMIC_PROG_HDR]
	BEQ	1f

	TEQ	r0, #PT_LOAD
	MOVEQ	r0, r4
	BLEQ	load_prog_segment
1:
	ADD	r4, r4, r6
	SUBS	r5, r5, #1
	BNE	0b

	LDMFD	sp!,{r4-r6,pc}

@; entry:
@;  r0 = handle of dynamic loader if known to system, otherwise 0
register_dynamic_loader:
	STMFD	sp!,{r0-r6,r10,r11,lr}

	SUB	sp, sp, #OBJINFO_STRUCT_SIZE
	MOVS	r11, r0
	BEQ	0f
	MOV	r1, sp
	MOV	r2, #0	@; query global list
	SWI	XSOM_QUERY_OBJECT
	B	1f
0:
	LDR	r10, [vars, #VARS_DL_BASE_ADDR]
	STR	r10, [sp, #OBJINFO_BASE_ADDR]

	LDR	r0, [r10, #E_PHOFF]
	ADD	r4, r10, r0
	LDR	r5, [vars, #VARS_DL_PHNUM]
	LDR	r6, [vars, #VARS_DL_PHENTSIZE]
2:
	LDR	r0, [r4, #P_TYPE]
	TEQ	r0, #PT_LOAD
	BNE	3f

	LDR	r0, [r4, #P_FLAGS]
	TST	r0, #PF_W
	BEQ	4f

	LDR	r1, [r4, #P_VADDR]
	ADD	r1, r1, r10
	STR	r1, [sp, #OBJINFO_PUBLIC_RW_PTR]
	LDR	r3, [r4, #P_MEMSZ]
	STR	r3, [sp, #OBJINFO_RW_SIZE]
	LDR	r1, [r4, #P_FILESZ]
	STR	r1, [sp, #OBJINFO_BSS_OFFSET]
	SUB	r1, r3, r1	@; memsz - filesz = bss size
	STR	r1, [sp, #OBJINFO_BSS_SIZE]
	B	4f
3:
	TEQ	r0, #PT_DYNAMIC
	BNE	4f

	LDR	r1, [sp, #OBJINFO_PUBLIC_RW_PTR]
	LDR	r0, [r4, #P_VADDR]
	ADD	r0, r0, r10
	SUB	r3, r0, r1
	STR	r3, [sp, #OBJINFO_DYN_OFFSET]
	LDR	r3, [r4, #P_FILESZ]
	STR	r3, [sp, #OBJINFO_DYN_SIZE]

	@; Find the GOT of the loader
5:
	LDMIA	r0!, {r2,r3}
	TEQ	r2, #DT_NULL
	TEQNE	r2, #DT_PLTGOT
	BNE	5b

	TEQ	r2, #DT_NULL
	MOVEQ	r0, #ERROR_NO_GOT
	BEQ	exit_program_error

 	ADD	r3, r3, r10		@; Find absolute address of GOT
	SUB	r3, r3, r1		@; Find offset of GOT in R/W segment
	STR	r3, [sp, #OBJINFO_GOT_OFFSET]
4:
	ADD	r4, r4, r6
	SUBS	r5, r5, #1
	BNE	2b

	LDR	r0, [vars, #VARS_DL_NAME]
	STR	r0, [sp, #OBJINFO_NAME]

	LDR	r11, [vars, #VARS_DL_BASE_ADDR]
1:
	LDR	r0, [vars, #VARS_FREE_MEMORY]
	STR	r0, [sp, #OBJINFO_PRIVATE_RW_PTR]
	LDR	r1, [sp, #OBJINFO_RW_SIZE]
	ADD	r0, r0, r1
	STR	r0, [vars, #VARS_FREE_MEMORY]

	MOV	r0, #0
	STR	r0, [sp, #OBJINFO_FLAGS]

	MOV	r0, #SOM_REGISTER_LOADER
	MOV	r1, r11
	MOV	r2, sp
	SWI	XSOM_REGISTER_OBJECT
	BVS	exit_os_error

	@; Copy the R/W segment from the public copy to the private copy
	LDR	r0, [sp, #OBJINFO_PRIVATE_RW_PTR]
	LDR	r1, [sp, #OBJINFO_PUBLIC_RW_PTR]
	LDR	r2, [sp, #OBJINFO_RW_SIZE]
	BL	memcpy

	@; Zero the bss area
	LDR	r0, [sp, #OBJINFO_PRIVATE_RW_PTR]
	LDR	r1, [sp, #OBJINFO_BSS_OFFSET]
	ADD	r0, r0, r1
	LDR	r1, [sp, #OBJINFO_BSS_SIZE]
	MOV	r2, #0
	BL	fillmem

	ADD	sp, sp, #OBJINFO_STRUCT_SIZE
	LDMFD	sp!, {r0-r6,r10,r11,pc}

register_client:
	STMFD	sp!,{r0-r6}

	SUB	sp, sp, #OBJINFO_STRUCT_SIZE
	MOV	r0, #0
	STR	r0, [sp, #OBJINFO_BASE_ADDR]

	@; Go through the program headers extracting the info we need
	MOV	r4, #0x8000
	LDR	r0, [r4, #E_PHOFF]
	ADD	r4, r4, r0	@; r4 = addr of program headers
	LDR	r5, [vars, #VARS_ELF_PHNUM]
	LDR	r6, [vars, #VARS_ELF_PHENTSIZE]
0:

	LDR	r0, [r4, #P_TYPE]
	TEQ	r0, #PT_LOAD
	BNE	1f

	LDR	r0, [r4, #P_FLAGS]
	TST	r0, #PF_W
	BEQ	2f

	LDR	r1, [r4, #P_VADDR]
	STR	r1, [sp, #OBJINFO_PUBLIC_RW_PTR]
	STR	r1, [sp, #OBJINFO_PRIVATE_RW_PTR]
	LDR	r1, [r4, #P_MEMSZ]
	STR	r1, [sp, #OBJINFO_RW_SIZE]
	LDR	r2, [r4, #P_FILESZ]
	STR	r2, [sp, #OBJINFO_BSS_OFFSET]
	SUB	r1, r1, r2
	STR	r1, [sp, #OBJINFO_BSS_SIZE]
	B	2f
1:
	TEQ	r0, #PT_DYNAMIC
	BNE	2f

	LDR	r1, [sp, #OBJINFO_PUBLIC_RW_PTR]
	LDR	r0, [r4, #P_VADDR]
	SUB	r3, r0, r1
	STR	r3, [sp, #OBJINFO_DYN_OFFSET]
	LDR	r3, [r4, #P_FILESZ]
	STR	r3, [sp, #OBJINFO_DYN_SIZE]

	@; Find the GOT of the client
got_loop:
	LDMIA	r0!, {r2,r3}
	TEQ	r2, #DT_NULL
	TEQNE	r2, #DT_PLTGOT
	BNE	got_loop

	SUB	r3, r3, r1	@; Substract address of R/W segment to find offset
	STR	r3, [sp, #OBJINFO_GOT_OFFSET]
2:
	ADD	r4, r4, r6
	SUBS	r5, r5, #1
	BNE	0b

	LDR	r0, [vars, #VARS_CMD_LINE_ELF_PROG]
	STR	r0, [sp, #OBJINFO_NAME]
	MOV	r0, #0
	STR	r0, [sp, #OBJINFO_FLAGS]

	MOV	r0, #SOM_REGISTER_CLIENT
	MOV	r1, #0x8000
	MOV	r2, sp

	SWI	XSOM_REGISTER_OBJECT

	ADD	sp, sp, #OBJINFO_STRUCT_SIZE
	LDMFD	sp!,{r0-r6}
	MOV	pc, lr

@;* entry:
@;*  r0 = pointer to start of argv
setup_argv:
	STMFD	sp!, {r0-r2, r4, lr}

	MOV	r4, r0

	LDR	r0, [vars, #VARS_ARG_DATA] @; Pointer to place where arg string will be stored

	STR	r0, [r4], #4

	LDR	r1, [vars, #VARS_ARGV0]
	BL	strcpy

	MOV	r0, #0
	STR	r0, [r4, #0]	@; Terminate argv

	LDMFD	sp!, {r0-r2, r4, pc}

@;* exit:
@;*  r0 = size of memory block required to hold args (word aligned)
calc_arg_data_size:
	STMFD	sp!, {r1-r6,lr}

	MOV	r0, #1
	STR	r0, [vars, #VARS_ARGC]

	@; argv[0] is the program name, just use the leaf name
	LDR	r0, [vars, #VARS_CMD_LINE_ELF_PROG]

	MOV	r4, r0
	MOV	r5, #0
0:
	LDRB	r6, [r0], #1
	TEQ	r6, #'.'
	MOVEQ	r5, r0
	BEQ	0b
	TEQ	r6, #0
	BNE	0b

	TEQ	r5, #0
	MOVEQ	r0, r4	@; If there's no '.', then use whole name
	MOVNE	r0, r5
	STR	r0, [vars, #VARS_ARGV0]
 	BL	strlen

	@; Allow for terminator and word align
	ADD	r0, r0, #4
	BIC	r0, r0, #3

	ldmfd	sp!, {r1-r6,pc}

@;* exit:
@;*  r0 = pointer to env array
@;*  r1 = pointer to SOM allocated buffer holding environment string
setup_env_array:
	STMFD	sp!,{r0-r4,lr}

	MOV	r3, #0

	LDRB	r2, [r1, #0]
	TEQ	r2, #0
	BEQ	0f

	STR	r1, [r0], #4
1:
	TEQ	r2, #32
	STREQB	r3, [r1, #0]
	LDRB	r2, [r1, #1]!
	STREQ	r1, [r0], #4
	TEQ	r2, #0
	BNE	1b
0:
	STR	r3, [r0, #0]

	LDMFD	sp!,{r0-r4,pc}

@;* entry:
@;*  r0 = ptr to start of aux data array
@;*
@;* preserves all registers
setup_aux_data:
	STMFD	sp!,{r0-r3,lr}

	@;* set up auxillary data array

 	MOV	r3, r0

	MOV	r1, #AUX_ARRAY_SIZE
	MOV	r2, #0
	BL	fillmem

	MOV	r0, r3

	@;* store the ELF program's program header address
	MOV	r1, #AT_PHDR				@; # 0
	STR	r1, [r0], #4
	MOV	r14, #0x8000
	LDR	r1, [r14, #E_PHOFF]
	ADD	r1, r14, r1
	STR	r1, [r0], #4

	@;* store the number of program headers in the ELF program
	MOV	r1, #AT_PHNUM				@; # 1
	STR	r1, [r0], #4
	LDR	r1, [vars, #VARS_ELF_PHNUM]
	STR	r1, [r0], #4

	@;* store base address of dynamic loader
	MOV	r1, #AT_BASE				@; # 2
	STR	r1, [r0], #4
	LDR	r3, [vars, #VARS_DL_BASE_ADDR]
	STR	r3, [r0], #4

	@;* store entry point of ELF program
	MOV	r1, #AT_ENTRY				@; # 3
	STR	r1, [r0], #4
	LDR	r1, [vars, #VARS_ELF_HEADER]
	LDR	r2, [r1, #E_ENTRY]
	STR	r2, [r0], #4

	@;* Tell Dynamic Loader where free memory starts
	MOV	r1, #AT_DATA				@; # 4
	STR	r1, [r0], #4
	LDR	r1, [vars, #VARS_FREE_MEMORY]
	STR	r1, [r0], #4
	@;* store ids
@;	MOV	r2,#-1
@;	MOV	r1,#AT_UID
@;	STR	r1,[r0],#4
@;	STR	r2,[r0],#4
@;	MOV	r1,#AT_EUID
@;	STR	r1,[r0],#4
@;	STR	r2,[r0],#4
@;	MOV	r1,#AT_GID
@;	STR	r1,[r0],#4
@;	STR	r2,[r0],#4
@;	MOV	r1,#AT_EGID
@;	STR	r1,[r0],#4
@;	STR	r2,[r0],#4

	@;* null terminate list
	MOV	r1, #AT_NULL				@; # 5
	STR	r1, [r0], #4
	STR	r1, [r0, #0]

	LDMFD	sp!,{r0-r3,pc}

usage:
	SWI	XOS_WriteS
	.ascii	"Usage:\r\n"
	.asciz	"\tloader <elf-executable>\r\n"
	.align
	SWI	XOS_Exit

loader_path:
	.asciz	"DSOLib:lib."
default_loader:
	.asciz	"ld-riscos/so/1"
swi_test_name:
	.asciz	"XSOM_Alloc"
env_array_name:
	.asciz	"LD$Env"
env_default_string:
	.asciz	""

	.align

print_hex:
	STMFD	sp!,{r0-r2,r14}
	ADR	r1, 0f
	MOV	r2, #20
	SWI	0xd4
	MOV	r2, #0
	STRb	r2, [r1, #0]
	SWI	0x2

	LDMFD	sp!,{r0-r2,pc}
0:
	.word	0,0,0,0,0

	.end
