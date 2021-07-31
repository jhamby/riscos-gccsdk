/* errno test */

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <kernel.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void
show_errno (bool expectROErr)
{
#ifdef __TARGET_SCL__
  if (errno == -1)
    {
      printf ("errno is -1, no further checking.\n");
      return;
    }
#endif

  char *errstr = strerror (errno);
  assert (errstr != NULL && strcmp (errstr, "Unknown Error"));

  char buf[200];
  if (strerror_r (errno, buf, sizeof (buf)) < 0)
    fprintf (stderr, "FAILED: can't get errno as string (errno is now %d).\n", errno);
  else
    printf ("Current error is: \"%s\" (errno %d).\n", buf, errno);

  /* When we expect a RISC OS error to be the cause of errno, the result
     of strerror_r() should be a longer string than strerror().
     Otherwise, it should be exactly the same.  */
  if (expectROErr)
    assert (strlen (errstr) < strlen (buf));
  else
    assert (!strcmp (errstr, buf));
}

int
main (int argc, char *argv[])
{
#if 0
  for (int i = 0; i < sys_nerr; ++i)
    {
      const char *err1 = strerror (i);
      printf ("errnum %d : %s\n", i, err1);
    }
#endif

  /* Initially we don't have any error.  */
  printf ("*** Test initial errno value.\n");
  assert (errno == 0);
  show_errno (false);

#ifndef __TARGET_SCL__
  printf ("*** Test if open() fails correctly.\n");
  errno = 0;
  int fh1 = open ("BogusFS::NonExistingDisc.$.NonExistingDir.NoFile", O_RDONLY);
  assert (fh1 < 0);
  show_errno (true);
#endif

  /* SCL implementation of fopen() sets errno to -1 on failure.  */
  printf ("*** Test if fopen() fails correctly.\n");
  errno = 0;
  FILE *fh2 = fopen ("BogusFS::NonExistingDisc.$.NonExistingDir.NoFile", "rb");
  assert (fh2 == NULL);
  show_errno (true);

#ifndef __TARGET_SCL__
  printf ("*** Test if chroot() fails correctly.\n");
  errno = 0;
  int rtrn = chroot ("NonExistingDir");
  assert (rtrn < 0);
  /* 'false' because chroot() is not implemented.  */
  show_errno (false);
#endif

  /* SCL implementation of _kernel_swi() does not set errno, nor copies RISC OS
     error block on failure.  */
  printf ("*** Test if _kernel_swi() fails correctly.\n");
  errno = 0;
  _kernel_swi_regs regs;
  _kernel_oserror *err_ro1 = _kernel_swi (-1, &regs, &regs);
  assert (err_ro1 != NULL);
#ifndef __TARGET_SCL__
  show_errno (true);
#else
  show_errno (false);
#endif
  
  printf ("*** Test if _kernel_last_oserror() behaves correctly.\n");
  _kernel_oserror *err_ro2 = _kernel_last_oserror ();
  assert (err_ro2 != NULL && err_ro1->errnum == err_ro2->errnum && !strcmp (err_ro1->errmess, err_ro2->errmess));
  /* A 2nd call to _kernel_last_oserror() should return with NULL.  */
  err_ro2 = _kernel_last_oserror ();
  assert (err_ro2 == NULL);

  return EXIT_FAILURE;
}
