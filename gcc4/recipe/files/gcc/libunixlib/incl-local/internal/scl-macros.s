@ libscl specific macros used by all its assembler files.
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

	.macro	MakePtr ptrname
#ifdef __TARGET_MODULE__
	.word	\ptrname(GOTOFF)
#else
	.word	\ptrname
#endif
	.endm

	@ Add global labels 'fncname' and '__fncname'
	@ whom the former one is weak definition.
	.macro	DefSCLGbl fncname
	.global	__\fncname
	.type	__\fncname, %function
	.global	\fncname
	.type	\fncname, %function
	.weak	\fncname
\fncname:
__\fncname:
	.endm

	@ Add a function stub entry with labels 'fncname' and '__fncname'
	@ whom the former one is weak definition.
	.macro	DefSCLFnc fncname
	DefSCLGbl \fncname
	MOV	PC, #0
	.endm

