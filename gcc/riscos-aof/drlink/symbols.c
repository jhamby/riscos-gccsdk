/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This module contains the functions concerned with manipulating the
** various symbol tables
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "drlhdr.h"
#include "chunkhdr.h"
#include "symbolhdr.h"
#include "edithdr.h"
#include "procdefs.h"

/* Variables referenced from other files */

symbol *globalsyms[MAXGLOBALS];	/* Global symbol table */

symbol
  *image_robase,		/* Symbol table entries of pre-defined symbols */
  *image_rwbase,
  *image_zibase,
  *image_rolimit,
  *image_rwlimit,
  *image_zilimit,
  *image_codebase,		/* Old symbols used by Fortran 77 */
  *image_codelimit,
  *image_database,
  *image_datalimit,
  *reloc_code;

libtable *current_libsyms; 	/* Pointer to current library symbol table */
libheader *current_lib;		/* Pointer to libheader entry for library being searched */

symtentry
  *current_symtbase,		/* Start of OBJ_SYMT chunk in current file containing references */
  *current_symtend;		/* End of OBJ_SYMT chunk in current file */

unsigned int
  totalsymbols,			/* Total number of symbols defined in program */
  numwanted,			/* Number of symbols currently wanted */
  numfound,			/* Number of symbols found this library pass */
  lldsize;			/* Space required for names in low-level debug table */

/* Private declarations */

typedef struct missing {
  const char *symname, *filename;	/* Symbol not found and the file that referenced it */
  struct missing *left;			/* Left in binary tree                              */
  struct missing *right;		/* Right in binary tree                             */
} missing;

static symbol *commonsyms[MAXCOMMON];	/* Common Block symbol table */

static symbol
  *symblock,			/* Memory to hold symbol entries for file when reading file */
  *wantedlist;			/* 'Wanted symbols' list in current file */

static filelist *current_file;	/* File list entry for file being processed */
static symtable *current_table;	/* Pointer to local symbol table of current file */
static editcmd
  *current_symedit,		/* Pointer to symbol edits for current file */
  *current_refedit;		/* Pointer to reference edits for current file */

/*
** 'stricmp' performs a comparison of two null-terminated character
** strings ignoring the case of the letters
*/
int stricmp(const char *str1, const char *str2) {
  while (*str1 && tolower(*str1)==tolower(*str2)) {
    str1++;
    str2++;
  }
  return *str1-*str2;
}

/*
** 'hash' returns a hash value for the character string passed to it.
** The character is OR'ed with 0x20 to do a quick and dirty
** conversion to lower case because we need to ignore the case of
** any letters. I suppose it would be better to use 'tolower' but
** it's only a hashing function.
*/
int hash(const char *name) {
  int total = 0;

  while (*name != 0) {
    total = total * 5 ^ (*name | 0x20);
    name++;
  }
  return total;
}

/*
** 'isrelocatable' is called to determine if a relocation refers to
** something that itself is relocatable. References to areas are
** relocatable but references to symbols can be absolute.
*/
bool isrelocatable(relocation *rp) {
  unsigned int symtindex, reltype;
  symtentry *sp;
  bool istype2;
  reltype = rp->reltypesym;
  istype2 = (reltype & REL_TYPE2)!=0;
  if (istype2) {
    reltype = get_type2_type(reltype);
  }
  else {
    reltype = get_type1_type(reltype);
  }
  if ((reltype & REL_SYM)==0) {	/* Simple area reference */
    return TRUE;
  }
  if (istype2) {
    symtindex = get_type2_index(rp->reltypesym);
  }
  else {
    symtindex = get_type1_index(rp->reltypesym);
  }
  sp = symtbase+symtindex;
  if ((sp->symtattr & SYM_WEAKREF)!=0) {	/* Reference to a weak external */
    return FALSE;
  }
  else {
    if ((sp->symtattr & SYM_DEFN)==0) {		/* Reference to external symbol */
      sp = sp->symtarea.symdefptr;
    }
  }
  return (sp->symtattr & SYM_ABSVAL)==0;
}

/*
** 'check_edits' is called to see if there are any edits for the
** current file and to set up pointers to the edits if there are
*/
static void check_edits(void) {
  editcmd *ep;
  int hashval;
  current_symedit = NIL;
  current_refedit = NIL;
  hashval = current_file->chfilehash;
  if (symedit_count>0) {	/* Symbol edits exist */
    ep = symedit_list;
    while (ep!=NIL && hashval>ep->edtfnhash) ep = ep->edtnext;
    if (ep!=NIL && hashval==ep->edtfnhash && stricmp(current_file->chfilename, ep->edtfile)==0) {
      current_symedit = ep;
    }
  }
  if (refedit_count>0) {	/* Reference edits exist */
    ep = refedit_list;
    while (ep!=NIL && hashval>ep->edtfnhash) ep = ep->edtnext;
    if (ep!=NIL && hashval==ep->edtfnhash && stricmp(current_file->chfilename, ep->edtfile)==0) {
      current_refedit = ep;
    }
  }
}

/*
** 'check_symedit' is called to check if the symbol passed
** to it has to be edited and if so to carry out that edit.
** Note that multiple edits on one symbol are allowed, so
** that, for example, a symbol can be renamed and made local
** or global at the same time.
*/
static void check_symedit(symtentry *symtp) {
  int fnhashval, syhashval;
  editcmd *ep;
  const char *symname;
  symname = symtp->symtname;
  fnhashval = current_file->chfilehash;
  syhashval = hash(symname);
  ep = current_symedit;

  while (ep!=NIL && ep->edtfnhash==fnhashval) {
    if (ep->edtsyhash==syhashval &&
     ((!opt_case && strcmp(ep->edtold, symname)==0)
      || (opt_case && stricmp(ep->edtold, symname)==0))) {	/* Found symbol */
      symedit_count-=1;
      ep->edtdone = TRUE;
      switch (ep->edtoper) {
      case EDT_RENAME:
        current_file->edited = TRUE;
        symtp->symtname = ep->edtnew;
        if (opt_verbose) {
          error("Warning: Symbol '%s' in '%s' renamed as '%s'", ep->edtold, ep->edtfile, ep->edtnew);
        }
        break;
      case EDT_HIDE:
        if ((symtp->symtattr & SYM_SCOPE)==SYM_GLOBAL) {	/* Global symbol */
          symtp->symtattr = symtp->symtattr-(SYM_GLOBAL-SYM_LOCAL);
          if (opt_verbose) {
            error("Warning: Symbol '%s' in '%s' now marked as 'local'", ep->edtold, ep->edtfile);
          }
        }
        else {
          error("Warning: Symbol '%s' in '%s' is already a 'local' symbol", symtp->symtname, ep->edtfile);
        }
        break;
      case EDT_REVEAL:
        if ((symtp->symtattr & SYM_SCOPE)==SYM_LOCAL) {		/* Local symbol */
          symtp->symtattr = symtp->symtattr+(SYM_GLOBAL-SYM_LOCAL);
          if (opt_verbose) {
            error("Warning: Symbol '%s' in '%s' now marked as 'global'", ep->edtold, ep->edtfile);
          }
        }
        else {
          error("Warning: Symbol '%s' in '%s' is already a 'global' symbol", symtp->symtname, ep->edtfile);
        }
      default:
        break;  
      }
    }
    ep = ep->edtnext;
  }
}

/*
** 'check_refedit' is called to check if the symbol reference
** passed to it has to be changed and if so to change it
*/
static void check_refedit(symtentry *symtp) {
  int syhashval, fnhashval;
  editcmd *ep;
  const char *symname;
  symname = symtp->symtname;
  fnhashval = current_file->chfilehash;
  syhashval = hash(symname);
  ep = current_refedit;
  while (ep!=NIL && ep->edtfnhash==fnhashval) {
    if (ep->edtsyhash==syhashval &&
     ((!opt_case && strcmp(ep->edtold, symname)==0)
     || (opt_case && stricmp(ep->edtold, symname)==0))) {	/* Found symbol */
      refedit_count-=1;
      ep->edtdone = TRUE;
      current_file->edited = TRUE;
      symtp->symtname = ep->edtnew;
      if (opt_verbose) {
        error("Warning: Symbol reference '%s' in '%s' changed to '%s'", ep->edtold, ep->edtfile, ep->edtnew);
      }
    }
    ep = ep->edtnext;
  }
}


static symbol **insert_symbol(symbol **tree, const char *name) {

  while (*tree != NIL) {
    int compval = opt_case ? stricmp(name, (*tree)->symtptr->symtname) : strcmp(name, (*tree)->symtptr->symtname);

    if (compval > 0) {
      tree = &(*tree)->right;
    } else {
      tree = &(*tree)->left;
    }
  }

  return tree;
}


/*
** 'add_symbol' adds a symbol to one of the symbol tables. It returns
** 'TRUE' if the new entry was created successfully or FALSE if the
** call failed. It checks for duplicate entries and flags any it finds.
** It is possible for there to be two occurences of a symbol, one
** ordinary one and one with the 'strong' attribute set. 'symtptr'
** points at the 'strong' definition if one is found (as this will
** be the one most widely used). 'symnormal' points at the non-strong
** definition when there are two versions, else it is nil. At present,
** however, the case where a strong symbol that duplicates a non-strong
** one is detected whilst scanning libraries cannot be handled. The
** strong version will be used in preference to the non-strong one when
** future external references are dealt with, almost certainly leading
** to errors in the link. (To fix this properly would require the linker
** to scan the OBJ_SYMT chunk of every entry in every library for
** every symbol *just in case* there is a strong version. This would slow
** things down horribly.) Another case to check for is duplicated symbols
** in code and data common areas. In this case, the second and subsequent
** symbol definitions have to be ignored. This can be dealt with as the
** area containing each symbol will be the same (the OBJ_HEAD
** processing takes care of this) so it is just a matter of checking
** the 'area' pointers and if they are the same quietly ignoring the
** symbol.
**
** If there are duplicate entries then, for a local' table entry, this
** means there is something wrong with the OBJ_SYMT chunk but for a
** 'global' symbol then it is just a simple case (of the same symbol
** appearing in more than one file.
**
** There is a kludge in here to handle something the C compiler does
** in its AOF files. It generates local symbols that are not associated
** with any area. If one of these is found, the symbol is converted
** to an absolute value. This would be okay, except that, in certain
** cases, the C compiler also generates relocations for the symbol as
** well, which means the relocation code has to be kludged to handle
** them too.
*/
static bool add_symbol(symtentry *symtp) {
  symbol *p;
  symbol **table;
  int hashval;
  unsigned int attr;
  const char *name;
  arealist *ap;

  symtp->symtname = symtp->symtname+COERCE(strtbase, unsigned int);

  if (current_symedit!=NIL) check_symedit(symtp);
  name = symtp->symtname;
  hashval = hash(name);
  attr = symtp->symtattr & SYM_ATMASK;

  if ((attr & SYM_ABSVAL)==0) {	/* Symbol is not an absolute value. Find its area */
    ap = find_area(strtbase+symtp->symtarea.areaname);
    if (ap==NIL) {	/* Dodgy C symbol reference to unknown area */
      symtp->symtattr = symtp->symtattr | SYM_ABSVAL;	/* Convert symbol to absolute */
    }
  }
  else {
    ap = NIL;
  }

  symtp->symtarea.areaptr = ap;

  if ((attr & SYM_SCOPE)==SYM_LOCAL) {
    table = &(*current_table)[hashval & LOCALMASK];
  }
  else {
    table = &globalsyms[hashval & GLOBALMASK];
  }

  if (current_file->keepdebug) lldsize += strlen(name) + sizeof(char);

  table = insert_symbol(table, name);
  p = *table;

  if (p == NIL) {  /* Entry not found */
    p = symblock;
    symblock++;
    p->symhash = hashval;
    p->symtptr = symtp;
    p->symnormal = NIL;
    p->left = p->right = NIL;
    *table = p;
  }
  else if ((p->symtptr->symtattr & SYM_STRONG)==(attr & SYM_STRONG)) {	/* Duplicate symbol */
    arealist *cp = p->symtptr->symtarea.areaptr;

    if ((ap->aratattr & (ATT_COMDEF|ATT_COMMON))!=0	/* Symbol's area is common block */
     && (cp->aratattr & (ATT_COMDEF|ATT_COMMON))!=0	/* Dup symbol's area is common block too */
     && strcmp(ap->arname, cp->arname)==0		/* Common block names are the same */
     && symtp->symtvalue==p->symtptr->symtvalue) {		/* Symbol values are the same */
      return TRUE;					/* No problem */
    }

    if (link_state==LIB_SEARCH)
      error("Error: '%s' in '%s(%s)' duplicates a symbol already read in '%s'",
       name, current_lib->libname, objectname, cp->arfileptr->chfilename);
    else {
      error("Error: '%s' in '%s' duplicates a symbol already read in '%s'", 
       name, objectname, cp->arfileptr->chfilename);
    }
    return FALSE;

  }
  else {	/* New symbol of different 'strength' to existing definition */
    if ((attr & SYM_STRONG)!=0) {	/* New symbol is strong; old one is non-strong */
      if (link_state==LIB_SEARCH) {	/* Avoid possible use of wrong symbol... */
        error("Error: 'Strong' definition of '%s' found in '%s' after non-strong definition has been used",
         name, objectname);
        return FALSE;
      }
      p->symnormal = p->symtptr;
      p->symtptr = symtp;
    }
    else {	/* Old symbol is strong; new one is non-strong */
      p->symnormal = symtp;
    }
  }

  return TRUE;
}

/*
** 'add_externref' adds an externally defined symbol to a file's
** 'wanted symbols' list.
** Memory for the symbol is taken from the block allocated for
** symbols in the file being processed.
*/
static void add_externref(symtentry *symtp) {
  symbol *sp, **insert;
  const char *name;

  symtp->symtname = symtp->symtname+COERCE(strtbase, unsigned int);

  if (current_refedit!=NIL) check_refedit(symtp);
  name = symtp->symtname;
  sp = symblock;
  symblock++;
  sp->symhash = hash(name);
  sp->symtptr = symtp;
  sp->symnormal = NIL;
  sp->left = sp->right = NIL;

  insert = insert_symbol(&wantedlist, name);
  if (!*insert) *insert = sp;

  numwanted++;  /* Bump up count of wanted symbols */
}

/*
** 'create_externref' creates dummy OBJ_SYMT and symbol entries for
** the symbol passed to it. It is used when dealing with strong
** symbols.
**
** External references created by this routine are dealt with in the
** same way as normal references. They are added to the current AOF
** AOF file's 'wanted' list. 'resolve' will come across these in the
** normal way, but will find that the symbol definition lies within
** the same module as the reference (although the OBJ_SYMT external
** reference is not: it only checks the whereabouts of the
** definition. If there is a non-strong definition, then there is no
** problem. If none, the library search code has to deal with the
** reference.
*/
symbol *create_externref(symtentry *stp) {
  symbol *sp, **insert;
  symtentry *newstp;
  const char *name;

  sp = allocmem(sizeof(symbol));
  newstp = allocmem(sizeof(symtentry));
  if (sp==NIL || newstp==NIL) error("Fatal: Out of memory in 'create_externref'");
  name = stp->symtname;
  newstp->symtname = name;
  newstp->symtattr = SYM_EXTERN;
  newstp->symtvalue = 0;
  newstp->symtarea.symdefptr = NIL;
  sp->symhash = hash(name);
  sp->symtptr = newstp;
  sp->symnormal = NIL;
  sp->left = sp->right = NIL;

  insert = insert_symbol(&wantedlist, name);
  if (!*insert) *insert = sp;

  numwanted++;

  return sp;
}

/*
** 'add_commonref' is called when a symbol has the 'common reference' bit
** set to check if a common block whose name matches that of the symbol
** exists and if not to create it. The reference to the common block
** is resolved at this time as well, so, although the 'external ref'
** bit is set, the symbol is not added to the 'wanted symbols' list.
** One slightly iffy things the code does is to set the size of the
** common block to zero after creating the common block to stop the
** resolution code using the size of the common block as an offset (the
** words 'bug fix' and 'hack' come to mind here).
*/
static void add_commonref(symtentry *stp) {
  const char *name;
  symbol *sp;

  stp->symtname = stp->symtname+COERCE(strtbase, unsigned int);
  if (current_refedit!=NIL) check_refedit(stp);

  name = stp->symtname;
  sp = symblock;
  symblock++;
  sp->symhash             = hash(name);
  sp->symtptr             = stp;
  sp->symnormal           = NIL;
  sp->left = sp->right    = NIL;

  stp->symtarea.symdefptr = make_commonarea(sp);
  stp->symtvalue          = 0;
}

/*
** 'scan_symt' is called to add the symbols in the OBJ_SYMT chunk to
** the different symbol tables. It adds symbol definitions to the
** symbol tables and external references to a linked list of references
** for the file. Note that memory to hold all the SYMT entries is
** allocated at the start for reasons of efficiency and in a vain
** attempt to make the linker go as fast as ARM's. The proc returns
** 'TRUE' if this all worked okay otherwise it returns FALSE.
**
** There is a kludge in here to handle a problem with partially-linked
** AOF files produced by version 5 of the ARM linker. It creates
** external references with the 'absolute' attribute bit set. This is
** meaningless for a reference, so the code clears it. The code to
** handle relocations goes wrong otherwise.
*/
bool scan_symt(filelist *fp) {
  int i, symcount, attr;
  symtentry *symtp;
  bool ok, addok, gotstrong;
  unsigned int strtsize;

  symcount = fp->symtcount;
  symblock = allocmem(symcount*sizeof(symbol));
  if (symblock==NIL) error("Fatal: Out of memory in 'scan_symt' reading file '%s'", objectname);

  current_file = fp;
  current_table = &fp->localsyms;
  check_edits();	/* Are there any link edits for this file? */
  symtp = fp->objsymtptr;
  strtsize = fp->objstrtsize;
  wantedlist = NIL;
  gotstrong = FALSE;
  addok = ok = TRUE;

  for (i = 1; i <= symcount && ok; i++) {
    if (COERCE(symtp->symtname, unsigned int) >= strtsize) {
      error("Error: Offset of symbol name in OBJ_SYMT chunk is bad in '%s'", fp->chfilename);
      symtp->symtname = NIL;
      ok = FALSE;
    }

    if (strcmp(symtp->symtname+COERCE(strtbase, unsigned int), "$d")==0 ||
        strcmp(symtp->symtname+COERCE(strtbase, unsigned int), "$a")==0) {
      /* Convert symbol to absolute to prevent an attempt at relocation */
      symtp->symtattr = symtp->symtattr | SYM_ABSVAL;
      /* Ignore these symbols due to possible objasm bug? */
      symtp++;
      continue;
    }
    attr = symtp->symtattr;
    if (aofv3flag && (attr & SYM_A3ATTR)!=0) {	/* Reject AOF 3 attributes */
      error("Error: Symbol '%s' in '%s' has unsupported AOF symbol attributes (%06x)",
       symtp->symtname+COERCE(strtbase, unsigned int), fp->chfilename,
       attr & SYM_A3ATTR);
      ok = FALSE;
    }
    attr = attr & SYM_ATMASK;

    if ((attr & SYM_DEFN)!=0) {	/* Add symbol defintion to symbol table */
      addok = addok && add_symbol(symtp);
      gotstrong = gotstrong || (attr & SYM_STRONG)!=0;
      totalsymbols+=1;
    }
    else if (attr==(SYM_COMMON|SYM_EXTERN)) {	/* Reference to a common block */
      add_commonref(symtp);
    }
    else if (attr==SYM_EXTERN) {	/* Reference to an external symbol */
      add_externref(symtp);
    }
    else if (attr==(SYM_EXTERN|SYM_ABSVAL)) {	/* Dodgy ARM linker trick... */
      symtp->symtattr-=SYM_ABSVAL;		/* Lose 'absolute' attribute bit */
      add_externref(symtp);
    }
    else {
      error("Error: OBJ_SYMT attribute value (0x%x) of symbol '%s' in '%s' is bad. Is file corrupt?",
       attr, COERCE(strtbase, unsigned int)+symtp->symtname, fp->chfilename);
      ok = FALSE;
    }
    symtp++;
  }
  if (gotstrong) check_strongrefs(fp);	/* Special processing for strong symbols */
  fp->symtries.wantedsyms = wantedlist;
  return ok && addok;
}


static symbol *find_symbol(symbol *tree, const char *name, bool ignorecase) {

  while (tree != NIL) {
    int compval = ignorecase ? stricmp(name, tree->symtptr->symtname) : strcmp(name, tree->symtptr->symtname);

    if (compval == 0) {
      return tree;
    }
    if (compval > 0)
      tree = tree->right;
    else
      tree = tree->left;
  }

  return NIL;
}



/*
** 'search_global' is a custom routine for searching the global symbol table as
** quickly as possible
*/
static symbol *search_global(symbol *wantedsym) {
  symbol *p;
  const char *name;

  p = globalsyms[wantedsym->symhash & GLOBALMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol(p, name, opt_case || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'search_local' searches the current local symbol table for the required
** symbol, returning a pointer to its entry if found or nil if not.
*/
static symbol *search_local(symbol *wantedsym) {
  symbol *p;
  const char *name;

  p = (*current_table)[wantedsym->symhash & LOCALMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol(p, name, opt_case || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'search_common' searches the common block symbol table for the required
** symbol, returning a pointer to its entry if found or nil if not.
*/
symbol *search_common(symbol *wantedsym) {
  symbol *p;
  const char *name;

  p = commonsyms[wantedsym->symhash & COMMONMASK];
  name = wantedsym->symtptr->symtname;

  return find_symbol(p, name, opt_case || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'find_common' is called to look for the common block 'name'. It
** has to create a dummy 'symbol' and 'symt' entry for the common
** block and returns a pointer to the symbol table for the common
** block if found or 'nil'
*/
symbol *find_common(const char *name) {
  symbol dummysym;
  symtentry dummysymt;
  dummysym.symhash = hash(name);
  dummysym.symtptr = &dummysymt;
  dummysymt.symtname = name;
  dummysymt.symtattr = 0;
  return search_common(&dummysym);
}

/*
** 'add_unresolved' adds a symbol to one of the missing symbol
** lists if there is no reference to it already there
*/
static void add_unresolved(missing **tree, symbol *sp, filelist *fp) {
  const char *sname = sp->symtptr->symtname;

  while (*tree != NIL) {
    int compval = (opt_case || (sp->symtptr->symtattr & SYM_IGNCASE) != 0) ?
     stricmp(sname, (*tree)->symname) : strcmp(sname, (*tree)->symname);

    if (compval == 0)
      return;
 
    if (compval > 0) {
      tree = &(*tree)->right;
    } else {
      tree = &(*tree)->left;
    }
  }

  if (*tree == NIL) {	/* New unresolved or weak symbol */
    if ((*tree = allocmem(sizeof(missing)))==NIL) error("Fatal: Out of memory in 'add_unresolved'");
    (*tree)->symname = sname;
    (*tree)->filename = fp->chfilename;
    (*tree)->left = (*tree)->right = NIL;
  }
}

/*
** 'list_unresolved' is called when one or more symbol references are
** left unresolved to build up lists of missing symbols and unresolved
** weak external references. It is permissible to leave weak symbols
** unresolved, although the linker prints out a list of them at the
** end.
*/
static void list_unresolved_print(missing *tree) {
  if (tree != NIL) {
    list_unresolved_print(tree->left);
    error("    '%s' referenced in '%s'", tree->symname, tree->filename);
    list_unresolved_print(tree->right);
  }
}


static void list_unresolved_add(symbol *sp, filelist *fp, missing **misslist, missing **weaklist) {
  if (sp != NIL) {
    list_unresolved_add(sp->left, fp, misslist, weaklist);

    if ((sp->symtptr->symtattr & SYM_WEAKREF) != 0) { /* No problem if weak external ref */
      add_unresolved(weaklist, sp, fp);
      numwanted--;

    } else {
      add_unresolved(misslist, sp, fp);
    }

    list_unresolved_add(sp->right, fp, misslist, weaklist);
  }
}


static void list_unresolved(void) {
  filelist *fp;
  missing *misslist = NIL, *weaklist = NIL;

  fp = aofilelist;

  do {
    list_unresolved_add(fp->symtries.wantedsyms, fp, &misslist, &weaklist);
    fp = fp->nextfile;
  } while (fp != NIL);

  if (weaklist != NIL && (opt_verbose || opt_info)) {
    error("Warning: The following symbols are unresolved 'weak' references:");
    list_unresolved_print(weaklist);
  }

  if (misslist != NIL) {
    error("Error: The following symbols could not be found:");
    list_unresolved_print(misslist);
  }
}

/*
** 'search_lib' seaches the current library's symbol table for
** the symbol 'wantedsym'. If it is found, the function returns
** a pointer to the library symbol table entry, otherwise it
** returns 'nil'.
*/
static libentry *search_lib_tree(libentry *lp, const char *name, bool ignorecase) {

  if (lp != NIL) {
    int compval = ignorecase ? stricmp(name, lp->libname) : strcmp(name, lp->libname);
 
    if (compval == 0)
      return lp;
 
    if (compval > 0) {
      return search_lib_tree(lp->right, name, ignorecase);
    } else {
      return search_lib_tree(lp->left, name, ignorecase);
    }
  }
 
  return NIL;
}

static libentry *search_lib(symbol *wantedsym) {
  return search_lib_tree((*current_libsyms)[wantedsym->symhash & LIBEMASK], wantedsym->symtptr->symtname,
                         opt_case || (wantedsym->symtptr->symtattr & SYM_IGNCASE) != 0);
}

/*
** 'get_nextlib' returns a pointer to the next entry in the hash chain that
** matches the one passed to it
*/
static libentry *get_nextlib(libentry *lp) {
  int hashval;
  char *name;
  hashval = lp->libhash;
  name = lp->libname;
  while (lp!=NIL && (lp->libhash!=hashval || strcmp(name, lp->libname)!=0)) lp = lp->left;
  return lp;
}

/*
** 'remove_entry' deletes an entry from a file's wanted list
*/
static void remove_entry(symbol **wp) {
 symbol *sym = *wp, *subnode;
 int before, after;

  /* Binary tree removal.

     If there's no children, nothing needs to be done.
     If there's only one child on either side, then move it up to the current position.

     If there's two children, then when a subtree exists on the right side, replace it
     with the inorder successor.

  */

  if (sym->left == NIL && sym->right != NIL) {
    subnode = sym->right;
    freemem(sym, sizeof(symbol));
    *wp = subnode;

  } else if (sym->left != NIL && sym->right == NIL) {
    subnode = sym->left;
    freemem(sym, sizeof(symbol));
    *wp = subnode;

  } else if (sym->left != NIL && sym->right != NIL) {
    /* Find successor */
    symbol **success = &sym->right;

    if (!(*success)->left) {
      /* When there's no left successor, just move the right node up */

      (*success)->left = sym->left;
      *wp = *success;

    } else {
      while ((*success)->left != NIL) {
        success = &(*success)->left;
      }
      subnode = *success;

      *success = subnode->right;
      subnode->left = sym->left;
      subnode->right = sym->right;
      *wp = subnode;
    }

    freemem(sym, sizeof(symbol));

  } else {
    freemem(sym, sizeof(symbol));
    *wp = NULL;
  }

  numwanted--;
}

/*
** 'check_library' is called to go through a file's 'wanted' list and loads
** any library members that satisfy any of the external references in the
** 'wanted' list. It returns 'TRUE' if this was achieved successfully
** otherwise it returns 'FALSE'. Note that it is necessary to check the
** global symbol table before searching the library to see if the reference
** can be satisfied by a library member now in memory.
**
** The search process is complicated is the presence of strong symbols.
** Cases to consider:
** a) A non-strong definition exists and the library module contains
**    a strong one
** b) A strong definition exists and the library module contains a
**    non-strong one.
**
** a) cannot be dealt with unless the linker is rewritten. All references
**    will have been resolved to the non-strong definition before the
**    strong definition was found and the information to enable the
**    linker to go back and correct the references will have been
**    discarded. (Put out a warning message about this? Will it ever
**    happen?)
** b) At this point, the module containing the reference and the definition
**    have to be different. If the module that contains the strong definition
**    is the one that contains the reference, the non-strong definition should
**    be used otherwise the strong one is the one to employ.
**
** If opt_leaveweak is enabled, weak symbol references that would lead to
** a library module being loaded are ignored. If a library member that
** satisfies the reference is already in memory, it is still used. This
** could lead to some confusion as it is possible that a later reference
** could load a module that would resolve the original weak ref. Unless
** '-rescan' is used, that original ref will remain unresolved. OTOH, if
** the module is already in memory, it will be resolved. To put it another
** way, whether or not a weak reference is resolved depends on the order
** in which the non-weak and weak references are encountered...
*/
static bool check_library_tree(symbol **wp, filelist *fp) {
  bool ok = TRUE;

  if (*wp != NIL) {
    symbol *sp;
    libentry *lp;

    ok &= check_library_tree(&(*wp)->left, fp);
    ok &= check_library_tree(&(*wp)->right, fp);

/*
** First, check if the symbol is now available. It is possible that a
** library member now loaded contains the symbol that is required here.
** If so, we should use this symbol rather than search the library
** again. A complication, as ever, is the possibility of a strong
** symbol...
*/
    sp = search_global(*wp);	/* Is symbol now available? */
    if (sp != NIL) {		/* Yes */
/*
** At this point, a module that contains a symbol definition is in
** memory. There are two possibilities to deal with here:
** a)  The symbol definition is strong and is located in the module
**     whose external references are being processed.
** b)  The symbol definition is non-strong
**
** In case (a), the symbol is not the one to use. We have to search the
** library for the symbol and, as the module being processed is the one
** with the strong definition, we have to find a non-strong version.
** of course, if the strong symbol came from this library, the search
** mechanism will find it first, so we have to ignore that entry and
** find another...
** In case (b), there is no problem. This is the definition to use
*/
      if ((sp->symtptr->symtattr & SYM_STRONG) != 0 &&
       sp->symtptr >= current_symtbase && sp->symtptr < current_symtend && sp->symnormal == NIL) {

        lp = search_lib(*wp);
        if (lp!=NIL && isloaded(lp)) {	/* Found symbol in library. Has member been loaded? */
          lp = get_nextlib(lp);		/* If yes, look for another version */
          if (isloaded(lp)) lp = NIL;
        }

        if (lp != NIL) {
          numfound++;
          ok &= load_member(lp, fp, *wp);	/* Not in memory, so load it */
        }
      }
      else {	/* Non-strong or strong and non-strong def'n known */
        numfound+=1;
        lp = COERCE(sp, libentry*);		/* This is just so that lp!=nil */
      }
    }
    else if (((*wp)->symtptr->symtattr & SYM_WEAKREF) != 0 && opt_leaveweak) {	/* Ignore if weak external */
      lp = NIL;
    }
    else {	/* Symbol is not available yet. Search library */
      lp = search_lib(*wp);
      if (lp != NIL) {	/* Entry found */
        numfound+=1;
/*
** Load member. The member containing the ref has to be different to the
** one containing the definition, so there are no problems with strong
** definitions and references in the same modules meaning we've got the
** wrong module
*/
        ok &= load_member(lp, fp, *wp);
        sp = search_global(*wp);
      }
    }

    if (ok && lp!=NIL) {	/* Now link symbol and reference */
      if (sp==NIL) {
        error("Error: Cannot find symbol '%s' in library member '%s(%s)'. Is library corrupt?",
         (*wp)->symtptr->symtname, current_lib->libname, lp->libmember);
      }
      else {
        symtentry *wstp = (*wp)->symtptr;	/* OBJ_SYMT entry of external reference */

        if (sp->symnormal!=NIL && sp->symtptr>=current_symtbase && sp->symtptr<current_symtend) {
          wstp->symtarea.symdefptr = sp->symnormal;
        }
        else {	/* Only one def or strong def in another module */
          wstp->symtarea.symdefptr = sp->symtptr;
        }
        if ((wstp->symtattr & SYM_WEAKREF)!=0) {	/* Resolving a weak ref? */
          wstp->symtattr-=SYM_WEAKREF;
        }
      }
      remove_entry(wp);
    }
  }
  return ok;
}



static bool check_library(filelist *fp) {
  libentry *lp;
  symtentry *wstp;	/* OBJ_SYMT entry of external reference */
  current_symtbase = fp->objsymtptr;	/* For detecting 'strong symbol' refs */
  current_symtend = fp->objsymtptr+fp->objsymtsize;	/* These point at module containing ref */

  return check_library_tree(&fp->symtries.wantedsyms, fp);
}

/*
** 'resolve_refs' is called to try to resolve a file's external symbols
** references using the local, global and common symbol tables. It also
** takes care of strong and non-strong references. This is a bit messy
** so an explanation might be useful for future reference.
**
** In the file being processed, if both a strong and a non-strong
** definition of a symbol exists, the non-strong one is used. In all
** other cases the strong definition is the one employed. The presence of
** two versions of the symbol is indicated by the value of 'symnormal'.
** If it is 'nil' then only one definition exists (strong or non-strong;
** it does not matter which). If not 'nil', then two definitions exists
** and 'symnormal' points at the SYMT entry of the non-strong definition.
** The routine detects whether or not the strong definition of the symbol
** is in the current file by comparing the address of that symbol's SYMT
** entry against the base and end address of the current file's OBJ_SYMT
** chunk.
**
** A complication is that when resolving a symbol reference in the same
** module as its definition, if the definition is a strong definition, it
** cannot be assumed it will be the correct one as a non-strong definition
** might pop up later during the library scans. To allow for this, the
** symbol resolution has to be defered until the 'last pass' when invented
** symbols are dealt with.
*/
void resolve_refs_tree(symbol **wp) {

  if (*wp != NIL) {
    symbol *sp;

    resolve_refs_tree(&(*wp)->left);
    resolve_refs_tree(&(*wp)->right);

    sp = search_local(*wp);
    if (sp == NIL) {
      sp = search_global(*wp);
      if (sp==NIL) sp = search_common(*wp);
    }

    if (sp != NIL) { /* Symbol found */
      symtentry *wstp = (*wp)->symtptr;	/* Point at SYMT entry for the external ref */
      symtentry *stp  = sp->symtptr;	/* Point at first SYMT entry for definition of symbol */

      if (stp>=current_symtbase && stp<=current_symtend) {	/* Is ref in same module as def'n? */
        if (sp->symnormal != NIL) {			/* Non-strong def'n exists - use that */
          wstp->symtarea.symdefptr = sp->symnormal;	/* Point at non-strong definition's SYMT entry */
        }
        else if ((stp->symtattr & SYM_STRONG)==0) {	/* Non-strong symbol */
          wstp->symtarea.symdefptr = stp;
        }
        else {	/* Strong def'n: do not resolve at this time */
          sp = NIL;		/* Pretend symbol was not found */
        }
      }
      else {	/* Symbol def'n is elsewhere */
        wstp->symtarea.symdefptr = stp;		/* Point at strong or only definition's SYMT entry */
      }

      if (sp != NIL) {
        remove_entry(wp);
        if ((wstp->symtattr & SYM_WEAKREF) != 0) {  /* Resolving a weak ref? */
          wstp->symtattr -=SYM_WEAKREF;
        }
      }
    }
  }
}


void resolve_refs(filelist *fp) {

  current_table = &fp->localsyms;
  current_symtbase = fp->objsymtptr;	/* For detecting 'strong symbol' refs */
  current_symtend = fp->objsymtptr+fp->objsymtsize;

  resolve_refs_tree(&fp->symtries.wantedsyms);
}

/*
** 'resolve' is the main function called when trying to resolve
** symbol references. This process is carried out in three stages.
** The first step is to go through all the files in the file
** list and try to resolve symbols between them. If there are
** any unresolved symbols after this, the routine then goes
** through the libraries to find the symbols. Library members that
** satisfy references are loaded immediately and any references they
** contain that can be resolved from the loaded AOF files are
** handled there and then. The files are added to the file list
** anyway, so searching the current library 'comes out in the
** wash'. If the '-rescan' option is used, it continues to search
** the libraries until all references have been satisfied or it
** cannot find them in any library. If a partially-linked AOF
** file is to be created, the symbol resolution step ends here.
** If any other type of image is being created, The final step is
** to search the global symbol table again. This is necessary to
** deal with strong symbols and because global symbols such as
** 'wibble$$Base' can be invented as a side effect of loading
** library members. If there is a reference somewhere to a
** yet-to-be-invented symbol, the library search mechanism will not
** find it.
**
** One thing to note is how multiple scans of libraries are handled.
** What happens is that the library list is turned into a circular
** list and the code just keeps going around the ring until either
** all references have been satisfied or all the libraries have
** been searched for a particular symbol.
*/
bool resolve(void) {
  filelist *fp;
  libheader *lp, *lastlp;
  bool ok;

  link_state = AOF_SEARCH;
  fp = aofilelist;
  if (fp==NIL) error("Fatal: There is nothing to link!");
  if (opt_verbose) error("Drlink: Resolving symbol references (%d to find)...", numwanted);

  while (fp!=NIL) {	/* Resolve symbol references between AOF files */
    if (fp->symtries.wantedsyms != NIL) resolve_refs(fp);
    fp = fp->nextfile;
  }

  /* Now search the libraries */
  link_state = LIB_SEARCH;
  opt_rescan = opt_rescan && liblist!=liblast;
  if (opt_rescan && liblist!=NIL) liblast->libflink = liblist;	/* Make library list circular */

  lp = liblist;
  ok = TRUE;
  lastlp = NIL;
  numfound = 0;

  while (lp != NIL && numwanted != 0 && lp != lastlp && ok) {
    numfound = 0;

    if (opt_verbose) {
      error("Drlink: Searching library '%s' to resolve symbol references (%d to find)...",
       lp->libname, numwanted);
    }

    ok = open_library(lp);
    fp = aofilelist;

    while (fp!=NIL && numwanted != 0 && ok) {
      if (fp->symtries.wantedsyms!=NIL) ok = check_library(fp);
      fp = fp->nextfile;
    }

    if (numfound!=0 || lastlp==NIL) lastlp = lp;
    close_library(lp);
    lp = lp->libflink;
  }
  free_libmem();

  if (imagetype == AOF) {		/* Quick way out for partially-linked AOF files */
    if (opt_verbose) {
      error("Drlink: %d reference(s) left unresolved in partially-linked AOF file", numwanted);
    }
    numwanted = 0;

  } else {
    /* Now check for invented entries and unresolved strong symbols */
    fp = aofilelist;
    while (fp!=NIL && numwanted!=0) {
      if (fp->symtries.wantedsyms!=NIL) resolve_refs(fp);
      fp = fp->nextfile;
    }
    if (numwanted > 0 && !got_errors()) list_unresolved();
  }

  return numwanted == 0;
}

/*
** 'relocate_symbols' goes through the OBJ_SYMT chunk of each
** AOF file and relocates each entry in it.
*/
void relocate_symbols(void) {
  filelist *fp;
  symtentry *sp;
  int n, last;
  fp = aofilelist;

  do {
    sp = fp->objsymtptr;
    last = fp->symtcount;
    for (n = 1; n<=last; n++) {
      if (sp->symtarea.areaptr != (void *)4) {

        if ((sp->symtattr & (SYM_DEFN|SYM_ABSVAL))==SYM_DEFN) {	/* Relocate if not absolute */
          sp->symtvalue+=sp->symtarea.areaptr->arplace;
        }
      }
      sp++;
    }
    fp = fp->nextfile;
  } while (fp!=NIL);
}

/*
** 'define_symbol' is used to set the given symbol to the supplied value
*/
void define_symbol(symbol *sp, unsigned int value) {
  sp->symtptr->symtvalue = value;
}

/*
** 'make_symbol' creates entries in the either the global symbol table
** or the common block table for pre-defined linker symbols and other
** special entries. The table used is controlled by whether the 'common'
** attribute is set or not. Note that this is a bit kludgy in that the
** 'common' symbol attribute denotes a reference to a common block and
** not a common block definition! Another point to note that entries
** are marked as being relocatable although they are not relocated by
** the linker at all.
*/
symbol *make_symbol(const char *name, unsigned int attributes) {
  unsigned int hashval;
  symbol *sp;
  symtentry *stp;

  sp = allocmem(sizeof(symbol));
  stp = allocmem(sizeof(symtentry));
  if (sp==NIL || stp==NIL) error("Fatal: Out of memory in 'make_symbol'");
  hashval = hash(name);
  stp->symtname = name;
  stp->symtattr = attributes | SYM_LINKDEF;
  stp->symtvalue = 0;
  stp->symtarea.areaptr = NIL;
  sp->symhash = hashval;
  sp->symtptr = stp;
  sp->symnormal = NIL;
  sp->left = sp->right = NIL;

  if ((attributes & SYM_COMMON)==0) {	/* Put in global table */
    symbol **insert = insert_symbol(&globalsyms[hashval & GLOBALMASK], name);
    *insert = sp;

  }
  else {	/* Put in common block table */
    symbol **insert = insert_symbol(&commonsyms[hashval & COMMONMASK], name);
    *insert = sp;

  }

  totalsymbols+=1;
  return sp;
}

/*
** 'new_symbol' is called to create a symbol that will be added to the
** low-level debugging tables
*/
static symbol *new_symbol(const char *name) {
  if (opt_keepdebug) lldsize+=strlen(name)+sizeof(char);
  return make_symbol(name, SYM_GLOBAL);
}

/*
** 'find_areasymbol' is called to try to find a globally defined symbol
** from the area 'ap'. It returns a pointer to a name or NIL if it
** cannot find anything
*/
const char *find_areasymbol(arealist *ap) {
  symtentry *sp;
  filelist *fp;
  const char *anp;
  unsigned int symcount, n;
  anp = ap->arname;
  fp = ap->arfileptr;
  sp = fp->objsymtptr;
  symcount = fp->symtcount;
  for (n = 1; n<=symcount && (sp->symtarea.areaptr!=ap || (sp->symtattr&SYM_SCOPE)!=SYM_GLOBAL); n++) sp++;
  if (n<=symcount) {
    return sp->symtname;
  }
  else {
    return NIL;
  }
}

/*
** 'find_nonstrong' is called to locate a non-strong version of the
** symbol passed to it, returning a pointer to the OBJ_SYMT entry
** of the non-strong symbol. This is needed to deal with strong
** symbols, when a relocation refers to the *definition* of a strong
** symbol in the file in which the strong symbol was defined. The
** relocation has to refer to the non-strong version, which would
** have been dragged in by the symbol resolution code.
*/
symtentry *find_nonstrong(symtentry *stp) {
  symbol dummysym;
  symtentry dummysymt;
  const char *name;
  symbol *sp;
  name = stp->symtname;
  dummysym.symhash = hash(name);
  dummysym.symtptr = &dummysymt;
  dummysymt.symtname = name;
  dummysymt.symtattr = 0;
  sp = search_global(&dummysym);
  if (sp==NIL) error("Fatal: Could not find non-strong version of symbol '%s' in 'find_nonstrong'", name);
  return sp->symnormal;
}

/* The next few functions deal with the creation of the symbol listing */

static symtentry **symbinfotable;
static unsigned int symbcount;

/*
** 'build_symblist' builds a list of the symbols in the executable
** image. This is later sorted into address order and printed
*/
static void build_symblist(void) {
  filelist *fp;
  symtentry *sp;
  symtentry **sip;
  unsigned int n, count;
  symbcount = 0;
  fp = aofilelist;
  sip = symbinfotable;
  do {
    sp = fp->objsymtptr;
    count = fp->symtcount;
    for (n = 1; n<=count; n++) {
      if ((sp->symtattr & (SYM_ABSVAL|SYM_DEFN))==SYM_DEFN && strcmp(sp->symtname, "__codeseg")!=0) {
        if ((sp->symtattr & SYM_ABSVAL)!=0 || (sp->symtarea.areaptr!=NIL && sp->symtarea.areaptr->arefcount!=0)) {
          *sip = sp;
          sip++;
          symbcount+=1;
        }
      }
      sp++;
    }
    fp = fp->nextfile;
  } while (fp!=NIL);
  if (imagetype!=AOF) {	/* These symbols will not exist in a partially-linked file */
    *sip = image_robase->symtptr;
    sip++;
    *sip = image_rolimit->symtptr;
    sip++;
    *sip = image_rwbase->symtptr;
    sip++;
    *sip = image_rwlimit->symtptr;
    sip++;
    *sip = image_zibase->symtptr;
    sip++;
    *sip = image_zilimit->symtptr;
    sip++;
    symbcount+=6;
    if (got_oldlibs) {	/* Include symbols for old-style libraries */
      *sip = image_codebase->symtptr;
      sip++;
      *sip = image_codelimit->symtptr;
      sip++;
      *sip = image_database->symtptr;
      sip++;
      *sip = image_datalimit->symtptr;
      sip++;
      symbcount+=4;
    }
  }
}

static void print_symblist(void) {
  symtentry **sip;
  unsigned int n;
  sip = symbinfotable;
  for (n = 1; n<=symbcount; n++) {
    write_symbol(*sip);
    sip++;
  }
}

/*
** 'symtcmp' compares the values of the two symbols passed to it
*/
static int symtcmp(const void *first, const void *second) {
  if (((*COERCE(first, symtentry **)))->symtvalue<((*COERCE(second, symtentry **)))->symtvalue) return -1;
  if (((*COERCE(first, symtentry **)))->symtvalue>((*COERCE(second, symtentry **)))->symtvalue) return 1;
  return 0;
}

/*
** 'print_symbols' is called to print a listing of the symbols found
** in the image file. These are listed by file.
*/
void print_symbols(void) {
  if ((symbinfotable = allocmem(totalsymbols*sizeof(symtentry *)))==NIL) {
    error("Error: Cannot create symbol listing");
    return;
  }
  open_symbol();
  build_symblist();
  qsort(symbinfotable, symbcount, sizeof(symtentry *), symtcmp);
  print_symblist();
  close_symbol();
}

/*
** The next few functions deal with the processing needed for
** constructor and destructor lists generated by the Acorn C++
** compiler
*/

#define LINKNAME "__link"
#define HEADNAME "__head"

/*
** 'build_cdlist' builds the constructor-destructor linked list
** that C++ programs contain. These are denoted by a local symbol in
** each AOF file with the name of '__link'. '__head' points at the
** first entry in the list.
*/
void build_cdlist(void) {
  symbol tempsym;
  symtentry tempsymt;
  unsigned int lastlink, offset;
  symbol *sp;
  symtentry *stp;
  arealist *ap;
  filelist *fp;
  tempsym.symhash = hash(LINKNAME);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = LINKNAME;
  fp = aofilelist;
  lastlink = 0;
  if (opt_verbose) error("Drlink: Performing C++ specific operations...");
  while (fp!=NIL) {
    if (fp->chfilesize!=0) {	/* file size = 0 means filelist entry is a dummy one */
      current_table = &fp->localsyms;
      if ((sp = search_local(&tempsym))!=NIL) {
        stp = sp->symtptr;
        ap = stp->symtarea.areaptr;
        offset = stp->symtvalue-ap->arplace;	/* Offset within area of __link */
        *(ap->arobjdata+offset) = lastlink;	/* ???? */
        lastlink = stp->symtvalue;
      }
    }
    fp = fp->nextfile;
  }
  tempsym.symhash = hash(HEADNAME);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = HEADNAME;
  sp = search_global(&tempsym);
  if (sp!=NIL) {	/* Just to prevent embarrassing problems if this is not really C++ */
    stp = sp->symtptr;
    ap = stp->symtarea.areaptr;
    offset = stp->symtvalue-ap->arplace;	/* Offset within area of __head */
    *(ap->arobjdata+offset) = lastlink;		/* ???? */
  }
}

/*
** 'find_cdareas' is called to mark all areas that contain constructors
** or destructors as 'not deletable'
*/
void find_cdareas(void) {
  symbol tempsym;
  symtentry tempsymt;
  symbol *sp;
  filelist *fp;
  tempsym.symhash = hash(LINKNAME);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = LINKNAME;
  fp = aofilelist;
  while (fp!=NIL) {
    if (fp->chfilesize!=0) {	/* file size = 0 means filelist entry is a dummy one */
      current_table = &fp->localsyms;
      if ((sp = search_local(&tempsym))!=NIL) {	/* File contains a constructor/destructor */
        mark_area(sp->symtptr->symtarea.areaptr);
      }
    }
    fp = fp->nextfile;
  }
  tempsym.symhash = hash(HEADNAME);
  tempsym.symtptr = &tempsymt;
  tempsymt.symtname = HEADNAME;
  sp = search_global(&tempsym);
  if (sp!=NIL) {	/* Just to prevent embarrassing problems if this is not really C++ */
    mark_area(sp->symtptr->symtarea.areaptr);
  }
}

/*
** 'init_symbols' is called at the start of the link to initialise
** the global symbol table and other stuff to do with symbols
*/
void init_symbols(void) {
  int i;
  for (i = 0; i<MAXGLOBALS; i++) globalsyms[i] = NIL;
  for (i = 0; i<MAXCOMMON; i++) commonsyms[i] = NIL;
  wantedlist = NIL;
  totalsymbols = numwanted = lldsize = 0;
}

/*
** 'create_linksyms' is called to add the standard linker
** symbols to the symbol table, unless the image file being
** created is a partially-linked AOF file in which case
** the symbols have to be left out as it is important that
** references to them are not resolved at this stage
*/
void create_linksyms(void) {
  if (imagetype!=AOF) {
    image_robase = new_symbol("Image$$RO$$Base");
    image_rolimit = new_symbol("Image$$RO$$Limit");
    image_rwbase = new_symbol("Image$$RW$$Base");
    image_rwlimit = new_symbol("Image$$RW$$Limit");
    image_zibase = new_symbol("Image$$ZI$$Base");
    image_zilimit = new_symbol("Image$$ZI$$Limit");
    image_codebase = new_symbol("Image$$CodeBase");
    image_codelimit = new_symbol("Image$$CodeLimit");
    image_database = new_symbol("Image$$DataBase");
    image_datalimit = new_symbol("Image$$DataLimit");
    reloc_code = new_symbol("__RelocCode");
  }
  else {
    image_robase = image_rolimit = image_rwbase = image_rwlimit = NIL;
    image_zibase = image_zilimit = reloc_code = NIL;
    image_codebase = image_codelimit = image_database = image_datalimit = NIL;
  }
}
