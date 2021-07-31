/* readlink() unittest.
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
 * Test creating basic readlink.
 */
static bool
Test_001_BasicReadLink (void)
{
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-file", "001_src");
  STEP(Check_ReadLink, "001_src", "non-existing-file");
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() buf size behaviour.
 */
static bool
Test_002_BufSize (void)
{
  STEP(Check_DirEmpty);

  char buf[sizeof ("non-existing-object")-1 + 3];
  STEP(SymLink, "non-existing-object", "symlink");

  /* Given buf size is one byte too big.  */
  memset (buf, '\xc8', sizeof (buf));
  STEP(ExpectCall_ReadLink, "symlink", buf, sizeof ("non-existing-object")-1 + 1, sizeof ("non-existing-object")-1, 0);
  STEP(memcmp, buf, "non-existing-object\xc8\xc8\xc8", sizeof (buf));

  /* Given buf size is just big enough.  */
  memset (buf, '\xc8', sizeof (buf));
  STEP(ExpectCall_ReadLink, "symlink", buf, sizeof ("non-existing-object")-1 + 0, sizeof ("non-existing-object")-1, 0);
  STEP(memcmp, buf, "non-existing-object\xc8\xc8\xc8", sizeof (buf));

  /* Given buf size is two bytes too small.  */
  memset (buf, '\xc8', sizeof (buf));
  STEP(ExpectCall_ReadLink, "symlink", buf, sizeof ("non-existing-object")-1 - 2, sizeof ("non-existing-object")-1 - 2, 0);
  STEP(memcmp, buf, "non-existing-obje\xc8\xc8\xc8\xc8\xc8", sizeof (buf));

  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() EINVAL generation.
 *   - Buffer size required which is bigger than SSIZE_MAX (not realistic to
 *     test for)
 *   - Given file is not a symbolic link.
 */
static bool
Test_003_GenerationOfEINVAL (void)
{
  STEP(Check_DirEmpty);

  char buf[12];

  STEP(Create_File, "afile");
  STEP(ExpectCall_ReadLink, "afile", buf, sizeof (buf)-1, -1, EINVAL);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "adir");
  STEP(ExpectCall_ReadLink, "adir", buf, sizeof (buf)-1, -1, EINVAL);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() EFAULT generation.
 */
static bool
Test_004_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  char buf[5];
  STEP(ExpectCall_ReadLink, NULL, buf, sizeof (buf), -1, EFAULT);
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-object", "asymlink");
  STEP(ExpectCall_ReadLink, "asymlink", NULL, sizeof (buf), -1, EFAULT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() ENOENT generation.
 */
static bool
Test_005_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  char buf[5];
  STEP(ExpectCall_ReadLink, "non-existing-object", buf, sizeof (buf)-1, -1, ENOENT);
  STEP(Check_DirEmpty);

  STEP(ExpectCall_ReadLink, "", buf, sizeof (buf)-1, -1, ENOENT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() ENOTDIR generation.
 */
static bool
Test_006_GenerationOfENOTDIR (void)
{
  STEP(Check_DirEmpty);

  /* FIXME: this ENOTDIR case is not yet properly supported in UnixLib.  */
  STEP(Create_Dir, "dir1");
  STEP(Create_File, "dir1/file2");
  char buf[5];
  STEP(ExpectCall_ReadLink, "dir1/file2/symlink", buf, sizeof (buf)-1, -1, ENOTDIR);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test readlink() ENAMETOOLONG error generation.
 */
static bool
Test_007_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  char buf[5];
  STEP(ExpectCall_ReadLink, longFileName, buf, sizeof (buf)-1, -1, ENAMETOOLONG);
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
  TESTER_ENTRY(Test_001_BasicReadLink),
  TESTER_ENTRY(Test_002_BufSize),
  TESTER_ENTRY(Test_003_GenerationOfEINVAL),
  TESTER_ENTRY(Test_004_GenerationOfEFAULT),
  TESTER_ENTRY(Test_005_GenerationOfENOENT),
  TESTER_ENTRY(Test_006_GenerationOfENOTDIR),
  TESTER_ENTRY(Test_007_GenerationOfENAMETOOLONG),
};

#include "include/main.c"

