#include <iostream>
#include "Librarian.h"
#include "BError.h"

int main(int argc,char **argv)
{
  try
    {
      Librarian libFile (argc, argv);
      libFile.run();
    }

  catch (BError err)
    {
      cerr << &err;
      return 1;
    }

  return 0;
}
