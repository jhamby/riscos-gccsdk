#include <stdio.h>

class Foo
{
  public:
    Foo() { fprintf (stderr, "Foo::Foo()\n"); }
    ~Foo() { fprintf (stderr, "Foo::~Foo()\n"); }
};

void throwfn (void)
  {
    Foo a_foo;

    fprintf (stderr, "About to throw\n");

    /* throw(5); */
    throw("wobbly");

    fprintf (stderr, "Shouldn't get here\n");
  }

int main (void)
{
  fprintf (stderr, "Entered main\n");
  try
  {
    throwfn();
  }
  catch(const char* text)
  {
    fprintf (stderr, "Caught a '%s'\n", text);
  }
  catch(int x)
  {
    fprintf (stderr, "Caught a number %d\n", x);
    /* fprintf (stderr, "How did I catch this?\n"); */
  }
  fprintf (stderr, "Exiting main.\n");
  return 0;
}
