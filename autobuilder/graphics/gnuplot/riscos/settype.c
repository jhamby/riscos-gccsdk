/* > RISCOS_set_tyoe
 *
 * Set filetype after gpoutfile is closed.
 *
 * (c) B.A. Bramd, 2001
 * v1.01 20040502
 */

#include "plot.h"
#ifndef __GNUC__
/* Norcroft only? */
#include <sys/os.h>
#else
/*#define __UNIXLIB_INTERNALS
#include <oslib/os.h>
#undef __UNIXLIB_INTERNALS
*/
#ifndef os_file
#define os_file(a,b,c) __os_file(a,b,c)
#endif
#ifndef os_swi
#define os_swi(a,b) __os_swi(a,b)
#endif
#endif
#include <string.h>
#include <unixlib/local.h> /* for __riscosify() */

extern char * outstr; /* from term.c, not in plot.h */

static void dosettype(int type);

/* the types (from stronghelp's unofficial list of filetypes) */
#define FT_DATA     0xffd
#define FT_DRAWFILE 0xaff
#define FT_DXF      0xdea
#define FT_GIF      0x695
#define FT_HPGL     0xcae
#define FT_LASERJET 0xff3 /* HP deskjet, HP laserjet */
#define FT_LATEX    0xaca
#define FT_METAFILE 0x405 /* Note: CGM is also typed 0x666 in some app's. */
#define FT_METAFONT 0xca0
#define FT_PNG      0xb60
#define FT_POSCRIPT 0xff5
#define FT_PRINTOUT 0xff4
#define FT_SPRITE   0xff9
#define FT_TEX      0xce5
#define FT_TEXT     0xfff

void RISCOS_set_type(void) {
  if (!outstr || !*outstr) return; /* no filename */

  /* Look at all possible terminals and set filetype (if known). */

  if (!stricmp(term->name, "table"))
    dosettype(FT_TEXT); /* ASCII table */
  else if (!stricmp(term->name, "gpic"))
    dosettype(FT_TEXT); /* groff input, readable text */
  else if (!stricmp(term->name, "regis"))
    dosettype(FT_TEXT); /* REGIS graphics language, readable text */
  else if (!stricmp(term->name, "cgm"))
    dosettype(FT_METAFILE); /* CGM or MetaFile */
  else if (!stricmp(term->name, "corel"))
    dosettype(FT_POSCRIPT); /* EPS variation */
  else if (!stricmp(term->name, "dumb"))
    dosettype(FT_TEXT); /* printer dump in readable text */
  else if (!stricmp(term->name, "dxf"))
    dosettype(FT_DXF); /* AutoDesk Data eXchange Format */
  else if (!stricmp(term->name, "fig"))
    dosettype(FT_TEXT); /* fig (x11) graphics language, readable text */
  else if (!stricmp(term->name, "gif"))
    dosettype(FT_GIF); /* (CompuServe) Graphics Interchange Format */
  else if (!stricmp(term->name, "hp2323a") ||
           !stricmp(term->name, "hp2648"))
    dosettype(FT_DATA); /* I'm quite sure there is a filetype for HP
                           plotters, it's just not in the list I'm
                           currently using. */
  else if (!stricmp(term->name, "hp500c") ||
           !stricmp(term->name, "pcl5")   ||
           !stricmp(term->name, "hpljii") ||
           !stricmp(term->name, "hpdj")   ||
           !stricmp(term->name, "hppj")) /* paintjet is a bit of a gamble */
    dosettype(FT_LASERJET);
  else if (!stricmp(term->name, "hpgl"))
    dosettype(FT_HPGL);
  else if (!stricmp(term->name, "png"))
    dosettype(FT_PNG);
  else if (!stricmp(term->name, "postscript"))
    dosettype(FT_POSCRIPT);
  else if (!stricmp(term->name, "tgif"))
    dosettype(FT_TEXT); /* readable text */
  else if (!stricmp(term->name, "epson_180dpi") ||
           !stricmp(term->name, "epson_30dpi")  ||
           !stricmp(term->name, "epson_lx800")  ||
           !stricmp(term->name, "nec_cp6")      ||
           !stricmp(term->name, "okidata")      ||
           !stricmp(term->name, "starc"))
    dosettype(FT_PRINTOUT); /* various dot-matrix printers */
  else if (!stricmp(term->name, "latex")    ||
           !stricmp(term->name, "emtex")    ||
           !stricmp(term->name, "pslatex")  ||
           !stricmp(term->name, "eepic")    ||
           !stricmp(term->name, "tpic")     ||
           !stricmp(term->name, "pstricks") ||
           !stricmp(term->name, "texdraw"))
    dosettype(FT_LATEX); /* LaTeX format */
  else if (!stricmp(term->name, "pstex"))
    dosettype(FT_TEX); /* plain TeX */
  else if (!stricmp(term->name, "mf"))
    dosettype(FT_METAFONT);
/*  else if (!stricmp(term->name, "mp"))
    dosettype(FT_???) */
  else if (!stricmp(term->name, "rodraw"))
    dosettype(FT_DRAWFILE);
  else if (!stricmp(term->name, "rosprite"))
    dosettype(FT_SPRITE);
  else
    /* All other types are either pure data or do not (yet) have
       a RISC OS filetype assigned. */
    dosettype(FT_DATA);
}

static void dosettype(int type) {
  int regs[6];
  int dummy = type;
  char ro_name [PATH_MAX];

  /* When we get here, the file is already created and unixlib's
     fopen() riscossifies the name.  If __riscosify() fails something
     really strange is happening. */
  if (!__riscosify_std(outstr, 0, ro_name, sizeof(ro_name), &dummy)) {
    fprintf(stderr, "cannot riscosify %s\n", outstr);
    return;
  }
#ifdef RISCOS_BB_DEBUG
  else
    fprintf(stderr, "setting type of %s to %03x\n", ro_name, type);
#endif /* RISCOS_BB_DEBUG */

  memset(regs,0,sizeof(regs));
  regs[2] = type;
  /* No error checking, if it failed the type will be text which
     is more or less OK. */
  /*os_file(18, ro_name, regs);*/
  osfile_set_type (ro_name, regs[2]) ;
}
