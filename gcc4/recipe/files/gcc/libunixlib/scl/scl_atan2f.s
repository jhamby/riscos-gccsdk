@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2003-2010 UnixLib Developers

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/elf-macros.s"
#include "internal/scl-macros.s"

	.text

	NAME	atan2f
	DefSCLFncB atan2f

	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	CMF	f0, #0
	CMFEQ	f1, #0
	POLNES	f0, f1, f0
	MOV	pc, lr

	DefSCLFncE atan2f

	.end
