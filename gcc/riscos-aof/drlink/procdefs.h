/*
** Drlink AOF Linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** Functions definitions. All functions called externally are defined
** here
*/

#ifndef procdefs_h
#define procdefs_h

#include "chunkhdr.h"
#include "filehdr.h"

/* -- In 'Drlmain' -- */

extern int align (int value);

/* -- In 'Heap' -- */

extern bool initheap (void);
extern void *allocmem (unsigned int);
extern void freemem (void *, unsigned int);
extern void release_heap (void);
extern void print_heapstats (void);

/* -- In 'Linkfiles' -- */

extern void link_program (void);
extern bool read_tables (filelist *);

/* -- In 'Files' -- */

extern void init_files (void);
extern filelist *create_filelist (const char *fname, unsigned int fsize);
extern void addto_filelist (filelist * fp);
extern void free_filelist (filelist * fp);
extern void alloc_filebuffer (void);
extern void resize_filebuffer (void);
extern bool get_files (const char *);
extern int find_filesize (const char *);
extern bool check_and_adjust_chunkheader (chunkheader *ch, const char *filename, unsigned int filesize);
extern unsigned int check_and_get_chunkclass_before_adjust (const chunkhdr *ch, const char *filename, bool *isoldlibP);
extern unsigned int check_and_get_chunkclass (const chunkhdr *ch, const char *filename);
extern bool obj_check_and_adjust (chunkhdr * ch, const char *filename, unsigned int filesize, obj_overview *objoverviewp);
extern void addto_debuglist (const char *);
extern void check_debuglist (void);
extern bool load_viafile (const char *);
extern bool load_editfile (const char *);
extern fileinfo examine_file (const char *);
extern void tidy_files (void);
extern void open_image (void);
extern void write_image (void *, unsigned int);
extern void write_string (const char *);
extern void write_zeroes (unsigned int);
extern void close_image (void);
extern void reset_image (unsigned int);
extern void flush_image (void);
extern void open_symbol (void);
extern void write_symbol (symtentry *);
extern void close_symbol (void);
extern void open_mapfile (void);
extern void write_mapfile (const char *);
extern void close_mapfile (void);
#ifdef WORDS_BIGENDIAN
extern void convert_endian (void *words, size_t size);
#else
# define convert_endian(w, s)
#endif

/* -- In 'Messages' -- */

extern void announce (void);
extern void error (const char *, ...);
extern bool got_errors (void);
#ifndef CROSS_COMPILE
extern void end_throwback (void);
#endif

/* -- In 'Areas' -- */

extern void init_areas (void);
extern int get_type1_type (int);
extern int get_type1_index (int);
extern int get_type2_type (int);
extern int get_type2_index (int);
extern void check_strongrefs (filelist *);
extern bool scan_head (filelist *);
extern symtentry *make_commonarea (symbol *);
extern arealist *find_area (const char *);
extern void free_srchlist (void);
extern void find_unused (void);
extern void check_entryarea (void);
extern void relocate_areas (void);
extern bool relocate (void);
extern bool fixup_relocs (void);
extern void mark_area (arealist *);
extern void create_image (void);
extern void print_areamap (void);
extern void print_unusedlist (void);
extern void print_xref (void);
extern void print_mapfile (void);

/* -- In 'Symbols' -- */

#ifndef HAVE_STRICMP
extern int stricmp (const char *, const char*);
#endif
extern symbol *make_symbol (const char *, unsigned int);
extern void create_linksyms (void);
extern symbol *create_externref (symtentry *);
extern void init_symbols (void);
extern bool read_symbols (void);
extern bool scan_symt (filelist *);
extern void resolve_refs (filelist *);
extern bool resolve (void);
extern void relocate_symbols (void);
extern void define_symbol (symbol *, unsigned int);
extern int hash (const char *);
extern const char *find_areasymbol (arealist *);
extern symtentry *find_nonstrong (symtentry *);
extern symbol *search_common (symbol *);
extern symbol *find_common (const char *);
extern bool isrelocatable (relocation *);
extern void build_cdlist (void);
extern void find_cdareas (void);
extern void print_symbols (void);
extern void build_symbols (void);
extern const char *check_libedit (const char *, const char *, int);

/* -- In 'Linkedit' -- */

extern bool scan_editfile (void);
extern bool verify_edits (void);
extern void init_edit (void);

/* -- In 'Aofiles' -- */

extern unsigned int find_areaindex (arealist *);
extern void create_aofile (void);

/* -- In 'Stdcode' -- */

extern void get_hdrcode (segtype, unsigned int **, unsigned int *);
extern void setup_binhdr (unsigned int *, unsigned int);
extern void setup_aifhdr (unsigned int *, unsigned int);
extern void setup_modcode (unsigned int *);

/* -- In 'Debug' -- */

extern void write_lldtable (void);
extern bool build_debugtables (void);

#endif
