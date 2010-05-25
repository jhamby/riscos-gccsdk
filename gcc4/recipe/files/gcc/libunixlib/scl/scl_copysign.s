@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	NAME	copysign
	DefSCLFncB copysign

	TST	a3, #0x80000000
	ORRNE	a1, a1, #0x80000000
	BICEQ	a1, a1, #0x80000000
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MOV	pc, lr

	DefSCLFncE copysign

	DefSCLFncAlias copysignl copysign

	.end
