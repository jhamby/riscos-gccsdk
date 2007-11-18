/* putenv ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <unixlib/unix.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

/* Add NAME=VALUE to the environment. If NAME is already in the environment,
   only add when replace is non-zero.  If string is non-NULL then it is
   used directly, otherwise a copy of name and value is used. */
int
__addenv_to_env (char *string, const char *name, const char *value, int replace)
{
  char **ep = NULL;
  size_t envcnt = 0;
  size_t namelen;
  size_t valuelen;

  PTHREAD_UNSAFE

#ifdef DEBUG
  debug_printf ("-- __add_to_env: %s %s, environ=%p, __last_environ=%p\n",
		string ? string : name, string ? "" : value, environ,
		__last_environ);
#endif

  if (string)
    {
      char *equals;

      equals = strchr (string, '=');
      if (equals == NULL)
        return __set_errno (EINVAL);

      name = string;
      namelen = equals - string;
      valuelen = 0;
    }
  else
    {
      namelen = strlen (name);
      valuelen = strlen (value);
    }

  /* Search environment for old value.  */
  if (environ != NULL)
    {

      for (ep = environ; *ep; ++ep)
        if (!strncmp (*ep, name, namelen) && (*ep)[namelen] == '=')
          break;
        else
          ++envcnt;
    }

  if (environ == NULL || *ep == NULL)  /* Did not find old value.  */
    {
      char **new_environ;
      /* If we allocated the environ, we can extend it, else allocate it.
         A memory leak is likely when a new environ is malloc'ed, but we
         cannot free the old environ, since it may not have been malloc'ed.  */
      if (environ == __last_environ)
        new_environ = realloc (__last_environ, (envcnt + 2) * sizeof (char*));
      else
        if ((new_environ = malloc ((envcnt + 2) * sizeof (char*))) != NULL)
          memcpy (new_environ, environ, envcnt * sizeof (char*));

      /* Failed to allocate the environment.  */
      if (new_environ == NULL)
        return __set_errno (ENOMEM);

      if (string)
        {
          /* Use the string directly */
          new_environ[envcnt] = string;
        }
      else
        {
          /* Take a copy of the name and value */
          new_environ[envcnt] = malloc (namelen + valuelen + 2);
          if (new_environ[envcnt] == NULL)
            {
              if (environ != __last_environ)
                free (new_environ);
              return __set_errno (ENOMEM);
            }

          memcpy (new_environ[envcnt], name, namelen);
          new_environ[envcnt][namelen] = '=';
          memcpy (new_environ[envcnt] + namelen + 1, value, valuelen + 1);
        }

      new_environ[envcnt + 1] = NULL;
      __last_environ = environ = new_environ;
    }
  else if (!replace)
    {
      return 0;
    }
  else
    {
      /* Replace existing variable */
      if (string)
        {
          /* The environment should point to the user supplied string */
          *ep = string;
        }
      else
        {
          /* If we cannot use the old space, then allocate.  */
          if (strlen (*ep) < namelen + 1 + valuelen)
            {
              char *str = malloc (namelen + 2 + valuelen);
              /* Allocate buffer for this environment value, leaving old value
                 on failure.  Likely memory leak here, but we must not try
                 to free the old string, since it may not have been malloc'ed.  */
              if (str == NULL)
                return __set_errno (ENOMEM);
              *ep = str;
            }

          /* Copy into enviroment */
          memcpy (*ep, name, namelen);
          (*ep)[namelen] = '=';
          memcpy (&(*ep)[namelen + 1], value, valuelen + 1);
        }
    }

  return 0;
}

/* Put STRING, which is of the form "NAME=VALUE", in the environment.
   If there is no '=' in STRING, then remove it from the environment.  */
int
putenv (char *string)
{
  if (strchr (string, '='))
    return __addenv_to_env (string, NULL, NULL, 1);
  else
    return unsetenv (string);
}
