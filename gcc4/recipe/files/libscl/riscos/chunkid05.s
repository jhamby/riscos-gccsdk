@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2007 UnixLib Developers
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

	.macro	MakePtr ptrname
#ifdef __TARGET_MODULE__
	.word	\ptrname(GOTOFF)
#else
	.word	\ptrname
#endif
	.endm

	.section .riscos.libscl.chunkstub.id05,"ax",%progbits
	@ Chunk 5 is optional
	.word	5                       @ lib chunk id : Clib extra functions
	MakePtr	extra_vectors_begin
	MakePtr	extra_vectors_end
	.word	0
	.word	0

	.text
extra_vectors_begin:

	@.global	__fpclassifyf
__fpclassifyf_FIXME_float_arg:	MOV	PC, #0
	.global	__fpclassifyd
__fpclassifyd:			MOV	PC, #0
	.global	__signbitf
__signbitf:			MOV	PC, #0
	.global	__signbitd
__signbitd:			MOV	PC, #0
	.global	copysign
copysign:			MOV	PC, #0
	@.global	copysignf
copysignf_not_used:		MOV	PC, #0
	.global	nan
nan:				MOV	PC, #0
	.global	nanf
nanf:				MOV	PC, #0
	.global	nextafter
nextafter:			MOV	PC, #0
	@.global	nextafterf
nextafterf_FIXME_float_arg:	MOV	PC, #0
	.global	fdim
fdim:				MOV	PC, #0
	@.global	fdimf
fdimf_FIXME_float_arg:		MOV	PC, #0
	.global	fmax
fmax:				MOV	PC, #0
	@.global	fmaxf
fmaxf_FIXME_float_arg:		MOV	PC, #0
	.global	fmin
fmin:				MOV	PC, #0
	@.global	fminf
fminf_FIXME_float_arg:		MOV	PC, #0
	@.global	fabsf
fabsf_not_used:			MOV	PC, #0
	.global	hypot
hypot:				MOV	PC, #0
	.global	hypotf
hypotf:				MOV	PC, #0
	.global	feclearexcept
feclearexcept:			MOV	PC, #0
	.global	fegetexceptflag
fegetexceptflag:		MOV	PC, #0
	.global	feraiseexcept
feraiseexcept:			MOV	PC, #0
	.global	fesetexceptflag
fesetexceptflag:		MOV	PC, #0
	.global	fetestexcept
fetestexcept:			MOV	PC, #0
	.global	fegetround
fegetround:			MOV	PC, #0
	.global	fesetround
fesetround:			MOV	PC, #0
	.global	fegetenv
fegetenv:			MOV	PC, #0
	.global	feholdexcept
feholdexcept:			MOV	PC, #0
	.global	fesetenv
fesetenv:			MOV	PC, #0
	.global	feupdateenv
feupdateenv:			MOV	PC, #0
	.global	_snprintf
_snprintf:			MOV	PC, #0
	.global	snprintf
snprintf:			MOV	PC, #0
	.global	__gcc_vsnprintf
	.global	vsnprintf
__gcc_vsnprintf:
vsnprintf:			MOV	PC, #0
	.global	__gcc_vfscanf
	.global	vfscanf
__gcc_vfscanf:
vfscanf:			MOV	PC, #0
	.global	__gcc_vscanf
	.global	vscanf
__gcc_vscanf:
vscanf:				MOV	PC, #0
	.global	__gcc_vsscanf
	.global	vsscanf
__gcc_vsscanf:
vsscanf:			MOV	PC, #0
	@.global	ceilf
ceilf_not_used:			MOV	PC, #0
	@.global	floorf
floorf_not_used:		MOV	PC, #0
	.global	nearbyint
nearbyint:			MOV	PC, #0
	@.global	nearbyintf
nearbyintf_FIXME_float_arg:	MOV	PC, #0
	.global	rint
rint:				MOV	PC, #0
	@.global	rintf
rintf_FIXME_float_arg:		MOV	PC, #0
	.global	lrint
lrint:				MOV	PC, #0
	@.global	lrintf
lrintf_FIXME_float_arg:		MOV	PC, #0
	.global	round
round:				MOV	PC, #0
	@.global	roundf
roundf_FIXME_float_arg:		MOV	PC, #0
	.global	lround
lround:				MOV	PC, #0
	@.global	lroundf
lroundf_FIXME_float_arg:	MOV	PC, #0
	.global	trunc
trunc:				MOV	PC, #0
	@.global	truncf
truncf_FIXME_float_arg:		MOV	PC, #0
	.global	remainder
remainder:			MOV	PC, #0
	@.global	remainderf
remainderf_FIXME_float_arg:	MOV	PC, #0
	.global	llabs
llabs:				MOV	PC, #0
	.global	lldiv
lldiv:				MOV	PC, #0
	.global	atoll
atoll:				MOV	PC, #0
	.global	strtoll
strtoll:			MOV	PC, #0
	.global	strtoull
strtoull:			MOV	PC, #0
	.global	imaxabs
imaxabs:			MOV	PC, #0
	.global	imaxdiv
imaxdiv:			MOV	PC, #0
	.global	strtoimax
strtoimax:			MOV	PC, #0
	.global	strtoumax
strtoumax:			MOV	PC, #0
	.global	__assert2
__assert2:			MOV	PC, #0
	.global	_Exit
_Exit:				MOV	PC, #0
	@.global	acosf
acosf_not_used:			MOV	PC, #0
	@.global	asinf
asinf_not_used:			MOV	PC, #0
	@.global	atanf
atanf_not_used:			MOV	PC, #0
	@.global	atan2f
atan2f_not_used:		MOV	PC, #0
	@.global	cosf
cosf_not_used:			MOV	PC, #0
	@.global	sinf
sinf_not_used:			MOV	PC, #0
	@.global	tanf
tanf_not_used:			MOV	PC, #0
	.global	acosh
acosh:				MOV	PC, #0
	@.global	acoshf
acoshf_FIXME_float_arg:		MOV	PC, #0
	.global	asinh
asinh:				MOV	PC, #0
	@.global	asinhf
asinhf_FIXME_float_arg:		MOV	PC, #0
	.global	atanh
atanh:				MOV	PC, #0
	@.global	atanhf
atanhf_FIXME_float_arg:		MOV	PC, #0
	@.global	expf
expf_not_used:			MOV	PC, #0
	.global	exp2
exp2:				MOV	PC, #0
	@.global	exp2f
exp2f_FIXME_float_arg:		MOV	PC, #0
	.global	expm1
expm1:				MOV	PC, #0
	@.global	expm1f
expm1f_FIXME_float_arg:		MOV	PC, #0
	@.global	frexpf
frexpf_FIXME_float_arg:		MOV	PC, #0
	.global	ilogb
ilogb:				MOV	PC, #0
	@.global	ilogbf
ilogbf_FIXME_float_arg:		MOV	PC, #0
	@.global	ldexpf
ldexpf_FIXME_float_arg:		MOV	PC, #0
	@.global	logf
logf_not_used:			MOV	PC, #0
	@.global	log10f
log10f_FIXME_float_arg:		MOV	PC, #0
	.global	log1p
log1p:				MOV	PC, #0
	@.global	log1pf
log1pf_FIXME_float_arg:		MOV	PC, #0
	.global	log2
log2:				MOV	PC, #0
	@.global	log2f
log2f_FIXME_float_arg:		MOV	PC, #0
	.global	logb
logb:				MOV	PC, #0
	@.global	logbf
logbf_FIXME_float_arg:		MOV	PC, #0
	@.global	modff
modff_not_used:			MOV	PC, #0
	@.global	fmodf
fmodf_FIXME_float_arg:		MOV	PC, #0
	.global	scalbn
scalbn:				MOV	PC, #0
	@.global	scalbnf
scalbnf_FIXME_float_arg:	MOV	PC, #0
	.global	scalbln
scalbln:			MOV	PC, #0
	@.global	scalblnf
scalblnf_FIXME_float_arg:	MOV	PC, #0
	.global	cbrt
cbrt:				MOV	PC, #0
	@.global	cbrtf
cbrtf_FIXME_float_arg:		MOV	PC, #0
	@.global	powf
powf_not_used:			MOV	PC, #0
	@.global	sqrtf
sqrtf_FIXME_float_arg:		MOV	PC, #0
	.global	erf
erf:				MOV	PC, #0
	@.global	erff
erff_FIXME_float_arg:		MOV	PC, #0
	.global	erfc
erfc:				MOV	PC, #0
	@.global	erfcf
erfcf_FIXME_float_arg:		MOV	PC, #0
	.global	lgamma
lgamma:				MOV	PC, #0
	@.global	lgammaf
lgammaf_FIXME_float_arg:	MOV	PC, #0
	.global	tgamma
tgamma:				MOV	PC, #0
	@.global	tgammaf
tgammaf_FIXME_float_arg:	MOV	PC, #0
	.global	nexttoward
nexttoward:			MOV	PC, #0
	@.global	nexttowardf
nexttowardf_FIXME_float_arg:	MOV	PC, #0
	.global	fmaf
fmaf:				MOV	PC, #0
extra_vectors_mid:
	.space			extra_vectors_mid - extra_vectors_begin
extra_vectors_end:

	.end
