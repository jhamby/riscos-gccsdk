/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** Debugging functions for Drlink
*/

#include <stdio.h>
#include "drlhdr.h"
#include "filehdr.h"
#include "symbolhdr.h"
#include "areahdr.h"
#include "debugprocs.h"

void list_areas(arealist *list) {
  arearef *rp;
  while (list!=NIL) {
    printf("Area entry at %p, areaname='%s', hash=%x, base entry at %p, file entry at %p, AT=%x\n",
     list, list->arname, list->arhash, list->arbase, list->arfileptr, list->aratattr);
    printf("  OBJ_AREA at %p, size=%x, relocations at %p, number=%d, area address=%x",
     list->arobjdata, list->arobjsize, list->areldata, list->arnumrelocs, list->arplace);
    if (list->arsymbol!=NIL) printf(", symbol address=%p", list->arsymbol);
    printf("\n");
    printf("  Reference count=%d, reference list at %p", list->arefcount, list->areflist);
    rp = list->areflist;
    if (rp==NIL) {
      printf(", ** No references **\n");
    }
    else {
      printf(", References:\n  ");
      while (rp!=NIL) {
        printf("  %p=%d", rp->arefarea, rp->arefcount);
        rp = rp->arefnext;
      }
      printf("\n");
    }
    list = list->arflink;
  }
}

void list_allareas(void) {
  printf("\n========== Read-only code list ==========\n");
  list_areas(rocodelist);
  printf("\n========== Read-only data list ==========\n");
  list_areas(rodatalist);
  printf("\n========== Read/write code list ==========\n");
  list_areas(rwcodelist);
  printf("\n========== Read/write data list ==========\n");
  list_areas(rwdatalist);
  printf("\n========== Zero-initialised data and common block list ==========\n");
  list_areas(zidatalist);
  printf("\n========== Debug list ==========\n");
  list_areas(debuglist);
}

void list_symbols(symbol *list) {
  symtentry *sp;
  while (list!=NIL) {
    sp = list->symtptr;
    printf("    Symbol at %p, symt at %p, name='%s', hash=%x, attributes=%x, value=%x",
     list, sp, sp->symtname, list->symhash, sp->symtattr, sp->symtvalue);
    if (sp->symtarea.areaptr!=NIL) {
      printf(", area at %p", sp->symtarea.areaptr);
    }
    printf("\n");
    list = list->symflink;
  }
}

void list_symtable(symbol *table[], unsigned int stsize) {
  symbol *list;
  unsigned int n, low;
  printf("  Symbol table at %p\n", table);
  n = 0;
  do {
    low = n;
    while ((list = table[n])==NIL && n<stsize) n+=1;
    if (n!=low) printf("  Index=%2d..%-2d, no entries\n", low, n-1);
    if (n<stsize) {
      printf("  Index=%2d, symbol list starts at %p\n", n, list);
      list_symbols(list);
      n+=1;
    }
  } while (n<stsize);
}

void list_libentries(libentry *list) {
  while (list!=NIL) {
    printf("    Lib entry at %p, name='%s', hash=%x, member name='%s', offset in library=%x, size=%x\n",
     list, list->libname, list->libhash, list->libmember, list->liboffset, list->libsize);
    list = list->libflink;
  }
}

void list_libtable(libtable *table) {
  libentry *list;
  int n;
  printf("  Library symbol table at %p\n", table);
  for (n = 0; n<MAXENTRIES; n++) {
    if ((list = *table[n])==NIL) {
      printf("  Index=%2d, no entries\n", n);
    }
    else {
      printf("  Index=%2d, symbol list starts at %p\n", n, list);
      list_libentries(list);
    }
  }
}

void list_filelist(filelist *list) {
  while (list!=NIL) {
    printf("File def at %p, name='%s', file size=%x\n",
     list, list->chfilename, list->chfilesize);
    printf("  OBJ_HEAD is at %p, size=%x,   OBJ_SYMT is at %p, size=%x\n",
     list->objheadptr, list->objheadsize, list->objsymtptr, list->objsymtsize);
    printf("  OBJ_AREA is at %p, size=%x,   OBJ_STRT is at %p, size=%x\n",
     list->objareaptr, list->objareasize, list->objstrtptr, list->objstrtsize);
    printf("  Areas=%u, symbols=%u Local symbols at %p, 'wanted' list at %p\n",
     list->areacount, list->symtcount, &list->localsyms, list->symtries.wantedsyms);
    list_symtable(list->localsyms, MAXLOCALS);
    list = list->nextfile;
  }
}

void list_liblist(libheader *list) {
  while (list!=NIL) {
    printf("Library def at %p, name='%s', loaded at %p, library size=%x\n",
     list, list->libname, list->libase, list->libextent);
    list = list->libflink;
  }
}

void list_files(void) {
  printf("\n========== AOF file list ==========\n");
  list_filelist(aofilelist);
  printf("\n========== Library list ==========\n");
  list_liblist(liblist);
  printf("\n========== Global Symbol Table ==========\n");
  list_symtable(globalsyms, MAXGLOBALS);
}
