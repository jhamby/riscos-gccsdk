#include <iostream>

#include <cstdlib>

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
      std::cerr << "libfile: " << err << std::endl;
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
