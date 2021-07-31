@
@ toolbox-support-asm.s
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"

@---------------------------------------------------------------------------
@
@ SWI Toolbox_ObjectMiscOp veneers with various register input/output combinations.
@
@---------------------------------------------------------------------------

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6 (uint flags, uint ObjectID, int method,
	@					       uint r3, uint r4, uint r5, uint r6)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6
rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6:
	MOV	ip, sp
	STMFD   sp!, {v1-v3,lr}

	LDR	r4, [ip, #0]
	LDR	r5, [ip, #4]
	LDR	r6, [ip, #8]
	SWI	0x64EC6
	MOVVC	r0, #0
	LDMFD	sp!, {v1-v3, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4R5R6

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3GetR0 (uint flags, uint ObjectID, int method, uint r3,
	@ 					      uint *r0_return)
	.global rdn_Toolbox_ObjectMiscOp_SetR3GetR0
rdn_Toolbox_ObjectMiscOp_SetR3GetR0:
	MOV	ip, sp
	STR	lr, [sp, #-4]!

	SWI	0x64EC6
	BVS	99f
	LDR	lr, [ip, #0]
	STR	r0, [lr]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3GetR0

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1 (uint flags, uint ObjectID, int method, uint r3,
	@ 						uint *r0_return, uint *r1_return)
	.global rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1
rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1:
	MOV	ip, sp
	STR	lr, [sp, #-4]!

	SWI	0x64EC6
	BVS	99f
	LDR	lr, [ip, #0]
	STR	r0, [lr]
	LDR	lr, [ip, #4]
	STR	r1, [lr]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1R2 (uint flags, uint ObjectID, int method, uint CmpID,
	@ 				  uint *r0_return, uint *r1_return, uint *r2_return)
	.global rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1R2
rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1R2:
	MOV	ip, sp
	STR	lr, [sp, #-4]!

	SWI	0x64EC6
	BVS	99f
	LDR	lr, [ip, #0]
	STR	r0, [lr]
	LDR	lr, [ip, #4]
	STR	r1, [lr]
	LDR	lr, [ip, #8]
	STR	r2, [lr]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3GetR0R1R2

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5 (uint flags, uint WindowID, int method, uint r3,
	@ 			 	  void *r4, int r5, int *r5_out)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5
rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5:
	MOV	ip, sp
	STMFD   sp!, {v1, v2, lr}

	LDR	r4, [ip, #0]
	LDR	r5, [ip, #4]
	SWI	0x64EC6
	BVS	99f
	LDR	lr, [sp, #20]
	STR	r5, [lr]
	MOV	r0, #0
99:	LDMFD	sp!, {v1, v2, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4R5GetR5

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3 (uint flags, uint ObjectID, int method, uint r3)
	.global rdn_Toolbox_ObjectMiscOp_SetR3
rdn_Toolbox_ObjectMiscOp_SetR3:
	MOV	ip, lr

	SWI	0x64EC6
	MOVVC	r0, #0
	MOV	pc, ip
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4GetR4 (uint flags, uint WindowID, int method,
	@ 			 			char *buffer, int buffer_size, int *used)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4GetR4
rdn_Toolbox_ObjectMiscOp_SetR3R4GetR4:
	MOV	ip, sp
	STMFD   sp!, {v1, lr}

	LDR	r4, [ip, #0]
	SWI	0x64EC6
	BVS	99f
	LDR	lr, [sp, #12]
	STR	r4, [lr]
	MOV	r0, #0
99:	LDMFD	sp!, {v1, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4GetR4

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4GetR0R4 (uint flags, uint WindowID, int method,
	@ 			 			  char *r3, int r4,
	@ 			 			  int *r0_out, int *r4_out)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4GetR0R4
rdn_Toolbox_ObjectMiscOp_SetR3R4GetR0R4:
	MOV	ip, sp
	STMFD   sp!, {v1, lr}

	LDR	r4, [ip, #0]
	SWI	0x64EC6
	BVS	99f
	LDR	lr, [ip, #4]
	STR	r0, [lr]
	LDR	lr, [ip, #8]
	STR	r4, [lr]
	MOV	r0, #0
99:	LDMFD	sp!, {v1, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4GetR0R4

	@ IntPtr rdn_Toolbox_ObjectMiscOp_GetR0 (uint flags, uint ObjectID, int method, uint *r0_return)
	.global rdn_Toolbox_ObjectMiscOp_GetR0
rdn_Toolbox_ObjectMiscOp_GetR0:
	STR	lr, [sp, #-4]!

	MOV	ip, r3
	SWI	0x64EC6
	BVS	99f
	STR	r0, [ip]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_GetR0

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4 (uint flags, uint ObjectID, int method, uint r3, uint r4)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4
rdn_Toolbox_ObjectMiscOp_SetR3R4:
	MOV	ip, sp
	STMFD   sp!, {v1, lr}

	LDR	r4, [ip, #0]
	SWI	0x64EC6
	MOVVC	r0, #0
	LDMFD	sp!, {v1, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4

	@ IntPtr rdn_Toolbox_ObjectMiscOp_SetR3R4R5 (uint flags, uint ObjectID, int method, uint r3, uint r4)
	.global rdn_Toolbox_ObjectMiscOp_SetR3R4R5
rdn_Toolbox_ObjectMiscOp_SetR3R4R5:
	MOV	ip, sp
	STMFD   sp!, {v1, v2, lr}

	LDR	r4, [ip, #0]
	LDR	r5, [ip, #4]
	SWI	0x64EC6
	MOVVC	r0, #0
	LDMFD	sp!, {v1, v2, pc}
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_SetR3R4R5

	@ IntPtr rdn_Toolbox_ObjectMiscOp_GetR0R1 (uint flags, uint ObjectID, int method,
	@ 					   uint *r0_return, uint *r1_return)
	.global rdn_Toolbox_ObjectMiscOp_GetR0R1
rdn_Toolbox_ObjectMiscOp_GetR0R1:
	MOV	ip, sp
	STR	lr, [sp, #-4]!

	SWI	0x64EC6
	BVS	99f
	STR	r0, [r3]
	LDR	lr, [ip, #0]
	STR	r1, [lr]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_GetR0R1

	@ IntPtr rdn_Toolbox_ObjectMiscOp_GetR0R1R2 (uint flags, uint ObjectID, int method,
	@ 					   int *r0_return, int *r1_return, int *r2_return)
	.global rdn_Toolbox_ObjectMiscOp_GetR0R1R2
rdn_Toolbox_ObjectMiscOp_GetR0R1R2:
	MOV	ip, sp
	STR	lr, [sp, #-4]!

	SWI	0x64EC6
	BVS	99f
	STR	r0, [r3]
	LDR	lr, [ip, #0]
	STR	r1, [lr]
	LDR	lr, [ip, #4]
	STR	r2, [lr]
	MOV	r0, #0
99:	LDR	pc, [sp], #4
	DECLARE_FUNCTION rdn_Toolbox_ObjectMiscOp_GetR0R1R2

	.end
