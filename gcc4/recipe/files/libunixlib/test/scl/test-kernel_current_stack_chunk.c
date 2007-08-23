/* Test _kernel_current_stack_chunk.  */

#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>

int main(int argc, char *argv[])
{
  _kernel_stack_chunk *chunk = _kernel_current_stack_chunk ();

  if (chunk == NULL)
    abort ();

  if (chunk->sc_mark != 0xF60690FF)
    abort ();

  return 0;
}
