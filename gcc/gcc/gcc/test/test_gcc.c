/* Ackermann function */

#include <stdio.h>
#include <setjmp.h>

void foo (int);

jmp_buf jmp;

int main(int argc, char *argv[])
{
  char buff[0x1b0];

  if (setjmp (jmp))
    {
      printf ("We reached here.\n");
      __print_chunk_lists ();
    }
  else
    {
      foo ((int)buff);
    }
  return 0;
}


void foo (int a)
{
  int i,r,v,s,t;
  void bar (int a1, int a2, int a3, int a4, int a5, int a6)
  {
    int x,y,z;
    int buffer[r];
    int buff[s];
    void smart (int zzz)
    {
      printf ("Smart values:\nzzz = %d\na1 = %d\na2 = %d\n", zzz, a1, a2);
      printf ("a3 = %d\na4 = %d\na5 = %d\na6 = %d\n", a3, a4, a5, a6);
      printf ("i = %d\nr = %d\nv = %d\ns = %d\nt = %d\n", i, r, v, s, t);
      printf ("x = %d\nz = %d\n", x, z);
      longjmp (jmp, 1);
    }


    x=56;
    z=x+i;
    y=buffer[z*x/i] + buff[53];
    smart (55);
    printf("Bar Values:\na = %d\ni = %d\n", a, i);
    printf("r = %d\nv = %d\ns = %d\nt = %d\n", r, v, s, t);
    printf("x = %d\nz = %d\n", x, z);
    printf("a1 = %d\na2 = %d\na3 = %d\n", a1, a2, a3);
    printf("a4 = %d\na5 = %d\na6 = %d\n", a4, a5, a6);
  }

  i = 2;
  r = 3000;
  v = 4;
  s = 5000;
  t = 6;


  bar (i,r,v,s,t, 7);
}

int ohyeah (int x)
{
  register void *sp asm ("sp");

  if ((void *)sp <= (void *)x)
    return 1;
}
