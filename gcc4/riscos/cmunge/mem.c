/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdlib.h>

#include "error.h"

void *Malloc(int s) {

  void *v;

  v = malloc(s);
  if (v == NULL)
    ErrorFatal("Malloc failed (%d bytes); please increase WimpSlot!");
  return v;
}

void *Realloc(void *v, int s) {

  v = realloc(v, s);
  if (v == NULL)
    ErrorFatal("Realloc failed (%d bytes); please increase WimpSlot!");
  return v;
}

void Free(void *v) {

  free(v);
}
