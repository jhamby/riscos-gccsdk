/* utimes() unittest.
   Copyright (c) 2013 UnixLib Developers.

   Note: very similar to test-utimes.c.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "include/expect-utimes.c"
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
Test_001_BasicUTimes (void)
{
  STEP(Check_DirEmpty);

  const char * const testFiles[] =
    {
      "file",
      "file,abc",
      "file,fff"
    };

  /* Only test the change in modification date and up to one csec accuracy.  */
  for (size_t idx = 0; idx != sizeof (testFiles)/sizeof (testFiles[0]); ++idx)
    {
      STEP(Create_File, testFiles[idx]);
      struct timeval times[2] =
        { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
      for (unsigned mi = 0; mi != 17; ++mi)
        {
          for (unsigned cs = 0; cs != 100; cs += 2)
            {
              times[1].tv_usec = cs*10000; /* 1 csec.  */
              STEP(ExpectCall_UTimes, testFiles[idx], times, 0);
              STEP(Check_FileModDate, testFiles[idx], times[1].tv_sec, cs);
            }
          times[1].tv_sec = 4*times[1].tv_sec + 3;
        }
      STEP(Clean_CurDir);
      STEP(Check_DirEmpty);
    }

  /* Same but now via a symlink.  */
  for (size_t idx = 0; idx != sizeof (testFiles)/sizeof (testFiles[0]); ++idx)
    {
      STEP(Create_File, testFiles[idx]);
      STEP(SymLink, testFiles[idx], "symlink");
      struct timeval times[2] =
        { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
      for (unsigned mi = 0; mi != 17; ++mi)
        {
          for (unsigned cs = 0; cs != 100; cs += 2)
            {
              times[1].tv_usec = cs*10000; /* 1 csec.  */
              STEP(ExpectCall_UTimes, "symlink", times, 0);
              STEP(Check_FileModDate, testFiles[idx], times[1].tv_sec, cs);
            }
          times[1].tv_sec = 4*times[1].tv_sec + 3;
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
      STEP(ExpectCall_UTimes, testFiles[idx], NULL, 0);
      STEP(Check_FileModDate, testFiles[idx], tp.tv_sec, tp.tv_nsec / 10000000);
      STEP(Clean_CurDir);
      STEP(Check_DirEmpty);
    }

  return false;
}

/**
 * Test utimes() ENOENT error generation.
 */
static bool
Test_002_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  struct timeval times[2] =
    { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
  STEP(ExpectCall_UTimes, "nofile", times, ENOENT);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file,abc");
  STEP(ExpectCall_UTimes, "file", times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file");
  STEP(ExpectCall_UTimes, "file,abc", times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(ExpectCall_UTimes, "", times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Dangling symlink.  */
  STEP(SymLink, "non-existing-object", "symlink");
  STEP(ExpectCall_UTimes, "symlink", times, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test utimes() EFAULT error generation.
 */
static bool
Test_003_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  struct timeval times[2] =
    { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
  STEP(ExpectCall_UTimes, NULL, times, EFAULT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test utimes() ENAMETOOLONG error generation.
 */
static bool
Test_004_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  struct timeval times[2] =
    { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
  STEP(ExpectCall_UTimes, longFileName, times, ENAMETOOLONG);
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
  TESTER_ENTRY(Test_001_BasicUTimes),
  TESTER_ENTRY(Test_002_GenerationOfENOENT),
  TESTER_ENTRY(Test_003_GenerationOfEFAULT),
  TESTER_ENTRY(Test_004_GenerationOfENAMETOOLONG),
};

#include "include/main.c"

