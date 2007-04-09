@ Provide program entry and initialise the UnixLib world
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	@ Keep in sync with error_table.
.set	ERR_NO_MEMORY, 0
.set	ERR_NO_CALLASWI, 1
.set	ERR_NO_SUL, 2
.set	ERR_NO_FPE, 3
.set	ERR_UNRECOVERABLE, 4
.set	ERR_TOO_LOW_CPU_ARCH, 5

@ Constants for field offsets within a stack chunk
@ The stack is allocated in chunks, each chunk is a multiple of 4KB, and
@ contains a 24 byte header.
@ The next and prev fields form a doubly linked list of allocated chunks.
@ This is fairly similar to how the SCL's stack works
@ (see PRMs, 4-239 and Appendix C)
@
@  ______
@ |	 | +0
@ |Header|
@ |	 |
@ |______|
@ |	 | +24
@ |	 |
@ |	 | +536 <- sl
@ |	 |
@ |	 |
@ ________/
@/
@ |	 |
@ |______|
@	  +4096 * n  <- sp (initially)

.set	MAX_DA_NAME_SIZE, 32

	.text


	.weak	__program_name
	.weak	__dynamic_no_da
	.weak	__dynamic_da_name
	.weak	__dynamic_da_max_size

	@ RMEnsure the minimum version of the SharedUnixLibrary we need.
.set	SUL_MIN_VERSION, 107
rmensure1:
	.ascii	"RMEnsure SharedUnixLibrary 1.07 "
	.asciz	"RMLoad UnixLib:Modules.SharedULib"
rmensure2:
	.ascii	"RMEnsure SharedUnixLibrary 1.07 "
	.asciz	"RMLoad System:Modules.SharedULib"
	@ The exact error message is not important as it will get ignored.
rmensure3:
	.asciz	"RMEnsure SharedUnixLibrary 1.07 Error XYZ"
	.align

	.global	__main
	NAME	__main

__main:
	@ The dynamic loader passes, in a1, the address where free memory starts
	@ after it has grabbed what it needs. a2 & a3 come from crt1.o which is
	@ linked into the executable. They contain MEM_ROBASE and MEM_RWBASE
	@ respectively. a4 contains a pointer to main() which the shared library
	@ can jump to when ready to call the user's code.
 PICEQ "MOV	v1, a1"
 PICEQ "MOV	v2, a2"
 PICEQ "MOV	v3, a3"
 PICEQ "MOV	v5, a4"

 PICEQ "MOV	r0, lr"	@ Preserve the return address
 PICEQ "BL	__rt_load_pic"
	@ v4 is already in use below, so use v6 as the PIC register
 PICEQ "MOV	v6, v4"

	@ Read environment parameters
	@ On exit:
	@   a1 = pointer to environment string
	@   a2 = permitted RAM limit (highest address available + 1)
	@   a3 = pointer to real time the program was started (5 bytes)
	SWI	XOS_GetEnv

	@ struct_base is the start of our memory environment variables
	@ See the end of this file.  For the initialisation here, we
	@ will always use ip as the base register.
	LDR	ip, .L0			@=__ul_global
 PICEQ "LDR	ip, [v6, ip]"
	LDR	fp, .L0+4		@=__ul_memory
 PICEQ "LDR	fp, [v6, fp]"

	@ __unixlib_cli = pointer to command line
	STR	a1, [ip, #GBL_UNIXLIB_CLI]
	@ __image_rw_himem = permitted RAM limit
	STR	a2, [fp, #MEM_APPSPACE_HIMEM]

	@ For the shared library, fill in the linker generated values that
	@ are passed in by crt1.o. These are only known at runtime.
 PICEQ "STR	v2, [fp, #MEM_ROBASE]"
 PICEQ "STR	v1, [fp, #MEM_RWLOMEM]"
 PICEQ "STR	v3, [fp, #MEM_RWBASE]"

	@ Also store the pointer to the program's main function for calling later.
 PICEQ "STR	v5, [ip, #GBL_MAIN]"

	LDMIA	a3, {a1, a2}			@ Get time
	STR	a1, [ip, #GBL_TIME_LOW]		@ __time (low word)
	STR	a2, [ip, #GBL_TIME_HIGH]	@ __time (high word)

	MVNS	a1, #0			@ ensure a flag is set
	TEQ	pc, pc			@ EQ if in 32-bit mode, NE if 26-bit
	STREQ	a1, [ip, #GBL_32BIT]	@ __32bit

	@ Obtain the application space limit.  Note that this is different
	@ to the permitted RAM limit returned by OS_GetEnv.
	MOV	a1, #14
	MOV	a2, #0
	MOV	a3, #0
	SWI	XOS_ChangeEnvironment
	STR	a2, [fp, #MEM_APPSPACE_LIMIT]

	@ For a description of the memory layout of a UnixLib application
	@ see sys/brk.c.
	LDR	a2, [fp, #MEM_RWLOMEM]	@ __rwlomem
	STR	a2, [fp, #MEM_RWBREAK]	@ __break = __rwlomem
	STR	a2, [fp, #MEM_UNIXLIB_STACK_LIMIT] @ __stack_limit = __rwlomem

	@ The stack is allocated in chunks in the wimpslot, with the first
	@ 4KB chunk immediately below 'appspace_himem'.  We cannot place it
	@ in a dynamic area because GCC might generate trampolines.

	LDR	sp, [fp, #MEM_APPSPACE_HIMEM]

	@ 8 bytes are needed above the initial chunk
	@ for the stackalloc heap
	@ Reserve the top 4K for the signal handler stack
	SUB	sp, sp, #8
	STR	sp, [ip, #GBL_SIGNALHANDLER_SP]
	@ The signal handler stack cannot be extended, so we don't need to
	@ setup the chunk structure.
	SUB	sp, sp, #4096
	ADD	sl, sp, #512
	STR	sl, [ip, #GBL_SIGNALHANDLER_SL]

	SUB	a1, sp, #4096
	ADD	sl, a1, #512 + CHUNK_OVERHEAD

	SUB	a3, a1, #8
	@ __stackalloc_init needs a minimum of 8 bytes below the initial
	@ chunk for its heap - check this doesn't overlap the code section
	STR	a3, [fp, #MEM_UNIXLIB_STACK]	@ __stack = bottom of stack
	CMP	a3, a2
	MOVCC	a1, #ERR_NO_MEMORY
	BCC	__exit_with_error_num	@ No room for stack, exit.

	@ Initialise the first stack chunk
	MOV	a2, #0
	STR	a2, [a1, #CHUNK_PREV]
	STR	a2, [a1, #CHUNK_NEXT]
	STR	a2, [a1, #CHUNK_DEALLOC]
	STR	a2, [a1, #CHUNK_RETURN]
	LDR	a2, __stackchunk_magic_number
	STR	a2, [a1, #CHUNK_MAGIC]
	MOV	a2, #4096
	STR	a2, [a1, #CHUNK_SIZE]

	MOV	v1, ip		@ Temporary variable

	@ Check for the existence of CallASWI.  Terminate the process
	@ with an appropriate error message, if it doesn't exist.
	ADR	a1, error_no_callaswi
	MOV	a2, a1
	MOV	ip, #OS_GenerateError
	ORR	ip, ip, #X_Bit
	SWI	XOS_CallASWIR12
	@ If CallASWI is unknown then R0 will point to a different address,
	@ the error block for Unknown SWI
	TEQ	a2, a1
	MOVNE	a1, #ERR_NO_CALLASWI
	BNE	__exit_with_error_num

	MOV	ip, v1		@ Restore ip.

	LDR	a1, .L0+12	@=rmensure1	; If no SUL or not recent enough, ...
 PICEQ "LDR	a1, [v6, a1]"
	SWI	XOS_CLI			@ load it.
	LDR	a1, .L0+16	@=rmensure2	; Try a second location.
 PICEQ "LDR	a1, [v6, a1]"
	SWI	XOS_CLI			@
	LDRVC	a1, .L0+20	@=rmensure3	; If still not recent enough, ...
 PICEQ "LDRVC	a1, [v6, a1]"
	SWIVC	XOS_CLI			@ complain.
	MOVVS	a1, #ERR_NO_SUL
	BVS	__exit_with_error_num

	@ Use of DAs explicitly overridden if __dynamic_no_da is declared
	LDR	a1, ___dynamic_no_da
 PICEQ "LDR	a1, [v6, a1]"
	TEQ	a1, #0
	BNE	no_dynamic_area

	@ Checks for minimum ARM CPU architecture.  When it is an unknown CPU
	@ architecture, we continue without error.
	BL	__get_cpu_arch
	TEQ	a1, #0
	BEQ	unknown_cpu_arch
	CMP	a1, #__ARM_ARCH__
	MOVCC	a1, #ERR_TOO_LOW_CPU_ARCH
	BCC	__exit_with_error_num
unknown_cpu_arch:

	@ Check OS version for RISC OS 3.5 or more recent.
	MOV	a1, #129
	MOV	a2, #0
	MOV	a3, #255
	SWI	XOS_Byte
	BVS	no_dynamic_area
	CMP	a2, #165
	BCC	no_dynamic_area

	@ Area name is <program name> + "$Heap".  Figure now out what
	@ <program name> is.
	LDR	a1, ___program_name
 PICEQ "LDR	a1, [v6, a1]"
	TEQ	a1, #0
	LDREQ	a1, [ip, #GBL_UNIXLIB_CLI]	@ __cli
	LDRNE	a1, [a1, #0]	@ __program_name
	MOV	a2, a1
	@ Search for space or end of cli string
t01:
	LDRB	a3, [a1], #1
	TEQ	a3, #'.'
	MOVEQ	a2, a1
	TEQ	a3, #':'
	MOVEQ	a2, a1
	CMP	a3, #' '
	BGT	t01
	SUB	a1, a1, #1	@ back up to point at terminator char

	@ Use a maximum of (dynamic_area_name_end - dynamic_area_name_begin)
	@ characters from the program name
	LDR	v1, .L0+8	@=dynamic_area_name_end
 PICEQ "LDR	v1, [v6, v1]"
	MOV	a3, #0
	STRB	a3, [v1, #5]	@ Terminate the $Heap

	SUB	a4, a1, a2
	CMP	a4, #MAX_DA_NAME_SIZE
	SUBCS	v4, v1, #MAX_DA_NAME_SIZE
	SUBCC	v4, v1, a4

	MOV	a3, v4
t02:
	LDRB	a4, [a2], #1
	STRB	a4, [a3], #1
	CMP	a3, v1
	BCC	t02
	@ v4 => OS variable to check if Heap or DA needs to be used.

	@ Allow the user the option of setting their own name for the
	@ dynamic area used as a heap.	 If the variable __dynamic_da_name
	@ exists, then it must be a const char pointer (not an array) to an
	@ alternate name.  DAs are always used in this case, and there's
	@ no need to set a $Heap variable.

	@ The main use of this is when the binary is called !RunImage.
	@ e.g.:  const char * const __dynamic_da_name = "Nettle Heap";

	LDR	v5, ___dynamic_da_name
 PICEQ "LDR	v5, [v6, v5]"
	TEQ	v5, #0
	LDRNE	v5, [v5, #0]	@ get the actual string referred to
	BNE	t07

	@ Check environment variable for creating a DA
	MOV	a1, v4
	MOV	a3, #-1
	MOV	a4, #0
	MOV	v1, #0
	SWI	XOS_ReadVarVal
	CMP	a3, #0
	BGE	no_dynamic_area
	MOV	v5, v4

	@ v5 => DA area name
t07:
	@ Default max size for DA is 32MB
	MOV	v2, #32*1024*1024
	@ If __dynamic_da_max_size is defined, use its value as the max size
	LDR	v1, ___dynamic_da_max_size
 PICEQ "LDR	v1, [v6, v1]"
	TEQ	v1, #0
	LDRNE	v2, [v1]

	LDR	a1, .L0+8	@=dynamic_area_name_end
 PICEQ "LDR	a1, [v6, a1]"
	MOV	a3, #'M'
	STRB	a3, [a1, #5]	@ Change back to $HeapMax

	MOV	a1, v4
	MOV	a3, #4
	SUB	sp, sp, a3
	MOV	a2, sp
	MOV	a4, #0
	SWI	XOS_ReadVarVal	@ Read value of progname$HeapMax
	BVS	t08
	TEQ	v1, #1		@ Should be a number variable
	LDREQ	v2, [sp], #4
	MOVEQ	v2, v2, LSL#20	@ Convert MB into bytes

	@ v2 = size of DA area
t08:
	LDR	a1, .L0+8	@=dynamic_area_name_end
 PICEQ "LDR	a1, [v6, a1]"
	MOV	a3, #0
	STRB	a3, [a1, #5]	@ Change back to $Heap

	@ Create dynamic area
	MOV	a1, #0
	MOV	a2, #-1
	MOV	a3, #0		@ Set initial size of dynamic area to 0K
	MOV	a4, #-1
	MOV	v1, #0x80
	@ v2 is already set from above
	MOV	v3, #0
	MOV	v4, #0
	@ v5 is already set from above
	SWI	XOS_DynamicArea
	BVS	__exit_with_error_block	@ no DA, report and exit
	MOV	v1, a3				@ setup for deletion at exit
	STR	a2, [ip, #GBL_DYNAMIC_NUM]	@ __dynamic_num

	@ v1 is size left in area, a4 is start offset
	ADD	a1, v1, a4
	@ __lomem = start of dynamic area
	STR	a4, [fp, #MEM_DALOMEM]
	@ __break = end of used part of DA
	STR	a1, [fp, #MEM_DABREAK]
	@ __real_break = end of used part of DA
	STR	a1, [fp, #MEM_DALIMIT]

no_dynamic_area:
	MOV	fp, #0

	@ Find out whether we are executing within a TaskWindow or not.
	MOV	a1, #0
	SWI	XTaskWindow_TaskInfo
	MOVVS	a1, #0
	STR	a1, [ip, #GBL_TASKWINDOW]	@ __taskwindow

	@ Find out whether we are executing as a WIMP program or not.
	MOV	a1, #3		@ Text output mode, or desktop mode ?
	SWI	XWimp_ReadSysInfo
	MOVVS	a1, #0
	TEQ	a1, #0
	MOVNE	a1, #5		@ When desktop mode, get the taskhandle
	SWINE	XWimp_ReadSysInfo
	STR	a1, [ip, #GBL_TASKHANDLE]	@ __taskhandle

	@ Cache the system page size as this call can be slow.
	@ Used by getpagesize ().
	SWI	XOS_ReadMemMapInfo
	STR	a1, [ip, #GBL_UL_PAGESIZE]	@ __ul_pagesize

#ifndef __SOFTFP__
	@ Recognise the Floating Point facility by determining whether
	@ the SWI FPEmulator_Version actually exists (and works).
	@ We insist on having at least version 4.00.
	@ We don't care about this in soft-float builds
	SWI	XFPEmulator_Version
	MOVVS	a1, #0
	CMP	a1, #400	@ We want 4.00 or above so we can use SFM/LFM
	MOVCC	a1, #ERR_NO_FPE
	BCC	__exit_with_error_num
#endif

	@ Now we'll initialise the C library, then call the user program.

	MOV	a1, #SUL_MIN_VERSION
	@ SUL version we are expecting. Newer versions should be backwards
	@ compatible, if not they will return an error
	SWI	XSharedUnixLibrary_Initialise
	BVS	__exit_with_error_block
	STR	a1, [ip, #GBL_PROC]
	STR	a2, [ip, #GBL_UPCALL_HANDLER_ADDR]
	STR	a3, [ip, #GBL_UPCALL_HANDLER_R12]


	@ Initialise the malloc memory allocator.
	BL	__ul_malloc_init
	@ Read the current RISC OS environment handler state
	BL	__env_read
	@ Install the UnixLib environment handlers
	BL	__env_unixlib
	@ Initialise the stack heap in application space
	BL	__stackalloc_init

	@ Initialise the UnixLib library
	@ NOTE:	 No calls to brk, sbrk, or malloc should occur before
	@ calling this function.
	BL	__unixinit

	BL	_main
	@ C programs always terminate by calling exit.
	B	exit
.L0:
	WORD	__ul_global
	WORD	__ul_memory
	WORD	dynamic_area_name_end
	WORD	rmensure1
	WORD	rmensure2
	WORD	rmensure3
	DECLARE_FUNCTION __main

___program_name:
	WORD	__program_name
	DECLARE_OBJECT ___program_name
___dynamic_da_name:
	WORD	__dynamic_da_name
	DECLARE_OBJECT ___dynamic_da_name
___dynamic_da_max_size:
	WORD	__dynamic_da_max_size
	DECLARE_OBJECT ___dynamic_da_max_size
___dynamic_no_da:
	WORD	__dynamic_no_da
	DECLARE_OBJECT ___dynamic_no_da

	@ Can only be used to report fatal errors under certain conditions.
	@ Be sure that at this point the UnixLib environment handlers
	@ are not registered (anymore) because otherwise an OS_GenerateError
	@ straight jumps in our code again.
	@ Cfr. PRM 1-294 & 1-295.
	@ a1 contains an index to the error to print.
	NAME	__exit_with_error_num
__exit_with_error_num:
 PICEQ "BL	__rt_load_pic"
 PICEQ "LDR	a2, .L7"		@ error_table
 PICEQ "LDR	a2, [v4, a2]"

 PICNE "ADR	a2, error_table"
	CMP	a1, #(error_table_end - error_table) >> 2
	MOVCS	a1, #ERR_UNRECOVERABLE
	LDR	a1, [a2, a1, LSL #2]

__exit_with_error_block:
	SWI	OS_GenerateError
.L7:
	WORD	error_table
	DECLARE_FUNCTION __exit_with_error_num
	DECLARE_FUNCTION __exit_with_error_block

#if PIC
	.data
#endif

	@ Make sure there are no absolute addresses in shared library by
	@ placing error_table in read/write segment and addressing via GOT.
error_table:
	.word	error_no_memory
	.word	error_no_callaswi
	.word	error_no_sharedunixlib
	.word	error_no_fpe
	.word	error_unrecoverable_loop
	.word	error_too_low_cpu_arch
error_table_end:
	DECLARE_OBJECT error_table

#if PIC
	.text
#endif

error_no_callaswi:
	.word	SharedUnixLibrary_Error_NoCallASWI
	.asciz	"Module CallASWI is not present"
	.align
error_no_memory:
	.word	SharedUnixLibrary_Error_NotEnoughMem
	.asciz	"Insufficient memory for application"
	.align
error_no_sharedunixlib:
	.word	SharedUnixLibrary_Error_NotRecentEnough
	.ascii	"This application requires at least version "
	.asciz	"1.07 of the SharedUnixLibrary module"
	.align
error_no_fpe:
#ifndef __SOFTFP__
	.word	SharedUnixLibrary_Error_NoFPE
	.ascii	"This application requires version 4.00 "
	.asciz	"or later of the FPEmulator module"
	.align
#endif
error_unrecoverable_loop:
	.word	SharedUnixLibrary_Error_FatalError
	.asciz	"Unrecoverable fatal error detected"
	.align
error_too_low_cpu_arch:
	.word	SharedUnixLibrary_Error_TooLowCPUArch
	.ascii	"Build settings used to create this program require at least "
	.ascii	"an ARM Architecture " __ARM_ARCH_STR__ " CPU "
	.asciz	"at runtime."
	.align

	.global	__dynamic_area_exit
	NAME	__dynamic_area_exit
__dynamic_area_exit:
 PICNE "STMFD	sp!, {lr}"
 PICEQ "STMFD	sp!, {v4, lr}"
 PICEQ "BL	__rt_load_pic"

	LDR	a1, .L1			@=__dynamic_area_refcount
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a2, [a1]
	SUBS	a2, a2, #1		@ Decrement __dynamic_area_refcount,
	STR	a2, [a1]		@ and only remove the areas when the
 PICNE "LDMNEFD	sp!, {pc}"		@ count reaches zero
 PICEQ "LDMNEFD	sp!, {v4, pc}"

	BL	__munmap_all

	LDR	a2, .L1+4	@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a2, [a2, #GBL_DYNAMIC_NUM]
	MOV	a1, #1
	CMP	a2, #-1
	SWINE	XOS_DynamicArea
 PICEQ "LDMFD	sp!, {v4, pc}"
 PICNE "LDMFD	sp!, {pc}"
.L1:
	WORD	__dynamic_area_refcount
	WORD	__ul_global
	DECLARE_FUNCTION __dynamic_area_exit

	@ Restore original RISC OS environment handlers
	.global	__env_riscos
	NAME __env_riscos
__env_riscos:
 PICNE "STMFD	sp!, {v1, v2, lr}"
 PICEQ "STMFD	sp!, {v1, v2, v4, lr}"
 PICEQ "BL	__rt_load_pic"

	SWI	XOS_IntOff
	MOV	v1, #0
	LDR	v2, .L2	@=__calling_environment
 PICEQ "LDR	v2, [v4, v2]"
t04:
	MOV	a1, v1
	LDMIA	v2!, {a2, a3, a4}
	TEQ	a1, #0		@ Don't restore the himem limit or the
	TEQNE	a1, #14		@ appspace limit, as SUL handles these for us
	SWINE	XOS_ChangeEnvironment
	ADD	v1, v1, #1
	CMP	v1, #17		@  __ENVIRONMENT_HANDLERS
	BCC	t04
	SWI	XOS_IntOn
 PICNE "LDMFD	sp!, {v1, v2, pc}"
 PICEQ "LDMFD	sp!, {v1, v2, v4, pc}"
.L2:
	WORD	__calling_environment
	DECLARE_FUNCTION __env_riscos

	@ Get current environment handler setup
	NAME	__env_read
__env_read:
 PICNE "STMFD	sp!, {a1, a2, a3, a4, v1, v2, lr}"
 PICEQ "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v4, lr}"
 PICEQ "BL	__rt_load_pic"

	MOV	v1, #0
	LDR	v2, .L2	@=__calling_environment
 PICEQ "LDR	v2, [v4, v2]"
t05:
	MOV	a1, v1
	MOV	a2, #0
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment
	STMIA	v2!, {a2, a3, a4}
	ADD	v1, v1, #1
	CMP	v1, #17
	BCC	t05
 PICEQ "LDMFD	sp!, {a1, a2, a3, a4, v1, v2, v4, pc}"
 PICNE "LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}"
	DECLARE_FUNCTION __env_read

	@ Install the UnixLib environment handlers
	.global	__env_unixlib
	NAME	__env_unixlib
__env_unixlib:
 PICEQ "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v4, lr}"
 PICNE "STMFD	sp!, {a1, a2, a3, a4, v1, v2, lr}"
 PICEQ "BL	__rt_load_pic"

	SWI	XOS_IntOff

	MOV	v1, #0
 PICNE "ADR	v2, handlers"

 PICEQ "LDR	v2, .L3+12"		@ handlers
 PICEQ "LDR	v2, [v4, v2]"
t06:
	MOV	a1, v1
	LDR	a2, [v2], #4

	@ For the shared library, pass the GOT pointer to the handlers
	@ that are used by UnixLib.
 PICEQ "CMP	a2, #0"
 PICEQ "MOVEQ	a3, #0"
 PICEQ "MOVNE	a3, v4"
 PICNE "MOV	a3, #0"

	MOV	a4, #0
	TEQ	v1, #6		@ Error handler ?
	LDREQ	a4, .L3	@=__ul_errbuf
 PICEQ "LDREQ	a4, [v4, a4]"
	TEQ	v1, #7		@ CallBack handler ?
	LDREQ	a4, .L3+4	@=__cbreg
 PICEQ "LDREQ	a4, [v4, a4]"
	SWI	XOS_ChangeEnvironment
	ADD	v1, v1, #1
	CMP	v1, #16
	BCC	t06

	LDR	a4, .L3+8	@=__ul_global
 PICEQ "LDR	a4, [v4, a4]"
	MOV	a1, #16
	LDR	a2, [a4, #GBL_UPCALL_HANDLER_ADDR]
	LDR	a3, [a4, #GBL_UPCALL_HANDLER_R12]
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment

	SWI	XOS_IntOn
 PICEQ "LDMFD	sp!, {a1, a2, a3, a4, v1, v2, v4, pc}"
 PICNE "LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}"
.L3:
	WORD	__ul_errbuf
	WORD	__cbreg
	WORD	__ul_global
 PICEQ "WORD	handlers"
	DECLARE_FUNCTION __env_unixlib

#if PIC
	.data
#endif

	@ In the shared library, we can't have absolute addresses in the read only
	@ segment or else we'll end up with text relocations, therefore handlers
	@ has to go in the read/write segment.
handlers:
	.word	0		@ Memory limit
	.word	__h_sigill	@ Undefined instruction
	.word	__h_sigsegv0	@ Prefetch abort
	.word	__h_sigsegv1	@ Data abort
	.word	__h_sigbus	@ Address exception
	.word	0		@ Other exception
	.word	__h_error	@ Error
	.word	__h_cback	@ CallBack
	.word	0		@ Breakpoint
	.word	__h_sigint	@ Escape
	.word	__h_event	@ Event
	.word	__h_exit	@ Exit
	.word	0		@ Unknown SWI  FIXME: why not __h_sigsys ?
	.word	0		@ Exception registers
	.word	0		@ Application space
	.word	0		@ Currently active object
	.word	0		@ UpCall

#if PIC
	.text
#endif

	@ Same as the SCL's magic number, for compatibility in libgcc
__stackchunk_magic_number:
	.word	0xF60690FF

	@ Allocate 512 bytes more stack
	@ Round up to 4K as stack is only allocated in multiples of 4K
	.global	__rt_stkovf_split_small
	NAME	__rt_stkovf_split_small
__rt_stkovf_split_small:
	@ This must store the same regs as for __rt_stkovf_split_big
 PICNE "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, lr}"
 PICEQ "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, v4, lr}"

	MOV	v1, #4096
	B	stack_overflow_common


	@ Allocate stack to below <ip>
	.global	__rt_stkovf_split_big
	NAME	__rt_stkovf_split_big
__rt_stkovf_split_big:
	CMP	ip, sl		@ sanity check
	MOVPL	pc, lr
 PICNE "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, lr}"
 PICEQ "STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, v4, lr}"
	@ This must store the same regs as for __rt_stkovf_split_small
	@ and if changed, also update the 8*4 in the frame size
	@ calculation below

	SUB	v1, sp, ip	@ Get required frame size
 PICNE "ADD	v1, v1, #8*4"	@ Take account of the earlier STMFD (static library)
 PICEQ "ADD	v1, v1, #9*4"	@ Take account of the earlier STMFD (shared library)

	@ Size of chunk header and space for stack extension procedures
	ADD	v1, v1, #512+CHUNK_OVERHEAD
	ADD	v1, v1, #0xFF0	@round up to nearest 4KB
	ADD	v1, v1, #0x00F
	BIC	v1, v1, #0xFF0
	BIC	v1, v1, #0x00F
	@ Fall through !

	@ The rest is common to both split_small and split_big
	@ Thread-safe, other than the __stackalloc/free calls
	@ v1 = extra size needed.
stack_overflow_common:
 PICEQ "BL	__rt_load_pic"

	@ The signal handler stack chunk can't be extended.
	LDR	a1, .L4	@=__ul_global
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a1, [a1, #GBL_EXECUTING_SIGNALHANDLER]
	TEQ	a1, #0
	BNE	signalhandler_overflow

#if __UNIXLIB_EXTREMELY_PARANOID
	BL	__check_stack
#endif

	@ Find bottom of chunk
	SUB	v2, sl, #512+CHUNK_OVERHEAD
#if __UNIXLIB_STACK_CHECK_MAGIC
	LDR	a1, __stackchunk_magic_number
	LDR	a2, [v2, #CHUNK_MAGIC]
	TEQ	a1, a2
	BNE	stack_corrupt
#endif

	MOV	v3, sp	@ Store old sp
	@ We know there is enough stack to call stackalloc or stackfree, so
	@ sl just needs to be set to a value that won't cause a recursive
	@ stack overflow
	SUB	sl, sp, #512

	LDR	a1, [v2, #CHUNK_NEXT]
	TEQ	a1, #0
	@ There isn't a spare chunk, so we need to alloc a new one
	BEQ	alloc_new_chunk

#if __UNIXLIB_STACK_CHECK_MAGIC
	LDR	a3, __stackchunk_magic_number
	LDR	a4, [a1, #CHUNK_MAGIC]
	TEQ	a3, a4
	BNE	stack_corrupt
#endif
	LDR	a2, [a1, #CHUNK_SIZE]
	CMP	v1, a2
	MOVLS	v1, a2
	BLS	use_existing_chunk	@ Spare chunk is big enough, so use it

	@ Spare chunk is too small, so free it and alloc another one
	BL	__free_stack_chain

alloc_new_chunk:
	MOV	a1, v1
	BL	__stackalloc
	TEQ	a1, #0
	BEQ	raise_sigstak		@ No free memory

	LDR	a3, __stackchunk_magic_number
	STR	a3, [a1, #CHUNK_MAGIC]

	@ Add chunk into linked list
	STR	a1, [v2, #CHUNK_NEXT]
	STR	v2, [a1, #CHUNK_PREV]

use_existing_chunk:
	@ a1 contains the new/existing chunk to be used, v1 contains its size.
	MOV	a2, #0
	STR	a2, [a1, #CHUNK_NEXT]
	STR	v1, [a1, #CHUNK_SIZE]

	ADD	sp, a1, v1	@ New sp is the top of the new chunk
	ADD	sl, a1, #512+CHUNK_OVERHEAD

	LDR	a2, .L4+4	@=__stackfree
 PICEQ "LDR	a2, [v4, a2]"
	STR	a2, [a1, #CHUNK_DEALLOC] @ For compatibility with scl, in libgcc

	LDR	a2, [fp, #-4]	@ Load the return address
	STR	a2, [a1, #CHUNK_RETURN]	@ Remember it

	ADR	a3, __free_stack_chunk
	STR	a3, [fp, #-4]	@ Replace it with our chunk free procedure

 PICNE "LDMFD	v3, {a1, a2, a3, a4, v1, v2, v3, pc}"
 PICEQ "LDMFD	v3, {a1, a2, a3, a4, v1, v2, v3, v4, pc}"
.L4:
	WORD	__ul_global
	WORD	__stackfree
raise_sigstak:
	@ The 256 bytes left on the stack aren't enough for the signal
	@ handler so setup a larger stack
	MOV	v1, fp
	MOV	v2, sp
	MOV	v3, lr
	BL	__setup_signalhandler_stack

	ADR	v4,__rt_stkovf_split_small+12	@ Point at function name
	STMFD	sp!, {v1, v2, v3, v4}	@ Setup an APCS-32 stack frame to
	ADD	fp, sp, #12		@ link to the old stack

	MOV	a1, #0
	MOV	a2, #SIGSTAK
	BL	__unixlib_raise_signal
	B	_exit		@ __unixlib_raise_signal shouldn't return

#if __UNIXLIB_EXTREMELY_PARANOID || __UNIXLIB_STACK_CHECK_MAGIC
stack_corrupt_msg:
	.asciz	"***Fatal error: Stack corruption detected***"
	.align
stack_corrupt:
	ADR	a1, stack_corrupt_msg
	B	__unixlib_fatal
#endif

signalhandler_overflow_msg:
	.asciz	"***Fatal error: Stack overflow in signal handler***"
	.align
signalhandler_overflow:
	ADR	a1, signalhandler_overflow_msg
	B	__unixlib_fatal
	DECLARE_FUNCTION __rt_stkovf_split_small
	DECLARE_FUNCTION __rt_stkovf_split_big

#if __UNIXLIB_EXTREMELY_PARANOID
	@ Check every stack chunk in the chain to ensure it contains
	@ sensible values.
	@ v4 set by caller to UnixLib GOT pointer
	.global	__check_stack
	NAME	__check_stack
__check_stack:
	MOV	ip, sp
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, fp, ip, lr, pc}
	SUB	fp, ip, #4

	LDR	a1, .L5	@=__ul_global
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a1, [a1, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
	BLNE	__pthread_disable_ints
	LDR	a1, .L5+4	@=__ul_memory
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a2, [a1, #MEM_UNIXLIB_STACK]
	LDR	a3, [a1, #MEM_APPSPACE_HIMEM]
	LDR	a4, [a1, #MEM_ROBASE]

	SUB	a1, sl, #512+CHUNK_OVERHEAD
__check_stack_l1:
	CMP	a1, #0
	BEQ	__check_stack_l2
	BL	__check_chunk
	LDR	a1, [a1, #CHUNK_NEXT]
	B	__check_stack_l1

__check_stack_l2:
	SUB	a1, sl, #512+CHUNK_OVERHEAD
__check_stack_l3:
	CMP	a1, #0
	BNE	__check_stack_l4
	LDR	a1, .L5	@=__ul_global
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a1, [a1, #GBL_PTH_SYSTEM_RUNNING]
	CMP	a1, #0
	BLNE	__pthread_enable_ints
	LDMEA	fp, {a1, a2, a3, a4, v1, v2, fp, sp, pc}
.L5:
	WORD	__ul_global
	WORD	__ul_memory
__check_stack_l4:
	BL	__check_chunk
	LDR	a1, [a1, #CHUNK_PREV]
	B	__check_stack_l3

	@ a1 = chunk, a2 = __unixlib_stack,
	@ a3 = __image_rw_himem, a4 = __image_ro_base
__check_chunk:
	CMP	a1, a2
	BCC	stack_corrupt
	CMP	a1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_MAGIC]
	LDR	v2, __stackchunk_magic_number
	TEQ	v1, v2
	BNE	stack_corrupt

	LDR	v1, [a1, #CHUNK_SIZE]
	CMP	v1, #0x1000
	BCC	stack_corrupt
	SUB	v2, a3, a2
	CMP	v1, v2
	BHI	stack_corrupt

	LDR	v1, [a1, #CHUNK_NEXT]
	CMP	v1, #0
	BEQ	__check_chunk_l1
	CMP	v1, a2
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt
__check_chunk_l1:
	LDR	v1, [a1, #CHUNK_PREV]
	TEQ	v1, #0
	@ If the previous chunk is zero then dealloc and return
	@ may also be zero
	BEQ	__check_chunk_l2

	CMP	v1, a2
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_DEALLOC]
	TEQ	a1, a1
	TEQ	pc, pc	@ EQ if in a 32-bit mode, NE if 26-bit
	BICNE	v1, v1, #0xfc000003
	CMP	v1, a4
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_RETURN]
	TEQ	a1, a1
	TEQ	pc, pc	@ EQ if in a 32-bit mode, NE if 26-bit
	BICNE	v1, v1, #0xfc000003
	CMP	v1, a4
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt
__check_chunk_l2:
	MOV	pc, lr
	DECLARE_FUNCTION __check_stack
#endif @ __UNIXLIB_EXTREMELY_PARANOID


	@ Remove any unused stack chunks
	.global	__trim_stack
__trim_stack:
	SUB	a2, sl, #512 + CHUNK_OVERHEAD	@ Find bottom of chunk
	LDR	a1, [a2, #CHUNK_NEXT]
	TEQ	a1, #0
	MOVEQ	pc, lr
	MOV	a3, #0
	STR	a3, [a2, #CHUNK_NEXT]
	@ Falls through to __free_stack_chain

	@ Free a chain of stack chunks pointer to by a1
	.global	__free_stack_chain
__free_stack_chain:
	STMFD	sp!, {v1, lr}
__free_stack_chain_l1:
	@ Spare chunk is too small, so free it and alloc another one
#if __UNIXLIB_STACK_CHECK_MAGIC
	MOV	a2, #0	@ Invalidate the magic number
	STR	a2, [a1, #CHUNK_MAGIC]
#endif
	LDR	v1, [a1, #CHUNK_NEXT]
	BL	__stackfree
	MOVS	a1, v1
	BNE	__free_stack_chain_l1
	LDMFD	sp!, {v1, pc}
	DECLARE_FUNCTION __free_stack_chain
	DECLARE_FUNCTION __trim_stack


	.global	__free_stack_chunk
__free_stack_chunk:
	@ Called on exit from a function that caused stack extension
	@ a3-a4, ip, sl and lr can be corrupted, all others (including sp)
	@ must be preserved. GCC uses a1 & a2 to return 64bit results.
	@ Stack chunks are freed with one chunk of latency,
	@ so we need to free the previously unused chunk, not the chunk that
	@ has just been finished with. Therefore the chunk just finished
	@ with can be used as a stack for this function
#if __UNIXLIB_EXTREMELY_PARANOID
	BL	__check_stack
#endif
	SUB	sl, sl, #512+CHUNK_OVERHEAD	@ sl = start of current chunk
	LDR	ip, [sl, #CHUNK_SIZE]
	ADD	ip, sl, ip		@ ip = top of chunk
	STMFD	ip!, {a1, a2, fp, sp}	@ stack important regs
	MOV	sp, ip	@ set up sp
	@ sl remains at the bottom of the chunk, but there's 4K of space and
	@ __stackfree won't need more than 256 bytes of it so this is ok

#if __UNIXLIB_STACK_CHECK_MAGIC
	LDR	a1, __stackchunk_magic_number
	LDR	a2, [sl, #CHUNK_MAGIC]
	TEQ	a1, a2
	BNE	stack_corrupt
#endif

	LDR	a1, [sl, #CHUNK_NEXT]
	CMP	a1, #0
	BEQ	no_chunk_to_free

#if __UNIXLIB_STACK_CHECK_MAGIC
	MOV	a2, #0	@ Invalidate the magic number
	STR	a2, [a1, #CHUNK_MAGIC]
#endif
	BL	__stackfree
	MOV	a1, #0
	STR	a1, [sl, #CHUNK_NEXT]

no_chunk_to_free:
	LDMFD	sp, {a1, a2, fp, sp}	@ Restore important regs

	LDR	lr, [sl, #CHUNK_RETURN]	@ Get the real return address

	@ Set sl up correctly for the old stack chunk
	LDR	sl, [sl, #CHUNK_PREV]
	ADD	sl, sl, #512+CHUNK_OVERHEAD
	MOV	pc, lr
	DECLARE_FUNCTION __free_stack_chunk

	@ Globally used panic button.
	@ void __unixlib_fatal(const char *message) __attribute__ ((__noreturn__))
	@ Protects itself from recursive calling.
	.global	__unixlib_fatal
	NAME	__unixlib_fatal
__unixlib_fatal:
	@ We don't want to assume anything about the stack as the stack
	@ corruption detection routines will call this routine in case
	@ something is wrong.

	@ For shared library, preserve v4 & lr without using stack
 PICEQ "MOV	fp, v4"
 PICEQ "MOV	ip, lr"
 PICEQ "BL	__rt_load_pic"
 PICEQ "MOV	lr, ip"

	LDR	a4, .L6	@=__ul_global
 PICEQ "LDR	a4, [v4, a4]"
 PICEQ "MOV	v4, fp"
	LDR	sp, [a4, #GBL_SIGNALHANDLER_SP]
	MOV	fp, #0
	LDR	sl, [a4, #GBL_SIGNALHANDLER_SL]

	MOV	ip, sp
 PICNE "STMDB	sp!, {v1, fp, ip, lr, pc}"
 PICEQ "STMDB	sp!, {v1, v4, fp, ip, lr, pc}"
 PICEQ "BL	__rt_load_pic"

	SUB	fp, ip, #4

	@ We've been here before ? If so, we're looping in our fatal
	@ error handling.  As last resort to avoid an infinite loop
	@ we go for a straight OS_Exit scenario.  Anything better we
	@ can do ?
	MOV	a2, #1
	ADD	a3, a4, #GBL_PANIC_MODE
	SWP	a2, a2, [a3]
	TEQ	a2, #0
	BEQ	__unixlib_fatal_cont1

	BL	__env_riscos
	MOV	a1, #ERR_UNRECOVERABLE
	B	__exit_with_error_num

	@ a1 => NULL or NUL terminated message to print
__unixlib_fatal_cont1:
	MOVS	v1, a1
	BNE	__unixlib_fatal_got_msg
	__get_errno	v1, a1
	BL	strerror
	MOV	v1, a1

	@ a1 => NUL terminated message to print
__unixlib_fatal_got_msg:
	@ FIXME: in Wimp environment but not when in TaskWindow environment, we could pop
	@ up a Wimp_ReportError box.  Might help the "my program vanishes from the screen
	@ without any trace" case.
	SWI	XOS_NewLine
	MOV	a1, v1
	SWI	XOS_Write0
	SWI	XOS_NewLine
	MOV	a1, #1
	BL	_exit
	@ Should never return
 PICNE "LDMDB	fp, {v1, fp, sp, pc}"
 PICEQ "LDMDB	fp, {v1, v4, fp, sp, pc}"
.L6:
	WORD	__ul_global
	DECLARE_FUNCTION __unixlib_fatal

	.global	__unixlib_get_fpstatus
__unixlib_get_fpstatus:
#ifndef __SOFTFP__
	rfs	r0
#else
	mov	r0, #0
#endif
	mov	pc, lr
	DECLARE_FUNCTION __unixlib_get_fpstatus

	.global	__unixlib_set_fpstatus
__unixlib_set_fpstatus:
#ifndef __SOFTFP__
	wfs	r0
#endif
	mov	pc, lr
	DECLARE_FUNCTION __unixlib_set_fpstatus

	@ int __valid_address (const void *lower, const void *upper)
	@ Return non-zero value when address range <lower> - <upper> (excl)
	@ is a valid address range.
	.global	__valid_address
__valid_address:
	SWI	XOS_ValidateAddress
	@ If the C flag is clear then the address range is OK
	@ If the C flag is set then the address range is not OK.
	MOVCC	a1, #1
	MOVCS	a1, #0
	MOV	pc, lr
	DECLARE_FUNCTION __valid_address

	.section ".bss"

	@ This space is reserved for UnixLib to store the environment
	@ handlers of the calling application.
	@ Size: 17 handlers x 3 words x 4 bytes/word = 204 bytes
	.global	__calling_environment
__calling_environment:
	.space	204
	DECLARE_OBJECT __calling_environment

	.data

	.global	__dynamic_area_refcount
__dynamic_area_refcount:
	.word	1
	DECLARE_OBJECT __dynamic_area_refcount
dynamic_area_name_begin:
	.space	MAX_DA_NAME_SIZE
dynamic_area_name_end:
	.asciz	"$HeapMax"
	.align
	DECLARE_OBJECT dynamic_area_name_begin



@ Various data section variables can be grouped together under one
@ large structure.  The advantage of this is that functions
@ that reference several elements will only need to obtain the base
@ address of the structure once, therefore reducing the number of
@ load instructions required.
@
@ The intention is to move virtually all global variables into this
@ structure and patch all assembler and C source files to reference
@ them via __ul_global, thereby allowing us to drop the EXPORT directives
@ below.  If all assembler files are converted, then it would be possible
@ to redefine this structure in C and drop the definitions here.
@
@ Offsets to elements within the __ul_global structure are maintained in
@ unixlib/asm_dec.s and prefixed 'GBL_'.  If you change this structure
@ you must change that file.

	.global	__unixlib_cli		@ CLI from OS_GetEnv
	.global	__time	@ start time - 5 byte format
	.global	__taskwindow	@ non-zero if executing in a TaskWindow
	.global	__taskhandle	@ WIMP task handle, or zero if non WIMP task
	.global	__dynamic_num
	.global	__32bit	@ non-zero if executing in 32-bit mode
	.global	__panic_mode	@ non-zero when we're panicing.
	.global	__proc
	.global	__ul_pagesize	@ system page size
	.global	__upcall_handler_addr
	.global	__upcall_handler_r12
	.global	__pthread_return_address
	.global	__pthread_worksemaphore
	.global	__pthread_system_running
	.global	__pthread_callback_semaphore
	.global	__pthread_callback_missed
	.global	__pthread_num_running_threads
	.global	__executing_signalhandler
	.global	__signalhandler_sl
	.global	__signalhandler_sp

#if PIC
	.global	main
#endif

	@ This variable refers to the base address of the UnixLib
	@ global structure.
	.global	__ul_global
__ul_global:

	@ Altering this structure will require fixing __main.
__unixlib_cli:		.word	0				@ offset = 0
__time:		.word	0, 0	@ low word, high byte	; offset = 4
__notused1:		.word	0				@ offset = 12

__taskwindow:		.word	0				@ offset = 16
__taskhandle:		.word	0				@ offset = 20

__dynamic_num:		.word	-1				@ offset = 24
__old_u:		.word	0				@ offset = 28

__32bit:		.word	0				@ offset = 32

__panic_mode:		.word	0				@ offset = 36

__proc:		.word	0				@ offset = 40
__ul_pagesize:		.word	0				@ offset = 44
__upcall_handler_addr:	.word	0				@ offset = 48
__upcall_handler_r12:	.word	0				@ offset = 52

__pthread_return_address:
	.word	0						@ offset = 56
	DECLARE_OBJECT __pthread_return_address

__pthread_worksemaphore:
	.word	0						@ offset = 60
	DECLARE_OBJECT __pthread_worksemaphore

	@ Prevent a callback being set whilst servicing another callback
__pthread_callback_semaphore:
	.word	0						@ offset = 64
	DECLARE_OBJECT __pthread_callback_semaphore

	@ Global initialisation flag.  UnixLib internally uses this to
	@ test whether or not to use mutexes for locking critical structures.
__pthread_system_running:
	.word	0						@ offset = 68
	DECLARE_OBJECT __pthread_system_running

	@ Non zero if a callback occured when context switching was
	@ temporarily disabled
__pthread_callback_missed:
	.word	0						@ offset = 72
	DECLARE_OBJECT __pthread_callback_missed

	@ Number of running threads.
__pthread_num_running_threads:
	.word	1						@ offset = 76
	DECLARE_OBJECT __pthread_num_running_threads

	@ Non-zero if we are currently executing a signal handler
__executing_signalhandler:
	.word	0						@ offset = 80
	DECLARE_OBJECT __executing_signalhandler

	@ Stack limit for signal handlers
__signalhandler_sl:
	.word	0						@ offset = 84
	DECLARE_OBJECT __signalhandler_sl

	@ Stack pointer for signal handlers
__signalhandler_sp:
	.word	0						@ offset = 88
	DECLARE_OBJECT __signalhandler_sp

	.word	0	@ __mutex		offset = 92
	.word	0	@ malloc_state		offset = 96
	
#if PIC
	@ A pointer used by the shared library to call the program's main function.
.hidden main
main:
#endif
	.word	0	@ main			offset = 100

	DECLARE_OBJECT __ul_global


	@ In the shared library, these values cannot refer to any absolute
	@ addresses. They are set to zero at compile time and initialised
	@ at runtime where required.
	.global	__ul_memory
__ul_memory:
	.word	0			@ mutex			offset = 0
	.word	0			@ appspace_himem	offset = 4
	.word	0			@ unixlib_stack		offset = 8
#if PIC
	.word	0			@ robase		offset = 12
	.word	0			@ rwlomem		offset = 16
	.word	0			@ rwbase		offset = 20
#else
	.word	Image$$RO$$Base		@ robase		offset = 12
	.word	Image$$RW$$Limit	@ rwlomem		offset = 16
	.word	Image$$RW$$Base		@ rwbase		offset = 20
#endif
	.word	0			@ rwbreak		offset = 24
	.word	0			@ unixlib_stack_limit	offset = 28
	.word	0			@ dalomem		offset = 32
	.word	0			@ dabreak		offset = 36
	.word	0			@ dalimit		offset = 40
	.word	0			@ appspace_limit	offset = 44
	.word	0			@ old_himem		offset = 48
	DECLARE_OBJECT __ul_memory
