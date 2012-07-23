@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	NAME	modff
	DefSCLFncB modff

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	RNDSZ	f1, f0
	STFS	f1, [a2, #0]
	SUFS	f0, f0, f1
	MOV	pc, lr

	DefSCLFncE modff
#endif

	.end
