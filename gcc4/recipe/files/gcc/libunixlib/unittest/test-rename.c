/* rename() unittest.
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/expect-rename.c"
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
 * Test rename file '001_src' to '001_dst' (most basic case).
 * Test rename file '001_xxx' to '001_XXX' (i.e. change case).
 * Test rename file '001_foo' to '001_foo' (i.e. no change).
 */
static bool
Test_001_BasicRenameFile (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "001_src");
  STEP(ExpectCall_Rename, "001_src", "001_dst", 0);
  static const char * const fnames1[] = { "001_dst" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "001_src");
  STEP(ExpectCall_Rename, "./001_src", "./001_dst", 0);
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "001_xxx");
  STEP(ExpectCall_Rename, "001_xxx", "001_XXX", 0);
  static const char * const fnames2[] = { "001_XXX" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "001_foo");
  STEP(ExpectCall_Rename, "001_foo", "001_foo", 0);
  static const char * const fnames3[] = { "001_foo" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename directory '002_src' to '002_dst' (most basic case).
 * Test rename directory '002_xxx' to '002_XXX' (i.e. change case).
 * Test rename directory '002_foo' to '002_foo' (i.e. no change).
 */
static bool
Test_002_BasicRenameDir (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "002_src");
  STEP(ExpectCall_Rename, "002_src", "002_dst", 0);
  static const char * const fnames1[] = { "002_dst" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "002_src");
  STEP(ExpectCall_Rename, "./002_src", "./002_dst", 0);
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "002_xxx");
  STEP(ExpectCall_Rename, "002_xxx", "002_XXX", 0);
  static const char * const fnames2[] = { "002_XXX" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "002_foo");
  STEP(ExpectCall_Rename, "002_foo", "002_foo", 0);
  static const char * const fnames3[] = { "002_foo" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename symlink '001_src' to '001_dst' (most basic case).
 * Test rename symlink '001_xxx' to '001_XXX' (i.e. change case).
 * Test rename symlink '001_foo' to '001_foo' (i.e. no change).
 */
static bool
Test_003_BasicRenameSymLink (void)
{
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-object", "003_src");
  STEP(ExpectCall_Rename, "003_src", "003_dst", 0);
  static const char * const fnames1[] = { "003_dst" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-object", "003_src");
  STEP(ExpectCall_Rename, "./003_src", "./003_dst", 0);
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-object", "003_xxx");
  STEP(ExpectCall_Rename, "003_xxx", "003_XXX", 0);
  static const char * const fnames2[] = { "003_XXX" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(SymLink, "non-existing-object", "003_foo");
  STEP(ExpectCall_Rename, "003_foo", "003_foo", 0);
  static const char * const fnames3[] = { "003_foo" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* rename() shouldn't generate ELOOP (or some other error) when we have
     symlink loop.  */
  STEP(SymLink, "symlink2", "symlink1");
  STEP(SymLink, "symlink3", "symlink2");
  STEP(SymLink, "symlink1", "symlink3");
  STEP(ExpectCall_Rename, "symlink1", "foo", 0);
  STEP(ExpectCall_Rename, "foo", "symlink1", 0);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename file '003_src' to '003_dst' with '003_dst' preexisting.
 */
static bool
Test_004_RenameFileAndDstExists (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "004_src");
  STEP(Create_File, "004_dst");
  STEP(ExpectCall_Rename, "004_src", "004_dst", 0);
  static const char *fnames[] = { "004_dst" };
  STEP(Check_DirContents, fnames, sizeof (fnames)/sizeof (fnames[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename dir '004_src' to '004_dst' with '004_dst' preexisting.
 */
static bool
Test_005_RenameDirAndDstExists (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "005_src");
  STEP(Create_Dir, "005_dst");
  STEP(ExpectCall_Rename, "005_src", "005_dst", 0);
  static const char *fnames[] = { "005_dst" };
  STEP(Check_DirContents, fnames, sizeof (fnames)/sizeof (fnames[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename file '005_file' to existing '005_dir' -> EISDIR.
 */
static bool
Test_006_GenerationOfEISDIR (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "006_file");
  STEP(Create_Dir, "006_dir");
  STEP(ExpectCall_Rename, "006_file", "006_dir", EISDIR);
  static const char *fnames[] = { "006_file", "006_dir" };
  STEP(Check_DirContents, fnames, sizeof (fnames)/sizeof (fnames[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename() ENOENT error generation.
 *  - src does not exist.
 *  - directory component of dst does not exist.
 *  - src or dst is empty string.
 */
static bool
Test_007_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "dst");
  STEP(ExpectCall_Rename, "nofile", "dst", ENOENT);
  static const char *fnames1[] = { "dst" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file");
  STEP(Create_Dir, "dir");
  STEP(ExpectCall_Rename, "file", "dir/nodir/foo", ENOENT);
  static const char *fnames2[] = { "file", "dir" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(ExpectCall_Rename, "", "foo", ENOENT);
  STEP(Check_DirEmpty);

  STEP(Create_File, "bar");
  STEP(ExpectCall_Rename, "bar", "", ENOENT);
  static const char *fnames3[] = { "bar" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "foo,cde");
  STEP(Check_ROFileType, "foo,cde", 0xcde);
  STEP(ExpectCall_Rename, "foo,fff", "bar", ENOENT);
  static const char *fnames4[] = { "foo,cde" };
  STEP(Check_DirContents, fnames4, sizeof (fnames4)/sizeof (fnames4[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename() ENOTDIR error generation.
 *  - directory component in src or dst is not a directory.
 *  - src is a directory but dst is a file.
 */
static bool
Test_008_GenerationOfENOTDIR (void)
{
  STEP(Check_DirEmpty);

  /* FIXME: this ENOTDIR case is not yet properly supported in UnixLib.  */
  STEP(Create_File, "file");
  STEP(ExpectCall_Rename, "file/nofile", "other_file", ENOTDIR);
  static const char *fnames1[] = { "file" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "file1");
  STEP(Create_File, "file2");
  STEP(ExpectCall_Rename, "file1", "file2/nofile", ENOTDIR);
  static const char *fnames2[] = { "file1", "file2" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "dir");
  STEP(Create_File, "file");
  STEP(ExpectCall_Rename, "dir", "file", ENOTDIR);
  static const char *fnames3[] = { "file", "dir" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename() EFAULT error generation.
 *   - src or dst being an illegal address.
 */
static bool
Test_009_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  STEP(ExpectCall_Rename, NULL, "foo", EFAULT);
  STEP(Check_DirEmpty);

  STEP(Create_File, "bar");
  STEP(ExpectCall_Rename, "bar", NULL, EFAULT);
  static const char *fnames1[] = { "bar" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test rename() EINVAL error generation.
 *   - src being a parent dir of dst.
 */
static bool
Test_010_GenerationOfEINVAL (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "dir");
  STEP(Create_Dir, "dir/subdir");
  STEP(ExpectCall_Rename, "dir", "dir/subdir/foo", EINVAL);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_Dir, "dir");
  STEP(Create_Dir, "dir/subdir");
  STEP(ExpectCall_Rename, "dir", "dir/subdir", EINVAL);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

#ifdef __riscos__
  /* On non-RISC OS, this rather gives EBUSY.  */
  STEP(Create_Dir, "dir");
  STEP(ExpectCall_Rename, ".", "dir", EINVAL);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);
#endif

  return false;
}

/**
 * Test rename() ENAMETOOLONG error generation.
 */
static bool
Test_011_GenerationOfENAMETOOLONG (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "file");
  /* Assuming RISC OS isn't supporting 32K long filenames.  */
  char *longFileName = malloc (32*1024);
  for (size_t i = 0; i != 32*1024; ++i)
    longFileName[i] = '0' + (i % 10);
  STEP(ExpectCall_Rename, "file", longFileName, ENAMETOOLONG);
  static const char *fnames1[] = { "file" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  free (longFileName);

  return false;
}

/**
 * Test rename() with ,xzy suffixes.  That could involve RISC OS filetype
 * changes.
 */
static bool
Test_012_RenameWithFiletypeSuffix (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "foo");
  STEP(Check_ROFileType, "foo", 0xfff);
  STEP(ExpectCall_Rename, "foo", "foo,fff", 0);
#ifdef __riscos
  /* On RISC OS : foo,fff and foo are the same file.  */
  static const char *fnames1[] = { "foo" };
#else
  static const char *fnames1[] = { "foo,fff" };
#endif
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(Check_ROFileType, "foo", 0xfff);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "foo,cde");
  STEP(Check_ROFileType, "foo,cde", 0xcde);
  STEP(ExpectCall_Rename, "foo,cde", "foo,345", 0); /* Filetype change.  */
  static const char *fnames2[] = { "foo,345" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(Check_ROFileType, "foo,345", 0x345);
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Filetype extension to src directory does not confuse rename().  */
  STEP(Create_Dir, "dir");
  STEP(ExpectCall_Rename, "dir,123", "dir2", ENOENT);
  static const char *fnames3[] = { "dir" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

#ifdef __riscos__
  /* Filetype extension on dst directory does not confuse rename() [part 1].  */
  STEP(Create_Dir, "dir");
  STEP(ExpectCall_Rename, "dir", "dir,123", ENOTDIR); /* Same directory name.  */
  static const char *fnames4[] = { "dir" };
  STEP(Check_DirContents, fnames4, sizeof (fnames4)/sizeof (fnames4[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  /* Filetype extension on dst directory does not confuse rename() [part 2].  */
  STEP(Create_Dir, "dir");
  STEP(ExpectCall_Rename, "dir", "dir2,123", ENOTDIR); /* Another directory name.  */
  static const char *fnames5[] = { "dir" };
  STEP(Check_DirContents, fnames5, sizeof (fnames5)/sizeof (fnames5[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);
#endif

  return false;
}

static bool
Test_013_RenameWithExtension (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "foo");
  STEP(ExpectCall_Rename, "foo", "foo.c", 0);
  static const char *fnames1[] = { "foo.c" };
  STEP(Check_DirContents, fnames1, sizeof (fnames1)/sizeof (fnames1[0]));
  STEP(ExpectCall_Rename, "foo.c", "foo.s", 0);
  static const char *fnames2[] = { "foo.s" };
  STEP(Check_DirContents, fnames2, sizeof (fnames2)/sizeof (fnames2[0]));
  STEP(ExpectCall_Rename, "foo.s", "foo", 0);
  static const char *fnames3[] = { "foo" };
  STEP(Check_DirContents, fnames3, sizeof (fnames3)/sizeof (fnames3[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  STEP(Create_File, "foo1");
  STEP(Create_File, "foo2");
  STEP(ExpectCall_Rename, "foo1", "foo1.c", 0);
  STEP(ExpectCall_Rename, "foo2", "foo2.c", 0);
  static const char *fnames21[] = { "foo1.c", "foo2.c" };
  STEP(Check_DirContents, fnames21, sizeof (fnames21)/sizeof (fnames21[0]));
  STEP(ExpectCall_Rename, "foo1.c", "foo", 0);
  static const char *fnames22[] = { "foo", "foo2.c" };
  STEP(Check_DirContents, fnames22, sizeof (fnames22)/sizeof (fnames22[0]));
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

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
  TESTER_ENTRY(Test_001_BasicRenameFile),
  TESTER_ENTRY(Test_002_BasicRenameDir),
  TESTER_ENTRY(Test_003_BasicRenameSymLink),
  TESTER_ENTRY(Test_004_RenameFileAndDstExists),
  TESTER_ENTRY(Test_005_RenameDirAndDstExists),
  TESTER_ENTRY(Test_006_GenerationOfEISDIR),
  TESTER_ENTRY(Test_007_GenerationOfENOENT),
  TESTER_ENTRY(Test_008_GenerationOfENOTDIR),
  TESTER_ENTRY(Test_009_GenerationOfEFAULT),
  TESTER_ENTRY(Test_010_GenerationOfEINVAL),
  TESTER_ENTRY(Test_011_GenerationOfENAMETOOLONG),
  TESTER_ENTRY(Test_012_RenameWithFiletypeSuffix),
  TESTER_ENTRY(Test_013_RenameWithExtension),
};

#include "include/main.c"

