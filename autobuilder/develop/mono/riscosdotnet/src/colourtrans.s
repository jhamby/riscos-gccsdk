@
@ colourtrans.s - ColourTrans SWI wrappers
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"
#include "swis.h"

	.text
	@ void rdn_ColourTrans_SetGCOL (unsigned int gcol_action,
	@			    unsigned int palette)
	NAME	"rdn_ColourTrans_SetGCOL"
	.global	rdn_ColourTrans_SetGCOL
rdn_ColourTrans_SetGCOL:
	STR	r4, [sp, #-4]!

	MOV	r4, r0
	MOV	r0, r1
	MOV	r3, #0
	SWI	SWI_ColourTrans_SetGCOL

	LDR	r4, [sp], #4
	MOV	pc, lr
	DECLARE_FUNCTION rdn_ColourTrans_SetGCOL

	.end
