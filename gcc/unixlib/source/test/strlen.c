#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int strlen1 (const char *);

int main (void)
{
  char *buffer[1024];
  int x, y, z;
  int s1, s2, e1, e2;

  /* create strings.  */
  for (x = 0; x <= 1023; x++)
    {
      char *p;
      buffer[x] = p = (char *) malloc (256);
      *p = '\0';
      for (y = 0; y < x; y++)
        *p++ = 'a';
      *p = '\0';
    }

  s1 = clock ();
  for (z = 0; z <= 10; z++)
    for (x = 0; x <= 1023; x++)
      {
        y = strlen (buffer[x]);
        if (y != x)
          printf ("strlen: error\n");
      }
  e1 = clock ();

  s2 = clock ();
  for (z = 0; z <= 10; z++)
    for (x = 0; x <= 1023; x++)
      {
        y = strlen1 (buffer[x]);
        if (y != x)
          printf ("strlen1: error\n");
      }
  e2 = clock ();

  printf ("strlen: time = %d csecs\n", e1 - s1);
  printf ("strlen1: time = %d csecs\n", e2 - s2);

  return 0;
}
