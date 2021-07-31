/* Copyright (c) 2005-2009 GCCSDK Developers
 *
 * Example module using GCC but *not* the SharedCLibrary.  It might be that
 * you have to provide your own C library routines like memset, memcpy,
 * memmove and memcmp.
 * You can use the StandardCLibrary *header* files to get useful standard C
 * types (or RISC OS specific types like _kernel_oserror) but that's as far
 * as you can get.
 */

#include "module.h"

static inline void
OS_Write0(const char *str)
{
  register unsigned int r0in __asm("r0") = (unsigned int)str;
  __asm__ __volatile__ ("SWI\t0x20002\n\t"
                        : : "r" (r0in) : "r14", "cc");
}

static inline void
OS_WriteC(char c)
{
  register unsigned int r0in __asm("r0") = c;
  __asm__ __volatile__ ("SWI\t0x20000\n\t"
                        : : "r" (r0in) : "r14", "cc");
}

static void
PrintDec(unsigned int i)
{
  char buf[16];
  char *s = &buf[0];

  if (i == 0)
    OS_WriteC('0');
  else
    {
      while (i)
        {
          *s++ = (i % 10) + '0';
          i = i / 10;
        }
      do
        OS_WriteC(*--s);
      while (s != &buf[0]);
    }
}

static void
PrintHex(unsigned int i)
{
  OS_Write0("0x");
  for (int index = 0; index < 8; ++index)
    {
      unsigned int c = i >> 28;
      OS_WriteC((c >= 10) ? c - 10 + 'A' : c + '0');
      i = i << 4;
    }
}

_kernel_oserror *
gccmodule_init(const char *cmd_tail, int podule_base, void *pw)
{
  OS_Write0("GCC example module initialise");
  OS_Write0("\r\n  - cmd_tail: "); PrintHex((unsigned int)cmd_tail);
  OS_Write0("\r\n  - podule_base: "); PrintHex((unsigned int)podule_base);
  OS_Write0("\r\n  - pw: "); PrintHex((unsigned int)pw);
  OS_Write0("\r\n    workspace at: "); PrintHex(*(unsigned int *)pw);
  OS_Write0("\r\n");

  return (void *)0;
}


_kernel_oserror *
gccmodule_final(int fatal, int podule, void *pw)
{
  OS_Write0("GCC example module finalise");
  OS_Write0("\r\n  - fatal: "); PrintHex((unsigned int)fatal);
  OS_Write0("\r\n  - podule: "); PrintHex((unsigned int)podule);
  OS_Write0("\r\n  - pw: "); PrintHex((unsigned int)pw);
  OS_Write0("\r\n    workspace at: "); PrintHex(*(unsigned int *)pw);
  OS_Write0("\r\n");

  return (void *)0;
}

