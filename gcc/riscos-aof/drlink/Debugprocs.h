/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** Debugging functions for Drlink
*/

#ifndef debugprocs_h
#define debugprocs_h

extern void list_areas(arealist *);
extern void list_allareas(void);
extern void list_symbols(symbol *);
extern void list_symtable(symbol *[], unsigned int);
extern void list_libentries(libentry *);
extern void list_libtable(libtable *);
extern void list_filelist(filelist *);
extern void list_liblist(libheader *);
extern void list_files(void);

#endif
