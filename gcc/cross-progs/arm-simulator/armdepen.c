/* -> $.ARMemu.C.ARMdepen

   System dependent routines for the ARM2 emulator

   (C) Edwin Dorre 1990
 */

#include <stdio.h>
#include <string.h>

#include "armhdr.h"

#ifdef P_ARCHIMEDES
#include <kernel.h>
#include <swis.h>
#endif

/*****************************************************************************
 Several RISC-OS operating system routines
 *****************************************************************************/

int 
getkey ()
{
#ifdef P_ARCHIMEDES
  _kernel_swi_regs regs;
  _kernel_swi (OS_ReadC, &regs, &regs);
  return (regs.r[0]);
#else
#ifdef P_PC
  return (getch ());
#else
  return (getchar ());
#endif
#endif
}

int 
inkey (int lo, int hi)
{
#ifdef P_ARCHIMEDES
  _kernel_swi_regs regs;
  regs.r[0] = 129;
  regs.r[1] = lo;
  regs.r[2] = hi;
  _kernel_swi (OS_Byte, &regs, &regs);
  if (regs.r[2] == 0)
    return (regs.r[1]);
  return (-1);
#else
  return (-1);
#endif
}

void 
writech (unsigned char a)
{
#ifdef P_ARCHIMEDES
  _kernel_swi_regs regs;
  regs.r[0] = a;
  _kernel_swi (OS_WriteC, &regs, &regs);
#else
  putchar (a);
#endif
}

int 
xpos ()
{
#ifdef P_ARCHIMEDES
  _kernel_swi_regs regs;
  regs.r[0] = 134;
  _kernel_swi (OS_Byte, &regs, &regs);
  return (regs.r[1]);
#else
  return (0);
#endif
}

int 
ypos ()
{
#ifdef P_ARCHIMEDES
  _kernel_swi_regs regs;
  regs.r[0] = 134;
  _kernel_swi (OS_Byte, &regs, &regs);
  return (regs.r[2]);
#else
  return (0);
#endif
}

void 
window (int y1, int y2)
{
#ifdef P_ARCHIMEDES
  writech (28);
  writech (0);
  writech (y2);
  writech (79);
  writech (y1);
#endif
}

void 
gotoxy (int x, int y)
{
#ifdef P_ARCHIMEDES
  writech (31);
  writech (x);
  writech (y);
#endif
}

void 
mode (int m)
{
#ifdef P_ARCHIMEDES
  writech (22);
  writech (m);
#endif
}

void 
circlefill (int xm, int ym, int xr, int yr)
{
  xm = xm;
  ym = ym;
  xr = xr;
  yr = yr;
}

void 
circleoutline (int xm, int ym, int xr, int yr)
{
  xm = xm;
  ym = ym;
  xr = xr;
  yr = yr;
}

void 
line (int x1, int y1, int x2, int y2)
{
  x1 = x1;
  y1 = y1;
  x2 = x2;
  y2 = y2;
}

void 
rectanglefill (int x1, int y1, int x2, int y2, int x3, int y3)
{
  x1 = x1;
  y1 = y1;
  x2 = x2;
  y2 = y2;
  x3 = x3;
  y3 = y3;
}

void 
trianglefill (int x1, int y1, int x2, int y2, int x3, int y3)
{
  x1 = x1;
  y1 = y1;
  x2 = x2;
  y2 = y2;
  x3 = x3;
  y3 = y3;
}
