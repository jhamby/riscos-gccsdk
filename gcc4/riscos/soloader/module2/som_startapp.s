@ som_startapp.s
@
@ Copyright 2007 GCCSDK Developers
@ Written by Lee Noar
@

.set	OS_Exit,		0x11
.set	XOS_ReadSysInfo,	0x000058 + (1<<17)
.set	XOS_TaskControl,	0x000078 + (1<<17)

@ This is too low level to be written in C.
@ Flatten the SVC stack, enter USR mode, set up registers
@ and jump to given entry point which could be the user
@ program or the Dynamic Loader.
@
@ Entry:
@  R0 = pointer to program entry point
@  R1 = sp
@  R2 = stack size
@  Should be called in SVC mode.
	.global	som_start_app
som_start_app:
	MOV	r4, r0
	MOV	r5, r1
	MOV	r6, r2

	@ Flatten the SVC stack.
	@ When OS_TaskControl is available, we use it by preference.
	@ RISC OS 6
	MOV	r0, #0
	SWI	XOS_TaskControl
	BVS	som_start_app_altflat

	MOV	lr, pc
	MOV	pc, r0
	B	som_start_app_gousr

som_start_app_altflat:
	@ RISC OS 4 and 5
	MOV	r0, #6
	MOV	r1, #0
	MOV	r2, #16
	SWI	XOS_ReadSysInfo
	BVS	som_start_app_fatalexit
	MOV	r13, r2

som_start_app_gousr:
	@ Change to USR mode
	TEQ	pc, pc
	TEQNEP	pc, #0
	MRSEQ	r0, CPSR
	BICEQ	r0, r0, #0xCF
	MSREQ	CPSR_c, r0
	MOV	r0, r0		@ NOP

	@ Initialise frame pointer
	MOV	fp, #0

	@ Set up stack pointer
	MOV	sp, r5

	@ Calculate stack limit
	SUB	sl, sp, r6

	@ Get return address in case user program returns.
	MOV	lr, pc

	@ Jump to user program entry point
	MOV	pc, r4

	@ In case the user program returns.
som_start_app_fatalexit:
	SWI	OS_Exit

	.end
