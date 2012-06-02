@
@ font-support.s
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"

	@ Adapted from generated OSLib code.
	.global rdn_Font_ScanString
rdn_Font_ScanString:
	MOV	IP, SP
	STMFD	SP!, {V1, V2, V3, V4, V5, LR}
	MOV	V5, R1			@ Save pointer to start of string
	LDMFD	IP, {R4, R5, R6, R7}
	SWI	0x600A1
	BVS	99f
	SUB	R1, R1, V5		@ Find index of split point
	LDR	LR, [SP, #40]
	STR	R1, [LR]
	LDR	LR, [SP, #44]
	STR	R3, [LR]
	LDR	LR, [SP, #48]
	STR	R4, [LR]
	LDR	LR, [SP, #52]
	STR	R7, [LR]
	MOV	R0, #0
99:	LDMFD	SP!, {V1, V2, V3, V4, V5, PC}
	DECLARE_FUNCTION rdn_Font_ScanString

	.end
