@ Chunk 4 stub routines for linking with the SharedCLibrary
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

	.section .riscos.libscl.chunkstub.id04,"ax",%progbits
	@ Chunk 4 is optional
	.word	4                       @ C99 low level support functions
	MakePtr	c99low_vectors_begin
	MakePtr	c99low_vectors_end
	.word	0
	.word	0

	.text
c99low_vectors:
c99low_vectors_begin:

	@ A 'long long' can be effectively represented as a structure thus :
	@
	@ typedef __value_in_regs struct longlong_s {
	@   unsigned long lo;
	@   unsigned long hi;
	@   } longlong_t;
	@
	@ Values are passed to functions as if there were two registers
	@ containing the lo and hi values.
	@
	@ typedef struct slldiv_t {
	@   long long quot;
	@   long long rem;
	@   } ulldiv_t;
	@
	@ typedef struct slldiv_t {
	@   long long quot;
	@   long long rem;
	@   } slldiv_t;
	@
	@ Many of the functions are suffixed with characters indicating the
	@ argument types :
	@
	@  l - indicates a 64bit argument
	@  s - indicates a 32bit signed argument
	@  u - indicates a 32bit unsigned argument

	@ Entry 0
	@ void *__rt_allocauto (int size);
	@ Allocates a block of memory of the size specified for a Variable
	@ Length Array. The current implementation will not be automatically
	@ freed on return from the calling function. Clients using longjmp
	@ should take care to retain their own lists of allocated storage as
	@ it is not guaranteed to be tracked by the implementation. Returns
	@ a pointer to the allocated space.
	.global	__rt_allocauto
__rt_allocauto:
	MOV	PC, #0

	@ Entry 1
	@ void __rt_freeauto (void *);
	@ Frees a block of memory allocated by the __rt_allocauto function.
	@ This call will be automatically added by the compiler as
	@ automatically allocated storage goes out of scope.
	.global	__rt_freeauto
__rt_freeauto:
	MOV	PC, #0

	@ Entry 2
	@ long long _ll_from_u(unsigned long x);
	@ Converts from 32bit values to 64bit values.
	.global	_ll_from_u
_ll_from_u:
	MOV	PC, #0

	@ Entry 3
	@ long long _ll_from_l(long x);
	@ Converts from 32bit values to 64bit values.
	.global	_ll_from_l
_ll_from_l:
	MOV	PC, #0

	@ Entry 4
	@ long _ll_to_l(long long x);
	@ Converts from a 64bit value to a 32bit value.
	.global	_ll_to_l
_ll_to_l:
	MOV	PC, #0

	@ Entry 5
	@ long long _ll_add(long long x, long long y);
	@ Adds the two arguments together and returns the result as a 64bit
	@ value.
	.global	_ll_add
_ll_add:
	MOV	PC, #0

	@ Entry 6
	@ long long _ll_addlu(long long x, unsigned long y);
	@ Adds the two arguments together and returns the result as a 64bit
	@ value.
	.global	_ll_addlu
_ll_addlu:
	MOV	PC, #0

	@ Entry 7
	@ long long _ll_addls(long long x, signed long y);
	@ Adds the two arguments together and returns the result as a 64bit
	@ value.
	.global	_ll_addls
_ll_addls:
	MOV	PC, #0

	@ Entry 8
	@ long long _ll_adduu(unsigned long x, unsigned long y);
	@ Adds the two arguments together and returns the result as a 64bit
	@ value.
	.global	_ll_adduu
_ll_adduu:
	MOV	PC, #0

	@ Entry 9
	@ long long _ll_addss(signed long x, signed long y);
	@ Adds the two arguments together and returns the result as a 64bit
	@ value.
	.global	_ll_addss
_ll_addss:
	MOV	PC, #0

	@ Entry 10
	@ long long _ll_sub(long long x, long long y);
	@ Subtracts the second argument from the first and returns the result
	@ as a 64bit value.
	.global	_ll_sub
_ll_sub:
	MOV	PC, #0

	@ Entry 11
	@ long long _ll_sublu(long long x, unsigned long y);
	@ Subtracts the second argument from the first and returns the result
	@ as a 64bit value.
	.global	_ll_sublu
_ll_sublu:
	MOV	PC, #0

	@ Entry 12
	@ long long _ll_subls(long long x, signed long y);
	@ Subtracts the second argument from the first and returns the result
	@ as a 64bit value.
	.global	_ll_subls
_ll_subls:
	MOV	PC, #0

	@ Entry 13
	@ long long _ll_subuu(unsigned long x, unsigned long y);
	@ Subtracts the second argument from the first and returns the result
	@ as a 64bit value.
	.global	_ll_subuu
_ll_subuu:
	MOV	PC, #0

	@ Entry 14
	@ long long _ll_subss(signed long x, signed long y);
	@ Subtracts the second argument from the first and returns the result
	@ as a 64bit value.
	.global	_ll_subss
_ll_subss:
	MOV	PC, #0

	@ Entry 15
	@ long long _ll_rsb(long long x, long long y);
	@ Subtracts the first argument from the second and returns the result
	@ as a 64bit value.
	.global	_ll_rsb
_ll_rsb:
	MOV	PC, #0

	@ Entry 16
	@ long long _ll_rsblu(long long x, unsigned long y);
	@ Subtracts the first argument from the second and returns the result
	@ as a 64bit value.
	.global	_ll_rsblu
_ll_rsblu:
	MOV	PC, #0

	@ Entry 17
	@ long long _ll_rsbls(long long x, signed long y);
	@ Subtracts the first argument from the second and returns the result
	@ as a 64bit value.
	.global	_ll_rsbls
_ll_rsbls:
	MOV	PC, #0

	@ Entry 18
	@ long long _ll_rsbuu(unsigned long x, unsigned long y);
	@ Subtracts the first argument from the second and returns the result
	@ as a 64bit value.
	.global	_ll_rsbuu
_ll_rsbuu:
	MOV	PC, #0

	@ Entry 19
	@ long long _ll_rsbss(signed long x, signed long y);
	@ Subtracts the first argument from the second and returns the result
	@ as a 64bit value.
	.global	_ll_rsbss
_ll_rsbss:
	MOV	PC, #0

	@ Entry 20
	@ long long _ll_mul(long long x, long long y);
	@ Multiplies the arguments and returns the result as a 64bit value.
	.global	_ll_mul
_ll_mul:
	MOV	PC, #0

	@ Entry 21
	@ long long _ll_mullu(long long x, unsigned long y);
	@ Multiplies the arguments and returns the result as a 64bit value.
	.global	_ll_mullu
_ll_mullu:
	MOV	PC, #0

	@ Entry 22
	@ long long _ll_mulls(long long x, signed long y);
	@ Multiplies the arguments and returns the result as a 64bit value.
	.global	_ll_mulls
_ll_mulls:
	MOV	PC, #0

	@ Entry 23
	@ long long _ll_muluu(unsigned long x, unsigned long y);
	@ Multiplies the arguments and returns the result as a 64bit value.
	.global	_ll_muluu
_ll_muluu:
	MOV	PC, #0

	@ Entry 24
	@ long long _ll_mulss(signed long x, signed long y);
	@ Multiplies the arguments and returns the result as a 64bit value.
	.global	_ll_mulss
_ll_mulss:
	MOV	PC, #0

	@ Entry 25
	@ __value_in_regs ulldiv_t _ll_udiv(unsigned long long x, unsigned long long y);
	@ Divides a unsigned 64bit value by another unsigned 64bit value and
	@ returns the result as an unsigned 64bit value. The result is
	@ returned in R0-R3.
	@ {quotient, remainder} = _ll_udiv(x,y) = x/y
	.global	_ll_udiv
_ll_udiv:
	MOV	PC, #0

	@ Entry 26
	@ __value_in_regs ulldiv_t _ll_urdv(unsigned long long x, unsigned long long y);
	@ Divides a unsigned 64bit value by another unsigned 64bit value and
	@ returns the result as an unsigned 64bit value. The result is
	@ returned in R0-R3.
	@ {quotient, remainder} = _ll_urdv(x,y) = y/x
	.global	_ll_urdv
_ll_urdv:
	MOV	PC, #0

	@ Entry 27
	@ unsigned long long _ll_udiv10(unsigned long long x);
	@ Divides an unsigned 64bit value by 10 and returns the result as a
	@ 64bit value.
	.global	_ll_udiv10
_ll_udiv10:
	MOV	PC, #0

	@ Entry 28
	@ __value_in_regs slldiv_t _ll_sdiv(signed long long x, signed long long y);
	@ Divides a signed 64bit value by another signed 64bit value and
	@ returns the result as a signed 64bit value. The result is returned
	@ in r0-r3.
	@ {quotient, remainder} = _ll_sdiv(x,y) = x/y
	.global	_ll_sdiv
_ll_sdiv:
	MOV	PC, #0

	@ Entry 29
	@ __value_in_regs slldiv_t _ll_srdv(signed long long x, signed long long y);
	@ Divides a signed 64bit value by another signed 64bit value and
	@ returns the result as a signed 64bit value. The result is returned
	@ in r0-r3.
	@ {quotient, remainder} = _ll_srdv(x,y) = y/x
	.global	_ll_srdv
_ll_srdv:
	MOV	PC, #0

	@ Entry 30
	@ signed long long _ll_sdiv10(signed long long x);
	@ Divides a signed 64bit value by 10 and returns the result as signed a 64bit value.
	.global	_ll_sdiv10
_ll_sdiv10:
	MOV	PC, #0

	@ Entry 31
	@ long long _ll_not(long long x);
	@ Returns the binary inverse of the argument.
	.global	_ll_not
_ll_not:
	MOV	PC, #0

	@ Entry 32
	@ long long _ll_neg(long long x);
	@ Returns 0 - x.
	.global	_ll_neg
_ll_neg:
	MOV	PC, #0

	@ Entry 33
	@ long long _ll_and(long long x, long long y);
	@ Returns the binary AND of the two arguments.
	.global	_ll_and
_ll_and:
	MOV	PC, #0

	@ Entry 34
	@ long long _ll_or(long long x, long long y);
	@ Returns the binary OR of the two arguments.
	.global	_ll_or
_ll_or:
	MOV	PC, #0

	@ Entry 35
	@ long long _ll_eor(long long x, long long y);
	@ Returns the binary EOR of the two arguments.
	.global	_ll_eor
_ll_eor:
	MOV	PC, #0

	@ Entry 36
	@ long long _ll_shift_l(long long x, int n);
	@ Returns a 64bit value shifted left by n bits. Negative values are
	@ not supported.
	.global	_ll_shift_l
_ll_shift_l:
	MOV	PC, #0

	@ Entry 37
	@ long long _ll_ushift_r(long long x, int n);
	@ Returns a 64bit value shifted right by n bits. Negative values are
	@ not supported.
	.global	_ll_ushift_r
_ll_ushift_r:
	MOV	PC, #0

	@ Entry 38
	@ long long _ll_sshift_r(long long x, int n);
	@ Returns a 64bit value arithmetically shifted right by n bits.
	@ Negative values are not supported.
	.global	_ll_sshift_r
_ll_sshift_r:
	MOV	PC, #0

	@ Entry 39
	@ _ll_cmpu
	@ Compares two 64bit values in r0,r1 and r2,r3. The comparison sets
	@ the processor flags in the same manner as 'CMP'.
	@ Note: Does not follow APCS conventions.
	.global	_ll_cmpu
_ll_cmpu:
	MOV	PC, #0

	@ Entry 40
	@ _ll_cmpge
	@ Compares two 64bit values in r0,r1 and r2,r3. The result is the same
	@ as 'SUBS first, first, second'. r0,r1 are corrupted by the
	@ subtraction.
	@ Note: Does not follow APCS conventions.
	.global	_ll_cmpge
_ll_cmpge:
	MOV	PC, #0

	@ Entry 41
	@ long long _ll_cmple
	@ Compares two 64bit values in r0,r1 and r2,r3. The result is the same
	@ as 'RSBS first, first, second'. r0,r1 are corrupted by the
	@ subtraction.
	@ Note: Does not follow APCS conventions.
	.global	_ll_cmple
_ll_cmple:
	MOV	PC, #0

	@ Entry 42
	@ double _ll_uto_d(unsigned long long x);
	@ Converts unsigned 64bit values to double values.
	.global	_ll_uto_d
_ll_uto_d:
	MOV	PC, #0

	@ Entry 43
	@ double _ll_sto_d(signed long long x);
	@ Converts signed 64bit values to float values.
	.global	_ll_sto_d
_ll_sto_d:
	MOV	PC, #0

	@ Entry 44
	@ float _ll_uto_f(unsigned long long x);
	@ Converts unsigned 64bit values to double values.
	.global	_ll_uto_f
_ll_uto_f:
	MOV	PC, #0

	@ Entry 45
	@ float _ll_sto_f(signed long long x);
	@ Converts signed 64bit values to float values.
	.global	_ll_sto_f
_ll_sto_f:
	MOV	PC, #0

	@ Entry 46
	@ unsigned long long _ll_ufrom_d(double x);
	@ Converts from a double to an unsigned 64bit value.
	.global	_ll_ufrom_d
_ll_ufrom_d:
	MOV	PC, #0

	@ Entry 47
	@ signed long long _ll_sfrom_d(double x);
	@ Converts from a double to an unsigned 64bit value.
	.global	_ll_sfrom_d
_ll_sfrom_d:
	MOV	PC, #0

	@ Entry 48
	@ unsigned long long _ll_ufrom_f(float x);
	@ Converts from a double to a signed 64bit value.
	.global	_ll_ufrom_f
_ll_ufrom_f:
	MOV	PC, #0

	@ Entry 49
	@ signed long long _ll_sfrom_f(float x);
	@ Converts from a double to a signed 64bit value.
	.global	_ll_sfrom_f
_ll_sfrom_f:
	MOV	PC, #0

c99low_vectors_end:
	.space	c99low_vectors_end - c99low_vectors_begin
	.size	c99low_vectors, . - c99low_vectors

	.end

