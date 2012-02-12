@
@ toolbox.s - Toolbox SWI wrappers
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"
#include "swis.h"

	.text

	@ os_error *rdn_Toolbox_Initialise (unsigned int flags,
	@				    unsigned int wimp_version,
	@				    unsigned int *message_list,		// Input - cannot be NULL
	@				    unsigned int *toolbox_event_list,	// Input - cannot be NULL
	@				    char *dir_name,			// Input - cannot be NULL
	@				    unsigned int *message_file_desc,	// Input - cannot be NULL
	@				    unsigned int *id_buffer,		// Input - cannot be NULL
	@				    unsigned int *task_handle,		// Output - may be NULL
	@				    void *sprite_area)			// Output - may be NULL
	NAME	"rdn_Toolbox_Initialise"
	.global rdn_Toolbox_Initialise
rdn_Toolbox_Initialise:
	STMFD	sp!, {r4, r5, r6}

	LDR	r4, [sp, #(3 * 4) + 0]
	LDR	r5, [sp, #(3 * 4) + 4]
	LDR	r6, [sp, #(3 * 4) + 8]
	SWI	SWI_Toolbox_Initialise
	LDR	ip, [sp, #(3 * 4) + 12]
	TEQ	ip, #0
	STRNE	r1, [ip, #0]
	LDR	ip, [sp, #(3 * 4) + 16]
	TEQ	ip, #0
	STRNE	r2, [ip, #0]

	LDMFD	sp!, {r4, r5, r6}
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_Initialise

	@ os_error *rdn_Toolbox_CreateObject (unsigned int flags,
	@				      void *template,
	@				      unsigned int *ID) // Output - required parameter - cannot be NULL
	NAME	"rdn_Toolbox_CreateObject"
	.global rdn_Toolbox_CreateObject
rdn_Toolbox_CreateObject:
	SWI	SWI_Toolbox_CreateObject
	STR	r0, [r2, #0]
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_CreateObject

	@ os_error *rdn_Toolbox_DeleteObject (unsigned int flags,
	@				      unsigned int ID)
	NAME	"rdn_Toolbox_DeleteObject"
	.global rdn_Toolbox_DeleteObject
rdn_Toolbox_DeleteObject:
	SWI	SWI_Toolbox_DeleteObject
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_DeleteObject

	@ os_error *rdn_Toolbox_ShowObject (unsigned int flags,
	@				    unsigned int object_id,
	@				    int show_type,
	@				    void *type,
	@				    unsigned int parent_id,
	@				    int parent_cmp)
	NAME	"rdn_Toolbox_ShowObject"
	.global rdn_Toolbox_ShowObject
rdn_Toolbox_ShowObject:
	STMFD	sp!, {r4, r5}

	LDR	r4, [sp, #(2 * 4) + 0]	@ (number of saved regs * 4) + offset
	LDR	r5, [sp, #(2 * 4) + 4]
	SWI	SWI_Toolbox_ShowObject
	MOVVC	r0, #0

	LDMFD	sp!, {r4, r5}
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_ShowObject

	@ os_error *rdn_Toolbox_HideObject (unsigned int flags,
	@				    unsigned int object_id)
	NAME	"rdn_Toolbox_HideObject"
	.global rdn_Toolbox_HideObject
rdn_Toolbox_HideObject:
	SWI	SWI_Toolbox_HideObject
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_HideObject

	@ os_error *rdn_Toolbox_GetObjectState (unsigned int flags,
	@					unsigned int object_id,
	@					unsigned int *state) // Output - required parameter - cannot be NULL
	NAME	"rdn_Toolbox_GetObjectState"
	.global rdn_Toolbox_GetObjectState
rdn_Toolbox_GetObjectState:
	SWI	SWI_Toolbox_GetObjectState
	STRVC	r0, [r2, #0]
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_GetObjectState

	@ os_error *rdn_Toolbox_GetClientHandle (unsigned int flags,
	@					 unsigned int object_id,
	@					 void *handle) // Output - required parameter - cannot be NULL
	NAME	"rdn_Toolbox_GetClientHandle"
	.global	rdn_Toolbox_GetClientHandle
rdn_Toolbox_GetClientHandle:
	SWI	SWI_Toolbox_GetClientHandle
	STRVC	r0, [r2, #0]
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_GetClientHandle

	@ os_error *rdn_Toolbox_SetClientHandle (unsigned int flags,
	@					 unsigned int object_id,
	@					 void *handle)
	NAME	"rdn_Toolbox_SetClientHandle"
	.global	rdn_Toolbox_SetClientHandle
rdn_Toolbox_SetClientHandle:
	SWI	SWI_Toolbox_SetClientHandle
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Toolbox_SetClientHandle


	.end
