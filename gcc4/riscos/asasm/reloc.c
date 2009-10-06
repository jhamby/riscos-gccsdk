/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developersrs
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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "aoffile.h"
#include "area.h"
#include "code.h"
#include "elf.h"
#include "error.h"
#include "eval.h"
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
  if ((newReloc = malloc (sizeof (Reloc))) != NULL)
    {
      newReloc->more = more;
      newReloc->Tag = tag;
      newReloc->lineno = inputLineNo;
      newReloc->file = inputName;
      newReloc->offset = offset;
      /* fprintf (stderr, "relocNew: line=%d, offset=%d\n", inputLineNo, offset); */
      newReloc->value = valueCopy (*value);
    }
  else
    errorOutOfMem ("relocNew2");
  return newReloc;
}


static void
relocOp (int word, const Value *value, RelocTag tag)
{
  if (areaCurrentSymbol)
    {
      Reloc *newReloc;

      newReloc = relocNew (areaCurrentSymbol->area.info->relocs, tag,
			   areaCurrentSymbol->value.ValueInt.i, value);
      newReloc->extra = word;
      areaCurrentSymbol->area.info->relocs = newReloc;
    }
  else
    areaError ();
}


void
relocShiftImm (WORD shiftop, const Value *shift)
{
  relocOp (shiftop, shift, RelocShiftImm);
}


void
relocImm8s4 (WORD ir, const Value *im8s4)
{
  relocOp (ir, im8s4, RelocImm8s4);
}


void
relocImmFloat (WORD ir, const Value *value)
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
relocCpuOffset (WORD ir, const Value *offset)
{
  relocOp (ir, offset, RelocCpuOffset);
}


void
relocCopOffset (WORD ir, const Value *offset)
{
  relocOp (ir, offset, RelocCopOffset);
}


void
relocAdr (WORD ir, const Value *addr)
{
  relocOp (ir, addr, RelocAdr);
}


void
relocAdrl (WORD ir, const Value *addr)
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
  if (areaCurrentSymbol)
    {
      newReloc->more = areaCurrentSymbol->area.info->relocs;
      areaCurrentSymbol->area.info->relocs = newReloc;
    }
  else
    areaError ();
}


static int
relocLate2Reloc (Reloc *r, Value *value)
{
  int size = 0;
  int norelocs = 0;
  LateInfo *late;

  for (late = value->ValueLate.late; late; late = late->next)
    {
      if (late->factor > 0)
	{			/* kludge: apparently <0 for area symbols */
	  norelocs += late->factor;	/* late->factor cannot be less that 0 here */
	  if (late->factor > 1)
	    {
	      r->value.ValueCode.c[size].Tag = CodeValue;
	      r->value.ValueCode.c[size].CodeValue.value.Tag.t = ValueInt;
	      r->value.ValueCode.c[size++].CodeValue.value.ValueInt.i = late->factor;
	    }
	  r->value.ValueCode.c[size].Tag = CodeSymbol;
	  r->value.ValueCode.c[size++].CodeSymbol.symbol = late->symbol;
	}
    }
  if (size > r->value.ValueCode.len)
    errorLine (r->lineno, r->file, ErrorSerious, FALSE, "Overflow in relocation data");
  r->value.ValueCode.len = size;
  return norelocs;
}


static int
relocEval (Reloc *r, Value *value, const Symbol *area)
{
  int norelocs = 0;

  codeInit ();
  *value = codeEvalLow (ValueAll, r->value.ValueCode.len, r->value.ValueCode.c);
  switch (value->Tag.t)
    {
    case ValueIllegal:
      errorLine (r->lineno, r->file, ErrorError, TRUE, "Cannot evaluate expression (illegal)");
      r->Tag = RelocNone;
      return 0;

    case ValueCode:
      errorLine (r->lineno, r->file, ErrorError, TRUE, "Cannot evaluate expression (code)");
      r->Tag = RelocNone;
      return 0;

    case ValueLateLabel:
      switch (r->Tag)
	{
	case RelocCpuOffset:
	case RelocCopOffset:
/*
   for (late = value->ValueLate.late; late; late = late->next)
   if(late->symbol == area) {
   late->factor = 0;
   break;
   }
 */
	  break;

	case RelocBranch:
	case RelocBranchT:
	  {
	    LateInfo *late;
	    int thisF = 0;

	  for (late = value->ValueLate.late; late; late = late->next)
	    if (late->symbol == area)
	      {
		thisF = late->factor;
		late->factor = 0;
		break;
	      }
	  for (late = value->ValueLate.late; late; late = late->next)
	    if (late->factor > 0)
	      {
		if (!(late->symbol->type & SYMBOL_AREA))
		  late->symbol->used++;
		thisF += late->factor;
	      }
	    else if (late->factor < 0)
	      {
		errorLine (r->lineno, r->file, ErrorError, TRUE, "Only positive relocation allowed");
		late->factor = 1;
	      }
	  if (thisF)
	    errorLine (r->lineno, r->file, ErrorError, TRUE, "Unbalanced relocation (%d)", thisF);
	  break;
	  }

	case RelocImmN:
	  {
	    LateInfo *late;

	  for (late = value->ValueLate.late; late; late = late->next)
	    {
	      if (late->factor > 0)
		{
		  if (!(late->symbol->type & SYMBOL_AREA))
		    late->symbol->used++;
		  else if (r->extra != 4)
		    errorLine (r->lineno, r->file, ErrorError, TRUE,
			       "8/16 bits field cannot be allocated with area (%s)",
			       late->symbol->str);
		}
	      else if (late->factor < 0)
		{
		  errorLine (r->lineno, r->file, ErrorError, TRUE, "Only positive relocation allowed");
		  late->factor = 1;
		}
	    }
	  break;
	  }

	case RelocAdr:
	case RelocAdrl:
	  {
	    LateInfo *late;

	  for (late = value->ValueLate.late; late; late = late->next)
	    if (late->factor > 0 && !(late->symbol->type & SYMBOL_AREA))
	      late->symbol->used++;
	  break;
	  }

	default:
	  errorLine (r->lineno, r->file, ErrorError, TRUE, "Linker cannot handle %s", reloc2String (r->Tag));
	  r->Tag = RelocNone;
	  return 0;
	}			/* ValueLateLabel */

      norelocs = relocLate2Reloc (r, value);
      break;

    case ValueInt:
      break;

    default:
      errorLine (r->lineno, r->file, ErrorSerious, FALSE, "Illegal ValueTag in relocEval");
      break;
    }

  return norelocs;
}


static void
relocWrite (Reloc *r, const Value *value, unsigned char *image)
{
  const int offset = r->offset;
  
  switch (value->Tag.t)
    {
      case ValueLateLabel:
      case ValueInt:
	switch (r->Tag)
	{			/* Write out the value */
	  case RelocBranch:
	    {
	      /* The R_ARM_PC24 ELF reloc needs to happen for a "B + PC - 8"
	         instruction, while in AOF this needs to happen for a "B 0".  */
	      int bv = (!option_aof && value->Tag.t == ValueLateLabel) ?
		value->ValueInt.i + offset : value->ValueInt.i;
	      WORD w = fixBranch (r->lineno, bv);
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocBranchT:
	    {
	      WORD w = READWORD (image, offset);
	      w |= fixBranchT (r->lineno, value->ValueInt.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocCpuOffset:
	    {
	      WORD w = READWORD (image, offset);
	      /* fprintf (stderr, "RelocCpuOffset: tag = %d, lineno = %d, offset = %d, val=%s\n",
			  value->Tag.t, r->lineno, value->ValueInt.i,
			  (value->Tag.t == ValueLateLabel) ? value->ValueLate.late->symbol->str : "-"); */
	      if (value->Tag.t == ValueLateLabel)
		{
		  const Symbol *sym = value->ValueLate.late->symbol;
		  if ((sym->type & SYMBOL_AREA)
		      && (sym->area.info->type & AREA_BASED))
		    {
		      /* fprintf (stderr, "  --- based: off = %d, adjusted = %d\n",
				  offset,
				  value->ValueInt.i + offset + 8); */
		      /* value->ValueInt.i += offset + 8; */
		      w = fixCpuOffset (r->lineno, w,
					value->ValueInt.i + offset + 8);
		      w &= ~LHS_OP(15);
		      w |= LHS_OP((sym->area.info->type & 0x0f000000) >> 24);
		      /* r->offset = value->ValueInt.i + offset + 8; */
		    }
		  else
		    w = fixCpuOffset (r->lineno, w, value->ValueInt.i);
		}
	      else
		w = fixCpuOffset (r->lineno, w, value->ValueInt.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocCopOffset:
	    {
	      WORD w = READWORD (image, offset);
	      w = fixCopOffset (r->lineno, w, value->ValueInt.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocImmN:
	    {
	      /* fprintf (stderr, "RelocImmN: lineno = %d\n", r->lineno); */
	      WORD w = fixInt (r->lineno, r->extra, value->ValueInt.i);
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
	      WORD w = READWORD (image, offset);
	      w = fixAdr (r->lineno, w, value->ValueInt.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  case RelocAdrl:
	    {
	      WORD w = READWORD (image, offset);
	      WORD w1 = (value->Tag.t == ValueLateLabel) ? value->ValueLate.late->symbol->type & SYMBOL_DEFINED : 1;
	      /* warn if symbol is defined or isn't a late label */
	      fixAdrl (r->lineno, &w, &w1, value->ValueInt.i, w1);
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
	      WORD w = READWORD (image, offset);
	      w = fixImm8s4 (r->lineno, w, value->ValueInt.i);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  default:
	    errorLine (r->lineno, r->file, ErrorError, TRUE, "Cannot handle %s when value is %s", reloc2String (r->Tag), "int");
	    r->Tag = RelocNone;
	    return;
	}
	if (value->Tag.t == ValueInt || r->value.ValueCode.len == 0)	/* Value is known */
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
		translate.f = (float) value->ValueFloat.f;
	      else
		translate.d = (double) value->ValueFloat.f;
	      for (i = 0; i < r->extra; i++)
		image[offset + i] = translate.u.c[i];
	    }
	    break;
	  case RelocImmFloat:
	    {
	      WORD w = READWORD (image, offset);
	      w = fixImmFloat (r->lineno, w, value->ValueFloat.f);
	      image[offset + 3] = (w >> 24) & 0xff;
	      image[offset + 2] = (w >> 16) & 0xff;
	      image[offset + 1] = (w >> 8) & 0xff;
	      image[offset + 0] = w & 0xff;
	      break;
	    }
	  default:
	    errorLine (r->lineno, r->file, ErrorError, TRUE, "Cannot handle %s when value is %s", reloc2String (r->Tag), "float");
	    break;
	}
	r->Tag = RelocNone;
	break;
      default:
	errorLine (r->lineno, r->file, ErrorError, TRUE, "Internal relocWrite: illegal value");
	r->Tag = RelocNone;
	break;
    }
}


int
relocFix (const Symbol *area)
{
  unsigned char *image = area->area.info->image;
  Reloc *relocs;
  int norelocs = 0;

  for (relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      Value value;
      switch (relocs->value.Tag.t)
	{
	case ValueInt:
	case ValueFloat:
	  value = relocs->value;
	  break;
	case ValueCode:
	  norelocs += relocEval (relocs, &value, area);
	  break;
	default:
	  errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Internal relocFix: not a legal value");
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
  Reloc *relocs;

  for (relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      int ip;
      unsigned int How;
      AofReloc areloc;

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
	      errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Linker cannot handle RelocImmN with size %d", relocs->extra);
	      continue;
	    }
	  break;
	case RelocCpuOffset:
	  if ((relocs->extra & 0x0F000000) < 0x04000000)
	    errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE,
		       "Linker cannot handle ARMv4 extended LDR/STR");
	case RelocCopOffset:
	  if (relocs->value.Tag.t == ValueCode
	      && (relocs->value.ValueCode.c->CodeSymbol.symbol->type & SYMBOL_AREA)
	      && (relocs->value.ValueCode.c->CodeSymbol.symbol->area.info->type & AREA_BASED))
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
	  errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Linker cannot handle this");
	  continue;
	}
      areloc.Offset = armword(relocs->offset);
      for (ip = 0; ip < relocs->value.ValueCode.len; ip++)
	{
	  int loop;
	  if (relocs->value.ValueCode.c[ip].Tag == CodeValue)
	    {
	      if (relocs->value.ValueCode.c[ip].CodeValue.value.Tag.t != ValueInt)
		{
		  errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Internal relocsOutput: not an int");
		  loop = 0;
		}
	      else
		loop = relocs->value.ValueCode.c[ip++].CodeValue.value.ValueInt.i;
	    }
	  else
	    loop = 1;
	  if (relocs->value.ValueCode.c[ip].Tag != CodeSymbol)
	    errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Internal error in relocsOutput");
	  areloc.How = How | relocs->value.ValueCode.c[ip].CodeSymbol.symbol->used;
	  if (!(relocs->value.ValueCode.c[ip].CodeSymbol.symbol->type & SYMBOL_AREA))
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
  Reloc *relocs;
  for (relocs = area->area.info->relocs; relocs; relocs = relocs->more)
    {
      int ip;
      unsigned int How;
      Elf32_Rel areloc;

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
              errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Linker cannot handle RelocImmN with size %d", relocs->extra);
              continue;
            }
          break;
        case RelocCpuOffset:
          if ((relocs->extra & 0x0F000000) < 0x04000000)
            errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE,
                       "Linker cannot handle ARMv4 extended LDR/STR");
        case RelocCopOffset:
          if (relocs->value.Tag.t == ValueCode
              && (relocs->value.ValueCode.c->CodeSymbol.symbol->type & SYMBOL_AREA)
              && (relocs->value.ValueCode.c->CodeSymbol.symbol->area.info->type & AREA_BASED))
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
          errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Linker cannot handle this");
          continue;
        }
      areloc.r_offset = armword(relocs->offset);
      for (ip = 0; ip < relocs->value.ValueCode.len; ip++)
        {
	  int loop, symbol, type;
          if (relocs->value.ValueCode.c[ip].Tag == CodeValue)
            {
              if (relocs->value.ValueCode.c[ip].CodeValue.value.Tag.t != ValueInt)
		{
                  errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Internal relocsOutput: not an int");
		  loop = 0;
		}
              else
                loop = relocs->value.ValueCode.c[ip++].CodeValue.value.ValueInt.i;
            }
          else
            loop = 1;
          if (relocs->value.ValueCode.c[ip].Tag != CodeSymbol)
            errorLine (relocs->lineno, relocs->file, ErrorSerious, TRUE, "Internal error in relocsOutput");

          How |= relocs->value.ValueCode.c[ip].CodeSymbol.symbol->used;
          if (!(relocs->value.ValueCode.c[ip].CodeSymbol.symbol->type & SYMBOL_AREA))
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
