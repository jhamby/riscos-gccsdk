/* putenv ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <internal/unix.h>
#include <pthread.h>

/* Add NAME=VALUE to the environment. If NAME is already in the environment,
   only add when replace is non-zero.  If string is non-NULL then it is
   used directly, otherwise a copy of name and value is used. */
int
__addenv_to_env (char *string, const char *name, const char *value, int replace)
{
  struct ul_global *gbl = &__ul_global;
  char **ep = NULL;
  size_t envcnt = 0;
  size_t namelen;
  size_t valuelen;

  PTHREAD_UNSAFE

#ifdef DEBUG
  debug_printf ("-- __add_to_env: %s %s, environ=%p, gbl->last_environ=%p\n",
		string ? string : name, string ? "" : value, environ,
		gbl->last_environ);
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
      if (environ == gbl->last_environ)
        new_environ = realloc (gbl->last_environ, (envcnt + 2) * sizeof (char*));
      else
        if ((new_environ = malloc ((envcnt + 2) * sizeof (char*))) != NULL)
          memcpy (new_environ, environ, envcnt * sizeof (char*));

      /* Failed to allocate the environment.  */
      if (new_environ == NULL)
        return -1;

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
              if (environ != gbl->last_environ)
                free (new_environ);
              return -1;
            }

          memcpy (new_environ[envcnt], name, namelen);
          new_environ[envcnt][namelen] = '=';
          memcpy (new_environ[envcnt] + namelen + 1, value, valuelen + 1);
        }

      new_environ[envcnt + 1] = NULL;
      gbl->last_environ = environ = new_environ;
    }
  else if (replace)
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
                return -1;
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
  const char *equal = strchr (string, '=');

  if (!equal)
    return unsetenv (string);
  else
    {
      const char *dollar = strchr (string, '$');

      if (dollar && dollar < equal)
        {
          char *name = alloca(equal - string + 1);
          memcpy(name, string, equal - string);
          name[equal - string] = '\0';

          return __addenv_to_os (name, equal + 1, 1);
        }
      else
        return __addenv_to_env (string, NULL, NULL, 1);
    }
}
