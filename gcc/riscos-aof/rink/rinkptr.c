/* **************** ptr map for rink ******************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOOL int
#define FALSE 0
#define TRUE 1

/* **************************************************** */
/* **************************************************** */

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

#define RELOC_TYPE_TEST_VAL 8
#define RELOC_FT_IS_WORD    2

typedef struct
{
  int offset;
  union
  {
    struct
    {
      unsigned int z:28;
      unsigned int n:4;
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
      unsigned int val:4;
    }
    type2;
  }
  info;
}
relocation;

typedef struct
{
  int name_offset;
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
  info;
  int value;
  int area_name_offset;
}
symbol_entry;

/* **************************************************** */

#define CHUNKS_IN_FILE 5
#define USED_CHUNKS    5

#define BRANCH 0xEA000000

#define N_SUGARS 3

/* **************************************************** */

static char *aof_symbols;
static int aof_sym_l, aof_sym_i;
static int n_aof_symbols;

static char *area_chunk;
static int area_chunk_size;

static char *symbol_table;
static int symbol_table_size;

static char *string_table;
static int string_table_size;

static char *header;
static int header_size;

static char id_chunk[] = { "rinkptr (c) Ben Summers" };
static int id_chunk_size;

static char *chunk_file;
static unsigned int chunk_file_size;

char alloc_fn[256] = "malloc";
char free_fn[256] = "free";
char trans_fn[256] = "_rink_default_trans";

typedef struct
{
  char id[12];
  char *buffer;
}
sugar_trans_def;

sugar_trans_def sugar_trans[] =
  { {"alloc", alloc_fn}, {"free", free_fn}, {"errtrans", trans_fn} };

/* **************************************************** */

#define SYMBOL_BLOCK (16*1024)

#define EXTRA_BLOCK  1024

/* **************************************************** */

char map_name[256], aof_name[256];

#define MAX_PARAM 1
char *params[] = { map_name, aof_name };
const char *param_trans[] = { "pointer block map", "output AOF file" };

static char *symbols;
static int sym_len;
static int sym_i;
static int sym_n;

static BOOL verbose = FALSE;
static BOOL vverbose = FALSE;

/* **************************************************************************** misc functions */

static void
add_symbol (char *symbol)
{
  int l;

  l = strlen (symbol);

  if ((sym_i + l + 4) > sym_len)
    {				/* extend symbol block */
      if ((symbols = realloc (symbols, sym_len += SYMBOL_BLOCK)) == NULL)
	{
	  printf ("no room\n\n");
	  exit (1);
	}
    }
  strcpy (symbols + sym_i, symbol);
  sym_i += l + 1;

  sym_n++;
}

/* *************************************************************************** create aof file */

static BOOL
create_aof_file (void)
{
  FILE *output;
  int l;
  int len;
  relocation *re;
  int i;
  int var_name_i, area_name_i;
  symbol_entry *se;
  aof_header *ah;
  area_header_entry *he;
  chunk_file_hdr *ch;
  chunk_entry *ce;
  int si;

  if ((output = fopen (aof_name, "wb")) == NULL)
    {
      printf ("rinkptr: could not open output aof file for output\n\n");
      return 1;
    }

  /* read map into memory */
  if ((aof_symbols = malloc (aof_sym_l = SYMBOL_BLOCK)) == NULL)
    {
      printf ("rinkptr: no room\n\n");
      return 1;
    }
  aof_sym_i = sizeof (int);

  n_aof_symbols = sym_n;
  si = 0;

  for (l = 0; l < n_aof_symbols; l++)
    {
      len = strlen (symbols + si);

      if ((aof_sym_i + len + 4) > aof_sym_l)
	{
	  if ((aof_symbols = realloc (aof_symbols, aof_sym_l += SYMBOL_BLOCK))
	      == NULL)
	    {
	      printf ("rinkptr: no room\n\n");
	      return 1;
	    }
	}
      strcpy (aof_symbols + aof_sym_i, symbols + si);
      aof_sym_i += len + 1;

      si += len + 1;
    }

  if ((aof_sym_i + SYMBOL_BLOCK) > aof_sym_l)
    {
      if ((aof_symbols = realloc (aof_symbols, aof_sym_l += SYMBOL_BLOCK)) ==
	  NULL)
	{
	  printf ("rinkptr: no room\n\n");
	  return 1;
	}
    }
  /* add additional entries to end of string table */
  /* name of functions to sugar to follow */
  /* when adding, remember to increase N_SUGARS */
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "%s", alloc_fn) + 1;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "%s", free_fn) + 1;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "%s", trans_fn) + 1;

  /* names of sugar function follow in the same order as above */
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "_rink_alloc") + 1;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "_rink_free") + 1;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "_rink_errtrans") + 1;

  /* misc symbols follow */
  var_name_i = aof_sym_i;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "_rink_pointerblock") + 1;
  area_name_i = aof_sym_i;
  aof_sym_i += sprintf (aof_symbols + aof_sym_i, "rink$ptrblk") + 1;

  aof_symbols[aof_sym_i] = '\0';

  string_table = aof_symbols;
  string_table_size = (aof_sym_i + 4) & (~3);
  *((int *)(void *) aof_symbols) = string_table_size;


  printf ("rinkptr: size of _rink_pointerblock: %d\n\n",
	  n_aof_symbols * sizeof (int *));

  /* construct a aof file to create the pointer block */

  /* construct the area chunk */
  area_chunk_size =
    ((n_aof_symbols + N_SUGARS) * sizeof (int *)) +
    ((n_aof_symbols + N_SUGARS) * sizeof (relocation));
  if ((area_chunk = malloc (area_chunk_size)) == NULL)
    {
      printf ("rinkptr: no room\n\n");
      return 1;
    }
  memset (area_chunk, '\0', (n_aof_symbols * sizeof (int *)));
  for (l = n_aof_symbols; l < (n_aof_symbols + N_SUGARS); l++)
    ((int *)(void *) area_chunk)[l] = BRANCH;
  re =
    (relocation *)(void *) (area_chunk +
		    ((n_aof_symbols + N_SUGARS) * sizeof (int *)));
  for (l = 0; l < (n_aof_symbols + N_SUGARS); l++)
    {
      re[l].offset = l * sizeof (int *);
      if (l < n_aof_symbols)
	{
	  re[l].info.type2.sid = l;
	  re[l].info.type2.ft = 2;	/* word */
	  re[l].info.type2.r = 0;	/* additive relocation */
	  re[l].info.type2.a = 1;	/* aof_symbol relocation */
	  re[l].info.type2.val = RELOC_TYPE_TEST_VAL;	/* flag as type 2 */
	}
      else
	{
	  re[l].info.type1.sid = l;
	  re[l].info.type1.ft = 2;
	  re[l].info.type1.r = 1;	/* PC-relative relocation */
	  re[l].info.type1.a = 1;
	  re[l].info.type1.zero = 0;
	}
    }

  /* construct symbol table */
  symbol_table_size =
    ((n_aof_symbols + 1 + (N_SUGARS * 2)) * sizeof (symbol_entry));
  if ((symbol_table = malloc (symbol_table_size)) == NULL)
    {
      printf ("rinkptr: no room\n\n");
      return 1;
    }
  i = sizeof (int);
  se = (symbol_entry *)(void *) symbol_table;
  for (l = 0; l < (n_aof_symbols + (N_SUGARS * 2)); l++)
    {
      se->name_offset = i;
      if (l < (n_aof_symbols + N_SUGARS))
	{
	  se->info.at_defined = UNSET;
	  se->info.at_global = SET;
	  se->info.at_absolute = UNSET;
	  se->info.at_case = UNSET;
	  se->info.at_weak = UNSET;
	  se->info.at_strong = UNSET;
	  se->info.at_common = UNSET;
	  se->info.zero = 0;
	  se->value = 0;
	  se->area_name_offset = 0;
	}
      else
	{
	  se->info.at_defined = SET;
	  se->info.at_global = SET;
	  se->info.at_absolute = UNSET;
	  se->info.at_case = UNSET;
	  se->info.at_weak = UNSET;
	  se->info.at_strong = UNSET;
	  se->info.at_common = UNSET;
	  se->info.zero = 0;
	  se->value = (l - N_SUGARS) * sizeof (int);
	  se->area_name_offset = area_name_i;
	}

      se++;
      i += strlen (string_table + i) + 1;
    }
  /* make definition of the pointer block */
  se->name_offset = var_name_i;
  se->info.at_defined = SET;
  se->info.at_global = SET;
  se->info.at_absolute = UNSET;
  se->info.at_case = UNSET;
  se->info.at_weak = UNSET;
  se->info.at_strong = UNSET;
  se->info.at_common = UNSET;
  se->info.zero = 0;
  se->value = 0;		/* block starts at offset 0 of area */
  se->area_name_offset = area_name_i;


  /* make AOF file header */
  header_size = sizeof (aof_header) + sizeof (area_header_entry);
  if ((header = malloc (header_size)) == NULL)
    {
      printf ("rinkptr: no room\n\n");
      return 1;
    }
  ah = (aof_header *)(void *) header;
  he = (area_header_entry *) (ah + 1);
  ah->object_file_type = 0xC5E2D080;
  ah->version = 150;
  ah->n_areas = 1;
  ah->n_symbols = n_aof_symbols + 1 + (N_SUGARS * 2);
  ah->entry_area = 0;
  ah->entry_area_offset = 0;
  he->name_offset = area_name_i;
  he->info.al = 2;
  he->info.at_zero = 0;
  he->info.at_code = UNSET;
  he->info.at_common = UNSET;
  he->info.at_common_ref = UNSET;
  he->info.at_zero_init = UNSET;
  he->info.at_read_only = UNSET;
  he->info.at_zero2 = 0;
  he->info.at_debug = UNSET;
  he->info.zero = 0;
  he->area_size = ((n_aof_symbols + N_SUGARS) * sizeof (int *));
  he->relocations = n_aof_symbols + N_SUGARS;
  he->reserved = 0;


  /* make identification chunk */
  id_chunk_size = (sizeof (id_chunk) + 4) & (~3);


  /* put all the bits together to make the object file */
  chunk_file_size = area_chunk_size + symbol_table_size + id_chunk_size +
    string_table_size + header_size +
    sizeof (chunk_file_hdr) + (sizeof (chunk_entry) * CHUNKS_IN_FILE);

  if ((chunk_file = malloc (chunk_file_size)) == NULL)
    {
      printf ("rinkptr: no room\n\n");
      return 1;
    }
  i = sizeof (chunk_file_hdr) + (sizeof (chunk_entry) * CHUNKS_IN_FILE);
  ch = (chunk_file_hdr *)(void *) chunk_file;
  ce = (chunk_entry *) (ch + 1);

  ch->chunkfield = 0xC3CBC6C5;
  ch->maxchunks = CHUNKS_IN_FILE;
  ch->numchunks = USED_CHUNKS;

  /* stick header in file */
  strcpy (ce->chunkid, "OBJ_HEAD");
  ce->offset = i;
  ce->size = header_size;
  ce++;
  memcpy (chunk_file + i, header, header_size);
  i += header_size;

  /* stick areas in file */
  strcpy (ce->chunkid, "OBJ_AREA");
  ce->offset = i;
  ce->size = area_chunk_size;
  ce++;
  memcpy (chunk_file + i, area_chunk, area_chunk_size);
  i += area_chunk_size;

  /* stick symbol table in file */
  strcpy (ce->chunkid, "OBJ_SYMT");
  ce->offset = i;
  ce->size = symbol_table_size;
  ce++;
  memcpy (chunk_file + i, symbol_table, symbol_table_size);
  i += symbol_table_size;

  /* stick string table in file */
  strcpy (ce->chunkid, "OBJ_STRT");
  ce->offset = i;
  ce->size = string_table_size;
  ce++;
  memcpy (chunk_file + i, string_table, string_table_size);
  i += string_table_size;

  /* stick identification in file */
  strcpy (ce->chunkid, "OBJ_IDFN");
  ce->offset = i;
  ce->size = id_chunk_size;
  ce++;
  memcpy (chunk_file + i, id_chunk, id_chunk_size);
  i += id_chunk_size;


  if (fwrite (chunk_file, sizeof (char), chunk_file_size, output) !=
      chunk_file_size)
    {
      printf ("rinkptr: save aof file failed\n\n");
      return 1;
    }

  fclose (output);

  return TRUE;
}

/* ************************************************************************ parse command line */

static BOOL
parse_commandline (int argc, char *argv[])
{
  int l, ll;

  for (l = 0; l <= MAX_PARAM; l++)
    params[l][0] = '\0';

  if (argc < 2)
    {
      printf ("rinkptr: bad command line arguments\n\n");
      return FALSE;
    }

  for (l = 1; l < argc; l++)
    {
      if (argv[l][0] == '-')
	{
	  if (((l + 1) >= argc || argv[l + 1][0] == '-') && argv[l][1] != 'v')
	    {
	      printf
		("rinkptr: bad command line arguments - no value following -%c switch\n\n",
		 argv[l][1]);
	      return FALSE;
	    }
	  switch (argv[l][1])
	    {
	    case 'm':
	      strcpy (map_name, argv[++l]);
	      break;

	    case 'a':
	      strcpy (aof_name, argv[++l]);
	      break;

	    case 'v':
	      verbose = TRUE;
	      if (argv[l][2] == 'v')
		vverbose = TRUE;
	      break;

	    case 'A':
	      strcpy (alloc_fn, argv[++l]);
	      break;

	    case 'F':
	      strcpy (free_fn, argv[++l]);
	      break;

	    case 'T':
	      strcpy (trans_fn, argv[++l]);
	      break;

	    default:
	      printf ("rinkptr: bad command line switch -%c\n", argv[l][1]);
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
	}
    }

  for (l = 0; l <= MAX_PARAM; l++)
    {
      if (params[l][0] == '\0')
	{
	  printf
	    ("rinkptr: bad command line arguments, %s filename missing\n\n",
	     param_trans[l]);
	  return FALSE;
	}
    }

  return TRUE;
}

/* ************************************************************************************** main */

int
main (int argc, char *argv[])
{
  FILE *map_file;
  int l, Line;
  char line[256];

  printf ("rinkptr v0.10  [" __DATE__ "]\n(c) Ben Summers 1995\n\n");

  if (parse_commandline (argc, argv) != TRUE)
    return 1;

  if ((map_file = fopen (map_name, "r")) == NULL)
    {
      printf ("could not pointer block map file\n\n");
      return 1;
    }

  /* claim block for symbols */
  if ((symbols = malloc (SYMBOL_BLOCK)) == 0)
    {
      printf ("no room\n\n");
      return 1;
    }
  sym_len = SYMBOL_BLOCK;
  sym_n = sym_i = 0;

  // read in the symbols from the map file
  Line = 0;
  while (fgets (line, 256, map_file) != 0)
    {
      Line++;

      // check it's not a comment or a blank line
      if (line[0] == '#' || line[0] < ' ')
	continue;

      // terminate the line properly
      l = 0;
      while (line[l] > ' ')
	{
	  l++;
	}
      // check there isn't a space in there
      if (line[l] == ' ')
	{
	  printf
	    ("rinkptr: invalid line in pointer block map (contains space) at line %d\n\n",
	     Line);
	  return 1;
	}
      // terminate it properly
      line[l] = '\0';

      // add it as a symbol...
      add_symbol (line);
    }

  // create the aof file
  if (create_aof_file () != TRUE)
    return 1;

  return 0;
}
