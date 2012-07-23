@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	NAME	ldexpf
	DefSCLFncB ldexpf

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	FLTS	f1, a2
	MVFS	f2, #2
	POWS	f3, f2, f1
	MUFS	f0, f0, f3
	MOV	pc, lr

	DefSCLFncE ldexpf
#endif

	.end
