/*
 * put.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <stdint.h>
#include "put.h"
#include "error.h"
#include "area.h"

extern int align;


static void 
putAlign (int align, const char *msg)
{
  error (ErrorInfo, TRUE, "Unaligned %s", msg);
  if (areaCurrent)
    while (areaCurrent->value.ValueInt.i & align)
      areaCurrent->area.info->image[areaCurrent->value.ValueInt.i++] = 0;
  else
    areaError ();
}


void 
putData (int size, WORD data)
{
  if (!areaCurrent)
    {
      areaError ();
      return;
    }
  if (align)
    {
      switch (size)
	{
	case 1:
	  break;
	case 2:
	  if (areaCurrent->value.ValueInt.i & 1)
	    putAlign (1, "halfword");
	  break;
	case 4:
	  if (areaCurrent->value.ValueInt.i & 3)
	    putAlign (3, "word");
	  break;
	default:
	  error (ErrorSerious, TRUE, "Internal putData: illegal size");
	}
    }

  if (AREA_IMAGE (areaCurrent->area.info))
    {
      if (AREA_NOSPACE (areaCurrent->area.info, areaCurrent->value.ValueInt.i + size))
	areaGrow (areaCurrent->area.info, size);
      switch (size)
	{
	case 4:
	  areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 3] = (data >> 24) & 0xff;
	  areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 2] = (data >> 16) & 0xff;
	case 2:
	  areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 1] = (data >> 8) & 0xff;
	case 1:
	  areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 0] = data & 0xff;
	}
    }
  else if (data)
    {
      error (ErrorError, TRUE, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
  areaCurrent->value.ValueInt.i += size;
}


void 
putDataFloat (int size, FLOAT data)
{
  union
  {
    double d;
    float f;
    struct
      {
	char c[8];		/* endianness? */
      }
    u;
  }
  translate;
  int i;

  if (!areaCurrent)
    {
      areaError ();
      return;
    }
  switch (size)
    {
    case 4:
      translate.f = (float) data;
      if (areaCurrent->value.ValueInt.i & 3)
	putAlign (3, "float single");
      break;
    case 8:
      translate.d = (double) data;
      if (areaCurrent->value.ValueInt.i & 3)
	putAlign (3, "float double");
      break;
    default:
      error (ErrorSerious, TRUE, "Internal putDataFloat: illegal size %d", size);
    }

  if (AREA_IMAGE (areaCurrent->area.info))
    {
      if (AREA_NOSPACE (areaCurrent->area.info, areaCurrent->value.ValueInt.i + size))
	areaGrow (areaCurrent->area.info, size);
      for (i = 0; i < size; i++)
	{
	  areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + i] = translate.u.c[i];
	}
    }
  else if (data)
    {
      error (ErrorError, TRUE, "Trying to define a non-zero value in an uninitialised area");
      return;
    }
  areaCurrent->value.ValueInt.i += size;
}


void 
putIns (WORD ins)
{
  if (!areaCurrent)
    {
      areaError ();
      return;
    }
  if (areaCurrent->value.ValueInt.i & 3)
    putAlign (3, "instruction");

  if (AREA_IMAGE (areaCurrent->area.info))
    {
      if (AREA_NOSPACE (areaCurrent->area.info, areaCurrent->value.ValueInt.i + 4))
	areaGrow (areaCurrent->area.info, 4);
      areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 3] = (ins >> 24) & 0xff;
      areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 2] = (ins >> 16) & 0xff;
      areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 1] = (ins >> 8) & 0xff;
      areaCurrent->area.info->image[areaCurrent->value.ValueInt.i + 0] = ins & 0xff;
      areaCurrent->value.ValueInt.i += 4;
    }
  else
    error (ErrorError, TRUE, "Trying to define code an uninitialised area");
}
