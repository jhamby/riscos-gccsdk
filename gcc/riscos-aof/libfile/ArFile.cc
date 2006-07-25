#include <iostream>

#include <cstdlib>

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
      std::cerr << "ar: " << err << std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
