@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 2007-2008 UnixLib Developers
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

	.text

	@ This is a small wrapper for the real SharedCLibrary setjmp
	@ implementation storing the head of the alloca() chain in the
	@ setjmp's buffer so that we can restore it at longjmp time.
	.global	setjmp
setjmp:
	ldr	r1, =__libscl_chunk_head
	str	r1, [r0, #22*4]
	b	__libscl_redirected_setjmp

	@ This is a small wrapper for the real SharedCLibrary longjmp
	@ implementation restoring the head of the alloca() chain based
	@ on the 22nd word in the supplied buffer.
	.global	longjmp
longjmp:
	mov	v1, a1
	mov	v2, a2
	ldr	a1, [a1, #22*4]
	ldr	a2, =__libscl_chunk_head
	teq	a1, a2
	blne	__gcc_alloca_longjmp_free
	mov	a1, v1
	mov	a2, v2
	b	__libscl_redirected_longjmp

	.end
