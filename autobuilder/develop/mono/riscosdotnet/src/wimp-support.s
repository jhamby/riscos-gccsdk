@
@ wimp-support.s
@
@ Part of libriscosdotnet - a binding of the RISC OS API to C#.
@
@ Author: Lee Noar (leenoar@sky.com)
@

#include "macros.h"

	.text

	@ os_error *rdn_Wimp_Poll (unsigned int mask,
	@			   unsigned int *poll_word)	// Input - can be NULL
	NAME	"rdn_Wimp_Poll"
	.global rdn_Wimp_Poll
rdn_Wimp_Poll:
	PIC	ip

	MOV	r2, r1			@ r4 = pollword

	LDR	r1, .L0
 PICEQ "LDR	r3, [ip, r1]"
	ADD	r1, r3, #4		@ Skip event type field
	B	xwimp_poll
.L0:
	WORD	event
	DECLARE_FUNCTION rdn_Wimp_Poll

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
