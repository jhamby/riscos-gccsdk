@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	NAME	sinf
	DefSCLFncB sinf

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	SINS	f0, f0
	MOV	pc, lr

	DefSCLFncE sinf
#endif

	.end
