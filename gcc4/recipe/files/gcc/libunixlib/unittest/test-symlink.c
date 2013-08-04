/* symlink() unittest.
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/expect-symlink.c"
#include "include/expect-readlink.c"
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
 * Test creating basic symlink.
 */
static bool
Test_001_BasicSymLink (void)
{
  STEP(Check_DirEmpty);

  /* Symlink 001_src to 001_dst (file).  */
  STEP(Create_File, "001_dst");
  STEP(ExpectCall_SymLink, "001_dst", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "001_dst");
  static const char * const fnames1[] = { "001_dst", "001_src" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to 001_dst (directory).  */
  STEP(Create_Dir, "001_dst");
  STEP(ExpectCall_SymLink, "001_dst", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "001_dst");
  static const char * const fnames2[] = { "001_dst", "001_src" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to . (directory).  */
  STEP(ExpectCall_SymLink, ".", "001_src", 0);
  STEP(Check_ReadLink, "001_src", ".");
  static const char * const fnames3[] = { "001_src" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to .. (directory).  */
  STEP(ExpectCall_SymLink, "..", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "..");
  static const char * const fnames4[] = { "001_src" };
  STEP(Check_DirContents, fnames4, sizeof (fnames4)/sizeof (fnames4[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to ../.. (directory).  */
  STEP(ExpectCall_SymLink, "../..", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "../..");
  static const char * const fnames5[] = { "001_src" };
  STEP(Check_DirContents, fnames5, sizeof (fnames5)/sizeof (fnames5[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to unexisting object.  */
  STEP(ExpectCall_SymLink, "non_existing_object", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "non_existing_object");
  static const char * const fnames6[] = { "001_src" };
  STEP(Check_DirContents, fnames6, sizeof (fnames6)/sizeof (fnames6[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Symlink 001_src to itself.  */
  STEP(ExpectCall_SymLink, "001_src", "001_src", 0);
  STEP(Check_ReadLink, "001_src", "001_src");
  static const char * const fnames7[] = { "001_src" };
  STEP(Check_DirContents, fnames7, sizeof (fnames7)/sizeof (fnames7[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test symlink() EEXIST generation.
 */
static bool
Test_003_GenerationOfEEXIST (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "afile");
  STEP(ExpectCall_SymLink, "non_existing_object", "afile", EEXIST);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "adir");
  STEP(ExpectCall_SymLink, "non_existing_object", "adir", EEXIST);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(SymLink, "nowhere", "asymlink");
  STEP(ExpectCall_SymLink, "non_existing_object", "asymlink", EEXIST);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test symlink() EFAULT generation.
 */
static bool
Test_004_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  STEP(ExpectCall_SymLink, "non_existing_object", NULL, EFAULT);
  STEP(ExpectCall_SymLink, NULL, "non_existing_object", EFAULT);
  STEP(ExpectCall_SymLink, NULL, NULL, EFAULT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test symlink() ENOENT generation.
 *   - symlink(xxx,) when xxx is empty string
 *   - symlink(,yyy) when yyy does not exist or is dangling symlink
 */
static bool
Test_005_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  STEP(ExpectCall_SymLink, "", "symlink", ENOENT);
  STEP(Check_DirEmpty);

  STEP(ExpectCall_SymLink, "non_existing_object", "dir1/dir2/symlink", ENOENT);
  STEP(Check_DirEmpty);

  STEP(SymLink, "non_existing_object", "dir");
  STEP(ExpectCall_SymLink, "nowhere", "dir/symlink", ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test symlink() ENOTDIR generation.
 *   - symlink(,yyy) when component in yyy is not a directory.
 */
static bool
Test_006_GenerationOfENOTDIR (void)
{
  STEP(Check_DirEmpty);

  /* FIXME: this ENOTDIR case is not yet properly supported in UnixLib.  */
  STEP(Create_Dir, "dir1");
  STEP(Create_File, "dir1/file2");
  STEP(ExpectCall_SymLink, "non_existing_object", "dir1/file2/symlink", ENOTDIR);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test symlink() ENAMETOOLONG error generation.
 */
static bool
Test_007_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  STEP(ExpectCall_SymLink, "non_existing_object", longFileName, ENAMETOOLONG);
  STEP(ExpectCall_SymLink, longFileName, "symlink", ENAMETOOLONG);
  STEP(Check_DirEmpty);

  free (longFileName);

  return false;
}

#define TESTER_ENTRY(a) \
  { __STRING(a), a }
typedef bool (*Tester)(void);
static const struct
{
  const char *name;
  Tester routine;
} oTesters[] =
{
  TESTER_ENTRY(Test_001_BasicSymLink),
  TESTER_ENTRY(Test_003_GenerationOfEEXIST),
  TESTER_ENTRY(Test_004_GenerationOfEFAULT),
  TESTER_ENTRY(Test_005_GenerationOfENOENT),
  TESTER_ENTRY(Test_006_GenerationOfENOTDIR),
  TESTER_ENTRY(Test_007_GenerationOfENAMETOOLONG),
};

#include "include/main.c"

