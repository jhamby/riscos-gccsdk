@ clock
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	@ clock_t clock (void)
	@ Return the elapsed processor time. The base time is
	@ arbitary, but doesn't change.
	.global	clock
	NAME	clock
clock:
	SWI	XOS_ReadMonotonicTime
	MOV	pc, lr

	.end
