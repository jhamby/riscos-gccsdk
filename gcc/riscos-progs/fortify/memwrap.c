
/**
 * Fortify memory wrappers
 *
 * This file is used to redirect the memory allocation functions via the fortify
 * memory check library.  The original symbols in alloc.o are renamed to
 * __unixlib_real_malloc, etc, and the calls in fortify to the functions
 * redirected to the these new names.  This means that any other references to
 * the functions come here, to allow whatever processing we choose.
 *
 * The symbol renaming is achieved via drlink's link editing feature.
 */

#include <stdlib.h>
#include "fortify.h"

#undef malloc
#undef calloc
#undef realloc
#undef free

void *malloc(size_t size) {
  return Fortify_malloc(size, NULL, 0);
}

void *realloc(void *ptr, size_t size) {
  return Fortify_realloc(ptr, size, NULL, 0);
}

void *calloc(size_t nmemb, size_t size) {
  return Fortify_calloc(nmemb, size, NULL, 0);
}

void free(void *ptr) {
  return Fortify_free(ptr, NULL, 0);
}

