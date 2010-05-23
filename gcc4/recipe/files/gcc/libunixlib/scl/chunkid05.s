@ Chunk 5 stub routines for linking with the SharedCLibrary
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

	.section .riscos.libscl.chunkstub.id05,"ax",%progbits
	@ Chunk 5 is optional
	.word	5                       @ Clib extra functions
	MakePtr	extra_vectors_begin
	MakePtr	extra_vectors_end
	.word	0
	.word	0

	.text
extra_vectors:
extra_vectors_begin:

	@ Entry 0
	@ int __fpclassifyf(float);
	@ Returns a classification of the floating point argument passed, as
	@ specified below :
	@   0 (FP_ZERO) = Value is zero
	@   1 (FP_SUBNORMAL) = Value is a sub-normal FP value
	@   2 (FP_NORMAL) = Value is a normal FP value
	@   3 (FP_INFINITY) = Value is an infinity FP value
	@   4 (FP_NAN) = Value is Not-A-Number (NAN) FP value
	@ Note: FP values are passed in FP registers.
	@.global	__fpclassifyf
__fpclassifyf_FIXME_float_arg:	MOV	PC, #0

	@ Entry 1
	@ int __fpclassifyd(double);
	@ Returns a classification of the floating point argument passed, as
	@ specified below :
	@   0 (FP_ZERO) = Value is zero
	@   1 (FP_SUBNORMAL) = Value is a sub-normal FP value
	@   2 (FP_NORMAL) = Value is a normal FP value
	@   3 (FP_INFINITY) = Value is an infinity FP value
	@   4 (FP_NAN) = Value is Not-A-Number (NAN) FP value
	@ Note: FP values are passed in FP registers.
	.global	__fpclassifyd
__fpclassifyd:
	MOV	PC, #0

	@ Entry 2
	@ int __signbitf(float);
	@ Returns the value of the sign bit for the floating point argument
	@ passed. 1 if the sign bit is set, 0 if the sign bit is clear.
	@ Note: FP values are passed in FP registers.
	.global	__signbitf
__signbitf:
	MOV	PC, #0

	@ Entry 3
	@ int __signbitd(double);
	@ Returns the value of the sign bit for the floating point argument
	@ passed. 1 if the sign bit is set, 0 if the sign bit is clear.
	@ Note: FP values are passed in FP registers.
	.global	__signbitd
__signbitd:
	MOV	PC, #0

	@ Entry 4
	@ double copysign(double x, double y);
	@ Returns a floating point value whose sign matches that of y, and
	@ whose magnitude is that of x. Effectively, copies the sign of y
	@ into x and returns it.
	@ Note: FP values are passed in FP registers.
	DefSCLFnc	copysign

	@ Entry 5
	@ floats copysignf(float x, float y);
	@ Returns a floating point value whose sign matches that of y, and
	@ whose magnitude is that of x. Effectively, copies the sign of y
	@ into x and returns it.
	@ Note: FP values are passed in FP registers.
	@.global	copysignf
copysignf_not_used:
	MOV	PC, #0


	@ Entry 6
	@ double nan(const char *tag);
	@ Returns a Not-A-Number (NAN) FP value. The value of tag is
	@ implementation defined. Current implementations ignore the value of
	@ tag.
	DefSCLFnc	nan

	@ Entry 7
	@ float nanf(const char *tag);
	@ Returns a Not-A-Number (NAN) FP value. The value of tag is
	@ implementation defined. Current implementations ignore the value of
	@ tag.
	DefSCLFnc	nanf

	@ Entry 8
	@ double nextafter(double x, double y);
	@ Returns the next representable value in the specified format after
	@ x in the direction of y.
	DefSCLFnc	nextafter

	@ Entry 9
	@ float nextafterf(float x, float y);
	@ Returns the next representable value in the specified format after
	@ x in the direction of y.
	@.global	nextafterf
nextafterf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 10
	@ double fdim(double x, double y);
	@ Determines the positive difference between the arguments:
	@ if x > y, returns x-y
	@ if x <= y, returns +0
	@ A range error may occur.
	@ Returns: the positive difference value.
	DefSCLFnc	fdim

	@ Entry 11
	@ float fdimf(float x, float y);
	@ Determines the positive difference between the arguments:
	@ if x > y, returns x-y
	@ if x <= y, returns +0
	@ A range error may occur.
	@ Returns: the positive difference value.
	@.global	fdimf
fdimf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 12
	@ double fmax(double x, double y);
	@ Returns the higher of the two arguments.
	DefSCLFnc	fmax

	@ Entry 13
	@ float fmaxf(float x, float y);
	@ Returns the higher of the two arguments.
	@.global	fmaxf
fmaxf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 14
	@ double fmin(double x, double y);
	@ Returns the lower of the two arguments.
	DefSCLFnc	fmin

	@ Entry 15
	@ float fminf(float x, float y);
	@ Returns the lower of the two arguments.
	@.global	fminf
fminf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 16
	@ float fabsf(float x);
	@ Returns the absolute value of the argument.
	@.global	fabsf
fabsf_not_used:
	MOV	PC, #0

	@ Entry 17
	@ double hypot(double x, double y);
	@ Returns the square root of the sum of the squares of x and y,
	@ without undue overflow or underflow.
	@ A range error may occur.
	DefSCLFnc	hypot

	@ Entry 18
	@ float hypotf(float x, float y);
	@ Returns the square root of the sum of the squares of x and y,
	@ without undue overflow or underflow.
	@ A range error may occur.
	@.global	hypotf
hypotf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 19
	@ int feclearexcept(int exceptions);
	@ Clears floating point exceptions.
	@ Returns the bit mask of the exceptions which could not be cleared.
	@ Exceptions bit mask :
	@   bit 0 = Invalid operation
	@   bit 1 = Division by zero
	@   bit 2 = Overflow
	@   bit 3 = Underflow
	@   bit 4 = Inexact
	DefSCLFnc	feclearexcept

	@ Entry 20
	@ int fegetexceptflag(unsigned int *flagsp, int exceptions);
	@ Reads the representation of the FP status flags indicated by the
	@ exceptions arguments.
	@ Returns zero if successful, non-zero if failed.
	DefSCLFnc	fegetexceptflag

	@ Entry 21
	@ int feraiseexcept(int exceptions);
	@ Attempts to raise the floating point exceptions specified within
	@ the exceptions flags.
	@ Returns zero if successful, non-zero if failed.
	DefSCLFnc	feraiseexcept

	@ Entry 22
	@ int fesetexceptflag(unsigned int *flagsp, int exceptions);
	@ Attempts to set the status flags indicated by the exceptions
	@ argument to those stored in the value pointed to by flagsp.
	@ This function can be used together with fegetexceptflag() to
	@ preserve and restore floating point status flags.
	@ Returns zero if successful, or non-zero if failed.
	DefSCLFnc	fesetexceptflag

	@ Entry 23
	@ int fetestexcept(int exceptions);
	@ Determines which of the exceptions specified in the argument are
	@ currently set.
	@ Returns the exceptions which are currently set, from those
	@ specified within the argument.
	DefSCLFnc	fetestexcept

	@ Entry 24
	@ int fegetround(void);
	@ Reads the current rounding direction.
	@ Returns 0 indicating 'round to nearest'.
	@ The current implementation of the ARM FP system cannot change its
	@ rounding direction at runtime.
	DefSCLFnc	fegetround

	@ Entry 25
	@ int fesetround(int direction);
	@ Changes the rounding direction.
	@ Returns 0 indicating success if the direction specified was 0, or 1
	@ indicating a failure otherwise.
	DefSCLFnc	fesetround

	@ Entry 26
	@ int fegetenv(const fenv_t *envp);
	@ Reads the current floating point environment into the structure
	@ specified. The structure must be 6 words long.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFnc	fegetenv

	@ Entry 27
	@ int feholdexcept(const fenv_t *envp);
	@ Reads the current floating point environment into the structure
	@ specified, clears any pending exceptions and disables all floating
	@ point exceptions.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFnc	feholdexcept

	@ Entry 28
	@ int fesetenv(const fenv_t *envp);
	@ Attempts to set the floating point enviroment to that stored in the
	@ argument. No exceptions are raised.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFnc	fesetenv

	@ Entry 29
	@ int feupdateenv(const fent_t *envp);
	@ Attempts to set the floating point enviroment to that stored in the
	@ argument. Exceptions are then raised.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFnc	feupdateenv

	@ Entry 30
	@ int _snprintf(char *output, size_t max, const char *format, ...);
	@ Integer-only form of sprintf, limited to max output bytes.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	.global	_snprintf
_snprintf:
	MOV	PC, #0

	@ Entry 31
	@ int snprintf(char *output, size_t max, const char *format, ...);
	@ Form of sprintf, limited to max output bytes.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	DefSCLFnc	snprintf

	@ Entry 32
	@ int vsnprintf(char *output, size_t max, const char *format, va_list ap);
	@ Variadic arguments form of snprintf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	.global	__gcc_vsnprintf
	.global	vsnprintf
__gcc_vsnprintf:
vsnprintf:
	MOV	PC, #0

	@ Entry 33
	@ int vfscanf(FILE *f, const char *format, va_list);
	@ Variadic form of fscanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	.global	__gcc_vfscanf
	.global	vfscanf
__gcc_vfscanf:
vfscanf:
	MOV	PC, #0

	@ Entry 34
	@ int vscanf(const char *format, va_list);
	@ Variadic form of scanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	.global	__gcc_vscanf
	.global	vscanf
__gcc_vscanf:
vscanf:
	MOV	PC, #0

	@ Entry 35
	@ int vsscanf(const char *string, const char *format, va_list);
	@ Variadic form of sscanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	.global	__gcc_vsscanf
	.global	vsscanf
__gcc_vsscanf:
vsscanf:
	MOV	PC, #0

	@ Entry 36
	@ float ceilf(float x);
	@ Returns the smallest integer not less than x.
	@.global	ceilf
ceilf_not_used:
	MOV	PC, #0

	@ Entry 37
	@ float floorf(float x);
	@ Returns the largest integer not less than x.
	@.global	floorf
floorf_not_used:
	MOV	PC, #0

	@ Entry 38
	@ double nearbyint(double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Does not raise the inexact exception.
	DefSCLFnc	nearbyint

	@ Entry 39
	@ float nearbyintf(float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Does not raise the inexact exception.
	@.global	nearbyintf
nearbyintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 40
	@ double rint(double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	DefSCLFnc	rint

	@ Entry 41
	@ float rintf(float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	@.global	rintf
rintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 42
	@ long int lrint(double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	DefSCLFnc	lrint

	@ Entry 43
	@ long int lrintf(float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	@.global	lrintf
lrintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 44
	@ double round(double x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	DefSCLFnc	round

	@ Entry 45
	@ float roundf(float x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	@.global	roundf
roundf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 46
	@ long int lround(double x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	DefSCLFnc	lround

	@ Entry 47
	@ long int lroundf(float x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	@.global	lroundf
lroundf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 48
	@ double trunc(double x);
	@ Rounds its argument to the integer value, nearest to but no larger
	@ in magnitude than the argument.
	DefSCLFnc	trunc

	@ Entry 49
	@ float truncf(float x);
	@ Rounds its argument to the integer value, nearest to but no larger
	@ in magnitude than the argument.	@.global	truncf
truncf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 50
	@ double remainder(double x);
	@ Computes the remainder x REM y required by IEEE 754
	DefSCLFnc	remainder

	@ Entry 51
	@ float remainderf(float x);
	@.global	remainderf
remainderf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 52
	@ long long llabs(long long j);
	@ Returns the absolute value of j.
	DefSCLFnc	llabs

	@ Entry 53
	@ lldiv_t lldiv(long long j, long long k);
	@ Computes numer / denom and numer % denom in a single operation.
	@ Returns: a structure of type lldiv_t, comprising both the
	@ quotient and the remainder.
	@  typedef struct lldiv_t {
	@    long quot;
	@    long long rem;
	@    } lldiv_t;
	DefSCLFnc	lldiv

	@ Entry 54
	@ long long atoll(const char *s);
	@ Returns the value of the string in s. This is equivalent to
	@ strtoll(s, (char **)NULL, 10); with the exception that errno is
	@ unaffected by the conversion.
	DefSCLFnc	atoll

	@ Entry 55
	@ long long strtoll(const char *s, char **e, int b);
	@ Returns the value of the string in s, given in base b, storing the
	@ end of the conversion in pointer at e. e may be NULL if no end
	@ pointer is required. b may be 0 to indicate that the number is
	@ decimal, octal, or hexadecimal based on the prefix - '0x' indicates
	@ hexadecimal, '0' indicates octal, and all other values are decimal.
	@ Leading spaces, as determined by isspace(), are skipped. If b is 16,
	@ the string may be prefixed by '0x' and this will be skipped. The
	@ base may be any value from 2 to 36. If the value cannot be converted
	@ because it is too large, it will return LLONG_MAX or LLONG_MIN,
	@ depending on whether the overflowing value was positive or negative,
	@ and errno will be set to ERANGE.
	DefSCLFnc	strtoll

	@ Entry 56
	@ unsigned long long strtoull(const char *s, char **e, int b);
	@ Returns the value of the string in s as an unsigned value, given in
	@ base b, storing the end of the conversion in pointer at e. This
	@ function is identical to strtoll, except that underflow cannot be
	@ reported, and an overflow is indicated by ULLONG_MAX.
	DefSCLFnc	strtoull

	@ Entry 57
	@ long long imaxabs(long long i);
	@ Returns the absolute value of a 64bit value.
	@ Equivalent to llabs.
	DefSCLFnc	imaxabs

	@ Entry 58
	@ intmax_t imaxdiv(long long j, long long k);
	@ Divides an intmax_t value by a second intmax_t value.
	@ Equivalent to lldiv.
	DefSCLFnc	imaxdiv

	@ Entry 59
	@ imax_t strtoimax((const char *s, char **e, int b);
	@ This function is equivalent to strtoll.
	DefSCLFnc	strtoimax

	@ Entry 60
	@ imax_t strtouimax((const char *s, char **e, int b);
	@ This function is equivalent to strtoull.
	DefSCLFnc	strtoumax

	@ Entry 61
	@ void __assert2(const char *message, const char *func, const char *file, int line);
	@ Displays an error to stderr in response to an assertion, and then
	@ calls abort().
	@ Never returns.
	.global	__assert2
__assert2:
	MOV	PC, #0

	@ Entry 62
	@ void _Exit(int status),
	@ Terminates the program immediately, without calling the 'atexit'
	@ functions. This may mean under a C++ environment that destructors
	@ are not invoked. File descriptors opened by the standard stream
	@ functions will be closed.
	.global	_Exit
_Exit:
	MOV	PC, #0

	@ Entry 63
	@
	@.global	acosf
acosf_not_used:
	MOV	PC, #0

	@ Entry 64
	@
	@.global	asinf
asinf_not_used:
	MOV	PC, #0

	@ Entry 65
	@
	@.global	atanf
atanf_not_used:
	MOV	PC, #0

	@ Entry 66
	@
	@.global	atan2f
atan2f_not_used:
	MOV	PC, #0

	@ Entry 67
	@
	@.global	cosf
cosf_not_used:
	MOV	PC, #0

	@ Entry 68
	@
	@.global	sinf
sinf_not_used:
	MOV	PC, #0

	@ Entry 69
	@
	@.global	tanf
tanf_not_used:
	MOV	PC, #0

	@ Entry 70
	@
	DefSCLFnc	acosh

	@ Entry 71
	@
	@.global	acoshf
acoshf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 72
	@
	DefSCLFnc	asinh

	@ Entry 73
	@
	@.global	asinhf
asinhf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 74
	@
	DefSCLFnc	atanh

	@ Entry 75
	@
	@.global	atanhf
atanhf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 76
	@
	@.global	expf
expf_not_used:
	MOV	PC, #0

	@ Entry 77
	@
	DefSCLFnc	exp2

	@ Entry 78
	@
	@.global	exp2f
exp2f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 79
	@
	DefSCLFnc	expm1

	@ Entry 80
	@
	@.global	expm1f
expm1f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 81
	@
	@.global	frexpf
frexpf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 82
	@
	DefSCLFnc	ilogb

	@ Entry 83
	@
	@.global	ilogbf
ilogbf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 84
	@
	@.global	ldexpf
ldexpf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 85
	@
	@.global	logf
logf_not_used:
	MOV	PC, #0

	@ Entry 86
	@
	@.global	log10f
log10f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 87
	@
	DefSCLFnc	log1p

	@ Entry 88
	@
	@.global	log1pf
log1pf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 89
	@
	DefSCLFnc	log2

	@ Entry 90
	@
	@.global	log2f
log2f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 91
	@
	DefSCLFnc	logb

	@ Entry 92
	@
	@.global	logbf
logbf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 93
	@
	@.global	modff
modff_not_used:
	MOV	PC, #0

	@ Entry 94
	@
	@.global	fmodf
fmodf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 95
	@
	DefSCLFnc	scalbn

	@ Entry 96
	@
	@.global	scalbnf
scalbnf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 97
	@
	DefSCLFnc	scalbln

	@ Entry 98
	@
	@.global	scalblnf
scalblnf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 99
	@
	DefSCLFnc	cbrt

	@ Entry 100
	@
	@.global	cbrtf
cbrtf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 101
	@
	@.global	powf
powf_not_used:
	MOV	PC, #0

	@ Entry 102
	@
	@.global	sqrtf
sqrtf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 103
	@
	DefSCLFnc	erf

	@ Entry 104
	@
	@.global	erff
erff_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 105
	@
	DefSCLFnc	erfc

	@ Entry 106
	@
	@.global	erfcf
erfcf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 107
	@
	DefSCLFnc	lgamma

	@ Entry 108
	@
	@.global	lgammaf
lgammaf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 109
	@
	DefSCLFnc	tgamma

	@ Entry 110
	@
	@.global	tgammaf
tgammaf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 111
	@
	DefSCLFnc	nexttoward

	@ Entry 112
	@
	@.global	nexttowardf
nexttowardf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 113
	@
	DefSCLFnc	fmaf

extra_vectors_end:
	.space	extra_vectors_end - extra_vectors_begin
	.size	extra_vectors, . - extra_vectors

	.end
