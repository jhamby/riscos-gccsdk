/* Sanity check routines for module code generation in GCCSDK.
 * Written by John Tytgat <John.Tytgat@aaug.net>
 */

#include <stdint.h>
#include <stdio.h>

#include "check.h"

/* #define REPORT_EXPECTATIONS 1 */

/** Variables exported from constants.s  */
extern const int Image_RO_Base;
extern const int Image_RO_Limit;
extern const int Image_RW_Base;
extern const int Image_RW_Limit;
extern const int Image_ZI_Base;
extern const int Image_ZI_Limit;

#ifndef DISABLE_MODULE_CHECKS

static intptr_t oModWS_NonZeroData_Begin = 0;
static intptr_t oModWS_NonZeroData_End = 0;
static intptr_t oModWS_ZeroData_Begin = 0;
static intptr_t oModWS_ZeroData_End = 0;

/**
 * Initialisation routine to be called before any of the other
 * check routines.
 * \param pw Pointer to the module's private word (holding the module's RMA
 * workspace pointer).
 */
void
CheckInit (void *pw)
{
  const intptr_t beginUserWS = *(intptr_t *)pw + 4*3; /* SCL uses the first 3 words in pw.  */

#ifdef REPORT_EXPECTATIONS
  printf ("RO code from 0x%x to 0x%x (offset 0x%x)\n"
	  "Module RW code from 0x%x (offset 0x%x) to 0x%x (offset 0x%x)\n"
	  "Module ZI code from 0x%x (offset 0x%x) to 0x%x (offset 0x%x)\n"
	  "Module private workspace at %p\n",
	  Image_RO_Base,                                Image_RO_Limit, Image_RO_Limit - Image_RO_Base,
	  Image_RW_Base, Image_RW_Base - Image_RO_Base, Image_RW_Limit, Image_RW_Limit - Image_RO_Base,
	  Image_ZI_Base, Image_ZI_Base - Image_RO_Base, Image_ZI_Limit, Image_ZI_Limit - Image_RO_Base,
	  *(int *)pw);
#endif

  oModWS_NonZeroData_Begin = beginUserWS;
  oModWS_NonZeroData_End = oModWS_NonZeroData_Begin + Image_ZI_Base - Image_RW_Base;
  oModWS_ZeroData_Begin = oModWS_NonZeroData_End;
  oModWS_ZeroData_End = oModWS_NonZeroData_Begin + Image_ZI_Limit - Image_RW_Base;

#ifdef REPORT_EXPECTATIONS
  printf ("Private workspace non-zero initialised RW code from 0x%x to 0x%x\n"
          "Private workspace zero initialized RW code from 0x%x to 0x%x\n",
          oModWS_NonZeroData_Begin, oModWS_NonZeroData_End,
          oModWS_ZeroData_Begin, oModWS_ZeroData_End);
#endif
}

/**
 * Given pointer should point to module code.  I.e. we expect it to be
 * between Image$$RO$$Base and Image$$RO$$Limit.
 * \param ptr Pointer value under test.
 * \return true when expection is met, false otherwise.
 *
 * Same check is done for CheckForNonWritableDataPtr.
 */
bool
CheckForCodePtr (const void *ptr)
{
  if ((intptr_t)ptr < Image_RO_Base || (intptr_t)ptr >= Image_RO_Limit)
    {
      fprintf (stderr, "Error: %s: %p, expected to be between 0x%x and 0x%x\n",
	       __func__, ptr, Image_RO_Base, Image_RO_Limit);
      return false;
    }

  return true;
}

/**
 * Given pointer is expected to point to non-writable (const) data not
 * neededing any relocation.
 * \param ptr Pointer value under test.
 * \return true when expection is met, false otherwise.
 *
 * Same check is done as for CheckForCodePtr.
 * \see CheckForNonWritableRelocNeedingDataPtr
 */
bool
CheckForNonWritableDataPtr (const void *ptr)
{
  if ((intptr_t)ptr < Image_RO_Base || (intptr_t)ptr >= Image_RO_Limit)
    {
      fprintf (stderr, "Error: %s: %p, expected to be between 0x%x and 0x%x\n",
	       __func__, ptr, Image_RO_Base, Image_RO_Limit);
      return false;
    }

  return true;
}

/**
 * Given pointer is expected to point to non-writable (const) data needing relocation
 * before executing.  I.e. it is not 100% RO data (which is located between
 * Image$$RO$$Base and Image$$RO$$Limit) but lives between Image$$RO$$Limit
 * and Image$$RW$$Base.
 * \param ptr Pointer value under test.
 * \return true when expection is met, false otherwise.
 */
bool
CheckForNonWritableRelocNeedingDataPtr (const void *ptr)
{
  if ((intptr_t)ptr < Image_RO_Limit || (intptr_t)ptr >= Image_RW_Base)
    {
      fprintf (stderr, "Error: %s: %p, expected to be between 0x%x and 0x%x\n",
	       __func__, ptr, Image_RO_Limit, Image_RW_Base);
      return false;
    }

  return true;
}

/**
 * Given pointer is expected to point to data in writable section and that
 * data has an initial non-zero value.  I.e. the data lives in module's
 * workspace.
 * \param ptr Pointer value under test.
 * \return true when expection is met, false otherwise.
 */
bool
CheckForWritableNonZeroDataPtr (const void *ptr)
{
  if ((intptr_t)ptr < oModWS_NonZeroData_Begin || (intptr_t)ptr >= oModWS_NonZeroData_End)
    {
      fprintf (stderr, "Error: %s: %p, expected to be between 0x%x and 0x%x\n",
	       __func__, ptr, oModWS_NonZeroData_Begin, oModWS_NonZeroData_End);
      return false;
    }

  return true;
}

bool
CheckForWritableZeroDataPtr (const void *ptr)
{
  if ((intptr_t)ptr < oModWS_ZeroData_Begin || (intptr_t)ptr >= oModWS_ZeroData_End)
    {
      fprintf (stderr, "Error: %s: %p, expected to be between 0x%x and 0x%x\n",
	       __func__, ptr, oModWS_ZeroData_Begin, oModWS_ZeroData_End);
      return false;
    }

  return true;
}

#endif
