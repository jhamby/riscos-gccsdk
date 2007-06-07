/* som_register.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_REGISTER_H
#define SOM_REGISTER_H

#include <kernel.h>
#include "som.h"

/* SWI "SOM_RegisterObject"
 *
 * entry:
 *  r0 = Reason code
 *  r1 = handle
 *  r2 = ptr to OBJINFO_* struct
 * exit:
 *  all other registers preserved if no error,
 *  otherwise r0 = ptr to error block
 */
_kernel_oserror *som_register_object(_kernel_swi_regs *regs);

/* SWI "SOM_DeregisterClient"
 *
 * All registers preserved.
 */
_kernel_oserror *som_deregister_client(void);

/* SWI "SOM_DeregisterSharedObject"
 *
 * entry:
 *  r0 = handle
 * exit:
 *  all registers preserved
 */
_kernel_oserror *som_deregister_shared_object(_kernel_swi_regs *regs);

/* Register a library for the current client. If the library is not already
 * registered in the global object list, then an OBJECT_* structure is
 * allocated and placed in the global list. Then regardless of whether the
 * library was in the global list or not, its OBJECT_* structure is cloned
 * and placed in the client's list.
 */
_kernel_oserror *som_register_sharedobject(som_handle handle, som_objinfo *objinfo, som_object **object_ret);

_kernel_oserror *som_register_client(som_handle handle, som_objinfo *objinfo);

#endif
