@ Chunk 1 stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2010 UnixLib Developers
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

	@ Entry 0
	DefSCLFncS	_kernel_init

	@ Entry 1
	DefSCLFncS	_kernel_exit

	@ Entry 2
	DefSCLFncS	_kernel_setreturncode

	@ Entry 3
	DefSCLFncS	_kernel_exittraphandler

	@ Entry 4
	DefSCLFncS	_kernel_unwind

	@ Entry 5
	DefSCLFncS	_kernel_procname

	@ Entry 6
	DefSCLFncS	_kernel_language

	@ Entry 7
	DefSCLFncS	_kernel_command_string

	@ Entry 8
	DefSCLFncS	_kernel_hostos

	@ Entry 9
	DefSCLFncS	_kernel_swi

	@ Entry 10
	DefSCLFncS	_kernel_osbyte

	@ Entry 11
	DefSCLFncS	_kernel_osrdch

	@ Entry 12
	DefSCLFncS	_kernel_oswrch

	@ Entry 13
	DefSCLFncS	_kernel_osbget

	@ Entry 14
	DefSCLFncS	_kernel_osbput

	@ Entry 15
	DefSCLFncS	_kernel_osgbpb

	@ Entry 16
	DefSCLFncS	_kernel_osword

	@ Entry 17
	DefSCLFncS	_kernel_osfind

	@ Entry 18
	DefSCLFncS	_kernel_osfile

	@ Entry 19
	DefSCLFncS	_kernel_osargs

	@ Entry 20
	DefSCLFncS	_kernel_oscli

	@ Entry 21
	DefSCLFncS	_kernel_last_oserror

	@ Entry 22
	DefSCLFncS	_kernel_system

	@ Entry 23
	DefSCLFncS	_kernel_getenv

	@ Entry 24
	DefSCLFncS	_kernel_setenv

	@ Entry 25
	DefSCLFncS	_kernel_register_allocs

	@ Entry 26
	DefSCLFncS	_kernel_alloc

	@ Entry 27
	DefSCLFncS	_kernel_stkovf_split_0frame

	@ Entry 28
	DefSCLFncS	_kernel_stkovf_split

	@ Entry 29
	DefSCLFncS	_kernel_stkovf_copyargs

	@ Entry 30
	DefSCLFncS	_kernel_stkovf_copy0args

	@ Entry 31
	DefSCLFncS	_kernel_udiv

	@ Entry 32
	DefSCLFncS	_kernel_urem

	@ Entry 33
	DefSCLFncS	__rt_udiv10
	.global _kernel_udiv10
_kernel_udiv10 = __rt_udiv10

	@ Entry 34
	DefSCLFncS	_kernel_sdiv

	@ Entry 35
	DefSCLFncS	_kernel_srem

	@ Entry 36
	DefSCLFncS	__rt_sdiv10
	.global	_kernel_sdiv10
_kernel_sdiv10 = __rt_sdiv10

	@ Entry 37
	DefSCLFncS	_kernel_fpavailable

	@ Entry 38
	DefSCLFncS	_kernel_moduleinit

	@ Entry 39
	DefSCLFncS	_kernel_irqs_on

	@ Entry 40
	DefSCLFncS	_kernel_irqs_off

	@ Entry 41
	DefSCLFncS	_kernel_irqs_disabled

	@ Entry 42
	DefSCLFncS	_kernel_entermodule

	@ Entry 43
	DefSCLFncS	_kernel_escape_seen

	@ Entry 44
	DefSCLFncS	_kernel_current_stack_chunk

	@ Entry 45
	DefSCLFncS	_kernel_swi_c

	@ Entry 46
	DefSCLFncS	_kernel_register_slotextend

	@ Entry 47
	DefSCLFncS	_kernel_raise_error

kernel_vectors_end:
	.space	kernel_vectors_end - kernel_vectors_begin
	.size	kernel_vectors, . - kernel_vectors

	.section .bss.riscos.libscl.chunkstub.id01,"w",%nobits
	.align 2
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

	@ When non-zero, _stub_errorNumber and _stub_errorString are valid.
	.global _stub_errorBuffer
.set	_stub_errorBuffer, kernel_statics_begin + 0x011c
	@ Last RISC OS error happened via a call through these stubs.
	.global _stub_errorNumber
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

