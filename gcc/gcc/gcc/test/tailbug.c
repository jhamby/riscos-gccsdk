/*
This function generates code which BL's to memset and immediately returns
from the function with MOVS PC,LR. The optimiser thinks that lr does not need
saving (which is true), but gets the tail call wrong. The call to memset should
be via B instead.

Compile with `gcc -O1 tail-bug.c -S'.

Note, -O2 and -O3 have the same effect. The bug can be avoided by adding some
code. Compile with -DFIX_BUG.

*/

#define MAXDIRTY 512
extern int use_dirty;
extern char* dirty_new;

void osd_mark_dirty (int x1, int y1, int x2, int y2, int ui)
{
#ifdef FIX_BUG
  int count =0;
#endif

  if (use_dirty)
    {
      int tmp;
      if (y1 < 0) y1 = 0;
      if (y1 >= MAXDIRTY) y1 = MAXDIRTY-1;
      if (y2 >= MAXDIRTY) y2 = MAXDIRTY-1;
        
#ifdef FIX_BUG
      count++;
#endif

      memset (&dirty_new[y1], 1, y2-y1+1);
    }
}
