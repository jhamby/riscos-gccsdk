#include <iostream>
#include "Ar.h"
#include "BError.h"

int main(int argc,char **argv)
{
  try
    {
      Ar libFile (argc, argv);
      libFile.run();
    }

  catch (BError err)
    {
      cerr << &err;
    }

  return 0;
}
