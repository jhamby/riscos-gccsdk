/* config.h */
#ifndef GCC_CONFIG_H
#define GCC_CONFIG_H
#ifdef HOST_ARM_RISCOS
#include "auto-host-riscos.h"
#else
#include "auto-host.h"
#endif
#ifdef IN_GCC
#include "ansidecl.h"
#endif /* IN_GCC */

#endif /* GCC_CONFIG_H */

