/* ***************** run time linker ****************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

/* **************************************************** */

#define SCRAP_FILE "o.rinkXscrap"

#define RINK_LINKS_ID      "rink"
#define RINK_LINKS_VERSION 100

#define PARAM_LEN 256

#define MAX_HEADER_ENTRIES 64
#define MAX_NAMED_ENTRIES  128

#define CODE_ID_SIZE 8

#define LINKS_BLOCK (8*1024)

#define ALLOWED_AL 2

#define PC_OFFSET 8

#define OS_FILE_READ 17

#define BOOL int
#define TRUE 1
#define FALSE 0

/* **************************************************** */

int link_bug_patch = 0;

#define SET   1
#define UNSET 0

typedef struct
{
  int chunkfield;
  int maxchunks, numchunks;
}
chunk_file_hdr;

#define CHUNKID_SIZE 8
typedef struct
{
  char chunkid[CHUNKID_SIZE];
  int offset, size;
}
chunk_entry;

typedef struct
{
  int name_offset;
  struct
  {
    unsigned int al:8;
    unsigned int at_zero:1;
    unsigned int at_code:1;
    unsigned int at_common:1;
    unsigned int at_common_ref:1;
    unsigned int at_zero_init:1;
    unsigned int at_read_only:1;
    unsigned int at_zero2:1;
    unsigned int at_debug:1;
    unsigned int zero:16;
  }
  info;
  int area_size;
  int relocations;
  int reserved;
}
area_header_entry;

typedef struct
{
  int object_file_type;
  int version;
  int n_areas;
  int n_symbols;
  int entry_area;
  int entry_area_offset;
}
aof_header;

#define RELOC_TYPE_TEST_VAL 1
#define RELOC_FT_IS_WORD    2
#define RELOC_FT_IS_INSTR   3

#define MIN_INSTR_VERSION 310

typedef struct
{
  int offset;
  union
  {
    int v;
    struct
    {
      unsigned int z:31;
      unsigned int n:1;
    }
    type_test;
    struct
    {
      unsigned int sid:16;
      unsigned int ft:2;
      unsigned int r:1;
      unsigned int a:1;
      unsigned int zero:12;
    }
    type1;
    struct
    {
      unsigned int sid:24;
      unsigned int ft:2;
      unsigned int r:1;
      unsigned int a:1;
      unsigned int b:1;
      unsigned int ii:2;
      unsigned int val:1;
    }
    type2;
  }
  info;
}
relocation;

typedef struct
{
  int name_offset;
  union
  {
    struct
    {
      unsigned int at_defined:1;
      unsigned int at_global:1;
      unsigned int at_absolute:1;
      unsigned int at_case:1;
      unsigned int at_weak:1;
      unsigned int at_strong:1;
      unsigned int at_common:1;
      unsigned int zero:25;
    }
    bits;
    unsigned int value;
  }
  info;
  int value;
  int area_name_offset;
}
symbol_entry;

/* **************************************************** */

typedef struct
{				/* remember to update the runtime system! */
  char id[CODE_ID_SIZE];
  int main_version;
  int code_version;
  int fn_entries;
  int code_size;
  int zero_init_size;
  int named_entries;
  int named_offset;
  int reserved1;
  int reserved2;
}
rink_code_hdr;

typedef struct
{
  int fn_offset;
/*  int arguments;*/
}
rink_code_fn_entry;

typedef struct
{
  int fn_offset;
  int name_offset;
}
rink_code_named_fn_entry;

typedef struct
{
  int id;
  int version;
  int n_links;
  int reserved;
}
rink_links_hdr;

typedef struct
{
  signed int offset:30;
  unsigned int type:2;
}
link_info;

typedef struct
{
  link_info sys;
  int index;
}
rink_link;

typedef struct
{
  signed int offset:16;
  signed int index:14;
  signed int type:2;
}
link_info_packed;

#define OFFSET_PACK_LIMIT 0xffff
#define INDEX_PACK_LIMIT  0x3fff

#define TYPE_ADD_BASE      0
#define TYPE_MOD_BRANCH    1
#define TYPE_ADD_FN_ADDR   2
#define TYPE_ADD_FN_PACKED 3

/* **************************************************** */

BOOL verbose = FALSE;
BOOL vverbose = FALSE;

char code_name[PARAM_LEN];
char links_name[PARAM_LEN];
char header_name[PARAM_LEN];
char map_name[PARAM_LEN];
char aof_name[PARAM_LEN];

#define MAX_PARAM 3
char *params[] = { code_name, links_name, header_name, map_name };
char *param_trans[] = { "code", "links", "header description", "map" };

typedef struct
{
  char *name;
  int arguments;
}
header_entry_symbol;

header_entry_symbol header_entry_symbols[MAX_HEADER_ENTRIES];
int header_entries;
char code_id[CODE_ID_SIZE + 4];
int main_version;
int code_version;
int header_total_size;

header_entry_symbol named_entry_symbols[MAX_NAMED_ENTRIES];
int named_entries;
int named_total_symbol_size;

char *aof;
int aof_size;

char *header_cp;
char *areas;
char *identification;
char *symbol_table;
char *string_table;

aof_header *header;

char **aof_chunks[] =
  { &header_cp, &areas, &identification, &symbol_table, &string_table };
char aof_chunkids[7][9] =
  { "OBJ_HEAD", "OBJ_AREA", "OBJ_IDFN", "OBJ_SYMT", "OBJ_STRT", "" };
#define OBJ_IDFN 2
#define MAX_OBJ_CHUNK 4

typedef struct
{
  char *name;			/* pointer to name of area */
  int start_offset;		/* offset in real or zero chunks */
  int size;			/* size of the area */
  int area_index;		/* index of the area within the aof file */
  BOOL is_zero;
}
area_map_entry;

area_map_entry *real_map;
int real_map_entries;
int real_size;
area_map_entry *zero_map;
int zero_map_entries;
int zero_size;

char *code;


BOOL have_valid_output = TRUE;

char *links;
int links_size;
int links_end;
int n_links = 0;

char *map;
int map_entries;

#define MAP_INITIAL_SIZE    (16*1024)
#define MAP_CHUNK           1024

/* **************************************************** */

/* ********************************************************************** */

/* **************************************************************************** map functions */

char *
map_find_name (int n)
{
  int l, i;

  i = 0;
  for (l = 0; l < n; l++)
    {
      i += strlen (map + i) + 1;
    }

  return map + i;
}

int
map_lookup (char *symbol)
{
  int l, i;

  i = 0;
  for (l = 0; l < map_entries; l++)
    {
      if (strcmp (map + i, symbol) == 0)
	{
	  return l;
	}
      i += strlen (map + i) + 1;
    }

  return -1;
}

BOOL
map_load (char *filename)
{
  FILE *f;
  int entries, l;
  char Line[512];
  int MapSize, Offset;
  int LineNumber;

  if ((f = fopen (filename, "r")) == NULL)
    {
      printf ("rink: could not open map file\n\n");
      return FALSE;
    }

  // check that we haven't got an old format map
  if (fscanf (f, "rink_map:%d", &entries) == 1)
    {
      printf
	("rink: old style map supplied - recreate using extractsym and rinkptr\n\n");
      return FALSE;
    }

  fseek (f, 0, SEEK_SET);

  if ((map = malloc (MAP_INITIAL_SIZE)) == 0)
    {
      printf ("rink: no room\n\n");
      return FALSE;
    }
  MapSize = MAP_INITIAL_SIZE;

  map_entries = 0;

  // read in the symbols from the map file
  LineNumber = 0;
  Offset = 0;
  while (fgets (Line, 256, f) != 0)
    {
      LineNumber++;

      // check it's not a comment or a blank line
      if (Line[0] == '#' || Line[0] < ' ')
	continue;

      // terminate the line properly
      l = 0;
      while (Line[l] > ' ')
	{
	  l++;
	}
      // check there isn't a space in there
      if (Line[l] == ' ')
	{
	  printf
	    ("rink: invalid line in pointer block map (contains space) at line %d\n\n",
	     LineNumber);
	  return FALSE;
	}
      // terminate it properly
      Line[l] = '\0';

      // add it as a symbol...
      if ((Offset + l + 4) >= (MapSize))
	{
	  // get some more memory
	  if ((map = realloc (map, MapSize += MAP_CHUNK)) == 0)
	    {
	      printf ("rink: no room\n\n");
	      return FALSE;
	    }
	}
      strcpy (map + Offset, Line);
      Offset += l + 1;
      map_entries++;
    }

  fclose (f);

  return TRUE;
}

/* *************************************************************************** misc functions */

void
delete_file (char *name)
{
  unlink (name);
}

BOOL
save_file (char *name, char *start, int length, int type)
{
  FILE *handle;

  handle = fopen (name, "w");
  fwrite (start, 1, length, handle);
  fclose (handle);
  return TRUE;
}

char *
find_area_address (int index)
{
  int l;
  area_header_entry *ae;
  char *loc = areas;

  ae = (area_header_entry *) (header + 1);
  for (l = 0; l < index; l++)
    {
      loc += ae[l].area_size + ae[l].relocations * sizeof (relocation);
    }

  return loc;
}

relocation *
find_area_reloc_address (int index)
{
  int l;
  area_header_entry *ae;
  char *loc = areas;
  int t;

  ae = (area_header_entry *) (header + 1);
  for (l = 0; l < index; l++)
    {
      t = ae[l].area_size;
      if ((t % 4) != 0)
	t += 4 - (t % 4);

      loc += t + ae[l].relocations * sizeof (relocation);
    }

  t = ae[index].area_size;
  if ((t % 4) != 0)
    t += 4 - (t % 4);

  loc += t;

  return (relocation *) loc;
}

area_map_entry *
find_area_search (char *name, area_map_entry * table, int entries)
{
  int l;

  for (l = 0; l < entries; l++)
    {
      if (strcmp (table->name, name) == 0)
	return table;

      table++;
    }

  return 0;
}

char *
name_of_area (int index)
{
  area_header_entry *ai = (area_header_entry *) (header + 1);

  return string_table + ai[index].name_offset;
}

area_map_entry *
find_area_index (char *name)
{
  area_map_entry *a;

  a = find_area_search (name, real_map, real_map_entries);
  if (a != 0)
    return a;

/*  a = find_area_search(name, zero_map, zero_map_entries);
  if(a != 0) return a;*/

  return 0;
}

/* ********************************************************************* get info on a symbol */

typedef struct
{
  int area_base;
  int value;
  BOOL absolute_value;
  BOOL is_function;
  int fn_index;
  unsigned int at;
}
symbol_info;

  /* fills in a symbol_info given an index into the symbol table,
     returns TRUE if the symbol can be resolved (reports error to user, returns FALSE if not)  */
BOOL
get_symbol_info (int s_index, symbol_info * info)
{
  symbol_entry *se;
  area_map_entry *fa;

  if (s_index >= header->n_symbols)
    {
      printf ("\nrink: non-existant symbol referenced\n\n");
      have_valid_output = FALSE;
      return FALSE;
    }

  /* find symbol definition */
  se = ((symbol_entry *) symbol_table) + s_index;

  info->at = se->info.value;
//printf("\nAT = %X, name_offset = %d\n",se->info.value,se->name_offset);

  if (se->info.bits.at_common == SET)
    {
      printf ("\nrink: fatal error: common symbols not supported\n\n");
      exit (1);
    }

  if (se->info.bits.at_case == SET)
    {
      printf
	("\nrink: fatal error: external case-insensitive matching not supported\n\n");
      exit (1);
    }

  if (se->info.bits.at_absolute == SET && se->info.bits.at_defined == SET)
    {
      info->absolute_value = TRUE;
      info->value = se->value;
      info->is_function = FALSE;
      return TRUE;
    }

  if (se->info.bits.at_defined == SET)
    {				/* symbol defined here */
      /* find base of area */
      if (
	  (fa =
	   find_area_search (string_table + se->area_name_offset, real_map,
			     real_map_entries)) == 0)
	{
	  printf
	    ("\nrink: serious error: area '%s' not found in real or zero-init maps\n\n",
	     string_table + se->area_name_offset);
	  have_valid_output = FALSE;
	  return FALSE;
	}
      info->area_base = fa->start_offset;
      info->value = se->value;
      info->absolute_value = FALSE;
      info->is_function = FALSE;
      return TRUE;
    }

  if (se->info.bits.at_defined == UNSET)
    {				/* symbol is not defined here */
      int index;
      /* search to find it's address in the pointer block */
      if ((index = map_lookup (string_table + se->name_offset)) == -1)
	{
	  printf
	    ("\nrink: serious error: symbol '%s' could not be resolved\n\n",
	     string_table + se->name_offset);
	  have_valid_output = FALSE;
	  return FALSE;
	}
      info->is_function = TRUE;
      info->fn_index = index;
      return TRUE;
    }

  printf ("\nrink: error: unknown symbol type detected\n\n");
  have_valid_output = FALSE;

  return FALSE;
}

/* ****************************************************************************** make header */

BOOL
make_header (void)
{
  int l, ll;
  rink_code_fn_entry *en =
    (rink_code_fn_entry *) (code + sizeof (rink_code_hdr));
  rink_code_named_fn_entry *nn =
    (rink_code_named_fn_entry *) (((char *) en) +
				  (sizeof (rink_code_fn_entry) *
				   header_entries));
  char *named_sym =
    ((char *) nn) + (sizeof (rink_code_named_fn_entry) * named_entries);
  symbol_entry *se;
  area_map_entry *ae;
  BOOL null;


  for (l = 0; l < header_entries; l++)
    {
      null = FALSE;
      /* find the symbol */
      se = (symbol_entry *) symbol_table;
      for (ll = 0;; ll++)
	{
	  if (ll >= header->n_symbols)
	    {
	      printf
		("rink: warning: header function '%s' not found, null entry assumed\n\n",
		 header_entry_symbols[l].name);
	      null = TRUE;
	      break;
	    }

	  if (strcmp
	      (header_entry_symbols[l].name,
	       string_table + se->name_offset) == 0)
	    break;

	  se++;
	}

      if (null == FALSE)
	{
	  if (
	      (ae =
	       find_area_search (string_table + se->area_name_offset,
				 real_map, real_map_entries)) == 0)
	    {
	      printf
		("rink: error: header symbol '%s' is not in a code area\n\n",
		 header_entry_symbols[l].name);
	      return FALSE;
	    }

	  en->fn_offset = ae->start_offset + se->value;
/*      en->arguments = header_entry_symbols[l].arguments;*/
	  en++;
	}
      else
	{
	  en->fn_offset = 0;
/*      en->arguments = 0;*/
	  en++;
	}
    }

  for (l = 0; l < named_entries; l++)
    {
      /* find the symbol */
      se = (symbol_entry *) symbol_table;
      for (ll = 0;; ll++)
	{
	  if (ll >= header->n_symbols)
	    {
	      printf ("rink: error: named entry function '%s' not found\n\n",
		      named_entry_symbols[l].name);
	      return FALSE;
	      break;
	    }

	  if (strcmp
	      (named_entry_symbols[l].name,
	       string_table + se->name_offset) == 0)
	    break;

	  se++;
	}

      if (
	  (ae =
	   find_area_search (string_table + se->area_name_offset, real_map,
			     real_map_entries)) == 0)
	{
	  printf
	    ("rink: error: named entry symbol '%s' is not in a code area\n\n",
	     named_entry_symbols[l].name);
	  return FALSE;
	}

      nn->fn_offset = ae->start_offset + se->value;
      nn->name_offset = named_sym - code;
      nn++;

      strcpy (named_sym, named_entry_symbols[l].name);
      named_sym += strlen (named_sym) + 1;
    }

  return TRUE;
}

/* ********************************************************************** perform relocations */

void
links_check_size (int data_size)
{
  if ((links_end + data_size + 4) > links_size)
    {
      if ((links = realloc (links, links_size += LINKS_BLOCK)) == NULL)
	{
	  printf ("rink: no room\n\n");
	  exit (1);
	}
    }
}

void
add_link (rink_link * link)
{
  links_check_size (sizeof (rink_link));

  if (vverbose)
    {
      switch (link->sys.type)
	{
	case TYPE_ADD_BASE:
	  printf ("\nbase relocation      %d", link->sys.offset);
	  break;

	case TYPE_MOD_BRANCH:
	  printf ("\nbranch modification  %-8d %-22s", link->sys.offset,
		  map_find_name (link->index));
	  break;

	case TYPE_ADD_FN_ADDR:
	  printf ("\nsymbol relocation    %-8d %s", link->sys.offset,
		  map_find_name (link->index));
	  break;

	default:
	  break;
	}
    }

  if (link->sys.type == TYPE_MOD_BRANCH
      && link->sys.offset < OFFSET_PACK_LIMIT
      && link->index < INDEX_PACK_LIMIT)
    {
      link_info_packed pl;
      /* change link into a packed link */

      pl.offset = link->sys.offset;
      pl.index = link->index;
      pl.type = TYPE_ADD_FN_PACKED;

      *((link_info_packed *) (links + links_end)) = pl;
      links_end += sizeof (link_info_packed);
      n_links++;

      if (vverbose)
	printf (" (packed)");

      return;
    }

  *((rink_link *) (links + links_end)) = *link;
  links_end +=
    (link->sys.type ==
     TYPE_ADD_BASE) ? sizeof (link_info) : sizeof (rink_link);
  n_links++;
}

BOOL
make_relocations (void)
{
  char id[] = RINK_LINKS_ID;
  int area;
  int reloc, n;
  unsigned int nn;
  area_header_entry *ae;
  relocation *re;
  BOOL type2;
  rink_link link;
  int base;
  area_map_entry *fa;
  symbol_info si;

  if (verbose)
    printf ("rink: generating run-time relocation table\n\n");

  if (vverbose)
    printf
      ("links:\ntype                 offset   symbol\n-------------------- -------- ----------------------");

  /* set up links block */
  if ((links = malloc (LINKS_BLOCK)) == NULL)
    {
      printf ("rink: no room\n\n");
      return FALSE;
    }
  links_size = LINKS_BLOCK;
  /* make links file header */
  ((rink_links_hdr *) links)->id = *((int *) id);
  ((rink_links_hdr *) links)->version = RINK_LINKS_VERSION;
  n_links = 0;
  ((rink_links_hdr *) links)->reserved = 0;

  links_end = sizeof (rink_links_hdr);

  /* go through each area in turn relocating or generating a run-time link */
  for (area = 0; area < real_map_entries; area++)
    {
      ae = ((area_header_entry *) (header + 1)) + real_map[area].area_index;
      re = find_area_reloc_address (real_map[area].area_index);

      for (reloc = 0; reloc < ae->relocations; reloc++)
	{
/*printf("%X %X\n", re->offset, re->info.v);*/
	  if (re->info.type_test.n == RELOC_TYPE_TEST_VAL)
	    type2 = TRUE;
	  else
	    type2 = FALSE;

	  if ((type2 ? re->info.type2.ft : re->info.type1.ft) !=
	      RELOC_FT_IS_WORD
	      && ((type2 ? re->info.type2.ft : re->info.type1.ft) !=
		  RELOC_FT_IS_INSTR || header->version < MIN_INSTR_VERSION))
	    {
	      printf
		("rink: serious error: %s relocation in area '%s' does not refer to a word or an instruction\n",
		 type2 ? "type 2" : "type 1", real_map[area].name);
	      have_valid_output = FALSE;
	      re++;
	      continue;
	    }

	  if ((type2 ? re->info.type2.r : re->info.type1.r) == UNSET)
	    {			/* additive relocation */


	      if ((type2 ? re->info.type2.a : re->info.type1.a) == UNSET)
		{		/* internal relocation */
/*printf("internal relocation\n");*/
		  /* need to add the base address of the area to this field */
		  /* first, find the offset of the base of the area to which this relocation applies */
		  if (type2 == FALSE)
		    {
/*printf("type 1\n");*/
		      base = real_map[area].start_offset;
		    }
		  else
		    {
/*printf("search for area '%s'\n", name_of_area(re->info.type2.sid));*/
		      /* find the area specified by the SID, and whether it's in the zero-init area */
		      if (
			  (fa =
			   find_area_search (name_of_area
					     (re->info.type2.sid), real_map,
					     real_map_entries)) == 0)
			{
			  printf
			    ("rink: serious error: area '%s' not found in real or zero-init maps\n\n",
			     name_of_area (re->info.type2.sid));
			  have_valid_output = FALSE;
			  re++;
			  continue;
			}
		      base = fa->start_offset;
		    }

		  /* next, add the offset of the base of the area from the field */
		  *((int *) (code + real_map[area].start_offset + re->offset))
		    += base;

		  /* and then generate a run-time relocation directive to add the
		     base address of the code to this field */
		  link.sys.offset = real_map[area].start_offset + re->offset;
		  link.index = 0;	/* index reserved in this case */
		  link.sys.type = TYPE_ADD_BASE;
		  add_link (&link);

		}
	      else
		{		/* symbol relocation */

/*printf("symbol relocation\n");*/
		  /* need to add the value of a symbol to this field */
		  /* NOTE: type 1 and 2 are the same only in that only the SID length differs */

		  /* get info on the symbol */
		  if (get_symbol_info
		      (type2 ? re->info.type2.sid : re->info.type1.sid,
		       &si) == FALSE)
		    {
		      have_valid_output = FALSE;
		      re++;
		      continue;
		    }
		  link.sys.offset = real_map[area].start_offset + re->offset;

		  if (si.is_function == TRUE)
		    {
		      link.index = si.fn_index;
		      link.sys.type = TYPE_ADD_FN_ADDR;
		      add_link (&link);

		    }
		  else if (si.absolute_value)
		    {
		      /* add constant to field */
		      *((int *)
			(code + real_map[area].start_offset + re->offset)) +=
			si.value;

		    }
		  else
		    {
		      /* add address of location within an area */
		      *((int *)
			(code + real_map[area].start_offset + re->offset)) +=
			si.area_base + si.value;
		      link.index = 0;
		      link.sys.type = TYPE_ADD_BASE;
		      add_link (&link);

		    }
		}

	    }
	  else
	    {			/* PC-relative relocation */
/*printf("pc-relative relocation\n");*/

	      if (type2 == TRUE && re->info.type2.a == UNSET)
		{
/*printf("offset to base of area\n");*/
		  /* add offset of the base of the area referenced by the SID from the
		     base of this area */

		  /* find the area specified by the SID, and whether it's in the zero-init area */
		  if (
		      (fa =
		       find_area_search (name_of_area (re->info.type2.sid),
					 real_map, real_map_entries)) == 0)
		    {
		      printf
			("rink: serious error: area '%s' not found in real or zero-init maps\n\n",
			 name_of_area (re->info.type2.sid));
		      have_valid_output = FALSE;
		      re++;
		      continue;
		    }
		  base = fa->start_offset;

		  /* add offset of start base */
		  *((int *) (code + real_map[area].start_offset + re->offset))
		    += base - real_map[area].start_offset;


		}
	      else
		{
/*printf("offset to symbol\n");*/
		  /* generate an offset to the symbol referenced */
		  /* get info on the symbol */
		  if (get_symbol_info
		      (type2 ? re->info.type2.sid : re->info.type1.sid,
		       &si) == FALSE)
		    {
		      have_valid_output = FALSE;
		      re++;
		      continue;
		    }

		  if (si.absolute_value == TRUE)
		    {
		      printf
			("rink: rink cannot generate PC-relative offsets to constants\n\n");
		      have_valid_output = FALSE;
		      re++;
		      continue;
		    }

		  link.sys.offset = real_map[area].start_offset + re->offset;

		  if (si.is_function)
		    {
		      /* add offset of function to word */
		      if (type2 == TRUE)
			{
			  n =
			    0 -
			    ((real_map[area].start_offset) / sizeof (int));
			}
		      else
			{
			  n =
			    0 -
			    ((real_map
			      [area].start_offset + re->offset +
			      PC_OFFSET) / sizeof (int));
			}

		      /* check that we've got a branch instruction here */
		      if (
			  (*
			   ((unsigned
			     int *) (code + real_map[area].start_offset +
				     re->offset)) & 0x0e000000) != 0x0a000000)
			{
			  // oh dear, it's not a branch...
			  printf
			    ("rink: relocation of non-branch instructions not supported (reloc type = %d, AT = %X)\n\n",
			     (type2 ? re->info.type2.ft : re->info.type1.ft),
			     si.at);
			  have_valid_output = FALSE;
			  re++;
			  continue;
			}

		      /* perform the relocation */
		      nn =
			*((unsigned int *)
			  (code + real_map[area].start_offset +
			   re->offset)) & 0xff000000;
		      n =
			((signed
			  int) ((*
				 ((unsigned
				   int *) (code +
					   real_map[area].start_offset +
					   re->offset)) & 0x00ffffff) << 8)) +
			(n << 8);
		      *((unsigned int *)
			(code + real_map[area].start_offset + re->offset)) =
			nn | (((unsigned int) n) >> 8);

		      link.index = si.fn_index;
		      link.sys.type = TYPE_MOD_BRANCH;
		      add_link (&link);
		    }
		  else
		    {
		      /* add offset of symbol within another area */
		      printf
			("rink: serious error: link did not perform internal relocation\n\n");
		      have_valid_output = FALSE;
//            printf("rink: unsupported link (reloc type = %d, AT = %X\n\n", (type2?re->info.type2.ft:re->info.type1.ft), si.at);
		    }
		}

	    }

	  re++;
	}
    }

  ((rink_links_hdr *) links)->n_links = n_links;

  if (vverbose)
    printf ("\n\n\n");

  return TRUE;
}

/* ********************************************************** copy areas into real code block */

BOOL
make_code (void)
{
  int l;
  area_header_entry *ae;

  if ((code = malloc (real_size)) == NULL)
    {
      printf ("rink: no room\n\n");
      return 0;
    }

  /* set up the header */
  strcpy (((rink_code_hdr *) code)->id, code_id);
  ((rink_code_hdr *) code)->main_version = main_version;
  ((rink_code_hdr *) code)->code_version = code_version;
  ((rink_code_hdr *) code)->fn_entries = header_entries;
  ((rink_code_hdr *) code)->code_size = real_size;
  ((rink_code_hdr *) code)->zero_init_size = zero_size;
  ((rink_code_hdr *) code)->named_entries = named_entries;
  
    ((rink_code_hdr *) code)->named_offset =
    sizeof (rink_code_hdr) + (sizeof (rink_code_fn_entry) * header_entries);
  ((rink_code_hdr *) code)->reserved1 = 0;
  ((rink_code_hdr *) code)->reserved2 = 0;

  ae = (area_header_entry *) (header + 1);

  for (l = 0; l < real_map_entries; l++)
    {
      /* copy real areas into the code chunk */
      if (real_map[l].is_zero == FALSE)
	memcpy (code + real_map[l].start_offset,
		find_area_address (real_map[l].area_index), ae[l].area_size);
    }

  return TRUE;
}

/* ************************************************** generate maps for real and zeroed areas */

BOOL
generate_area_map (void)
{
  int l;
  area_header_entry *ae;
  int real_offset;
  int zero_offset;

  /* generate two maps for the real part, and the zero initialised area */
  real_map_entries = 0;
  zero_map_entries = 0;

  /* allocate memory for the two maps */
  if ((real_map = malloc ((header->n_areas + 2) * sizeof (area_map_entry))) ==
      NULL)
    {
      printf ("rink: no room\n\n");
      return FALSE;
    }
  if ((zero_map = malloc ((header->n_areas + 2) * sizeof (area_map_entry))) ==
      NULL)
    {
      printf ("rink: no room\n\n");
      return FALSE;
    }

  ae = (area_header_entry *) (header + 1);
  real_offset = header_total_size;
  zero_offset = 0;

  for (l = 0; l < header->n_areas; l++)
    {
      /* check for unsupported features */
      if (ae[l].info.at_debug == SET)
	{
	  printf
	    ("rink: rink cannot generate code with debug areas (area '%s')\n\n",
	     string_table + ae[l].name_offset);
	  return FALSE;
	}
      if (ae[l].info.at_zero == SET || ae[l].info.at_zero2 == SET)
	{
	  printf ("rink: area '%s' has unknown attribute set\n\n",
		  string_table + ae[l].name_offset);
	  return FALSE;
	}
      if (ae[l].info.al != ALLOWED_AL)
	{
	  printf ("rink: area '%s' has unknown AL value (%d)\n\n",
		  string_table + ae[l].name_offset, ae[l].info.al);
	  return FALSE;
	}

      /* add area to appropriate map */
      if (ae[l].info.at_common == SET || ae[l].info.at_zero_init == UNSET)
	{
	  /* add to real map */
	  real_map[real_map_entries].name = string_table + ae[l].name_offset;
	  real_map[real_map_entries].start_offset = real_offset;
	  real_map[real_map_entries].size = ae[l].area_size;
	  real_map[real_map_entries].area_index = l;
	  real_map[real_map_entries].is_zero = FALSE;
	  real_map_entries++;
	  real_offset += ae[l].area_size;
	  if (verbose)
	    {
	      printf
		("rink: mapping area '%s' (code/data), size %d, relocations %d\n",
		 string_table + ae[l].name_offset, ae[l].area_size,
		 ae[l].relocations);
	    }
	}
      else
	{
	  /* add to zero init map */
	  if (ae[l].info.at_common_ref == UNSET)
	    {
	      zero_map[zero_map_entries].name =
		string_table + ae[l].name_offset;
	      zero_map[zero_map_entries].start_offset = zero_offset;
	      zero_map[zero_map_entries].size = ae[l].area_size;
	      zero_map[zero_map_entries].area_index = l;
	      zero_map[zero_map_entries].is_zero = TRUE;
	      zero_map_entries++;
	      zero_offset += ae[l].area_size;
	      if (verbose)
		{
		  printf
		    ("rink: mapping area '%s' (zero initialised), size %d\n",
		     string_table + ae[l].name_offset, ae[l].area_size);
		}
	      if (ae[l].relocations != 0)
		{
		  printf
		    ("rink: serious error: zero initialised area '%s' has relocation directives\n\n",
		     string_table + ae[l].name_offset);
		  return FALSE;
		}
	    }
	  else
	    {
	      if (verbose)
		{
		  printf ("rink: common area reference '%s' found, size %d\n",
			  string_table + ae[l].name_offset, ae[l].area_size);
		}
	    }
	}
    }

  real_size = real_offset;
  zero_size = zero_offset;

  /* copy zeroed entries to the end of the real map */
  for (l = 0; l < zero_map_entries; l++)
    {
      real_map[real_map_entries] = zero_map[l];
      real_map[real_map_entries].start_offset += real_offset;
      real_map_entries++;
    }

/*for(l = 0; l < real_map_entries; l++) {
printf("\nentry %d\nname %s\nstart_offset %d\nsize %d\narea_index %d\nis_zero %d\n\n", l, real_map[l].name, real_map[l].start_offset, real_map[l].size, real_map[l].area_index, real_map[l].is_zero);
}*/

  if (verbose)
    printf
      ("\nrink: mapping complete: real area size %d, zero init area size %d\n\n",
       real_size, zero_size);

  return TRUE;
}

/* ************************************************************ parse header description file */

#define MAX_TOKEN_LEN 32
BOOL
parse_header_file (char *filename)
{
  FILE *f;
  char line[512];
  int l, ll;
  char token[MAX_TOKEN_LEN];
  char fn[256];

  if ((f = fopen (filename, "r")) == NULL)
    {
      printf ("rink: could not open header description file\n\n");
      return FALSE;
    }

  named_entries = 0;
  named_total_symbol_size = 0;

  while (fgets (line, 512, f) != NULL)
    {
      line[strlen (line) - 1] = '\0';
      if (line[0] == '\0')
	continue;

      ll = 0;
      while (ll < MAX_TOKEN_LEN && line[ll] != ':')
	{
	  token[ll] = line[ll];
	  ll++;
	}
      token[ll++] = '\0';

      if (strcmp (token, "id") == 0)
	{
	  if (strlen (line + ll) != CODE_ID_SIZE)
	    {
	      printf ("segment id '%s' is not exactly %d characters long\n\n",
		      line + ll, CODE_ID_SIZE);
	      return FALSE;
	    }

	  strncpy (code_id, line + ll, CODE_ID_SIZE);
	  code_id[CODE_ID_SIZE] = '\0';
	}
      else if (strcmp (token, "main_version") == 0)
	{
	  sscanf (line + ll, "%d", &main_version);
	}
      else if (strcmp (token, "code_version") == 0)
	{
	  sscanf (line + ll, "%d", &code_version);
	}
      else if (strcmp (token, "entries") == 0)
	{
	  /* parse entries in header */
	  l = 0;
	  while (TRUE)
	    {
	      if (fgets (line, 512, f) == NULL)
		{
		  printf
		    ("rink: unexpected end of file in header description\n\n");
		  return FALSE;
		}
	      if (strcmp (line, "//\n") == 0)
		{
		  header_entries = l;
		  break;
		}
	      if (line[0] != '0')
		{
		  sscanf (line, "%s %d\n", fn,
			  &header_entry_symbols[l].arguments);
		  if (
		      (header_entry_symbols[l].name =
		       malloc (strlen (fn) + 4)) == NULL)
		    {
		      printf ("rink: no room\n\n");
		      return FALSE;
		    }
		  strcpy (header_entry_symbols[l].name, fn);
		}
	      else
		{
		  header_entry_symbols[l].name = 0;
		  header_entry_symbols[l].arguments = 0;
		}
	      l++;
	      if (l >= MAX_HEADER_ENTRIES)
		{
		  printf ("rink: too many entries in header\n\n");
		  return FALSE;
		}
	    }
	}
      else if (strcmp (token, "named_entries") == 0)
	{
	  l = 0;
	  named_total_symbol_size = 0;
	  while (TRUE)
	    {
	      if (fgets (line, 512, f) == NULL)
		{
		  printf
		    ("rink: unexpected end of file in header description\n\n");
		  return FALSE;
		}
	      if (strcmp (line, "//\n") == 0)
		{
		  named_entries = l;
		  break;
		}
	      if (line[0] != '0')
		{
		  sscanf (line, "%s\n", fn);
		  if ((named_entry_symbols[l].name = malloc (strlen (fn) + 4))
		      == NULL)
		    {
		      printf ("rink: no room\n\n");
		      return FALSE;
		    }
		  strcpy (named_entry_symbols[l].name, fn);
		  named_total_symbol_size += strlen (fn) + 1;
		}
	      else
		{
		  printf
		    ("rink: invalid line in header description file (named symbols)\n\n");
		  return FALSE;
		}
	      l++;
	      if (l >= MAX_NAMED_ENTRIES)
		{
		  printf ("rink: too many entries in header\n\n");
		  return FALSE;
		}
	    }
	}
      else
	{
	  printf ("rink: unknown token '%s' in header description file\n\n",
		  token);
	  return FALSE;
	}
    }

  fclose (f);

  return TRUE;
}

/* *********************************************************************** parse command line */

BOOL
parse_commandline (int argc, char *argv[])
{
  int l, ll;
  char aof_parts[1024];
  int aof_i = 0, aof_files = 0;
  char link_cmd[1024];

  for (l = 0; l <= MAX_PARAM; l++)
    params[l][0] = '\0';

  if (argc < 2)
    {
      printf ("rink: bad command line arguments\n\n");
      return FALSE;
    }

  for (l = 1; l < argc; l++)
    {
      if (argv[l][0] == '-')
	{
	  if (((l + 1) >= argc || argv[l + 1][0] == '-') && argv[l][1] != 'v')
	    {
	      printf
		("rink: bad command line arguments - no value following -%c switch\n\n",
		 argv[l][1]);
	      return FALSE;
	    }
	  switch (argv[l][1])
	    {
	    case 'o':
	      strcpy (code_name, argv[++l]);
	      break;

	    case 'l':
	      strcpy (links_name, argv[++l]);
	      break;

	    case 'h':
	      strcpy (header_name, argv[++l]);
	      break;

	    case 'm':
	      strcpy (map_name, argv[++l]);
	      break;

	    case 'w':
	      sscanf (argv[++l], "%d", &link_bug_patch);
	      break;

	    case 'a':
	      /* more than one aof file expected... */
	      l++;
	      while (argv[l][0] != '-' && l < argc)
		{
		  aof_i +=
		    sprintf (aof_parts + aof_i, "%s%s",
			     (aof_files > 0) ? " " : "", argv[l++]);
		  aof_files++;
		}
	      l--;
	      break;

	    case 'v':
	      verbose = TRUE;
	      if (argv[l][2] == 'v')
		vverbose = TRUE;
	      break;

	    default:
	      printf ("rink: bad command line switch -%c\n", argv[l][1]);
	      return FALSE;
	      break;
	    }
	}
      else
	{
	  for (ll = 0; ll <= MAX_PARAM; ll++)
	    {
	      if (params[ll][0] == '\0')
		{
		  strcpy (params[ll], argv[l]);
		  break;
		}
	    }
	  if (ll > MAX_PARAM)
	    {
	      aof_i +=
		sprintf (aof_parts + aof_i, "%s%s",
			 (aof_files > 0) ? " " : "", argv[l]);
	      aof_files++;
	    }
	}
    }

  for (l = 0; l <= MAX_PARAM; l++)
    {
      if (params[l][0] == '\0')
	{
	  printf ("rink: bad command line arguments, %s filename missing\n\n",
		  param_trans[l]);
	  return FALSE;
	}
    }

  if (aof_i == 0 || aof_files == 0)
    {
      printf ("rink: bad command line arguments, aof files missing\n\n");
      return FALSE;
    }

  /* invoke link to combine all the aof files and fix up internal links */
  strcpy (aof_name, SCRAP_FILE);
  sprintf (link_cmd, "link -o %s -aof %s", aof_name, aof_parts);
  if (system (link_cmd) != 0)
    {
      printf ("\nrink: link failed\n\n");
      return FALSE;
    }

  return TRUE;
}

/* ************************************************************************************* main */

int
main (int argc, char *argv[])
{
  chunk_entry *ce;
  int l, n;
  struct stat sb;
  FILE *handle;

  printf ("rink v0.10  [" __DATE__ "]\n(c) Ben Summers 1995\n\n");

  if (parse_commandline (argc, argv) != TRUE)
    return 1;

  if (parse_header_file (header_name) != TRUE)
    return 1;

  if (map_load (map_name) != TRUE)
    return 1;

  header_total_size = (sizeof (rink_code_hdr)
		       + (sizeof (rink_code_fn_entry) * header_entries)
		       + named_total_symbol_size
		       + (sizeof (rink_code_named_fn_entry) * named_entries)
		       + sizeof (int));
  header_total_size &= ~3;	/* word align size */

  /* load source aof file */
  if (stat (aof_name, &sb))
    {
      printf ("rink: error finding source aof file\n\n");
      return 1;
    }
  aof_size = sb.st_size;
  if ((aof = malloc (aof_size + 16)) == NULL)
    {
      printf ("rink: no room\n\n");
      return 1;
    }
  handle = fopen (aof_name, "r");
  if (handle == NULL)
    {
      printf ("rink: error loading source aof file\n\n");
      return 1;
    }

  fread (aof, 1, aof_size, handle);
  fclose (handle);

  /* set up pointers to the areas within the file */
  if (((chunk_file_hdr *) aof)->chunkfield != 0xC3CBC6C5)
    {
      printf ("rink: source 'aof' file is not an aof file\n\n");
      return 1;
    }
  ce = (chunk_entry *) (((chunk_file_hdr *) aof) + 1);
  for (l = 0; l < ((chunk_file_hdr *) aof)->numchunks; l++)
    {
      n = 0;
      while (aof_chunkids[n][0] != '\0')
	{
	  if (strncmp (ce[l].chunkid, aof_chunkids[n], CHUNKID_SIZE) == 0)
	    {
	      *aof_chunks[n] = aof + ce[l].offset;
	    }
	  n++;
	}
    }
  /* check important chunks are present */
  for (l = 0; l <= MAX_OBJ_CHUNK; l++)
    {
      if (*aof_chunks[l] == 0 && l != OBJ_IDFN)
	{
	  printf ("rink: chunk '%s' missing in source aof file\n\n",
		  aof_chunkids[l]);
	  return 1;
	}
    }

  if (verbose && identification != 0)
    {
      printf ("rink: source aof file created by '%s'\n\n", identification);
    }

  header = (aof_header *) header_cp;

  if (header->version == 311)
    areas += link_bug_patch;

  /* check the aof file header */
  if (header->object_file_type != 0xC5E2D080)
    {
      printf
	("rink: source aof file is not in relocatable object format\n\n");
      return 1;
    }

  if (header->version != 311 && header->version != 200
      && header->version != 150)
    {
      printf
	("rink: rink can only handle version 1.xx or 2.xx aof files (v%d)\n\n",
	 header->version);
      return 1;
    }

  if (generate_area_map () != TRUE)
    return 1;

  if (make_code () != TRUE)
    return 1;

  if (make_relocations () != TRUE)
    return 1;

  if (verbose)
    printf ("rink: completed relocations\n\n");

  if (have_valid_output == FALSE)
    {
      printf ("\nrink: no output: errors in creation\n\n");
      return 1;
    }

  if (make_header () != TRUE)
    return 1;

  if (save_file (code_name, code, real_size, 0xffd) == FALSE)
    {
      printf ("rink: could not save code file\n\n");
      return 1;
    }

  if (save_file (links_name, links, links_end, 0xffd) == FALSE)
    {
      printf ("rink: could not save code file\n\n");
      return 1;
    }

  delete_file (SCRAP_FILE);

  return 0;
}
