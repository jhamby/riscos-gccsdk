#include <stdio.h>

static int count;

class foo
{
  public:
    foo();
    ~foo();
};

foo::foo()
{
  printf ("*** foo::foo(): %d -> %d\n", count, count + 1);
  ++count;
}

foo::~foo()
{
  printf ("*** foo::~foo(): %d -> %d\n", count, count - 1);
  --count;
}

foo test;

int
main (int argc, char *argv[])
{
  printf ("*** main() - begin\n");
  foo test2;
  printf ("*** main() - end\n");
  return 0;
}
