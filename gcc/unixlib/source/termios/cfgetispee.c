/* cfgetispeed ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>
#include <termios.h>

/* Return the input baud rate stored in the termios structure.  */
__speed_t
cfgetispeed (const struct termios *termios_p)
{
  if (termios_p == NULL)
    return __set_errno (EINVAL);

  return termios_p->c_ispeed;
}
