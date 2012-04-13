#include "syscall.h"
#include "sysdep.h"

void ATTRIBUTE_HIDDEN
_dl_exit_internal (int status)
{
  _dl_exit (status);
}
