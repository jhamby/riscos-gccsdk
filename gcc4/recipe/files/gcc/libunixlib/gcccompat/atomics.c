/* Implementation of atomic related functions for GCC 4/LLVM
 * Copyright (c) 2015 UnixLib Developers
 * Written by Lee Noar <leenoar@sky.com>
 *
 * This code is based on the descriptions given at:
 * 
 * http://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 * 
 * and
 * 
 * http://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html
 * 
 * The memory model argument is ignored.
 * 
 */

#include <pthread.h>
#include <stdint.h>
#include <sys/cdefs.h>

/* These built-in functions perform the operation suggested by the name,
 * and return the value that had previously been in *ptr. That is,
 *
 *  {
 *    tmp = *ptr;
 *    *ptr op= val;
 *    return tmp;
 *  }
 */
#define ATOMIC_FETCH_OP(name, op, size, type) \
type \
__atomic_fetch_##name##_##size (type *ptr, type val, int memmodel) \
{ \
  type temp; \
  \
  __pthread_disable_ints (); \
  \
  temp = *ptr; \
  op; \
  \
  __pthread_enable_ints (); \
  \
  return temp; \
}


ATOMIC_FETCH_OP(add,(*ptr) += val,1,char)
ATOMIC_FETCH_OP(sub,(*ptr) -= val,1,char)
ATOMIC_FETCH_OP(or,(*ptr) |= val,1,char)
ATOMIC_FETCH_OP(xor,(*ptr) ^= val,1,char)
ATOMIC_FETCH_OP(and,(*ptr) &= val,1,char)
ATOMIC_FETCH_OP(nand,(*ptr) = ~((*ptr) & val),1,char)

ATOMIC_FETCH_OP(add,(*ptr) += val,2,short)
ATOMIC_FETCH_OP(sub,(*ptr) -= val,2,short)
ATOMIC_FETCH_OP(or,(*ptr) |= val,2,short)
ATOMIC_FETCH_OP(xor,(*ptr) ^= val,2,short)
ATOMIC_FETCH_OP(and,(*ptr) &= val,2,short)
ATOMIC_FETCH_OP(nand,(*ptr) = ~((*ptr) & val),2,short)

ATOMIC_FETCH_OP(add,(*ptr) += val,4,uint32_t)
ATOMIC_FETCH_OP(sub,(*ptr) -= val,4,uint32_t)
ATOMIC_FETCH_OP(or,(*ptr) |= val,4,uint32_t)
ATOMIC_FETCH_OP(xor,(*ptr) ^= val,4,uint32_t)
ATOMIC_FETCH_OP(and,(*ptr) &= val,4,uint32_t)
ATOMIC_FETCH_OP(nand,(*ptr) = ~((*ptr) & val),4,uint32_t)

ATOMIC_FETCH_OP(add,(*ptr) += val,8,uint64_t)
ATOMIC_FETCH_OP(sub,(*ptr) -= val,8,uint64_t)
ATOMIC_FETCH_OP(or,(*ptr) |= val,8,uint64_t)
ATOMIC_FETCH_OP(xor,(*ptr) ^= val,8,uint64_t)
ATOMIC_FETCH_OP(and,(*ptr) &= val,8,uint64_t)
ATOMIC_FETCH_OP(nand,(*ptr) = ~((*ptr) & val),8,uint64_t)


/* This built-in function implements an atomic exchange operation.
 * It writes val into *ptr, and returns the previous contents of *ptr.  */
#define ATOMIC_EXCHANGE(size, type) \
type \
__atomic_exchange_##size (type *ptr, type val, int memmodel) \
{ \
  type temp; \
  \
  __pthread_disable_ints (); \
  \
  temp = *ptr; \
  *ptr = val; \
  \
  __pthread_enable_ints (); \
  \
  return temp; \
}

ATOMIC_EXCHANGE(1,uint8_t)
ATOMIC_EXCHANGE(2,short)
ATOMIC_EXCHANGE(4,uint32_t)

/* This built-in function implements an atomic compare and exchange operation.
 * This compares the contents of *ptr with the contents of *expected and if equal,
 * writes desired into *ptr. If they are not equal, the current contents of *ptr
 * is written into *expected. weak is true for weak compare_exchange, and false
 * for the strong variation. Many targets only offer the strong variation and ignore
 * the parameter. When in doubt, use the strong variation.
 *
 * True is returned if desired is written into *ptr and the execution is considered
 * to conform to the memory model specified by success_memmodel. There are no restrictions
 * on what memory model can be used here.
 *
 * False is returned otherwise, and the execution is considered to conform to failure_memmodel.
 * This memory model cannot be __ATOMIC_RELEASE nor __ATOMIC_ACQ_REL. It also cannot be a
 * stronger model than that specified by success_memmodel.
 */

#define ATOMIC_COMPARE_AND_EXCHANGE(size, type) \
char \
__atomic_compare_exchange_##size (type *ptr, \
				  type *expected, \
				  type desired, \
				  char weak, \
				  int success_memmodel, \
				  int failure_memmodel) \
{ \
  uint32_t result; \
  \
  __pthread_disable_ints (); \
  \
  if (*ptr == *expected) \
  { \
    *ptr = desired; \
    result = 1; \
  } \
  else \
  { \
    *expected = *ptr; \
    result = 0; \
  } \
  \
  __pthread_enable_ints (); \
  \
  return result; \
}

ATOMIC_COMPARE_AND_EXCHANGE(1,uint8_t)
ATOMIC_COMPARE_AND_EXCHANGE(2,short)
ATOMIC_COMPARE_AND_EXCHANGE(4,uint32_t)

#ifndef __ARM_EABI__

/* These built-in functions perform the operation suggested by the name,
 * and return the value that had previously been in *ptr. That is,
 *
 *  {
 *    tmp = *ptr;
 *    *ptr op= val;
 *    return tmp;
 *  }
 */
#define SYNC_FETCH_AND_OP(name, op, size, type) \
type \
__builtin_sync_fetch_and_##name##_##size (volatile type *ptr, type val, ...) \
{ \
  type temp; \
  \
  __pthread_disable_ints (); \
  \
  temp = *ptr; \
  (*ptr) op##= val; \
  \
  __pthread_enable_ints (); \
  \
  return temp; \
} \
__asm__(".hidden\t__builtin_sync_fetch_and_"__STRING(name)"_"__STRING(size)"\n"); \
__asm__(".global\t__sync_fetch_and_"__STRING(name)"_"__STRING(size)"\n"); \
__asm__("__sync_fetch_and_"__STRING(name)"_"__STRING(size)"=__builtin_sync_fetch_and_"__STRING(name)"_"__STRING(size));

SYNC_FETCH_AND_OP(add,+,1,char)
SYNC_FETCH_AND_OP(sub,-,1,char)
SYNC_FETCH_AND_OP(or,|,1,char)
SYNC_FETCH_AND_OP(xor,^,1,char)
SYNC_FETCH_AND_OP(and,&,1,char)

SYNC_FETCH_AND_OP(add,+,2,short)
SYNC_FETCH_AND_OP(sub,-,2,short)
SYNC_FETCH_AND_OP(or,|,2,short)
SYNC_FETCH_AND_OP(xor,^,2,short)
SYNC_FETCH_AND_OP(and,&,2,short)

SYNC_FETCH_AND_OP(add,+,4,int)
SYNC_FETCH_AND_OP(sub,-,4,int)
SYNC_FETCH_AND_OP(or,|,4,int)
SYNC_FETCH_AND_OP(xor,^,4,int)
SYNC_FETCH_AND_OP(and,&,4,int)

#endif

/* These built-in functions perform the operation suggested by the name,
 * and return the new value. That is,
 * 
 *  { *ptr op= value; return *ptr; }
 */
#define SYNC_OP_AND_FETCH(name, op, size, type) \
type \
__builtin_sync_##name##_and_fetch_##size (volatile type *ptr, type val, ...) \
{ \
  __pthread_disable_ints (); \
  \
  (*ptr) op##= val; \
  \
  __pthread_enable_ints (); \
  \
  return *ptr; \
} \
__asm__(".hidden\t__builtin_sync_"__STRING(name)"_and_fetch_"__STRING(size)"\n"); \
__asm__(".global\t__sync_"__STRING(name)"_and_fetch_"__STRING(size)"\n"); \
__asm__("__sync_"__STRING(name)"_and_fetch_"__STRING(size)"=__builtin_sync_"__STRING(name)"_and_fetch_"__STRING(size));

SYNC_OP_AND_FETCH(add,+,1,char)
SYNC_OP_AND_FETCH(sub,-,1,char)
SYNC_OP_AND_FETCH(or,|,1,char)
SYNC_OP_AND_FETCH(xor,^,1,char)
SYNC_OP_AND_FETCH(and,&,1,char)

SYNC_OP_AND_FETCH(add,+,2,short)
SYNC_OP_AND_FETCH(sub,-,2,short)
SYNC_OP_AND_FETCH(or,|,2,short)
SYNC_OP_AND_FETCH(xor,^,2,short)
SYNC_OP_AND_FETCH(and,&,2,short)

SYNC_OP_AND_FETCH(add,+,4,int)
SYNC_OP_AND_FETCH(sub,-,4,int)
SYNC_OP_AND_FETCH(or,|,4,int)
SYNC_OP_AND_FETCH(xor,^,4,int)
SYNC_OP_AND_FETCH(and,&,4,int)

/* These built-in functions perform an atomic compare and swap. That is,
 * if the current value of *ptr is oldval, then write newval into *ptr.
 * The “bool” version returns true if the comparison is successful and
 * newval is written. The “val” version returns the contents of *ptr
 * before the operation. 
 */
#define SYNC_BOOL_COMPARE_SWAP(size, type) \
_Bool \
__builtin_sync_bool_compare_and_swap_##size (volatile type *ptr, type oldval, type newval, ...) \
{ \
  uint32_t result = 0; \
  \
  __pthread_disable_ints (); \
  \
  if (*ptr == oldval) \
  { \
    *ptr = newval; \
    result = 1; \
  } \
  \
  __pthread_enable_ints (); \
  \
  return result; \
} \
__asm__(".hidden\t__builtin_sync_bool_compare_and_swap_"__STRING(size)"\n"); \
__asm__(".global\t__sync_bool_compare_and_swap_"__STRING(size)"\n"); \
__asm__("__sync_bool_compare_and_swap_"__STRING(size)"=__builtin_sync_bool_compare_and_swap_"__STRING(size));

SYNC_BOOL_COMPARE_SWAP(1,char)
SYNC_BOOL_COMPARE_SWAP(2,short)
SYNC_BOOL_COMPARE_SWAP(4,int)

#define SYNC_VAL_COMPARE_SWAP(size, type) \
type \
__builtin_sync_val_compare_and_swap_##size (volatile type *ptr, type oldval, type newval, ...) \
{ \
  type result; \
  \
  __pthread_disable_ints (); \
  \
  result = *ptr; \
  if (*ptr == oldval) \
    *ptr = newval; \
  \
  __pthread_enable_ints (); \
  \
  return result; \
} \
__asm__(".hidden\t__builtin_sync_val_compare_and_swap_"__STRING(size)"\n"); \
__asm__(".global\t__sync_val_compare_and_swap_"__STRING(size)"\n"); \
__asm__("__sync_val_compare_and_swap_"__STRING(size)"=__builtin_sync_val_compare_and_swap_"__STRING(size));
SYNC_VAL_COMPARE_SWAP(1,char)
SYNC_VAL_COMPARE_SWAP(2,short)
SYNC_VAL_COMPARE_SWAP(4,int)

/* This built-in function implements an atomic load operation.
 * It returns the contents of *ptr.
 */
#define ATOMIC_LOAD_N(size, type) \
type \
__atomic_load_##size (type *ptr, int memmodel) \
{ \
  type result; \
  \
  __pthread_disable_ints(); \
  \
  result = *ptr; \
  \
  __pthread_enable_ints(); \
  \
  return result; \
}

ATOMIC_LOAD_N(1,char)
ATOMIC_LOAD_N(2,short)
ATOMIC_LOAD_N(4,uint32_t)
ATOMIC_LOAD_N(8,uint64_t)

/* This built-in function implements an atomic store operation.
 * It writes val into *ptr.
 */
#define ATOMIC_STORE_N(size, type) \
void \
__atomic_store_##size (type *ptr, type val, int memmodel) \
{ \
  __pthread_disable_ints(); \
  \
  *ptr = val; \
  \
  __pthread_enable_ints(); \
}

ATOMIC_STORE_N(1,char)
ATOMIC_STORE_N(2,short)
ATOMIC_STORE_N(4,uint32_t)
ATOMIC_STORE_N(8,uint64_t)

/* These built-in functions perform the operation suggested by
 * the name, and return the result of the operation. That is,
 *        { *ptr op= val; return *ptr; }
 * 
 * FIXME: There's no indication of the size of the type for these
 * operations. It's possible that there is an implicit size argument
 * before the other arguments, but I haven't been able to test that.
 * For now, I've assumed a 32 bit size.
 */
#define ATOMIC_OP_FETCH(name, op, type) \
type \
__builtin_atomic_##name##_fetch(type *ptr, type val, int memmodel) \
{ \
  __pthread_disable_ints(); \
  \
  op; \
  \
  __pthread_enable_ints(); \
  \
  return *ptr; \
} \
__asm__(".hidden\t__builtin_atomic_"__STRING(name)"_fetch\n"); \
__asm__(".global\t__atomic_"__STRING(name)"_fetch\n"); \
__asm__("__atomic_"__STRING(name)"_fetch=__builtin_atomic_"__STRING(name)"_fetch");

ATOMIC_OP_FETCH(add,(*ptr) += val,unsigned)
ATOMIC_OP_FETCH(sub,(*ptr) -= val,unsigned)
ATOMIC_OP_FETCH(and,(*ptr) &= val,unsigned)
ATOMIC_OP_FETCH(xor,(*ptr) ^= val,unsigned)
ATOMIC_OP_FETCH(or,(*ptr) |= val,unsigned)
ATOMIC_OP_FETCH(nand,(*ptr) = ~((*ptr) & val),unsigned)

static int __cmpxchg(int oldval, int newval, volatile int *ptr)
{
  int result, scratch;

  __asm volatile ("1:	ldrex	%[scratch], [%[ptr]]\n"
		  "	subs	%[scratch], %[scratch], %[oldval]\n"
		  "	strexeq	%[scratch], %[newval], [%[ptr]]\n"
		  "	teqeq	%[scratch], #1\n"
		  "	beq	1b\n"
		  "	rsbs	%[result], %[scratch], #0\n"
		  : [result] "=r" (result), [scratch] "+r" (scratch), [newval] "+r" (newval)
		  : [oldval] "r" (oldval),
		    [ptr] "r" (ptr)
		  : "cc", "memory");
  return result;
}

int __builtin_sync_lock_test_and_set_4 (volatile int *ptr, int val)
{
  int failure, oldval;

  do {
    oldval = *ptr;
    failure = __cmpxchg (oldval, val, ptr);
  } while (failure != 0);

  return oldval;
}
__asm__(".hidden\t__builtin_sync_lock_test_and_set_4\n"); \
__asm__(".global\t__sync_lock_test_and_set_4\n"); \
__asm__("__sync_lock_test_and_set_4=__builtin_sync_lock_test_and_set_4");

void
__builtin_sync_synchronize (void)
{
}
__asm__(".hidden\t__builtin_sync_synchronize\n"); \
__asm__(".global\t__sync_synchronize\n"); \
__asm__("__sync_synchronize=__builtin_sync_synchronize");
