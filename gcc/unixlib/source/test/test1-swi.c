
/**
 * UnixLib test 1
 *
 * _swix and _swi.
 *
 * Quite a few things rely on these functions, including backtrace
 * handlers, so it's important they work.
 *
 */

#include <stdio.h>
#include <kernel.h>
#include <swis.h>

int main (int argc, char *argv[])
{
  unsigned int r0, r5, pc, ret;

  /* *FX 0 */
  puts("Calling _swix");
  _swix(OS_Byte, _IN(0), 0);
  puts("Calling _swi");
  _swi(OS_Byte, _IN(0), 0);
  puts("Calling _swix");
  _swix(OS_Byte, _IN(0), 0);

  puts("");
  puts("Calling _swi - testing _OUT and _RETURN");
  puts("ret and pc should be identical, r0 should be the result from OS_Confirm");
  ret = _swi(OS_Confirm, _OUT(0) | _OUT(_FLAGS) | _RETURN(_FLAGS), &r0, &pc);
  printf("ret: %x r0: %d pc: %x\n", ret, r0, pc);
  puts("");
  puts("Calling _swix - testing _OUT");
  puts("r5 should contain the attributes of this file");
  _swix(OS_File, _INR(0,1) | _OUT(0) | _OUT(5), 17, argv[0], &r0, &r5);
  printf("r5: %x\n", r5);

  puts("");
  puts("Calling _swix - testing _BLOCK");
  puts("A black square should be plotted at the bottom left corner of the screen");
  _swix(Draw_Fill, _INR(1,3) | _BLOCK(0), 0, 0, 0, 2, 0, 100*256,
  						   8, 100*256, 100*256,
  						   8, 100*256, 0,
  						   8, 0, 0,
  						   0, 0);

  return 0;
}
