/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/coredump.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: coredump.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
#endif

/* signal.c.coredump. Write out core dump/termination diagnostic
   information for a process. */

/* This source file should be compiled with stack checking turned off.
   Ideally they should be re-written in assembler.  */

#include <unixlib/sigstate.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/swis.h>
#include <sys/os.h>
#include <unixlib/local.h>

#pragma no_check_stack

static char *__rname[16] =
{
  "a1", "a2", "a3", "a4",
  "v1", "v2", "v3", "v4", "v5", "v6",
  "sl", "fp", "ip", "sp", "lr", "pc"
};

struct trace
  {
    unsigned int *fp;
    unsigned int *sp;
    unsigned int *lr;
    unsigned int *pc;
  };

void
__backtrace (register const unsigned int *fp)
{
  const struct trace *f;
  unsigned int *pc;
  int i;
  char *n;
  int features;
  int regs[10];
  struct env *environment;
  const unsigned int *old_fp = 0;

  /* Remove environment handlers.  */
  environment = __calling_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    if (i != 11)		/* Do not remove exit handler. */
      __write_environment_handler (i, environment->handlers + i);

  os_print ("\rstack backtrace:\r\n\n");

  regs[0] = 0;
  /* If SWI returns an error then assume unknown and thus not StrongARM.  */
  if (os_swi (OS_PlatformFeatures, regs))
    features = 0;
  else
    features = regs[0];


  while (fp)
    {
      if (fp == old_fp)
	{
	  os_print ("fp unchanged at 0x");
	  os_prhex ((int) fp);
	  /*printf( "fp=%p, oldfp=%p\n", fp, old_fp); */
	  break;
	}
      old_fp = fp;

      f = (const struct trace *) (fp - 3);
      if (!__address_valid ((void *)(fp - 3), (void *)fp))
	{
	  os_print ("Stack frame has gone out of bounds with address ");
	  os_prhex ((int) (fp - 3));
	  os_print ("\r\n");
	  break;
	}
      pc = (unsigned int *) ((unsigned int) f->pc & 0x03fffffcU);
      /* Bit 3 of features set => stored PC is PC+8 rather than PC+12. */
      if (features & 0x08)
	pc++;

      if ((unsigned int) (f->fp) & 0x80000000U)
	{
	  os_print ("\r\n\n pc: ");
	  os_prhex ((int) pc);
	  os_print (" sp: ");
	  os_prhex ((int) f->sp);
	  n = "?";		/* SJC */

	  if (!__address_valid (pc, pc))
	    {
	      os_print ("\r\nInvalid pc address ");
	      os_prhex ((int) pc);
	      os_nl ();
	    }
	  else
	    {
	      if (__address_valid (pc - 7, pc - 3))
		{
		  for (i = -3; i > -7; i--)
		    if ((pc[i] & 0xffffff00U) == 0xff000000U)
		      {
			n = (char *) (pc + i) - (pc[i] & 0xffU);
			break;
		      }
		  os_print (" ");
		  os_print (n);
		  os_print ("()\r\n");
		}
	      else
		{
		  os_print ("[invalid address]\r\n");
		}
            }
	  fp++;
	  os_print ("\r\nRegister dump:\r\n");
	  for (i = 0; i < 16; i++)
	    {
	      if (!(i & 3))
	        os_print ("\r\n ");
	      os_print (__rname[i]);
	      os_print (": ");
	      if (__address_valid ((void *)(fp + i), (void *)(fp + i)))
	        os_prhex (fp[i]);
	      else
	        os_print ("bad addr");
	      os_print (" ");
	    }
	  os_print ("\r\n\n\n");
	  fp = (unsigned int *) ((unsigned int) (f->fp) & ~0x80000000U);
	}
      else
	{
	  if (!__address_valid (pc, pc))
	    {
	      os_print ("Invalid pc address ");
	      os_prhex ((int) pc);
	      os_nl ();
	    }
	  else
	    {
	      n = "?";

	      os_print (" pc: ");
	      os_prhex ((int) pc);
	      os_print (" sp: ");
	      os_prhex ((int) f->sp);
	      os_print (" ");

	      if (__address_valid (pc - 7, pc - 3))
		{
		  for (i = -3; i > -7; i--)
		    if ((pc[i] & 0xffffff00U) == 0xff000000U)
		      {
			n = (char *) (pc + i) - (pc[i] & 0xffU);
			break;
		      }
		  os_print (n);
		  os_print ("()\r\n");
		}
	      else
		{
		  os_print ("[invalid address]\r\n");
		}
	    }
	  fp = f->fp;
	}
    }
  os_nl ();

  /* Reinstall environment handlers.  */
  environment = __c_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    if (i != 11)		/* Exit handler was not removed, so do not restore. */
      __write_environment_handler (i, environment->handlers + i);
}

int
__write_corefile (int signo)
{
  os_print ("\n\rFatal signal received: ");
  os_print (sys_siglist[signo]);
  os_print ("\n\rA core dump will now follow ...\n\r");

  /* Do a core-dump.  */
  __core ();
  return 1;
}

void
__write_termination (int signo)
{
  os_print ("\n\rTermination signal received: ");
  os_print (sys_siglist[signo]);
  os_nl ();
}
