
/*
 * put.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _put_h
#define _put_h


#ifndef _global_h
#include "global.h"
#endif

void putData (int size, WORD data);
void putDataFloat (int size, FLOAT data);
void putIns (WORD ins);

#endif
