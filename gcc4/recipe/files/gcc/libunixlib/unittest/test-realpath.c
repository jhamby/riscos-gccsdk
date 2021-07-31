/* realpath() unittest
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>

#include "include/expect-realpath.c"
#include "include/expect-symlink.c"
#include "include/check.c"

#ifdef __riscos__
int __riscosify_control = __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_FFF_EXT;
#endif

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

/**
 * Basic realpath() test.
 */
static bool
Test_001_BasicRealPath (void)
{
  STEP(Check_DirEmpty);

  char *testDir;
  STEP(RealPath, ".", &testDir);

  char buf[PATH_MAX];

  STEP(ExpectCall_RealPath, ".", NULL, testDir, "", 0);
  STEP(Create_File, "file1");
  STEP(ExpectCall_RealPath, "file1", NULL, testDir, "file1", 0);
  STEP(ExpectCall_RealPath, "file1", buf, testDir, "file1", 0);
  STEP(Create_Dir, "dir1");
  STEP(ExpectCall_RealPath, "dir1", NULL, testDir, "dir1", 0);
  STEP(ExpectCall_RealPath, "dir1", buf, testDir, "dir1", 0);
  STEP(ExpectCall_RealPath, "dir1/../file1", NULL, testDir, "file1", 0);
  STEP(ExpectCall_RealPath, "dir1/../file1", buf, testDir, "file1", 0);
  STEP(ExpectCall_RealPath, "dir1/../dir1/../dir1", NULL, testDir, "dir1", 0);
  STEP(ExpectCall_RealPath, "dir1/../dir1/../dir1", buf, testDir, "dir1", 0);

  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  free (testDir);
  return false;
}

/**
 * Test realpath() EINVAL generation.
 */
static bool
Test_003_GenerationOfEINVAL (void)
{
  STEP(Check_DirEmpty);

  STEP(ExpectCall_RealPath, NULL, NULL, NULL, NULL, EINVAL);
  char *foo = "";
  STEP(ExpectCall_RealPath, NULL, foo, NULL, NULL, EINVAL);

  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test realpath() ELOOP generation.
 */
static bool
Test_004_GenerationOfELOOP (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "dir1");
  STEP(SymLink, "../dir2/subdir2", "dir1/subdir1");
  STEP(Create_Dir, "dir2");
  STEP(SymLink, "../dir1/subdir1", "dir2/subdir2"); /* Make dir1/subdir1 -> dir2/subdir2 -> dir1/subdir1 symlink loop.  */
  STEP(ExpectCall_RealPath, "dir1/subdir1", NULL, NULL, NULL, ELOOP);

  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test realpath() ENOENT generation.
 */
static bool
Test_005_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  STEP(ExpectCall_RealPath, "notexisting", NULL, NULL, NULL, ENOENT);

  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test realpath() ENOTDIR generation.
 */
static bool
Test_006_GenerationOfENOTDIR (void)
{
  STEP(Check_DirEmpty);

  /* FIXME: this ENOTDIR case is not yet properly supported in UnixLib.  */
  STEP(Create_Dir, "dir1");
  STEP(Create_File, "dir1/file2");
  STEP(ExpectCall_RealPath, "dir1/file2/object", NULL, NULL, NULL, ENOTDIR);

  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

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

#define TESTER_ENTRY(a) \
  { __STRING(a), a }
typedef bool (*Tester)(void);
static const struct
{
  const char *name;
  Tester routine;
} oTesters[] =
{
  TESTER_ENTRY(Test_001_BasicRealPath),
  TESTER_ENTRY(Test_003_GenerationOfEINVAL),
  TESTER_ENTRY(Test_004_GenerationOfELOOP),
  TESTER_ENTRY(Test_005_GenerationOfENOENT),
  TESTER_ENTRY(Test_006_GenerationOfENOTDIR),
};

#include "include/main.c"

