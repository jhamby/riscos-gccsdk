/* check.c
   Copyright (c) 2013 UnixLib Developers.

   Collection of DirContents_*(), Check_*(), Clean_*() and Create_*() routines
   which can be used in unittests.
  
   Written by John Tytgat / BASS.  */

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __riscos__
# include <kernel.h>
# include <unixlib/local.h>
#endif

typedef struct
{
  struct stat stat;
  const char *name;
} ObjInfo_t;

typedef struct
{
  size_t numEntries;
  size_t maxNumObjInfos;
  ObjInfo_t *objInfos;
} DirContents_t;

static bool
DirContents_Get (const char *dir, DirContents_t *dirContents)
{
  dirContents->numEntries = 0;
  dirContents->maxNumObjInfos = 0;
  dirContents->objInfos = NULL;

  char tmpPath[_POSIX_PATH_MAX];
  size_t dirLen = strlen (dir);
  memcpy (tmpPath, dir, dirLen);
  tmpPath[dirLen++] = '/';

  DIR *dhandle = opendir (dir);
  if (dhandle == NULL)
    {
      fprintf (stderr, "opendir(%s) failed : %s (errno %d)\n", dir, strerror (errno), errno);
      return true;
    }
  struct dirent *dirEntry;
  while ((dirEntry = readdir (dhandle)) != NULL)
    {
      /* Filter out "." and "..".  */
      if (!strcmp (dirEntry->d_name, ".") || !strcmp (dirEntry->d_name, ".."))
        continue;
      if (dirContents->numEntries == dirContents->maxNumObjInfos)
        {
          size_t newMaxNumObjInfos = (dirContents->maxNumObjInfos + 3)*2;
          ObjInfo_t *newObjInfos = realloc (dirContents->objInfos, sizeof (ObjInfo_t)*newMaxNumObjInfos);
          if (newObjInfos == NULL)
            {
              fprintf (stderr, "realloc() failed\n");
              closedir (dhandle);
              return true;
            }
          dirContents->maxNumObjInfos = newMaxNumObjInfos;
          dirContents->objInfos = newObjInfos;
        }
      strcpy (tmpPath + dirLen, dirEntry->d_name);
      if (stat (tmpPath, &dirContents->objInfos[dirContents->numEntries].stat) != 0)
        {
          fprintf (stderr, "stat() failed for %s\n", tmpPath);
          closedir(dhandle);
          return true;
        }
      dirContents->objInfos[dirContents->numEntries].name = strdup (dirEntry->d_name);
      dirContents->numEntries++;
    }
  closedir (dhandle);

  return false;
}

static void
DirContents_Print (const DirContents_t *dirContents)
{
  for (size_t i = 0; i != dirContents->numEntries; ++i)
    printf ("%s\n", dirContents->objInfos[i].name);
}

static void
DirContents_Free (DirContents_t *dirContents)
{
  for (size_t i = 0; i != dirContents->numEntries; ++i)
    free ((void *)dirContents->objInfos[i].name);
  free (dirContents->objInfos);
  memset (dirContents, 0, sizeof (DirContents_t));
}

/**
 * Checks if current directory is empty.
 * \return true in case of failure.
 */
static bool
Check_DirEmpty (void)
{
  DirContents_t dirContents;
  if (DirContents_Get (".", &dirContents))
    return true;
  bool rtrn = dirContents.numEntries != 0;
  DirContents_Free (&dirContents);
  return rtrn;
}

/**
 * Compares the contents of the current directory with give list of expected
 * files and subdirectories.
 * \return true when there is a mismatched with expected list of files and
 * subdirectories.
 */
static bool
Check_DirContents (const char * const fnames[], size_t numEntries)
{
  bool rtrn = false;
  DirContents_t dirContents;
  if (DirContents_Get (".", &dirContents))
    return true;
  for (size_t i = 0; i != dirContents.numEntries; ++i)
    {
      size_t j;
      for (j = 0; j != numEntries && strcmp (dirContents.objInfos[i].name, fnames[j]); ++j)
        /* */;
      if (j == numEntries)
        {
          fprintf (stderr, "Found '%s' and was unexpected\n", dirContents.objInfos[i].name);
          rtrn = true;
        }
    }
  for (size_t i = 0; i != numEntries; ++i)
    {
      size_t j;
      for (j = 0; j != dirContents.numEntries && strcmp (fnames[i], dirContents.objInfos[j].name); ++j)
        /* */;
      if (j == dirContents.numEntries)
        {
          fprintf (stderr, "Expected '%s' but was not found\n", fnames[i]);
          rtrn = true;
        }
    }
  DirContents_Free (&dirContents);
  return rtrn;
}

/**
 * Given an Unix path fpath, check on RISC OS if it has the required filetype.
 * \return true in case of failure.
 */
static bool
Check_ROFileType (const char *fpath, unsigned roftype)
{
#ifdef __riscos__
  int roftype_fpath; /* RISC OS filetype derived from Unix path fpath.  */
  char rofpath[_POSIX_PATH_MAX];
  if (!__riscosify_std (fpath, 0, rofpath, sizeof (rofpath), &roftype_fpath))
    {
      fprintf (stderr, "Check_ROFileType: __riscosify_std(%s) failed, buffer was too small ?\n",
               fpath);
      return true;
    }
  /* When filetype is not specified in fpath, we assume &fff (FIXME: not ok for directory, imagefs).  */
  if (roftype_fpath == -1)
    roftype_fpath = 0xFFF;
  if (roftype_fpath != roftype)
    {
      fprintf (stderr, "Check_ROFileType(%s): given Unix path indicatea a different RISC OS filetype (&%x) than expected (&%x)\n",
               fpath, roftype_fpath, roftype);
      return true;
    }
  /* Fetch filetype directly.  */
  _kernel_swi_regs regs;
  regs.r[0] = 23;
  regs.r[1] = (int)rofpath;
  const _kernel_oserror *err;
  if ((err = _kernel_swi (8 /* OS_File */, &regs, &regs)) != NULL)
    {
      fprintf (stderr, "Check_ROFileType(%s): _kernel_swi(OS_File, %s) failed: 0x%x %s\n",
               fpath, rofpath, err->errnum, err->errmess);
      return true;
    }
  if (regs.r[0] == 0)
    {
      fprintf (stderr, "Check_ROFileType(%s): _kernel_swi(OS_File, %s): no object\n",
               fpath, rofpath);
      return true;
    }
  if (regs.r[6] != roftype)
    {
      fprintf (stderr, "Check_ROFileType(%s) expected RISC OS filetype &%x but we found &%x\n",
               fpath, roftype, regs.r[6]);
      return true;
    }
  return false;
#else
  return false;
#endif
}

static bool
Check_FileSize (const char *fname, off_t fsize)
{
  struct stat statBuf;
  if (stat (fname, &statBuf) != 0)
    {
      fprintf (stderr, "stat(%s) failed with errno %d (%s)\n",
               fname, errno, strerror (errno));
      return true;
    }
  if (statBuf.st_size != fsize)
    {
      fprintf (stderr, "File %s is %u bytes long, but we expected %u bytes.\n",
               fname, (unsigned)statBuf.st_size, (unsigned)fsize);
      return true;
    }
  return false;
}


static bool
Check_FileModDate (const char *fname, time_t mtime_sec, unsigned mtime_csec)
{
  struct stat statBuf;
  if (stat (fname, &statBuf) != 0)
    {
      fprintf (stderr, "stat(%s) failed with errno %d (%s)\n",
               fname, errno, strerror (errno));
      return true;
    }
  if (statBuf.st_mtim.tv_sec != mtime_sec
      || abs (statBuf.st_mtim.tv_nsec - mtime_csec * 10000000) >= 10000000)
    {
      char buf1[64];
      if (ctime_r (&statBuf.st_mtim.tv_sec, buf1))
	buf1[strlen (buf1) - 1] = '\0';
      else
	buf1[0] = '\0';
      char buf2[64];
      if (ctime_r (&mtime_sec, buf2))
	buf2[strlen (buf2) - 1] = '\0';
      else
	buf2[0] = '\0';
      fprintf (stderr, "File %s has modification date %u.%09u (0x%08x 0x%08x) '%s', but we expected %u.%09u (0x%08x 0x%08x) '%s'.\n",
               fname,
               (unsigned)statBuf.st_mtim.tv_sec, (unsigned)statBuf.st_mtim.tv_nsec,
               (unsigned)statBuf.st_mtim.tv_sec, (unsigned)statBuf.st_mtim.tv_nsec,
               buf1,
               (unsigned)mtime_sec, (unsigned)mtime_csec * 10000000,
               (unsigned)mtime_sec, (unsigned)mtime_csec * 10000000,
               buf2);
      return true;
    }
  return false;
}

/**
 * Removes all files and subdirectories from the given directory.
 * \return true in case of failure.
 */
static bool
Clean_Dir (const char *dir)
{
  char tmpPath[_POSIX_PATH_MAX];
  size_t dirLen = strlen (dir);
  memcpy (tmpPath, dir, dirLen);
  tmpPath[dirLen++] = '/';

  DirContents_t dirContents;
  if (DirContents_Get (dir, &dirContents))
    return true;
  bool rtrn = false;
  for (size_t i = 0; i != dirContents.numEntries; ++i)
    {
      strcpy (tmpPath + dirLen, dirContents.objInfos[i].name);
      if (S_ISDIR (dirContents.objInfos[i].stat.st_mode))
        {
          if (Clean_Dir (tmpPath))
            rtrn = true;
        }
      if (remove (tmpPath) != 0)
        {
          fprintf (stderr, "remove(%s) failed : %s (errno %d)\n", tmpPath, strerror (errno), errno);
          rtrn = true;
        }
    }
  DirContents_Free (&dirContents);
  return rtrn;
}

/**
 * Removes all files and subdirectories from the current directory.
 * \return true in case of failure.
 */
static bool
Clean_CurDir (void)
{
  return Clean_Dir (".");
}

/**
 * Creates file.
 * \return true in case of failure.
 */
static bool
Create_File (const char *fname)
{
  int fh = creat (fname, S_IWUSR | S_IRUSR);
  if (fh == -1)
    {
      fprintf (stderr, "creat(%s) failed: %s (errno %d)\n", fname, strerror (errno), errno);
      return true;
    }
  if (close (fh) != 0)
    {
      fprintf (stderr, "close() failed for handle obtained by creat(%s): %s (errno %d)\n",
               fname, strerror (errno), errno);
      return true;
    }
  return false;
}

/**
 * Creates directory.
 * \return true in case of failure.
 */
static bool
Create_Dir (const char *dname)
{
  if (mkdir (dname, S_IWUSR | S_IRUSR | S_IXUSR) != 0)
    {
      fprintf (stderr, "mkdir(%s) failed: %s (errno %d)\n", dname, strerror (errno), errno);
      return true;
    }
  return false;
}

