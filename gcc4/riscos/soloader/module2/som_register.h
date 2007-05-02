/* som_register.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_REGISTER_H
#define SOM_REGISTER_H

#include <kernel.h>

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
_kernel_oserror *som_deregister_client(_kernel_swi_regs *regs);

/* SWI "SOM_DeregisterSharedObject"
 *
 * entry:
 *  r0 = handle
 * exit:
 *  all registers preserved
 */
_kernel_oserror *som_deregister_shared_object(_kernel_swi_regs *regs);

#endif
