/* killpg ()
 * Copyright (c) 2011 UnixLib Developers
 */

#include <signal.h>

int
killpg (int pgrp, int sig)
{
  return kill (-pgrp, sig);
}

