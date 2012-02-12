@
@ wimp.s - WIMP SWI wrappers
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"
#include "swis.h"

	.text

	@ os_error *rdn_Wimp_Initialise (int version,
	@				 char *desc,
	@				 void *mess_list,
	@				 unsigned int *handle)
	NAME	"rdn_Wimp_Initialise"
	.global rdn_Wimp_Initialise
rdn_Wimp_Initialise:
	MOV	ip, r3
	MOV	r3, r2
	MOV	r2, r1
	LDR	r1, =0x4b534154
	SWI	SWI_Wimp_Initialise
	MOVVC	r0, #0
	TEQ	ip, #0
	STRNE	r1, [ip, #0]
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_Initialise

	@ os_error *rdn_Wimp_CloseDown (unsigned int handle)
	NAME	"rdn_Wimp_CloseDown"
	.global rdn_Wimp_CloseDown
rdn_Wimp_CloseDown:
	LDR	r1, =0x4b534154
	SWI	SWI_Wimp_CloseDown
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_CloseDown

	@ os_error *rdn_Wimp_Poll (unsigned int mask,
	@			   unsigned int *poll_word,	// Input - can be NULL
	@			   unsigned int *sender)		// Output - can be NULL
	NAME	"rdn_Wimp_Poll"
	.global rdn_Wimp_Poll
rdn_Wimp_Poll:
	STR	r4, [sp, #-4]!

	PIC	ip

	MOV	r4, r3			@ r4 = sender address

	MOV	r3, r1
	LDR	r1, .L0
 PICEQ "LDR	r1, [ip, r1]"
	ADD	r1, r1, #4		@ Skip event type field
	SWI	SWI_Wimp_Poll
	BVS	0f
	SUB	r1, r1, #4		@ Get base address of event
	STRNE	r0, [r1, #0]
	TEQ	r4, #0
	STRNE	r2, [r4, #0]
	MOV	r0, #0
0:
	LDR	r4, [sp], #4
	MOV	pc, lr
.L0:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_Poll

	@ int rdn_Wimp_GetEventType (void)
	NAME	"rdn_Wimp_GetEventType"
	.global rdn_Wimp_GetEventType
rdn_Wimp_GetEventType:
	PIC	ip

	LDR	r0, .L1
 PICEQ "LDR	r0, [ip, r0]"
	LDR	r0, [r0, #0]

	MOV	pc, lr
.L1:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_GetEventType

	@ void *rdn_Wimp_GetEvent (void)
	NAME	"rdn_Wimp_GetEvent"
	.global rdn_Wimp_GetEvent
rdn_Wimp_GetEvent:
	PIC	ip

	LDR	r0, .L2
 PICEQ "LDR	r0, [ip, r0]"
	ADD	r0, r0, #4

	MOV	pc, lr
.L2:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_GetEvent

	@ os_error *rdn_Wimp_CreateWindow (void *window_block,
	@				   unsigned int *handle)
	NAME	"rdn_Wimp_CreateWindow"
	.global rdn_Wimp_CreateWindow
rdn_Wimp_CreateWindow:
	MOV	ip, r1
	MOV	r1, r0
	SWI	SWI_Wimp_CreateWindow
	BVS	0f
	TEQ	ip, #0
	STRNE	r0, [ip, #0]
	MOV	r0, #0
0:
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_CreateWindow

	@ os_error *rdn_Wimp_DeleteWindow (void *block)
	NAME	"rdn_Wimp_DeleteWindow"
	.global rdn_Wimp_DeleteWindow
rdn_Wimp_DeleteWindow:
	MOV	r1, r0
	SWI	SWI_Wimp_DeleteWindow
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_DeleteWindow

	@ os_error *rdn_Wimp_CloseWindow (void *block)
	NAME	"rdn_Wimp_CloseWindow"
	.global rdn_Wimp_CloseWindow
rdn_Wimp_CloseWindow:
	MOV	r1, r0
	SWI	SWI_Wimp_CloseWindow
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_CloseWindow

	@ os_error *rdn_Wimp_RedrawWindow (void *block,
	@				   int *more) // Required parameter, cannot be NULL
	NAME	"rdn_Wimp_RedrawWindow"
	.global rdn_Wimp_RedrawWindow
rdn_Wimp_RedrawWindow:
	MOV	ip, r1
	MOV	r1, r0
	SWI	SWI_Wimp_RedrawWindow
	STR	r0, [ip, #0]
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_RedrawWindow

	@ os_error *rdn_Wimp_GetRectangle (void *block,
	@				   int *more) // Required parameter, cannot be NULL
	NAME	"rdn_Wimp_GetRectangle"
	.global rdn_Wimp_GetRectangle
rdn_Wimp_GetRectangle:
	MOV	ip, r1
	MOV	r1, r0
	SWI	SWI_Wimp_GetRectangle
	STR	r0, [ip, #0]
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_GetRectangle

	@ os_error *rdn_Wimp_SetExtent (unsigned int handle,
	@				void *os_rect)
	NAME	"rdn_Wimp_SetExtent"
	.global rdn_Wimp_SetExtent
rdn_Wimp_SetExtent:
	SWI	SWI_Wimp_SetExtent
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_SetExtent

	@ os_error *rdn_Wimp_OpenWindow (void *open_window_block)
	NAME	"rdn_Wimp_OpenWindow"
	.global rdn_Wimp_OpenWindow
rdn_Wimp_OpenWindow:
	MOV	r1, r0
	SWI	SWI_Wimp_OpenWindow
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_OpenWindow

	@ os_error *rdn_Wimp_GetWindowState (void *window_state_block)
	NAME	"rdn_Wimp_GetWindowState"
	.global rdn_Wimp_GetWindowState
rdn_Wimp_GetWindowState:
	MOV	r1, r0
	SWI	SWI_Wimp_GetWindowState
	MOVVC	r0, #0
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_GetWindowState

	@ os_error *rdn_Wimp_GetNestedWindowState (void *window_state_block,
	@					   unsigned int *parent, // Output - can be NULL
	@					   unsigned int *nesting_flags) // Output - can be NULL
	NAME	"rdn_Wimp_GetNestedWindowState"
	.global rdn_Wimp_GetNestedWindowState
rdn_Wimp_GetNestedWindowState:
	STMFD	sp!, {r4, r5, r6}

	MOV	r5, r1				@ r5 = parent
	MOV	r6, r2				@ r6 = nesting_flags

	MOV	r1, r0
	LDR	r2, =0x4b534154
	SWI	SWI_Wimp_GetWindowState
	BVS	0f

	TEQ	r5, #0
	STRNE	r3, [r5, #0]
	TEQ	r6, #0
	STRNE	r4, [r6, #0]
	MOV	r0, #0
0:
	LDMFD	sp!, {r4, r5, r6}
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_GetNestedWindowState

	@ os_error *rdn_Wimp_OpenNestedWindow (void *window_state_block,
	@					unsigned int parent,
	@					unsigned int nesting_flags)
	NAME	"rdn_Wimp_OpenNestedWindow"
	.global rdn_Wimp_OpenNestedWindow
rdn_Wimp_OpenNestedWindow:
	STR	r4, [sp, #-4]!

	MOV	r4, r2
	MOV	r3, r1
	LDR	r2, =0x4b534154
	MOV	r1, r0
	SWI	SWI_Wimp_OpenWindow
	MOVVC	r0, #0

	LDR	r4, [sp], #4
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_OpenNestedWindow

	@ os_error *rdn_Wimp_CreateIcon (unsigned int priority,
	@				 void *icon_block,
	@				 unsigned int *icon_handle) // Required parameter - cannot be NULL
	NAME	"rdn_Wimp_CreateIcon"
	.global rdn_Wimp_CreateIcon
rdn_Wimp_CreateIcon:
	MOV	ip, r2			@ r2 is not corrupted by SWI, but just in case.
	SWI	SWI_Wimp_CreateIcon
	STRVC	r0, [ip, #0]
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_CreateIcon

	@ os_error *rdn_Wimp_DeleteIcon (void *icon_block)
	NAME	"rdn_Wimp_DeleteIcon"
	.global rdn_Wimp_DeleteIcon
rdn_Wimp_DeleteIcon:
	MOV	r1, r0
	SWI	SWI_Wimp_DeleteIcon
	MOV	pc, lr
	DECLARE_FUNCTION rdn_Wimp_DeleteIcon

	.bss

event:
event_type:
	.space 4
event_block:
	.space 256
	DECLARE_OBJECT event

	.end
