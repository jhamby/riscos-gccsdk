/* utime() unittest.
   Copyright (c) 2013 UnixLib Developers.

   Note: very similar to test-utimes.c.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <utime.h>

#include "include/expect-utime.c"
#include "include/expect-symlink.c"
#include "include/check.c"

static bool gVerbose = false;

#define STEP(call, ...) \
  do \
    { \
      if (gVerbose) \
        fprintf (stderr, "  call " __STRING(call) "\n"); \
      if (call(__VA_ARGS__)) \
        { \
          fprintf (stderr, "  *** " __STRING(call) " FAILED at line %d !\n", __LINE__); \
          return true; \
        } \
    } while (0)

static bool
Test_001_BasicUTime (void)
{
  STEP(Check_DirEmpty);

  const char * const testFiles[] =
    {
      "file",
      "file,abc",
      "file,fff"
    };

  /* Only test the change in modification date.  */
  for (size_t idx = 0; idx != sizeof (testFiles)/sizeof (testFiles[0]); ++idx)
    {
      STEP(Create_File, testFiles[idx]);
      struct utimbuf times = { .actime = 0, .modtime = 0 };
      for (unsigned mi = 0; mi != 17; ++mi)
        {
          STEP(ExpectCall_UTime, testFiles[idx], &times, 0);
          STEP(Check_FileModDate, testFiles[idx], times.modtime, 0);
          times.modtime = 4*times.modtime + 3;
        }
      STEP(Clean_CurDir);
      STEP(Check_DirEmpty);
    }

  /* Same, but now via a symlink.  */
  for (size_t idx = 0; idx != sizeof (testFiles)/sizeof (testFiles[0]); ++idx)
    {
      STEP(Create_File, testFiles[idx]);
      STEP(Create_SymLink, testFiles[idx], "symlink");
      struct utimbuf times = { .actime = 0, .modtime = 0 };
      for (unsigned mi = 0; mi != 17; ++mi)
        {
          STEP(ExpectCall_UTime, "symlink", &times, 0);
          STEP(Check_FileModDate, testFiles[idx], times.modtime, 0);
          times.modtime = 4*times.modtime + 3;
        }
      STEP(Clean_CurDir);
      STEP(Check_DirEmpty);
    }

  /* Check NULL ptr for times parameter.  */
  for (size_t idx = 0; idx != sizeof (testFiles)/sizeof (testFiles[0]); ++idx)
    {
      STEP(Create_File, testFiles[idx]);
      struct timespec tp;
      STEP(clock_gettime, CLOCK_REALTIME, &tp);
      STEP(ExpectCall_UTime, testFiles[idx], NULL, 0);
      STEP(Check_FileModDate, testFiles[idx], tp.tv_sec, tp.tv_nsec / 10000000);
      STEP(Clean_CurDir);
      STEP(Check_DirEmpty);
    }

  return false;
}

/**
 * Test utime() ENOENT error generation.
 */
static bool
Test_002_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  struct utimbuf times = { .actime = 0, .modtime = 0 };
  STEP(ExpectCall_UTime, "nofile", &times, ENOENT);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file,abc");
  STEP(ExpectCall_UTime, "file", &times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file");
  STEP(ExpectCall_UTime, "file,abc", &times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(ExpectCall_UTime, "", &times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Dangling symlink.  */
  STEP(Create_SymLink, "non-existing-object", "symlink");
  STEP(ExpectCall_UTime, "symlink", &times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test utime() EFAULT error generation.
 */
static bool
Test_003_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  struct utimbuf times = { .actime = 0, .modtime = 0 };
  STEP(ExpectCall_UTime, NULL, &times, EFAULT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test utime() ENAMETOOLONG error generation.
 */
static bool
Test_004_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  struct utimbuf times = { .actime = 0, .modtime = 0 };
  STEP(ExpectCall_UTime, longFileName, &times, ENAMETOOLONG);
  STEP(Check_DirEmpty);

  free (longFileName);

  return false;
}

/* FIXME: Add tests:
     - EACCES
 */

#define TESTER_ENTRY(a) \
  { __STRING(a), a }
typedef bool (*Tester)(void);
static const struct
{
  const char *name;
  Tester routine;
} oTesters[] =
{
  TESTER_ENTRY(Test_001_BasicUTime),
  TESTER_ENTRY(Test_002_GenerationOfENOENT),
  TESTER_ENTRY(Test_003_GenerationOfEFAULT),
  TESTER_ENTRY(Test_004_GenerationOfENAMETOOLONG),
};

#include "include/main.c"

