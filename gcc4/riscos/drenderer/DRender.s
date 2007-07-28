;*
;* drender.s
;*
;* Assembler interface to DigitalRenderer functions
;* (C) 1997-2000 Andreas Dehmel (zarquon@t-online.de)
;*
;* This file is part of wimplib. wimplib is released under the Lesser
;* GNU Public License (LGPL). For more information see the License
;* provided with the library release.


; standard register names
r0	rn	0
r1	rn	1
r2	rn	2
r3	rn	3
r4	rn	4
r5	rn	5
r6	rn	6
r7	rn	7
r8	rn	8
r9	rn	9
r10	rn	10
r11	rn	11
r12	rn	12
sp	rn	13
lr	rn	14
pc	rn	15



	IDFN	(C) 1997-2000 by Andreas Dehmel

	AREA	CODE, READONLY


DigiRendChunk	equ	0x6F700		;X flag set

	align	4
|wimplib_return_code|:
	teq	r0, r0
	teq	pc, pc
	moveq	pc, lr
	movs	pc, lr


	align	4
	export	|DigitalRenderer_Activate|
	=	"DigitalRenderer_Activate"
	align	4

|DigitalRenderer_Activate|:
	swi	DigiRendChunk
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_Deactivate|
	=	"DigitalRenderer_Deactivate"
	align	4

|DigitalRenderer_Deactivate|:
	swi	DigiRendChunk+1
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_Pause|
	=	"DigitalRenderer_Pause"
	align	4

|DigitalRenderer_Pause|:
	swi	DigiRendChunk+2
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_Resume|
	=	"DigitalRenderer_Resume"
	align	4

|DigitalRenderer_Resume|:
	swi	DigiRendChunk+3
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_GetTables|
	=	"DigitalRenderer_GetTables"
	align	4

|DigitalRenderer_GetTables|:
	mov	r2,r0
	mov	r3,r1
	swi	DigiRendChunk+4
	strvc	r0,[r2, #0]
	strvc	r1,[r3, #0]
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_ReadState|
	=	"DigitalRenderer_ReadState"
	align	4

|DigitalRenderer_ReadState|:
	swi	DigiRendChunk+5
	mvnvs	r0,#0			;returns -1 on error
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_NewSample|
	=	"DigitalRenderer_NewSample"
	align	4

|DigitalRenderer_NewSample|:
	swi	DigiRendChunk+6
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_New16BitSample|
	=	"DigitalRenderer_New16BitSample"
	align	4

|DigitalRenderer_New16BitSample|:
	swi	DigiRendChunk+7
	movvc	r0,#0
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_BufferStatistics|
	=	"DigitalRenderer_BufferStatistics"
	align	4

|DigitalRenderer_BufferStatistics|:
	mov	r2,r0
	mov	r3,r1
	swi	DigiRendChunk+8
	str	r0,[r2, #0]
	str	r1,[r3, #0]
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_NumBuffers|
	=	"DigitalRenderer_NumBuffers"
	align	4

|DigitalRenderer_NumBuffers|:
	swi	DigiRendChunk+9
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_StreamSamples|
	=	"DigitalRenderer_StreamSamples"
	align	4

|DigitalRenderer_StreamSamples|:
	swi	DigiRendChunk+10
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_Stream16BitSamples|
	=	"DigitalRenderer_Stream16BitSamples"
	align	4

|DigitalRenderer_Stream16BitSamples|:
	swi	DigiRendChunk+11
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_StreamStatistics|
	=	"DigitalRenderer_StreamStatistics"
	align	4

|DigitalRenderer_StreamStatistics|:
	swi	DigiRendChunk+12
	b	|wimplib_return_code|


	align	4
	export	|DigitalRenderer_StreamFlags|
	=	"DigitalRenderer_StreamFlags"
	align	4

|DigitalRenderer_StreamFlags|:
	swi	DigiRendChunk+13
	b	|wimplib_return_code|



	align 	4
	export	|DigitalRenderer_SetDefaults|
	=	"DigitalRenderer_SetDefaults"
	align	4

|DigitalRenderer_SetDefaults|:
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


	align	4
	export	|DigitalRenderer_Activate16|
	=	"DigitalRenderer_Activate16"
	align	4

|DigitalRenderer_Activate16|:
	swi	DigiRendChunk+15
	movvc	r0,#0
	b	|wimplib_return_code|



	align	4
	export	|DigitalRenderer_GetFrequency|
	=	"DigitalRenderer_GetFrequency"
	align	4

|DigitalRenderer_GetFrequency|:
	swi	DigiRendChunk+16
	b	|wimplib_return_code|



	align	4
	export	|DigitalRenderer_ActivateAuto|
	=	"DigitalRenderer_ActivateAuto"
	align	4

|DigitalRenderer_ActivateAuto|:
	swi	DigiRendChunk+17
	movvc	r0,#0
	b	|wimplib_return_code|



	align	4
	export	|DigitalRenderer_SampleFormat|
	=	"DigitalRenderer_SampleFormat"
	align	4

|DigitalRenderer_SampleFormat|:
	swi	DigiRendChunk+18
	b	|wimplib_return_code|



	END
