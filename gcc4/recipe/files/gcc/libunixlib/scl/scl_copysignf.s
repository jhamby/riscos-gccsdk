@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	NAME	copysignf
	DefSCLFncB copysignf

	TST	a2, #0x80000000
	ORRNE	a1, a1, #0x80000000
	BICEQ	a1, a1, #0x80000000
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MOV	pc, lr

	DefSCLFncE copysignf
#endif

	.end
