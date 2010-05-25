@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	NAME	hypotf
	DefSCLFncB hypotf

	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	MUFS	f0, f0, f0
	MUFS	f1, f1, f1
	ADFS	f0, f0, f1
	SQTS	f0, f0
	MOV	pc, lr

	DefSCLFncE hypotf

	.end
