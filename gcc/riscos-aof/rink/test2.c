/* *** test file 2 for rtl *** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char array[1024];
char text[64];
char zonk[] = "This is some text\n\n";

/*char textz[68*1024] = "text";

char *(*pstrcpy)(char *, const char *) = strcpy;*/

int
test_function2 (int number, char *string)
{
  int l;
  int i1, i2, i3, i4;

  printf ("test_function2 called\n\n");
  printf ("number: %d\nstring: %s\n\n", number, string);

  printf ("%s", zonk);

  strcpy (text, "Zonk zonk wibble\n\n");
  printf ("%s\n", text);

  printf (text, "45 12 67 123\n");
  sscanf (text, "%d %d %d %d\n", &i1, &i2, &i3, &i4);

  for (l = 0; l < 512; l++)
    {
      array[l] = l;
    }

  for (l = 0; l < 16; l++)
    {
      printf ("%d\n", array[l]);
    }

  return 84;
}

void
named_fn1 (void)
{
  printf ("named_fn1 called\n\n");
}

void
named_fn2 (void)
{
  printf ("named_fn2 called\n\n");
}
