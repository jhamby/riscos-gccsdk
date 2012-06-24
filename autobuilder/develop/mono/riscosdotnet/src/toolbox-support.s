@
@ toolbox-support.s
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"

	@ os_error *rdn_Toolbox_Initialise (unsigned int flags,
	@ 				    unsigned int wimp_version,
	@ 				    int *message_list,
	@ 				    int *event_list,
	@ 				    char *dir_name,
	@ 				    int *version_out,
	@ 				    int *task_out,
	@ 				    int *sprite_out)
	@ Based on the output of OSLib.
	.global	rdn_Toolbox_Initialise
rdn_Toolbox_Initialise:
	MOV	ip, sp
	STMFD   sp!, {v1, v2, v3, lr}
	LDR	r4, [ip, #0]

	PIC	ip

	LDR	r5, .L0+4
 PICEQ "LDR	r5, [ip, r5]"

	LDR	r6, .L0
 PICEQ "LDR	r6, [ip, r6]"

	SWI	0x64ECF
	BVS	99f
	LDR	lr, [sp, #28]
	STR	r0, [lr]
	LDR	lr, [sp, #32]
	STR	r1, [lr]
	LDR	lr, [sp, #36]
	STR	r2, [lr]
	MOV	r0, #0
99:
	LDMFD   sp!, {v1, v2, v3, pc}
.L0:
	WORD	id_block
	WORD	messtrans_fd
	DECLARE_FUNCTION rdn_Toolbox_Initialise

	@ IntPtr rdn_Toolbox_GetIDBlock ()
	.global	rdn_Toolbox_GetIDBlock
rdn_Toolbox_GetIDBlock:
	PIC	ip

	LDR	r1, .L1
 PICEQ "LDR	r1, [ip, r1]"

	MOV	r0, r1

	MOV	pc, lr
.L1:
	WORD	id_block
	DECLARE_FUNCTION rdn_Toolbox_GetIDBlock

	@ IntPtr rdn_Toolbox_GetMessTransFD ()
	.global	rdn_Toolbox_GetMessTransFD
rdn_Toolbox_GetMessTransFD:
	PIC	ip

	LDR	r1, .L2
 PICEQ "LDR	r1, [ip, r1]"
	MOV	r0, r1

	MOV	pc, lr
.L2:
	WORD	messtrans_fd
	DECLARE_FUNCTION rdn_Toolbox_GetMessTransFD

	.bss

id_block:
	.space	24
	DECLARE_OBJECT id_block

messtrans_fd:
	.space	16
	DECLARE_OBJECT messtrans_fd

	.end
