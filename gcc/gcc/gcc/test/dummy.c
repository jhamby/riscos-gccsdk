#include <assert.h>

void
foobar (void)
{
  assert (1 == 0);
}

int main (void)
{
  foobar (45);
  return 0;
}
