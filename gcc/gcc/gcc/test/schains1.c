#include <stdio.h>


typedef int (*fspam1)(int, int);
void spam3 (fspam1 fn, int a1, int a2)
{
  int spam2 (int i1, int i2)
  {
    printf ("spam2: %d, %d %d %d\n", a1, a2, i1, i2);
    (*fn)(i1, i2);
    return a1;
  }
  printf ("calling spam2 (%d, %d)\n", a1, a2);
  printf ("return val from spam2 = %d, should be %d\n", spam2 (a1, a2), a1);
}

int spam (int a1, int a2, int a3, int a4, int a5)
{
  int spam1 (int i1, int i2)
  {
    printf ("spam1: %d, %d, %d, %d", a1, a2, a3, a4);
    printf (", %d, %d, %d\n", a5, i1, i2);
    return a1;
  }
  spam3 (spam1, a4, a1);
  spam1 (a5, a1);
  spam3 (spam1, a3, a4);
  spam1 (a2, a3);
  return a1+a2+a3+a4+a5;
}

int main(int argc, char *argv[])
{

  printf ("spam = %d\n", spam (1, 2, 3, 4, 5));
  return 0;
}

