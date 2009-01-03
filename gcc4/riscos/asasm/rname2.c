#include "rname.h"

static char rname_buffer[1024];

char *rname(const char *name) {
  return riscos_to_unix(name, rname_buffer);
}
