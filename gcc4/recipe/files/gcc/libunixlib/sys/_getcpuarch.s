/* Get ARM CPU Architecture.
   Copyright (c) 2007-2008 UnixLib Developers
   Written by John Tytgat.  */

#include "internal/asm_dec.s"

	.text

	@ Rough architecture determination in order to verify if the minimal
	@ CPU architecture limitation (imposed by the build options) is
	@ fulfilled at runtime.
	@ unsigned int __get_cpu_arch (void);
	@ Returns 0 for unknown CPU architecture.
	.global	__get_cpu_arch
	NAME	__get_cpu_arch
__get_cpu_arch:
	@ Read ARM ARM (B2.3) and ARM application note 99 if you want to
	@ understand some of the following bits.
	STMFD	R13!, {R14}
	MRS	a2, CPSR
	SWI	XOS_EnterOS
	MVNVS	a1, #0
	MRCVC	p15, 0, a1, c0, c0, 0	@ get CPU ID
	MSR	CPSR_c, a2

	@ Gets bits 15:12 which determines pre-ARM7, ARM7, or later than
	@ ARM7.
	MOV	a2, a1, LSR#12
	ANDS	a2, a2, #0xF
	BEQ	cpu_is_pre_arm7

	TEQ	a2, #7
	BEQ	cpu_is_arm7

	@ Later than ARM7:
	@ Select on the architecture bits:
	AND	a2, a1, #0xF<<16
	TEQ	a2, #1<<16		@ ARM architecture 4
	TEQNE	a2, #2<<16		@ ARM architecture 4T
	MOVEQ	a1, #4
	LDMEQFD	R13!, {PC}

	TEQ	a2, #3<<16		@ ARM architecture 5
	TEQNE	a2, #4<<16		@ ARM architecture 5T
	TEQNE	a2, #5<<16		@ ARM architecture 5TE
	TEQNE	a2, #6<<16		@ ARM architecture 5TEJ
	MOVEQ	a1, #5
	LDMEQFD	R13!, {PC}

	TEQ	a2, #7<<16		@ ARM architecture 6
	MOVEQ	a1, #6
	MOVNE	a1, #0			@ Currently unknown !
	LDMFD	R13!, {PC}

cpu_is_arm7:
	TST	a1, #1<<23
	MOVEQ	a1, #3			@ ARM7, architecture 3
	MOVNE	a1, #4			@ ARM7, architecture 4T	
	LDMFD	R13!, {PC}

cpu_is_pre_arm7:
	BIC	a1, a1, #0xF		@ Nuke revision bits
	LDR	a2, =0x41560300
	TEQ	a1, a2
	MOVEQ	a1, #2			@ ARM3, architecture 2
	LDMEQFD	R13!, {PC}
	LDR	a2, =0x41560600		@ ARM600, architecture 3
	TEQ	a1, a2
	LDRNE	a2, =0x41560610		@ ARM610, architecture 3
	TEQNE	a1, a2
	LDRNE	a2, =0x41560620		@ ARM630, architecture 3
	TEQNE	a1, a2
	MOVEQ	a1, #3			@ ARM6x0, architecture 3
	MOVNE	a1, #0			@ Unknown !
	LDMFD	R13!, {PC}
	DECLARE_FUNCTION __get_cpu_arch
