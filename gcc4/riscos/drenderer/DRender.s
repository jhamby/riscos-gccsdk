;*
;* drender.s
;*
;* Assembler interface to DigitalRenderer functions
;* (C) 1997-2000 Andreas Dehmel (zarquon@t-online.de)
;* (C) 2012 GCCSDK Developers
;*
;* This file was part of wimplib. wimplib is released under the Lesser
;* GNU Public License (LGPL). For more information see the License
;* provided with the library release.
;*
;* Currently part of the GCCSDK project.

	AREA	|Asm$$Code|, CODE, READONLY

DigiRendChunk	EQU	0x6F700		;X flag set

	ALIGN	4
|wimplib_return_code|
	teq	r0, r0
	teq	pc, pc
	moveq	pc, lr
	movs	pc, lr


	ALIGN	4
	EXPORT	|DigitalRenderer_Activate|
	=	"DigitalRenderer_Activate"
	ALIGN	4

|DigitalRenderer_Activate|
	swi	DigiRendChunk
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_Deactivate|
	=	"DigitalRenderer_Deactivate"
	ALIGN	4

|DigitalRenderer_Deactivate|
	swi	DigiRendChunk+1
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_Pause|
	=	"DigitalRenderer_Pause"
	ALIGN	4

|DigitalRenderer_Pause|
	swi	DigiRendChunk+2
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_Resume|
	=	"DigitalRenderer_Resume"
	ALIGN	4

|DigitalRenderer_Resume|
	swi	DigiRendChunk+3
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_GetTables|
	=	"DigitalRenderer_GetTables"
	ALIGN	4

|DigitalRenderer_GetTables|
	mov	r2,r0
	mov	r3,r1
	swi	DigiRendChunk+4
	strvc	r0,[r2, #0]
	strvc	r1,[r3, #0]
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_ReadState|
	=	"DigitalRenderer_ReadState"
	ALIGN	4

|DigitalRenderer_ReadState|
	swi	DigiRendChunk+5
	mvnvs	r0,#0			;returns -1 on error
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_NewSample|
	=	"DigitalRenderer_NewSample"
	ALIGN	4

|DigitalRenderer_NewSample|
	swi	DigiRendChunk+6
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_New16BitSample|
	=	"DigitalRenderer_New16BitSample"
	ALIGN	4

|DigitalRenderer_New16BitSample|
	swi	DigiRendChunk+7
	movvc	r0,#0
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_BufferStatistics|
	=	"DigitalRenderer_BufferStatistics"
	ALIGN	4

|DigitalRenderer_BufferStatistics|
	mov	r2,r0
	mov	r3,r1
	swi	DigiRendChunk+8
	str	r0,[r2, #0]
	str	r1,[r3, #0]
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_NumBuffers|
	=	"DigitalRenderer_NumBuffers"
	ALIGN	4

|DigitalRenderer_NumBuffers|
	swi	DigiRendChunk+9
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_StreamSamples|
	=	"DigitalRenderer_StreamSamples"
	ALIGN	4

|DigitalRenderer_StreamSamples|
	swi	DigiRendChunk+10
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_Stream16BitSamples|
	=	"DigitalRenderer_Stream16BitSamples"
	ALIGN	4

|DigitalRenderer_Stream16BitSamples|
	swi	DigiRendChunk+11
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_StreamStatistics|
	=	"DigitalRenderer_StreamStatistics"
	ALIGN	4

|DigitalRenderer_StreamStatistics|
	swi	DigiRendChunk+12
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_StreamFlags|
	=	"DigitalRenderer_StreamFlags"
	ALIGN	4

|DigitalRenderer_StreamFlags|
	swi	DigiRendChunk+13
	b	|wimplib_return_code|



	ALIGN 	4
	EXPORT	|DigitalRenderer_SetDefaults|
	=	"DigitalRenderer_SetDefaults"
	ALIGN	4

|DigitalRenderer_SetDefaults|
	stmdb	sp!,{r4-r9,lr}
	mov	r6,r0
	mov	r7,r1
	mov	r8,r2
	mov	r9,r3
	ldr	r0,[r0, #0]
	ldr	r1,[r1, #0]
	ldr	r2,[r2, #0]
	ldr	r3,[r3, #0]
	ldr	r4,[sp,#28]		;7 regs on stack
	ldr	r4,[r4, #0]
	ldr	r5,[sp,#32]
	ldr	r5,[r5, #0]
	swi	DigiRendChunk+14
	str	r0,[r6, #0]
	str	r1,[r7, #0]
	str	r2,[r8, #0]
	str	r3,[r9, #0]
	ldr	r0,[sp,#28]
	str	r4,[r0, #0]
	ldr	r0,[sp,#32]
	str	r5,[r0, #0]
	ldmia	sp!,{r4-r9,lr}
	b	|wimplib_return_code|


	ALIGN	4
	EXPORT	|DigitalRenderer_Activate16|
	=	"DigitalRenderer_Activate16"
	ALIGN	4

|DigitalRenderer_Activate16|
	swi	DigiRendChunk+15
	movvc	r0,#0
	b	|wimplib_return_code|



	ALIGN	4
	EXPORT	|DigitalRenderer_GetFrequency|
	=	"DigitalRenderer_GetFrequency"
	ALIGN	4

|DigitalRenderer_GetFrequency|
	swi	DigiRendChunk+16
	b	|wimplib_return_code|



	ALIGN	4
	EXPORT	|DigitalRenderer_ActivateAuto|
	=	"DigitalRenderer_ActivateAuto"
	ALIGN	4

|DigitalRenderer_ActivateAuto|
	swi	DigiRendChunk+17
	movvc	r0,#0
	b	|wimplib_return_code|



	ALIGN	4
	EXPORT	|DigitalRenderer_SampleFormat|
	=	"DigitalRenderer_SampleFormat"
	ALIGN	4

|DigitalRenderer_SampleFormat|
	swi	DigiRendChunk+18
	b	|wimplib_return_code|



	END
