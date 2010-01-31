@ clock
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ clock_t clock (void)
	@ Return the elapsed processor time. The base time is
	@ arbitary, but doesn't change.
	.global	clock
	NAME	clock
clock:
	SWI	XOS_ReadMonotonicTime
	MOV	pc, lr
	DECLARE_FUNCTION clock

	.end
