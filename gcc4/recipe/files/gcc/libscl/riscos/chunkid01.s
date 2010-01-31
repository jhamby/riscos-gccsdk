@ Chunk 1 stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2008 UnixLib Developers
@ All rights reserved.

@ Redistribution and use in source and binary forms, with or without
@ modification, are permitted provided that the following conditions
@ are met:
@ 1. Redistributions of source code must retain the above copyright
@    notice, this list of conditions and the following disclaimer.
@ 2. Redistributions in binary form must reproduce the above copyright
@    notice, this list of conditions and the following disclaimer in the
@    documentation and/or other materials provided with the distribution.
@ 3. The name of the author may not be used to endorse or promote products
@    derived from this software without specific prior written permission.
@
@ THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
@ IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
@ OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
@ IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
@ INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
@ NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
@ THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/scl-macros.s"

	.section .riscos.libscl.chunkstub.id01,"ax",%progbits
	@ Chunk 1 is a requirement and always needed.
	.word	1			@ Kernel
	MakePtr	kernel_vectors_begin
	MakePtr	kernel_vectors_end
	MakePtr	kernel_statics_begin
	MakePtr	kernel_statics_end

	.text
kernel_vectors:
kernel_vectors_begin:

	.global	_kernel_init
_kernel_init:			MOV	PC, #0
	.global	_kernel_exit
_kernel_exit:			MOV	PC, #0
	.global	_kernel_setreturncode
_kernel_setreturncode:		MOV	PC, #0
	.global	_kernel_exittraphandler
_kernel_exittraphandler:	MOV	PC, #0
	.global	_kernel_unwind
_kernel_unwind:			MOV	PC, #0
	.global	_kernel_procname
_kernel_procname:		MOV	PC, #0
	.global	_kernel_language
_kernel_language:		MOV	PC, #0
	.global	_kernel_command_string
_kernel_command_string:		MOV	PC, #0
	.global	_kernel_hostos
_kernel_hostos:			MOV	PC, #0
	.global	_kernel_swi
_kernel_swi:			MOV	PC, #0
	.global	_kernel_osbyte
_kernel_osbyte:			MOV	PC, #0
	.global	_kernel_osrdch
_kernel_osrdch:			MOV	PC, #0
	.global	_kernel_oswrch
_kernel_oswrch:			MOV	PC, #0
	.global	_kernel_osbget
_kernel_osbget:			MOV	PC, #0
	.global	_kernel_osbput
_kernel_osbput:			MOV	PC, #0
	.global	_kernel_osgbpb
_kernel_osgbpb:			MOV	PC, #0
	.global	_kernel_osword
_kernel_osword:			MOV	PC, #0
	.global	_kernel_osfind
_kernel_osfind:			MOV	PC, #0
	.global	_kernel_osfile
_kernel_osfile:			MOV	PC, #0
	.global	_kernel_osargs
_kernel_osargs:			MOV	PC, #0
	.global	_kernel_oscli
_kernel_oscli:			MOV	PC, #0
	.global	_kernel_last_oserror
_kernel_last_oserror:		MOV	PC, #0
	.global	_kernel_system
_kernel_system:			MOV	PC, #0
	.global	_kernel_getenv
_kernel_getenv:			MOV	PC, #0
	.global	_kernel_setenv
_kernel_setenv:			MOV	PC, #0
	.global	_kernel_register_allocs
_kernel_register_allocs:	MOV	PC, #0
	.global	_kernel_alloc
_kernel_alloc:			MOV	PC, #0
	.global	_kernel_stkovf_split_0frame
_kernel_stkovf_split_0frame:	MOV	PC, #0
	.global	_kernel_stkovf_split
_kernel_stkovf_split:		MOV	PC, #0
	.global	_kernel_stkovf_copyargs
_kernel_stkovf_copyargs:	MOV	PC, #0
	.global	_kernel_stkovf_copy0args
_kernel_stkovf_copy0args:	MOV	PC, #0
	.global	_kernel_udiv
_kernel_udiv:			MOV	PC, #0
	.global	_kernel_urem
_kernel_urem:			MOV	PC, #0
	.global	__rt_udiv10
__rt_udiv10:
	.global	_kernel_udiv10
_kernel_udiv10:			MOV	PC, #0
	.global	_kernel_sdiv
_kernel_sdiv:			MOV	PC, #0
	.global	_kernel_srem
_kernel_srem:			MOV	PC, #0
	.global	__rt_sdiv10
__rt_sdiv10:
	.global	_kernel_sdiv10
_kernel_sdiv10:			MOV	PC, #0
	.global	_kernel_fpavailable
_kernel_fpavailable:		MOV	PC, #0
	.global	_kernel_moduleinit
_kernel_moduleinit:		MOV	PC, #0
	.global	_kernel_irqs_on
_kernel_irqs_on:		MOV	PC, #0
	.global	_kernel_irqs_off
_kernel_irqs_off:		MOV	PC, #0
	.global	_kernel_irqs_disabled
_kernel_irqs_disabled:		MOV	PC, #0
	.global	_kernel_entermodule
_kernel_entermodule:		MOV	PC, #0
	.global	_kernel_escape_seen
_kernel_escape_seen:		MOV	PC, #0
	.global	_kernel_current_stack_chunk
_kernel_current_stack_chunk:	MOV	PC, #0
	.global	_kernel_swi_c
_kernel_swi_c:			MOV	PC, #0
	.global	_kernel_register_slotextend
_kernel_register_slotextend:	MOV	PC, #0
	.global	_kernel_raise_error
_kernel_raise_error:		MOV	PC, #0
kernel_vectors_end:
	.space	kernel_vectors_end - kernel_vectors_begin
	.size	kernel_vectors, . - kernel_vectors

	.section .bss.riscos.libscl.chunkstub.id01,"w",%nobits
kernel_statics_begin:
	.space	0x31c
kernel_statics_end:
.set	_stub_imageBase, kernel_statics_begin + 0x0000
.set	_stub_rtsDataBase, kernel_statics_begin + 0x0004
.set	_stub_rtsDataLimit, kernel_statics_begin + 0x0008
.set	_stub_IIHandlerInData, kernel_statics_begin + 0x000c
.set	_stub_PAHandlerInData, kernel_statics_begin + 0x001c
.set	_stub_DAHandlerInData, kernel_statics_begin + 0x002c
.set	_stub_AEHandlerInData, kernel_statics_begin + 0x003c
.set	_stub_undStack, kernel_statics_begin + 0x004c
.set	_stub_svcStack, kernel_statics_begin + 0x0050
.set	_stub_unused4, kernel_statics_begin + 0x0054
.set	_stub_registerDump, kernel_statics_begin + 0x0078
.set	_stub_oldAbortHandlers, kernel_statics_begin + 0x00bc
.set	_stub_oldExitHandler, kernel_statics_begin + 0x00cc
.set	_stub_oldMemoryLimit, kernel_statics_begin + 0x00d4
.set	_stub_oldErrorHandler, kernel_statics_begin + 0x00d8
.set	_stub_oldErrorR0, kernel_statics_begin + 0x00dc
.set	_stub_oldErrorBuffer, kernel_statics_begin + 0x00e0
.set	_stub_oldCallBackHandler, kernel_statics_begin + 0x00e4
.set	_stub_oldEscapeHandler, kernel_statics_begin + 0x00f0
.set	_stub_oldEventHandler, kernel_statics_begin + 0x00f8
.set	_stub_oldUpCallHandler, kernel_statics_begin + 0x0100
.set	_stub_languageEnvSave, kernel_statics_begin + 0x0108
.set	_stub_hadEscape, kernel_statics_begin + 0x0114
	.global	_stub_kallocExtendsWS
.set	_stub_kallocExtendsWS, kernel_statics_begin + 0x0115
.set	_stub_inTrapHandler, kernel_statics_begin + 0x0116
.set	_stub_beingDebugged, kernel_statics_begin + 0x0117
.set	_stub_fpPresent, kernel_statics_begin + 0x0118
.set	_stub_initialised, kernel_statics_begin + 0x0119
.set	_stub_callbackInactive, kernel_statics_begin + 0x011a
.set	_stub_unused_byte_2, kernel_statics_begin + 0x011b
.set	_stub_errorBuffer, kernel_statics_begin + 0x011c
.set	_stub_errorNumber, kernel_statics_begin + 0x0120
.set	_stub_errorString, kernel_statics_begin + 0x0124
.set	_stub_FatalErrorBuffer, kernel_statics_begin + 0x0220
.set	_stub_eventCode, kernel_statics_begin + 0x0220
.set	_stub_eventUserR13, kernel_statics_begin + 0x0224
.set	_stub_eventRegisters, kernel_statics_begin + 0x0228
.set	_stub_unused2, kernel_statics_begin + 0x0258
.set	_stub_heapTop, kernel_statics_begin + 0x0284
.set	_stub_heapLimit, kernel_statics_begin + 0x0288
.set	_stub_allocProc, kernel_statics_begin + 0x028c
.set	_stub_freeProc, kernel_statics_begin + 0x0290
.set	_stub_returnCode, kernel_statics_begin + 0x0294
.set	_stub_moduleDataWord, kernel_statics_begin + 0x0298
.set	_stub_ClientFlags, kernel_statics_begin + 0x029c
.set	_stub_escapeSeen, kernel_statics_begin + 0x029d
.set	_stub_unwinding, kernel_statics_begin + 0x029e
.set	_stub_underDesktop, kernel_statics_begin + 0x029f
.set	_stub_heapBase, kernel_statics_begin + 0x02a0
.set	_stub_ArgString, kernel_statics_begin + 0x02a4
.set	_stub_heapExtender, kernel_statics_begin + 0x02a8
.set	_stub_knownSlotSize, kernel_statics_begin + 0x02ac
.set	_stub_initSlotSize, kernel_statics_begin + 0x02b0
.set	_stub_lk_RestoreOSHandlers, kernel_statics_begin + 0x02b4
.set	_stub_extendChunk, kernel_statics_begin + 0x02b8
.set	_stub_rootStackChunk, kernel_statics_begin + 0x02bc
.set	_stub_pc_hex_buff, kernel_statics_begin + 0x02c0
.set	_stub_returnCodeLimit, kernel_statics_begin + 0x02c0
.set	_stub_reg_hex_buff, kernel_statics_begin + 0x02cc
	.global	disable_stack_extension
.set	disable_stack_extension, kernel_statics_begin + 0x02d8
.set	_stub_unused, kernel_statics_begin + 0x02dc

	.end

