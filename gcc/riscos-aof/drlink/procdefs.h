/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** Functions definitions. All functions called externally are defined
** here
*/

#ifndef procdefs_h
#define procdefs_h

#include "chunkhdr.h"
#include "filehdr.h"

/* -- In 'Drlmain' -- */

extern int align(int value);

/* -- In 'Heap' -- */

extern bool initheap(void);
extern void *allocmem(unsigned int);
extern void freemem(void *, unsigned int);
extern void release_heap(void);
extern void print_heapstats(void);

/* -- In 'Linkfiles' -- */

extern void link_program(void);
extern bool read_tables(filelist *);

/* -- In 'Files' -- */

extern void init_files(void);
extern void alloc_filebuffer(void);
extern void resize_filebuffer(void);
extern bool get_files(const char *);
extern int find_filesize(const char *);
extern bool read_chunk(const char *, int, int, void *);
extern bool cache_files(void);
extern void addto_debuglist(const char *);
extern void check_debuglist(void);
extern bool load_viafile(const char *);
extern bool load_editfile(const char *);
extern bool open_object(const char *);
extern void close_object(void);
extern bool read_libchunkhdr(libheader *);
extern fileinfo examine_file(const char *);
extern filelist *read_member(libentry *, filelist *, symbol *);
extern bool extract_member(chunkindex *);
extern void tidy_files(void);
extern void open_image(void);
extern void write_image(void *, unsigned int);
extern void write_string(const char *);
extern void write_zeroes(unsigned int);
extern void close_image(void);
extern void reset_image(unsigned int);
extern void flush_image(void);
extern void open_symbol(void);
extern void write_symbol(symtentry *);
extern void close_symbol(void);
extern void open_mapfile(void);
extern void write_mapfile(const char *);
extern void close_mapfile(void);

/* -- In 'Libraries' -- */

extern void init_library(void);
extern bool addto_liblist(const char *, unsigned int *, unsigned int);
extern bool read_libinfo(libheader *);
extern bool open_library(libheader *);
extern void close_library(libheader *);
extern bool load_member(libentry *, filelist *, symbol *);
extern bool load_wholelib(const char *, unsigned int);
extern bool isloaded(libentry *);
extern bool isoldlib(void);
extern void free_libmem(void);
extern bool discard_libraries(void);

/* -- In 'Messages' -- */

extern void announce(void);
extern void error(const char *, ...);
extern bool got_errors(void);
#ifdef TARGET_RISCOS
extern void end_throwback(void);
#endif

/* -- In 'Areas' -- */

extern void init_areas(void);
extern int get_type1_type(int);
extern int get_type1_index(int);
extern int get_type2_type(int);
extern int get_type2_index(int);
extern void check_strongrefs(filelist *);
extern bool scan_head(filelist *);
extern symtentry *make_commonarea(symbol *);
extern arealist *find_area(const char *);
extern void free_srchlist(void);
extern void find_unused(void);
extern void check_entryarea(void);
extern void relocate_areas(void);
extern bool relocate(void);
extern bool fixup_relocs(void);
extern void mark_area(arealist *);
extern void create_image(void);
extern void print_areamap(void);
extern void print_unusedlist(void);
extern void print_xref(void);
extern void print_mapfile(void);

/* -- In 'Symbols' -- */

extern int stricmp(const char *, const char*);
extern symbol *make_symbol(const char *, unsigned int);
extern void create_linksyms(void);
extern symbol *create_externref(symtentry *);
extern void init_symbols(void);
extern bool read_symbols(void);
extern bool scan_symt(filelist *);
extern void resolve_refs(filelist *);
extern bool resolve(void);
extern void relocate_symbols(void);
extern void define_symbol(symbol *, unsigned int);
extern int hash(const char *);
extern const char *find_areasymbol(arealist *);
extern symtentry *find_nonstrong(symtentry *);
extern symbol *search_common(symbol *);
extern symbol *find_common(const char *);
extern bool isrelocatable(relocation *);
extern void build_cdlist(void);
extern void find_cdareas(void);
extern void print_symbols(void);
extern void build_symbols(void);

/* -- In 'Linkedit' -- */

extern bool scan_editfile(void);
extern bool verify_edits(void);
extern void init_edit(void);

/* -- In 'Aofiles' -- */

extern unsigned int find_areaindex(arealist *);
extern void create_aofile(void);

/* -- In 'Stdcode' -- */

extern void get_hdrcode(segtype, unsigned int **, unsigned int *);
extern void setup_binhdr(unsigned int *, unsigned int);
extern void setup_aifhdr(unsigned int *, unsigned int);
extern void setup_modcode(unsigned int *);

/* -- In 'Debug' -- */

extern void write_lldtable(void);
extern bool build_debugtables(void);

#endif
