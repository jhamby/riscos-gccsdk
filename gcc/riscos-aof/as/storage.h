/*
 * storage.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef storage_header_included
#define storage_header_included

#include "value.h"
#include "symbol.h"

Value storageValue (void);
void c_record (void);
void c_alloc (Symbol * sym);

#endif
