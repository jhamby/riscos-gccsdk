/* Unittest expect call for realpath().
   Copyright (c) 2013 UnixLib Developers.

   Written by John Tytgat / BASS.  */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Calls realpath() for testing purposes.
 */
static bool
Call_RealPath (const char *path, char *resolved_path, char **result)
{
  errno = -1;
  *result = realpath (path, resolved_path);
  if (*result != NULL && errno != -1)
    {
      fprintf (stderr, "realpath(%s, %s) returned non-NULL value %p '%s' but errno changed to %d '%s'\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", *result, *result,
	       errno, strerror (errno));
      return true;
    }
  if (*result == NULL && errno == -1)
    {
      fprintf (stderr, "realpath(%s, %s) returned NULL value but errno did not change\n",
               path ? path : "NULL", resolved_path ? resolved_path : "NULL");
      return true;
    }
  if (resolved_path != NULL && *result != NULL && *result != resolved_path)
    {
      fprintf (stderr, "realpath(%s, %s) returned a non-NULL value %p '%s' which is different than given non-NULL buffer %p\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", *result, *result,
	       resolved_path);
      return true;
    }
  return false;
}


static bool
ExpectCall_RealPath (const char *path, char *resolved_path, const char *testdir,
		     const char *exp_rtrn, int exp_errno)
{
  assert ((exp_rtrn != NULL && exp_errno == 0) || (exp_rtrn == NULL && exp_errno != 0));

  char *rtrn;
  if (Call_RealPath (path, resolved_path, &rtrn))
    return true;
  if (rtrn != NULL
      && exp_rtrn != NULL
      && !(!strncmp (rtrn, testdir, strlen (testdir))
           && (exp_rtrn[0] == '\0'
               || (exp_rtrn[0] != '\0' && rtrn[strlen (testdir)] == '/' && !strcmp (rtrn + strlen (testdir) + 1, exp_rtrn)))))
    {
      fprintf (stderr, "realpath(%s, %s) returned a non-NULL value %p '%s' which doesn't match expected result '%s/%s'\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", rtrn, rtrn, testdir, exp_rtrn);
      return true;
    }
  if (rtrn != NULL && exp_rtrn == NULL)
    {
      fprintf (stderr, "realpath(%s, %s) return a non-NULL value %p '%s' but we expected error %d (%s) instead\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", rtrn, rtrn, exp_errno, strerror (exp_errno));
      return true;
    }
  if (rtrn == NULL && exp_rtrn != NULL)
    {
      fprintf (stderr, "realpath(%s, %s) return a NULL value (errno %d '%s') but we expected '%s' as result\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", errno, strerror (errno), exp_rtrn);
      return true;
    }
  if (rtrn == NULL && errno != exp_errno)
    {
      fprintf (stderr, "realpath(%s, %s) returned a NULL value (errno %d '%s') but we expected errno %d '%s' instead\n",
	       path ? path : "NULL", resolved_path ? resolved_path : "NULL", errno, strerror (errno), exp_errno, strerror (exp_errno));
      return true;
    }
  if (resolved_path == NULL)
    free (rtrn);
  return false;
}


/**
 * Can be used in non-realpath() related tests.
 * \param result needs to be free()'d.
 */
bool
RealPath (const char *path, char **result)
{
  return Call_RealPath (path, NULL, result);
}

