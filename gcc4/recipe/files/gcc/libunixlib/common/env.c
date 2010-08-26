/* Common environment related routines.
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <kernel.h>
#include <string.h>
#include <swis.h>

#include <internal/local.h>
#include <internal/os.h>

static inline size_t
get_var_len (const char *name)
{
  register const char *r0in __asm ("r0") = name;
  register size_t len __asm ("r2");
  /* When the system variable is already defined, the following XOS_ReadVarVal
     will return a buffer overflow error but R2 on exit will contain
     the length of its value. So basically we'll ignore the RISC OS error.  */
  __asm__ volatile ("MOV\tr1, #0\n\t"
		    "MOV\tr2, #1<<31\n\t"
		    "MOV\tr3, #0\n\t"
		    "MOV\tr4, #0\n\t"
		    "SWI\t%[SWI_XOS_ReadVarVal]\n\t"
		    : "=r" (len)
		    : "r" (r0in),
		      [SWI_XOS_ReadVarVal] "i" (OS_ReadVarVal | (1<<17))
		    : "r1", "r3", "r4", "r14", "cc");
  return len;
}

static inline const _kernel_oserror *
set_var (const char *name, const char *value, size_t value_len)
{
  register const char *r0in __asm ("r0") = name;
  register const char *r1in __asm ("r1") = value;
  register size_t r2in __asm ("r2") = value_len;
  register _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("MOV\tr3, #0\n\t"
		    "MOV\tr4, #4\n\t"
		    "SWI\t%[SWI_XOS_SetVarVal]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
		    : "r" (r0in), "r" (r1in), "r" (r2in),
		      [SWI_XOS_SetVarVal] "i" (OS_SetVarVal | (1<<17))
		    : "r3", "r4", "r14", "cc");
  return err;
}

/* Put value into the OS environment. Will not overwrite an existing
   variable of the same name if replace is 0.  */
int
__addenv_to_os (const char *name, const char *value, int replace)
{
  if (!replace)
    {
      /* Check whether the variable already exists.  */
      if (get_var_len (name))
	return 0;
    }

  const _kernel_oserror *err;
  if ((err = set_var (name, value, strlen (value))) != NULL)
    return __ul_seterr (err, EOPSYS);

  return 0;
}


/* Remove NAME from the OS environment (always succeeds and can be used for
   tail calls from unsetenv()).  */
int
__remenv_from_os (const char *name)
{
  register const char *r0in __asm ("r0") = name;
  __asm__ volatile ("MOV\tr1, #0\n\t"
		    "MOV\tr2, #-1\n\t"
		    "MOV\tr3, #0\n\t"
		    "MOV\tr4, #0\n\t"
		    "SWI\t%[SWI_XOS_SetVarVal]\n\t"
		    :
		    : "r" (r0in),
		      [SWI_XOS_SetVarVal] "i" (OS_SetVarVal | (1<<17))
		    : "r1", "r2", "r3", "r4", "r14", "cc");

  return 0;
}

