/* fopen() unittest.
   Copyright (c) 2013 UnixLib Developers.
  
   Written by John Tytgat / BASS.  */

#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>

#include "include/expect-fclose.c"
#include "include/expect-fopen.c"
#include "include/expect-fgetc.c"
#include "include/expect-fputc.c"
#include "include/expect-fseek.c"
#include "include/expect-feof.c"
#include "include/check.c"

static bool
Create_FileWithSize (const char *fname, size_t size)
{
  int fh = creat (fname, S_IWUSR | S_IRUSR);
  if (fh == -1)
    {
      fprintf (stderr, "creat(%s) failed: %s (errno %d)\n", fname, strerror (errno), errno);
      return true;
    }
  while (size)
    {
      size_t toWrite = size > 26 ? 26 : size;
      if (write(fh, "abcdefghijklmnopqrstuvwxyz", toWrite) != toWrite)
        {
          fprintf (stderr, "write() failed to write all bytes: %s (errno %d)\n",
                   strerror (errno), errno);
          close (fh);
          return true;
        }
      size -= toWrite;
    }
  if (close (fh) != 0)
    {
      fprintf (stderr, "close() failed for handle obtained by creat(%s): %s (errno %d)\n",
               fname, strerror (errno), errno);
      return true;
    }
  return false;
}

static bool
Check_FileContents (const char *fname, const char *contents)
{
  size_t expectedFileSize = strlen (contents);
  if (Check_FileSize (fname, expectedFileSize))
    return true;

  char *buf = malloc (expectedFileSize);
  if (buf == NULL)
    {
      fprintf (stderr, "malloc() failed when claiming %zu bytes\n",
               expectedFileSize);
      return true;
    }
  int fd = open (fname, O_RDONLY);
  if (fd == -1)
    {
      fprintf (stderr, "open(%s) failed with errno %d (%s)\n",
               fname, errno, strerror (errno));
      free (buf);
      return true;
    }
  ssize_t readBytes = read (fd, buf, expectedFileSize);
  if (readBytes != expectedFileSize)
    {
      /* FIXME: not 100% ok */
      fprintf (stderr, "read() requested %zu bytes to read, got %zu with errno %d %s\n",
               expectedFileSize, readBytes, errno, strerror (errno));
      close (fd);
      free (buf);
      return true;
    }
  if (close (fd) != 0)
    {
      fprintf (stderr, "close() failed, errno %d %s\n",
               errno, strerror (errno));
      free (buf);
    }
  if (memcmp (buf, contents, expectedFileSize))
    {
      fprintf (stderr, "File %s has contents '%.*s' while we expect '%s'\n",
               fname, (int)expectedFileSize, buf, contents);
      free (buf);
      return true;
    }
  free (buf);
  return false;
}

static bool gVerbose = false;
#ifdef __riscos__
static const bool gOnRISCOS = true;
#else
static const bool gOnRISCOS = false;
#endif

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

#define STEP_CONDFAIL(condition, call,...) \
  do \
    { \
      if (condition) \
        { \
          if (gVerbose) \
            fprintf (stderr, "  call " __STRING(call) "\n"); \
          if (call(__VA_ARGS__)) \
            { \
              fprintf (stderr, "  *** " __STRING(call) " failed at line %d !\n", __LINE__); \
              fprintf (stderr, "  *** However, this is known to fail for " __STRING(condition) " so you can ignore this\n"); \
            } \
        } \
      else \
        STEP(call, __VA_ARGS__); \
   } while (0)

#define INITIAL_FILESIZE 15
const struct ValidModeTest
{
  const char *mode;
  struct Result
  {
    int errFOpen; /* Expected errno for fopen().  */
    int errFGetC; /* Expected errno for fgetc().  -1 when not relevant.  */
    int errFPutC; /* Expected errno for fputc().  -1 when not relevant.  */
    int byteRead; /* Expected byte read.  Use -1 if not such test.  */
    off_t fileSize; /* WHen file exists, expected size.  */
  } results[6];
} oValidModeTests[] =
{
  { "r",
    {
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, 0, -1, 'a', INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, EBADF, -1, INITIAL_FILESIZE } /* Pre-existing file. Do fputc().  */
    }
  },
  { "r+",
    {
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, 0, -1, 'a', INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, INITIAL_FILESIZE } /* Pre-existing file. Do fputc().  */
    }
  },
  { "rb+",
    {
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { ENOENT, -1, -1, -1, 0 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, 0, -1, 'a', INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, INITIAL_FILESIZE } /* Pre-existing file. Do fputc().  */
    }
  },
  { "w",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, EBADF, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, 0 }, /* Pre-existing file.  */
      { 0, EBADF, -1, -1, 0 }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 } /* Pre-existing file. Do fputc().  */
    }
  },
  { "w+",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, 0 }, /* Pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 } /* Pre-existing file. Do fputc().  */
    }
  },
  { "wb+",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, 0 }, /* Pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 } /* Pre-existing file. Do fputc().  */
    }
  },
  { "a",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, EBADF, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, EBADF, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, INITIAL_FILESIZE + 1 } /* Pre-existing file. Do fputc().  */
    }
  },
  { "a+",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, 0, -1, 'a', INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, INITIAL_FILESIZE + 1 } /* Pre-existing file. Do fputc().  */
    }
  },
  { "ab+",
    {
      { 0, -1, -1, -1, 0 }, /* No pre-existing file.  */
      { 0, 0, -1, -1, 0 }, /* No pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, 1 }, /* No pre-existing file. Do fputc().  */
      { 0, -1, -1, -1, INITIAL_FILESIZE }, /* Pre-existing file.  */
      { 0, 0, -1, 'a', INITIAL_FILESIZE }, /* Pre-existing file. Do fgetc().  */
      { 0, -1, 0, -1, INITIAL_FILESIZE + 1 } /* Pre-existing file. Do fputc().  */
    }
  },
};

static bool
Test_001_BasicFileOpen (void)
{
  STEP(Check_DirEmpty);

  for (size_t idx = 0; idx != sizeof (oValidModeTests)/sizeof (oValidModeTests[0]); ++idx)
    {
      for (size_t subIdx = 0; subIdx != 6; ++subIdx)
        {
          if (gVerbose)
            fprintf (stderr, "  Mode test %u \"%s\", sub test %u\n",
                     (unsigned)idx, oValidModeTests[idx].mode, (unsigned)subIdx);

          bool createFile = subIdx >= 3;
          bool readByte = (subIdx % 3) == 1;
          bool writeByte = (subIdx % 3) == 2;

	  STEP(Check_DirEmpty);
          if (createFile)
            Create_FileWithSize ("foo", INITIAL_FILESIZE);
          FILE *fh;
          STEP(ExpectCall_FOpen, &fh, "foo", oValidModeTests[idx].mode, oValidModeTests[idx].results[subIdx].errFOpen);
          if (fh != NULL)
            {
              if (readByte)
                STEP(ExpectCall_FGetC, fh, oValidModeTests[idx].results[subIdx].byteRead, oValidModeTests[idx].results[subIdx].errFGetC);
              if (writeByte)
                STEP(ExpectCall_FPutC, '@', fh, oValidModeTests[idx].results[subIdx].errFPutC);
              STEP(ExpectCall_FClose, fh, 0);
              STEP(Check_FileSize, "foo", oValidModeTests[idx].results[subIdx].fileSize);
            }
          STEP(Clean_CurDir);
        }
    }
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test fopen() when directory path is used.  When writable mode is used,
 * EISDIR needs to be returned.
 */
static bool
Test_002_BasicDirOpen (void)
{
  STEP(Check_DirEmpty);

  const struct FOpenDirTest
    {
      const char *mode;
      bool success;
    } oFOpenDirTest[] =
    {
      { "r", true }, { "r+", false },
      { "w", false }, { "w+", false },
      { "a", false }, { "a+", false }
    };
  STEP(Create_Dir, "dir");
  for (size_t idx = 0; idx != sizeof (oFOpenDirTest)/sizeof (oFOpenDirTest[0]); ++idx)
    {
      FILE *fh;
      STEP(ExpectCall_FOpen, &fh, "dir", oFOpenDirTest[idx].mode, oFOpenDirTest[idx].success ? 0 : EISDIR);
      if (oFOpenDirTest[idx].success)
        STEP(ExpectCall_FClose, fh, 0);
    }
  STEP(Clean_CurDir);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test fopen() append specifically.  Any write operation needs to do an
 * implicit seek to EOF first.
 */
static bool
Test_003_FOpenAppend (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_FileWithSize, "file", 16);
  FILE *fh;
  STEP(ExpectCall_FOpen, &fh, "file", "a+", 0);
  /* File pos at the begin file.  */
  STEP(ExpectCall_FGetC, fh, 'a', 0);
  STEP(ExpectCall_FGetC, fh, 'b', 0);

  STEP(ExpectCall_FPutC, '@', fh, 0);

  /* read op after write needs fseek in between.
     write op after read needs fseek when read didn't return EOF.  */
  STEP(ExpectCall_FSeek, fh, 0, SEEK_CUR, 0);

  STEP(ExpectCall_FGetC, fh, EOF, 0);
  STEP(ExpectCall_FEOF, fh, true);
  /* Do not clear feof ourselves, fseek should do that.  */

  STEP(ExpectCall_FSeek, fh, 4, SEEK_SET, 0);
  STEP(ExpectCall_FEOF, fh, false);

  STEP(ExpectCall_FPutC, '1', fh, 0);
  STEP(ExpectCall_FClose, fh, 0);
  STEP(Check_FileContents, "file", "abcdefghijklmnop@1");
  STEP(Clean_CurDir);

  STEP(Check_DirEmpty);
  return false;
}

/**
 * Open same file twice, once for read, once for write.
 * This is not supported on all RISC OS FS (known to fail for FileCase based FS,
 * but works for Sunfish (NFS)).
 */
static bool
Test_004_SameFileOpenTwice (void)
{
  STEP(Check_DirEmpty);

  STEP(Create_File, "file");
  FILE *fhWr;
  STEP(ExpectCall_FOpen, &fhWr, "file", "w", 0);
  FILE *fhRd;
  STEP_CONDFAIL(gOnRISCOS, ExpectCall_FOpen, &fhRd, "file", "r", 0);
  if (fhRd != NULL)
    STEP(ExpectCall_FClose, fhRd, 0);
  STEP(ExpectCall_FClose, fhWr, 0);
  STEP(Clean_CurDir);

  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test fopen() EINVAL error generation.
 *   - mode description is not correct.
 */
static bool
Test_005_GenerationOfEINVAL (void)
{
  STEP(Check_DirEmpty);

  const char * const invalidMode[] =
    {
      "z"
    };
  for (size_t idx = 0; idx != sizeof (invalidMode)/sizeof (invalidMode[0]); ++idx)
    {
      FILE *fh;
      STEP(ExpectCall_FOpen, &fh, "foo", invalidMode[idx], EINVAL);
    }
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test fopen() EFAULT error generation.
 *   - fname being NULL (mode being NULL is not required to return EFAULT).
 */
static bool
Test_006_GenerationOfEFAULT (void)
{
  STEP(Check_DirEmpty);

  FILE *fh;
  STEP(ExpectCall_FOpen, &fh, NULL, "w", EFAULT);
  STEP(Check_DirEmpty);

  return false;
}

/**
 * Test fopen() ENOENT error generation.
 *   - fname being an empty string.
 */
static bool
Test_007_GenerationOfENOENT (void)
{
  STEP(Check_DirEmpty);

  FILE *fh;
  STEP(ExpectCall_FOpen, &fh, "", "w+", ENOENT);
  STEP(Check_DirEmpty);

  return false;
}

/* FIXME: Add tests:
     - symlink test
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
  TESTER_ENTRY(Test_001_BasicFileOpen),
  TESTER_ENTRY(Test_002_BasicDirOpen),
  TESTER_ENTRY(Test_003_FOpenAppend),
  TESTER_ENTRY(Test_004_SameFileOpenTwice),
  TESTER_ENTRY(Test_005_GenerationOfEINVAL),
  TESTER_ENTRY(Test_006_GenerationOfEFAULT),
  TESTER_ENTRY(Test_007_GenerationOfENOENT),
};

#include "include/main.c"

