/* Instruction printing code for the ARM
   Copyright (C) 1994, 95, 96, 97, 1998 Free Software Foundation, Inc. 
   Contributed by Richard Earnshaw (rwe@pegasus.esprit.ec.org)
   Modification by James G. Smith (jsmith@cygnus.co.uk)

This file is part of libopcodes. 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option)
any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details. 

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <stdio.h>
#include <sys/stat.h>
#define DEFINE_TABLE
#include "arm-opc.h"

static char *arm_conditional[] =
{"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
 "hi", "ls", "ge", "lt", "gt", "le", "", "nv"};

static char *arm_regnames[] =
{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
 "r8", "r9", "sl", "fp", "ip", "sp", "lr", "pc"};

static char *arm_fp_const[] =
{"0.0", "1.0", "2.0", "3.0", "4.0", "5.0", "0.5", "10.0"};

static char *arm_shift[] = 
{"lsl", "lsr", "asr", "ror"};

static void
arm_decode_shift (unsigned long given, FILE *stream)
{
  fprintf (stream, "%s", arm_regnames[given & 0xf]);
  if ((given & 0xff0) != 0)
    {
      if ((given & 0x10) == 0)
	{
	  int amount = (given & 0xf80) >> 7;
	  int shift = (given & 0x60) >> 5;
	  if (amount == 0)
	    {
	      if (shift == 3)
		{
		  fprintf (stream, ", rrx");
		  return;
		}
	      amount = 32;
	    }
	  fprintf (stream, ", %s #%d", arm_shift[shift], amount);
	}
      else
	fprintf (stream, ", %s %s", arm_shift[(given & 0x60) >> 5],
		 arm_regnames[(given & 0xf00) >> 8]);
    }
}

/* Print one instruction from PC on INFO->STREAM.
   Return the size of the instruction (always 4 on ARM). */

static int
print_insn_arm (unsigned long pc, unsigned long given, FILE *stream)
{
  struct arm_opcode *insn;

  for (insn = arm_opcodes; insn->assembler; insn++)
    {
      if ((given & insn->mask) == insn->value)
	{
	  char *c;
	  for (c = insn->assembler; *c; c++)
	    {
	      if (*c == '%')
		{
		  switch (*++c)
		    {
		    case '%':
		      fprintf (stream, "%%");
		      break;

		    case 'a':
		      if (((given & 0x000f0000) == 0x000f0000)
			  && ((given & 0x02000000) == 0))
			{
			  int offset = given & 0xfff;
			  if ((given & 0x00800000) == 0)
			    offset = -offset;

			  fprintf (stream, "&%08x", offset + pc + 8);
			}
		      else
			{
			  fprintf (stream, "[%s", 
				arm_regnames[(given >> 16) & 0xf]);
			  if ((given & 0x01000000) != 0)
			    {
			      if ((given & 0x02000000) == 0)
				{
				  int offset = given & 0xfff;
				  if (offset)
				    fprintf (stream, ", #%s%d",
					  (((given & 0x00800000) == 0)
					   ? "-" : ""), offset);
				}
			      else
				{
				  fprintf (stream, ", %s",
					(((given & 0x00800000) == 0)
					 ? "-" : ""));
				  arm_decode_shift (given, stream);
				}

			      fprintf (stream, "]%s", 
				    ((given & 0x00200000) != 0) ? "!" : "");
			    }
			  else
			    {
			      if ((given & 0x02000000) == 0)
				{
				  int offset = given & 0xfff;
				  if (offset)
				    fprintf (stream, "], #%s%d",
					  (((given & 0x00800000) == 0)
					   ? "-" : ""), offset);
				  else 
				    fprintf (stream, "]");
				}
			      else
				{
				  fprintf (stream, "], %s",
					(((given & 0x00800000) == 0) 
					 ? "-" : ""));
				  arm_decode_shift (given, stream);
				}
			    }
			}
		      break;

		    case 's':
                      if ((given & 0x004f0000) == 0x004f0000)
			{
                          /* PC relative with immediate offset */
			  int offset = ((given & 0xf00) >> 4) | (given & 0xf);
			  if ((given & 0x00800000) == 0)
			    offset = -offset;

			  fprintf (stream, "&%08x", offset + pc + 8);
			}
		      else
			{
			  fprintf (stream, "[%s", 
				arm_regnames[(given >> 16) & 0xf]);
			  if ((given & 0x01000000) != 0)
			    {
                              /* pre-indexed */
			      if ((given & 0x00400000) == 0x00400000)
				{
                                  /* immediate */
                                  int offset = ((given & 0xf00) >> 4) | (given & 0xf);
				  if (offset)
				    fprintf (stream, ", #%s%d",
					  (((given & 0x00800000) == 0)
					   ? "-" : ""), offset);
				}
			      else
				{
                                  /* register */
				  fprintf (stream, ", %s%s",
					(((given & 0x00800000) == 0)
					 ? "-" : ""),
                                        arm_regnames[given & 0xf]);
				}

			      fprintf (stream, "]%s", 
				    ((given & 0x00200000) != 0) ? "!" : "");
			    }
			  else
			    {
                              /* post-indexed */
			      if ((given & 0x00400000) == 0x00400000)
				{
                                  /* immediate */
                                  int offset = ((given & 0xf00) >> 4) | (given & 0xf);
				  if (offset)
				    fprintf (stream, "], #%s%d",
					  (((given & 0x00800000) == 0)
					   ? "-" : ""), offset);
				  else 
				    fprintf (stream, "]");
				}
			      else
				{
                                  /* register */
				  fprintf (stream, "], %s%s",
					(((given & 0x00800000) == 0)
					 ? "-" : ""),
                                        arm_regnames[given & 0xf]);
				}
			    }
			}
		      break;
			  
		    case 'b':
		      fprintf (stream, "&%08x", BDISP (given) * 4 + pc + 8);
		      break;

		    case 'c':
		      fprintf (stream, "%s",
			    arm_conditional [(given >> 28) & 0xf]);
		      break;

		    case 'm':
		      {
			int started = 0;
			int reg;

			fprintf (stream, "{");
			for (reg = 0; reg < 16; reg++)
			  if ((given & (1 << reg)) != 0)
			    {
			      if (started)
				fprintf (stream, ", ");
			      started = 1;
			      fprintf (stream, "%s", arm_regnames[reg]);
			    }
			fprintf (stream, "}");
		      }
		      break;

		    case 'o':
		      if ((given & 0x02000000) != 0)
			{
			  int rotate = (given & 0xf00) >> 7;
			  int immed = (given & 0xff);
			  fprintf (stream, "#%d",
				((immed << (32 - rotate))
				 | (immed >> rotate)) & 0xffffffff);
			}
		      else
			arm_decode_shift (given, stream);
		      break;

		    case 'p':
		      if ((given & 0x0000f000) == 0x0000f000)
			fprintf (stream, "p");
		      break;

		    case 't':
		      if ((given & 0x01200000) == 0x00200000)
			fprintf (stream, "t");
		      break;

		    case 'h':
		      if ((given & 0x00000020) == 0x00000020)
			fprintf (stream, "h");
                      else
                        fprintf (stream, "b");
		      break;

		    case 'A':
		      fprintf (stream, "[%s", arm_regnames [(given >> 16) & 0xf]);
		      if ((given & 0x01000000) != 0)
			{
			  int offset = given & 0xff;
			  if (offset)
			    fprintf (stream, ", #%s%d]%s",
				  ((given & 0x00800000) == 0 ? "-" : ""),
				  offset * 4,
				  ((given & 0x00200000) != 0 ? "!" : ""));
			  else
			    fprintf (stream, "]");
			}
		      else
			{
			  int offset = given & 0xff;
			  if (offset)
			    fprintf (stream, "], #%s%d",
				  ((given & 0x00800000) == 0 ? "-" : ""),
				  offset * 4);
			  else
			    fprintf (stream, "]");
			}
		      break;

		    case 'C':
		      switch (given & 0x00090000)
			{
			case 0:
			  fprintf (stream, "_???");
			  break;
			case 0x10000:
			  fprintf (stream, "_ctl");
			  break;
			case 0x80000:
			  fprintf (stream, "_flg");
			  break;
			}
		      break;

		    case 'F':
		      switch (given & 0x00408000)
			{
			case 0:
			  fprintf (stream, "4");
			  break;
			case 0x8000:
			  fprintf (stream, "1");
			  break;
			case 0x00400000:
			  fprintf (stream, "2");
			  break;
			default:
			  fprintf (stream, "3");
			}
		      break;
			
		    case 'P':
		      switch (given & 0x00080080)
			{
			case 0:
			  fprintf (stream, "s");
			  break;
			case 0x80:
			  fprintf (stream, "d");
			  break;
			case 0x00080000:
			  fprintf (stream, "e");
			  break;
			default:
			  fprintf (stream, "<illegal precision>");
			  break;
			}
		      break;
		    case 'Q':
		      switch (given & 0x00408000)
			{
			case 0:
			  fprintf (stream, "s");
			  break;
			case 0x8000:
			  fprintf (stream, "d");
			  break;
			case 0x00400000:
			  fprintf (stream, "e");
			  break;
			default:
			  fprintf (stream, "p");
			  break;
			}
		      break;
		    case 'R':
		      switch (given & 0x60)
			{
			case 0:
			  break;
			case 0x20:
			  fprintf (stream, "p");
			  break;
			case 0x40:
			  fprintf (stream, "m");
			  break;
			default:
			  fprintf (stream, "z");
			  break;
			}
		      break;

		    case '0': case '1': case '2': case '3': case '4': 
		    case '5': case '6': case '7': case '8': case '9':
		      {
			int bitstart = *c++ - '0';
			int bitend = 0;
			while (*c >= '0' && *c <= '9')
			  bitstart = (bitstart * 10) + *c++ - '0';

			switch (*c)
			  {
			  case '-':
			    c++;
			    while (*c >= '0' && *c <= '9')
			      bitend = (bitend * 10) + *c++ - '0';
			    if (!bitend)
			      abort ();
			    switch (*c)
			      {
			      case 'r':
				{
				  long reg;
				  reg = given >> bitstart;
				  reg &= (2 << (bitend - bitstart)) - 1;
				  fprintf (stream, "%s", arm_regnames[reg]);
				}
				break;
			      case 'd':
				{
				  long reg;
				  reg = given >> bitstart;
				  reg &= (2 << (bitend - bitstart)) - 1;
				  fprintf (stream, "%d", reg);
				}
				break;
			      case 'x':
				{
				  long reg;
				  reg = given >> bitstart;
				  reg &= (2 << (bitend - bitstart)) - 1;
				  fprintf (stream, "0x%08x", reg);
				}
				break;
			      case 'f':
				{
				  long reg;
				  reg = given >> bitstart;
				  reg &= (2 << (bitend - bitstart)) - 1;
				  if (reg > 7)
				    fprintf (stream, "#%s",
					  arm_fp_const[reg & 7]);
				  else
				    fprintf (stream, "f%d", reg);
				}
				break;
			      default:
				abort ();
			      }
			    break;
			  case '`':
			    c++;
			    if ((given & (1 << bitstart)) == 0)
			      fprintf (stream, "%c", *c);
			    break;
			  case '\'':
			    c++;
			    if ((given & (1 << bitstart)) != 0)
			      fprintf (stream, "%c", *c);
			    break;
			  case '?':
			    ++c;
			    if ((given & (1 << bitstart)) != 0)
			      fprintf (stream, "%c", *c++);
			    else
			      fprintf (stream, "%c", *++c);
			    break;
			  default:
			    abort ();
			  }
			break;

		      default:
			abort ();
		      }
		    }
		}
	      else
		fprintf (stream, "%c", *c);
	    }
	  return 4;
	}
    }
  abort ();
}

int
print_insn_little_arm (unsigned char *buffer, long base_address,
		       long pc, FILE *stream)
{
  unsigned long given, offset;

  offset = pc & ~0xfc000003;
  given = buffer[offset] | (buffer[offset + 1] << 8)
    | (buffer[offset + 2] << 16) | (buffer[offset + 3] << 24);

  fprintf (stream, "%08x %08x ", base_address + pc, given);
  return print_insn_arm (base_address + pc, given, stream);
}

int
main (int argc, char *argv[])
{
  FILE *stream;
  struct stat st;
  int program_size, pc;
  long base_address;
  char *program_buffer;

  if (argc == 1 || argc > 4)
    {
      printf ("Usage: armdis <program> <base address> [<output>]\n");
      return 1;
    }

  if (stat (argv[1], &st) != 0)
    {
      printf ("-- could not stat '%s'\n", argv[1]);
      return 1;
    }

  base_address = strtol (argv[2], NULL, 16);

  program_size = st.st_size;
  program_buffer = (char *) malloc (program_size + 1);
  if (program_buffer == NULL)
    {
      printf ("-- failed to allocate %d bytes of storage\n", program_size);
      return 1;
    }

  /* Read program into memory.  */
  stream = fopen (argv[1], "rb");
  if (stream == NULL)
    {
      printf ("-- could not open '%s' for reading\n", argv[1]);
      return 1;
    }

  fread (program_buffer, 1, program_size, stream);
  fclose (stream);

  /* If a output file was given on the command line, then open it for
     writing otherwise default to stdout.  */
  if (argc == 4)
    {
      stream = fopen (argv[3], "w");
      if (stream == NULL)
	{
	  printf ("-- could not open '%s' for writing\n", argv[3]);
	  return 1;
	}
    }
  else
    stream = stdout;

  for (pc = 0; pc <= program_size; pc += 4)
    {
      print_insn_little_arm (program_buffer, base_address, pc, stream);
      fprintf (stream, "\n");
    }

  /* Close the output file.  */
  if (argc == 4)
    fclose (stream);
  
  return 0;
}


