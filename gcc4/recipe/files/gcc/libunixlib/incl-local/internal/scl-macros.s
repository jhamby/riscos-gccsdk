@ SCL stubs specific macros used by all its assembler files.
@ Copyright (c) 2008-2010 UnixLib Developers
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

#ifndef __TARGET_SCL__
#  error "Macros not suited for non-SCL targets."
#endif

	.macro	MakePtr ptrname
#ifdef __TARGET_MODULE__
	.word	\ptrname(GOTOFF)
#else
	.word	\ptrname
#endif
	.endm

	@ Add global labels 'fncname' and '__fncname' whom the former one is
	@ weak definition.
	@ Split in two macro's DefSCLFncB and DefSCLFncE, to be put in front
	@ and end of the function body respectively.
	.macro	DefSCLFncB fncname
	.global	__\fncname
	.type	__\fncname, %function
__\fncname:
	.endm

	.macro	DefSCLFncE fncname
	.size	__\fncname, . - __\fncname
	.weak	\fncname
\fncname = __\fncname
	.endm

	@ Add a SCL function stub entry with labels 'fncname' and '__fncname'
	@ whom the former one is weak definition.
	@ See DefSCLFncS as well.
	.macro	DefSCLFnc fncname
	DefSCLFncB \fncname
	MOV	PC, #0
	DefSCLFncE \fncname
	.endm

	@ Add a SCL function stub entry with label 'fncname'.
	@ See DefSCLFnc as well.
	.macro	DefSCLFncS fncname
	.global	\fncname
	.type	\fncname, %function
\fncname:
	MOV	PC, #0
	.size	\fncname, . - \fncname
	.endm

	@ Create alias set \fncalias and __\fncalias for function routines
	@ \fncname and __\fncname.
	.macro	DefSCLFncAlias fncalias fncname
	.weak	\fncalias
\fncalias = \fncname
	.global	__\fncalias
__\fncalias = __\fncname
	.endm

	@ Same values as in math.h for SCL target:
.set	FP_ZERO, 0
.set	FP_SUBNORMAL, 1
.set	FP_NORMAL, 2
.set	FP_INFINITE, 3
.set	FP_NAN, 4

