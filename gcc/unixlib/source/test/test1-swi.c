
/**
 * Unixlib test 1
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

int main (int argc, char *argv[]) {

  /* *FX 0 */
  puts("Calling _swix");
  _swix(OS_Byte, _IN(0), 0);
  puts("Calling _swi");
  _swi(OS_Byte, _IN(0), 0);
  puts("Calling _swix");
  _swix(OS_Byte, _IN(0), 0);

  return 0;
}
