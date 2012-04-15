@
@ wimp-support.s
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"

#define SWI_XWimp_Poll		0x600C7
#define SWI_XWimp_PollIdle	0x600E1

	.text

	@ os_error *rdn_Wimp_Poll (unsigned int mask,
	@			   unsigned int *poll_word)
	NAME	"rdn_Wimp_Poll"
	.global rdn_Wimp_Poll
rdn_Wimp_Poll:
	PIC	ip

	MOV	r3, r1
	LDR	r1, 0f
 PICEQ "LDR	r1, [ip, r1]"
	ADD	r1, r1, #4              @ Skip event type field
	SWI	SWI_XWimp_Poll
	MOVVS	pc, lr
	SUB	r1, r1, #4              @ Get base address of event
	STR	r0, [r1, #0]
	MOV	r0, #0

	MOV	pc, lr
0:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_Poll

	@ os_error *rdn_Wimp_PollIdle (unsigned int mask,
	@			       unsigned int time,
	@			       unsigned int *poll_word)
	NAME	"rdn_Wimp_PollIdle"
	.global rdn_Wimp_PollIdle
rdn_Wimp_PollIdle:
	PIC	ip

	MOV	r3, r2
	MOV	r2, r1
	LDR	r1, 0f
 PICEQ "LDR	r1, [ip, r1]"
	ADD	r1, r1, #4              @ Skip event type field
	SWI	SWI_XWimp_PollIdle
	MOVVS	pc, lr
	SUB	r1, r1, #4              @ Get base address of event
	STR	r0, [r1, #0]
	MOV	r0, #0

	MOV	pc, lr
0:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_PollIdle

	@ int rdn_Wimp_GetEventType (void)
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

	.bss

event:
event_type:
	.space 4
event_block:
	.space 256
	DECLARE_OBJECT event

	.end
