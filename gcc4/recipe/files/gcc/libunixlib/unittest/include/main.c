/* Main unittest body.
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

int
main (int argc, char *argv[])
{
  for (int i = 1; i != argc; ++i)
    {
      if (!strcmp(argv[i], "--help"))
        {
          fprintf (stderr, "Syntax: %s [ --help | --verbose ]\n", argv[0]);
          return EXIT_FAILURE;
        }
      else if (!strcmp(argv[i], "--verbose"))
        gVerbose = true;
      else
        {
          fprintf (stderr, "Unknown argument, use --help for more info.\n");
          return EXIT_FAILURE;
        }
    }

  if (Check_DirEmpty ())
    {
      fprintf (stderr, "Error: Current directory should be empty before testing.\n");
      return EXIT_FAILURE;
    }

  bool ok = true;
  for (size_t i = 0; i != sizeof (oTesters)/sizeof (oTesters[0]); ++i)
    {
      fprintf (stderr, "Test '%s'...\n", oTesters[i].name);
      if (oTesters[i].routine())
        {
          fprintf (stderr, "  *** Test '%s' FAILED !\n", oTesters[i].name);
          ok = false;
          if (Clean_CurDir ())
            {
              fprintf (stderr, "  Unable to continue testing.\n");
              break;
            }
        }
      else
        fprintf (stderr, "  ok\n");
    }

  if (ok)
    {
      printf ("All tests were ok\n");
      return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}

