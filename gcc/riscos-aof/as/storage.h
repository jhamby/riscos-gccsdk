/*
 * storage.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _storage_h
#define _storage_h

#ifndef _value_h
#include "value.h"
#endif

#ifndef _symbol_h
#include "symbol.h"
#endif

Value storageValue (void);
void c_record (void);
void c_alloc (Symbol * sym);

#endif
