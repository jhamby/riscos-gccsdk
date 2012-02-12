#include <kernel.h>
#include <stdio.h>
#include "macros.h"

_kernel_oserror *test_Toolbox_ShowObject (unsigned int flags,
				    unsigned int object_id,
				    int show_type,
				    void *type,
				    unsigned int parent_id,
				    int parent_cmp)
{
unsigned int block;//[5][4];
int x,y;

  for (x = 0; x < show_type; x++)
  {
    *((unsigned int *)type) = object_id + x;
    for (y = 0; y < parent_cmp; y++)
      block/*[x][y]*/ = parent_id + x * y;
  }
//block[0][1] = parent_id;
//block[1][2] = (unsigned int)parent_cmp;
return (_kernel_oserror *)block;
//  return (_kernel_oserror *)block[2][3];
}
