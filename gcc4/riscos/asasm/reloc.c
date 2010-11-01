/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developersrs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * reloc.c
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "aoffile.h"
#include "area.h"
#include "code.h"
#include "elf.h"
#include "error.h"
#include "eval.h"
#include "filestack.h"
#include "fix.h"
#include "get.h"
#include "global.h"
#include "input.h"
#include "lex.h"
#include "main.h"
#include "option.h"
#include "output.h"
#include "reloc.h"
#include "symbol.h"

#define READWORD(image,offset) \
  (((image)[(offset)+3]<<24) | ((image)[(offset)+2]<<16) | \
   ((image)[(offset)+1]<<8 ) |  (image)[(offset)  ])


static const char *
reloc2String (RelocTag tag)
{
  static const char * const relocstr[] =
  {
    "RelocShiftImm",
    "RelocImm8s4",
    "RelocImmFloat",
    "RelocBranch",
    "RelocBranchT",
    "RelocSWI",
    "RelocCpuOffset",
    "RelocCopOffset",
    "RelocAdr",
    "RelocAdrl",
    "RelocImmN",
    "RelocFloat",
    "RelocNone"
  };

  return relocstr[tag];
}


static Reloc *
relocNew (Reloc *more, RelocTag tag, int offset, const Value *value)
{
  Reloc *newReloc;
  if ((newReloc = malloc (sizeof (Reloc))) == NULL)
    errorOutOfMem ();
  newReloc->more = more;
  newReloc->Tag = tag;
  newReloc->lineno = FS_GetCurLineNumber ();
  newReloc->file = FS_GetCurFileName ();
  newReloc->offset = offset;
  newReloc->value = valueCopy (*value);
  return newReloc;
}


static void
relocOp (int word, const Value *value, RelocTag tag)
{
  Reloc *newReloc = relocNew (areaCurrentSymbol->area.info->relocs, tag,
			      areaCurrentSymbol->value.Data.Int.i, value);
  newReloc->extra = word;
  areaCurrentSymbol->area.info->relocs = newReloc;
}


void
relocShiftImm (ARMWord shiftop, const Value *shift)
{
  relocOp (shiftop, shift, RelocShiftImm);
}


void
relocImm8s4 (ARMWord ir, const Value *im8s4)
{
  relocOp (ir, im8s4, RelocImm8s4);
}


void
relocImmFloat (ARMWord ir, const Value *value)
{
  relocOp (ir, value, RelocImmFloat);
}


void
relocBranch (const Value *offset)
{
  relocOp (0, offset, RelocBranch);
}


void
relocBranchT (const Value *offset)
{
  relocOp (0, offset, RelocBranchT);
}


void
relocSwi (const Value *code)
{
  relocOp (0, code, RelocSwi);
}


void
relocCpuOffset (ARMWord ir, const Value *offset)
{
  relocOp (ir, offset, RelocCpuOffset);
}


void
relocCopOffset (ARMWord ir, const Value *offset)
{
  relocOp (ir, offset, RelocCopOffset);
}


void
relocAdr (ARMWord ir, const Value *addr)
{
  relocOp (ir, addr, RelocAdr);
}


void
relocAdrl (ARMWord ir, const Value *addr)
{
  relocOp (ir, addr, RelocAdrl);
}


void
relocMask (const Value *mask)
{
  relocOp (0, mask, RelocImmN);
}


void
relocInt (int size, const Value *value)
{
  relocOp (size, value, RelocImmN);
}


void
relocFloat (int size, const Value *value)
{
  relocOp (size, value, RelocFloat);
}


void
relocAdd (Reloc *newReloc)
{
  newReloc->more = areaCurrentSymbol->area.info->relocs;
  areaCurrentSymbol->area.info->relocs = newReloc;
}


static int
relocLate2Reloc (Reloc *r, Value *value)
{
  size_t size = 0;
  int norelocs = 0;
  for (LateInfo *late = value->Data.Late.late; late; late = late->next)
    {
      if (late->factor > 0)
	{			/* kludge: apparently <0 for area symbols */
	  norelocs += late->factor;	/* late->factor cannot be less that 0 here */
	  if (late->factor > 1)
	    {
	      r->value.Data.Code.c[size].Tag = CodeValue;
	      r->value.Data.Code.c[size].Data.value.Tag = ValueInt;
	      r->value.Data.Code.c[size++].Data.value.Data.Int.i = late->factor;
	    }
	  r->value.Data.Code.c[size].Tag = CodeSymbol;
	  r->value.Data.Code.c[size++].Data.symbol = late->symbol;
	}
    }
  if (size > r->value.Data.Code.len)
    errorAbortLine (r->lineno, r->file, "Overflow in relocation data");
  r->value.Data.Code.len = size;
  return norelocs;
}


static int
relocEval (Reloc *r, Value *value, const Symbol *area)
{
  int norelocs = 0;

  codeInit ();
  *value = codeEvalLow (ValueAll, r->value.Data.Code.len, r->value.Data.Code.c);
  switch (value->Tag)
    {
    case ValueIllegal:
      errorLine (r->lineno, r->file, ErrorError, "Cannot evaluate expression (illegal)");
      r->Tag = RelocNone;
      return 0;

    case ValueCode:
      errorLine (r->lineno, r->file, ErrorError, "Cannot evaluate expression (code)");
      r->Tag = RelocNone;
      return 0;

    case ValueLateLabel:
      switch (r->Tag)
	{
	case RelocCpuOffset:
	case RelocCopOffset:
/*
   for (late = value->Data.Late.late; late; late = late->next)
   if(late->symbol == area) {
   late->factor = 0;
   break;
   }
 */
	  break;

	case RelocBranch:
	case RelocBranchT:
	  {
	    int thisF = 0;

	    for (LateInfo *late = value->Data.Late.late; late; late = late->next)
	      if (late->symbol == area)
	        {
		  thisF = late->factor;
		  late->factor = 0;
		  break;
	        }
	    for (LateInfo *late = value->Data.Late.late; late; late = late->next)
	      if (late->factor > 0)
	        {
		  if (!(late->symbol->type & SYMBOL_AREA))
		    late->symbol->used++;
		  thisF += late->factor;
	        }
	      else if (late->factor < 0)
	        {
		  errorLine (r->lineno, r->file, ErrorError, "Only positive relocation allowed");
		  late->factor = 1;
	        }
	    if (thisF)
	      errorLine (r->lineno, r->file, ErrorError, "Unbalanced relocation (%d)", thisF);
	    break;
	  }

	case RelocImmN:
	  {
	    for (LateInfo *late = value->Data.Late.late; late; late = late->next)
	      {
	        if (late->factor > 0)
		  {
		    if (!(late->symbol->type & SYMBOL_AREA))
		      late->symbol->used++;
		    else if (r->extra != 4)
		      errorLine (r->lineno, r->file, ErrorError,
			         "8/16 bits field cannot be allocated with area (%s)",
			         late->symbol->str);
		  }
	        else if (late->factor < 0)
		  {
		    errorLine (r->lineno, r->file, ErrorError, "Only positive relocation allowed");
		    late->factor = 1;
		  }
	      }
	    break;
	  }

	case RelocAdr:
	case RelocAdrl:
	  {
	    for (LateInfo *late = value->Data.Late.late; late; late = late->next)
	      if (late->factor > 0 && !(late->symbol->type & SYMBOL_AREA))
	        late->symbol->used++;
	    break;
	  }

	default:
	  errorLine (r->lineno, r->file, ErrorError, "Linker cannot handle %s", reloc2String (r->Tag));
	  r->Tag = RelocNone;
	  return 0;
	}			/* ValueLateLabel */

      norelocs = relocLate2Reloc (r, value);
      break;

    case ValueInt:
      break;

    default:
      errorAbortLine (r->lineno, r->file, "Illegal ValueTag in relocEval");
      break;
    }

  return norelocs;
}


static void
relocWrite (Reloc *r, const Value *value, unsigned char *image)
{
  const int offset = r->offset;
  
  switch (value->Tag)
    {
      case ValueLateLabel:
      case ValueInt:
	switch (r->Tag)
	{			/* Write out the value */
	  case RelocBranch:
	    {
	      /* The R_ARM_PC24 ELF reloc needs to happen for a "B + PC - 8"
	         instruction, while in AOF this needs to happen for a "B 0".  */
	      int bv = (!option_aof && value->Tag == ValueLateLabel) ?
		value->Data.Int.i + offset : value->Data.Int.i;
	      ARMWord w = fixBranch (r->lineno, bv);
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocBranchT:
	    {
	      ARMWord w = READWORD (image, offset);
	      w |= fixBranchT (r->lineno, value->Data.Int.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocCpuOffset:
	    {
	      ARMWord w = READWORD (image, offset);
	      /* fprintf (stderr, "RelocCpuOffset: tag = %d, lineno = %d, offset = %d, val=%s\n",
			  value->Tag.t, r->lineno, value->Data.Int.i,
			  (value->Tag.t == ValueLateLabel) ? value->Data.Late.late->symbol->str : "-"); */
	      if (value->Tag == ValueLateLabel)
		{
		  const Symbol *sym = value->Data.Late.late->symbol;
		  if ((sym->type & SYMBOL_AREA)
		      && (sym->area.info->type & AREA_BASED))
		    {
		      /* fprintf (stderr, "  --- based: off = %d, adjusted = %d\n",
				  offset,
				  value->Data.Int.i + offset + 8); */
		      /* value->Data.Int.i += offset + 8; */
		      w = fixCpuOffset (r->lineno, w,
					value->Data.Int.i + offset + 8);
		      w &= ~LHS_OP(15);
		      w |= LHS_OP((sym->area.info->type & 0x0f000000) >> 24);
		      /* r->offset = value->Data.Int.i + offset + 8; */
		    }
		  else
		    w = fixCpuOffset (r->lineno, w, value->Data.Int.i);
		}
	      else
		w = fixCpuOffset (r->lineno, w, value->Data.Int.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocCopOffset:
	    {
	      ARMWord w = READWORD (image, offset);
	      w = fixCopOffset (r->lineno, w, value->Data.Int.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocImmN:
	    {
	      /* fprintf (stderr, "RelocImmN: lineno = %d\n", r->lineno); */
	      ARMWord w = fixInt (r->lineno, r->extra, value->Data.Int.i);
	      switch (r->extra)
		{
		  case 4:
		    image[offset + 3] = (w >> 24) & 0xff;
		    image[offset + 2] = (w >> 16) & 0xff;
		    /* Fall through */
		  case 2:
		    image[offset + 1] = (w >> 8) & 0xff;
		    /* Fall through */
		  case 1:
		    image[offset + 0] = w & 0xff;
		}
	      break;
	    }
	  case RelocAdr:
	    {
	      /* fprintf (stderr, "RelocAdr: lineno = %d\n", r->lineno); */
	      ARMWord w = READWORD (image, offset);
	      w = fixAdr (r->lineno, w, value->Data.Int.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocAdrl:
	    {
	      ARMWord w = READWORD (image, offset);
	      ARMWord w1 = (value->Tag == ValueLateLabel) ? value->Data.Late.late->symbol->type & SYMBOL_DEFINED : 1;
	      /* warn if symbol is defined or isn't a late label */
	      fixAdrl (r->lineno, &w, &w1, value->Data.Int.i, w1);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      image[offset + 7] = (w1 >> 24) & 0xff;
	      image[offset + 6] = (w1 >> 16) & 0xff;
	      image[offset + 5] = (w1 >> 8) & 0xff;
	      image[offset + 4] = w1 & 0xff;
	      break;
	    }
	  case RelocImm8s4:
	    {
	      ARMWord w = READWORD (image, offset);
	      w = fixImm8s4 (r->lineno, w, value->Data.Int.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  default:
	    errorLine (r->lineno, r->file, ErrorError, "Cannot handle %s when value is %s", reloc2String (r->Tag), "int");
	    r->Tag = RelocNone;
	    return;
	}
	if (value->Tag == ValueInt || r->value.Data.Code.len == 0)	/* Value is known */
	  r->Tag = RelocNone;
	break;
      case ValueFloat:
	switch (r->Tag)
	{			/* Write out the value */
	  case RelocImmN:	/* ? */
	    case RelocFloat:
	    {
	      unsigned int i;
	      union
		{
		  double d;
		  float f;
		  struct
		    {
		      char c[8];
		    } u;
		} translate;		/* Do endianness issues affect this struct? */
	      if (r->extra == 4)
		translate.f = (float) value->Data.Float.f;
	      else
		translate.d = (double) value->Data.Float.f;
	      for (i = 0; i < r->extra; i++)
		image[offset + i] = translate.u.c[i];
	    }
	    break;
	  case RelocImmFloat:
	    {
	      ARMWord w = READWORD (image, offset);
	      w = fixImmFloat (r->lineno, w, value->Data.Float.f);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  default:
	    errorLine (r->lineno, r->file, ErrorError, "Cannot handle %s when value is %s", reloc2String (r->Tag), "float");
	    break;
	}
	r->Tag = RelocNone;
	break;
      default:
	errorAbortLine (r->lineno, r->file, "Internal relocWrite: illegal value");
	r->Tag = RelocNone;
	break;
    }
}


int
relocFix (const Symbol *area)
{
  unsigned char *image = area->area.info->image;
  int norelocs = 0;
  for (Reloc *relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      Value value;
      switch (relocs->value.Tag)
	{
	case ValueInt:
	case ValueFloat:
	  value = relocs->value;
	  break;
	case ValueCode:
	  norelocs += relocEval (relocs, &value, area);
	  break;
	default:
	  errorAbortLine (relocs->lineno, relocs->file, "Internal relocFix: not a legal value");
	  relocs->Tag = RelocNone;
	  break;
	}
      if (relocs->Tag != RelocNone)	/* We now have a Value */
	relocWrite (relocs, &value, image);
    }
  return norelocs;
}


void
relocAOFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      unsigned int How;
      switch (relocs->Tag)
	{
	case RelocImmN:
	  switch (relocs->extra)
	    {
	    case 4:
	      How = HOW2_INIT | HOW2_WORD;
	      break;
	    case 2:
	      How = HOW2_INIT | HOW2_HALF;
	      break;
	    case 1:
	      How = HOW2_INIT | HOW2_BYTE;
	      break;
	    default:
	      errorAbortLine (relocs->lineno, relocs->file, "Linker cannot handle RelocImmN with size %d", relocs->extra);
	      continue;
	    }
	  break;
	case RelocCpuOffset:
	  if ((relocs->extra & 0x0F000000) < 0x04000000)
	    errorAbortLine (relocs->lineno, relocs->file,
			    "Linker cannot handle ARMv4 extended LDR/STR");
	  // FIXME: determine 'How' !
	  break;
	case RelocCopOffset:
	  if (relocs->value.Tag == ValueCode
	      && (relocs->value.Data.Code.c->Data.symbol->type & SYMBOL_AREA)
	      && (relocs->value.Data.Code.c->Data.symbol->area.info->type & AREA_BASED))
	    How = HOW3_INIT | HOW3_INSTR | HOW3_BASED;
	  else
	    How = HOW2_INIT | HOW2_SIZE | HOW2_SYMBOL;
	  break;
	case RelocImm8s4:
	  How = HOW2_INIT | HOW2_SIZE | HOW2_SYMBOL;
	  break;
	case RelocBranch:
	case RelocBranchT:
	case RelocAdr:
	case RelocAdrl:
	  How = HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE;
	  break;
	case RelocNone:
	  continue;
	default:
	  errorAbortLine (relocs->lineno, relocs->file, "Linker cannot handle this");
	  continue;
	}
      AofReloc areloc;
      areloc.Offset = armword(relocs->offset);
      for (size_t ip = 0; ip < relocs->value.Data.Code.len; ip++)
	{
	  int loop;
	  if (relocs->value.Data.Code.c[ip].Tag == CodeValue)
	    {
	      if (relocs->value.Data.Code.c[ip].Data.value.Tag != ValueInt)
		{
		  errorAbortLine (relocs->lineno, relocs->file, "Internal relocsOutput: not an int");
		  loop = 0;
		}
	      else
		loop = relocs->value.Data.Code.c[ip++].Data.value.Data.Int.i;
	    }
	  else
	    loop = 1;
	  if (relocs->value.Data.Code.c[ip].Tag != CodeSymbol)
	    errorAbortLine (relocs->lineno, relocs->file, "Internal error in relocsOutput");
	  areloc.How = How | relocs->value.Data.Code.c[ip].Data.symbol->used;
	  if (!(relocs->value.Data.Code.c[ip].Data.symbol->type & SYMBOL_AREA))
	    areloc.How |= HOW2_SYMBOL;
	  areloc.How = armword(areloc.How);
	  while (loop--)
	    fwrite (&areloc, 1, sizeof(AofReloc), outfile);
	}
    }
}

#ifndef NO_ELF_SUPPORT
void
relocELFOutput (FILE *outfile, const Symbol *area)
{
  for (const Reloc *relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      unsigned int How;
      switch (relocs->Tag)
        {
        case RelocImmN:
          switch (relocs->extra)
            {
            case 4:
              How = HOW2_INIT | HOW2_WORD;
              break;
            case 2:
              How = HOW2_INIT | HOW2_HALF;
              break;
            case 1:
              How = HOW2_INIT | HOW2_BYTE;
              break;
            default:
              errorAbortLine (relocs->lineno, relocs->file,
                              "Linker cannot handle RelocImmN with size %d", relocs->extra);
              continue;
            }
          break;
        case RelocCpuOffset:
          if ((relocs->extra & 0x0F000000) < 0x04000000)
            errorAbortLine (relocs->lineno, relocs->file,
			    "Linker cannot handle ARMv4 extended LDR/STR");
	  // FIXME: determine 'How' !
	  break;
        case RelocCopOffset:
          if (relocs->value.Tag == ValueCode
              && (relocs->value.Data.Code.c->Data.symbol->type & SYMBOL_AREA)
              && (relocs->value.Data.Code.c->Data.symbol->area.info->type & AREA_BASED))
            How = HOW3_INIT | HOW3_INSTR | HOW3_BASED;
          else
            How = HOW2_INIT | HOW2_SIZE | HOW2_SYMBOL;
          break;
        case RelocImm8s4:
          How = HOW2_INIT | HOW2_SIZE | HOW2_SYMBOL;
          break;
        case RelocBranch:
        case RelocBranchT:
        case RelocAdr:
        case RelocAdrl:
          How = HOW2_INIT | HOW2_SIZE | HOW2_RELATIVE;
          break;
        case RelocNone:
          continue;
        default:
          errorAbortLine (relocs->lineno, relocs->file, "Linker cannot handle this");
          continue;
        }
      Elf32_Rel areloc;
      areloc.r_offset = armword(relocs->offset);
      for (size_t ip = 0; ip < relocs->value.Data.Code.len; ip++)
        {
	  int loop, symbol, type;
          if (relocs->value.Data.Code.c[ip].Tag == CodeValue)
            {
              if (relocs->value.Data.Code.c[ip].Data.value.Tag != ValueInt)
		{
                  errorAbortLine (relocs->lineno, relocs->file, "Internal relocsOutput: not an int");
		  loop = 0;
		}
              else
                loop = relocs->value.Data.Code.c[ip++].Data.value.Data.Int.i;
            }
          else
            loop = 1;
          if (relocs->value.Data.Code.c[ip].Tag != CodeSymbol)
            errorAbortLine (relocs->lineno, relocs->file, "Internal error in relocsOutput");

          How |= relocs->value.Data.Code.c[ip].Data.symbol->used;
          if (!(relocs->value.Data.Code.c[ip].Data.symbol->type & SYMBOL_AREA))
            How |= HOW2_SYMBOL;

          symbol = (How & HOW3_SIDMASK) + 1;

          if (How & HOW3_RELATIVE)
            type = R_ARM_PC24;
          else
            type = R_ARM_ABS32;

          areloc.r_info = ELF32_R_INFO(symbol, type);
          while (loop--)
            fwrite (&areloc, 1, sizeof(Elf32_Rel), outfile);
        }
    }
}
#endif
