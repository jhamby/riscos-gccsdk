/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "gfile.h"
#include "error.h"
#include "mem.h"
#include "options.h"
#include "str.h"
#include "readfile.h"
#include "system.h"
#include "filename.h"
#include "blank.h"

#define UNUSED(x) { x=x; }

/*****************************************************************************
 * This set of functions are Fgetc/feof replacements with Fungetc.
 *****************************************************************************/

static char unget_c;
static int unget = 0;

static char Fgetc(FILE *file) {

  if (unget) {
    unget = 0;
    return unget_c;
  } else {
    return fgetc(file);
  }
}

static int Feof(FILE *file) {

  if (unget) {
    return 0;
  } else {
    return feof(file);
  }
}

static void Fungetc(FILE *file, char c) {

  unget = 1;
  unget_c = c;
}

/*****************************************************************************
 * This set of functions are a grotty line buffer implementation that
 * dynamically grows as required.
 *****************************************************************************/

static char *linebuf = NULL;
static int   bufsize = 128;
static int   linepos = 0;

static void bump(void) {

  bufsize *= 2;
  linebuf = Realloc(linebuf, bufsize);
}

static void lineinit(void) {

  linepos = 0;
}

static void insert(char c) {

  linebuf[linepos] = c;
  linepos++;
  if (linepos >= bufsize)
    bump();
}

static void uninsert(void) {

  linepos--;
}

/****************************************************************************
 * This nightmare function is a line parser for CMHG files. Its horrible, but
 * blame Acorn for the wierd rules CMHG set for when reading should continue
 * onto the next line, when commas are required etc...
 ***************************************************************************/

/* FIXME: Blatant assumption that preprocessor lines are < 1024 chars */
static char prelinebuf[1024];
/* FIXME: ... and that filenames are < 1024 chars */
static char filename[1024];

static void preprocess(FILE *file) {

  char c;
  int i;
  int line;

  if (Feof(file))
    ErrorFatal("Corrupt preprocessor line found!");
  i = 0;
  do {
    c = Fgetc(file);
    prelinebuf[i++] = c;
  } while ((!Feof(file)) && (c != '\n') && (i < 1023));
  if (c=='\n')
    i--;
  prelinebuf[i] = 0;
  if (c != '\n') {
    while ((!Feof(file)) && (c != '\n')) {
      c = Fgetc(file);
    }
  }

  /* Now we have the line in prelinebuf */
  /* Spot the preprocessor directives */
  if (sscanf(prelinebuf," %i \"%[^\"]\"",&line,filename)==2) {
    /* It appears that GCC always generates its filenames in unix-format,
       regardless of what you give it on the command line; to make the
       throwback work properly, we must convert this to RISC OS style
       if so given */
    const char *nativefilename;
#ifdef __riscos
    nativefilename = filename_unixtoriscos(filename, EXTLIST_NORCROFT_CMHG);
#else
    /* On non-RISC OS systems, you won't care about the RISC OS filename;
       it will only obscure the problems. */
    nativefilename = filename;
#endif
    opt.atline = line-1;
    Free(opt.infile);
    opt.infile = strdup_strip(nativefilename);
  }
  else if (sscanf(prelinebuf,"line %i \"%[^\"]\"",&line,filename)==2) {
    /* Norcroft always generates the filenames in native-format which is
       correct for throwback. */
    opt.atline = line-1;
    Free(opt.infile);
    opt.infile = strdup_strip(filename);
  } else {
    opt.atline++;
#if 0
    /* If this came from a pre-processed file, we should probably
       report the error in the processed file - except the file
       will be deleted when CMunge exits. So if we are pre-processing
       we might want to report the error as being in some special
       'processed-file' so that there is no confusion that this is
       in an input file. However, the line number will then be wrong,
       too.
       I have not had enough time to think on this - it's not likely to
       happen with the tools we use so it's probably not important.
       Because of this, I've left it #if 0'd out. Someone should probably
       think about it at some point. If only to satisfy the unlikely
       failure case. */
    if (opt.pfile)
      opt.infile = "(processed-file)";
#endif
    ErrorFatal("Unknown preprocessor command encountered: #%s", prelinebuf);
  }

  /* The pre-processor tool might be performing filename translation
     and thus generating its line directives with a different name to the
     input file. Because of this, we remember the first line declaration
     that we see. This will henceforth be considered the root input file
     and in non-extended pre-processor mode only lines from it will be
     processed. */
  if (opt.rootinfile == NULL)
  {
    opt.rootinfile = strdup_strip(opt.infile);
  }
}

static char *getline(FILE *file) {

  char c;
  int comma, discarding, whitespace;
  int quoting, level, escape, concat;
  int done, linestart;

  /* The 'retry' label is used to process lines which are being ignored
     because they are inside the pre-processor's lower level includes. */
retry:
  lineinit();
  /* First, skip whitespace */
  c = ' ';
  discarding = 0;
  whitespace = 0;
  quoting = 0;
  level = 0;
  done = 0;
  comma = 0;
  escape = 0;
  concat = 0;
  linestart = 1;

  while (!Feof(file) && (!done)) {
    while (!Feof(file) && (isspace(c))) {
      if (c=='\n') {
        opt.atline++;
        linestart = 1;
      }
      c = Fgetc(file);
    }
    if (!Feof(file)) {
      while (!Feof(file)) {
        if (linestart && concat && (!isspace(c)) && (c != '\"') && (level == 0)) {
          Fungetc(file,c);
          done=1;
          break;
        }
        /* Check for preprocess line */
        if ((c == '#') && linestart) {
          preprocess(file);
          linestart = 1;
        } else {
          if (!isspace(c))
            linestart = 0;
          if (c == '\n')
            linestart = 1;
          /* Not a preprocess line */
          if (c == '\\') {
            if (escape) {
              insert('\\');
              escape = 0;
            } else
              escape = 1;
            if (!discarding) {
              comma = 0;
              concat = 0;
            }
          } else if (c == '\"') {
            if (!discarding) {
              if (!escape) {
                quoting = !quoting;
                if (concat) {
                  concat = 0;
                  uninsert();
                } else {
                  if (!quoting)
                    concat = 1;
                  insert('"');
                }
                comma = 0;
              } else {
                insert('\\');
                insert('"');
                concat = 0;
                comma = 0;
              }
            }
          } else if (c == 'n') {
            if (!discarding) {
              if (escape) {
                insert('\n');
                comma = 0;
                concat = 0;
              } else {
                insert('n');
                comma = 0;
                concat = 0;
              }
            }
          } else if (c == 't') {
            if (!discarding) {
              if (escape) {
                insert('\t');
                comma = 0;
                concat = 0;
              } else {
                insert('t');
                comma = 0;
                concat = 0;
              }
          }
          } else if (c == 'v') {
            if (!discarding) {
              if (escape) {
                insert('\v');
                comma = 0;
                concat = 0;
              } else {
                insert('v');
                comma = 0;
                concat = 0;
              }
            }
          } else if (c == 'r') {
            if (!discarding) {
              if (escape) {
                insert('\r');
                comma = 0;
                concat = 0;
              } else {
                insert('r');
                comma = 0;
                concat = 0;
              }
            }
          } else if (quoting) {
            if (c == '\n')
              ErrorFatal("Newline found in unclosed string");
            insert(c);
            comma = 0;
            concat = 0;
          } else if (c == '(') {
            if (!discarding) {
              insert(c);
              comma = 0;
              concat = 0;
              level++;
            }
          } else if (c == ')') {
            if (!discarding) {
              insert(c);
              comma = 0;
              concat = 0;
              level--;
              if (level < 0)
                ErrorFatal("Too many closing parentheses (%i)!",level);
            }
          } else if (c == ';') {
            /* Discard to end of line */
            discarding = 1;
          } else if (c == '\n') {
            opt.atline++;
            discarding = 0;
            if ((comma == 0) && (level == 0) && (concat==0))
              done = 1;
            break;
          } else if (c == ',') {
            if (!discarding) {
              insert(',');
              comma = 1;
              concat = 0;
            }
          } else if (isspace(c)) {
            if (!whitespace && !discarding && !concat) {
              insert(' ');
              if (!quoting)
                whitespace = 1;
            }
          } else {
            if (!discarding) {
              insert(c);
              whitespace = 0;
              comma = 0;
              concat = 0;
            }
          }
        }
        if (c != '\\')
          escape = 0;
        c = Fgetc(file);
      }
    }
  }
  insert(0);

  /* If we not processing extended pre-processor operations, we need to act slightly
     differently. CMHG will ignore anything which is not in the top level file
     when it processes its input. This means that any text in a #include file
     will be ignored completely in non-extended mode. In extended mode we process these
     which means that we can therefore have fragments of the header files in
     separate included files. */
  if (!opt.pextended)
  {
    if (!Feof(file) && opt.rootinfile && strcmp(opt.rootinfile, opt.infile)!=0)
    {
      /* We are NOT inside the root input file that we were given. This means we're
         in an included file. As such we need to just ignore the line. This is what
         CMHG does and we're just copying them. */
#if 0
      /* JRF: I'm not sure that warning about these lines is really necessary so I've
              commented it out for now. */
      char *str = linebuf;
      while (*str == ' ' || *str == '\t')
        str++;
      if (*str != '\0')
      {
        Warning("Ignoring line from included file '%s'", linebuf);
      }
#endif
      /* For simplicity, we just return to the top of this routine to fetch the
         next line. */
      goto retry;
    }
  }

  return linebuf;
}

/*****************************************************************************
 * Starts to get cleaner from here :-)
 * parse_line calls getline and then deals with the returned string, setting
 * up the 'options' structure as required.
 *****************************************************************************/

static void read_services(const char *s, FILE *file) {

  int_list *l;

  UNUSED(file);

  if (opt.service)
    ErrorFatal("Only supply one service-call-handler!");
  s = strduptok(s, &opt.service);
  if (*s == ',')
    while (isspace(*s))
      s++;
  if (*s == 0) {
    Warning("No list of services given for handler; may degrade system performance!");
    return;
  }
  l = &opt.services;
  while (*s != 0) {
    *l = Malloc(sizeof(**l));
    s = strint(s, &(*l)->val);
    while (isspace(*s))
      s++;
    /* Skip a , if there is one */
    if (*s == ',')
      s++;
    while (isspace(*s))
      s++;
    l = &((*l)->next);
  }
  *l = NULL;
}

static void read_events(const char *s, FILE *file) {

  int_list *l;
  handler_list *lastp = &opt.events;
  handler_list event;

  if (*lastp && opt.cmhg)
  {
    /* If they want CMHG-style behaviour, we kill off the previous entries and just
     * use the last one
     */
    Warning("multiple event-handlers specified (ignoring all but last for CMHG)");
    *lastp = NULL;
  }

  /* Find the end of the list, so that we append to whatever is already there, rather
     than replacing it */
  while (*lastp)
    lastp=&(*lastp)->next;

  UNUSED(file);

  event = Malloc(sizeof(*event));
  s = strfindpair(s, &event->name, &event->handler);
  event->next = NULL;
  if (*s == '(')
    ErrorFatal("Event handlers cannot be passed parameters");

  if (*s == ',')
    while (isspace(*s))
      s++;
  if (*s == 0) {
    Warning("Not specifying event numbers impedes system performance!");
    return;
  }
  l = (int_list *)&event->extradata;
  while (*s != 0) {
    *l = Malloc(sizeof(**l));
    s = strint(s, &(*l)->val);
    /* Skip a , if there is one */
    if (*s == ',')
      while (isspace(*s))
        s++;
    l = &((*l)->next);
  }
  *l = NULL;

  event->next = *lastp;
  *lastp=event;
}

static void read_swis(const char *s, FILE *file) {

  swi_list *l;

  UNUSED(file);

  if (opt.swi_names)
    ErrorFatal("Only supply one swi-decoding-table!");

  opt.swi_names = Malloc(sizeof(*opt.swi_names));
  opt.swi_names->handler = NULL; /* Never used but for safety */
  s = strduptok(s, &opt.swi_names->name);
  l = &opt.swi_names->next;
  s = strcomma(s);
  /* This is probably actually legal! */
  //if (*s == 0)
  //  ErrorFatal("SWI prefix supplied, but no SWI names!");
  while (*s != 0) {
    *l = Malloc(sizeof(**l));
    s = strfindpair(s, &(*l)->name, &(*l)->handler);
    if ((*l)->handler)
    {
      opt.swi_codesupplied=1;
      if (opt.cmhg)
        CMHGWarning("handler supplied for swi %s",(*l)->name);
    }
    if (*s == '(')
      ErrorFatal("SWI handlers cannot be passed parameters");

    l = &(*l)->next;
    s = strcomma(s);
  }
  *l = NULL;
}

static void read_errors(const char *s, FILE *file) {

  error_list *l;

  l = &opt.errors;
  while (*s != 0) {
    *l = Malloc(sizeof(**l));
    s = strduptok(s, &(*l)->name);
    s = strskip(s, '(');
    (*l)->message        = NULL;
    (*l)->number         = -1;
    while (*s != ')') {
      if (*s=='\"') {
        if ((*l)->message != NULL)
          ErrorFatal("error message for %s supplied twice!",(*l)->name);
        s = strstring(s, &(*l)->message);
      } else if (*s == 0) {
        while ((*s == 0) && !Feof(file)) {
          s = getline(file);
          while (isspace(*s) && (!Feof(file)))
            s++;
        }
      } else {
        /* It /must/ be a number, otherwise we don't know it */
        if ((*l)->number!=-1)
          ErrorFatal("error number for %s supplied twice!",(*l)->name);

        s = strint(s, (unsigned int *)&(*l)->number);
        opt.errors_special=1;
        /* ErrorFatal("Unknown argument in error-identifiers: %s", s); */
        /* break; */
      }
      s = strcomma(s);
    }
    if ((*l)->message     == NULL)
      ErrorFatal("Missing error message in %s",(*l)->name);
    if ((*l)->number      == -1)
      (*l)->number = 0;

    s = strskip(s, ')');
    s = strcomma(s);
    l = &(*l)->next;
  }
  *l = NULL;
}



const char *readcmd_handler(const char *trailer, help_list helpl)
{
  trailer = strduptok(trailer, &helpl->handler);
  opt.command_codesupplied=1;
  return trailer;
}

static help_list_s skeletoncmd;

cmdfield_t cmdfields[] = {
  { "min-args",      { NULL, ft_uint, &skeletoncmd.min_args },
    { "<number>",
      "Minimum number of arguments acceptable for this command."
    }, IN_BOTH
  },

  { "max-args",      { NULL, ft_uint, &skeletoncmd.max_args },
    { "<number>",
      "Maximum number of arguments acceptable for this command."
    }, IN_BOTH
  },

  { "gstrans-map",   { NULL, ft_uint, &skeletoncmd.gstrans },
    { "<bits>",
      "A bit-field describing which arguments to the command will be "
      "processed by GSTrans before being passed to the module."
    }, IN_BOTH
  },

  { "fs-command",    { NULL, ft_flag_on, &skeletoncmd.fs_command },
    { "",
      "This command is a filing system specific command. "
      "It will only be used if the current filing system is this module."
    }, IN_BOTH
  },

  { "international", { NULL, ft_flag_on, &skeletoncmd.international },
    { "",
      "The help text for this command should be translated through the "
      "messages file supplied in the 'international-help-file' field."
    }, IN_BOTH
  },

  { "add-syntax",    { NULL, ft_flag_on, &skeletoncmd.add_syntax },
    { "",
      "The text supplied in the 'invalid-syntax' field will be appended "
      "to the text supplied in the 'help-text' field."
    }, IN_BOTH
  },

  { "configure",     { NULL, ft_flag_on, &skeletoncmd.configure },
    { "",
      "The command is used for *Status and *Configure commands only. "
      "This is equivilent to the 'status' field."
    }, IN_BOTH
  },
  { "status",        { NULL, ft_flag_on, &skeletoncmd.status },
    { "",
      "The command is used for *Status and *Configure commands only. "
      "This is equivilent to the 'configure' field."
    }, IN_BOTH
  },

  { "help",          { NULL, ft_flag_on, &skeletoncmd.help },
    { "",
      "The command handler will be called to process help requests."
    }, IN_BOTH
  },

  { "invalid-syntax", { NULL, ft_quoted, &skeletoncmd.invalid_syntax },
    { "<string>",
      "When an invalid syntax has been supplied, the supplied text will "
      "be displayed. If the 'add-syntax' field is used, the text will be "
      "appended to the content of the 'help-text' field. If the "
      "'international' field is set, the string supplied is a token to "
      "be looked up in the messages file."
    }, IN_BOTH
  },

  { "help-text",     { NULL, ft_quoted, &skeletoncmd.help_text },
    { "<string>",
      "When an invalid syntax has been supplied, the supplied text will "
      "be displayed. If the 'add-syntax' field is used, the text will be "
      "appended to the content of the 'help-text' field. If the "
      "'international' field is set, the string supplied is a token to "
      "be looked up in the messages file."
    }, IN_BOTH
  },

  { "handler",       { readcmd_handler, ft_check, &skeletoncmd.handler },
    { "<function>",
      "The function to call to handle this command. Setting this field "
      "overrides the use of default handler function."
    }, IN_CMUNGE
  },

  { "no-handler",    { NULL, ft_flag_on, &skeletoncmd.no_handler },
    { "",
      "The command has no handler function present. This is useful for "
      "creating help-only commands."
    }, IN_CMUNGE
  },

  /* End of list */
  { NULL }
};

static void read_commands(const char *s, FILE *file) {

  help_list *l;

  if (opt.commands)
    ErrorFatal("Only supply one command-keyword-table!");
  s = strduptok(s, &opt.helpfn);
  if (strcmp(opt.helpfn, "-")==0 || strcmp(opt.helpfn, "")==0)
    opt.helpfn = NULL;
  s = strcomma(s);
  while ((*s == 0) && !Feof(file)) {
    s = getline(file);
    while (isspace(*s) && (!Feof(file)))
      s++;
  }
  l = &opt.commands;
  while (*s != 0) {
    *l = Malloc(sizeof(**l));
    s = strduptok(s, &(*l)->name);
    s = strskip(s, '(');
    (*l)->min_args       = -1;
    (*l)->max_args       = -1;
    (*l)->gstrans        = -1;
    (*l)->fs_command     = -1;
    (*l)->status         = -1;
    (*l)->configure      = -1;
    (*l)->help           = -1;
    (*l)->status         = -1;
    (*l)->international  = -1;
    (*l)->add_syntax     = -1;
    (*l)->invalid_syntax = NULL;
    (*l)->help_text      = NULL;
    (*l)->handler        = NULL;
    (*l)->no_handler     = -1;
    s = strcomma(s);
    for (; *s != ')' && !Feof(file); s = strcomma(s)) {
      void *valuep;
      const
      char *tail;
      const char *find = s;
      cmdfield_t *field;

      do {
        field=cmdfields;
        while (field->name &&
               (strprefix(find, field->name)==0 ||
                find[strlen(field->name)] != ':'))
          field++;
        if (field->name==NULL)
          ErrorFatal("Unknown field: '%s'", find);
        if (field->values.type==ft_alias)
          find=(char *)field->values.value; /* It's an alias */
      } while (field->values.type==ft_alias);

      if (field == NULL)
      {
        if (*s == 0) {
          while ((*s == 0) && !Feof(file)) {
            s = getline(file);
            while (isspace(*s) && (!Feof(file)))
              s++;
          }
          continue;

        } else {
          ErrorFatal("Unknown argument in command-description: %s", s);
          break;
        }
      }

      s = strchr(s, ':')+1; /* Skip over field and the ':' */

      if (field->hgtype==0)
        ErrorFatal("%s is not supported by any header generator (programmer error)",field->name);
      if (!(field->hgtype & IN_CMUNGE))
        ErrorFatal("%s is not supported by CMunge in command-keyword-table",field->name);
      if (!(field->hgtype & IN_CMHG) && (opt.cmhg))
        CMHGWarning("%s used in command-keyword-table",field->name);

      if (field->values.func)
      { /* We have a function for special processing of this field, so we
           have to call it to deal with the more complex trailling data */
        int *value=(int *)field->values.value;
        if (field->values.type==ft_check)
        { /* They've asked us to check dupe declarations, so we'd better */
          if (*value != 0)
            ErrorFatal("Only supply one %s field in command-keyword-table!",
                       field->name);
        }
        s = field->values.func(s,(*l));
      }
      else
      {
        /* Correct the value pointer to point at the current help_list, rather
           than the skeletoncmd. Pointer arithmetic - may not be portable so
           it may need checking. Probably we'd be better using offsetof in the
           structure ? */
        valuep = ((char *)field->values.value) -
                 ((char *)&skeletoncmd) +
                 ((char *)(*l));

        tail = s;

        while (isspace(*tail))
          tail++;

        /* Do the type based checks */
        switch (field->values.type)
        {
          case ft_flag_on:
          case ft_flag_off:
            {
              int *value=(int *)valuep;
              if (*tail=='\0' || *tail==',' || *tail==')')
              { /* They want the default operation */
                if (field->values.type==ft_flag_on)
                  *value=1;
                else
                  *value=0;
              }
              else
              {
                /* You might like to add On/Off checking here to add some nicer
                   user interface for it */
                ErrorFatal("Trailing junk after %s",field->name);
              }
            }
            break;

          case ft_int:
            {
              unsigned int *value=(unsigned int *)valuep;
              if (*value!=0)
                ErrorFatal("Only supply one %s field in command-keyword-table!",field->name);
              tail = strint(tail, value);
              if (*value==0)
                ErrorFatal("Illegal %s: %i in command-keyword-table", field->name,*value);
              if (*tail!='\0' && *tail!=',' && *tail!=')')
                ErrorFatal("Trailing junk after %s in command-keyword-table",field->name);
            }
            break;

          case ft_uint:
            {
              unsigned int *value=(unsigned int *)valuep;
              if (*value!=-1)
                ErrorFatal("Only supply one %s field in command-keyword-table!",field->name);
              tail = strint(tail, value);
              if (*value==-1)
                ErrorFatal("Illegal %s: %i in command-keyword-table", field->name,*value);
              if (*tail!='\0' && *tail!=',' && *tail!=')')
                ErrorFatal("Trailing junk after %s in command-keyword-table",field->name);
            }
            break;

          case ft_int64:
          case ft_xint64:
            {
              unsigned int *value=(unsigned int *)valuep;
              if (*value!=0)
                ErrorFatal("Only supply one %s field in command-keyword-table!",field->name);
              tail = strint(tail, value);
              if ((*value==0) || (*value & 0x3f))
                ErrorFatal("Illegal %s: 0x%08x in command-keyword-table", field->name,*value);
              if ((field->values.type==ft_xint64) && (*value & 0x20000))
                ErrorFatal("Illegal %s: 0x%08x (x bit set) in command-keyword-table", field->name,*value);
              if (*tail!='\0' && *tail!=',' && *tail!=')')
                ErrorFatal("Trailing junk after %s in command-keyword-table",field->name);
            }
            break;

          case ft_label:
          case ft_quoted:
            {
              const char **value = (const char **)valuep;
              if (*value!=NULL)
                ErrorFatal("Only supply one %s field in command-keyword-table!",field->name);

              if (field->values.type==ft_label)
                tail = strduptok(tail, value);
              else
                tail = strstring(tail, value);
              if (*tail!='\0' && *tail!=',' && *tail!=')')
                ErrorFatal("Trailing junk after %s in command-keyword-table",field->name);
            }
            break;

          case ft_string:
            {
              char **value=(char **)valuep;
              if (*value!=NULL)
                ErrorFatal("Only supply one %s field in command-keyword-table!",field->name);

              *value=strdup_strip(tail);
            }
            break;

          default:
            /* Erm... what the hell have you done to my nice little description
               block */
            ErrorFatal("Programmer confusion error (%s:%s: ft: %i) in command-keyword-table",
                       field->name,s,field->values.type);
        }

        s = tail;
      }
    }

    if (*s=='\0' && Feof(file)) {
      ErrorFatal("Ran out of file when parsing command details!");
      break;
    }
    if ((*l)->min_args      == -1)
      (*l)->min_args      = 0;
    if ((*l)->max_args      == -1)
      (*l)->max_args      = 0;
    if ((*l)->gstrans       == -1)
      (*l)->gstrans       = 0;
    if ((*l)->fs_command    == -1)
      (*l)->fs_command    = 0;
    if ((*l)->status        == -1)
      (*l)->status        = 0;
    if ((*l)->configure     == -1)
      (*l)->configure     = 0;
    if ((*l)->help          == -1)
      (*l)->help          = 0;
    if ((*l)->status        == -1)
      (*l)->status        = 0;
    if ((*l)->international == -1)
      (*l)->international = 0;
    if ((*l)->add_syntax    == -1)
      (*l)->add_syntax    = 0;
    if ((*l)->no_handler    == -1)
      (*l)->no_handler    = 0;

    /* Check the sanity of these settings */
    if ((*l)->min_args > 255)
      ErrorFatal("min-args: must be between 0 and 255 in command %s",(*l)->name);
    if ((*l)->max_args > 255)
      ErrorFatal("max-args: must be between 0 and 255 in command %s",(*l)->name);
    if ((*l)->gstrans > 255)
      ErrorFatal("gstrans: mapping may only describe 8 bits in command %s",(*l)->name);
    if ((*l)->add_syntax && (*l)->international)
      ErrorFatal("add-syntax: and international: are mutually exclusive in command %s",(*l)->name);
    if ((*l)->add_syntax && !(*l)->invalid_syntax)
      Warning("add-syntax: has no meaning without invalid-syntax: in command %s",(*l)->name);
    if ((*l)->add_syntax && !(*l)->help_text)
      Warning("add-syntax: has no meaning without help-text: in command %s",(*l)->name);
    if ((*l)->help)
      ErrorFatal("help: is not supported in command %s (unsupported in CMunge, unreliable in CMHG)", (*l)->name);
    if ((*l)->no_handler && (*l)->handler)
      ErrorFatal("no-handler: and handler: are mutually exclusive in command %s",(*l)->name);
    if (opt.helpfn == NULL && (*l)->handler==NULL && (*l)->no_handler==0)
      (*l)->no_handler = 1; /* Silently promote to 'no-handler' if nothing
                               present */

    s = strskip(s, ')');
    s = strcomma(s);
    l = &(*l)->next;
  }
  *l = NULL;
}

#define HANDLER_ALLOW_PW    (1<<0)
#define HANDLER_ALLOW_CARRY (1<<1)
#define HANDLER_ALLOW_ERROR (1<<2)

static void read_handlers(const char *s, handler_list *h, unsigned long allow,
                          FILE *file) {

  /* Find the end of the list, so that we append to whatever is already there, rather
     than replacing it */
  while (*h)
    h=&(*h)->next;

  while (*s != 0) {
    *h = Malloc(sizeof(**h));
    (*h)->extradata = NULL;
    s = strfindpair(s, &(*h)->name, &(*h)->handler);
    (*h)->private_word = -1;
    (*h)->carry_capable = 0;
    (*h)->error_capable = 0;
    if (*s == '(')
    {
      if (opt.cmhg)
        CMHGWarning("handler arguments supplied for handler %s",(*h)->name);

      s = strskip(s, '(');
      while (*s != ')') {
        if (strprefix(s, "private-word:")) {
          if (!(allow & HANDLER_ALLOW_PW))
            ErrorFatal("private-word argument not permitted");
          if ((*h)->private_word != -1)
            ErrorFatal("private-word supplied twice!");
          s += strlen("private-word:");
          while (isspace(*s))
            s++;

          if (*s!='R' && *s!='r')
            ErrorFatal("private-word value must be a register!");
          s++;
          s = strint(s, (unsigned int *)&(*h)->private_word);
          if ((*h)->private_word < 0 ||
              (*h)->private_word > 12)
            ErrorFatal("private-word register must be r0-r12!");
        } else if (strprefix(s, "carry-capable:")) {
          if (!(allow & HANDLER_ALLOW_CARRY))
            ErrorFatal("carry-capable argument not permitted");
          (*h)->carry_capable = 1;

          s += strlen("carry-capable:");
          while (isspace(*s))
            s++;
        } else if (strprefix(s, "error-capable:")) {
          if (!(allow & HANDLER_ALLOW_ERROR))
            ErrorFatal("error-capable argument not permitted");
          (*h)->error_capable = 1;

          s += strlen("error-capable:");
          while (isspace(*s))
            s++;
        } else if (Feof(file)) {
          ErrorFatal("Ran out of file when parsing handler details!");
          break;
        } else if (*s == 0) {
          while ((*s == 0) && !Feof(file)) {
            s = getline(file);
            while (isspace(*s) && (!Feof(file)))
              s++;
          }
        } else {
          ErrorFatal("Unknown argument in handler: %s", s);
          break;
        }
        s = strcomma(s);
      }
      s++;
    }

    h = &(*h)->next;
    s = strcomma(s);
  }
  *h = NULL;
}

static void read_pdriver(const char *s, FILE *file)
{
  pdriver_list **pl;

  UNUSED(file);

  if (opt.pdriver_entry)
    ErrorFatal("Only supply one pdriver-handler!");

  s = strduptok(s, &opt.pdriver_entry);
  if (opt.pdriver_entry[0] == '\0')
    ErrorFatal("No entry point defined for pdriver-handler!");
  if (*s++ != ',')
      ErrorFatal("Wrong separator found in pdriver-handler!");
  while (isspace(*s))
    ++s;

  pl = &opt.pdriver_names;
  while (*s)
  {
    (*pl) = Malloc(sizeof(**pl));
    (*pl)->next = NULL;
    s = strduptok(s, &(*pl)->handler);
    if (*s == ',')
    {
      ++s;
      while (isspace(*s))
	++s;
    }
    else if (*s != '\0')
      ErrorFatal("Wrong separator found in pdriver-handler!");
    pl = &(*pl)->next;
  }
}

static void read_swidec(const char *s, FILE *file)
{
  handler_list h;

  UNUSED(file);

  h = Malloc(sizeof(*h));
  h->next = NULL;
  s = strfindpair(s, &h->name, &h->handler);
  opt.swi_decoder = h;
  if (*s != 0)
    ErrorFatal("Junk found at end of swi-decoding-code!");
}

static void read_generics(const char *s, FILE *file)
{
  read_handlers(s, &opt.generics, HANDLER_ALLOW_PW | HANDLER_ALLOW_CARRY, file);
}

static void read_irqs(const char *s, FILE *file)
{
  read_handlers(s, &opt.irqs, 0, file);
}

static void read_vectors(const char *s, FILE *file)
{
  read_handlers(s, &opt.vectors, HANDLER_ALLOW_ERROR, file);
}

static void read_vector_traps(const char *s, FILE *file)
{
  read_handlers(s, &opt.vector_traps, 0, file);
}

static void read_runnable(const char *s, FILE *file)
{
  switch (opt.runnable)
  {
    case run_none:
      opt.runnable=run_clib;
      break;

    case run_blib:
      break; /* We've already specified it on the CLI, this just confirms */

    case run_clib:
    default:
      ErrorFatal("module-is-runnable cannot be supplied twice!");
  }

  while (*s != 0) {
    /* Please note that simple-app === run_rmaapp_simple and is a VERY
       experimental feature. It gives you an application with an equivalent
       entry sequence to that of ShareFS. It does NOT make the application
       multiply threadable in user mode. It does NOT handle any exceptions
       or other environment handlers. It does NOT provide a useful context
       for anything other than writing a very simple Wimp application. This
       is all it is intended for. To use it for anything else would be very
       unwise. I've spent pretty much all day working on this and trying to
       get my head around a way to do the whole thing safely. The simple
       answer is that it's impossible to do properly under the current
       implementation of RISC OS without some collusion over the taskhandles
       which I'm unwilling to incorporate because it means that some
       knowledge of the environment is required and it /still/ won't cope
       with a multiply threaded single-tasked module. There are a number of
       issues here :

        1 You must be able to create a stack in RMA of arbitrary size for
          the currently entered task
        2 You must be able to remove this stack on module kill, and for
          preference, disable the application that relies on it.
        3 You must be able to trap errors, exceptions and exits and remove
          the stack prior to returning to the previous application.
        4 Each application must be able to have its own C context such that
          exceptions can be restored correctly
        5 You must, on the external death of the application using the stack,
          be able to remove the stack without it having exited in any manner.
        6 Stacks should be extensible.

       Which requires :
        1 An entry sequence similar to that provided here.
        2 A list of all stacks must be maintained for freeing on exit.
        3 Handlers must be aware of the existance of multiple RMA stacks
          which are unrelated to one another but freeable at the instance
          of their calls.
        4 C contexts can be copied from the base area, with the actual state
          stored at the base of the stack (or linked off it). This should
          work. The SCL static data living there cannot be accessed off the
          main SVC entry points. This should not be an issue.
        5 Requires an associated handle for each application started such
          that its stacks can be removed. This requires some knowledge of
          how applications are identified. At present it is not possible to
          do this with any accuracy because of the way in which applications
          are not tasks. It is intended that this issue be ignored for the
          simplification of the system
        6 Stack extension handled by the SCL should occur seamlessly and
          without special support. Freeing multiple stacks should work.

       This implementation has :
        1 Fulfilled.
        2 Only one stack can exist, so this is fulfilled to that extent.
        3 Any error, exception or exit is handled by the parent handler - no
          handlers are installed. Memory leaks are likely.
        4 Ignored - main C context used. As only one stack is present, this
          is not an issue.
        5 Ignored, as per requirements
        6 Ignored. No idea what happens on attempted stack extension. Examine
          this.

       Consequently, this implementation is only useful for SIMPLE
       applications which cannot be re-threaded.
    */
    if (strprefix(s, "simple-app:")) {
      opt.runnable=run_rmaapp_simple; /* Experimental */
      s += strlen("simple-app:");
    } else if (Feof(file)) {
      ErrorFatal("Ran out of file when parsing command details!");
      break;
    } else if (*s == 0) {
      while ((*s == 0) && !Feof(file)) {
        s = getline(file);
        while (isspace(*s) && (!Feof(file)))
          s++;
      }
    } else {
      ErrorFatal("Unknown argument in command-description: %s", s);
      break;
    }
    s = strcomma(s);
  }
}

/* Notes about the blank section :
    * The 'blank' data is used to generate a blank cmhg file.
    * All ft_alias entries will be ignored.
    * Fields will be used IN ORDER
    * Comments will be word wrapped by the blank generation code
    * End your sentances with a .
*/

field_t fields[] =
{
  { "module-is-runnable",          { read_runnable },
    { "",
      "If you declare module-is-runnable the module will be entered "
      "at the main() function in user mode (ie like a normal C program).\n"
      "You will probably never need this field."
    }, IN_BOTH },

  { "module-is-not-reentrant",     { NULL, ft_flag_off, &opt.reentrant },
    { "",
      "When your module cannot be reentered you should declare this. "
      "At present it does nothing.\n"
      "You will probably never need this field."
    }, IN_CMUNGE /* I had this as IN_BOTH, but CMHG seems to fault it ? */
  },

  { "module-is-c-plus-plus",       { NULL, ft_flag_on, &opt.cplusplus },
    { "",
      "When your module includes C++ code built using the CFront compiler, "
      "you should declare this. As part of the module initialisation "
      "sequence, the C++ global constructors will be called. Essentially "
      "this removes the requirement to manually invoke ______main."
    }, IN_CMUNGE
  },

  { "title-string",                { NULL, ft_label,    &opt.title },
    { "ModuleName",
      "You should give your module a sensible name which describes its "
      "purpose both accurately and concisely. You must register any "
      "module names you use if they are to be distributed."
    }, IN_BOTH },

  { "help-string",                 { NULL, ft_string,   &opt.help },
    { "ModuleDescription 0.01 Tail message",
      "The help-string is the name that is displayed to the user in *Help "
      "Modules and other places that a more readable name is required. "
      "You should attempt to make this a readable version of the "
      "title-string, and - at worst - use the same name as in the "
      "title-string. The tail message may be used to indicate a variant "
      "of the module, or a copyright string."
    }, IN_BOTH },

  { "swi-chunk-base-number",       { NULL, ft_xint64,   &opt.swi_base },
    { "&C0000", /* Now in the user allocatable range */
      "If you supply SWIs in your module, you must provide a base number "
      "at which these start. Each chunk is 64 entries long and must be "
      "registered if the module is going to be distributed.\n"
      "SWI chunks     &0- &3FFC0 are for operating system use.\n"
      "           &40000- &7FFC0 are for operating system exetensions.\n"
      "           &80000- &BFFC0 are for third party applications.\n"
      "           &C0000- &FFFC0 are for user applications.\n"
      "Bit 17 will always be clear on the SWI chunk number.\n"
      "Consult PRM 1-26 for more details."
    }, IN_BOTH },

  /* For now we'll relax the error-base restraints. This should /really/
     be checking for some constraints. Error blocks are allocated in
     chunks of 256 entries. Originally this code restricted you to 64
     entries, but Theo has pointed out that this may not be sufficiently
     wide for all uses. Left as int for now. */
  { "error-base",                  { NULL, ft_int,      &opt.error_base },
    { "&840000", /* Closer to a 'user-range'. */
      "If you have any error messages in your module that are returned "
      "through the standard interfaces, you should use different error "
      "numbers for each, starting at a base number. You should register "
      "your error bases before distributing the module. Using random "
      "error numbers is dangerous.\n"
      "Error chunks        &0-   &7FFFFF are for operating system use.\n"
      "               &800000-   &FFFFFF are for other authors.\n"
      "             &1B000000- &1BFFFFFF are for program (NOT module) errors.\n"
      "             &80000000- &FFFFFFFF are for serious hardware exceptions.\n"
      "Consult PRM 1-43, 5a-493 for more details."
    }, IN_CMUNGE | IN_ERRORS },
  { "error-chunk-base-number",     { NULL, ft_alias,   (void*)"error-base" } },

  { "error-identifiers",           { read_errors, ft_check, &opt.errors },
    { "err_Silly(&1234,\"Silly\")",
      "Allocating error messages within the module is more efficient than "
      "doing so manually in your C code, and is less likely to pollute "
      "error number space. The number is optional, and if omitted will be "
      "allocated from the error chunk."
    }, IN_CMUNGE | IN_ERRORS },

  { "initialisation-code",         { NULL, ft_label,    &opt.init },
    { "Mod_Init",
      "When the module is initialised, this routine will be entered. You "
      "should be very careful to initialise your module safely. If anything "
      "fails, you can return an error message. Finalisation code will not "
      "be called if you return an error."
    }, IN_BOTH },

  { "finalisation-code",           { NULL, ft_label,    &opt.final },
    { "Mod_Final",
      "When the module is killed, this routine will be entered. If you "
      "do not wish to be killed then you should return an error and retain "
      "as much of the current state as possible. Causing an abort in this "
      "routine will leave a 'dead' module in the chain."
    }, IN_BOTH },

  { "international-help-file",     { NULL, ft_quoted,   &opt.mfile },
    { "\"Resources:$.ThirdParty.ModuleName.Messages\"",
      "Syntax and help messages may be internationalised by using a messages "
      "file."
    }, IN_BOTH },

  { "pdriver-handler",             { read_pdriver },
     { "PDriver_Entry, PDReason00_Handler, PDReason01_Handler, PDReason02_Handler",
       "The PDriver handler specifies the PDriver entry point for registering "
       "with SWI PDriver_DeclareDriver and is followed by a list of C "
       "routines one per PDriver reason code."
     }, IN_CMUNGE | IN_ERRORS },
    
  { "swi-handler-code",            { NULL, ft_label,    &opt.swi_handler },
    { "Mod_SWI",
      "The SWI handler is entered for every SWI the module provides. "
      "Generally, this is a switch statement, to dispatch each of the SWIs "
      "that are known."
    }, IN_BOTH },

  { "swi-decoding-code",           { read_swidec },
    { "Mod_SWIDecoder",
      "The SWI decoding code performs name-to-number and number-to-name "
      "translations for this module. In practice, you very rarely need to "
      "use this.\n"
      "You will probably never need this field."
    }, IN_BOTH },

  { "swi-decoding-table",          { read_swis },
    { "Prefix One Two Three",
      "The SWI decoding table lists the SWIs which the module supplies. "
      "The first entry is always the SWI prefix, used on all SWIs that the "
      "module provides. This should be the name of the module, or some "
      "obvious variant. You should register your SWI prefixes when you "
      "register a SWI chunk. Each name entry may be followed by /<entry> "
      "to use a particular routine to handle a SWI.\n"
    }, IN_BOTH },

  { "library-enter-code",          { NULL, ft_label,    &opt.lib_init },
    { "_clib_initialisemodule",
      "The library initialisation code sets up the C environment for use "
      "with this module. You may wish to perform some initialisation prior "
      "to the C environment being created. You must call "
      "_clib_initialisemodule prior to returning, or provide an identical "
      "functionality.\n"
      "You will probably never need this field."
    }, IN_BOTH },
  { "library-initialisation-code", { NULL, ft_alias, (void*)"library-enter-code"} },

  { "date-string",                 { NULL, ft_string,   &opt.datestring },
    { "21 Dec 1999",
      "The date-string is optional and should only be included if you feel "
      "that a date is a different reference to the version number. Certain "
      "ports from other platforms may wish to indicate their source base "
      "date using this field.\n"
      "You will probably never need this field."
    }, IN_BOTH },

  { "generic-veneers",            { read_generics,  ft_none, &opt.generics },
    { "Veneer_Entry/Veneer_Handler",
      "Generic veneers are used to provide a simple C environment for SVC "
      "and IRQ modes. You should register the Entry routine as the external "
      "interface (ie provide it as the code address to call). The Handler "
      "routine will be called from this. Parameters for the veneer may be "
      "specified in brackets in the same form as command keywords :\n"
      "\t({<field>: <value>})\n"
      "Field names:\n"
      "\tprivate-word: <register>\bPrivate word register\n"
      "\tcarry-capable:\bVeneer is capable of handling returning of the carry flag\n"
    }, IN_BOTH },

  { "irq-handlers",                { read_irqs,     ft_none, &opt.irqs },
    { "IRQ_Entry/IRQ_Handler",
      "IRQ veneers are the old name for vector-handlers. Use vector-handlers in "
      "preference to irq-handlers."
    }, IN_BOTH },

  { "vector-handlers",             { read_vectors,  ft_check, &opt.vectors },
    { "Vector_Entry/Vector_Handler",
      "Vector veneers are used to provide a simple C environment for SVC "
      "and IRQ modes. You should register the Entry routine as the external "
      "interface (ie provide it as the code address to call). The Handler "
      "routine will be called from this. You can return 0 or 1 from your "
      "routine to say you either handled the vector, or did not. "
      "Parameters for the veneer may be specified in brackets in the same "
      "form as command keywords :\n"
      "\t({<field>: <value>})\n"
      "Field names:\n"
      "\terror-capable:\bVeneer is capable of claiming the vector and "
                        "returning an error"
    }, IN_BOTH },

  { "vector-traps",                { read_vector_traps, ft_check, &opt.vector_traps },
    { "VectorTrap_Entry/VectorTrap_Handler",
      "VectorTrap veneers are used to provide a simple C environment for SVC "
      "and IRQ modes. You should register the Entry routine as the external "
      "interface (ie provide it as the code address to call). The Handler "
      "routine will be called from this. You can an error to pass this back "
      "to the calling. The function 'trap' can be called with a register "
      "block and private value to pass the call on to the vector. The trap "
      "function will return registers in the supplied block, and if an error "
      "is flagged this will be the return value."
    }, IN_CMUNGE },

  { "event-handler",               { read_events,   ft_none, &opt.events },
    {
      "Event_Entry/Event_Handler 1 2 3 4 5 6",
      "Event handlers are used to provide a simple C environment for SVC "
      "and IRQ modes. You should register the Entry routine as the external "
      "interface (ie provide it as the code address to call). The Handler "
      "routine will be called from this. You can return 0 or 1 from your "
      "routine to say you either handled the interrupt, or did not. The "
      "list of numbers at the end of the line indicates which events (in "
      "R0) will be passed to the Handler routine. You can also trap upcalls "
      "with an event handler, as their reason codes are also passed in R0."
    }, IN_BOTH },

  { "command-keyword-table",       { read_commands, ft_none },
    { "Mod_Command\n   ModuleCommand()",
      "The command table is one of the more complex fields in the CMHG "
      "file. The first entry is the routine to call to process the "
      "commands as they arrive. The remainder of the entries are the "
      "command definitions. These take the form :\n"
      "\t<Command>({<field>: <value>})\n"
      "\n"
      "Field names:"
#if 0
      /* Note: The spaces in this block are unimportant; it's the \b code
               that is important to line up the strings */
      "\n"
      "\tmin-args: <number>   \bMinimum number of arguments to command\n"
      "\tmax-args: <number>   \bMaximum number of arguments to command\n"
      "\tgstrans-map: <bits>  \bBitmap of arguments that should be GSTransd\n"
      "\tfs-command:          \bDeclares that this is a filing system command\n"
      "\tinternational:       \bDeclares this this uses international tokens\n"
      "\tadd-syntax:          \bAdds the syntax message to the help message\n"
      "\tconfigure:           \bDeclares that this is a configuration name\n"
      "\tstatus:              \bDeclares that this is a configuration name\n"
      "\thelp:                \bDeclares that we want to be called for help "
                               "message processing. Not implemented\n"
      "\thelp-text: <string>  \bThe text/token to use for *Help\n"
      "\tinvalid-syntax: <string>\bThe text/token to use for syntax errors"
#else
/* The help text for the table is now supplied by a function, using the
   cmdfields table. */
      , blank_commands
#endif
    }, IN_BOTH },

  { "service-call-handler",        { read_services },
    {
      "Mod_Service 1 2 3 4 5 6",
      "The module service entry will be called to process services. You "
      "are strongly recommended to include the numbers of the services "
      "you wish to handle."
    }, IN_BOTH },

  { NULL } /* zero terminated end of list */
};

static void parse_line(FILE *file) {

  char *line;

  line = getline(file);

  while (isspace(*line))
    line++;

  if (line[0] == '\0')
    return; /* Null lines are just fine */

  { /* Search for field block */
    field_t *field;
    char *colon=strchr(line,':');
    const char *tail;

    if (colon==NULL)
    {
      ErrorFatal("Field names should be of the form <field>: [<value>]");
    }
    *colon++='\0';
    tail=colon;

    /* Skip any prefix of whitespace */
    while (isspace(*tail))
      tail++;

    do {
      field=fields;
      while (field->name && !stricmp(field->name,line))
        field++;
      if (field->name==NULL)
        ErrorFatal("Unknown field: '%s'", line);
      if (field->values.type==ft_alias)
        line=(char *)field->values.value; /* It's an alias */
    } while (field->values.type==ft_alias);

    if (field->hgtype==0)
      ErrorFatal("%s is not supported by any header generator (programmer error)",field->name);
    if (!(field->hgtype & IN_CMUNGE))
      ErrorFatal("%s is not supported by CMunge",field->name);
    if (opt.mode_errors && !(field->hgtype & IN_ERRORS))
      ErrorFatal("%s is not supported by CMunge in 'errors' mode\n",field->name);
    if (!(field->hgtype & IN_CMHG) && (opt.cmhg))
      CMHGWarning("%s used",field->name);

    if (field->values.func)
    { /* We have a function for special processing of this field, so we
         have to call it to deal with the more complex trailling data */
      int *value=(int *)field->values.value;
      if (field->values.type==ft_check)
      { /* They've asked us to check dupe declarations, so we'd better */
        if (*value != 0)
          ErrorFatal("Only supply one %s field!",field->name);
      }
      field->values.func(tail,file);
      return; /* All the processing for that is done */
    }

    /* Do the type based checks */
    switch (field->values.type)
    {
      case ft_flag_on:
      case ft_flag_off:
        {
          int *value=(int *)field->values.value;
          if (*tail=='\0')
          { /* They want the default operation */
            if (field->values.type==ft_flag_on)
              *value=1;
            else
              *value=0;
          }
          else
          {
            /* You might like to add On/Off checking here to add some nicer
               user interface for it */
            ErrorFatal("Trailing junk after %s",field->name);
          }
        }
        break;

      case ft_int:
        {
          unsigned int *value=(unsigned int *)field->values.value;
          if (*value!=0)
            ErrorFatal("Only supply one %s field!",field->name);
          tail = strint(tail, value);
          if (*value==0)
            ErrorFatal("Illegal %s: %i", field->name,*value);
          if (*tail!='\0')
            ErrorFatal("Trailing junk after %s",field->name);
        }
        break;

      case ft_uint:
        {
          unsigned int *value=(unsigned int *)field->values.value;
          if (*value!=-1)
            ErrorFatal("Only supply one %s field!",field->name);
          tail = strint(tail, value);
          if (*value==-1)
            ErrorFatal("Illegal %s: %i", field->name,*value);
          if (*tail!='\0')
            ErrorFatal("Trailing junk after %s",field->name);
        }
        break;

      case ft_int64:
      case ft_xint64:
        {
          unsigned int *value=(unsigned int *)field->values.value;
          if (*value!=0)
            ErrorFatal("Only supply one %s field!",field->name);
          tail = strint(tail, value);
          if ((*value==0) || (*value & 0x3f))
            ErrorFatal("Illegal %s: 0x%08x", field->name,*value);
          if ((field->values.type==ft_xint64) && (*value & 0x20000))
            ErrorFatal("Illegal %s: 0x%08x (x bit set)", field->name,*value);
          if (*tail!='\0')
            ErrorFatal("Trailing junk after %s",field->name);
        }
        break;

      case ft_label:
      case ft_quoted:
        {
          const char **value = (const char **)field->values.value;
          if (*value!=NULL)
            ErrorFatal("Only supply one %s field!",field->name);

          if (field->values.type==ft_label)
            tail = strduptok(tail, value);
          else
            tail = strstring(tail, value);
          if (*tail != 0)
            ErrorFatal("Trailing junk after %s",field->name);
        }
        break;

      case ft_string:
        {
          char **value=(char **)field->values.value;
          if (*value!=NULL)
            ErrorFatal("Only supply one %s field!",field->name);

          *value=strdup_strip(tail);
        }
        break;

      default:
        /* Erm... what the hell have you done to my nice little description
           block */
        ErrorFatal("Programmer confusion error (%s:%s: ft: %i)",
                   field->name,line,field->values.type);
    }
  }
}

void ReadFile(void) {

  FILE *file;
  int rc;
  char buf[2048]; /* Make this a dynamic buffer, please! */
  char *bufend;
  defines_list defines;
  include_list includes;

  if (opt.pfile) {

    /**** Command name */
    switch (opt.toolchain)
    {
      case tc_gcc:
        /* -xc hints that the input language is to be considered as C
           because that is not necessarily guessable with the usual
           cmhg extension. */
#ifdef __riscos
#  define GCC_BINARY_NAME "gcc"
#else
#  define GCC_BINARY_NAME "arm-unknown-riscos-gcc"
#endif
        bufend = buf+sprintf(buf, GCC_BINARY_NAME " -E -nostdinc -xc");
        break;
      case tc_norcroft:
#ifdef __riscos
        bufend = buf+sprintf(buf, "cc -E -C++");
#else
        bufend = buf+sprintf(buf, "ncc -E -C++");
#endif
        break;
      case tc_lcc:
        bufend = buf+sprintf(buf, "cpp");
        break;
      default:
        fprintf(stderr,"Invalid Toolchain (internal error)\n");
        abort();
        return;
    }

    /**** Defines and undefines */
    defines = opt.defines;
    while (defines) {
      if (defines->invert) {
        bufend += sprintf(bufend, " -U%s", defines->name);
      } else {
        if (defines->value)
          bufend += sprintf(bufend, " -D%s=%s", defines->name, defines->value);
        else
          bufend += sprintf(bufend, " -D%s", defines->name);
      }
      defines=defines->next;
    }
    includes = opt.includes;
    while (includes) {
      bufend += sprintf(bufend, " -I%s", includes->path);
      includes = includes->next;
    }

    /**** Throwback and dependencies */
    switch (opt.toolchain)
    {
      case tc_gcc:
        /* Don't know how to do dependencies with gcc -
             -M gives output in a mix of unix and RISC OS which isn't useful
         */
        if (opt.throwback)
          bufend += sprintf(bufend," -mthrowback");
        /* See the Options_CheckSanity in c.options for warnings about the
           dependency usage. */
        break;

      case tc_norcroft:
        if (opt.throwback)
          bufend += sprintf(bufend," -throwback");
        if (opt.dfile)
          bufend += sprintf(bufend," -depend %s", opt.dfile);
        break;

      case tc_lcc:
        /* Don't know how to do throwback with lcc */
        /* Don't know how to do dependencies with lcc */
        /* See the Options_CheckSanity in c.options for warnings about the
           dependency and throwback usage. */
        break;
    }

    /**** Filenames */
    switch (opt.toolchain)
    {
      case tc_gcc:
        bufend += sprintf(bufend, " %s -o %s", opt.infile, opt.pfile);
        break;
      case tc_norcroft:
        if (opt.dfile)
        {
          const char *result_file = NULL;
          /* We have to provide the name of the file that is being generated
             so that we have a correct dependency in the !Depend file. */
          if (opt.ofile)
            result_file = opt.ofile;
          else if (opt.sfile && !opt.stemp)
            result_file = opt.sfile;
          else if (opt.hfile)
            result_file = opt.hfile;
          else if (opt.x_hdr)
            result_file = opt.x_hdr;
          else if (opt.x_h)
            result_file = opt.x_h;
          if (result_file != NULL)
            bufend += sprintf(bufend, " -o %s", result_file);
        }

        {
          FILE *f;
          const char *nativefilename = opt.infile;
          /* We need to identify whether the input file is valid or not;
             Norcroft CC does not understand (for example) file.cmhg as
             meaning cmhg.file. As such, we need to convert from the
             Unix-named convention to the actual file. */
          f = fopen(opt.infile, "r");
          if (f)
          {
            fclose(f);
            nativefilename = opt.infile;
          }
          else
          {
            const char *altfilename;
            altfilename = filename_unixtoriscos(opt.infile,
                                                EXTLIST_NORCROFT_CMHG);
#ifndef __riscos
            /* If we're not on RISC OS then a RISC OS name won't be valid
               and we will need the actual unix name to be able to check
               the file's existance. Because we specify no extension list,
               we just get the filename conventions changed, with no
               extension swapping.
             */
            altfilename = filename_riscostounix(altfilename, NULL);
#endif
            f = fopen(altfilename, "r");
            if (f)
            {
              fclose(f);
              nativefilename = altfilename;
            }
          }

          bufend += sprintf(bufend, " %s > %s", nativefilename, opt.pfile);
        }
        break;
      case tc_lcc:
        bufend += sprintf(bufend, " %s > %s", opt.infile, opt.pfile);
        break;
    }

    /* printf("Command: %s\n", buf); */
    rc = our_system(buf);
    switch(rc) {
      case EXIT_SUCCESS: /* All ok */
        break;
      case -2: /* Failed */
        ErrorFatal("Could not start pre-processor");
        break;
      default: /* Some other return code */
        fprintf(stderr,"Pre-processor exited with code %i unexpectedly\n",rc);
        exit(rc);
    }
    file = file_read(opt.pfile, remove_onclose);
    if (file == NULL)
      ErrorFatal("Temporary pre-processor file not found");
  } else {
    const char *filename;
    file = file_read(opt.infile, remove_never);
    if (file == NULL)
      ErrorFatal("File %s not found!", opt.infile);
    /* The opt.infile can be changed (because of suffix swapping, or
       canonicalisation). */
    filename = file_getfilename(file);
    if (filename == NULL)
      ErrorFatal("Internal program error. file_getfilename() failed unexpectedly.");
    Free(opt.infile);
    opt.infile = strdup_strip(filename);
  }

  /* Allocate us a linebuffer */
  bump();

  while (!Feof(file)) {
    parse_line(file);
  }
  /* We're at the end of our input file, so any errors/warnings
     should not mention the last line of our input file as source
     of the error/warning. */
  opt.atline = 0;
  file_close(file);
}
