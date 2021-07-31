/* Test program for UnixLib handler implementation.  */

#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <swis.h>

#define UNMAPPED_MEM_ADDRESS 0x00129990
#define ILL_MEM_ADDRESS 0xFF000000 /* Only for 26-bit only processors.  */

int test_case;

void
sub3()
{
  printf ("Test case %d\n", test_case);
  switch (test_case)
    {
      case 0:
	{
	  printf ("About to call ARM reset exception type\n");
	  void (*addr)() = (void(*)())0x0;
	  addr ();
	  break;
	}

      case 1:
	{
	  printf ("About to call an undefined instruction (SIGILL)\n");
	  __asm__ volatile (".word\t0xe7ffffff\n\t");
	  break;
	}

      case 2:
	{
	  printf ("About to call unimplemented SWI instruction\n");
	  __asm__ volatile ("swi\t0x00f00123\n\t");
	  break;
	}

      case 3:
	{
	  printf ("About to generate prefetch abort (SIGSEGV, case 1)\n");
	  if (1)
	    {
	      void (*addr)() = (void(*)()) UNMAPPED_MEM_ADDRESS;
	      addr ();
	    }
	  else
	    __asm__ volatile ("bkpt\t0x1234\n\t");
	  break;
	}

      case 4:
	{
	  printf ("About to generate data abort (SIGSEGV, case 2)\n");
	  int *addr = (int *) UNMAPPED_MEM_ADDRESS;
	  *addr = 0;
	  break;
	}

      case 5:
	{
	  /* Assuming this runs on pre-ARMv3 or ARMv3/v4 in 26-bit configuration mode.  */
	  printf ("About to generate address exception (SIGBUS)\n");
	  int *addr = (int *) ILL_MEM_ADDRESS;
	  int val = *addr;
	  break;
	}

      case 6:
        {
	  printf ("About to call RISC OS error handler\n");
	  const _kernel_oserror err = { 0x80000000, "Own made up RISC OS error" };
	  register const _kernel_oserror *r0 __asm ("r0") = &err;
	  __asm__ volatile ("swi\t%[SWI_OS_GenerateError]\n\t"
			    : /* no output */
			    : "r" (r0), [SWI_OS_GenerateError] "i" (OS_GenerateError)
			    : "cc");
	  break;
        }

      case 7:
	{
	  printf ("About to call RISC OS exit handler\n");
	  __asm__ volatile ("swi\t%[SWI_OS_Exit]\n\t"
			    : /* no output */
			    : [SWI_OS_Exit] "i" (OS_Exit)
			    : "cc");
	  break;
	}
    }

  printf ("XXX we shouldn't have come here\n");
}

void
sub2()
{
  sub3();
}

void
sub1()
{
  sub2();
}

int
main(int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf (stderr, "Syntax: %s <test number 0 - 7>\n", argv[0]);
      return EXIT_FAILURE;
    }

  test_case = atoi (argv[1]);
  if (test_case < 0 || test_case > 7)
    {
      fprintf (stderr, "Test number must be 0 .. 7\n");
      return EXIT_FAILURE;
    }

  sub1();

  return EXIT_SUCCESS;
}

