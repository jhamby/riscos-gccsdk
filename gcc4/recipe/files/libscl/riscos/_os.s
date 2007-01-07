; SWI call routine taken from UnixLib for compatibility with riscosify()
; Copyright (c) 1997-2005 UnixLib Developers
; All rights reserved.
 
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
; 1. Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
; 2. Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
; 3. The name of the author may not be used to endorse or promote products
;    derived from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
; IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
; OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
; IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
; INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
; NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
; THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

XOS_CallASWIR12                 EQU     &020071

	AREA	|C$$Code|, CODE, READONLY
	
	EXPORT	|__os_swi|
|__os_swi|
	STMFD	sp!, {a2, v1, v2, v3, v4, v5, v6, lr}
	ORR	ip, a1, #&20000		; X bit
	TEQ	a2, #0
	LDMNEIA a2, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6}
	SWI	XOS_CallASWIR12
	LDMFD	sp!, {lr}		; Retrieve passed in a2
	TEQ	lr, #0
	STMNEIA lr, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6}
	MOVVC	a1, #0
	LDMFD	sp!, {v1, v2, v3, v4, v5, v6, pc}

	END
