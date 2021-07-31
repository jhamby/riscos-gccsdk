/* alphasort ()
 * Written by Nick Burrett, 3 November 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <dirent.h>
#include <string.h>

int
alphasort (const struct dirent **a, const struct dirent **b)
{
  return strcmp ((*a)->d_name, (*b)->d_name);
}
