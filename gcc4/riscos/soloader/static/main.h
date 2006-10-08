@;* main.h
@;* ELF loader
@;* by Lee Noar
@;* 28th June, 2004

vars	.req	r12

.equ	DEBUG,	1

@; Global variable layout for static loader program

				.struct	0
VARS_ENV_MEM_TOP:		.skip	4	@; r1 from OS_GetEnv - top of wimp slot
VARS_ENV_PROG_TIME:		.skip	4	@; r2 from OS_GetEnv - time of program start

@; -----------------------------------------------------------
@; Variables associated with the ELF executable program

VARS_ELF_HEADER:		.skip	4
VARS_ELF_FILE_HANDLE:		.skip	4
VARS_ELF_PROGRAM_HEADERS:	.skip	4
@; The order of these 2 is important
VARS_ELF_PHNUM:			.skip	4	@; Number of program headers cached
VARS_ELF_PHENTSIZE:		.skip	4

VARS_ELF_DYNAMIC_PROG_HDR:	.skip	4	@; address of the dynamic segment header if any
VARS_ELF_INTERP_ADDR:		.skip	4

@; -----------------------------------------------------------
@; Variables associated with the dynamic loader

VARS_DL_HEADER:			.skip	4
VARS_DL_FILE_HANDLE:		.skip	4
@; The order of these 2 is important
VARS_DL_PHNUM:			.skip	4	@; Number of program headers in dynamic loader
VARS_DL_PHENTSIZE:		.skip	4	@; Size of each program header
VARS_DL_BASE_ADDR:		.skip	4
VARS_DL_ENTRY_ADDR:		.skip	4
VARS_DL_NAME:			.skip	4	@; Leaf name of the loader that was actually used

@; -----------------------------------------------------------

VARS_FREE_MEMORY:		.skip	4	@; Pointer to the start of free memory

						@; The 3 sections of a command line:
VARS_CMD_LINE_START:		.skip	4	@; Section 1; pointer to start of command line and also static loader name
VARS_CMD_LINE_ELF_PROG:		.skip	4	@; Section 2; pointer to ELF executable program name
VARS_CMD_LINE_ELF_ARGS:		.skip	4	@; Section 3; pointer to optional args (can be NULL)

VARS_AUX_DATA:			.skip	4	@; Data block that we need to pass to the dynamic loader
VARS_ARG_DATA:			.skip	4	@; Block of zero terminated strings that argv points to
VARS_ARGC:			.skip	4
VARS_ARGV0:			.skip	4	@; Pointer to leaf name of program that will be argv[0]
@;VARS_ERROR_BASE:		.skip	4
VARS_SIZE:
