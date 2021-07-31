/* main.h
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef MAIN_H
#define MAIN_H

#include "types.h"

_kernel_oserror *app_new(app_object **app_ret);
_kernel_oserror *app_find(app_object **app_ret);

_kernel_oserror *armeabi_cleanup(void);

#endif
