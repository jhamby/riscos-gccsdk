#include <stdio.h>

int spam (int a1, int a2, int a3, int a4, int a5)
{
  int spam1 (int i1, int i2)
  {
    printf ("spam1: %d, %d, %d, %d", a1, a2, a3, a4);
    printf (", %d, %d, %d\n", a5, i1, i2);
    return a1;
  }
  spam1 (a5, a1);
  spam1 (a2, a3);
  return a1+a2+a3+a4+a5;
}

int main(int argc, char *argv[])
{

  printf ("spam = %d\n", spam (1, 2, 3, 4, 5));
  return 0;
}

