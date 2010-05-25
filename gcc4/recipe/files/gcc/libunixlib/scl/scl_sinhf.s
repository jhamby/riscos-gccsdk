@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	NAME	sinhf
	DefSCLFncB sinhf

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1
	SUFS	f0, f0, f1
	DVFS	f0, f0, #2
	MOV	pc, lr

	DefSCLFncE sinhf

	.end
