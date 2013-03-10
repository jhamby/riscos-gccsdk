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

  /* Only test the change in modification date and up to one csec accuracy.  */
  STEP(Create_File, "file");
  struct timeval times[2] =
    { { .tv_sec = 0, .tv_usec = 0 }, { .tv_sec = 0, .tv_usec = 0 } };
  for (unsigned mi = 0; mi != 17; ++mi)
    {
      for (unsigned cs = 0; cs != 100; cs += 2)
        {
          times[1].tv_usec = cs*10000; /* 1 csec.  */
          STEP(ExpectCall_UTimes, "file", times, 0);
          STEP(Check_FileModDate, "file", times[1].tv_sec, cs);
        }
      times[1].tv_sec = 4*times[1].tv_sec + 3;
    }
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Check NULL ptr for times parameter.  */
  STEP(Create_File, "file");
  struct timespec tp;
  STEP(clock_gettime, CLOCK_REALTIME, &tp);
  STEP(ExpectCall_UTimes, "file", NULL, 0);
  STEP(Check_FileModDate, "file", tp.tv_sec, tp.tv_nsec / 10000000);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

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

  return false;
}

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
};

#include "include/main.c"

