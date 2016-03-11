@ Determine the capabilities of the CPU and allow callers to enquire
@ what they are.
@ Copyright (c) 2016 UnixLib Developers.
@ Written by Lee Noar.

#include "internal/asm_dec.s"

	.text

@ Determine what the CPU capabilities are, and record them.
@ Called early on during program initialisation in _syslib.s.
	.global __probe_cpu_caps
	NAME	__probe_cpu_caps
__probe_cpu_caps:
	MOV	r0, #0
	SWI	XOS_PlatformFeatures
	MOVVS	r0, #0

	MOV	r2, #0
	@ The OS flag is set to 1 to indicate _no_ SWP instruction available (for
	@ backwards compatibility). We reverse it here and set our flag to 1 to
	@ indicate that SWP _is_ available.
	TST	r0, #1<<11			@ SWP{B} not available
	ORREQ	r2, r2, #__CPUCAP_HAVE_SWP
	TST	r0, #1<<12			@ LDREX/STREX available
	ORRNE	r2, r2, #__CPUCAP_HAVE_REX
	TST	r0, #1<<13			@ LDREX/STREX{B/H/D} available
	ORRNE	r2, r2, #__CPUCAP_HAVE_REXBHD

 PICEQ "LDR	r0, =__GOTT_BASE__"
 PICEQ "LDR	r0, [r0, #0]"
 PICEQ "LDR	r0, [r0, #__GOTT_INDEX__]"	@ r0 = GOT ptr
	LDR	r1, .L0				@ =__ul_global
 PICEQ "LDR	r1, [r0, r1]"
	STR	r2, [r1, #GBL_CPU_FLAGS]

	MOV	pc, lr
.L0:
	WORD	__ul_global
	DECLARE_FUNCTION __probe_cpu_caps

	.global	__cpucap_have_swp
	NAME	__cpucap_have_swp
__cpucap_have_swp:
 PICEQ "LDR	r0, =__GOTT_BASE__"
 PICEQ "LDR	r0, [r0, #0]"
 PICEQ "LDR	r0, [r0, #__GOTT_INDEX__]"	@ r0 = GOT ptr
	LDR	r1, .L0				@ =__ul_global
 PICEQ "LDR	r1, [r0, r1]"
	LDR	r0, [r1, #GBL_CPU_FLAGS]
	TST	r0, #__CPUCAP_HAVE_SWP
	MOVEQ	r0, #0
	MOVNE	r0, #1

	MOV	pc, lr
	DECLARE_FUNCTION __cpucap_have_swp

@ Return 1 if LDREX/STREX instructions are available on the CPU, otherwise 0.
	.global	__cpucap_have_rex
	NAME	__cpucap_have_rex
__cpucap_have_rex:
 PICEQ "LDR	r0, =__GOTT_BASE__"
 PICEQ "LDR	r0, [r0, #0]"
 PICEQ "LDR	r0, [r0, #__GOTT_INDEX__]"	@ r0 = GOT ptr
	LDR	r1, .L0				@ =__ul_global
 PICEQ "LDR	r1, [r0, r1]"
	LDR	r0, [r1, #GBL_CPU_FLAGS]
	TST	r0, #__CPUCAP_HAVE_REX
	MOVEQ	r0, #0
	MOVNE	r0, #1

	MOV	pc, lr
	DECLARE_FUNCTION __cpucap_have_rex

@ Return 1 if LDREX/STREX instructions that work on non-32 bit data sizes
@ are available on the CPU, otherwise 0.
	.global	__cpucap_have_rexbhd
	NAME	__cpucap_have_rexbhd
__cpucap_have_rexbhd:
 PICEQ "LDR	r0, =__GOTT_BASE__"
 PICEQ "LDR	r0, [r0, #0]"
 PICEQ "LDR	r0, [r0, #__GOTT_INDEX__]"	@ r0 = GOT ptr
	LDR	r1, .L0				@ =__ul_global
 PICEQ "LDR	r1, [r0, r1]"
	LDR	r0, [r1, #GBL_CPU_FLAGS]
	TST	r0, #__CPUCAP_HAVE_REXBHD
	MOVEQ	r0, #0
	MOVNE	r0, #1

	MOV	pc, lr
	DECLARE_FUNCTION __cpucap_have_rexbhd
