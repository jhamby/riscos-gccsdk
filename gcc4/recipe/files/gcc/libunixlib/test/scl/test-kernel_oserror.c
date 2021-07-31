#include <errno.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <swis.h>

/* #define DEBUG */
#ifdef DEBUG
#  define dbg_printf printf
#else
#  define dbg_printf if (0) {} else printf
#endif

int
main (int argc, char *argv[])
{
  /* Test 1 */
  /* Initially there is no last RISC OS error.  */
  _kernel_oserror *err = _kernel_last_oserror ();
  dbg_printf ("Test1a: err %p == NULL.\n", err);
  if (err != NULL)
    abort ();
  /* Nor errno.  */
  dbg_printf ("Test1b : errno %d == 0.\n", errno);
  if (errno != 0)
    abort ();

  /* Test 2 */
  /* Generate a RISC OS error.  */
  int rtrn = _kernel_oscli ("non-existing-oscli-command-or-program");
  dbg_printf ("Test2a: return code %d == _kernel_ERROR (-2).\n", _kernel_ERROR);
  if (rtrn != _kernel_ERROR)
    abort ();
  /* There must now be a last RISC OS error.  */
  err = _kernel_last_oserror ();
  dbg_printf ("Test2b: err %p != NULL (error block: %d, '%s').\n",
              err, err ? err->errnum : 0, err ? err->errmess : "");
  if (err == NULL)
    abort ();
#ifndef __TARGET_SCL__
  /* UnixLib only: errno should be set to EOPSYS.  */
  dbg_printf ("Test2c: errno %d == EOPSYS (%d).\n", errno, EOPSYS);
  if (errno != EOPSYS)
    abort ();
#endif
  /* Query it again, now it should be gone.  */
  err = _kernel_last_oserror ();
  dbg_printf ("Test2d: err %p == NULL (error block: %d, '%s').\n",
              err, err ? err->errnum : 0, err ? err->errmess : "");
  if (err != NULL)
    abort ();
#ifndef __TARGET_SCL__
  /* errno should still be set to EOPSYS.  */
  dbg_printf ("Test2e: errno %d == EOPSYS (%d).\n", errno, EOPSYS);
  if (errno != EOPSYS)
    abort ();
#endif

  /* Test 3 */
#define TEST_ERRNUM 123456
#define TEST_ERRMESS "Specific test error message"
  errno = 0;
  _kernel_oserror testErr = { TEST_ERRNUM, TEST_ERRMESS };
  _kernel_swi_regs regs;
  regs.r[0] = (int) &testErr;
  err = _kernel_swi (OS_GenerateError, &regs, &regs);
  dbg_printf ("Test3a: err %p != NULL, errnum %d == %d, errmess '%s' == '" TEST_ERRMESS "'.\n",
              err,
              err ? err->errnum : 0, TEST_ERRNUM,
              err ? err->errmess : "");
  if (err == NULL
      || err->errnum != TEST_ERRNUM
      || strcmp (err->errmess, TEST_ERRMESS))
    abort ();
  /* Check if last RISC OS error message can be retrieved again once.  */
  err = _kernel_last_oserror ();
  dbg_printf ("Test3b: err %p != NULL, errnum %d == %d, errmess '%s' == '" TEST_ERRMESS "'.\n",
              err,
              err ? err->errnum : 0, TEST_ERRNUM,
              err ? err->errmess : "");
  if (err == NULL
      || err->errnum != TEST_ERRNUM
      || strcmp (err->errmess, TEST_ERRMESS))
    abort ();
#ifndef __TARGET_SCL__
  dbg_printf ("Test3c: errno %d == EOPSYS (%d).\n", errno, EOPSYS);
  if (errno != EOPSYS)
    abort ();
#endif
  /* Query it again, now it should be gone.  */
  err = _kernel_last_oserror ();
  dbg_printf ("Test3d: err %p == NULL (error block: %d, '%s').\n",
              err, err ? err->errnum : 0, err ? err->errmess : "");
  if (err != NULL)
    abort ();
#ifndef __TARGET_SCL__
  dbg_printf ("Test3e: errno %d == EOPSYS (%d).\n", errno, EOPSYS);
  if (errno != EOPSYS)
    abort ();
#endif

  /* We're happy, basic stuff seems to work.  */
  return EXIT_SUCCESS;
}
