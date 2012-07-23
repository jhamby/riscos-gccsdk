@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2012 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

#if !defined(__SOFTFP__) && !defined(__VFP_FP__)
	NAME	tanhf
	DefSCLFncB tanhf

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1

	SUFS	f2, f0, f1
	ADFS	f3, f0, f1
	DVFS	f0, f2, f3
	MOV	pc, lr

	DefSCLFncE tanhf
#endif

	.end
