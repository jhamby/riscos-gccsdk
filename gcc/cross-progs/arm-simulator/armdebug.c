/* -> $.ARMemu.C.ARMdebug

   Debugger for the ARM2 emulator

   (C) Edwin Dorre 1990
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "armhdr.h"

FILE *ftrace = NULL;
int brk;
WORD breakpoint;

/*****************************************************************************
 showregs: show registers and state of processor
 *****************************************************************************/

void 
showregs ()
{
  window (0, 15);
  gotoxy (0, 0);

  printf ("Register dump is:\n\n");

  printf ("USR: R0  = %08lX R1  = %08lX R2  = %08lX R3  = %08lX\n",
	  rg[0], rg[1], rg[2], rg[3]);
  printf ("     R4  = %08lX R5  = %08lX R6  = %08lX R7  = %08lX\n",
	  rg[4], rg[5], rg[6], rg[7]);
  printf ("     R8  = %08lX R9  = %08lX R10 = %08lX R11 = %08lX\n",
	  rg[8], rg[9], rg[10], rg[11]);
  printf ("     R12 = %08lX R13 = %08lX R14 = %08lX R15 = %08lX PC  = %08lX\n\n",
	  rg[12], rg[13], rg[14], r15, r15 & 0x03fffffc);

  printf ("FIQ: R8  = %08lX R9  = %08lX R10 = %08lX R11 = %08lX\n",
	  r8fiq, r9fiq, r10fiq, r11fiq);
  printf ("     R12 = %08lX R13 = %08lX R14 = %08lX\n\n",
	  r12fiq, r13fiq, r14fiq);

  printf ("IRQ:                R13 = %08lX R14 = %08lX\n\n", r13irq, r14irq);
  printf ("SVC:                R13 = %08lX R14 = %08lX\n\n", r13svc, r14svc);

  printf ("Mode ");
  if ((r15 & 3) == USR_MODE)
    printf ("USR");
  if ((r15 & 3) == FIQ_MODE)
    printf ("FIQ");
  if ((r15 & 3) == IRQ_MODE)
    printf ("IRQ");
  if ((r15 & 3) == SVC_MODE)
    printf ("SVC");
  printf (" flags set: ");
  if (r15 & N_BIT)
    printf ("N");
  else
    printf ("n");
  if (r15 & Z_BIT)
    printf ("Z");
  else
    printf ("z");
  if (r15 & C_BIT)
    printf ("C");
  else
    printf ("c");
  if (r15 & V_BIT)
    printf ("V");
  else
    printf ("v");
  if (r15 & I_BIT)
    printf ("I");
  else
    printf ("i");
  if (r15 & F_BIT)
    printf ("F");
  else
    printf ("f");
  printf ("\n");
}

/*****************************************************************************
 showins: show instruction to be executed 
 *****************************************************************************/

void 
showins ()
{
  static int x = 0, y = 0;
  register WORD ins, pc, cond;

  window (16, 46);
  gotoxy (x, y);

  pc = r15 & 0x3fffffc;
  if (!PCwrite)
    pc -= 8;
  ins = wmem[pc >> 2];
  ins = SWAPWORD (ins);
  cond = (ins & 0xf0000000) >> 28;

  printf ("%08lX %08lX", pc, ins);
  if (!conditie[cond] ())
    printf (" Skip ");
  else
    printf ("      ");
  print_insn_little_arm ((char *) bmem, 0, pc, stdout);
  printf ("\n");

  x = xpos ();
  y = ypos ();
}

/*****************************************************************************
 execstep: the ARM executor, gets one instruction decodes and executes it!
 *****************************************************************************/


void 
execstep (void)
{
  register WORD ins, cond, pc, l;

  if (PCwrite)
    {
      PCwrite = FALSE;
      r15 = addPC (r15, 8);
    }

  pc = (r15 & 0x3fffffc) - 8;
  if (!brk && pc == breakpoint)
    brk = TRUE;
  else
    {
      ins = wmem[((r15 & 0x3fffffc) >> 2) - 2];
      ins = SWAPWORD (ins);
      cond = (ins & 0xf0000000) >> 28;

      if (ftrace)
	fprintf (ftrace, "%lX %lX", pc, ins);

      if (conditie[cond] ())
	{
	  main_decode[((ins & 0x0f000000) >> 22) |
		      ((ins & 0x00000080) >> 6) |
		      ((ins & 0x00000010) >> 4)] (ins);
	}

      if (ftrace)
	{
	  for (l = 0; l < 16; l++)
	    fprintf (ftrace, " %lX", r (l));
	  fprintf (ftrace, "\n");
	}

      if (PCwrite)
	{
	  PCwrite = FALSE;
	  r15 = addPC (r15, 8);
	}
      else
	r15 = addPC (r15, 4);
    }
}

void 
swapstep (int a)
{
  static int x = 0, y = 0;

  if (a == 0)
    {
      window (48, 57);
      gotoxy (x, y);
    }

  if (a == 1)
    {
      x = xpos ();
      y = ypos ();
    }
}

void 
doexecstep (void)
{
  swapstep (0);
  execstep ();
  swapstep (1);
}

/*****************************************************************************
 debugger: the ARM debugger
 *****************************************************************************/

void 
debugger ()
{
  int key = 0;
  char buf[20];

  mode (25);

  showregs ();
  showins ();

  while (TRUE)
    {
      brk = TRUE;

      window (59, 59);
      key = getkey ();
      if (key >= 'a' && key <= 'z')
	key -= 32;

      switch (key)
	{
	case 'C':
	  breakpoint = 0;
	  printf ("\nNo breakpoint");
	  break;

	case 'B':
	  printf ("\nEnter breakpoint address &");
	  scanf ("%lX", &breakpoint);
	  printf ("\nBreakpoint: &%08lX", breakpoint);
	  break;

	case 'S':
	  doexecstep ();
	  showregs ();
	  showins ();
	  break;

	case 'F':
	  doexecstep ();
	  showins ();
	  break;

	case 'G':
	  printf ("\nGO - Q to stop");
	  swapstep (0);
	  execstep ();
	  brk = FALSE;
	  do
	    {
	      key = inkey (0, 0);
	      if (key >= 'a' && key <= 'z')
		key -= 32;
	      if (key != 'Q')
		execstep ();
	    }
	  while (key != 'Q' && !brk);
	  swapstep (1);
	  window (59, 59);
	  if (brk)
	    printf ("\nBreak point!");
	  else
	    printf ("\nStopped");
	  showregs ();
	  showins ();
	  break;

	case 'E':
	  mode (0);
	  emulator ();
	  break;

	case 'T':
	  printf ("\nTRACE - Q to stop");
	  doexecstep ();
	  brk = FALSE;
	  do
	    {
	      key = inkey (0, 0);
	      if (key >= 'a' && key <= 'z')
		key -= 32;
	      if (key != 'Q')
		{
		  doexecstep ();
		  showins ();
		}
	    }
	  while (key != 'Q' && !brk);
	  window (59, 59);
	  if (brk)
	    printf ("\nBreak point!");
	  else
	    printf ("\nStopped");
	  showregs ();
	  break;

	case 'U':
	  showregs ();
	  break;

	case 'Q':
	  mode (0);
	  printf ("Quit\n\n");
	  exit (0);

	case 'O':
	  printf ("\nFTRACE - Filename: ");
	  scanf ("%s", buf);
	  ftrace = fopen (buf, "wt");
	  break;

	case 'P':
	  printf ("\nClose FTRACE");
	  if (ftrace)
	    fclose (ftrace);
	  break;

	default:
	  printf ("\nC-ClrBrkPnt B-SetBrkPnt S-Step F-FStep G-Go T-Trace U-UpdRegs Q-Quit E-Emulate");
	  break;
	}
    }
}
