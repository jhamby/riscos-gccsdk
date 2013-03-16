/* ro.c
   Utility functions for cvs which are specific for RISC OS
   John Tytgat <mailto:John.Tytgat@aaug.net>

   This file is part of GNU CVS.

   GNU CVS is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.  */

/* We may *not* include 'config.h' directly or indirectly otherwise
 * or 'getenv' filter will not work.
 */
#include <stdlib.h>
#include <string.h>

/* 'getenv' filter :
 *   - if 'CVS_*' env var can't be found, look again for 'CVS$*' env var
 *   - if 'CVS*' env var can't be found, look again for 'CVS$*' env var
 *   - if '*' env var can't be found, look again for 'CVS$*' env var
 */
char *ro_getenv(const char *env)
{
    char *rslt;

  rslt = getenv(env);
  if (rslt == NULL)
  {
    int tryit;

    if (env[0] == 'C' && env[1] == 'V' && env[2] == 'S')
    {
      if (env[3] != '$')
        tryit = (env[3] == '_') ? 4 : 3;
      else
        tryit = -1;
    }
    else
      tryit = 0;

    if (tryit != -1)
    {
      char buf[48];

      if (strlen(env + tryit) + 4 >= 48)
        abort();

      buf[0] = 'C';
      buf[1] = 'V';
      buf[2] = 'S';
      buf[3] = '$';

      strcpy(buf + 4, env + tryit);
      rslt = getenv(buf);
    }
  }

  return(rslt);
}

char *ro_getlogin(void)
{
  static char *login;

  if (login == NULL)
  {
    if ((login = ro_getenv("CVS$USER")) == NULL)
      login = "uid1";
  }

  return(login);
}
