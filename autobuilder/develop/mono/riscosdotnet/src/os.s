@
@ os.s - OS SWI wrappers
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"
#include "swis.h"

	.text

	@ void rdn_OS_Plot (int code, int x, int y)
	NAME	rdn_OS_Plot
	.global rdn_OS_Plot
rdn_OS_Plot:
	SWI	SWI_OS_Plot
	MOV	pc, lr
	DECLARE_FUNCTION rdn_OS_Plot

	.end
