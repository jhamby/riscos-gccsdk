
/*
 * help_cpu.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "help_cpu.h"
#include "error.h"

static const int mask[] =
{
  0x000000ff, 0xc000003f, 0xf000000f, 0xfc000003,
  0xff000000, 0x3fc00000, 0x0ff00000, 0x03fc0000,
  0x00ff0000, 0x003fc000, 0x000ff000, 0x0003fc00,
  0x0000ff00, 0x00003fc0, 0x00000ff0, 0x000003fc
};

int 
help_cpuImm8s4 (int value)
{
  int i;
  for (i = 0; i < 16; i++)
    {
      if ((value & mask[i]) == value)	/* hittat mask */
	return (((((WORD) value >> ((16 - i) * 2)) | value << (i * 2)) & 0xff) | i << 8);
    }
  return -1;
}
