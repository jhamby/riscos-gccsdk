; *******************************************************************
; *
; * Digital Renderer module source code
; *
; * Support for sound playback from applications
; * (C) 1997-2006 Andreas Dehmel (zarquon@t-online.de)
; * (C) 2011-2012 Christopher Martin (belles@internode.on.net)
; *
; * This program is free software; you can redistribute it and/or modify
; * it under the terms of the GNU General Public License as published by
; * the Free Software Foundation; either version 2 of the License, or
; * (at your option) any later version.
; *
; * This program is distributed in the hope that it will be useful,
; * but WITHOUT ANY WARRANTY; without even the implied warranty of
; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; * GNU General Public License for more details.
; *
; * You should have received a copy of the GNU General Public License
; * along with this program; if not, write to the Free Software
; * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
; * 02111-1307  USA.
; *
; *******************************************************************


XOS_ConvertInteger4		*	&0200DC
XOS_FSControl			*	&020029
XOS_Find			*	&02000D
XOS_Module			*	&02001E
XOS_NewLine			*	&020003
XOS_ReadMonotonicTime		*	&020042
XOS_ReadUnsigned		*	&020021
XOS_UpCall			*	&020033
XOS_Write0			*	&020002
XOS_WriteC			*	&020000
XSound_AttachVoice		*	&060185
XSound_Configure		*	&060140
XSound_Control			*	&060189
XSound_Enable			*	&060141
XSound_InstallVoice		*	&060183
XSound_LinearHandler		*	&060145
XSound_Mode			*	&060144
XSound_RemoveVoice		*	&060184
XSound_SampleRate		*	&060146
XSound_Volume			*	&060180
XSound_WriteControlBlock	*	&06018C


; mini buffer for putting samples, must be a multiple of 2!
MiniBufferSize		*	32

			^	0
Work_MagicWord		#	4
Work_Channels		#	4	; DO NOT RE-ORDER - number of channels
Work_BuffSize		#	4	; DO NOT RE-ORDER - number of samples per channel buffer
Work_Period		#	4	; DO NOT RE-ORDER
Work_Buffer		#	4
Work_State		#	4
Work_OldChannels	#	4
Work_OldSize		#	4
Work_OldPeriod		#	4
Work_OldEnable		#	4
Work_OldVolume		#	4
Work_NumBuffers		#	4	; number of buffers in the streaming buffer ring, 0 if not streaming
Work_WriteBuffer	#	4	; index of the next buffer into which samples should be written for later read-out
Work_ReadBuffer		#	4	; index of the next buffer from which samples should be read-out to the sound system
Work_RingBuffer		#	4	; address of the array of pointers to streaming buffers, i.e. the buffer ring
Work_FillLevel		#	4	; byte offset [0..Work_TotBuffSize-1] to the next sample to be written into the current buffer
Work_LastReadBuff	#	4	; index of the last streaming buffer from which samples were read-out to the sound system
Work_StreamFlags	#	4
Work_ReadLevel		#	4	; byte offset [0..Work_TotBuffSize-1] to the next sample to read-out from the current buffer
Work_CallBackFill	#	4
Work_BuffersPlayed	#	4
Work_BuffersGiven	#	4
Work_VoiceSlot		#	4
Work_OldVoiceName	#	4
Work_LinToLog		#	4
Work_LogScale		#	4
Work_PollWord		#	4
Work_TotBuffSize	#	4	; total buffer size (bytes) = #channels * #samples/channel * #bytes/sample
Work_StreamHandle	#	4
Work_StrExtHand		#	4
Work_BuffOffset		#	4
Work_OldStrFlags	#	4
Work_OldNumBuff		#	4
Work_RingIsFull		#	4	; if Work_ReadBuffer==Work_WriteBuffer, 0/1 indicates streaming buffer ring is empty/full
Work_Frequency		#	4
Work_FreqIndex		#	4
Work_OldFreqIndex	#	4
Work_TableVolume	#	4
Work_OldLinHandler	#	4
Work_OldLinParam	#	4
Work_DMABuffSize	#	4
Work_SampleFormat	#	4

; defaults for file interface
Work_DfltChannels	#	4
Work_DfltFormat		#	4
Work_DfltPeriod		#	4
Work_DfltBuffSize	#	4
Work_DfltBuffers	#	4
Work_DfltFrequency	#	4

; arrays of 8 ints
Work_OldVoices		#	8*4
Work_OldCBWords		#	8*4

Work_MiniBuffer		#	MiniBufferSize
Work_LogTable		#	1024
Work_SIZE		*	@

; sound buffer states
State_Active		*	1
State_NeedData		*	2
State_Overflow		*	4
State_UpCall		*	8
State_Paused		*	16
State_NewSound		*	32
State_Restore16		*	64
State_Deactivating      *       128

; In what seems to me to be a classic design blunder, the Acorn 16-bit sound system requires samples in right-left order,
; although every other system I have encountered encodes samples in left-right order. This has the weird effect that
; "correct" left/right channel ordering on any other system is "incorrect" on RISC OS! This has a detrimental effect on
; software ported from other platforms because the rest of the world considers left-right order to be normal and so files
; with that ordering will NOT be swapped by the software to cater for the perverse Acorn right-left ordering. In other
; words, if the software says that the ordering is SWAPPED right-left, then that is in fact CORRECT for Acorn's system and
; so should NOT be swapped. It is counter-intuitive, but when software says that the order is correct left-right, that is
; when swapping must be done on Acorn's system. There is potential here for much confusion.
; --
; Christopher Martin, Fri 20th April 2012 16:16
;
; formats (8bit ulaw, 16bit signed linear)
Format_Undef		*	0
Format_8u		*	1
Format_16sle_rl		*	2	; right-left order is what the Acorn 16-bit sound system requires
Format_16sle_lr		*	3	; but left-right order is normal for non-Acorn formats and platforms

; shift factor for volume (assume 128 rather than 127)
Volume_Shift		*	7

; Filing system number (not allocated yet)
FSNumber		*	167

SWI_ChunkBase		*	&4F700
SWI_XChunkBase		*	SWI_ChunkBase:OR:&20000
XDR_Activate		*	SWI_XChunkBase
XDR_Deactivate		*	SWI_XChunkBase+1
XDR_Pause		*	SWI_XChunkBase+2
XDR_Resume		*	SWI_XChunkBase+3
XDR_GetTables		*	SWI_XChunkBase+4
XDR_ReadState		*	SWI_XChunkBase+5
XDR_NewSample		*	SWI_XChunkBase+6
XDR_NewSamp16		*	SWI_XChunkBase+7
XDR_BufferStat		*	SWI_XChunkBase+8
XDR_NumBuffers		*	SWI_XChunkBase+9
XDR_StreamSamples	*	SWI_XChunkBase+10
XDR_StreamSamples16	*	SWI_XChunkBase+11
XDR_StreamStatus	*	SWI_XChunkBase+12
XDR_StreamFlags		*	SWI_XChunkBase+13
XDR_Defaults		*	SWI_XChunkBase+14
XDR_Activate16		*	SWI_XChunkBase+15
XDR_GetFrequency	*	SWI_XChunkBase+16
XDR_ActivateAuto	*	SWI_XChunkBase+17
XDR_SampleFormat	*	SWI_XChunkBase+18

; offset in SCCB where workspace pointer of this module is stored (PRM4-11)
SCCB_PrivateOffset	*	64

; magic word to make sure the workspace pointer is correct in the voice handler
DigitalRendererMagicWord	*	&444e5244	;  "DRND"

; Stream flags
StrFlg_OverrunNull	*	1	; fill buffer with 0 on an overrun
StrFlg_IssueUpCall	*	2	; use upcalls for waiting
StrFlg_OverBlock	*	4	; block on stream overflow, else truncate

; DMA control block (as passed to Channel Handler in R9)
DMACB_Header		*	192

; Processor flags
PFlag_V                 *       (1<<28)
PFlag_C                 *       (1<<29)
PFlag_IRQ		*	(1<<27)
PFlag_FIRQ		*	(1<<26)
PMode_User		*	0
PMode_FIRQ		*	1
PMode_IRQ		*	2
PMode_SVC		*	3

; store module's private word in SCCB#1 here.
SCCB_PrivateWord	*	64

; Sound control flags (offset 3 in SCCB)
SFlag_F1		*	1
SFlag_F2		*	2
SFlag_Overrun		*	4
SFlag_Active		*	8
SFlag_FPending		*	16
SFlag_IPending		*	32
SFlag_KPending		*	64
SFlag_Qiet		*	128


	MACRO
	FNConvertToUlaw	$tab,$lo,$hi
	MOV	$lo,$hi,LSL #16
	LDRB	$lo,[$tab,$lo,LSR #19]
	LDRB	$hi,[$tab,$hi,LSR #19]
	ORR	$lo,$lo,$hi,LSL #8
	MEND

	MACRO
	FNPrintNumber
	SUB	R13,R13,#16
	MOV	R1,R13
	MOV	R2,#16
	SWI	XOS_ConvertInteger4
	SWI	XOS_Write0
	ADD	R13,R13,#16
	MEND

	MACRO
	FNPrintConfig	$str,$off
	ADR	R0,$str
	SWI	XOS_Write0
	LDR	R0,[R12,#$off]
	FNPrintNumber
	MEND

	MACRO
	FNexpandsample	$reg1,$reg2
	MOV	$reg1,$reg2,LSL #16
	BIC	$reg2,$reg2,$reg1,LSR #16
	ORR	$reg1,$reg1,$reg1,LSR #16
	ORR	$reg2,$reg2,$reg2,LSR #16
	MEND

	MACRO
	FNldr2	$r,$a
	LCLA	off
off	SETA	$a-.-8
	[ &$off < &80000000
		ADD     $r,PC,#(&$off:AND:&FFFFF000)
		LDR     $r,[$r,#(&$off:AND:&FFF)]
	|
off		SETA	-&$off
		SUB     $r,PC,#(&$off:AND:&FFFFF000)
		LDR     $r,[$r,#-(&$off:AND:&FFF)]
	]
	MEND

	MACRO
	FNlink	$r,$a	; Calculates a link address with 26-bit PSR
;  This way I think is more comprehensible, but ...
;	MOV	$r,PC
;	[ ($a-(.+4)) < &80000000	; ... asasm/objasm fault: this fails if label $a is not yet defined
;		ADD	$r,$r,#($a-(.+4))
;	|
;		SUB	$r,$r,#((.+4)-$a)
;	]
	MOV	$r,#($a-(.+12))		; asasm/objasm feature: will convert to MVN if the offset is negative
	ADD	$r,$r,PC
;
	MEND

	MACRO
	FNscalesamples	$reg,$temp,$mask
	MOV	$temp,$reg,ASR #16
	MUL	$temp,R0,$temp
	MOV	$reg,$reg,LSL #16
	MOV	$reg,$reg,ASR #16
	MUL	$reg,R0,$reg
	AND	$temp,$temp,$mask,LSL #Volume_Shift
	MOV	$temp,$temp,LSL #(16-Volume_Shift)
	AND	$reg,$reg,$mask,LSL #Volume_Shift
	ORR	$reg,$temp,$reg,LSR #Volume_Shift
	MEND

	MACRO
	FNscalesampshft	$reg1,$reg2,$temp,$mask
	MOV	$temp,$reg2,LSL #16
	MUL	$reg1,r0,$reg1
	MOV	$temp,$temp,ASR #16
	MUL	$temp,r0,$temp
	AND	$temp,$temp,$mask,LSL #Volume_Shift
	MOV	$temp,$temp,LSL #(16-Volume_Shift)
	AND	$reg1,$reg1,$mask,LSL #Volume_Shift
	ORR	$reg1,$temp,$reg1,LSR #Volume_Shift
	MEND

	MACRO
	FNsmalldiv32	$prec,$i,$j,$k,$m
	ROUT
	MOV	$m,#(1<<$prec)
	MOV	$k,#0
10 ;sdiv_alignloop
	CMP	$i,$j
	MOVHI	$j,$j,LSL #1
	MOVHI	$m,$m,LSL #1
	BHI	%10
20 ;sdiv_divloop
	CMP	$i,$j
	SUBCS	$i,$i,$j
	ORRCS	$k,$k,$m
	MOV	$i,$i,LSL #1
	MOVS	$m,$m,LSR #1
	BNE	%20
	ROUT
	MEND


; This is the general pattern to be followed when freeing RMA memory
; RMA_Free
; 	LDR	R2,[hook]
; 	MOV	zero,#0
; 	TEQ	R2,#0
; 	STRNE	zero,[hook]	; In case of IRQ, ensure memory to be freed won't be used
; 	MOV	R0,#7
; 	SWINE	XOS_Module

; This is the general pattern to be followed when allocating RMA memory
; RMA_Alloc
; 	LDR	R2,[hook]
; 	MOV	zero,#0
; 	TEQ	R2,#0
; 	STRNE	zero,[hook]	; In case of IRQ, ensure memory to be freed won't be used
; 	MOV	R0,#7
; 	SWINE	XOS_Module	; Avoid RMA memory leaks! First, free any old block
; 	MOV	R0,#6
; 	SWI	XOS_Module
; 	STRVC	R2,[hook]


	AREA	Module,CODE,READONLY

	ENTRY

DRenderModuleBase
	&	0		;start code
	&	InitCode
	&	FinalCode
	&	ServiceHandler
	&	TitleString
	&	HelpString
	&	HelpTable
	&	SWI_ChunkBase
	&	SWIHandler
	&	SWIDecodingTable
	&	0		;SWI decoding code
	&	0
	&	DRenderModuleFlags

DRenderMagic
	&	DigitalRendererMagicWord
DRenderModuleFlags
	&	1

InitCode
	LDR	R2,[R12]
	STR	R14,[R13,#-4]!
	TEQ	R2,#0
	BNE	ICreinit
	MOV	R0,#6
	MOV	R3,#Work_SIZE
	SWI	XOS_Module		;get workspace
	BVS	InitError
	STR	R2,[R12]
	MOV	R0,R2
	MOV	R1,R3
	BL	MemSet
	LDR	R0,DRenderMagic
	STR	R0,[R2,#Work_MagicWord]
ICreinit
	MOV	R0,#1			;defaults for 1channel 22kHz, 1 second buffer time
	STR	R0,[R2,#Work_DfltChannels]
	MOV	R0,#Format_16sle_rl
	STR	R0,[R2,#Work_DfltFormat]
	MOV	R0,#45
	STR	R0,[R2,#Work_DfltPeriod]
	MOV	R0,#512
	STR	R0,[R2,#Work_DfltBuffSize]
	MOV	R0,#43
	STR	R0,[R2,#Work_DfltBuffers]
	MOV	R0,#(22050:AND:&ff00)
	ORR	R0,R0,#(22050:AND:&ff)
	STR	R0,[R2,#Work_DfltFrequency]
	MOV	R0,#(StrFlg_OverrunNull + StrFlg_IssueUpCall)
	STR	R0,[R2,#Work_StreamFlags]
	BL	RegisterFilingSystem
	LDR	R14,[R13],#4
ModuleReturnOK
	TEQ	R0,R0
	TEQ	PC,PC
	MOVNES  PC,R14
	MSR	CPSR_f, #0
	MOV	PC,R14
InitError
	LDR	R14,[R13],#4
	ADR	R0,Error_Initialise
ModuleReturnError
	TEQ	R0,R0
	TEQ	PC,PC
	ORRNES  PC,R14,#PFlag_V
	MSR	CPSR_f,#PFlag_V
	MOV	PC,R14


FinalCode
	MOV	R4,R12			;preserve private word
	LDR	R12,[R12]
	LDR	R0,[R12,#Work_State]
	STR	R14,[R13,#-4]!
	TST	R0,#State_Active
	BLNE	RestoreOldContext
	TEQ	R10,#0
	BEQ	FCnonfatal		;if fatal free workspace
	MOV	R12,R4
	BL	DeregisterFilingSystem
	LDR	R2,[R4]
	MOV	R0,#0
	TEQ	R2,#0
	STRNE	R0,[R4]			;clear private word
	MOV	R0,#7
	SWINE	XOS_Module
FCnonfatal
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FCcantquit
	LDR	R14,[R13],#4
	ADR	R0,Error_NoHandlers
	B	ModuleReturnError


	&	ServiceTable
ServiceHandler
	MOV	R0,R0			; a magic word
	TEQ	R1,#&40
	MOVNE	PC,R14
	; else fall into the sole service handler


	; FILING SYSTEM

RegisterFilingSystem
	STMDB	R13!,{R0-R3,R14}
	MOV	R0,#12
	ADR	R1,DRenderModuleBase
	MOV	R2,#FilingSystemBlock-DRenderModuleBase
	MOV	R3,R12
	SWI	XOS_FSControl
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK

DeregisterFilingSystem
	STMDB	R13!,{R0,R1,R14}
	MOV	R0,#16
	ADR	R1,FSName
	SWI	XOS_FSControl
	LDMIA	R13!,{R0,R1,R14}
	B	ModuleReturnOK

FilingSystemBlock
	&	FSName
	&	FSStartup
	&	FSEntry_Open
	&	FSEntry_GetBytes
	&	FSEntry_PutBytes
	&	FSEntry_Args
	&	FSEntry_Close
	&	FSEntry_File
	&	FSNumber + (1 << 8) + (&38 << 16) + (&38 << 24)
	&	FSEntry_Func
	&	FSEntry_GBPB

FSName
	=	"DRender",0
        ALIGN
FSStartup
	=	0
        ALIGN
FSDfltSaveName
	=	"DRender:",0
	ALIGN


ServiceTable
	&	0
	&	RegisterFilingSystem
	&	&40
	&	0


MemSet	;clears buffer R0, size R1 to 0 (multiple of words!)
	STR	R14,[R13,#-4]!		;MUST BE SAFE FOR IRQ MODE!
MemStIRQ
	STMDB	R13!,{R0-R5}		;in IRQ mode, the caller must store the return address!
	MOV	R2,#0
	MOV	R3,#0
	MOV	R4,#0
	MOV	R5,#0
	CMP	R1,#16
	BLT	MSsmall
MSbig
	STMIA	R0!,{R2-R5}
	SUB	R1,R1,#16
	CMP	R1,#16
	BGE	MSbig
MSsmall
	TST	R1,#8
	STMNEIA R0!,{R2,R3}
	TST	R1,#4
	STRNE	R2,[R0],#4
	LDMIA	R13!,{R0-R5,PC}      ;MEMSET DOESN'T RESTORE FLAGS! IRQ SAFE!


Error_Initialise
	&	0
	=	"Unable to initialise.",0
	ALIGN
Error_BadSWI
	&	1
	=	"Bad SWI number.",0
	ALIGN
Error_NoReinit
	&	2
	=	"Digital Renderer is already active.",0
	ALIGN
Error_BadBuffSize
	&	3
	=	"Bad buffer size (must be multiple of 16).",0
	ALIGN
Error_Memory
	&	4
	=	"Couldn't claim enough memory.",0
	ALIGN
Error_Inactive
	&	5
	=	"Digital renderer not activated.",0
	ALIGN
Error_NoHandlers
	&	6
	=	"Must install new Channel Handler / Scheduler first.",0
	ALIGN
Error_RingBuffer
	&	7
	=	"Ring buffer not initialized",0
	ALIGN
Error_Install
	&	8
	=	"Unable to install voice generator",0
	ALIGN
Error_ParseError
	=	9
	=	"Parse error in command",0
	ALIGN
Error_No16Bit
	&	10
	=	"No 16bit sound available",0
	ALIGN
Error_NoRate
	&	11
	=	"No sample rate found",0
	ALIGN
Error_Channels16
	&	12
	=	"Number of channels must be 1 or 2 in 16bit mode",0


TitleString
	=	"DigitalRenderer",0
        ALIGN

HelpString
        =       "DigitalRenderer",9,"0.56 beta 8 GPL (15 May 2012)",13,10
	=	"Provides a means to playback samples from applications."
	=	" © 1997-2012 Andreas Dehmel, Christopher Martin",0
        ALIGN

HelpTable
	=	"DRenderOff",0
	ALIGN
	&	CommandDRoff
	&	&0000ff00
	&	0
	&	Help_DRoff
	=	"DRenderDefaults",0
	ALIGN
	&	CommandDRdefault
	&	&0010ff00
	&	0
	&	Help_DRdefault
	=	"DRenderStatus",0
	ALIGN
	&	CommandDRstatus
	&	&0000ff00
	&	0
	&	Help_DRstatus
	&	0

Help_DRoff
	=	"Switch off DigitalRenderer",0
        ALIGN
Help_DRdefault
	=	"Set the defaults or print the current ones if no parameters given",13,10,9
	=	"-c #: number of channels (1,2,4,8)",13,10,9
	=	"-f #: format (1: 8bit ulaw; 2: 16bit sl le r-l; 3: 16bit sl le l-r)",13,10,9
	=	"-p #: sample period (1e6/frequency)",13,10,9
	=	"-q #: sample frequency (for 16bit sound, zero to disable)",13,10,9
	=	"-s #: buffer size",13,10,9
	=	"-n #: number of buffers ( > 1!)",0
        ALIGN
Help_DRstatus
	=	"Show current status",0
	ALIGN


DoIntegerDivision
	STMDB	R13!,{R1-R2,R14}
	TEQ	R1,#0
	MVNEQ	R0,#0
	BEQ	DUIVexit
	FNsmalldiv32	0,R0,R1,R2,R14
	MOV	R0,R2
DUIVexit
	LDMIA	R13!,{R1-R2,R14}
	B	ModuleReturnOK


DisableRenderer
	STMDB	R13!,{R0-R3,R14}
	LDR	R1,[R12,#Work_StrExtHand]
	TEQ	R1,#0
        MOVNE   R0,#0                   ;make sure everything is closed correctly by using
        SWINE   XOS_Find                ;the external handle
	LDR	R2,[R12,#Work_Buffer]
	MOV	R0,#0
	TEQ	R2,#0
	STRNE	R0,[R12,#Work_Buffer]	; In case of IRQ, ensure memory to be freed won't be used
	STR	R0,[R12,#Work_State]
	MOV	R0,#7
	SWINE	XOS_Module
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK


RestoreOldContext
	STMDB	R13!,{R0-R5,R14}
	MOV	R0,#1
	SWI	XSound_Enable      ;switch off sound prior to handler change
	LDR	R0,[R12,#Work_State]
	TST	R0,#State_NewSound
	BNE	ROCnewsound
    ;;
    ;;
	LDR	R0,[R12,#Work_VoiceSlot]
	TEQ	R0,#0
	BEQ	ROCnotinitialized
	ADD	R3,R12,#Work_OldVoices
	LDR	R4,[R12,#Work_Channels]
	MOV	R5,#0
ROCunattachloop
	ADD	R0,R5,#1
	MOV	R1,#SCCB_PrivateOffset
	LDR	R2,[R3,#(Work_OldCBWords-Work_OldVoices)]
	SWI	XSound_WriteControlBlock
	ADD	R0,R5,#1
	LDR	R1,[R3],#4
	SWI	XSound_AttachVoice
	ADD	R5,R5,#1
	CMP	R5,R4
	BLT	ROCunattachloop
	LDR	R1,[R12,#Work_VoiceSlot]
	SWI	XSound_RemoveVoice
	MOV	R0,#0
	STR	R0,[R12,#Work_VoiceSlot]
ROCnotinitialized
	ADD	R0,R12,#Work_OldChannels
	LDMIA	R0,{R0-R2}
	MOV	R3,#0
	MOV	R4,#0
	SWI	XSound_Configure   ;restore previous sound context
	B	ROCepilogue
    ;;
    ;;
ROCnewsound
	TST	R0,#State_Restore16
	BNE	ROCnewrestore
	MOV	R0,#1
	MOV	R1,#0
        MOV     R2,R12
	SWI	XSound_LinearHandler
	MOV	R0,#1
	STR	R0,[R12,#Work_OldEnable] ;don't enable sound later on either
	B	ROCnewepilogue
ROCnewrestore
	LDR	R1,[R12,#Work_OldLinHandler]
	LDR	R2,[R12,#Work_OldLinParam]
	MOV	R0,#1
	SWI	XSound_LinearHandler
ROCnewepilogue
	MOV	R0,#0
	STR	R0,[R12,#Work_OldLinHandler]
	MOV	R3,#0
	LDR	R1,[R12,#Work_OldSize]
	MOV	R2,#0
	MOV	R4,#0
	SWI	XSound_Configure   ;restore old buffer size
    ;;
    ;;
ROCepilogue
        BL      DisableRenderer      ;remove channel handler AFTER disabling sound!!!
	LDR	R0,[R12,#Work_OldVolume]
	SWI	XSound_Volume
	LDR	R0,[R12,#Work_OldEnable]
	SWI	XSound_Enable      ;again, enable is the last thing we do ...
    ;; ... except that on the Iyonix, the sample rate must be restored while audio is ON.
    ;; On the BeagleBoard, be sure to do this AFTER the buffer size is reconfigured.
	LDR	R1,[R12,#Work_OldFreqIndex]
	MOV	R0,#3
	SWI	XSound_SampleRate
    ;;
	LDMIA	R13!,{R0-R5,R14}
	B	ModuleReturnOK


SWIHandler
	LDR	R12,[R12]
	CMP	R11,#((SWIJumpTableEnd-SWIJumpTableStart)>>2)
	ADDLO	PC,PC,R11,LSL #2
	B	SWI_NotSupported
SWIJumpTableStart
	B	SWIActivate
	B	SWIDeactivate
	B	SWIPause
	B	SWIResume
	B	SWIGetTables
	B	SWIReadState
	B	SWINewSample
	B	SWINew16Sample
	B	SWIBufferStatistics
	B	SWINumBuffers
	B	SWIStreamSamples
	B	SWIStreamSamples16
	B	SWIStreamStatistics
	B	SWIStreamFlags
	B	SWISetDefaults
	B	SWIActivate16
	B	SWIGetFrequency
	B	SWIActivateAuto
	B	SWISampleFormat
SWIJumpTableEnd

SWI_NotSupported
	ADRL	R0,Error_BadSWI
	B	ModuleReturnError


	; Call
	; ----
	;
	; R0 = Number of Channels (bits 0-7). bit 31 ==> r3 points to buffer fill code,
	;	else use polling. All other bits reserved.
	; R1 = sample length
	; R2 = sample period
	; R3 = ^ buffer fill code if R0 bit 31 set.
	;
	; returns with all registers preserved

SWIActivate ;claim buffer, init channel handler
	STMDB	R13!,{R0-R5,R14}
	LDR	R14,[R12,#Work_Buffer]
	TEQ	R14,#0
	BNE	SWIActNoReinit
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Active
	BNE	SWIActNoReinit
	TST	R1,#15
	BNE	SWIActBuffSize
	MOV	R4,R0
	MOV	R0,#1			;switch off sound before reconfiguring
	SWI	XSound_Enable
	STR	R0,[R12,#Work_OldEnable]
	TST	R4,#(1<<31)
	MOVEQ	R3,#0
	STR	R3,[R12,#Work_CallBackFill]
	ANDS	R0,R4,#&FF
	MOVEQ	R0,#1			;limit channels to 1,2,4 or 8 (rounding up)
	TEQ	R0,#3
	MOVEQ	R0,#4
	CMP	R0,#5
	MOVHS	R0,#8
	ADD	R14,R12,#Work_Channels
	STMIA	R14,{R0-R2}
	MUL	R14,R0,R1
	TEQ	R3,#0
	; MOVNE	R2,#0			UNNECESSARY - Given that [R12,#Work_Buffer] must be NULL on entry
	; STRNE	R2,[R12,#Work_Buffer]	UNNECESSARY - Given that [R12,#Work_Buffer] must be NULL on entry
	STR	R14,[R12,#Work_TotBuffSize]
	BNE	SWIActNoBuff		;don't need a buffer in case of callback handler
	MOV	R3,R14
	MOV	R0,#6
	SWI	XOS_Module		;get RMA buffer for data
	BVS	SWIActMemory
	MOVS	R0,R2
	MOV	R1,R3
	BLNE	MemSet
	STR	R2,[R12,#Work_Buffer]
SWIActNoBuff
	MOV	R0,#0
	MOV	R1,#0
	MOV	R2,#0
	MOV	R3,#0
	MOV	R4,#0
	SWI	XSound_Configure   ;get table pointers from old Channel Handler
	ADD	R14,R12,#Work_OldChannels
	STMIA	R14,{R0-R2}		;store old sound context
	LDR	R14,[R3,#8]		;memorize lin2log table
	STR	R14,[R12,#Work_LinToLog]
	LDR	R14,[R3,#12]
	STR	R14,[R12,#Work_LogScale]
	; MOV	R3,R0
	ADD	R0,R12,#Work_Channels
	LDMIA	R0,{R0-R2}
	; CMP	R0,R3		;did we have more channels configured than we need?
	; MOVLT	R0,R3		;yes ==> keep number of channels
	; STRLT	R0,[R12,#Work_Channels]
	MOV	R3,#0
	MOV	R4,#0
	SWI	XSound_Configure
	MOV	R0,#0
	SWI	XSound_Volume
	STR	R0,[R12,#Work_OldVolume]
	MOV	R0,#(State_Active + State_NeedData)
	STR	R0,[R12,#Work_State]
	MOV	R0,#0
	STR	R0,[R12,#Work_BuffersPlayed]
	STR	R0,[R12,#Work_BuffersGiven] ;reset buffer statistics
	STR	R0,[R12,#Work_RingIsFull]
	BL	FreeRingBufferBuffers
	BL	ClaimRingBufferBuffers
	; install voice
	ADRL	R0,DigitalVoiceGenerator
	MOV	R1,#0
	SWI	XSound_InstallVoice
	STR	R0,[R12,#Work_OldVoiceName]
	STR	R1,[R12,#Work_VoiceSlot]
	TEQ	R1,#0
	BEQ	SWIActInstall
	ADD	R3,R12,#Work_OldVoices
	LDR	R4,[R12,#Work_Channels]
	MOV	R5,#0
SWIActVoiceLoop
	ADD	R0,R5,#1		;first write a pointer to workspace into the SCCB
	MOV	R1,#SCCB_PrivateOffset	;and memorize the old value
	MOV	R2,R12
	SWI	XSound_WriteControlBlock
	STR	R2,[R3,#(Work_OldCBWords-Work_OldVoices)]
	ADD	R0,R5,#1
	LDR	R1,[R12,#Work_VoiceSlot]
	SWI	XSound_AttachVoice
	STR	R1,[R3],#4
	ADD	R5,R5,#1
	CMP	R5,R4
	BLT	SWIActVoiceLoop
	; voices installed
	MOV	R0,#Format_8u
	STR	R0,[R12,#Work_SampleFormat]
	MOV	R0,#2
	SWI	XSound_Enable      ;enable sound as last action
	; fire voices
	LDR	R4,[R12,#Work_Channels]
	MOV	R0,#1
	MOV	R1,#&0180
	MOV	R2,#&8000
	MOV	R3,#&ff
SWIActFireLoop
	SWI	XSound_Control
	ADD	R0,R0,#1
	CMP	R0,R4
	BLE	SWIActFireLoop
	LDMIA	R13!,{R0-R5,R14}
	B	ModuleReturnOK
SWIActNoReinit
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R5,R14}
	ADRL	R0,Error_NoReinit
	B	ModuleReturnError
SWIActBuffSize
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R5,R14}
	ADRL	R0,Error_BadBuffSize
	B	ModuleReturnError
SWIActMemory2
	LDR	R2,[R12,#Work_Buffer]
	MOV	R0,#0
	TEQ	R2,#0
	STRNE	R0,[R12,#Work_Buffer]	; In case of IRQ, ensure memory to be freed won't be used
	MOV	R0,#7
	SWINE	XOS_Module
SWIActMemory
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R5,R14}
	ADRL	R0,Error_Memory
	B	ModuleReturnError
SWIActInstall
	ADD	R13,R13,#4
        LDMIA   R13!,{R1-R5,R14}
	ADRL	R0,Error_Install
	B	ModuleReturnError


	; Call
	; ----
	;
	; no input, all registers preserved

SWIDeactivate ;free buffer, restore channel handler
	STR	R14,[R13,#-4]!
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Active
	BEQ	SWIDecInactive
        TST     R14,#State_Deactivating         ; check that we aren't caught in a deactivation loop
        BNE     SWIDecExit
        ORR     R14,R14,#State_Deactivating     ; flag that deactivation has begun
        STR     R14,[R12,#Work_State]
	BL	RestoreOldContext
	BL	FreeRingBuffer
        ;
        ; These lines should not be necessary because RestoreOldContext has called DisableRenderer
        ; which in turn has set [R12,#Work_State] to zero.
        ;LDR    R14,[R12,#Work_State]
        ;BIC    R14,R14,#State_Deactivating
        ;STR    R14,[R12,#Work_State]
SWIDecExit
	LDR	R14,[R13],#4
	B	ModuleReturnOK
SWIDecInactive
	ADRL	R0,Error_Inactive
	LDR	R14,[R13],#4
	B	ModuleReturnError


	; Call
	; ----
	;
	; no input, all registers preserved

SWIPause ;pause playback
	STMDB	R13!,{R0,R14}
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Active
	BEQ	SWIPsInactive
	ORR	R14,R14,#State_Paused
	STR	R14,[R12,#Work_State]
	MOV	R0,#1
	SWI	XSound_Enable
	LDMIA	R13!,{R0,R14}
	B	ModuleReturnOK
SWIPsInactive
	ADRL	R0,Error_Inactive
	ADD	R13,R13,#4
	LDR	R14,[R13],#4
	B	ModuleReturnError


	; Call
	; ----
	;
	; no input, all registers preserved

SWIResume ;resume playback
	STMDB	R13!,{R0,R14}
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Active
	BEQ	SWIRsInactive
	BIC	R14,R14,#State_Paused
	STR	R14,[R12,#Work_State]
	MOV	R0,#2
	SWI	XSound_Enable
	LDMIA	R13!,{R0,R14}
	B	ModuleReturnOK
SWIRsInactive
	ADRL	R0,Error_Inactive
	ADD	R13,R13,#4
	LDR	R14,[R13],#4
	B	ModuleReturnError


	; Call
	; ----
	;
	; no input. Returns with R0 = ^LinToLog, R1 = ^LogScale

SWIGetTables ;get pointers to LinToLog and LogScale tables
	LDR	R1,[R12,#Work_State]
	TST	R1,#State_Active
	BEQ	SWIGTInactive
	LDR	R1,[R12,#Work_LogScale]
	LDR	R0,[R12,#Work_LinToLog]
	B	ModuleReturnOK
SWIGTInactive
	ADRL	R0,Error_Inactive
	B	ModuleReturnError


	; Call
	; ----
	;
	; no input. Returns Work_State in R0

SWIReadState ;read state (has valid buffer yes/no)
	LDR	R0,[R12,#Work_State]
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = pointer to buffer with sample, all registers preserved

SWINewSample ;read sample from external buffer into internal buffer
	STMDB	R13!,{R0-R3,R14}
	LDR	R3,[R12,#Work_State]
	TST	R3,#State_Active
	BEQ	SWINSInactive
	LDR	R1,[R12,#Work_Buffer]
	BIC	R3,R3,#State_NeedData
	STR	R3,[R12,#Work_State]
	TEQ	R1,#0
	BEQ	SWINSInactive
	LDR	R2,[R12,#Work_BuffersGiven]
	TST	R3,#State_NewSound
	ADD	R2,R2,#1
	STR	R2,[R12,#Work_BuffersGiven]
	LDR	R2,[R12,#Work_TotBuffSize]
	BNE	SNSnewsound
	BL	CopyByteBuffer
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK
SNSnewsound
	MOV	R2,R2,LSR #1		;this call needs the number of _samples_!
	BL	CopyByteToShortBuffer
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK
SWINSInactive
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R3,R14}
	ADRL	R0,Error_Inactive
	B	ModuleReturnError


	; Call
	; ----
	;
	; R0 = pointer to buffer holding sampled (16 bit signed), all registers preserved

SWINew16Sample
	STMDB	R13!,{R0-R3,R14}
	LDR	R3,[R12,#Work_State]
	TST	R3,#State_Active
	BEQ	SWINS16Inactive
	LDR	R1,[R12,#Work_Buffer]
	BIC	R3,R3,#State_NeedData
	STR	R3,[R12,#Work_State]
	TEQ	R1,#0
	BEQ	SWINS16Inactive
	LDR	R2,[R12,#Work_BuffersGiven]
	TST	R3,#State_NewSound
	ADD	R2,R2,#1
	STR	R2,[R12,#Work_BuffersGiven]
	LDR	R2,[R12,#Work_TotBuffSize]
	BNE	SNS16newsound
	BL	CopyShortBuffer
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK
SNS16newsound
	MOV	R2,R2,LSR #1		;this call needs the number of _samples_!
	BL	CopyShortToShortBuffer
	LDMIA	R13!,{R0-R3,R14}
	B	ModuleReturnOK
SWINS16Inactive
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R3,R14}
	ADRL	R0,Error_Inactive
	B	ModuleReturnError


	; Call
	; ----
	;
	; returns R0 = "BuffersPlayed", R1 = "BuffersGiven"

SWIBufferStatistics
	LDR	R0,[R12,#Work_BuffersPlayed]
	LDR	R1,[R12,#Work_BuffersGiven]
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = number of buffers to use, or -1 to read.
	; returns number of buffers in R0

SWINumBuffers
	STMDB	R13!,{R1-R4,R14}
	MOVS	R4,R0
	LDR	R0,[R12,#Work_NumBuffers]
	BMI	SWINBexit
	TEQ	R0,R4
	BEQ	SWINBexit
	BL	FreeRingBuffer
	CMP	R4,#0
	BLE	SWINBexit
	MOV	R3,R4,LSL #2
	MOV	R0,#6
	SWI	XOS_Module
	BVS	SWINBerrmem
	MOV	R14,#0
SWINBclearloop
	SUBS	R3,R3,#4
	STR	R14,[R2,R3]
	BGT	SWINBclearloop
	STR	R4,[R12,#Work_NumBuffers]
	STR	R2,[R12,#Work_RingBuffer] ;must be safe against IRQ fill code
	MOV	R0,R4
SWINBexit
	LDMIA	R13!,{R1-R4,R14}
	B	ModuleReturnOK
SWINBerrmem
	LDMIA	R13!,{R1-R4,R14}
	MVN	R0,#0
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = pointer to buffer holding 8bit samples, r1 = number of samples
	; no return value

SWIStreamSamples
	STMDB	R13!,{R0-R11,R14}
	LDR	R2,[R12,#Work_State]
	MOV	R10,#1
	TST	R2,#State_NewSound
	BNE	SSSnewsound
	MOV	R11,#1
	ADRL	R9,CopyByteBuffer
	B	StreamSampleCore
SSSnewsound
	MOV	R11,#2
	ADRL	R9,CopyByteToShortBuffer
	B	StreamSampleCore


	; Call
	; ----
	;
	; R0 = pointer to buffer holding 16bit samples, r1 = number of samples
	; no return value

SWIStreamSamples16
	STMDB	R13!,{R0-R11,R14}
	LDR	R2,[R12,#Work_State]
	MOV	R10,#2
	TST	R2,#State_NewSound
	BNE	SSS16newsound
	MOV	R11,#1
	ADRL	R9,CopyShortBuffer
	B	StreamSampleCore
SSS16newsound
	MOV	R11,#2
	ADRL	R9,CopyShortToShortBuffer
	B	StreamSampleCore


	; Call
	; ----
	;
	; returns number of filled buffers

SWIStreamStatistics
	STMDB	R13!,{R1,R14}
	LDR	R0,[R12,#Work_WriteBuffer]
	LDR	R1,[R12,#Work_ReadBuffer]
	SUBS	R0,R0,R1
	LDR	R1,[R12,#Work_NumBuffers]
	BEQ	SWISSequal		;if both are equal check for the RingFull status
	ADDLT	R0,R0,R1
	LDR	R1,[R12,#Work_FillLevel] ;if any data was written to the buffer, add 1
	TEQ	R1,#0
	ADDNE	R0,R0,#1
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnOK
SWISSequal
	LDR	R0,[R12,#Work_RingIsFull]
	TEQ	R0,#0
	MOVNE	R0,R1			;if the ring is full, return num buffers, otherwise 0
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = eor mask, r1 = and mask (applied first), (0,-1) to read
	; returns old flags in r0

SWIStreamFlags
	STMDB	R13!,{R1,R14}
	LDR	R14,[R12,#Work_StreamFlags]
	AND	R1,R1,R14
	EOR	R1,R1,R0
	STR	R1,[R12,#Work_StreamFlags]
	MOV	R0,R14
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = channels, R1 = format, R2 = sample period, R3 = sample length, R4 = buffers
	; R5 = frequency, or for each parameter 0 to read
	; returns with the currently valid configuration

SWISetDefaults
	TEQ	R0,#0
	LDREQ	R0,[R12,#Work_DfltChannels]
	STRNE	R0,[R12,#Work_DfltChannels]
	TEQ	R1,#0
	LDREQ	R1,[R12,#Work_DfltFormat]
	STRNE	R1,[R12,#Work_DfltFormat]
	TEQ	R2,#0
	LDREQ	R2,[R12,#Work_DfltPeriod]
	STRNE	R2,[R12,#Work_DfltPeriod]
	TEQ	R3,#0
	LDREQ	R3,[R12,#Work_DfltBuffSize]
	STRNE	R3,[R12,#Work_DfltBuffSize]
	TEQ	R4,#0
	LDREQ	R4,[R12,#Work_DfltBuffers]
	STRNE	R4,[R12,#Work_DfltBuffers]
	TEQ	R5,#0
	LDREQ	R5,[R12,#Work_DfltFrequency]
	STRNE	R5,[R12,#Work_DfltFrequency]
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = number of channels (1 or 2), R1 = buffer size, R2 = frequency, R3 = flags
	; flags is a bitfield using 0 (restore old handler on exit)
	; returns with all registers preserved

SWIActivate16
	STMDB	R13!,{R0-R7,R14}
	LDR	R14,[R12,#Work_Buffer]
	TEQ	R14,#0
	BNE	SWIAct16Reinit
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Active
	BNE	SWIAct16Reinit
	TEQ	R0,#1
	TEQNE	R0,#2
	BNE	SWIAct16Channels
	STR	R0,[R12,#Work_Channels]
	STR	R1,[R12,#Work_BuffSize]
	STR	R2,[R12,#Work_Frequency]
	MUL	R0,R1,R0
	TST	R3,#1
	MOVEQ	R3,#0
	MOVNE	R3,#State_Restore16
	STR	R3,[R12,#Work_State]
	MOV	R3,R0,LSL #1
	STR	R3,[R12,#Work_TotBuffSize] 	; total buffer size = num channels * buffsize * 2
	LDR	R2,[R12,#Work_Buffer]
	MOV	R7,#0
	TEQ	R2,#0
	STRNE	R7,[R12,#Work_Buffer]		; In case of IRQ, ensure memory to be freed won't be used
	MOV	R0,#7
	SWINE	XOS_Module			; Avoid RMA memory leaks! First, free any old block
	MOV	R0,#6
	SWI	XOS_Module
	BVS	SWIAct16Memory
	STR	R2,[R12,#Work_Buffer]
	MOVS	R0,R2
	MOV	R1,R3
	BLNE	MemSet
	STR	R7,[R12,#Work_BuffersPlayed]
	STR	R7,[R12,#Work_BuffersGiven]
	STR	R7,[R12,#Work_RingIsFull]
	BL	FreeRingBufferBuffers
	BL	ClaimRingBufferBuffers
    ;;
    ;;
	MOV	R0,#0
	SWI	XSound_Mode
	BVS	SWIAct16GenErr
	TST	R0,#1
	BEQ	SWIAct16NotAvailable
    ;;
    ;;
	MOV	R0,#1
	SWI	XSound_Enable			; switch off sound before reconfiguring
	STRVC	R0,[R12,#Work_OldEnable]
    ;;
    ;;
	MOV	R0,#0
	SWI	XSound_Volume
	STRVC	R0,[R12,#Work_OldVolume]
    ;;
    ;; On the BeagleBoard, we must read the current sample rate now ...
	MOV	R0,#1
	SWI	XSound_SampleRate
	BVS	SWIAct16GenErr
	STR	R1,[R12,#Work_OldFreqIndex]	; this is the index, not the actual frequency (r2)!
    ;;
	MOV	R0,#0
	SWI	XSound_SampleRate		; read number of sample rates to r1
	BVS	SWIAct16GenErr
	LDR	R6,[R12,#Work_Frequency]
	MVN	R5,#0				; error so far (unsigned)
	MOV	R4,#0				; best match so far
	MOV	R0,#2
SWIAct16FindRate ; primitive linear search...
	SWI	XSound_SampleRate
	MOVVS	R2,#0
	SUBS	R3,R2,R6,LSL #10
	RSBLT	R3,R3,#0
	CMPNE	R3,R5
	MOVLS	R5,R3
	MOVLS	R4,R1
	MOVLS	R7,R2,LSR #10
	SUBNES  R1,R1,#1
	BNE	SWIAct16FindRate		; sample rate indices are 1-based
	TEQ	R4,#0
	BEQ	SWIAct16NoRate
	STR	R7,[R12,#Work_Frequency]
	STR	R4,[R12,#Work_FreqIndex]
    ;;
    ;; ... BEFORE configuring the DMA buffer size because this will maximise the sample rate!
	LDR	R1,[R12,#Work_BuffSize]
	MOV	R0,#0
	MOV	R2,#0
	MOV	R3,#0
	MOV	R4,#0
	SWI	XSound_Configure		; try to configure the buffer size
	BVS	SWIAct16GenErr
	STR	R1,[R12,#Work_OldSize]
    ;;
    ;;
	MOV	R0,#1
	ADRL	R1,LinearHandlerCode
	MOV	R2,R12
	SWI	XSound_LinearHandler		; install new linear handler, remember old one
	BVS	SWIAct16GenErr
	STR	R1,[R12,#Work_OldLinHandler]
	STR	R2,[R12,#Work_OldLinParam]
    ;;
    ;;
	MOV	R0,#2
	SWI	XSound_Enable			; turn on sound
	BVS	SWIAct16GenErr
    ;;
    ;; On the Iyonix, it is only NOW that setting the sample rate works properly!
    ;; Audio must be ON for any changing of the sample rate to have an effect on audio output.
    ;; Confusingly, if the rate is changed while '*audio off', RISC OS will report the new rate
    ;; as current while playing sounds at the old rate!
        LDR     R1,[R12,#Work_FreqIndex]
        MOV     R0,#3
        SWI     XSound_SampleRate               ; switch on the new sample rate we calculated above
    ;;
	MOV	R0,#Format_16sle_rl
	STR	R0,[R12,#Work_SampleFormat]
	LDR	R0,[R12,#Work_State]
	ORR	R0,R0,#(State_Active + State_NeedData + State_NewSound)
	STR	R0,[R12,#Work_State]
	LDMIA	R13!,{R0-R7,R14}
	B	ModuleReturnOK
SWIAct16Reinit
	ADRL	R0,Error_NoReinit
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R7,R14}
	B	ModuleReturnError
SWIAct16NotAvailable
	ADRL	R0,Error_No16Bit
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R7,R14}
	B	ModuleReturnError
SWIAct16NoRate
	ADRL	R0,Error_NoRate
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R7,R14}
	B	ModuleReturnError
SWIAct16Channels
	ADRL	R0,Error_Channels16
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R7,R14}
	B	ModuleReturnError
SWIAct16Memory
	ADRL	R0,Error_Memory
SWIAct16GenErr
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R7,R14}
	B	ModuleReturnError


	; Call
	; ----
	;
	; returns frequency in r0, or 0 if inactive

SWIGetFrequency
	STMDB	R13!,{R1,R14}
	LDR	R0,[R12,#Work_State]
	TST	R0,#State_Active
	MOVEQ	R0,#0
	BEQ	SWIGFexit
	TST	R0,#State_NewSound
	LDRNE	R0,[R12,#Work_Frequency]
	BNE	SWIGFexit
	LDR	R0,Constant_Mega
	LDR	R1,[R12,#Work_Period]
	ADD	R0,R0,R1,LSR #1		;for rounding
	BL	DoIntegerDivision
SWIGFexit
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnOK


	; Call
	; ----
	;
	; R0 = number of channels, R1 = buffer size, R2 = frequency
	; returns with all registers preserved

SWIActivateAuto
	STMDB	R13!,{R0-R4,R14}
	MOV	R4,R0
	MOV	R3,#1				;restore old handler
	BL	SWIActivate16
	BVC	SWIAAexit
	MOV	R3,R1
	LDR	R0,Constant_Mega
	MOV	R1,R2
	ADD	R0,R0,R1,LSR #1		;for rounding
	BL	DoIntegerDivision
	MOV	R2,R0
	MOV	R0,R4
	MOV	R1,R3
	BL	SWIActivate
	BVS	SWIAAGenErr
SWIAAexit
	LDMIA	R13!,{R0-R4,R14}
	B	ModuleReturnOK
SWIAAGenErr
	ADD	R13,R13,#4
	LDMIA	R13!,{R1-R4,R14}
	B	ModuleReturnError


	; Call
	; ----
	;
	; R0 = new sample format or 0 to read
	; returns sample format

SWISampleFormat
	TEQ	R0,#0
	LDREQ	R0,[R12,#Work_SampleFormat]
	STRNE	R0,[R12,#Work_SampleFormat]
	B	ModuleReturnOK


Constant_Mega ;used for conversions period <--> frequency
	&	1000000


SWIDecodingTable
	=	"DigitalRenderer",0
	=	"Activate",0
	=	"Deactivate",0
	=	"Pause",0
	=	"Resume",0
	=	"GetTables",0
	=	"ReadState",0
	=	"NewSample",0
	=	"New16BitSample",0
	=	"BufferStatistics",0
	=	"NumBuffers",0
	=	"StreamSamples",0
	=	"Stream16BitSamples",0
	=	"StreamStatistics",0
	=	"StreamFlags",0
	=	"SetDefaults",0
	=	"Activate16",0
	=	"GetFrequency",0
	=	"ActivateAuto",0
	=	"SampleFormat",0
	=	0
	ALIGN


	;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;; 8bit sound handler ;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;

DigitalVoiceGenerator
	B	VoiceFillCode		;fill code
	B	VoiceFillCode		;update code
	B	VoiceFillCode		;gate on code
	B	VoiceFillCode		;gate off code
	LDR	PC,[R13],#4
	LDR	PC,[R13],#4
	LDR	PC,[R13],#4
	&	VoiceGeneratorName - DigitalVoiceGenerator
VoiceGeneratorName
	=	"DigitalRenderer",0
	ALIGN


VoiceFillCode
	TEQ	R7,#0			;only do something for the first channel
	BNE	CHFCexit
	LDR	R0,[R9,#SCCB_PrivateOffset]   ;retrieve module workspace pointer
	FNldr2	R1,DRenderMagic
	LDR	R4,[R0,#Work_MagicWord]
	TEQ	R1,R4
	BNE	CHFCexit		;the private word pointer got corrupted somehow!
	LDR	R1,[R0,#Work_State]
	TST	R1,#State_NeedData
	ORRNE	R1,R1,#State_Overflow		;if data needed flag set, overflow occurred
	BICEQ	R1,R1,#State_Overflow
	ORR	R1,R1,#State_NeedData
	STR	R1,[R0,#Work_State]
	LDR	R1,[R0,#Work_BuffersPlayed]
	ADD	R1,R1,#1
	STR	R1,[R0,#Work_BuffersPlayed]
	SUB	R1,R10,R12
	STR	R1,[R0,#Work_DMABuffSize]
	LDR	R4,[R0,#Work_CallBackFill]
	TEQ	R4,#0
	BNE	CHFCcallback
	LDR	R4,[R0,#Work_NumBuffers]      ;stream mode?
	TEQ	R4,#0
	BNE	CHFCstreaming
	LDR	R0,[R0,#Work_Buffer]		;R0 = ^internal buffer
	TEQ	R0,#0
	BEQ	CHFCexit
CHFCBegin
	SUB	R4,R10,R12
	CMP	R4,#32
	BLT	CHFCsmall
CHFCbig
	LDMIA	R0!,{R1,R2,R5-R9,R11}
	STMIA	R12!,{R1,R2,R5-R9,R11}
	SUB	R4,R4,#32
	CMP	R4,#32
	BGE	CHFCbig
CHFCsmall
	TST	R4,#16
	LDMNEIA R0!,{R5-R8}
	STMNEIA R12!,{R5-R8}
	TST	R4,#8
	LDMNEIA R0!,{R1,R2}
	STMNEIA R12!,{R1,R2}
	TST	R4,#4
	LDRNE	R1,[R0],#4
	STRNE	R1,[R12],#4
CHFCDone
CHFCexit
	MOV	R0,#(1<<3)		;active (normal fill)
	LDR	PC,[R13],#4

CHFCovernull
	MOV	R0,#0
	STR	R0,[R5,#Work_RingIsFull]
	MOV	R0,R12
	FNlink	R1,CHFCexit
	STR	R1,[R13,#-4]!
	SUB	R1,R10,R12
	B	MemStIRQ			;CAN'T USE R14 IN IRQ MODE!

CHFCcallback
	FNlink	R1,CHFCDone
	STR	R1,[R13,#-4]!
	LDR	R1,[R0,#Work_LinToLog]
	LDR	R3,[R0,#Work_CallBackFill]
	SUB	R2,R10,R12		;R2 = buffer size
	MOV	R0,R12
	MOV	PC,R3			;call handler

CHFCstreaming ;R4 = num buffers
	LDR	R3,[R0,#Work_RingBuffer]
        LDR     R8,[R0,#Work_TotBuffSize]
	MOV	R5,R0
	MOV	R0,#1
	STR	R0,[R5,#Work_PollWord]		;on every buffer refill loop, set poll word
	TEQ	R3,#0
	BEQ	CHFCovernull
	LDR	R0,[R5,#Work_WriteBuffer]
	LDR	R6,[R5,#Work_ReadBuffer]
	TEQ	R0,R6
	BNE	CHFCloopstream
	LDR	R0,[R5,#Work_RingIsFull]      ;is the ring empty or completely full?
	TEQ	R0,#0
	BNE	CHFCloopstream
CHFCchkoveract
	LDR	R0,[R5,#Work_StreamFlags]
	TST	R0,#StrFlg_OverrunNull
	BNE	CHFCovernull
	LDR	R6,[R5,#Work_LastReadBuff]
	CMP	R6,R4
	MOVHS	R6,#0				;in case LastReadBuff >= NumBuffers...
	MOV	R4,#0				;buffer overrun ==> set R4 to 0
	STR	R4,[R5,#Work_ReadLevel]
CHFCloopstream
	LDR	R0,[R3,R6,LSL #2]
	TEQ	R0,#0
	BEQ	CHFCexit
	LDR	R7,[R5,#Work_ReadLevel]
	CMP	R7,R8
	MOVHS	R7,#0				;additional security
	ADD	R0,R0,R7
	SUB	R1,R8,R7			;bytes remaining in source buffer
	SUB	R2,R10,R12			;bytes remaining in DMA buffer
	CMP	R2,R1
	MOVHI	R2,R1
	FNlink	R1,CHFCcopyreturn		;IRQ CODE STORES RETURN ADDRESS BEFORE CALL!
	STR	R1,[R13,#-4]!
	MOV	R1,R12
	B	CopyByteBuffIRQ			;CAN'T USE R14 IN IRQ MODE!!!
CHFCcopyreturn
	ADD	R12,R12,R2
	STR	R6,[R5,#Work_LastReadBuff]
	ADD	R7,R7,R2			;increment read level
	CMP	R7,R8
	STRLO	R7,[R5,#Work_ReadLevel]
	BLO	CHFCnobuffinc
	MOV	R7,#0
	STR	R7,[R5,#Work_ReadLevel]
	TEQ	R4,#0
	BEQ	CHFCnobuffinc			;buffer overrun ==> don't increment read buffer
	ADD	R6,R6,#1
	CMP	R6,R4
	MOVHS	R6,#0
	STR	R6,[R5,#Work_ReadBuffer]
	LDR	R0,[R5,#Work_WriteBuffer]	;if ReadBuffer == WriteBuffer after incrementing
	TEQ	R6,R0				;the buffer is completely empty.
	BNE	CHFCnobuffinc
	MOV	R0,#0
	STR	R0,[R5,#Work_RingIsFull]	;make a note for StreamStatistics
	CMP	R12,R10
	BLO	CHFCchkoveract
CHFCnobuffinc
	CMP	R12,R10
	BLO	CHFCloopstream
	B	CHFCexit


	;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;; 16bit sound handler ;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;

LinearHandlerCode
	STMFD	R13!,{R11,R12,R14}
	MOV	R12,R0
	LDR	R0,[R12,#Work_State]
	MOV	R5,R3				;if (flags in r5 AND 7) == 2 then buffer is preset to silence
	TST	R0,#State_NeedData
        ORRNE   R0,R0,#State_Overflow           ;if data needed flag set, overflow occurred
	BICEQ	R0,R0,#State_Overflow
	ORR	R0,R0,#State_NeedData
	STR	R0,[R12,#Work_State]
	LDR	R0,[R12,#Work_BuffersPlayed]
	SUB	R9,R2,R1			;number of bytes to fill in r9
	LDR	R4,[R12,#Work_NumBuffers]
	ADD	R0,R0,#1
	STR	R0,[R12,#Work_BuffersPlayed]
	STR	R9,[R12,#Work_DMABuffSize]
	TEQ	R4,#0
	BNE	LHCstreaming
	LDR	R0,[R12,#Work_Buffer]
	TEQ	R0,#0
	BEQ	LHCovernull
	LDR	R3,[R12,#Work_Channels]
	LDR	R4,[R12,#Work_TotBuffSize]
	TEQ	R3,#1
	MOVEQ	R4,R4,LSL #1
	CMP	R9,R4
        MOVHI   R2,R4
        MOVLS   R2,R9
        STR	PC,[R13,#-4]!			;return address could be (.+8) or (.+12)
	B	CopyLinearBuffer		;does 1->2 channel expansion if necessary
	NOP					;in case stored PC is (.+12)
	LDMFD	R13!,{R11,R12,PC}		;DOESN'T RESTORE FLAGS! IRQ SAFE!

LHCstreaming ;streaming interface, similar to Voice handler
	LDR	R3,[R12,#Work_RingBuffer]
	LDR	R8,[R12,#Work_TotBuffSize]
	MOV	R0,#1
	STR	R0,[R12,#Work_PollWord]
	TEQ	R3,#0
	BEQ	LHCovernull
	LDR	R0,[R12,#Work_WriteBuffer]
	LDR	R6,[R12,#Work_ReadBuffer]
	LDR	R10,[R12,#Work_Channels]	;number of channels in r10
	TEQ	R0,R6
	BNE	LHCloopstream
	LDR	R0,[R12,#Work_RingIsFull]
	TEQ	R0,#0
	BNE	LHCloopstream
LHCchkoveract
	LDR	R0,[R12,#Work_StreamFlags]
	TST	R0,#StrFlg_OverrunNull
	BNE	LHCovernull
	LDR	R6,[R12,#Work_LastReadBuff]
	CMP	R6,R4
	MOVHS	R6,#0
	MOV	R4,#0
	STR	R4,[R12,#Work_ReadLevel]
LHCloopstream
  ;; vvvv
  	LDR	R0,[R12,#Work_WriteBuffer]
  	TEQ	R0,R6
  	LDREQ	R0,[R12,#Work_FillLevel]
  	MOVNE	R0,#0
  	TEQ	R0,#0
  	MOVNE	R8,R0
  ;; ^^^^
	LDR	R0,[R3,R6,LSL #2]
	TEQ	R0,#0
	BEQ	LHCovernull
	LDR	R7,[R12,#Work_ReadLevel]
	CMP	R7,R8
	MOVHS	R7,#0
	ADD	R0,R0,R7
	SUB	R2,R8,R7
	TEQ	R10,#1
	MOVEQ	R2,R2,LSL #1			;1 channel ==> pretend twice as much audio data
	CMP	R2,R9
	MOVHI	R2,R9
        STR	PC,[R13,#-4]!			;return address could be (.+8) or (.+12)
	B	CopyLinearBuffer		;does 1->2 channel expansion if necessary
	NOP					;in case stored PC is (.+12)
	ADD	R1,R1,R2
	SUB	R9,R9,R2
	STR	R6,[R12,#Work_LastReadBuff]
	TEQ	R10,#1
	ADDEQ	R7,R7,R2,LSR #1
	ADDNE	R7,R7,R2
	CMP	R7,R8
	STRLO	R7,[R12,#Work_ReadLevel]
	BLO	LHCnobuffinc
	MOV	R7,#0
	STR	R7,[R12,#Work_ReadLevel]
	TEQ	R4,#0
	BEQ	LHCnobuffinc
	ADD	R6,R6,#1
	CMP	R6,R4
	MOVHS	R6,#0
	LDR	R0,[R12,#Work_WriteBuffer]
	STR	R6,[R12,#Work_ReadBuffer]
	TEQ	R6,R0
	BNE	LHCnobuffinc
	MOV	R0,#0
	STR	R0,[R12,#Work_RingIsFull]
	TEQ     R9,#0
	BNE     LHCchkoveract
LHCnobuffinc
        TEQ     R9,#0
        BNE     LHCloopstream
LHCovernull
	LDMFD	R13!,{R11,R12}			;restore registers that must be preserved but leave return address stacked
	AND	R5,R5,#7
	TEQ	R5,#2				;only call MemSet if the OS hasn't already cleared the buffer
	MOVNE	R0,R1
	MOVNES	R1,R9				;and there is still space remaining in the buffer to be filled
	BNE	MemStIRQ
	LDR	PC,[R13],#4			;DOESN'T RESTORE FLAGS! IRQ SAFE!


FreeRingBufferBuffers ;R12 = ^workspace
	STMDB	R13!,{R0-R4,R14}
	LDR	R1,[R12,#Work_NumBuffers]
	LDR	R3,[R12,#Work_RingBuffer]
	SUBS	R1,R1,#1
	BLT	FRBBexit
	MOV	R0,#7
	MOV	R4,#0
FRBBloop
	LDR	R2,[R3,R1,LSL #2]
	TEQ	R2,#0
	STRNE	R4,[R3,R1,LSL #2]     ;must be safe against IRQ fill code!
	SWINE	XOS_Module
	SUBS	R1,R1,#1
	BGE	FRBBloop
FRBBexit
	LDMIA	R13!,{R0-R4,R14}
	B	ModuleReturnOK


FreeRingBuffer
	STMDB	R13!,{R0-R2,R14}
	LDR	R2,[R12,#Work_RingBuffer]
	TEQ	R2,#0
	BEQ	FRBexit
	BL	FreeRingBufferBuffers
	MOV	R0,#0
	STR	R0,[R12,#Work_NumBuffers]
	STR	R0,[R12,#Work_RingBuffer] ;must be safe against IRQ fill code!
	MOV	R0,#7
	SWI	XOS_Module
FRBexit
	LDMIA	R13!,{R0-R2,R14}
	B	ModuleReturnOK


ClaimRingBufferBuffers ;R12 = ^workspace
	STMDB	R13!,{R1-R6,R14}
	LDR	R5,[R12,#Work_NumBuffers]
	SUBS	R5,R5,#1
	BLT	CRBexit
	LDR	R4,[R12,#Work_RingBuffer]
	TEQ	R4,#0
	BEQ	CRBexit
	LDR	R3,[R12,#Work_TotBuffSize]
	MOV	R6,#0
CRBloop
	LDR	R2,[R4,R5,LSL #2]
	TEQ	R2,#0
	STRNE	R6,[R4,R5,LSL #2]	; In case of IRQ, ensure memory to be freed won't be used
	MOV	R0,#7
	SWINE	XOS_Module
	MOV	R0,#6
	SWI	XOS_Module
	BVS	CRBerror
	MOVS	R0,R2
	MOV	R1,R3
	BLNE	MemSet
	STR	R2,[R4,R5,LSL #2]
	SUBS	R5,R5,#1
	BGE	CRBloop
CRBexit
	MOV	R0,#0
	STR	R0,[R12,#Work_RingIsFull]
	STR	R0,[R12,#Work_ReadBuffer]
	STR	R0,[R12,#Work_WriteBuffer]
	STR	R0,[R12,#Work_FillLevel]
	STR	R0,[R12,#Work_ReadLevel]
	LDMIA	R13!,{R1-R6,R14}
	B	ModuleReturnOK
CRBerror
	MVN	R0,#0
	LDMIA	R13!,{R1-R6,R14}
	B	ModuleReturnOK


StreamSampleCore ;r9 pointer to copy code (w/out 26-bit PSR), r10 src sample size
	LDR	R8,[R12,#Work_RingBuffer] ;r11 dest sample size
	TEQ	R8,#0
	BEQ	SSCexit
	MOVS	R3,R1
	BLE	SSCexit
	LDR	R1,[R12,#Work_ReadBuffer]
	LDR	R4,[R12,#Work_TotBuffSize]
	LDR	R5,[R12,#Work_WriteBuffer]
	LDR	R6,[R12,#Work_NumBuffers]
	LDR	R7,[R12,#Work_FillLevel]
	TEQ	R1,R5
	BNE	SSCmainloop
	LDR	R14,[R12,#Work_RingIsFull]
	TEQ	R14,#0
	MOVNE	R2,#0
	BNE	SSCcheckblock
SSCmainloop
	LDR	R1,[R8,R5,LSL #2]		;ringbuffer[writebuffer]
	SUB	R2,R4,R7			;buffer size - fill level = bytes in buffer
	TEQ	R11,#1				;but we need samples!
	MOVNE	R2,R2,LSR #1			;in case we're using 16bit sound samples = bytes/2
	CMP	R2,R3
        MOVHI   R2,R3
	TEQ	R1,#0
	BEQ	SSCskipbuffer
	ADD	R1,R1,R7			;add fill level to dest buffer
	MOV	R14,PC				;return to SSCskipbuffer (called code doesn't preserve flags)
	MOV	PC,R9				;dynamic BL to the copy code
SSCskipbuffer
	MLA	R7,R11,R2,R7			;dest = dest + dest_sample_size * samples
	MLA	R0,R10,R2,R0			;src = src + src_sample_size * samples
	STR	R7,[R12,#Work_FillLevel]
	CMP	R7,R4
	BLT	SSCexit
	MOV	R7,#0
	STR	R7,[R12,#Work_FillLevel]
	ADD	R5,R5,#1
	CMP	R5,R6
	MOVGE	R5,#0
	LDR	R1,[R12,#Work_ReadBuffer]  ;if WriteBuffer == ReadBuffer _after_ incrementing,
	STR	R5,[R12,#Work_WriteBuffer]
	TEQ	R5,R1				;the ring buffer is filled completely.
	BNE	SSCspaceleft
	MOV	R14,#1
	STR	R14,[R12,#Work_RingIsFull] ;make a note of this (for StreamStatistics)
SSCcheckblock
	LDR	R14,[R12,#Work_StreamFlags]
	TST	R14,#StrFlg_OverBlock		;what to do on an overflow; block?
	BEQ	SSCexit			;no blocking ==> truncate
	LDR	R14,[R12,#Work_State]
	TST	R14,#State_Paused
	BNE	SSCexit			;if paused, we must NOT block!
SSCblockloop
	LDR	R14,[R12,#Work_ReadBuffer]
	TEQ	R14,R1				;R1 is still the current read buffer
	BEQ	SSCblockloop
SSCspaceleft
	SUBS	R3,R3,R2
	BGT	SSCmainloop
SSCexit
	LDMIA	R13!,{R0-R11,R14}
	B	ModuleReturnOK


	; THIS CODE MUST BE IRQ SAFE!
CopyByteBuffer	;r0 = src, r1 = dest, r2 = size
	STR	R14,[R13,#-4]!
CopyByteBuffIRQ ;in IRQ mode, the caller must store the return address
	CMP	R2,#0
	LDRLE	PC,[R13],#4      ;DON'T RESTORE FLAGS! IRQ SAFE!
	STMDB	R13!,{R0-R7}
CBBdoalign
	TST	R1,#3
	BEQ	CBBisaligned
	LDRB	R3,[R0],#1
	SUBS	R2,R2,#1
	STRB	R3,[R1],#1
	BGT	CBBdoalign
CBBisaligned
	TEQ	R2,#0
	BEQ	CBBexit
	AND	R3,R0,#3
	ADD	PC,PC,R3,LSL #2
	MOV	R0,R0
	B	CopyByteBuffer0
	B	CopyByteBuffer1
	B	CopyByteBuffer2
	B	CopyByteBuffer3

CopyByteBuffer0
	ROUT
	SUBS	R2,R2,#16
	BLT	%20
10 ;big
	LDMIA	R0!,{R3-R6}
	STMIA	R1!,{R3-R6}
	SUBS	R2,R2,#16
	BGE	%10
20 ;small
	ADDS	R2,R2,#16
	BLE	CBBexit
	TST	R2,#8
	LDMNEIA R0!,{R3,R4}
	STMNEIA R1!,{R3,R4}
	TST	R2,#4
	LDRNE	R3,[R0],#4
	STRNE	R3,[R1],#4
	ANDS	R2,R2,#3
	BEQ	CBBexit
CBBtail
	LDRB	R3,[R0],#1
	SUBS	R2,R2,#1
	STRB	R3,[R1],#1
	BGT	CBBtail
CBBexit
	LDMIA	R13!,{R0-R7,PC}      ;DON'T RESTORE FLAGS! IRQ SAFE!
	ROUT

	MACRO
$label	FNCopyBytes	$shift
$label	ROUT
	BIC	R0,R0,#3
	LDR	R3,[R0],#4
	SUBS	R2,R2,#16
	MOV	R3,R3,LSR #($shift*8)
	BLT	%20$label
10 ;big
	LDMIA	R0!,{R4-R7}
	ORR	R3,R3,R4,LSL #(32-8*$shift)
	MOV	R4,R4,LSR #8*$shift
	ORR	R4,R4,R5,LSL #(32-8*$shift)
	MOV	R5,R5,LSR #8*$shift
	ORR	R5,R5,R6,LSL #(32-8*$shift)
	MOV	R6,R6,LSR #8*$shift
	ORR	R6,R6,R7,LSL #(32-8*$shift)
	STMIA	R1!,{R3-R6}
	MOV	R3,R7,LSR #8*$shift
	SUBS	R2,R2,#16
	BGE	%10$label
20 ;small
	ADDS	R2,R2,#16
	BLE	CBBexit
	TST	R2,#8
	BEQ	%30$label
	LDMIA	R0!,{R4,R5}
	ORR	R3,R3,R4,LSL #(32-8*$shift)
	MOV	R4,R4,LSR #8*$shift
	ORR	R4,R4,R5,LSL #(32-8*$shift)
	STMIA	R1!,{R3,R4}
	MOV	R3,R5,LSR #8*$shift
30 ;tiny
	TST	R2,#4
	LDRNE	R4,[R0],#4
	ORRNE	R3,R3,R4,LSL #(32-8*$shift)
	STRNE	R3,[R1],#4
	MOVNE	R3,R4,LSR #8*$shift
	ANDS	R2,R2,#3
	BEQ	CBBexit
	CMP	R2,#(4-$shift)
	LDRGT	R4,[R0],#4
	ORRGT	R3,R3,R4,LSL #(32-8*$shift)
40 ;rest
	STRB	R3,[R1],#1
	MOV	R3,R3,LSR #8
	SUBS	R2,R2,#1
	BGT	%40$label
	LDMIA	R13!,{R0-R7,PC}		;DON'T RESTORE FLAGS! IRQ SAFE!
	ROUT
	MEND

CopyByteBuffer1	FNCopyBytes	1

CopyByteBuffer2	FNCopyBytes	2

CopyByteBuffer3	FNCopyBytes	3


	; this code is only run in SVC mode, but doesn't preserve flags
CopyShortBuffer ;R12 = *workspace
	STMDB	R13!,{R0-R11,R14}
	CMP	R2,#0
	BLE	CSBexit
	LDR	R11,[R12,#Work_LinToLog]
CSBdoalign
	TST	R1,#3
	BEQ	CSBisaligned
	LDRB	R3,[R0],#1
	LDRB	R4,[R0],#1
	SUBS	R2,R2,#1
	ORR	R3,R3,R4,LSL #8
	LDRB	R3,[R11,R3,LSR #3]
	STRB	R3,[R1],#1
	BGT	CSBdoalign
CSBisaligned
	TEQ	R2,#0
	BEQ	CSBexit
	TST	R0,#1
	BNE	CopyShortBuffer1

CopyShortBuffer0
	SUBS	R2,R2,#8
	BLT	CSB0small
CSB0big
	LDMIA	R0!,{R4,R6,R8,R10}
	FNConvertToUlaw	R11,R3,R4
	FNConvertToUlaw	R11,R5,R6
	ORR	R3,R3,R5,LSL #16
	FNConvertToUlaw	R11,R7,R8
	FNConvertToUlaw	R11,R9,R10
	ORR	R7,R7,R9,LSL #16
	STMIA	R1!,{R3,R7}
	SUBS	R2,R2,#8
	BGE	CSB0big
CSB0small
	ADDS	R2,R2,#8
	BLE	CSBexit
	TST	R2,#4
	BEQ	CSB0tiny
	LDMIA	R0!,{R4,R6}
	FNConvertToUlaw	R11,R3,R4
	FNConvertToUlaw	R11,R5,R6
	ORR	R3,R3,R5,LSL #16
	STR	R3,[R1],#4
CSB0tiny
	TST	R2,#2
	BEQ	CSB0rest
	LDR	R4,[R0], #4
	FNConvertToUlaw	R11,R3,R4
	STRB	R3,[R1],#1
	MOV	R3,R3,LSR #8
	STRB	R3,[R1],#1
CSB0rest
	TST	R2,#1
	BEQ	CSBexit
	LDRB	R3,[R0],#1
	LDRB	R4,[R0],#1
	ORR	R3,R3,R4,LSL #8
	LDRB	R3,[R11,R3,LSR #3]
	STRB	R3,[R1],#1
CSBexit
	LDMIA	R13!,{R0-R11,PC}

CopyShortBuffer1
	BIC	R0,R0,#3
	LDR	R14,[R0],#4
	SUBS	R2,R2,#8
	LDRB	R14,[R11,R14,LSR #19]
	BLT	CSB1small
CSB1big
	LDMIA	R0!,{R4,R6,R8,R10}
	FNConvertToUlaw	R11,R3,R4
	FNConvertToUlaw	R11,R5,R6
	ORR	R3,R14,R3,LSL #8
	ORR	R3,R3,R5,LSL #24
	MOV	R5,R5,LSR #8
	FNConvertToUlaw	R11,R7,R8
	FNConvertToUlaw	R11,R9,R10
	ORR	R5,R5,R7,LSL #8
	ORR	R5,R5,R9,LSL #24
	STMIA	R1!,{R3,R5}
	MOV	R14,R9,LSR #8
	SUBS	R2,R2,#8
	BGE	CSB1big
CSB1small
	ADDS	R2,R2,#8
	BLE	CSBexit
	TST	R2,#4
	BEQ	CSB1tiny
	LDMIA	R0!,{R4,R6}
	FNConvertToUlaw	R11,R3,R4
	FNConvertToUlaw	R11,R5,R6
	ORR	R3,R14,R3,LSL #8
	ORR	R3,R3,R5,LSL #24
	STR	R3,[R1],#4
	MOV	R14,R5,LSR #8
CSB1tiny
	TST	R2,#2
	BEQ	CSB1rest
	LDR	R4,[R0],#4
	FNConvertToUlaw	R11,R3,R4
	ORR	R3,R14,R3,LSL #8
	STRB	R3,[R1],#1
	STRB	R3,[R1],#1
	MOV	R14,R3,LSR #16
CSB1rest
	TST	R2,#1
	STRNEB  R14,[R1],#1
	LDMIA	R13!,{R0-R11,PC}


CopyByteToShortBuffer ; doesn't preserve flags
	STMDB	R13!,{R0-R4,R14}
	CMP	R2,#0
	BLE	CBSBexit
	BL	EnsureLogTable
	ADD	R3,R12,#Work_LogTable
	BIC	R1,R1,#1			; just to be on the safe side
	TST	R1,#2
	BEQ	CBSBaligned
	LDRB	R4,[R0],#1
	LDR	R4,[R3,R4,LSL #2]
	STRB	R4,[R1],#1
	MOV	R4,R4,LSR #8
	STRB	R4,[R1],#1
	SUBS	R2,R2,#1
	BLE	CBSBexit
CBSBaligned
	SUBS	R2,R2,#2
	BLT	CBSBrest
CBSBbigloop
	LDRB	R4,[R0],#1
	LDRB	R14,[R0],#1
	LDR	R4,[R3,R4,LSL #2]
	LDR	R14,[R3,R14,LSL #2]
	SUBS	R2,R2,#2
	ORR	R4,R4,R14,LSL #16
	STR	R4,[R1],#4
	BGE	CBSBbigloop
CBSBrest
	ADDS	R2,R2,#2
	BLE	CBSBexit
	LDRB	R4,[R0],#1
	LDR	R4,[R3,R4,LSL #2]
	STRB	R4,[R1],#1
	MOV	R4,R4,LSR #8
	STRB	R4,[R1],#1
CBSBexit
	LDMIA	R13!,{R0-R4,PC}


	; old version without volume scaling
	;.CopyShortToShortBuffer
	; STMDB   R13!,{R2,R14}
	; MOV	R2,R2,LSL #1
	; BL	CopyByteBuffer
	; LDMIA   R13!,{R2,PC}^

CopyShortToShortBuffer ;doesn't preserve flags
	STMDB	R13!,{R0-R7,R14}
	CMP	R2,#0
	BLE	CSSBexit
	MOV	R3,R0				;now r3 = source
	MOV	R0,#0
	SWI	XSound_Volume
	BIC	R1,R1,#1
	BIC	R3,R3,#1
	MVN	R6,#0
	MOV	R6,R6,LSR #16			;R6 = mask &ffff
	TST	R1,#2
	BEQ	CSSBaligned
	LDRB	R4,[R3],#1
	LDRB	R14,[R3],#1
	SUBS	R2,R2,#1
	MOV	R14,R14,LSL #24
	ORR	R4,R4,R14,ASR #16
	MUL	R4,R0,R4
	MOV	R4,R4,ASR #Volume_Shift
	STRB	R4,[R1],#1
	MOV	R4,R4,LSR #8
	STRB	R4,[R1],#1
	BLE	CSSBexit
CSSBaligned
	TST	R3,#2
	BNE	CSSBshift1
	SUBS	R2,R2,#4
	BLT	CSSBs0small
CSSBs0big
	LDMIA	R3!,{R4-R5}
	FNscalesamples	R4,R14,R6
	FNscalesamples	R5,R14,R6
	STMIA	R1!,{R4-R5}
	SUBS	R2,R2,#4
	BGE	CSSBs0big
CSSBs0small
	ADDS	R2,R2,#4
	BLE	CSSBexit
	TST	R2,#2
	BEQ	CSSBs0rest
	LDR	R4,[R3],#4
	FNscalesamples	R4,R14,R6
	STR	R4,[R1],#4
CSSBs0rest
	TST	R2,#1
	BEQ	CSSBexit
	LDRB	R4,[R3],#1
	LDRB	R14,[R3],#1
	MOV	R14,R14,LSL #24
	ORR	R4,R4,R14,ASR #16
	MUL	R4,R0,R4
	MOV	R4,R4,ASR #Volume_Shift
	STRB	R4,[R1],#1
	MOV	R4,R4,LSR #8
	STRB	R4,[R1],#1
CSSBexit
	LDMIA	R13!,{R0-R7,PC}

CSSBshift1
	BIC	R3,R3,#2
	LDR	R4,[R3],#4
	SUBS	R2,R2,#4
	MOV	R4,R4,ASR #16
	BLT	CSSBs1small
CSSBs1big
	LDMIA	R3!,{R5,R7}
	FNscalesampshft	R4,R5,R14,R6
	MOV	R5,R5,ASR #16
	FNscalesampshft	R5,R7,R14,R6
	STMIA	R1!,{R4,R5}
	MOV	R4,R7,ASR #16
	SUBS	R2,R2,#4
	BGE	CSSBs1big
CSSBs1small
	ADDS	R2,R2,#4
	BLE	CSSBexit
	TST	R2,#2
	BEQ	CSSBs1rest
	LDR	R5,[R3],#4
	FNscalesampshft	R4,R5,R14,R6
	STR	R4,[R1],#4
	MOV	R4,R5,ASR #16
CSSBs1rest
	TST	R2,#1
	BEQ	CSSBexit
	MUL	R4,R0,R4
	MOV	R4,R4,ASR #Volume_Shift
	STRB	R4,[R1],#1
	MOV	R4,R4,LSR #8
	STRB	R4,[R1],#1
	LDMIA	R13!,{R0-R7,PC}		;no flags


EnsureLogTable
	STMDB	R13!,{R0,R14}
	MOV	R0,#0
	SWI	XSound_Volume
	LDR	R14,[R12,#Work_TableVolume]
	TEQ	R0,R14
	BEQ	ELTexit
	STMDB	R13!,{R1-R5}
	STR	R0,[R12,#Work_TableVolume]
	ADRL	R1,LogarithmicTable
	ADD	R2,R12,#Work_LogTable
	MOV	R3,#128
	MVN	R5,#0
	MOV	R5,R5,LSR #16			;bitmask &ffff
ELTmakeloop
	LDR	R4,[R1], #4
	MUL	R4,R0,R4
	ADD	R4,R4,#64			;round-to-nearest
	MOV	R4,R4,LSR #Volume_Shift
	RSB	R14,R4,#0
	AND	R14,R14,R5			;store negative values with bits 16-31 clear!
	STMIA	R2!,{R4,R14}
	SUBS	R3,R3,#1
	BGT	ELTmakeloop
	LDMIA	R13!,{R1-R5}
ELTexit
	LDMIA	R13!,{R0,R14}
	B	ModuleReturnOK


	; THIS CODE MUST BE IRQ SAFE!
CopyLinearBuffer ;r0 = src, r1 = dest, r2 = num bytes (dest)
		 ;r11 = scrap, r12 = workspace, return address stacked
	CMP	R2,#0
	LDRLE	PC,[R13],#4			;DOESN'T RESTORE FLAGS! IRQ SAFE!
	LDR	R11,[R12,#Work_Channels]
	TEQ	R11,#1
	BEQ	CLBexpand
	LDR	R11,[R12,#Work_SampleFormat]
	TEQ	R11,#Format_16sle_lr
	BEQ	CLBswaplinear			;2 channels, but wrong order
	B	CopyByteBuffIRQ			;2 channels ==> just copy the whole thing

CLBexpand
	STMDB	R13!,{R0-R10}
	BIC	R1,R1,#3			;dest only makes sense word-aligned!
	BIC	R0,R0,#1
	TST	R0,#2
	BEQ	CLBaligned
	LDRB	R3,[R0],#1
	LDRB	R11,[R0],#1
	SUBS	R2,R2,#4
	ORR	R3,R3,R11,LSL #8
	ORR	R3,R3,R3,LSL #16
	STR	R3,[R1],#4
	BLE	CLBexpdone
CLBaligned
	SUBS	R2,R2,#32
	BLT	CLBexprest
CLBbigloop
	LDMIA	R0!,{R4,R6,R8,R10}
	FNexpandsample	R3,R4
	FNexpandsample	R5,R6
	FNexpandsample	R7,R8
	FNexpandsample	R9,R10
	STMIA	R1!,{R3-R10}
	SUBS	R2,R2,#32
	BGE	CLBbigloop
CLBexprest
	ADDS	R2,R2,#32
	BLE	CLBexpdone
	TST	R2,#16
	BEQ	CLBexpsmall
	LDMIA	R0!,{R4,R6}
	FNexpandsample	R3,R4
	FNexpandsample	R5,R6
	STMIA	R1!,{R3-R6}
CLBexpsmall
	TST	R2,#8
	BEQ	CLBexptiny
	LDR	R4,[R0],#4
	FNexpandsample	R3,R4
	STMIA	R1!,{R3,R4}
CLBexptiny
	TST	R2,#4
	BEQ	CLBexpdone
	LDRB	R3,[R0],#1
	LDRB	R11,[R0],#1
	ORR	R3,R3,R11,LSL #8
	ORR	R3,R3,R3,LSL #16
	STR	R3,[R1],#4
CLBexpdone
	LDMIA	R13!,{R0-R10,PC}		;no flags

CLBswaplinear
	STMDB	R13!,{R0-R7}
	BIC	R1,R1,#3			;dest only makes sense word-aligned
	BIC	R0,R0,#1
	TST	R0,#2
	BNE	CLBswapshift1
	SUBS	R2,R2,#16
	BLT	CLBss0small
CLBss0big
	LDMIA	R0!,{R3-R6}
	MOV	R3,R3,ROR #16
	MOV	R4,R4,ROR #16
	MOV	R5,R5,ROR #16
	MOV	R6,R6,ROR #16
	STMIA	R1!,{R3-R6}
	SUBS	R2,R2,#16
	BGE	CLBss0big
CLBss0small
	ADDS	R2,R2,#16
	BLE	CLBswapexit
	TST	R2,#8
	BEQ	CLBss0tiny
	LDMIA	R0!,{R3,R4}
	MOV	R3,R3,ROR #16
	MOV	R4,R4,ROR #16
	STMIA	R1!,{R3,R4}
CLBss0tiny
	TST	R2,#4
	LDRNE	R3,[R0],#4
	MOVNE	R3,R3,ROR #16
	STRNE	R3,[R1],#4
	TST	R2,#2				;this shouldn't normally happen
	LDRNE	R3,[R0],#4
	MOVNE	R3,R3,LSL #16
	STRNE	R3,[R1],#4
CLBswapexit
	LDMIA	R13!,{R0-R7,PC}		;no flags
CLBswapshift1
	BIC	R0,R0,#3
	LDR	R3,[R0],#4
	SUBS	R2,R2,#16
	MOV	R3,R3,LSR #16
	BLT	CLBss1small
CLBss1big
	LDMIA	R0!,{R4-R7}
	ORR	R3,R3,R4,LSL #16
	MOV	R3,R3,ROR #16
	MOV	R4,R4,LSR #16
	ORR	R4,R4,R5,LSL #16
	MOV	R4,R4,ROR #16
	MOV	R5,R5,LSR #16
	ORR	R5,R5,R6,LSL #16
	MOV	R5,R5,ROR #16
	MOV	R6,R6,LSR #16
	ORR	R6,R6,R7,LSL #16
	MOV	R6,R6,ROR #16
	STMIA	R1!,{R3-R6}
	MOV	R3,R7,LSR #16
	SUBS	R2,R2,#16
	BGE	CLBss1big
CLBss1small
	ADDS	R2,R2,#16
	BLE	CLBswapexit
	TST	R2,#8
	BEQ	CLBss1tiny
	LDMIA	R0!,{R4,R5}
	ORR	R3,R3,R4,LSL #16
	MOV	R3,R3,ROR #16
	MOV	R4,R4,LSR #16
	ORR	R4,R4,R5,LSL #16
	MOV	R4,R4,ROR #16
	STMIA	R1!,{R3,R4}
	MOV	R3,R5,LSR #16
CLBss1tiny
	TST	R2,#4
	BEQ	CLBss1rest
	LDR	R4,[R0],#4
	ORR	R3,R3,R4,LSL #16
	MOV	R3,R3,ROR #16
	STR	R3,[R1],#4
	MOV	R3,R4,LSR #16
CLBss1rest
	TST	R2,#2				;this shouldn't normally happen
	MOVNE	R3,R3,LSL #16
	STRNE	R3,[R1],#4
	LDMIA	R13!,{R0-R7,PC}		;no flags


	; COMMANDS

CommandDRoff
	STR	R14,[R13,#-4]!
	SWI	XDR_Deactivate
	BVS	CDRerror
	LDR	R14,[R13],#4
	B	ModuleReturnOK
CDRerror
	LDR	R14,[R13],#4
	B	ModuleReturnError


CommandDRdefault
	LDR	R12,[R12]
	STR	R14,[R13,#-4]!
	MOV	R4,#0
CDRDparse
	LDRB	R14,[R0],#1
	CMP	R14,#32
	BEQ	CDRDparse
	BLT	CDRDfinish
	TEQ	R14,#'-'
	BNE	CDRDbadargs
	LDRB	R3,[R0],#1
	TEQ	R3,#'c'
	TEQNE	R3,#'f'
	TEQNE	R3,#'p'
	TEQNE	R3,#'s'
	TEQNE	R3,#'n'
	TEQNE	R3,#'q'
	BNE	CDRDbadargs
	ADD	R4,R4,#1
	MOV	R1,R0
	MOV	R0,#10
	SWI	XOS_ReadUnsigned
	BVS	CDRDbadargs
	MOV	R0,R1
	TEQ	R3,#'c'
	STREQ	R2,[R12,#Work_DfltChannels]
	TEQ	R3,#'f'
	STREQ	R2,[R12,#Work_DfltFormat]
	TEQ	R3,#'p'
	STREQ	R2,[R12,#Work_DfltPeriod]
	TEQ	R3,#'s'
	STREQ	R2,[R12,#Work_DfltBuffSize]
	TEQ	R3,#'n'
	STREQ	R2,[R12,#Work_DfltBuffers]
	TEQ	R3,#'q'
	STREQ	R2,[R12,#Work_DfltFrequency]
	B	CDRDparse
CDRDfinish
	TEQ	R4,#0
	BEQ	CDRDprintstate
	LDR	R14,[R13],#4
	B	ModuleReturnOK
CDRDprintstate
	FNPrintConfig	ConfStr_Channels,Work_DfltChannels
	ADR	R0,ConfStr_Format
	SWI	XOS_Write0
	LDR	R14,[R12,#Work_DfltFormat]
	ADR	R0,ConfStr_FmtUndef
	TEQ	R14,#Format_8u
	ADREQ	R0,ConfStr_Fmt8u
	TEQ	R14,#Format_16sle_rl
	ADREQ	R0,ConfStr_Fmt16sle_rl
	TEQ	R14,#Format_16sle_lr
	ADREQ	R0,ConfStr_Fmt16sle_lr
	SWI	XOS_Write0
	FNPrintConfig	ConfStr_Period,Work_DfltPeriod
	FNPrintConfig	ConfStr_Frequency,Work_DfltFrequency
	FNPrintConfig	ConfStr_BuffSize,Work_DfltBuffSize
	FNPrintConfig	ConfStr_Buffers,Work_DfltBuffers
	SWI	XOS_NewLine
	LDR	R14,[R13],#4
	B	ModuleReturnOK
CDRDbadargs
	ADRL	R0,Error_ParseError
	LDR	R14,[R13],#4
	B	ModuleReturnError

ConfStr_Channels
	=	"Channels: ",0
	ALIGN
ConfStr_Format
	=	". Format: ",0
	ALIGN
ConfStr_Period
	=	". Period: ",0
	ALIGN
ConfStr_Frequency
	=	". Frequency: ",0
	ALIGN
ConfStr_BuffSize
	=	". Buffer size: ",0
	ALIGN
ConfStr_Buffers
	=	". Num buffers: ",0
	ALIGN
ConfStr_FmtUndef
	=	"undefined",0
	ALIGN
ConfStr_Fmt8u
	=	"8bit ulaw",0
	ALIGN
ConfStr_Fmt16sle_rl
	=	"16bit signed linear (little-endian, right-left order)",0
	ALIGN
ConfStr_Fmt16sle_lr
	=	"16bit signed linear (little-endian, left-right order)",0
	ALIGN

CommandDRstatus
	LDR	R12,[R12]
	STR	R14,[R13,#-4]!
	LDR	R3,[R12,#Work_State]
	TST	R3,#State_Active
	BEQ	CDRSinactive
	ADR	R0,ConfStr_Active
	SWI	XOS_Write0
	TST	R3,#State_Paused
	BEQ	CDRSnopause
	ADR	R0,ConfStr_Paused
	SWI	XOS_Write0
CDRSnopause
	LDR	R0,[R12,#Work_StreamHandle]
	TEQ	R0,#0
	BEQ	CDRSnofile
	ADR	R0,ConfStr_FileStream
	SWI	XOS_Write0
CDRSnofile
	LDR	R0,[R12,#Work_NumBuffers]
	TEQ	R0,#0
	BEQ	CDRSnostream
	FNPrintConfig	ConfStr_Streaming,Work_NumBuffers
	ADR	R0,ConfStr_StreamFilled
	SWI	XOS_Write0
	SWI	XDR_StreamStatus
	FNPrintNumber
CDRSnostream
	MOV	R0,#' '
	SWI	XOS_WriteC
	FNPrintConfig	ConfStr_Channels,Work_Channels
	TST	R3,#State_NewSound
	BEQ	CDRSoldsound
	FNPrintConfig	ConfStr_Frequency,Work_Frequency
	B	CDRSmergesound
CDRSoldsound
	FNPrintConfig	ConfStr_Period,Work_Period
CDRSmergesound
	FNPrintConfig	ConfStr_BuffSize,Work_BuffSize
	B	CDRSdone
CDRSinactive
	ADR	R0,ConfStr_Inactive
	SWI	XOS_Write0
CDRSdone
	SWI	XOS_NewLine
	LDR	R14,[R13],#4
	B	ModuleReturnOK

ConfStr_Active
	=	"Active",0
	ALIGN
ConfStr_Paused
	=	", Paused",0
	ALIGN
ConfStr_FileStream
	=	", Pseudodevice",0
	ALIGN
ConfStr_Streaming
	=	", Streaming buffers: ",0
	ALIGN
ConfStr_StreamFilled
	=	", filled: ",0
	ALIGN
ConfStr_Inactive
	=	"Inactive.",0
	ALIGN


Error_FSread
	&	(FSNumber<<8) +  0
	=	"Filing system is write-only",0
	ALIGN
Error_FSbusy
	&	(FSNumber<<8) +  1
	=	"Filing system is busy",0
	ALIGN
Error_FShandle
	&	(FSNumber<<8) +  2
	=	"Bad file handle",0
	ALIGN
Error_FSargnum
	&	(FSNumber<<8) +  3
	=	"Bad FS_Args number",0
	ALIGN
Error_FSfunction
	&	(FSNumber<<8) +  4
	=	"Unsupported FS_Function",0
	ALIGN


FSIssueUpCall
	STMDB	R13!,{R0,R1,R14}
	LDR	R0,[R12,#Work_StreamFlags]
	TST	R0,#StrFlg_IssueUpCall
	BEQ	FSIUCexit
	MOV	R0,#0
	STR	R0,[R12,#Work_PollWord]
	LDR	R0,[R12,#Work_State]
	ORR	R0,R0,#State_UpCall
	STR	R0,[R12,#Work_State]
	MOV	R0,#6
	ADD	R1,R12,#Work_PollWord
	SWI	XOS_UpCall
	LDR	R0,[R12,#Work_State]
	BIC	R0,R0,#State_UpCall
	STR	R0,[R12,#Work_State]
FSIUCexit
	LDMIA	R13!,{R0,R1,R14}
	B	ModuleReturnOK

FSWriteNoBlock
	STMDB	R13!,{R0-R6,R14}
	MOVS	R2,R0
	BLE	FSWNBexit
	LDR	R4,[R12,#Work_NumBuffers]
	LDR	R5,[R12,#Work_SampleFormat]
	MOV	R3,R1
FSWNBloop
	LDR	R0,[R12,#Work_WriteBuffer] ;calc number of used buffers (see StreamStatistics)
	LDR	R1,[R12,#Work_ReadBuffer]
	SUBS	R0,R0,R1
	ADDLT	R0,R0,R4		;number of used buffers in R0
	BEQ	FSWNBcheckfull
	LDR	R1,[R12,#Work_FillLevel]
	TEQ	R1,#0
	ADDNE	R0,R0,#1
	TEQ	R0,R4			;= total number of buffers?
	BNE	FSWNBdocopy
	B	FSWNBdowait
FSWNBcheckfull
	LDR	R0,[R12,#Work_RingIsFull]
	TEQ	R0,#0
	BEQ	FSWNBdocopy
FSWNBdowait
	BL	FSIssueUpCall
	B	FSWNBloop		;always check for free space after returning!
FSWNBdocopy
	LDR	R1,[R12,#Work_TotBuffSize]
	MOV	R0,R2
	CMP	R1,R3
	MOVHS	R1,R3
	MOV	R6,R1			;remember the size to copy
	TEQ	R5,#Format_8u
	BNE	FSWNBcopy16
	SWI	XDR_StreamSamples
	B	FSWNBcopydone
FSWNBcopy16
	MOV	R1,R1,LSR #1
	SWI	XDR_StreamSamples16
FSWNBcopydone
	ADD	R2,R2,R6
	SUBS	R3,R3,R6
	BGT	FSWNBloop
FSWNBexit
	LDMIA	R13!,{R0-R6,R14}
	B	ModuleReturnOK


FSFlushMiniBuffer
	STMDB	R13!,{R0,R1,R14}
	ADD	R0,R12,#Work_MiniBuffer
	LDR	R1,[R12,#Work_BuffOffset]
	BL	FSWriteNoBlock
	MOV	R0,#0
	STR	R0,[R12,#Work_BuffOffset]
	LDMIA	R13!,{R0,R1,R14}
	B	ModuleReturnOK


FSConfigOpen
	STMDB	R13!,{R1-R3,R14}
	LDR	R0,[R12,#Work_NumBuffers]
	STR	R0,[R12,#Work_OldNumBuff]
	LDR	R0,[R12,#Work_StreamFlags]
	STR	R0,[R12,#Work_OldStrFlags]
	LDR	R0,[R12,#Work_DfltBuffers]
	CMP	R0,#2
	MOVLT	R0,#2				;use at least 2 buffers!
	SWI	XDR_NumBuffers
	LDR	R0,[R12,#Work_DfltChannels]
	LDR	R1,[R12,#Work_DfltBuffSize]
	LDR	R2,[R12,#Work_DfltFrequency]
	CMP	R2,#0				;try 16bit sound
	BLE	FSCOnonewsound
	MOV	R3,#1
	SWI	XDR_Activate16
	BVC	FSCOnewsound
	LDR	R0,[R12,#Work_DfltChannels] ;the error message overwrote this!
FSCOnonewsound
	LDR	R2,[R12,#Work_DfltPeriod]
	SWI	XDR_Activate
FSCOnewsound
	MOVVC	R0,#0
	MOVVC	R1,#1
	STRVC	R1,[R12,#Work_StreamHandle]
	LDR	R1,[R12,#Work_DfltFormat]
	STR	R1,[R12,#Work_SampleFormat]
	LDMIA	R13!,{R1-R3,R14}
	B	ModuleReturnOK


FSConfigClose ;close and wait until sound is played
	STMDB	R13!,{R0,R5,R6,R14}
	LDR	R14,[R12,#Work_BuffOffset]
	TEQ	R14,#0
	BLNE	FSFlushMiniBuffer
    ;;
    ;; It appears on BeagleBoard RISC OS 5.18, a silent tail 3 times the buffer length is required
    ;; in order to avoid a nasty buzz if the file being played does not end in silence.
    ;; The tail may in fact be smaller than this, depending on the buffer length or sample rate,
    ;; but I found that 2 buffers worth of silence was not enough; 3 was.
    ;; Iyonix RISC OS 5.18 exhibits the same problem but to a lesser degree.
    ;; I haven't experimented enough to determine a precise formulation that is sure to work in all cases.
    ;; And, of course, this code doesn't help when the DRender: filesystem isn't being used.
    ;; --
    ;; Christopher Martin, Fri 20th April 2012 03:55
    ;;
;	STR	R1,[R13,#-4]!
;    	LDR	R6,[R12,#Work_TotBuffSize]
;    	MOV	R1,#MiniBufferSize
;    	ADD	R0,R12,#Work_MiniBuffer
;    	BL	MemSet
;    	ADD	R6,R6,R6,LSL #1		;silence for three times the length of a full buffer
;psycho	BL	FSWriteNoBlock
;	SUBS	R6,R6,R1
;	BHI	psycho
;	LDR	R1,[R13],#4
    ;;
	LDR	R6,[R12,#Work_ReadBuffer] ;last read buffer
	SWI	XOS_ReadMonotonicTime
	MOV	R5,R0				;time stamp of last read buffer
FSCCwait
        BL      FSIssueUpCall
        SWI     XOS_ReadMonotonicTime
        SUB     R14,R0,R5
        CMP     R14,#20                 ;if the read buffer hasn't changed in 20cs
        BHS     FSCCfinish                      ;we finish anyway (no infinite loops!!!)
	LDR	R4,[R12,#Work_ReadBuffer]
	TEQ	R4,R6
	MOVNE	R6,R4
	MOVNE	R5,R0				;if changed update timestamp and buffer number
	LDR	R14,[R12,#Work_WriteBuffer]
	TEQ	R4,R14
	LDREQ	R4,[R12,#Work_RingIsFull]
	TEQEQ	R4,#0
        BNE     FSCCwait
    ;;
    ;;
FSCCfinish
        MOV     R14,#0                          ;mark files closed _before_ calling deactivate!
        STR     R14,[R12,#Work_StreamHandle] ;otherwise deactivate tries to close again
        STR     R14,[R12,#Work_StrExtHand]
    ;;
        LDR     R14,[R12,#Work_State]
        TST     R14,#State_Deactivating         ;check that we aren't here because SWI XDR_Deactivate was called
        SWIEQ   XDR_Deactivate
    ;;
	LDR	R0,[R12,#Work_OldStrFlags]
	STR	R0,[R12,#Work_StreamFlags]
	LDR	R0,[R12,#Work_OldNumBuff]
	STR	R0,[R12,#Work_NumBuffers]
	LDMIA	R13!,{R0,R5,R6,R14}
	B	ModuleReturnOK


FSEntry_Open
	TEQ	R0,#1
	TEQNE	R0,#2
	BNE	FSEopen_read
	STR	R14,[R13,#-4]!
	LDR	R12,[R12]
	LDR	R14,[R12,#Work_State]
	ANDS	R14,R14,#(State_Active + State_UpCall)
	BNE	FSEopen_busy
	LDR	R14,[R12,#Work_StreamHandle] ;there can only be one file open!
	TEQ	R14,#0
	BNE	FSEopen_busy
	BL	FSConfigOpen
	TEQ	R0,#0
	BNE	FSEopen_generr
	STR	R3,[R12,#Work_StrExtHand]
	MOV	R0,#(&12 << 27)		;write only, unbuffered
	LDR	R1,[R12,#Work_StreamHandle]
	MOV	R2,#0
	MOV	R3,#0
	MOV	R4,#0
	STR	R4,[R12,#Work_BuffOffset]
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEopen_read
	ADRL	R0,Error_FSread
FSEopen_generr
	LDR	R14,[R13],#4
	B	ModuleReturnError
FSEopen_busy
	LDR	R14,[R13],#4
	ADRL	R0,Error_FSbusy
	B	ModuleReturnError


FSEntry_GetBytes ;should never be called
	TEQ	R0,R0
	TEQ	PC,PC
	ORRNES  PC,R14,#PFlag_C
	MSR	CPSR_f,#PFlag_C
	MOV	PC,R14


FSEntry_PutBytes
	STMDB	R13!,{R2,R14}
	LDR	R12,[R12]
	LDR	R2,[R12,#Work_State]
	AND	R2,R2,#(State_Active + State_UpCall)
	TEQ	R2,#State_Active
	BNE	FSEput_busy
	LDR	R2,[R12,#Work_StreamHandle]
	TEQ	R1,R2
	BNE	FSEput_handle
	LDR	R2,[R12,#Work_BuffOffset]
	CMP	R2,#MiniBufferSize
	BLGE	FSFlushMiniBuffer
	ADD	R14,R12,#Work_MiniBuffer
	LDR	R2,[R12,#Work_BuffOffset]
	STRB	R0,[R14,R2]
	ADD	R2,R2,#1
	STR	R2,[R12,#Work_BuffOffset]
	LDMIA	R13!,{R2,R14}
	B	ModuleReturnOK
FSEput_handle
	ADRL	R0,Error_FShandle
	LDMIA	R13!,{R2,R14}
	B	ModuleReturnError
FSEput_busy
	ADRL	R0,Error_FSbusy
	LDMIA	R13!,{R2,R14}
	B	ModuleReturnError


FSEntry_PopAndReturn
	LDR	R14,[R13],#4
	B	ModuleReturnOK

FSEntry_Args
	STR	R14,[R13,#-4]!
	LDR	R12,[R12]
	LDR	R14,[R12,#Work_State]
	AND	R14,R14,#(State_Active + State_UpCall)
	TEQ	R14,#State_Active
	BNE	FSEargs_busy
	CMP	R0,#((FSEargs_illegal - FSEargs_table) >> 2)
	ADDLO	PC,PC,R0,LSL #2
	B	FSEargs_illegal
FSEargs_table
	B	FSEntry_Arg0	;read file pointer
	B	FSEntry_PopAndReturn ;write file pointer
	B	FSEntry_Arg2	;read extent
	B	FSEntry_PopAndReturn ;write extent
	B	FSEntry_Arg4	;read allocated size
	B	FSEntry_Arg5	;eof check
	B	FSEntry_Arg6	;notify of flush
	B	FSEntry_PopAndReturn ;ensure file size
	B	FSEntry_PopAndReturn ;write zeroes (buffered only)
	B	FSEntry_Arg9	;read datestamp
	B	FSEntry_PopAndReturn ;new image stamp
FSEargs_illegal
	LDR	R14,[R13],#4
	ADRL	R0,Error_FSargnum
	B	ModuleReturnError
FSEargs_busy
	LDR	R14,[R13],#4
	ADRL	R0,Error_FSbusy
	B	ModuleReturnError

FSEntry_Arg0
	MOV	R2,#0				;no file pointers
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEntry_Arg2
	MOV	R2,#0
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEntry_Arg4
	LDR	R2,[R12,#Work_TotBuffSize]
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEntry_Arg5
	MVN	R2,#0
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEntry_Arg6
	LDR	R14,[R12,#Work_BuffOffset]
	TEQ	R14,#0
	BLNE	FSFlushMiniBuffer
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEntry_Arg9
	MOV	R2,#0
	MOV	R3,#0
	LDR	R14,[R13],#4
	B	ModuleReturnOK


FSEntry_Close
	STR	R14,[R13,#-4]!
	LDR	R12,[R12]
	LDR	R14,[R12,#Work_State]
	AND	R14,R14,#(State_Active + State_UpCall)
	TEQ	R14,#State_Active
	BNE	FSEclose_busy
	BL	FSConfigClose
	LDR	R14,[R13],#4
	B	ModuleReturnOK
FSEclose_busy
	LDR	R14,[R13],#4
	ADRL	R0,Error_FSbusy
	B	ModuleReturnError


FSEntry_File
	TEQ	R0,#0
	BEQ	FSEntry_File0			;save
	TEQ	R0,#5
	BEQ	FSEntry_File5			;read file info
	TEQ	R0,#10
	BEQ	FSEntry_File10			;read block size
	B	ModuleReturnOK

FSEntry_File0
	STMDB	R13!,{R0-R1,R14}
	LDR	R12,[R12]
	LDR	R14,[R12,#Work_State]
	ANDS	R14,R14,#(State_Active + State_UpCall)
	BNE	FSEsave_busy
	BL	FSConfigOpen
	TEQ	R0,#0
	BNE	FSEsave_generr
	MOV	R0,R4
	SUB	R1,R5,R4
	BL	FSWriteNoBlock
	BL	FSConfigClose
	ADRL	R6,FSDfltSaveName
	LDMIA	R13!,{R0-R1,R14}
	B	ModuleReturnOK
FSEsave_busy
	ADRL	R0,Error_FSbusy
FSEsave_generr
	ADD	R13,R13,#4
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnError

FSEntry_File5 ;always return defaults (see FastSpool)
	MOV	R0,#0
	LDR	R1,FSEinfodefault
	MOV	R3,#0
	MOV	R4,#0
	MOV	R5,#&88
	B	ModuleReturnOK
FSEinfodefault
	&	&fffffd00			;default load address (filetype data)

FSEntry_File10
	LDR	R12,[R12]
	LDR	R2,[R12,#Work_TotBuffSize]
	B	ModuleReturnOK


FSEntry_Func
	CMP	R0,#7
	BLO	FSEfunc_error			;all directory functions return an error
	TEQ	R0,#8
	BEQ	FSEntry_Func8			;rename
	CMP	R0,#10
	BLO	ModuleReturnOK
	CMP	R0,#16
	BLO	FSEfunc_error
	CMP	R0,#19
	BLO	ModuleReturnOK
	CMP	R0,#24
	BLO	FSEfunc_error
	BEQ	FSEntry_Func24			;resolve wildcard
	CMP	R0,#35
	BLO	FSEfunc_error
	B	ModuleReturnOK
FSEfunc_error
	ADRL	R0,Error_FSfunction
	B	ModuleReturnError

FSEntry_Func8
	MOV	R1,#1
	B	ModuleReturnOK

FSEntry_Func24
	MVN	R4,#0
	B	ModuleReturnOK


FSEntry_GBPB
	TEQ	R0,#1
	TEQNE	R0,#2
	BNE	FSEgbpb_read
	LDR	R12,[R12]
	STMDB	R13!,{R0,R1,R14}
	LDR	R14,[R12,#Work_State]
	AND	R14,R14,#(State_Active + State_UpCall)
	TEQ	R14,#State_Active
	BNE	FSEgbpb_busy
	LDR	R14,[R12,#Work_BuffOffset]
	TEQ	R14,#0
	BLNE	FSFlushMiniBuffer
	MOV	R0,R2
	MOV	R1,R3
	BL	FSWriteNoBlock
	ADD	R2,R2,R3
	ADD	R4,R4,R3
	MOV	R3,#0
	LDMIA	R13!,{R0,R1,R14}
	B	ModuleReturnOK
FSEgbpb_busy
	ADRL	R0,Error_FSbusy
	ADD	R13,R13,#4
	LDMIA	R13!,{R1,R14}
	B	ModuleReturnError
FSEgbpb_read
	ADRL	R0,Error_FSread
	B	ModuleReturnError


	; logarithmic table (8bit ulaw -> 16bit signed linear) for maximum volume (128).
	; Real maximum volume is 127, therefore the maximum value in the table is a little
	; higher (32961), but after volume-scaling it's fine (32961*127/128 = 32703)

LogarithmicTable
	BIN	$LogTableFileName	; This variable must be defined when the assembler is invoked.
        ALIGN                           ; It specifies the name of the file that holds the binary logtable data.
					; A logtable data file can be prepared using utility 'makelogtable'.
					; Example:
					; 	gcc -mlibscl -o makelogtable makelogtable.c
					; 	makelogtable >logtable
					; 	asasm -DLogTableFileName=logtable
					; 	objasm -PreDefine "LogTableFileName SETS \"logtable\""

	END
