/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "gfile.h"
#include "error.h"
#include "options.h"
#include "apcscli.h"

#include "copyright.h"

#include "VersionNum" /* CVS maintained version file */

/* Define this if you want the module header reducing if it would be 0s */
/* #define OPTIMISE_HEADERS */

/* Define this if you want to include a redundant fast service entry when
   no fast service entries have been defined, a la cmhg */
#define CMHG_REDUNDANT_FAST_SERVICE

/* Define this if you want to follow CMHG's positioning of the resource filename */
#define CMHG_RESOURCE_FILE_ORDERING

#ifdef OPTIMISE_HEADERS
/* Note: We can't optimse the headers if we're compiling as 32bit */
#define IF_OPTIMISED_HEADER(cond) if (cond && CODE26)
#else
#define IF_OPTIMISED_HEADER(cond) if (1)
#endif

static FILE *file;

static int finalise_imported = 0;

/* Special ways we relocate ourselves to point at our special data */

static void output_ADD_Lib_Reloc(void)
{
  if (opt.toolchain == tc_gcc)
    fputs("\t@ This is equivalent of 'ADD r10, r10, #0' + _Lib$Reloc$Off$DP\n"
          "\t.word\t_Lib$Reloc$Off$DP + 0xE28AA000\n", file);
  else
    fputs("\t; This is equivalent of 'ADD r10, r10, #0' + |_Lib$Reloc$Off$DP|\n"
          "\tDCD\t|_Lib$Reloc$Off$DP| + &E28AA000\n", file);
}

static void output_SUB_Lib_Reloc(void)
{
  if (opt.toolchain == tc_gcc)
    fputs("\t@ This is equivalent of 'SUB r10, r10, #0' + _Lib$Reloc$Off$DP\n"
          "\t.word\t_Lib$Reloc$Off$DP + 0xE24AA000\n", file);
  else
    fputs("\t; This is equivalent of 'SUB r10, r10, #0' + |_Lib$Reloc$Off$DP|\n"
          "\tDCD\t|_Lib$Reloc$Off$DP| + &E24AA000\n", file);
}

static void output_SUB_Lib_Reloc_S(void)
{
  if (opt.toolchain == tc_gcc)
    fputs("\t@ This is equivalent of 'SUBS r10, r10, #0' + _Lib$Reloc$Off$DP\n"
          "\t.word\t_Lib$Reloc$Off$DP + 0xE25AA000\n", file);
  else
    fputs("\t; This is equivalent of 'SUBS r10, r10, #0' + |_Lib$Reloc$Off$DP|\n"
          "\tDCD\t|_Lib$Reloc$Off$DP| + &E25AA000\n", file);
}

/* Write comment in assembler file.  */
static void output_comment(const char *comment)
{
  fputc((opt.toolchain == tc_gcc) ? '@' : ';', file);
  fputc(' ', file);
  fputs(comment, file);
}

/* Write comment in assembler file.  */
static void output_comment_vargs(const char *comment, ...)
{
  va_list varg;
  fputc((opt.toolchain == tc_gcc) ? '@' : ';', file);
  fputc(' ', file);
  va_start(varg, comment);
  vfprintf(file, comment, varg);
  va_end(varg);
}

/* Write import symbol statement in assembler file.  For GAS, this is
   not necessary.  */
static void output_import(const char *symbol)
{
  if (opt.toolchain != tc_gcc)
    fprintf(file, "\tIMPORT\t|%s|\n", symbol);
}

/* Write import symbol statement in assembler file.  For GAS, this is
   not necessary.  */
static void output_import_vargs(const char *symbol, ...)
{
  if (opt.toolchain != tc_gcc)
  {
    va_list varg;
    fputs("\tIMPORT\t|", file);
    va_start(varg, symbol);
    vfprintf(file, symbol, varg);
    va_end(varg);
    fputs("|\n", file);
  }
}

/* Write export symbol statement in assembler file.  */
static void output_export(const char *symbol)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "\t.global\t%s\n", symbol);
  else
    fprintf(file, "\tEXPORT\t|%s|\n", symbol);
}

/* Write export symbol statement in assembler file.  */
static void output_export_vargs(const char *symbol, ...)
{
  if (opt.toolchain == tc_gcc)
  {
    va_list varg;
    fputs("\t.global\t", file);
    va_start(varg, symbol);
    vfprintf(file, symbol, varg);
    va_end(varg);
    fputc('\n', file);
  }
  else
  {
    va_list varg;
    fputs("\tEXPORT\t|", file);
    va_start(varg, symbol);
    vfprintf(file, symbol, varg);
    va_end(varg);
    fputc('\n', file);
  }
}

/* Write import symbol statement with weak attribute in assembler file.  For
   GAS this just means a declaration of the symbol being weak.  */
static void output_weak_import(const char *symbol)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "\t.weak\t%s\n", symbol);
  else
    fprintf(file, "\tIMPORT\t|%s|, WEAK\n", symbol);
}

/* Write a label.  */
static void output_label(const char *label)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "%s:\n", label);
  else
    fprintf(file, "%s\n", label);
}

/* Write a label.  */
static void output_label_vargs(const char *label, ...)
{
  va_list varg;
  va_start(varg, label);
  vfprintf(file, label, varg);
  va_end(varg);
  if (opt.toolchain == tc_gcc)
    fputc(':', file);
  fputc('\n', file);
}

/* Write word value.  */
static void output_word(unsigned int wval, const char *comment)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "\t.word\t0x%x\t@ %s\n", wval, comment);
  else
    fprintf(file, "\tDCD\t&%x\t; %s\n", wval, comment);
}

/* Write word value using string.  */
static void output_word_as_str(const char *wval, const char *comment)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "\t.word\t%s\t@ %s\n", wval, comment);
  else
    fprintf(file, "\tDCD\t%s\t; %s\n", wval, comment);
}

static void output_word_vargs(const char *wval, ...)
{
  va_list varg;
  if (opt.toolchain == tc_gcc)
    fputs("\t.word\t", file);
  else
    fputs("\tDCD\t", file);
  va_start(varg, wval);
  vfprintf(file, wval, varg);
  va_end(varg);
  fputc('\n', file);
}

/* Write align statement for 4 byte alignment.  */
static void output_align(void)
{
  if (opt.toolchain == tc_gcc)
    fputs("\t.align\n", file);
  else
    fputs("\tALIGN\n", file);
}

/* Write a string which does not contain any special characters like quotes,
   top-bit chars, backslashes, newlines, etc.  */
static void output_simple_string(const char *str)
{
  if (opt.toolchain == tc_gcc)
    fprintf(file, "\t.asciz\t\"%s\"\n", str);
  else
    fprintf(file, "\t= \"%s\", 0\n", str);
}

/* Write a string which does not contain any special characters like quotes,
   top-bit chars, backslashes, newlines, etc.  */
static void output_simple_string_vargs(const char *str, ...)
{
  va_list varg;
  fputs((opt.toolchain == tc_gcc) ? "\t.asciz\t\"" : "\t= \"", file);
  va_start(varg, str);
  vfprintf(file, str, varg);
  va_end(varg);
  if (opt.toolchain == tc_gcc)
    fputs("\"\n", file);
  else
    fputs("\", 0\n", file);
}

/* Write the indication we're at the end of the assembler file.  */
static void output_endfile(void)
{
  if (opt.toolchain != tc_gcc)
    fprintf(file, "\tEND\n");
}

static void outstring(const char *s, int terminate, char newline, int translate_underscore)
{
  int inquote=0;
  int chars=0;

  while (*s || terminate)
  {
      char c=*s;
      if (!chars)
      {
        if (opt.toolchain == tc_gcc)
          fputs("\t.ascii\t", file);
        else
          fprintf(file, "\t= ");
      }
      if (translate_underscore && c=='_')
        c = ' ';
      if ((c>31) && (c<127) && (c!='"') && (c!='\\' || s[1]!='\"'))
      {
        if (!inquote)
        {
          if (opt.toolchain == tc_gcc)
          {
            if (chars)
              fputs("\n\t.ascii\t\"", file);
            else
              fputc('\"', file);
            chars = 1;
          }
          else
            chars+=fprintf(file,chars ? ",\"" : "\"");
          inquote=1;
        }
        fputc(c,file);
        chars++;
      }
      else
      {
        /* ctrl char, topbit char, '"' or any quoted char except quoted dbl quote */
        if (c=='\\')
          c = *++s;
        /* Terminate string if it is still open. */
        if (inquote)
        {
          chars+=fprintf(file, "\"");
          inquote=0;
        }
        /* Write value 'c' as one byte value. */
        if (opt.toolchain == tc_gcc)
        {
          if (chars)
          {
            fputc('\n', file);
            chars = 0;
          }
          else
            fputs("\"\"\n", file);
          fprintf(file, "\t.byte\t%u\n", (int)(c=='\n' ? newline : (c & 0xFF)));
        }
        else
          chars+=fprintf(file, "%s%u", chars ? "," : "", (int)(c=='\n'?newline:(c & 0xFF)));
      }
      if (c)
        s++;
      else
        terminate=0;
      if ((chars>60) || !(c || terminate))
      {
        /* Line too long, start new line in the assembler file.  */
        if (inquote)
          fputc('"', file);
        fputc('\n', file);
        inquote=0;
        chars=0;
      }
  }
}

static void asm_header(void)
{
  output_comment("Generated by CMunge " Module_FullVersionAndDate "\n");
  output_comment("CMunge " Copyright_CMunge "\n\n");

  if (opt.toolchain == tc_gcc)
    fputs("\t.ident\t\"CMunge " Module_FullVersionAndDate "\"\n\n", file);

  if (opt.base)
  {
    output_import("Image$$RO$$Base");
    output_export("Image__RO_Base");
    if (opt.toolchain == tc_gcc)
    {
      fputs("\
\t.section .rodata\n\
\t.align 2\n\
\t.type Image__RO_Base, %object\n\
\t.size Image__RO_Base, 4\n", file);
    }
    else
      fputs("\tAREA\tCode_Description, DATA, REL\n", file);
    output_label("Image__RO_Base");
    output_word_as_str("Image$$RO$$Base", "");
    fputc('\n', file);
  }

  if (opt.toolchain == tc_gcc)
  {
    /* FIXME: adr vs adrl support needs to be written when I start understanding the
       GAS macro assembler syntax better. */
    fputs("\
.macro\taddr reg:req object:req cc\n\
\tadrl\\cc\t\\reg, \\object\n\
.endm\n\n", file);
  }
  else
  {
    if (opt.toolchain == tc_lcc)
    {
      fprintf(file, "\
        GBLL    UsingAS\n\
UsingAS SETL    {TRUE}\n");
    }
    fprintf(file, "\
; Some handy macros\n\
        GBLL    immediate\n\
        MACRO\n\
        Immediate $var\n\
immediate SETL  {FALSE}\n\
        LCLA    count\n\
        LCLA    varcopy\n\
varcopy SETA    $var\n\
        WHILE   count <= 30\n\
 [ ((varcopy:SHL:count) + (varcopy:SHR:(32-count))) :AND: (:NOT: &FF) = 0\n\
immediate SETL  {TRUE}\n\
        MEXIT\n\
 ]\n\
count   SETA    count + 2\n\
        WEND\n\
        MEND\n\
\n\
        MACRO\n\
$label  addr    $reg, $object, $cc\n\
    [ :DEF: UsingAS\n\
$label  ADR$cc.L $reg, $object\n\
    |\n\
        LCLA    value\n\
value   SETA    .-$object+8\n\
        Immediate &$value\n\
 [ immediate\n\
$label  ADR$cc  $reg, $object\n\
 |\n\
$label  ADR$cc.L $reg, $object\n\
 ]\n\
    ]\n\
        MEND\n\
\n\
");
  }

  output_comment("Now the REAL module header\n");

  if (opt.toolchain == tc_gcc)
  {
    /* Put module header and all data in .init section and defining _start
       symbol here.  This means that the linker will start with this (and
       place it at front) and work further on it to get all the implied
       unresolved symbols resolved.  */
    fputs("\t.section\t\".init\",\"x\"\n"
          "\t.global\t_start\n"
          "\t.type\t_start, %function\n"
          "_start:\n", file);
  }
  else
    fprintf(file, "\tAREA\t|!Header$$code|, CODE, READONLY%s%s\n\n",
                  (opt.apcs & APCS_REENTRANT) ? ", PIC, REENTRANT" : "",
                  (opt.apcs & APCS_FPE3) ? ", FP3" : "");

  /* Things that everyone needs */
  output_import("_Lib$Reloc$Off$DP");
}

static void mod_header(void)
{
  if (opt.mode_errors)
    return;

  output_export("__module_header");
  output_label("__module_header"); fputc('\n', file);

  output_label("_CMUNGE_origin");
  if (opt.runnable)
    output_word_as_str("_CMUNGE_start\t\t-_CMUNGE_origin", "Start offset");
  else
    output_word(0, "Start offset");
  output_word_as_str("_CMUNGE_init\t\t-_CMUNGE_origin", "Initialisation offset");
  output_word_as_str("_CMUNGE_fin\t\t-_CMUNGE_origin", "Finalisation offset");
  if (opt.service)
    output_word_as_str("_CMUNGE_service\t\t-_CMUNGE_origin", "Service handler offset");
  else
    output_word(0, "Service handler offset");
  output_word_as_str("_CMUNGE_title\t\t-_CMUNGE_origin", "Title string offset");
  output_word_as_str("_CMUNGE_help\t\t-_CMUNGE_origin", "Help string offset");

  if (opt.commands)
    output_word_as_str("_CMUNGE_command\t\t-_CMUNGE_origin", "Command table offset");
  else
    IF_OPTIMISED_HEADER(opt.swi_base || opt.swi_names || opt.swi_decoder || opt.mfile)
      output_word(0, "No commands specified");

  if (opt.swi_base)
  {
    output_word(opt.swi_base, "SWI chunk base number");
    output_word_as_str("_CMUNGE_swi_handler\t-_CMUNGE_origin", "SWI handler offset");
  }
  else
  {
    IF_OPTIMISED_HEADER(opt.swi_names || opt.swi_decoder || opt.mfile)
    {
      output_word(0, "SWI chunk base number");
      output_word(0, "SWI handler offset");
    }
  }
  if (opt.swi_names)
    output_word_as_str("_CMUNGE_swi_table\t-_CMUNGE_origin", "SWI table offset");
  else
    IF_OPTIMISED_HEADER(opt.swi_decoder || opt.mfile)
      output_word(0, "SWI table offset");
  if (opt.swi_decoder)
    output_word_as_str("_CMUNGE_swi_decoder\t-_CMUNGE_origin", "SWI decoding code offset");
  else
    IF_OPTIMISED_HEADER(opt.mfile)
      output_word(0, "SWI decoding code offset");
  if (opt.mfile)
    output_word_as_str("_CMUNGE_message_file\t-_CMUNGE_origin", "Message file filename offset");
  else
  {
    IF_OPTIMISED_HEADER(opt.mfile)
      output_word(0, "Message file filename offset");
  }
  if (CODE26)
  {}
  else
    output_word_as_str("_CMUNGE_module_flags\t-_CMUNGE_origin", "Module flags offset");
}

static void strings(void)
{
  if (opt.mode_errors)
    return;

  output_label("_CMUNGE_title");
  outstring(opt.title,1,'\n',0);
  output_label("_CMUNGE_help");
  outstring(opt.help,1,'\n',1);
#ifndef CMHG_RESOURCE_FILE_ORDERING
  if (opt.mfile)
  {
    output_align();
    output_label("_CMUNGE_message_file");
    outstring(opt.mfile,1,'\n',0);
  }
#endif
}

static void swi_table(void)
{
  swi_list l;

  if (!opt.swi_names)
    return;

  output_label("_CMUNGE_swi_table");
  for (l = opt.swi_names; l != NULL; l = l->next)
    output_simple_string(l->name);
  output_simple_string("");
}

static void commands(void)
{
/* JRF: 32bit'd */
  help_list l;
  int n,nentry;
  int info;

  /* FIXME: Help entries can be commands - should work; or at least mirror CHMG */
  if (!opt.commands)
    return;

  for (l = opt.commands, n = 0; l != NULL; l = l->next, ++n)
  {
    if (l->help_text)
    {
      output_label_vargs("_CMUNGE_ht%d", n);
      if ((l->invalid_syntax) && (l->add_syntax))
        outstring(l->help_text,0,'\r',0);
      else
        outstring(l->help_text,1,'\r',0);
    }
    if (l->invalid_syntax)
    {
      output_label_vargs("_CMUNGE_is%d", n);
      outstring(l->invalid_syntax,1,'\r',0);
    }
  }

  output_align();

  nentry = 0;
  output_label("_CMUNGE_command");
  for (l = opt.commands, n = 0; l != NULL; l = l->next, ++n)
  {
    output_simple_string(l->name);
    output_align();
    if (l->no_handler)
      output_word(0, "No handler");
    else
    {
      output_word_vargs("_CMUNGE_ce%d\t- _CMUNGE_origin", n);
      nentry++;
    }
    info = l->min_args | (l->max_args<<16) | (l->gstrans<<8) |
           (l->fs_command ? (1U<<31) : 0) | (l->status ? (1<<30) : 0) |
           (l->configure ? (1<<30) : 0) | (l->help ? (1<<29) : 0) |
           (l->international ? (1<<28) : 0);
    output_word(info, "");
    if (l->invalid_syntax)
      output_word_vargs("_CMUNGE_is%d\t- _CMUNGE_origin", n);
    else
      output_word(0, "No invalid syntax line");
    if (l->help_text)
      output_word_vargs("_CMUNGE_ht%d\t- _CMUNGE_origin", n);
    else
      output_word(0, "No help line");
  }
  /* CMHG *doesn't* output the following, because the low byte of the next
     instruction is 0. Hacky or what? For compatibility I'll do the same,
     except when there are no entry points to follow, in which case we
     need the 0 as the following code doesn't have any.
   */
  if (nentry == 0)
  {
    output_word(0, "");
    /* Of course, if there are no entry points there is no point in writing
       out any veneers. */
    return;
  }

  nentry=0;
  for (l = opt.commands, n = 0; l != NULL; l = l->next, ++n)
  {
    if (!l->no_handler)
    {
      if (nentry != 0)
        fprintf(file, "\tB\t_CMUNGE_ce\n");
      output_label_vargs("_CMUNGE_ce%d", n);
      fprintf(file, "\tMOV\tr2,#%d\n", nentry++);
    }
  }
  output_label("_CMUNGE_ce");
  fprintf(file, "\tSTMDB\tr13!,{r0,r10,r11,r14}\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr3,r12\n");
  fprintf(file, "\tLDR\tr12,[r12]\n");
  fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
  fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
  output_ADD_Lib_Reloc();
  fprintf(file, "\tMOV\tr11,#0\n");
  if (opt.command_codesupplied)
  {
    help_list l;
    int numentries=0;

    for (l = opt.commands; l; l = l->next)
    {
      if (!l->no_handler)
        numentries++;
    }
    fprintf(file, "\tMOV\tr14,pc\n");
    fprintf(file, "\tADD\tr14,r14,# _CMUNGE_com_ret - _CMUNGE_com_pc\n");
    output_label("_CMUNGE_com_pc");
    fprintf(file, "\tCMP\tr2,#%i\n",numentries);
    fprintf(file, "\tADDLO\tpc,pc,r2,LSL #2\n");
    if (opt.helpfn)
    {
      output_import(opt.helpfn);
      fprintf(file, "\tB\t%s\n", opt.helpfn);
    }
    else
      fprintf(file, "\tB\t_CMUNGE_nocom_handler\n");

    for (l = opt.commands; l; l = l->next)
    {
      if (!l->no_handler)
      {
        if (l->handler)
        {
          output_import(l->handler);
          fprintf(file, "\tB\t%s\n", l->handler);
        }
        else
        {
          if (opt.commands)
            fprintf(file, "\tB\t%s\n", opt.helpfn);
          else
            fprintf(file, "\tB\t_CMUNGE_nocom_handler\n");
        }
      }
    }
    if (!opt.helpfn)
    {
      output_label("_CMUNGE_nocom_handler");
      fprintf(file, "\tMOV\tr0,#0\n"); /* no error (wrong!) */
    }
    output_label("_CMUNGE_com_ret");
  }
  else
  {
    output_import(opt.helpfn);
    fprintf(file, "\tBL\t%s\n", opt.helpfn);
  }
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tLDMIA\tr13!,{r1,r10,r11,r14}\n");
  fprintf(file, "\tCMP\tr1,r0\n");
  fprintf(file, "\tCMPNE\tr0,#0\n");
  if (CODE26)
    fprintf(file, "\tBICEQS\tpc,r14,#0x10000000\n");
  else
    fprintf(file, "\tMOVEQ\tpc,r14\n");
  fprintf(file, "\tCMN\tr0,#1\n");
  fprintf(file, "\tMOVEQ\tr0,#0\n");
  if (CODE26)
    fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
  else
  {
    fprintf(file, "\tMOV\tr1,#0\n");
    fprintf(file, "\tCMP\tr1,#1<<31\n");
    fprintf(file, "\tMOV\tpc,r14\n");
  }
}

static unsigned int representable(unsigned int i)
{
  unsigned int mask;

  mask = 255;
  while (((i & mask) & ~(mask<<2)) == 0)
    mask = (mask << 2) | (mask >> 30);
  return i & mask;
}

static void service(void)
{
/* JRF: 32bit */
  unsigned int n, m;
  int_list l;
  int big;
  int first;

  if (!opt.service)
    return;
  if (opt.services)
  {
    unsigned int min = 0;
    unsigned int found;

    output_label("_CMUNGE_service_fast");
    output_word(0, "");
    output_word_as_str("_CMUNGE_service_fast_entry\t- _CMUNGE_origin", "");
    /* List services in ascending order */
    do {
      found = 0;
      for (l = opt.services; l; l = l->next)
      {
        if (l->val > min && ((l->val < found) || (found == 0)))
          found = l->val;
      }
      output_word(found, "Service call number");
      min = found;
    } while (found);
    output_word_as_str("_CMUNGE_service_fast\t\t- _CMUNGE_origin", "");
    output_label("_CMUNGE_service");
    fprintf(file, "\tMOV\tr0,r0\n");
  }
  else
  {
#ifdef CMHG_REDUNDANT_FAST_SERVICE
    output_word(0, "");
    output_label("_CMUNGE_service");
    fprintf(file, "\tMOV\tr0,r0\n");
#else
    output_label("_CMUNGE_service");
#endif
  }

  if (opt.services)
  {
    /* Do we have any 'big' numbers? */
    big = 0;
    for (l = opt.services; l; l = l->next)
    {
      if (representable(l->val) != l->val)
      {
        big = 1;
        break;
      }
    }
    /* Now the rejection code */
    if (big)
      fprintf(file, "\tSTR\tr14,[r13,#-4]!\n");

    first = 1;
    for (l = opt.services; l != NULL; l = l->next)
    {
      n = l->val;
      m = representable(n);
      if (n == m)
      {
        if (first)
          fprintf(file, "\tTEQ\tr1,#0x%02x\n", n);
        else
          fprintf(file, "\tTEQNE\tr1,#0x%02x\n", n);
      }
      else
      {
        fprintf(file, "\tMOV\tr14,#0x%02x\n", m);
        n &= ~m;
        do {
          m = representable(n);
          fprintf(file, "\tORR\tr14,r14,#0x%02x\n", m);
          n &= ~m;
        } while (n);
        if (first)
          fprintf(file, "\tTEQ\tr1,r14\n");
        else
          fprintf(file, "\tTEQNE\tr1,r14\n");
      }
      first = 0;
    }
    if (big)
    {
      if (CODE26)
        fprintf(file, "\tLDMNEIA\tr13!,{pc}^\n");
      else
        fprintf(file, "\tLDMNEIA\tr13!,{pc}\n");
      /* CMHG is inefficient here, but I'll match it */
      fprintf(file, "\tLDR\tr14,[r13],#4\n");
    }
    else
    {
      if (CODE26)
        fprintf(file, "\tMOVNES\tpc,r14\n");
      else
        fprintf(file, "\tMOVNE\tpc,r14\n");
    }
  }
  output_label("_CMUNGE_service_fast_entry");
  fprintf(file, "\tSTMDB\tr13!,{r0-r11,r14}\n");
  fprintf(file, "\tMOV\tr0,r1\n");
  fprintf(file, "\tMOV\tr1,r13\n");
  if (CODE26)
  {
    fprintf(file, "\tMOV\tr6,pc\n");
    fprintf(file, "\tBIC\tr3,r6,#3\n");
    fprintf(file, "\tTEQP\tr3,#3\n");
  }
  else
  {
    fprintf(file, "\tTEQ      pc,pc\n");
    fprintf(file, "\tMOVNE    r8,pc\n");
    fprintf(file, "\tBICNE    r3,r8,#3\n");
    fprintf(file, "\tTEQNEP   r3,#3\n");
    fprintf(file, "\tMRSEQ    r6,cpsr\n");
    fprintf(file, "\tORREQ    r3,r6,#3\n");
    fprintf(file, "\tMSREQ    cpsr_ctl,r3\n");
  }
  fprintf(file, "\tMOV\tr11,#0\n");
  fprintf(file, "\tMOV\tr7,r14\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr2,r12\n");
  fprintf(file, "\tLDR\tr12,[r12,#0]\n");
  if (CODE26)
  {
    fprintf(file, "\tLDMIB\tr12,{r8,r9}\n");
    fprintf(file, "\tSTMIA\tr10,{r8,r9}\n");
  }
  else
  {
    fprintf(file, "\tLDMIB\tr12,{r12,r14}\n");
    fprintf(file, "\tSTMIA\tr10,{r12,r14}\n");
  }
  output_ADD_Lib_Reloc();
  output_import(opt.service);
  fprintf(file, "\tBL\t%s\n", opt.service);
  fprintf(file, "\tMOV\tr14,r7\n");
  if (CODE26)
  {
    fprintf(file, "\tTEQP\tr6,#0\n");
  }
  else
  {
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tMSREQ\tcpsr_ctl,r6\n");
    fprintf(file, "\tTEQNEP\tr8,#0\n");
  }
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  if (CODE26)
    fprintf(file, "\tLDMIA\tr13!,{r0-r11,pc}^\n");
  else
    fprintf(file, "\tLDMIA\tr13!,{r0-r11,pc}\n");
}

static void start(void)
{
  if (opt.runnable)
  {
    output_label("_CMUNGE_start");
    switch (opt.runnable)
    {
      case run_blib:
      case run_clib:
        {
          const char *routine;
          if (opt.runnable == run_blib)
            routine="_blib_entermodule";
          else
            routine="_clib_entermodule";

          output_import(routine);
          fprintf(file, "\tB\t%s\n",routine);
        }
        break;

      case run_rmaapp_simple: /* an incredibly simple task using RMA space */
        fprintf(file, "OS_Module * 30\n");
        fprintf(file, "OS_Exit * 17\n");
        fprintf(file, "OS_GenerateError * 43\n");
        fprintf(file, "\n");
        output_weak_import("__root_stack_size");
        output_import("main");
        output_export("_CMUNGE_stack");
        fprintf(file, "\tMOV\tv1,r0 \t; Save command tail\n");
        fprintf(file, "\tLDR\tr12,[r12]\t; Workspace ptr from private word\n");
        fprintf(file, "\tLDMIB\tr12,{v2,v3}\t; Module relocations\n");
        fprintf(file, "\tLDR\tv4,_CMUNGE_stackptr\t; A Ptr to stack variable\n");
        fprintf(file, "\tADD\tv4,v4,v3\t; gets relocated for user variable\n");
        fprintf(file, "\tLDR\tr0,[v4]\t; stack chunk\n");
        fprintf(file, "\tTEQ\tr0,#0\t; is it 0 ?\n");
        fprintf(file, "\tBNE\t_CMUNGE_apprunning\t; if not, there's a stack already\n");
        fprintf(file, "\tMOV\tr0,#6\n");
        fprintf(file, "\tLDR\tr3, =|__root_stack_size|\n");
        fprintf(file, "\tTEQ\tr3, #0\n");
        fprintf(file, "\tMOVEQ\tr3,#8192\t; default stack size\n");
        fprintf(file, "\tSWI\tOS_Module\n");
        fprintf(file, "\tMOV\tsl,r2\n");
        fprintf(file, "\tSTR\tsl,[v4]\n");
        fprintf(file, "\tADD\tsp,sl,r3\n");
        fprintf(file, "\tSTMIA\tsl,{v2,v3}\n");
        fprintf(file, "\n");
        output_ADD_Lib_Reloc();
        fprintf(file, "\n");
        fprintf(file, "\tMOV\tfp,#0\n");
        fprintf(file, "\tMOV\ta1,v1\n");
        fprintf(file, "\tBL\tmain\n");
        fprintf(file, "\n");
        output_SUB_Lib_Reloc();
        fprintf(file, "\n");
        fprintf(file, "\tMOV\tv1,r0\n");
        fprintf(file, "\n");
        fprintf(file, "\tMOV\tr2,sl\n");
        fprintf(file, "\tMOV\tr0,#7\n");
        fprintf(file, "\tSWI\tOS_Module\n");
        fprintf(file, "\tMOV\tr0,#0\n");
        fprintf(file, "\tSTR\tr0,[v4]\n");
        fprintf(file, "\n");
        fprintf(file, "\tMOV\tr0,v1\t; FIXME: this is wrong - it's ShareFS code\n");
        fprintf(file, "\tSWI\tOS_Exit\n");
        fprintf(file, "\n");
        output_label("_CMUNGE_freeappstack");
        fprintf(file, "\tSTMFD\tsp!,{r0-r4,r14}\n");
        fprintf(file, "\tLDR\tr4,_CMUNGE_stackptr\t; A Ptr to stack variable\n");
        fprintf(file, "\tLDR\tr3,[sl,#-536]\n");
        fprintf(file, "\tADD\tr4,r4,r3\t; gets relocated for user variable\n");
        fprintf(file, "\tLDR\tr2,[r4]\t; stack chunk pointer\n");
        fprintf(file, "\tTEQ\tr2,#0\t; is it allocated ?\n");
        fprintf(file, "\tMOVNE\tr0,#7\t; if so, free it\n");
        fprintf(file, "\tSWINE\tOS_Module\n");
        fprintf(file, "\tMOV\tr0,#0\n");
        fprintf(file, "\tSTR\tr0,[r4]\n");
        if (CODE26)
          fprintf(file, "\tLDMFD\tsp!,{r0-r4,pc}^\n");
        else
        {
          fprintf(file, "\tCMP\tpc,#0\t; clear V\n");
          fprintf(file, "\tLDMFD\tsp!,{r0-r4,pc}^\n");
        }
        fprintf(file, "\n");
        output_label("_CMUNGE_stackptr");
        output_word_as_str("_CMUNGE_stack", "");
        fprintf(file, "\n");
        output_label("_CMUNGE_apprunning");
        fprintf(file, "\tADR\tr0,_CMUNGE_apprunning_error\n");
        fprintf(file, "\tSWI\tOS_GenerateError\n");
        output_label("_CMUNGE_apprunning_error");
        output_word(0, "FIXME: This is a bad error number to use");
        output_simple_string_vargs("%s already running", opt.title);
        output_align();
        break;

      default:
        ErrorFatal("Start code type confusion");
    }
  }
}

static void init(void)
{
/* JRF: 32bit ok */
  if (opt.mode_errors)
    return;

  fputc('\n', file);
  output_label("_CMUNGE_init");
  fprintf(file, "\tSTMFD\tr13!,{r7-r11,r14}\n");
  fprintf(file, "\tMOV\tr0,#%i\n",opt.reentrant ? 1 : 0);
  if (opt.lib_init)
  {
    output_import(opt.lib_init);
    fprintf(file, "\tBL\t%s\n", opt.lib_init);
  }
  else
  {
    output_import("_clib_initialisemodule");
    fprintf(file, "\tBL\t_clib_initialisemodule\n");
  }
  if (!opt.init && !opt.cplusplus)
    fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}\n");
  else
  {
    if (CODE26)
    {
      fprintf(file, "\tLDMVSIA\tr13!,{r7-r11,pc}\n");
      fprintf(file, "\tMOV\tr6,r0\n");
      output_SUB_Lib_Reloc();
      fprintf(file, "\tLDMIA\tr10,{r0,r1}\n");
      fprintf(file, "\tSUB\tr10,r10,#0x14\n");
      fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
      fprintf(file, "\tSTMIA\tr10,{r0,r1}\n");
      output_ADD_Lib_Reloc();
      fprintf(file, "\tMOV\tr11,#0\n");
      if (opt.cplusplus)
      {
        if (opt.toolchain == tc_gcc)
        {
          output_import("__gccmain");
          fprintf(file, "\tBL\t__gccmain\n");
        }
        else
        {
          output_import("______main");
          fprintf(file, "\tBL\t______main\n");
        }
      }
      if (opt.init)
      {
        fprintf(file, "\tADD\tr0,r13,#0xc\n");
        fprintf(file, "\tLDMIA\tr0,{r0,r1}\n");
        fprintf(file, "\tMOV\tr2,r6\n");
        output_import(opt.init);
        fprintf(file, "\tBL\t%s\n", opt.init);
        fprintf(file, "\tTEQ\tr0,#0\n");
        fprintf(file, "\tBEQ\t_CMUNGE_init_10\n");
        fprintf(file, "\tSTR\tr0,[r13,#-4]!\n");
        fprintf(file, "\tMOV\tr0,r6\n");
        if (!finalise_imported)
        {
          output_import("_clib_finalisemodule");
          finalise_imported = 1;
        }
        fprintf(file, "\tBL\t_clib_finalisemodule\n");
        fprintf(file, "\tLDR\tr0,[r13],#4\n");
        output_label("_CMUNGE_init_10");
        output_SUB_Lib_Reloc();
        fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
        fprintf(file, "\tTEQ\tr0,#0\n");
        fprintf(file, "\tLDMEQIA\tr13!,{r7-r11,pc}^\n");
        fprintf(file, "\tLDMIA\tr13!,{r7-r11,r14}\n");
        fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
      }
      else
      {
        fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
        fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}^\n");
      }
    }
    else
    {
      fprintf(file, "\tLDMVSIA\tr13!,{r7-r11,pc}\n");
      fprintf(file, "\tMOV\tr6,r0\n");
      output_SUB_Lib_Reloc();
      fprintf(file, "\tLDMIA\tr10,{r0,r1}\n");
      fprintf(file, "\tSUB\tr10,r10,#0x14\n");
      fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
      fprintf(file, "\tSTMIA\tr10,{r0,r1}\n");
      output_ADD_Lib_Reloc();
      fprintf(file, "\tMOV\tr11,#0\n");
      if (opt.cplusplus)
      {
        if (opt.toolchain == tc_gcc)
        {
          output_import("__gccmain");
          fprintf(file, "\tBL\t__gccmain\n");
        }
        else
        {
          output_import("______main");
          fprintf(file, "\tBL\t______main\n");
        }
      }
      if (opt.init)
      {
        fprintf(file, "\tADD\tr0,r13,#0xc\n");
        fprintf(file, "\tLDMIA\tr0,{r0,r1}\n");
        fprintf(file, "\tMOV\tr2,r6\n");
        output_import(opt.init);
        fprintf(file, "\tBL\t%s\n", opt.init);
        fprintf(file, "\tTEQ\tr0,#0\n");
        fprintf(file, "\tBEQ\t_CMUNGE_init_10\n");
        fprintf(file, "\tSTR\tr0,[r13,#-4]!\n");
        fprintf(file, "\tMOV\tr0,r6\n");
        if (!finalise_imported)
        {
          output_import("_clib_finalisemodule");
          finalise_imported = 1;
        }
        fprintf(file, "\tBL\t_clib_finalisemodule\n");
        fprintf(file, "\tLDR\tr0,[r13],#4\n");
        output_label("_CMUNGE_init_10");
        output_SUB_Lib_Reloc();
        fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
        fprintf(file, "\tCMP\tr0,#0\n");
        fprintf(file, "\tMOVNE\tr1,#0\n");
        fprintf(file, "\tCMPNE\tr1,#1<<31\n");
        fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}\n");
      }
      else
      {
        fprintf(file, "\tCMP\tr0,r0\n"); /* Clear V */
        fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
        fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}\n");
      }
    }
  }
}

static void final(void)
{
  if (opt.mode_errors)
    return;

  output_label("_CMUNGE_fin");
  if (opt.final)
  {
    /* JRF: 32bit ok */
    fprintf(file, "\tSTMDB\tr13!,{r7-r12,r14}\n");
    fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
    fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
    fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tLDR\tr12,[r12,#0]\n");
    fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
    fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
    output_ADD_Lib_Reloc();
    fprintf(file, "\tMOV\tr11,#0\n");
    fprintf(file, "\tADD\tr0,r13,#0xc\n");
    fprintf(file, "\tLDMIA\tr0,{r0-r2}\n");
    output_import(opt.final);
    fprintf(file, "\tBL\t%s\n", opt.final);
    fprintf(file, "\tCMP\tr0,#0\n");
    if (CODE26)
    {
      fprintf(file, "\tLDREQ\tr0,[r13,#0x14]\n");
      if (!finalise_imported)
      {
        output_import("_clib_finalisemodule");
        finalise_imported = 1;
      }
      fprintf(file, "\tBLEQ\t_clib_finalisemodule\n");
      if (opt.runnable == run_rmaapp_simple)
        fprintf(file, "\tBLEQ\t_CMUNGE_freeappstack\n");
    }
    else
    {
      fprintf(file, "\tBNE\t_CMUNGE_final_10\n");
      fprintf(file, "\tLDR\tr0,[r13,#0x14]\n");
      if (!finalise_imported)
      {
        output_import("_clib_finalisemodule");
        finalise_imported = 1;
      }
      fprintf(file, "\tBL\t_clib_finalisemodule\n");
      if (opt.runnable == run_rmaapp_simple)
        fprintf(file, "\tBL\t_CMUNGE_freeappstack\n");
      fprintf(file, "\tMOVS\tr0,#0\n");
      output_label("_CMUNGE_final_10");
    }
    output_SUB_Lib_Reloc();
    fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
    if (CODE26)
    {
      fprintf(file, "\tLDMEQIA\tr13!,{r7-r12,pc}^\n");
      fprintf(file, "\tLDMIA\tr13!,{r7-r12,r14}\n");
      fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
    }
    else
    {
      fprintf(file, "\tMOVNE\tr1,#0\n");
      fprintf(file, "\tCMPNE\tr1,#1<<31\n");
      fprintf(file, "\tLDMIA\tr13!,{r7-r12,pc}\n");
    }
  } else {
    fprintf(file, "\tSTMDB\tr13!,{r7-r11,r14}\n");
    fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
    fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
    fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tMOV\tr0,r12\n");
    fprintf(file, "\tLDR\tr12,[r12]\n");
    fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
    fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
    output_ADD_Lib_Reloc();
    fprintf(file, "\tMOV\tr11,#0\n");
    if (!finalise_imported)
    {
      output_import("_clib_finalisemodule");
      finalise_imported = 1;
    }
    fprintf(file, "\tBL\t_clib_finalisemodule\n");
    if (opt.runnable == run_rmaapp_simple)
      fprintf(file, "\tBLVC\t_CMUNGE_freeappstack\n");
    if (CODE26)
      output_SUB_Lib_Reloc();
    else
      output_SUB_Lib_Reloc_S();
    fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
    if (CODE26)
      fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}^\n");
    else
      fprintf(file, "\tLDMIA\tr13!,{r7-r11,pc}\n");
  }
}

static void swi_handler(void)
{
  /* JRF: 32bit ok */
  if (!opt.swi_base)
    return;
  output_label("_CMUNGE_swi_handler");
  fprintf(file, "\tSTMDB\tr13!,{r0-r9,r14}\n");
  if (CODE26)
  {}
  else
    fprintf(file, "\tMRS\tr8,cpsr\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tMOV\tr0,r11\n");
  fprintf(file, "\tMOV\tr1,r13\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr2,r12\n");
  fprintf(file, "\tLDR\tr12,[r12]\n");
  fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
  fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
  output_ADD_Lib_Reloc();
  fprintf(file, "\tMOV\tr11,#0\n");
  if (opt.swi_codesupplied)
  {
    swi_list l;
    int numswis;

    /* First one must be valid */
    for (l = opt.swi_names->next, numswis = 0; l; l = l->next, ++numswis)
      /* */;
    fprintf(file, "\tMOV\tr14,pc\n");
    fprintf(file, "\tADD\tr14,r14,# _CMUNGE_swi_ret - _CMUNGE_swi_pc\n");
    output_label("_CMUNGE_swi_pc");
    fprintf(file, "\tCMP\tr0,#%i\n",numswis);
    fprintf(file, "\tADDLT\tpc,pc,r0,LSL #2\n");
    if (opt.swi_handler)
    {
      output_import(opt.swi_handler);
      fprintf(file, "\tB\t%s\n", opt.swi_handler);
    }
    else
      fprintf(file, "\tB\t_CMUNGE_noswi_handler\n");

    /* First one must be valid */
    for (l = opt.swi_names->next; l; l = l->next)
    {
      if (l->handler)
      {
        output_import(l->handler);
        fprintf(file, "\tB\t%s\n", l->handler);
      }
      else
      {
        if (opt.swi_handler)
          fprintf(file, "\tB\t%s\n", opt.swi_handler);
        else
          fprintf(file, "\tB\t_CMUNGE_noswi_handler\n");
      }
    }
    if (!opt.swi_handler)
    {
      output_label("_CMUNGE_noswi_handler");
      fprintf(file, "\tMOV\tr0,#-1\n"); /* 'SWI not known' */
    }
    output_label("_CMUNGE_swi_ret");
  }
  else
  {
    output_import(opt.swi_handler);
    fprintf(file, "\tBL\t%s\n", opt.swi_handler);
  }
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tTEQ\tr0,#0\n");
  if (CODE26)
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r9,pc}^\n");
  else
  {
    fprintf(file, "\tBNE\t_CMUNGE_swi_10\n");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDMNEIA\tr13!,{r0-r9,pc}^\n");
    output_word(0xe12ff008, "MSR cpsr_cxsf,r8");
    fprintf(file, "\tLDMIA\tr13!,{r0-r9,pc}\n");
    output_label("_CMUNGE_swi_10");
  }
  fprintf(file, "\tADD\tr13,r13,#4\n");
  fprintf(file, "\tCMN\tr0,#1\n");
  if (CODE26)
  {
    fprintf(file, "\tLDMNEIA\tr13!,{r1-r9,r14}\n");
    fprintf(file, "\tORRNES\tpc,r14,#0x10000000\n");
  }
  else
  {
    fprintf(file, "\tBEQ\t_CMUNGE_swi_30\n");
    output_label("_CMUNGE_swi_20");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDMNEIA\tr13!,{r1-r9,r14}\n");
    fprintf(file, "\tORRNES\tpc,r14,#0x10000000\n");
    fprintf(file, "\tORR\tr8,r8,#0x10000000\n");
    output_word(0xe12ff008, "MSR cpsr_cxsf,r8");
    fprintf(file, "\tLDMIA\tr13!,{r1-r9,pc}\n");
    output_label("_CMUNGE_swi_30");
  }
  fprintf(file, "\tADR\tr0,_CMUNGE_sht\n");
  fprintf(file, "\tMOV\tr1,#0\n");
  fprintf(file, "\tMOV\tr2,#0\n");
  fprintf(file, "\taddr\tr4,_CMUNGE_title\n");
  fprintf(file, "\tSWI\t0x61506\t"); output_comment("XMessageTrans_ErrorLookup\n");
  if (CODE26)
    fprintf(file, "\tLDMIA\tr13!,{r1-r9,pc}\n");
  else
    fprintf(file, "\tB\t_CMUNGE_swi_20\n");
  output_label("_CMUNGE_sht");
  output_word(0x1e6 , "");
  output_simple_string("BadSWI");
  output_align();
}

// FIXME: some generated ARM code can be shared with pieces emited in swi_handler()
static void pdriver_handler(void)
{
  pdriver_list *l;
  int numpdreasons;

  if (!opt.pdriver_entry)
    return;

  output_export(opt.pdriver_entry);
  output_label(opt.pdriver_entry);
  fprintf(file, "\tSTMDB\tr13!,{r0-r9,r14}\n");
  if (CODE26)
  {}
  else
    fprintf(file, "\tMRS\tr8,cpsr\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tMOV\tr0,r11\n");
  fprintf(file, "\tMOV\tr1,r13\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr2,r12\n");
  fprintf(file, "\tLDR\tr12,[r12]\n");
  fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
  fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
  output_ADD_Lib_Reloc();
  fprintf(file, "\tMOV\tr11,#0\n");
  for (l = opt.pdriver_names, numpdreasons = 0; l; l = l->next, ++numpdreasons)
    /* */;
  fprintf(file, "\tMOV\tr14,pc\n");
  fprintf(file, "\tADD\tr14,r14,# _CMUNGE_pdriver_ret - _CMUNGE_pdriver_pc\n");
  output_label("_CMUNGE_pdriver_pc");
  fprintf(file, "\tCMP\tr0,#%i\n", numpdreasons);
  fprintf(file, "\tADDLT\tpc,pc,r0,LSL #2\n");
  fprintf(file, "\tB\t_CMUNGE_nopdriver_handler\n");

  /* First one must be valid */
  for (l = opt.pdriver_names; l; l = l->next)
  {
     output_import(l->handler);
     fprintf(file, "\tB\t%s\n", l->handler);
  }
  output_label("_CMUNGE_nopdriver_handler");
  fprintf(file, "\tMOV\tr0,#-1\n"); /* 'SWI not known' */
  output_label("_CMUNGE_pdriver_ret");
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tTEQ\tr0,#0\n");
  if (CODE26)
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r9,pc}^\n");
  else
  {
    fprintf(file, "\tBNE\t_CMUNGE_pdriver_10\n");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDMNEIA\tr13!,{r0-r9,pc}^\n");
    output_word(0xe12ff008, "MSR cpsr_cxsf,r8");
    fprintf(file, "\tLDMIA\tr13!,{r0-r9,pc}\n");
    output_label("_CMUNGE_pdriver_10");
  }
  fprintf(file, "\tADD\tr13,r13,#4\n");
  fprintf(file, "\tCMN\tr0,#1\n");
  if (CODE26)
  {
    fprintf(file, "\tLDMNEIA\tr13!,{r1-r9,r14}\n");
    fprintf(file, "\tORRNES\tpc,r14,#0x10000000\n");
  }
  else
  {
    fprintf(file, "\tBEQ\t_CMUNGE_pdriver_30\n");
    output_label("_CMUNGE_pdriver_20");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDMNEIA\tr13!,{r1-r9,r14}\n");
    fprintf(file, "\tORRNES\tpc,r14,#0x10000000\n");
    fprintf(file, "\tORR\tr8,r8,#0x10000000\n");
    output_word(0xe12ff008, "MSR cpsr_cxsf,r8");
    fprintf(file, "\tLDMIA\tr13!,{r1-r9,pc}\n");
    output_label("_CMUNGE_pdriver_30");
  }
  fprintf(file, "\tADR\tr0,_CMUNGE_sht_pd\n");
  fprintf(file, "\tMOV\tr1,#0\n");
  fprintf(file, "\tMOV\tr2,#0\n");
  fprintf(file, "\taddr\tr4,_CMUNGE_title\n");
  fprintf(file, "\tSWI\t0x61506\t"); output_comment("XMessageTrans_ErrorLookup\n");
  if (CODE26)
    fprintf(file, "\tLDMIA\tr13!,{r1-r9,pc}\n");
  else
    fprintf(file, "\tB\t_CMUNGE_pdriver_20\n");
  output_label("_CMUNGE_sht_pd");
  output_word(0x1e6 , "");
  output_simple_string("BadSWI");
  output_align();
}

static void swi_decoder(void)
{
  if (!opt.swi_decoder)
    return;
  output_label("_CMUNGE_swi_decoder");
  if (opt.swi_decoder->handler)
  {
    fprintf(file, "\tSTMDB\tr13!,{r0-r3,r10,r11,r14}\n");
    fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
    fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
    fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tLDR\tr14,[r12]\n");
    fprintf(file, "\tLDMIB\tr14,{r11,r14}\n");
    fprintf(file, "\tSTMIA\tr10,{r11,r14}\n");
    output_ADD_Lib_Reloc();
    fprintf(file, "\tMOV\tr11,#0\n");
    fprintf(file, "\tCMP\tr0,#0\n");
    fprintf(file, "\tBGE\t_CMUNGE_sd1\n");
    fprintf(file, "\tMOV\tr0,r1\n");
    fprintf(file, "\tMOV\tr1,r12\n");
    output_import(opt.swi_decoder->name);
    fprintf(file, "\tBL\t%s\n", opt.swi_decoder->name);
    fprintf(file, "\tSTR\tr0,[r13,#0]\n");
    fprintf(file, "\tB\t_CMUNGE_sd2\n");
    output_label("_CMUNGE_sd1");
    fprintf(file, "\tSTMDB\tr13!,{r12}\n");
    output_import(opt.swi_decoder->handler);
    fprintf(file, "\tBL\t%s\n", opt.swi_decoder->handler);
    fprintf(file, "\tADD\tr13,r13,#4\n");
    fprintf(file, "\tSTR\tr0,[r13,#8]\n");
    output_label("_CMUNGE_sd2");
    if (CODE26)
      output_SUB_Lib_Reloc();
    else
      output_SUB_Lib_Reloc_S();
    fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
    if (CODE26)
      fprintf(file, "\tLDMIA\tr13!,{r0-r3,r10,r11,pc}^\n");
    else
      fprintf(file, "\tLDMIA\tr13!,{r0-r3,r10,r11,pc}\n");
  } else {
    fprintf(file, "\tSTMDB\tr13!,{r0-r3,r10,r11,r14}\n");
    fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
    fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
    fprintf(file, "\tMOV\tr0,r13\n");
    fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tMOV\tr1,r12\n");
    fprintf(file, "\tLDR\tr12,[r12]\n");
    fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
    fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
    output_ADD_Lib_Reloc();
    fprintf(file, "\tMOV\tr11,#0\n");
    output_import(opt.swi_decoder->name);
    fprintf(file, "\tBL\t%s\n", opt.swi_decoder->name);
    if (CODE26)
      output_SUB_Lib_Reloc();
    else
      output_SUB_Lib_Reloc_S();
    fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
    if (CODE26)
      fprintf(file, "\tLDMIA\tr13!,{r0-r3,r10,r11,pc}^\n");
    else
      fprintf(file, "\tLDMIA\tr13!,{r0-r3,r10,r11,pc}\n");
  }
}

static void veneers(void)
{
  int any_error = 0;
  int n;
  int first;
  handler_list l;

  first = 1;
  /* JRF: Note: we use the number in r9 to indicate the routine that is
                being entered, as number increasing by 1 (previously we
                increased it by 4), so that we can use the top bit to
                mean that the entry point can return an error. */
  /* Events */
  for (l = opt.events, n = 0; l != NULL; l = l->next, ++n)
  {
    if (!first)
      fprintf(file, "\tB\t_CMUNGE_ve\n");
    output_export(l->name);
    output_label(l->name);
    /* Output the event list */
    if (l->extradata)
    {
      int_list il;
      int firstteq = 1;

      for (il = (int_list)l->extradata; il != NULL; il = il->next)
      {
        if (firstteq)
          fprintf(file, "\tTEQ\tr0,#%d\n", il->val);
        else
          fprintf(file, "\tTEQNE\tr0,#%d\n", il->val);
        firstteq = 0;
      }
      if (CODE26)
        fprintf(file, "\tMOVNES\tpc,r14\n");
      else
        fprintf(file, "\tMOVNE\tpc,r14\n");
    }
    fprintf(file, "\tSTMDB\tr13!,{r0-r11,r14}\n");
    fprintf(file, "\tMOV\tr9,#%d\n", n);
    first = 0;
  }

  /* Vectors */
  for (l = opt.vectors; l != NULL; l = l->next, ++n)
  {
    if (!first)
      fprintf(file, "\tB\t_CMUNGE_ve\n");
    output_export(l->name);
    output_label(l->name);
    fprintf(file, "\tSTMDB\tr13!,{r0-r11,r14}\n");
    if (l->error_capable)
    {
      fprintf(file, "\tMOV\tr9,#%d :OR: (1<<31)\n", n);
      any_error = 1;
    }
    else
      fprintf(file, "\tMOV\tr9,#%d\n", n);
    first = 0;
  }

  /* Irqs */
  for (l = opt.irqs; l != NULL; l = l->next, ++n)
  {
    if (!first)
      fprintf(file, "\tB\t_CMUNGE_ve\n");
    output_export(l->name);
    output_label(l->name);
    fprintf(file, "\tSTMDB\tr13!,{r0-r11,r14}\n");
    fprintf(file, "\tMOV\tr9,#%d\n", n);
    first = 0;
  }

  if (n == 0)
    return;

  output_label("_CMUNGE_ve");
  fprintf(file, "\tMOV\tr0,r13\n");
  if (CODE26)
  {
    fprintf(file, "\tMOV\tr6,pc\n");
    fprintf(file, "\tBIC\tr3,r6,#3\n");
    fprintf(file, "\tTEQP\tr3,#3\n");
  }
  else
  {
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tMOVNE\tr8,pc\n");
    fprintf(file, "\tORRNE\tr3,r8,#3\n");
    fprintf(file, "\tTEQNEP\tr3,#0\n");
    fprintf(file, "\tMRSEQ\tr6,cpsr\n");
    fprintf(file, "\tORREQ\tr3,r6,#3\n");
    fprintf(file, "\tMSREQ\tcpsr_ctl,r3\n");
  }
  fprintf(file, "\tMOV\tr1,r12\n");
  fprintf(file, "\tMOV\tr7,r14\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tLDR\tr12,[r12]\n");
  fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
  fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
  output_ADD_Lib_Reloc();
  fprintf(file, "\tMOV\tr11,#0\n");
  if (CODE26)
  {
    fprintf(file, "\tMOV\tr14,pc\n");
    fprintf(file, "\tADD\tr14,r14,# _CMUNGE_vret - _CMUNGE_vpc\n");
  }
  else
    fprintf(file, "\tADD\tr14,pc,# _CMUNGE_vret - _CMUNGE_vpc - 4\n");
  output_label("_CMUNGE_vpc");
  fprintf(file, "\tADD\tpc,pc,r9,LSL #2\t"); output_comment("Notice that we lose the top bits\n");
  fprintf(file, "\tMOV\tr0,r0\n");

  /* Events */
  for (l = opt.events; l != NULL; l = l->next)
  {
    if (l->handler)
    {
      output_import(l->handler);
      fprintf(file, "\tB\t%s\n", l->handler);
    }
    else
    {
      output_import_vargs("%s_handler", l->name);
      fprintf(file, "\tB\t%s_handler\n", l->name);
    }
  }
  /* Vectors */
  for (l = opt.vectors; l != NULL; l = l->next)
  {
    if (l->handler)
    {
      output_import(l->handler);
      fprintf(file, "\tB\t%s\n", l->handler);
    }
    else
    {
      output_import_vargs("%s_handler", l->name);
      fprintf(file, "\tB\t%s_handler\n", l->name);
    }
  }
  /* Irqs */
  for (l = opt.irqs; l != NULL; l = l->next)
  {
    if (l->handler)
    {
      output_import(l->handler);
      fprintf(file, "\tB\t%s\n", l->handler);
    }
    else
    {
      output_import_vargs("%s_handler", l->name);
      fprintf(file, "\tB\t%s_handler\n", l->name);
    }
  }

  output_label("_CMUNGE_vret");
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr14,r7\n");
  if (CODE26)
    fprintf(file, "\tTEQP\tr6,#0\n");
  else
  {
    fprintf(file, "\tTEQ      pc,pc\n");
    fprintf(file, "\tBEQ      _CMUNGE_vret_10\n");
    fprintf(file, "\tTEQP     r8,#0\n");
  }
  if (any_error)
  {
    /* r9 = has bit 31 set if :
            r0 = 0 (claim)
                 1 (pass on)
                 pointer to error (claim + set v)
            otherwise:
            r0 = 0 (claim)
                 other (pass on)
     */
    fprintf(file, "\tTST\tr9,#1<<31\n");
    fprintf(file, "\tBEQ\t_CMUNGE_vret_26noerror\n");

    fprintf(file, "\tCMP\tr0,#1\n");
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r11,pc}^\n");
    fprintf(file, "\tLDMLOIA\tr13!,{r0-r11,r14,pc}^\n");
    fprintf(file, "\tSTR\tr0,[sp]\n");
    fprintf(file, "\tLDMIA\tr13!,{r0-r11,r14}\n");
    fprintf(file, "\tLDR\tr14,[sp], #4\n");
    fprintf(file, "\tORRS\tpc, r14, #1<<28\t"); output_comment("Return with V set\n");
    output_label("_CMUNGE_vret_26noerror");
  }
  fprintf(file, "\tTEQ\tr0,#0\n");
  fprintf(file, "\tLDMEQIA\tr13!,{r0-r11,r14,pc}^\n");
  fprintf(file, "\tLDMNEIA\tr13!,{r0-r11,pc}^\n");

  if (CODE26)
  {}
  else
  {
    output_label("_CMUNGE_vret_10");
    if (any_error)
    {
      /* r9 = has bit 31 set if :
              r0 = 0 (claim)
                   1 (pass on)
                   pointer to error (claim + set v)
              otherwise:
              r0 = 0 (claim)
                   other (pass on)
       */
      fprintf(file, "\tTST\tr9,#1<<31\n");
      fprintf(file, "\tBICNES\tr14,r0,#1\n");
      fprintf(file, "\tORRNE\tr6,r6,#1<<28\t"); output_comment("V set on returned flags\n");
      fprintf(file, "\tSTRNE\tr0,[sp, #0]\t"); output_comment("R0 updated for return\n");
      fprintf(file, "\tMOVNE\tr0,#0\t"); output_comment("Mark as claiming\n");
    }
    fprintf(file, "\tMOV\tr12,r6\n");
    fprintf(file, "\tMSR\tcpsr_ctl,r6\n");
    fprintf(file, "\tTEQ\tr0,#0\n");
    fprintf(file, "\tLDMIA\tr13!,{r0-r11,r14}\n");
    fprintf(file, "\tLDREQ\tr14,[r13],#4\n");
    fprintf(file, "\tMSR\tcpsr_flg,r12\n");
    fprintf(file, "\tMOV\tpc,r14\n");
  }
}

/* Vector traps are a special case of veneers */
static void vector_traps(void)
{
  int n;
  int first;
  handler_list l;

  first = 1;
  /* Vector traps */
  for (l = opt.vector_traps, n = 0; l != NULL; l = l->next, n += 4)
  {
    if (!first)
      fprintf(file, "\tB\t_CMUNGE_vte\n");
    output_export(l->name);
    output_label(l->name);
    fprintf(file, "\tSTMDB\tr13!,{r0-r11,r14}\n");
    fprintf(file, "\tMOV\tr2,#%d\n", n);
    first = 0;
  }

  if (n == 0)
    return;

  output_label("_CMUNGE_vte");
  fprintf(file, "\tMOV\tr0,r13\n");
  if (CODE26)
  {
    fprintf(file, "\tMOV\tr6,pc\n");
    fprintf(file, "\tBIC\tr3,r6,#3\n");
    fprintf(file, "\tTEQP\tr3,#3\n");
  }
  else
  {
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tMOVNE\tr8,pc\n");
    fprintf(file, "\tBICNE\tr3,r8,#3\n");
    fprintf(file, "\tTEQNEP\tr3,#3\n");
    fprintf(file, "\tMRSEQ\tr6,cpsr\n");
    fprintf(file, "\tORREQ\tr3,r6,#3\n");
    fprintf(file, "\tMSREQ\tcpsr_ctl,r3\n");
  }
  fprintf(file, "\tMOV\tr1,r12\n");
  fprintf(file, "\tMOV\tr7,r14\n");
  fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
  fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
  fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tLDR\tr12,[r12]\n");
  fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
  fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
  output_ADD_Lib_Reloc();
  fprintf(file, "\tMOV\tr11,#0\n");
  fprintf(file, "\tMOV\tr12,r2\n");
  fprintf(file, "\tADR\tr2,_CMUNGE_vtcall\n");
  fprintf(file, "\tMOV\tr3,r0\n");
  fprintf(file, "\tMOV\tr14,pc\n");
  fprintf(file, "\tADD\tr14,r14,# _CMUNGE_vtret - _CMUNGE_vtpc\n");
  output_label("_CMUNGE_vtpc");
  fprintf(file, "\tADD\tpc,pc,r12\n");
  fprintf(file, "\tMOV\tr0,r0\n");

  /* Vector traps */
  for (l = opt.vector_traps; l != NULL; l = l->next)
  {
    if (l->handler)
    {
      output_import(l->handler);
      fprintf(file, "\tB\t%s\n", l->handler);
    }
    else
    {
      output_import_vargs("%s_handler", l->name);
      fprintf(file, "\tB\t%s_handler\n", l->name);
    }
  }
  output_label("_CMUNGE_vtret");
  output_SUB_Lib_Reloc();
  fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
  fprintf(file, "\tMOV\tr14,r7\n");
  /* Return to the caller's mode */
  if (CODE26)
    fprintf(file, "\tTEQP\tr6,#0\n");
  else
  {
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tMSREQ\tcpsr_ctl,r6\n");
    fprintf(file, "\tTEQNEP\tr8,#0\n");
  }
  /* We need to return if 1,
                claim if 0,
                claim with VS, r0 passed on if other values
   */
  if (CODE26)
  {
    fprintf(file, "\tTEQ\tr0,#1\n");
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r11,pc}^\n");
    fprintf(file, "\tTEQ\tr0,#0\n");
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r11,r14,pc}^\n");
    fprintf(file, "\tADD\tr13,r13,#4\n");
    fprintf(file, "\tLDMIA\tr13!,{r1-r11,r14}\n");
    fprintf(file, "\tLDR\tr14,[r13],#4\n");
    fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
  }
  else
  {
    /* JRF: Checked 20 Feb 2006, and should now be correct */
    fprintf(file, "\tCMP\tr0, #1\n");
    fprintf(file, "\tBHI\t_CMUNGE_vtretv\n");
    fprintf(file, "\tMOV\tr12, r6\n");
    fprintf(file, "\tLDMEQIA\tr13!,{r0-r11}\t"); output_comment("r0=1 case (pass on)\n");
    fprintf(file, "\tLDMLOIA\tr13!,{r0-r11,r14}\t"); output_comment("r0=0 case (claim)\n");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDR\tr14, [sp], #4\n");
    fprintf(file, "\tMOVNES\tpc,r14\n");
    fprintf(file, "\tMSR\tcpsr_flg,r12\n");
    fprintf(file, "\tMOV\tpc, r14\n");

    output_comment("r0 = anything else (claim+VS+R0 returned)\n");
    output_label("_CMUNGE_vtretv");
    fprintf(file, "\tADD\tr13, r13, #4\n");
    fprintf(file, "\tLDMIA\tr13!,{r1-r11,r14}\n");
    fprintf(file, "\tTEQ\tpc,pc\n");
    fprintf(file, "\tLDR\tr14, [sp], #4\n");
    fprintf(file, "\tORREQ\tr12, r6, #0x10000000\n");
    fprintf(file, "\tORRNE\tr14, r14, #0x10000000\n");
    fprintf(file, "\tMOVNES\tpc,r14\n");
    fprintf(file, "\tMSR\tcpsr_flg,r12\n");
    fprintf(file, "\tMOV\tpc, r14\n");
  }

  /* Now the call routine...
     This routine is called with :
       r0-> register block to pass to routine
       r1-> *original* register block
     What we must do is...
  */

  output_label("_CMUNGE_vtcall");

  fprintf(file, "\tSTMFD\tr13!,{r0,r4-r11,r14}\n");
  fprintf(file, "\tADD\tr12,r1,#10*4\n");
  fprintf(file, "\tLDMIA\tr0,{r0-r9}\n");
  fprintf(file, "\tSTMFD\tr13!,{pc}\n");
  fprintf(file, "\tLDMFD\tr12,{r10,r11,pc}\n");
  fprintf(file, "\tNOP\n");
  fprintf(file, "\tLDR\tr12,[r13],#4\n");
  fprintf(file, "\tSTMIA\tr12,{r0-r9}\n");
  fprintf(file, "\tMOVVC\tr0,#0\n");
  if (CODE26)
    fprintf(file, "\tLDMFD\tr13!,{r4-r11,pc}^\n");
  else
  {
    /* We're returning to APCS-32 so no need to preserve flags -
       note that we're inside our client, so there is no
       need to check for 26bitness. To do so would violate APCS-32.
     */
    fprintf(file, "\tLDMFD\tr13!,{r4-r11,pc}\n");
  }
}

static void errors(void)
{
  int error = opt.error_base;
  error_list l;

  if (opt.errors==NULL)
    return;

  fputc('\n', file);
  output_comment_vargs("Error messages, based at &%08x\n", error);
  for (l = opt.errors; l != NULL; l = l->next)
  {
    output_export_vargs("__err_%s", l->name);
    output_label_vargs("__err_%s", l->name);
    output_word((l->number) ? l->number : error++, "An error number");
    output_simple_string(l->message);
    output_align();
  }
}

static void generics(void)
{
  int n;
  int first;
  handler_list l;

  /* So that we can preserve r12 when we use the generic veneers, we remember
     whether we have had any registers specified */
  int preserve_endreg=11;

  /* 0 if we're not doing the carry_capable handlers,
     1 if we're doing the carry_capable handlers */
  int carry_now;

  /* Check if we use any specific registers */
  for (l = opt.generics; l != NULL; l = l->next)
  {
    if (l->private_word != -1)
      preserve_endreg=12;
  }

  for (carry_now = 0; carry_now < 2; carry_now++)
  {
    first = 1;

    /* Generic */
    n = 0;
    for (l = opt.generics; l != NULL; l = l->next)
    {
      if (carry_now != l->carry_capable)
        continue;

      if (!first)
        fprintf(file, "\tB\t_CMUNGE_ge%s\n",carry_now ? "_carry" : "");
      output_export(l->name);
      output_label(l->name);
      fprintf(file, "\tSTMDB\tr13!,{r0-r%i,r14}\n", preserve_endreg);

      /* If the private word appears elsewhere, use that */
      if (l->private_word != -1 && l->private_word != 12)
        fprintf(file, "\tMOV\tr12,r%i\n", l->private_word);

      fprintf(file, "\tMOV\tr2,#%d\n", n);
      n += 4;
      first = 0;
    }

    if (n == 0)
      continue;

    output_label_vargs("_CMUNGE_ge%s", carry_now ? "_carry" : "");
    fprintf(file, "\tMOV\tr0,r13\n");
    if (CODE26)
    {
      fprintf(file, "\tMOV\tr6,pc\n");
      fprintf(file, "\tBIC\tr3,r6,#3\n");
      fprintf(file, "\tTEQP\tr3,#3\n");
    }
    else
    {
      fprintf(file, "\tTEQ\tpc,pc\n");
      fprintf(file, "\tMOVNE\tr8,pc\n");
      fprintf(file, "\tORRNE\tr3,r8,#3\n");
      fprintf(file, "\tTEQNEP\tr3,#0\n");
      fprintf(file, "\tMRSEQ\tr6,cpsr\n");
      fprintf(file, "\tORREQ\tr3,r6,#3\n");
      fprintf(file, "\tMSREQ\tcpsr_ctl,r3\n");
    }
    fprintf(file, "\tMOV\tr1,r12\n");
    fprintf(file, "\tMOV\tr7,r14\n");
    fprintf(file, "\tMOV\tr10,r13,LSR #20\n");
    fprintf(file, "\tMOV\tr10,r10,LSL #20\n");
    fprintf(file, "\tLDMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tLDR\tr12,[r12]\n");
    fprintf(file, "\tLDMIB\tr12,{r11,r12}\n");
    fprintf(file, "\tSTMIA\tr10,{r11,r12}\n");
    output_ADD_Lib_Reloc();
    fprintf(file, "\tMOV\tr11,#0\n");
    if (CODE26)
    {
      fprintf(file, "\tMOV\tr14,pc\n");
      fprintf(file, "\tADD\tr14,r14,#_CMUNGE_gret%s - _CMUNGE_gpc%s\n",
                    carry_now ? "_carry" : "",
                    carry_now ? "_carry" : "");
    }
    else
    {
      fprintf(file, "\tADD\tr14,pc,#_CMUNGE_gret%s - _CMUNGE_gpc%s - 4\n",
                    carry_now ? "_carry" : "",
                    carry_now ? "_carry" : "");
    }
    output_label_vargs("_CMUNGE_gpc%s", carry_now ? "_carry" : "");
    fprintf(file, "\tADD\tpc,pc,r2\n");
    fprintf(file, "\tMOV\tr0,r0\n");
    for (l = opt.generics; l != NULL; l = l->next)
    {
      if (carry_now != l->carry_capable)
        continue;

      if (l->handler)
      {
        output_import(l->handler);
        fprintf(file, "\tB\t%s\n", l->handler);
      }
      else
      {
        output_import_vargs("%s_handler", l->name);
        fprintf(file, "\tB\t%s_handler\n", l->name);
      }
    }
    output_label_vargs("_CMUNGE_gret%s", carry_now ? "_carry" : "");
    output_SUB_Lib_Reloc();
    fprintf(file, "\tSTMIA\tr10,{r4,r5}\n");
    fprintf(file, "\tMOV\tr14,r7\n");
    if (CODE26)
    {
      fprintf(file, "\tTEQP\tr6,#0\n");
      fprintf(file, "\tTEQ\tr0,#0\n");
      fprintf(file, "\tLDMEQIA\tr13!,{r0-r%i,pc}^\n",preserve_endreg);
      if (carry_now)
      {
        fprintf(file, "\tTEQ\tr0,#2\n");
        fprintf(file, "\tLDMEQIA\tr13!,{r0-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORREQS\tpc,r14,#0x20000000\n");
        fprintf(file, "\tADD\tr13,r13,#4\n");
        fprintf(file, "\tLDMIA\tr13!,{r1-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
      }
      else
      {
        fprintf(file, "\tADD\tr13,r13,#4\n");
        fprintf(file, "\tLDMIA\tr13!,{r1-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
      }
    }
    else
    {
      fprintf(file, "\tTEQ\tpc,pc\n");
      fprintf(file, "\tBEQ\t_CMUNGE_genv%s\t", carry_now ? "_carry" : ""); output_comment("32bit PC\n");
      fprintf(file, "\tTEQP\tr8,#0\n");
      fprintf(file, "\tTEQ\tr0,#0\n");
      fprintf(file, "\tLDMEQIA\tr13!,{r0-r11,pc}^\n");
      if (carry_now)
      {
        /* If r0 = 2, return CS
              r0 = 0, has already been dealt with
              r0 = anything else, return VS + R0 value
              r8 = 26bit PSR on entry
         */
        fprintf(file, "\tTEQ\tr0,#2\n");
        fprintf(file, "\tLDMEQIA\tr13!,{r0-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORREQS\tpc,r14,#0x20000000\n");
        fprintf(file, "\tADD\tr13,r13,#4\n");
        fprintf(file, "\tLDMIA\tr13!,{r1-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
        output_label("_CMUNGE_genv_carry");
        /* 32bit return for the same case as the above 26bit case, except:
              r0 = 0 has not been dealt with
              r6 = 32bit PSR on entry
         */
        fprintf(file, "\tTEQ\tr0,#2\n");
        fprintf(file, "\tORREQ\tr6,r6,#0x20000000\n");
        fprintf(file, "\tBICS\tr14,r0,#2\t"); output_comment("NE if r0 <> 0 and r0 <> 2\n");
        fprintf(file, "\tSTRNE\tr0,[r13,#0]\n");
        fprintf(file, "\tORRNE\tr6,r6,#0x10000000\n");
        fprintf(file, "\tMSR\tcpsr_flg,r6\n");
        fprintf(file, "\tLDMIA\tr13!,{r0-r11,pc}\n");
      }
      else
      {
        fprintf(file, "\tADD\tr13,r13,#4\n");
        fprintf(file, "\tLDMIA\tr13!,{r1-r%i,r14}\n",preserve_endreg);
        fprintf(file, "\tORRS\tpc,r14,#0x10000000\n");
        output_label("_CMUNGE_genv");
        /* 32bit return for the same case as the above 26bit case, except:
              r6 = 32bit PSR on entry
         */
        fprintf(file, "\tTEQ\tr0,#0\n");
        fprintf(file, "\tSTRNE\tr0,[r13,#0]\n");
        fprintf(file, "\tORRNE\tr6,r6,#0x10000000\n");
        fprintf(file, "\tMSR\tcpsr_flg,r6\n");
        fprintf(file, "\tLDMIA\tr13!,{r0-r11,pc}\n");
      }
    }
  }
}

static void trailer(void)
{
  /* We need to have some space allocated for the stack for an entered RMA
     application. This code needs re-working such that other parts of the
     system can place blocks in this section without code re-writes. This
     will basically mean that they can have a list of blocks to allocate
     and their sizes which can just be traversed here. That's the idea.
     How soon I implement it... gawd only knows... depends on when I next
     have the opportunity to do something fun with CMunge that doesn't mean
     large changes to the vast majority of RISC OS. Oh yes. I like RISC OS.
     I do. Really. Oh yes. */
  if (opt.runnable == run_rmaapp_simple)
  { /* Icky */
    fputs("\n\n", file);
    if (opt.toolchain == tc_gcc)
      fputs("\t.data\n", file); /* FIXME: perhaps too simple */
    else
      fprintf(file, "\tAREA\t|!Header$$Data|, DATA\n");
    output_label("_CMUNGE_stack");
    output_word(0, "No stack at present");
  }
  fprintf(file, "\n\n");
  output_endfile();
}

/* This function generates the module flags for 32bit modules */
static void flags(void)
{
  if (CODE26)
  {}
  else
  {
    output_label("_CMUNGE_module_flags");
    output_word(1, "32bit supported");
  }
}

void WriteFile(void)
{
  file = file_write(opt.sfile, remove_onfail);
  if (file == NULL)
    ErrorFatal("Couldn't open output file: %s", opt.sfile);

  asm_header();
  mod_header();
  strings(); /* Afterwards, PC can end on a non word boundary */
  swi_table(); /* Afterwards, PC could end on a non word boundary */
  output_align();
  /* From here onwards, PC is always word aligned */
  commands();
#ifdef CMHG_RESOURCE_FILE_ORDERING
  if (opt.mfile)
  {
    output_label("_CMUNGE_message_file");
    outstring(opt.mfile,1,'\n',0);
    output_align();
  }
#endif
  flags();
  service();
  start();
  init();
  final();
  swi_handler();
  swi_decoder();
  pdriver_handler();
  veneers();
  vector_traps();
  generics();
  errors();

  trailer();

  file_close(file);
}
