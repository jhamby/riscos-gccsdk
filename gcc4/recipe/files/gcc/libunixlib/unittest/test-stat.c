/* stat()/lstat() unittest.
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef TEST_LSTAT_INSTEAD_OF_STAT
# include "include/expect-lstat.c"
#else
# include "include/expect-stat.c"
#endif
#include "include/expect-symlink.c"
#include "include/check.c"

#ifdef TEST_LSTAT_INSTEAD_OF_STAT
# define EXPECTCALL_STAT_OR_LSTAT ExpectCall_LStat
#else
# define EXPECTCALL_STAT_OR_LSTAT ExpectCall_Stat
#endif

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

static bool
IsFile (const struct stat *buf)
{
  return !(S_ISREG(buf->st_mode) && !S_ISDIR(buf->st_mode)
            && !S_ISCHR(buf->st_mode) && !S_ISBLK(buf->st_mode)
            && !S_ISFIFO(buf->st_mode) && !S_ISLNK(buf->st_mode)
            && !S_ISSOCK(buf->st_mode));
}

static bool
IsDir (const struct stat *buf)
{
  return !(!S_ISREG(buf->st_mode) && S_ISDIR(buf->st_mode)
            && !S_ISCHR(buf->st_mode) && !S_ISBLK(buf->st_mode)
            && !S_ISFIFO(buf->st_mode) && !S_ISLNK(buf->st_mode)
            && !S_ISSOCK(buf->st_mode));
}

static bool
IsLink (const struct stat *buf)
{
  return !(!S_ISREG(buf->st_mode) && !S_ISDIR(buf->st_mode)
            && !S_ISCHR(buf->st_mode) && !S_ISBLK(buf->st_mode)
            && !S_ISFIFO(buf->st_mode) && S_ISLNK(buf->st_mode)
            && !S_ISSOCK(buf->st_mode));
}

/**
 * Test basic stat() functionality.
 */
static bool
Test_001_BasicStat (void)
{
  STEP(Check_DirEmpty);

  /* File.  */
  {
    STEP(Create_File, "file");
    struct stat buf;
    STEP(EXPECTCALL_STAT_OR_LSTAT, "file", &buf, 0);
    STEP(IsFile, &buf);

    STEP(Create_SymLink, "file", "symlink-to-file");
    STEP(EXPECTCALL_STAT_OR_LSTAT, "symlink-to-file", &buf, 0);
#ifdef TEST_LSTAT_INSTEAD_OF_STAT
    STEP(IsLink, &buf);
#else
    STEP(IsFile, &buf);
#endif

    STEP(Clean_CurDir);
    STEP(Check_DirEmpty);
  }

  /* Directory.  */
  {
    STEP(Create_Dir, "dir");
    struct stat buf;
    STEP(EXPECTCALL_STAT_OR_LSTAT, "dir", &buf, 0);
    STEP(IsDir, &buf);

    STEP(Create_SymLink, "dir", "symlink-to-dir");
    STEP(EXPECTCALL_STAT_OR_LSTAT, "symlink-to-dir", &buf, 0);
#ifdef TEST_LSTAT_INSTEAD_OF_STAT
    STEP(IsLink, &buf);
#else
    STEP(IsDir, &buf);
#endif

    STEP(Clean_CurDir);
    STEP(Check_DirEmpty);
  }

  return false;
}

/**
 * Test stat()/lstat() EFAULT generation.
 */
static bool
Test_003_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  struct stat buf;
  STEP(EXPECTCALL_STAT_OR_LSTAT, NULL, &buf, EFAULT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test stat()/lstat() ELOOP generation.
 */
static bool
Test_004_GenerationOfELOOP (void)
{
  STEP(Check_DirEmpty);

  /* Only applicable for stat().  */
#ifndef TEST_LSTAT_INSTEAD_OF_STAT
  STEP(Create_SymLink, "symlink2", "symlink1");
  STEP(Create_SymLink, "symlink3", "symlink2");
  STEP(Create_SymLink, "symlink1", "symlink3");
  struct stat buf;
  STEP(EXPECTCALL_STAT_OR_LSTAT, "symlink1", &buf, ELOOP);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);
#endif

  return false;
}

/**
 * Test stat()/lstat() ENOTDIR generation.
 */
static bool
Test_005_GenerationOfENOTDIR (void)
{
  STEP(Check_DirEmpty);

  /* FIXME: this ENOTDIR case is not yet properly supported in UnixLib.  */
  STEP(Create_Dir, "dir1");
  STEP(Create_File, "dir1/file2");
  struct stat buf;
  STEP(EXPECTCALL_STAT_OR_LSTAT, "dir1/file2/object", &buf, ENOTDIR);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test stat()/lstat() ENOENT generation.
 */
static bool
Test_006_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  struct stat buf;
  STEP(EXPECTCALL_STAT_OR_LSTAT, "non-existing-object", &buf, ENOENT);
  STEP(Check_DirEmpty);

  STEP(EXPECTCALL_STAT_OR_LSTAT, "", &buf, ENOENT);
  STEP(Check_DirEmpty);

  /* Only applicable for stat(): dangling symlink.  */
#ifndef TEST_LSTAT_INSTEAD_OF_STAT
  STEP(Create_SymLink, "non-existing-object", "symlink");
  STEP(EXPECTCALL_STAT_OR_LSTAT, "symlink", &buf, ENOENT);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);
#endif

  return false;
}

/**
 * Test stat()/lstat() ENAMETOOLONG error generation.
 */
static bool
Test_007_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  struct stat buf;
  STEP(EXPECTCALL_STAT_OR_LSTAT, longFileName, &buf, ENAMETOOLONG);
  STEP(Check_DirEmpty);

  free (longFileName);
}

/* FIXME: permission tests.  */

#define TESTER_ENTRY(a) \
  { __STRING(a), a }
typedef bool (*Tester)(void);
static const struct
{
  const char *name;
  Tester routine;
} oTesters[] =
{
  TESTER_ENTRY(Test_001_BasicStat),
  TESTER_ENTRY(Test_003_GenerationOfEFAULT),
  TESTER_ENTRY(Test_004_GenerationOfELOOP),
  TESTER_ENTRY(Test_005_GenerationOfENOTDIR),
  TESTER_ENTRY(Test_006_GenerationOfENOENT),
  TESTER_ENTRY(Test_007_GenerationOfENAMETOOLONG),
};

#include "include/main.c"

