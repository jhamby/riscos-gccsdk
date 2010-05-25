@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	NAME	log10f
	DefSCLFncB log10f

	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	LOGS	f0, f0
	MOV	pc, lr

	DefSCLFncE log10f

	.end
