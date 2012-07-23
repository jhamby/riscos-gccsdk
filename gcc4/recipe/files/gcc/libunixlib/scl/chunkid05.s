@ Chunk 5 stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2012 UnixLib Developers
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
	@ int __fpclassifyf (float);
	@ Returns a classification of the floating point argument passed, as
	@ specified below :
	@   0 (FP_ZERO) = Value is zero
	@   1 (FP_SUBNORMAL) = Value is a sub-normal FP value
	@   2 (FP_NORMAL) = Value is a normal FP value
	@   3 (FP_INFINITY) = Value is an infinity FP value
	@   4 (FP_NAN) = Value is Not-A-Number (NAN) FP value
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_fpclassifyf.s
	MOV	PC, #0

	@ Entry 1
	@ int __fpclassifyd (double);
	@ Returns a classification of the floating point argument passed, as
	@ specified below :
	@   0 (FP_ZERO) = Value is zero
	@   1 (FP_SUBNORMAL) = Value is a sub-normal FP value
	@   2 (FP_NORMAL) = Value is a normal FP value
	@   3 (FP_INFINITY) = Value is an infinity FP value
	@   4 (FP_NAN) = Value is Not-A-Number (NAN) FP value
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_fpclassify.s (as __fpclassify)
	MOV	PC, #0

	@ Entry 2
	@ int __signbitf (float);
	@ Returns the value of the sign bit for the floating point argument
	@ passed. 1 if the sign bit is set, 0 if the sign bit is clear.
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_signbit.s
	MOV	PC, #0

	@ Entry 3
	@ int __signbitd (double);
	@ Returns the value of the sign bit for the floating point argument
	@ passed. 1 if the sign bit is set, 0 if the sign bit is clear.
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_signbit.s
	MOV	PC, #0

	@ Entry 4
	@ double copysign (double x, double y);
	@ Returns a floating point value whose sign matches that of y, and
	@ whose magnitude is that of x. Effectively, copies the sign of y
	@ into x and returns it.
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_copysign.s
	MOV	PC, #0

	@ Entry 5
	@ floats copysignf (float x, float y);
	@ Returns a floating point value whose sign matches that of y, and
	@ whose magnitude is that of x. Effectively, copies the sign of y
	@ into x and returns it.
	@ Note: FP values are passed in FP registers.
	@ Implemented in scl_copysignf.s
	MOV	PC, #0

	@ Entry 6
	@ double nan (const char *tag);
	@ Returns a Not-A-Number (NAN) FP value. The value of tag is
	@ implementation defined. Current implementations ignore the value of
	@ tag.
	DefSCLFltFnc	nan

	@ Entry 7
	@ float nanf (const char *tag);
	@ Returns a Not-A-Number (NAN) FP value. The value of tag is
	@ implementation defined. Current implementations ignore the value of
	@ tag.
	DefSCLFltFnc	nanf

	@ Entry 8
	@ double nextafter (double x, double y);
	@ Returns the next representable value in the specified format after
	@ x in the direction of y.
	DefSCLFltFnc	nextafter

	@ Entry 9
	@ float nextafterf (float x, float y);
	@ Returns the next representable value in the specified format after
	@ x in the direction of y.
nextafterf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 10
	@ double fdim (double x, double y);
	@ Determines the positive difference between the arguments:
	@ if x > y, returns x-y
	@ if x <= y, returns +0
	@ A range error may occur.
	@ Returns: the positive difference value.
	DefSCLFltFnc	fdim

	@ Entry 11
	@ float fdimf (float x, float y);
	@ Determines the positive difference between the arguments:
	@ if x > y, returns x-y
	@ if x <= y, returns +0
	@ A range error may occur.
	@ Returns: the positive difference value.
fdimf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 12
	@ double fmax (double x, double y);
	@ Returns the higher of the two arguments.
	DefSCLFltFnc	fmax

	@ Entry 13
	@ float fmaxf (float x, float y);
	@ Returns the higher of the two arguments.
fmaxf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 14
	@ double fmin (double x, double y);
	@ Returns the lower of the two arguments.
	DefSCLFltFnc	fmin

	@ Entry 15
	@ float fminf (float x, float y);
	@ Returns the lower of the two arguments.
fminf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 16
	@ float fabsf (float x);
	@ Returns the absolute value of the argument.
	@ Implemented in scl_fabsf.s
	MOV	PC, #0

	@ Entry 17
	@ double hypot (double x, double y);
	@ Returns the square root of the sum of the squares of x and y,
	@ without undue overflow or underflow.
	@ A range error may occur.
	DefSCLFltFnc	hypot

	@ Entry 18
	@ float hypotf (float x, float y);
	@ Returns the square root of the sum of the squares of x and y,
	@ without undue overflow or underflow.
	@ A range error may occur.
	@ Implemented in scl_hypotf.s
	MOV	PC, #0

	@ Entry 19
	@ int feclearexcept (int exceptions);
	@ Clears floating point exceptions.
	@ Returns the bit mask of the exceptions which could not be cleared.
	@ Exceptions bit mask :
	@   bit 0 = Invalid operation
	@   bit 1 = Division by zero
	@   bit 2 = Overflow
	@   bit 3 = Underflow
	@   bit 4 = Inexact
	DefSCLFltFnc	feclearexcept

	@ Entry 20
	@ int fegetexceptflag (unsigned int *flagsp, int exceptions);
	@ Reads the representation of the FP status flags indicated by the
	@ exceptions arguments.
	@ Returns zero if successful, non-zero if failed.
	DefSCLFltFnc	fegetexceptflag

	@ Entry 21
	@ int feraiseexcept (int exceptions);
	@ Attempts to raise the floating point exceptions specified within
	@ the exceptions flags.
	@ Returns zero if successful, non-zero if failed.
	DefSCLFltFnc	feraiseexcept

	@ Entry 22
	@ int fesetexceptflag (unsigned int *flagsp, int exceptions);
	@ Attempts to set the status flags indicated by the exceptions
	@ argument to those stored in the value pointed to by flagsp.
	@ This function can be used together with fegetexceptflag () to
	@ preserve and restore floating point status flags.
	@ Returns zero if successful, or non-zero if failed.
	DefSCLFltFnc	fesetexceptflag

	@ Entry 23
	@ int fetestexcept (int exceptions);
	@ Determines which of the exceptions specified in the argument are
	@ currently set.
	@ Returns the exceptions which are currently set, from those
	@ specified within the argument.
	DefSCLFltFnc	fetestexcept

	@ Entry 24
	@ int fegetround (void);
	@ Reads the current rounding direction.
	@ Returns 0 indicating 'round to nearest'.
	@ The current implementation of the ARM FP system cannot change its
	@ rounding direction at runtime.
	DefSCLFltFnc	fegetround

	@ Entry 25
	@ int fesetround (int direction);
	@ Changes the rounding direction.
	@ Returns 0 indicating success if the direction specified was 0, or 1
	@ indicating a failure otherwise.
	DefSCLFltFnc	fesetround

	@ Entry 26
	@ int fegetenv (const fenv_t *envp);
	@ Reads the current floating point environment into the structure
	@ specified. The structure must be 6 words long.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFltFnc	fegetenv

	@ Entry 27
	@ int feholdexcept (const fenv_t *envp);
	@ Reads the current floating point environment into the structure
	@ specified, clears any pending exceptions and disables all floating
	@ point exceptions.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFltFnc	feholdexcept

	@ Entry 28
	@ int fesetenv (const fenv_t *envp);
	@ Attempts to set the floating point enviroment to that stored in the
	@ argument. No exceptions are raised.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFltFnc	fesetenv

	@ Entry 29
	@ int feupdateenv (const fent_t *envp);
	@ Attempts to set the floating point enviroment to that stored in the
	@ argument. Exceptions are then raised.
	@ Returns 0 if successful, or 1 if failed.
	DefSCLFltFnc	feupdateenv

	@ Entry 30
	@ int _snprintf (char *output, size_t max, const char *format, ...);
	@ Integer-only form of sprintf, limited to max output bytes.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	DefSCLFncS	_snprintf

	@ Entry 31
	@ int snprintf (char *output, size_t max, const char *format, ...);
	@ Form of sprintf, limited to max output bytes.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	DefSCLFnc	snprintf

	@ Entry 32
	@ int vsnprintf (char *output, size_t max, const char *format, va_list ap);
	@ Variadic arguments form of snprintf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of bytes which have been written to the output
	@ buffer, or would have been written if the buffer had been long
	@ enough.
	DefSCLFnc	vsnprintf
	.global	__gcc_vsnprintf
__gcc_vsnprintf = vsnprintf

	@ Entry 33
	@ int vfscanf (FILE *f, const char *format, va_list);
	@ Variadic form of fscanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	DefSCLFnc	vfscanf
	.global	__gcc_vfscanf
__gcc_vfscanf = vfscanf

	@ Entry 34
	@ int vscanf (const char *format, va_list);
	@ Variadic form of scanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	DefSCLFnc	vscanf
	.global	__gcc_vscanf
__gcc_vscanf = vscanf

	@ Entry 35
	@ int vsscanf (const char *string, const char *format, va_list);
	@ Variadic form of sscanf.
	@ Consult the C specification for details of the format parameters.
	@ Returns the number of arguments processed, or EOF if failed before
	@ any conversion.
	DefSCLFnc	vsscanf
	.global	__gcc_vsscanf
__gcc_vsscanf = vsscanf

	@ Entry 36
	@ float ceilf (float x);
	@ Returns the smallest integer not less than x.
	@ Implemented in scl_ceilf.s
	MOV	PC, #0

	@ Entry 37
	@ float floorf (float x);
	@ Returns the largest integer not less than x.
	@ Implemented in scl_floorf.s
	MOV	PC, #0

	@ Entry 38
	@ double nearbyint (double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Does not raise the inexact exception.
	DefSCLFltFnc	nearbyint

	@ Entry 39
	@ float nearbyintf (float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Does not raise the inexact exception.
nearbyintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 40
	@ double rint (double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	DefSCLFltFnc	rint

	@ Entry 41
	@ float rintf (float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
rintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 42
	@ long int lrint (double x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
	DefSCLFltFnc	lrint

	@ Entry 43
	@ long int lrintf (float x);
	@ Rounds its argument to an integer value, using the current rounding
	@ direction. Raises "inexact" if the result differs from the argument.
lrintf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 44
	@ double round (double x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	DefSCLFltFnc	round

	@ Entry 45
	@ float roundf (float x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
roundf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 46
	@ long int lround (double x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
	DefSCLFltFnc	lround

	@ Entry 47
	@ long int lroundf (float x);
	@ Rounds its argument to the nearest integer value, rounding halfway
	@ cases away from zero.
lroundf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 48
	@ double trunc (double x);
	@ Rounds its argument to the integer value, nearest to but no larger
	@ in magnitude than the argument.
	DefSCLFltFnc	trunc

	@ Entry 49
	@ float truncf (float x);
	@ Rounds its argument to the integer value, nearest to but no larger
	@ in magnitude than the argument.
truncf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 50
	@ double remainder (double x);
	@ Computes the remainder x REM y required by IEEE 754
	DefSCLFltFnc	remainder

	@ Entry 51
	@ float remainderf (float x);
remainderf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 52
	@ long long llabs (long long j);
	@ Returns the absolute value of j.
	DefSCLFnc	llabs

	@ Entry 53
	@ lldiv_t lldiv (long long j, long long k);
	@ Computes numer / denom and numer % denom in a single operation.
	@ Returns: a structure of type lldiv_t, comprising both the
	@ quotient and the remainder.
	@  typedef struct lldiv_t {
	@    long quot;
	@    long long rem;
	@    } lldiv_t;
	DefSCLFnc	lldiv

	@ Entry 54
	@ long long atoll (const char *s);
	@ Returns the value of the string in s. This is equivalent to
	@ strtoll (s, (char **)NULL, 10); with the exception that errno is
	@ unaffected by the conversion.
	DefSCLFnc	atoll

	@ Entry 55
	@ long long strtoll (const char *s, char **e, int b);
	@ Returns the value of the string in s, given in base b, storing the
	@ end of the conversion in pointer at e. e may be NULL if no end
	@ pointer is required. b may be 0 to indicate that the number is
	@ decimal, octal, or hexadecimal based on the prefix - '0x' indicates
	@ hexadecimal, '0' indicates octal, and all other values are decimal.
	@ Leading spaces, as determined by isspace (), are skipped. If b is 16,
	@ the string may be prefixed by '0x' and this will be skipped. The
	@ base may be any value from 2 to 36. If the value cannot be converted
	@ because it is too large, it will return LLONG_MAX or LLONG_MIN,
	@ depending on whether the overflowing value was positive or negative,
	@ and errno will be set to ERANGE.
	DefSCLFnc	strtoll

	@ Entry 56
	@ unsigned long long strtoull (const char *s, char **e, int b);
	@ Returns the value of the string in s as an unsigned value, given in
	@ base b, storing the end of the conversion in pointer at e. This
	@ function is identical to strtoll, except that underflow cannot be
	@ reported, and an overflow is indicated by ULLONG_MAX.
	DefSCLFnc	strtoull

	@ Entry 57
	@ long long imaxabs (long long i);
	@ Returns the absolute value of a 64bit value.
	@ Equivalent to llabs.
	DefSCLFnc	imaxabs

	@ Entry 58
	@ intmax_t imaxdiv (long long j, long long k);
	@ Divides an intmax_t value by a second intmax_t value.
	@ Equivalent to lldiv.
	DefSCLFnc	imaxdiv

	@ Entry 59
	@ imax_t strtoimax ( (const char *s, char **e, int b);
	@ This function is equivalent to strtoll.
	DefSCLFnc	strtoimax

	@ Entry 60
	@ imax_t strtouimax ( (const char *s, char **e, int b);
	@ This function is equivalent to strtoull.
	DefSCLFnc	strtoumax

	@ Entry 61
	@ void __assert2 (const char *message, const char *func, const char *file, int line);
	@ Displays an error to stderr in response to an assertion, and then
	@ calls abort ().
	@ Never returns.
	DefSCLFncS	__assert2

	@ Entry 62
	@ void _Exit (int status);
	@ Terminates the program immediately, without calling the 'atexit'
	@ functions. This may mean under a C++ environment that destructors
	@ are not invoked. File descriptors opened by the standard stream
	@ functions will be closed.
	DefSCLFncS	_Exit

	@ Entry 63
	@ Arc cosine function
	@ float acosf (float x);
	@ Implemented in scl_acosf.s
	MOV	PC, #0

	@ Entry 64
	@ Arc sine function
	@ float asinf (float x);
	@ Implemented in scl_asinf.s
	MOV	PC, #0

	@ Entry 65
	@ Arc tangent function
	@ float atanf (float x);
	@ Implemented in scl_atanf.s
	MOV	PC, #0

	@ Entry 66
	@ Arc tangent function of two variables
	@ float atan2f (float y, float x);
	@ Implemented in scl_atan2f.s
	MOV	PC, #0

	@ Entry 67
	@ Cosine function
	@ float cosf (float x);
	@ Implemented in scl_cosf.s
	MOV	PC, #0

	@ Entry 68
	@ Sine function
	@ float sinf (float x);
	@ Implemented in scl_sinf.s
	MOV	PC, #0

	@ Entry 69
	@ Tangent function
	@ float tanf (float x);
	@ Implemented in scl_tanf.s
	MOV	PC, #0

	@ Entry 70
	@ Inverse hyperbolic cosine function
	@ double acosh (double x);
	DefSCLFltFnc	acosh
	@ long double acoshl (long double x);
	DefSCLFltFncAlias	acoshl acosh

	@ Entry 71
	@ Inverse hyperbolic cosine function
	@ float acoshf (float x);
	@ Implemented in scl_acoshf.s
	MOV	PC, #0

	@ Entry 72
	@ Inverse hyperbolic sine function
	@ double asinh (double x);
	DefSCLFltFnc	asinh
	@ long double asinhl (long double x);
	DefSCLFltFncAlias	asinhl asinh

	@ Entry 73
	@ Inverse hyperbolic sine function
	@ float asinhf (float x);
	@ Implemented in scl_sinhf.s
	MOV	PC, #0

	@ Entry 74
	@ Inverse hyperbolic tangent function
	@ double atanh (double x);
	DefSCLFnc	atanh
	@ long double atanhl (long double x);
	DefSCLFltFncAlias	atanhl atanh

	@ Entry 75
	@ Inverse hyperbolic tangent function
	@ float atanhf (float x);
	MOV	PC, #0		@ FIXME

	@ Entry 76
	@ Base-e exponential function
	@ float expf (float x);
	@ Implemented in scl_expf.s
	MOV	PC, #0

	@ Entry 77
	@ Base-2 exponential function
	@ double exp2 (double x);
	DefSCLFltFnc	exp2
	@ long double exp2l (long double x);
	DefSCLFltFncAlias	exp2l exp2

	@ Entry 78
	@ Base-2 exponential function
	@ float exp2f (float x);
exp2f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 79
	@ Exponential minus 1
	@ double expm1 (double x);
	DefSCLFltFnc	expm1
	@ long double expm1l (long double x);
	DefSCLFltFncAlias	expm1l expm1

	@ Entry 80
	@ float expm1f (float x);
expm1f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 81
	@ Convert floating-point number to fractional and integral components
	@ float frexpf (float x, int *exp);
frexpf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 82
	@ Get integer exponent of a floating-point value
	@ int ilogb (double x);
	DefSCLFltFnc	ilogb
	@ int ilogbl (long double x);
	DefSCLFltFncAlias	ilogbl ilogb

	@ Entry 83
	@ Get integer exponent of a floating-point value
	@ int ilogbf (float x);
ilogbf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 84
	@ Multiply floating-point number by integral power of 2
	@ float ldexpf (float x, int exp);
	@ Implemented in scl_ldexpf.s
	MOV	PC, #0

	@ Entry 85
	@ Natural logarithmic function
	@ float logf (float x);
	@ Implemented in scl_logf.s
	MOV	PC, #0

	@ Entry 86
	@ Base-10 logarithmic function
	@ float log10f (float x);
	@ Implemented in scl_log10f.s
	MOV	PC, #0

	@ Entry 87
	@ Logarithm of 1 plus argument
	@ double log1p (double x);
	DefSCLFltFnc	log1p
	@ long double log1pl (long double x);
	DefSCLFltFncAlias	log1pl log1p

	@ Entry 88
	@ Logarithm of 1 plus argument
	@ float log1pf (float x);
log1pf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 89
	@ Base-2 logarithmic function
	@ double log2 (double x);
	DefSCLFltFnc	log2
	@ long double log2l (long double x);
	DefSCLFltFncAlias	log2l log2

	@ Entry 90
	@ Base-2 logarithmic function
	@ float log2f (float x);
log2f_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 91
	@ Get exponent of a floating-point value
	@ double logb (double x);
	DefSCLFltFnc	logb
	@ long double logbl (long double x);
	DefSCLFltFncAlias	logbl logb

	@ Entry 92
	@ Get exponent of a floating-point value
	@ float logbf (float x);
logbf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 93
	@ Extract signed integral and fractional values from floating-point number
	@ float modff (float x, float *iptr);
	@ Implemented in scl_modff.s
	MOV	PC, #0

	@ Entry 94
	@ Floating-point remainder function
	@ float fmodf (float x, float y);
fmodf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 95
	@ Multiply floating-point number by integral power of radix
	@ double scalbn (double x, int exp);
	DefSCLFltFnc	scalbn
	@ long double scalbnl (long double x, int exp);
	DefSCLFltFncAlias	scalbnl scalbn

	@ Entry 96
	@ Multiply floating-point number by integral power of radix
	@ float scalbnf (float x, int exp);
scalbnf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 97
	@ Multiply floating-point number by integral power of radix
	@ double scalbln (double x, long int exp);
	DefSCLFltFnc	scalbln
	@ long double scalblnl (long double x, long int exp);
	DefSCLFltFncAlias	scalblnl scalbln

	@ Entry 98
	@ Multiply floating-point number by integral power of radix
	@ float scalblnf (float x, long int exp);
scalblnf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 99
	@ Cube root function
	@ double cbrt (double x);
	DefSCLFltFnc	cbrt
	@ long double cbrtl (long double x);
	DefSCLFltFncAlias	cbrtl cbrt

	@ Entry 100
	@ Cube root function
	@ float cbrtf (float x);
cbrtf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 101
	@ Power function
	@ float powf (float x, float y);
	@ Implemented in scl_powf.s
	MOV	PC, #0

	@ Entry 102
	@ Square root function
	@ float sqrtf (float x);
sqrtf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 103
	@ Error function
	@ double erf (double x);
	DefSCLFltFnc	erf
	@ long double erfl (long double x);
	DefSCLFltFncAlias	erfl erf

	@ Entry 104
	@ Error function
	@ float erff (float x);
erff_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 105
	@ Complementary error function
	@ double erfc (double x);
	DefSCLFltFnc	erfc
	@ long double erfcl (long double x);
	DefSCLFltFncAlias	erfcl erfc

	@ Entry 106
	@ Complementary error function
	@ float erfcf (float x);
erfcf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 107
	@ Log gamma function
	@ double lgamma (double x);
	DefSCLFltFnc	lgamma
	@ long double lgammal (long double x);
	DefSCLFltFncAlias	lgammal lgamma

	@ Entry 108
	@ Log gamma function
	@ float lgammaf (float x);
lgammaf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 109
	@ True gamma function
	@ double tgamma (double x);
	DefSCLFltFnc	tgamma
	@ long double tgammal (long double x);
	DefSCLFltFncAlias	tgammal tgamma

	@ Entry 110
	@ True gamma function
	@ float tgammaf (float x);
tgammaf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 111
	@ Floating-point number manipulation
	@ double nexttoward (double x, long double y);
	DefSCLFltFnc	nexttoward
	@ long double nexttowardl (long double x, long double y);
	DefSCLFltFncAlias	nexttowardl nexttoward

	@ Entry 112
	@ Floating-point number manipulation
	@ float nexttowardf (float x, long double y);
nexttowardf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 113
	@ Floating-point multiply and add
	@ float fmaf (float x, float y, float z);
fmaf_FIXME_float_arg:
	MOV	PC, #0

	@ Entry 114
	DefSCLFnc	isblank

	@ Entry 115
	MOV	PC, #0	@ FIXME DefSCLFltFnc	strtof

	@ Entry 116
	DefSCLFnc	copysign

	@ Entry 117
	MOV	PC, #0	@ FIXME DefSCLFltFnc	copysignf

	@ Entry 118
	@ Floating point multiply and add
	@ double fma (double x, double y, double z);
	DefSCLFltFnc	fma

	@ Entry 119
	@ Remainder and part of quotient
	@ double remquo (double x, double y, int *quo);
	DefSCLFltFnc	remquo

	@ Entry 120
	@ Remainder and part of quotient
	@ float remquo (float x, float y, int *quo);
	MOV	PC, #0	@ FIXME DefSCLFltFnc	remquof

	@ Entry 121
	DefSCLFltFnc	llrint

	@ Entry 122
	MOV	PC, #0	@ FIXME DefSCLFltFnc	llrintf

	@ Entry 123
	DefSCLFnc	llround

	@ Entry 124
	MOV	PC, #0	@ FIXME DefSCLFltFnc	llroundf

	@ Entry 125
	DefSCLFltFncS	_cxd_mul

	@ Entry 126
	DefSCLFltFncS	_cxf_mul

	@ Entry
	DefSCLFltFncS	_cxd_div

	@ Entry
	DefSCLFltFncS	_cxf_div

	@ Entry
	DefSCLFltFncS	_cxd_rdv

	@ Entry
	DefSCLFltFncS	_cxf_rdv

	@ Entry
	DefSCLFltFnc	cacos

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cacosf

	@ Entry
	DefSCLFnc	casin

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	casinf

	@ Entry
	DefSCLFnc	catan

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	catanf

	@ Entry
	DefSCLFnc	ccos

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	ccosf

	@ Entry
	DefSCLFnc	csin

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	csinf

	@ Entry
	DefSCLFnc	ctan

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	ctanf

	@ Entry
	DefSCLFnc	cacosh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cacoshf

	@ Entry
	DefSCLFltFnc	casinh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	casinhf

	@ Entry
	DefSCLFnc	catanh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	catanhf

	@ Entry
	DefSCLFnc	ccosh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	ccoshf

	@ Entry
	DefSCLFnc	csinh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	csinhf

	@ Entry
	DefSCLFnc	ctanh

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	ctanhf

	@ Entry
	DefSCLFnc	cexp

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cexpf

	@ Entry
	DefSCLFnc	clog

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	clogf

	@ Entry
	DefSCLFnc	cabs

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cabsf

	@ Entry
	DefSCLFnc	cpow

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cpowf

	@ Entry
	DefSCLFnc	csqrt

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	csqrtf

	@ Entry
	DefSCLFnc	carg

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cargf

	@ Entry
	DefSCLFnc	cimag

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cimagf

	@ Entry
	DefSCLFnc	conj

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	conjf

	@ Entry
	DefSCLFnc	cproj

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	cprojf

	@ Entry
	DefSCLFnc	creal

	@ Entry
	MOV	PC, #0	@ FIXME DefSCLFltFnc	crealf

	@ Entry
	@ int fgetpos (FILE *stream, fpos_t *pos);
	@ with #define _FILE_OFFSET_BITS 64
	@ and #define _LARGEFILE_SOURCE
	@
	@ int fgetpos64 (FILE *stream, fpos64_t *pos);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	fgetpos64

	@ Entry
	@ FILE *fopen (const char *filename, const char *modes);
	@ with #define _FILE_OFFSET_BITS 64
	@
	@ FILE *fopen64 (const char *filename, const char *modes);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	fopen64

	@ Entry
	@ FILE *freopen (const char *filename, const char *modes, FILE *stream);
	@ with #define _FILE_OFFSET_BITS 64
	@
	@ FILE *freopen64 (const char *filename, const char *modes, FILE *stream);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	freopen64

	@ Entry
	@ int fseeko (FILE *stream, off_t offset, int whence);
	@ with #define _FILE_OFFSET_BITS 64
	@ and #define _LARGEFILE_SOURCE
	@
	@ int fseeko64 (FILE *stream, off64_t offset, int whence);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	fseeko64

	@ Entry
	@ int fsetpos (FILE *stream, fpos_t *pos);
	@ with #define _FILE_OFFSET_BITS 64
	@ and #define _LARGEFILE_SOURCE
	@
	@ int fsetpos64 (FILE *stream, fpos64_t *pos);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	fsetpos64

	@ Entry
	@ off_t ftello (FILE *stream);
	@ with #define _FILE_OFFSET_BITS 64
	@ and #define _LARGEFILE_SOURCE
	@
	@ off64_t ftello64 (FILE *stream);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	ftello64

	@ Entry
	@ FILE *tmpfile (void);
	@ with #define _FILE_OFFSET_BITS 64
	@
	@ FILE *tmpfile64 (void);
	@ with #define _LARGEFILE64_SOURCE
	DefSCLFnc	tmpfile64

extra_vectors_end:
	.space	extra_vectors_end - extra_vectors_begin
	.size	extra_vectors, . - extra_vectors

	.end
