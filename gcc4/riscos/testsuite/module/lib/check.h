/* Sanity check routines for module code generation in GCCSDK.
 * Written by John Tytgat <John.Tytgat@aaug.net>
 */

#ifndef CHECK_HEADER_INCLUDED
#define CHECK_HEADER_INCLUDED

#include <stdbool.h>

#ifndef DISABLE_MODULE_CHECKS
void CheckInit (void *pw);
bool CheckForCodePtr (const void *ptr);
bool CheckForNonWritableDataPtr (const void *ptr);
bool CheckForNonWritableRelocNeedingDataPtr (const void *ptr);
bool CheckForWritableNonZeroDataPtr (const void *ptr);
bool CheckForWritableZeroDataPtr (const void *ptr);
#else
#  define CheckInit(pw)
#  define CheckForCodePtr(ptr) true
#  define CheckForNonWritableDataPtr(ptr) true
#  define CheckForNonWritableRelocNeedingDataPtr(ptr) true
#  define CheckForWritableNonZeroDataPtr(ptr) true
#  define CheckForWritableZeroDataPtr(ptr) true
#endif

#endif

