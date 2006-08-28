/**
 * Fortify memory wrappers
 *
 * This file is used to redirect the memory allocation functions via the
 * Fortify memory check library.  The original symbols in UnixLib's alloc.o
 * are renamed to __unixlib_real_malloc, etc, and the calls in Fortify to
 * the functions redirected to the these new names.  This means that any
 * other references to the functions come here, to allow whatever processing
 * we choose.
 *
 * The symbol renaming is achieved via Drlink's link editing feature.
 *
 * Copyright (c) 2005, 2006 GCCSDK Developers
 */

#define __UNIXLIB_INTERNALS

#include <stdlib.h>
#include <stdio.h>
#include <swis.h>
#include <unixlib/unix.h>

#include "fortify.h"

#undef malloc
#undef realloc
#undef calloc
#undef strdup
#undef free

extern void *__unixlib_real_malloc(size_t size);

static int features;

/* Caller needs to take a copy of the return string.  */
static const char *
__get_backtrace (void)
{
  int *fp = __backtrace_getfp(), *oldfp = NULL;
  static char trace[1024];
  int tracelen = 1;
  char *ret;

  trace[0] = '\0';

  /* fp[0]  => pc
     fp[-1] => lr
     fp[-2] => sp
     fp[-3] => previous fp  */
  while (fp != NULL)
    {
      unsigned int *pc;

      /* Check that FP is different.  */
      if (fp == oldfp)
	break;

      /* Validate FP address.  */
      if (!__valid_address(fp - 3, fp))
	break;

      /* Retrieve PC counter.  */
      if (__32bit)
	pc = (unsigned int *)(fp[0] & 0xfffffffc);
      else
	pc = (unsigned int *)(fp[0] & 0x03fffffc);
      if (!(features & 0x8))
	pc += 1;

      if (!__valid_address(pc, pc))
	break;

      /* Retrieve function name.  */
      if (!__valid_address(pc - 7, pc))
	break;
      else
	{
	  int address;
	  const char *name = NULL;

	  for (address = -3; address > -8; address--)
	    {
	      if ((pc[address] & 0xffffff00) == 0xff000000)
		{
		  name = (const char *)(pc + address) - (pc[address] & 0xff);
		  break;
		}
	    }

	  /* Function name sanity check.  */
	  if (name != NULL
	      && (!__valid_address(name, (name + 256))
		  || strnlen(name, 256) == 256))
	    name = NULL;

	  if (!name)
	    {
	      tracelen += sizeof("?()/")-1;
	      if (tracelen > sizeof (trace))
	        break;
	      strcat(trace, "?()/");
	    }
	  else
	    {
	      tracelen += strlen (name) + sizeof("()/")-1;
	      if (tracelen > sizeof (trace))
	        break;
	      strcat(trace, name);
	      strcat(trace, "()/");
	    }
	}

      oldfp = fp;
      fp = (int *)fp[-3];
    }

  return trace;
}

static int count;
#undef Fortify_CheckAllMemory
#undef Fortify_OutputStatistics

void *malloc(size_t size)
{
  return Fortify_malloc(size, __get_backtrace (), 0);
}

void *realloc(void *ptr, size_t size)
{
  return Fortify_realloc(ptr, size, __get_backtrace (), 0);
}

void *calloc(size_t nmemb, size_t size)
{
  return Fortify_calloc(nmemb, size, __get_backtrace (), 0);
}

char *strdup (const char *string)
{
  return Fortify_strdup(string, __get_backtrace (), 0);
}

void free(void *ptr)
{
  Fortify_free(ptr, __get_backtrace (), 0);
}


static void memwrap_output(const char *message)
{
  int fatal = (strstr(message, "Invalid") != NULL) || (strstr(message, "Overwrite") != NULL);

  fputs(message, stderr);

  if (fatal)
    abort();
}


void __memwrap_init(void) __attribute__ ((constructor));
void __memwrap_final(void) __attribute__ ((destructor));

void __memwrap_init(void)
{
  fprintf(stderr, "*** Using Fortify memory checker ***\n");

  Fortify_SetOutputFunc(memwrap_output);

  if (_swix(OS_PlatformFeatures, _IN(0) | _OUT(0), 0, &features))
    features = 0;
}

void __memwrap_final(void)
{
  Fortify_OutputStatistics(NULL, 0);
}
