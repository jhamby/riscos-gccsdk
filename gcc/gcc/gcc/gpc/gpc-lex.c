/* GNU Pascal compiler lexical analyzer
   Copyright (C) 1989-2000, Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/*
 * Authors: Jukka Virtanen <jtv@hut.fi>
 *          Peter Gerwinski <peter@gerwinski.de>
 *
 * This file is derived from GCC's `c-lex.c'.
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "gbe.h"
#include "c-lex.h"
#include "gpc-defs.h"
#include "parse.h"
#include "module.h"
#include "util.h"
#include "types.h"
#include "gpc-options.h"
#ifdef EGCS
#include "dwarfout.h"
#endif

#ifdef MULTIBYTE_CHARS
#include <stdlib.h>
#include <locale.h>
#endif

#ifndef errno
extern int errno;
#endif

#ifndef HAVE_SIGALRM
#ifdef SIGALRM
#define HAVE_SIGALRM 1
#endif
#endif

#ifdef EGCS
#define LANG_DECODE_OPTION(OPTION) (lang_decode_option (1, &(OPTION)))
#else /* not EGCS */
#define LANG_DECODE_OPTION(OPTION) (lang_decode_option (OPTION))
#endif /* not EGCS */

/* Don't use the locale dependent routines. */
static int new_isdigit PROTO ((int));
static int new_isalpha PROTO ((int));
static int new_isalnum PROTO ((int));

static int
new_isdigit (c)
     int c;
{
  return c >= '0' && c <= '9';
}

static int
new_isalpha (c)
     int c;
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static int
new_isalnum (c)
     int c;
{
  return new_isalpha (c) || new_isdigit (c);
}

#undef isdigit
#undef isalpha
#undef isalnum
#define isdigit new_isdigit
#define isalpha new_isalpha
#define isalnum new_isalnum

/* Parser variables.
 */

/* Flag for debugging the parser.
 */
extern int yydebug;

/* The semantic value of the lookahead symbol.
 */
YYSTYPE yylval;


/* Compiler flags.
 */

/* Nonzero means to copy the source input to stderr.
 */
int flag_debug_source = 0;

/* If non-null, it gives the identifier of a procedure to be called
   automatically before each statement.
 */
tree debug_statement = NULL_TREE;

/* Nonzero means to output progress report messages.
 */
int flag_progress_messages = 0;

/* Nonzero means to output progress bar information.
 */
int flag_progress_bar = 0;


/* Other compiler variables.
 */

/* File used for outputting assembler code.
 */
extern FILE *asm_out_file;

/* This is NULL_TREE before the start of the program header is parsed.
 * We use this to recognize the keyword "Module" (and all other reserved
 * words.)
 */
extern tree main_program_name;


/* Lexical analyzer: Variables.
 */

/*
  Determines what the lexer returns for `=':
  < 0 means: `='
  = 0 means: LEX_CONST_EQUAL
  > 0 means: `=', but after that many closing parentheses/brackets, return LEX_CONST_EQUAL
*/
int lex_const_equal = -1;

/* Nonzero means: don't recognize BP style `^a' character constants.
 */
int lex_caret = 0;

tree lastiddecl;

/* List of types and structure classes of the current declaration.
 */
tree current_declspecs;

/* File being read.
 */
char *input_filename;

/* Top-level source file.
 */
char *main_input_filename;

/* Pointer to token buffer.
 * Actual allocated length is maxtoken + 2.
 */
char *token_buffer;

/* Current line number in file being read.
 */
int lineno;

/* Newlines encountered in the preprocessed input (all files).
 */
static int preprocessed_lineno = 0;

/* Input file.
 * This can be a pipe from the preprocessor.
 */
FILE *finput;

/* The reserved word recognition is done with
 * Doug Schmidt's gperf program in O(1) time.
 *
 * The structure resword is defined in this file.
 */
#include "hash.h"

/* Current nominal length of token buffer.
 */
static int maxtoken;

/* Nonzero if end-of-file has been seen on input.
 */
static int end_of_file;

/* Flag used for parsing `#line' directives.
 */
int is_pascal_source = TRUE;

/* An array with all long Pascal command-line switches.
 */
#define DEFINE_LANG_NAME(NAME)
#define GPC_OPT(SOURCE, NAME, DESCRIPTION) { SOURCE, NAME }
struct gpc_option gpc_options[] = {
#include "lang-options.h"
 { 0, NULL_PTR } };
#undef GPC_OPT
#undef DEFINE_LANG_NAME

/* Input stack for gpc_ungetc() below.
 */
#define UNGETC_STACK_SIZE 42
static char ungetc_stack[UNGETC_STACK_SIZE];
static int ungetc_stack_position = 0;

#ifdef HAVE_SIGALRM
/* Triggers for periodic progress output; set every
 * PROGRESS_TIME_INTERVAL microseconds.
 */
#define PROGRESS_TIME_INTERVAL 200000  /* 5 Hz */
static volatile int progress_message_alarm = 0;
static volatile int progress_bar_alarm = 0;
#endif /* HAVE_SIGALRM */


/* Lexical analyzer: Auxiliary functions.
 */

/* Prototypes.
 */
#ifdef HAVE_SIGALRM
static void alarm_handler PROTO ((int));
#endif /* HAVE_SIGALRM */
static void handle_progress_messages PROTO ((void));
static void handle_progress_bar PROTO ((void));
static inline int gpc_getc PROTO ((void));
static inline void gpc_ungetc PROTO ((char));

#ifdef HAVE_SIGALRM
/* Called periodically for outputting status messages.
 */
static void alarm_handler (int sig)
{
  progress_message_alarm = 1;
  progress_bar_alarm = 1;
  /* Install signal handler again if we have SYSV semantics.
     @@ It's probably better to use sigaction() (without SA_ONESHOT),
        but sigaction() is not available on every system, so we'd need
        an autoconf check (where???) */
  signal (sig, &alarm_handler);
  #ifdef linux
  siginterrupt (sig, 0);
  #endif
}
#endif /* HAVE_SIGALRM */

/* Non-inline subroutine of gpc_getc () below.
 */
static void
handle_progress_messages ()
{
#ifdef HAVE_SIGALRM
  if (progress_message_alarm)
    {
      fprintf (stderr, "#progress# %s (%d)\n", input_filename, lineno);
      progress_message_alarm = 0;
    }
#else /* not HAVE_SIGALRM */
  if (lineno % 256 == 0)
    fprintf (stderr, "#progress# %s (%d)\n", input_filename, lineno);
#endif /* not HAVE_SIGALRM */
}

/* Another non-inline subroutine of gpc_getc () below.
 */
static void
handle_progress_bar ()
{
  preprocessed_lineno++;
#ifdef HAVE_SIGALRM
  if (progress_bar_alarm)
    {
      fprintf (stderr, "#progress-bar# %d\n", preprocessed_lineno);
      progress_bar_alarm = 0;
    }
#else /* not HAVE_SIGALRM */
  if (preprocessed_lineno % 256 == 0)
    fprintf (stderr, "#progress-bar# %d\n", preprocessed_lineno);
#endif /* not HAVE_SIGALRM */
}

/* Read one character while handling `--debug-source',
 * `--progress-messages', and `--progress-bar'.
 */
static inline int
gpc_getc ()
{
  if (ungetc_stack_position)
    return (ungetc_stack[--ungetc_stack_position]);
  else
    {
      int ch = getc (finput);
      if (flag_debug_source && ch != EOF)
        fputc (ch, stderr);
      if (ch == '\n')
        {
          if (flag_progress_messages)
            handle_progress_messages ();
          if (flag_progress_bar)
            handle_progress_bar ();
        }
      return ch;
    }
}

/* Put one character back onto the input stack.
 * Since we are only gpc_ungetc()ing a *finite* (and small)
 * number of characters in sequence, there is no point in
 * bothering with dynamically growing stacks, but we use a
 * static buffer.
 */
static inline void
gpc_ungetc (ch)
     char ch;
{
  ungetc_stack[ungetc_stack_position++] = ch;
  if (ungetc_stack_position >= UNGETC_STACK_SIZE)
    abort ();
}

/* Initialize the lexical analyzer.
 */
void
init_lex ()
{
  /* Make identifier nodes long enough for the language-specific slots.
   */
  set_identifier_size (sizeof (struct lang_identifier));

  /* Start it at 0, because check_newline is called at the very beginning
   * and will increment it to 1.
   */
  lineno = 0;

#ifdef MULTIBYTE_CHARS
  /* Change to the native locale for multibyte conversions.
   */
  setlocale (LC_CTYPE, "");
#endif

  maxtoken = 40;
  token_buffer = (char *)xmalloc (maxtoken + 2);

#ifdef HAVE_SIGALRM
  /* Periodically trigger the output of progress messages.
   */
  if (flag_progress_messages || flag_progress_bar)
    {
      static struct itimerval timerval = { { 0, PROGRESS_TIME_INTERVAL },
                                           { 0, PROGRESS_TIME_INTERVAL } };
      signal (SIGALRM, &alarm_handler);
      #ifdef linux
      siginterrupt (SIGALRM, 0);
      #endif
      setitimer (ITIMER_REAL, &timerval, 0);
    }
#endif /* HAVE_SIGALRM */
}


static int
is_prefix_word (prefix, s)
     char *prefix;
     char *s;
{
  int l = strlen (prefix);
  return strncmp (prefix, s, l) == 0 && !(isalnum (s [l]) || s [l] == '_');
}

/* Handling compiler directives.
 */

/* Process compiler directives.  NAME is given in lower case, except
 * for strings and the argument to `{$M }'. Some directives have
 * already been handled by the preprocessor.
 * Return 0 if rest of directive shall be skipped, otherwise 1.
 */
static int
process_pascal_directive (name, length, next_char)
     char *name;
     int length;
     int next_char;
{
  int j, one_letter, temp;
  char *option, *option_name, *p;
  (void) default_options; /* Suppress a warning */
  if (PEDANTIC (U_B_D_PASCAL))
    {
      static int notified = 0;
      if (!notified)
        {
          notified = 1;
          pedwarn ("compiler directives are an UCSD Pascal extension");
        }
    }
  one_letter = length >= 2 && (name [1] == ' ' || name [1] == '\t' || name [1] == '\n' || name [1] == '\r');
  while (length > 0 &&
         (name [length - 1] == ' ' || name [length - 1] == '\t'
          || name [length - 1] == '\n' || name [length - 1] == '\r'))
    name [--length] = 0;
  if (length == 0)
    {
      warning ("empty Pascal compiler directive");
      return 1;
    }
  /* Directives already handled by preprocessor */
  if (is_prefix_word ("define", name)
      || is_prefix_word ("csdefine", name)
      || is_prefix_word ("cidefine", name)
      || is_prefix_word ("undef", name)
      || is_prefix_word ("include", name)
      || is_prefix_word ("if", name)
      || is_prefix_word ("ifdef", name)
      || is_prefix_word ("ifndef", name)
      || is_prefix_word ("ifopt", name)
      || is_prefix_word ("else", name)
      || is_prefix_word ("elif", name)
      || is_prefix_word ("endif", name))
    return 0;
  if (!strncmp (name, "debug-statement", temp = strlen ("debug-statement")))
    {
      tree temp_tree;
      name += temp;
      if (*name != '=' || !*++name)
        name = "Debugstatement";
      else
        {
          char *q;
          *name = toupper (*name);
          q = name;
          while (*++q)
            *q = tolower (*q);
        }
      debug_statement = get_identifier (name);
      temp_tree = lookup_name (debug_statement);
      if (!temp_tree || TREE_CODE (temp_tree) != FUNCTION_DECL)
        {
          error ("invalid debug statement `%s'", name);
          debug_statement = NULL_TREE;
        }
      return 1;
    }
  if (!strcmp (name, "no-debug-statement"))
    {
      debug_statement = NULL_TREE;
      return 1;
    }
  /* Allocate option_name on the heap because its value might be
   * used as the main program's name or such ... */
  if (one_letter && name [0] == 'w')
    option_name = concat ("-W", name + 2, NULL_PTR);
  else
    option_name = concat ("-f", name, NULL_PTR);
  /* From toplev.c:  If this is a language-specific option,
     decode it in a language-specific way. */
  p = option_name;
  while (*p && *p != '=' && *p != ':' && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
    p++;
  j = 0;
  while ((option = gpc_options[j].name) != NULL_PTR &&
         (strlen (option) != p - option_name ||
          strncmp (option, option_name, p - option_name) != 0))
    j++;
  if (option)
    {
      /* Do not accept pointless `$uses' directives. */
      if (strncmp (option_name, "-fuses", strlen ("-fuses")) == 0)
        {
          error ("`$uses' cannot be used as a directive - use `uses' clause instead");
          return 1;
        }
      /* Do not accept path names etc. in the source code. */
      if (!gpc_options[j].source)
        {
          error ("paths must not be specified in compiler directives");
          return 1;
        }
      if (!strncmp (option_name, "-fsetlimit", strlen ("-fsetlimit")))
        {
          warning ("internal problem: `setlimit' is not supported as a compiler directive");
          return 1;
        }
      LANG_DECODE_OPTION (option_name);
      return 1;
    }
  free (option);
  if (length == 2 && (name [1] == '+' || name [1] == '-'))
    {
      const struct gpc_short_option *short_option = gpc_short_options;
      while (short_option->short_name
             && (short_option->short_name != name [0]
                 || (!short_option->bp_option && (flag_what_pascal & U_B_D_PASCAL))))
        short_option++;
      if (short_option->short_name)
        {
          char *long_name;
          if (name [1] == '+')
            long_name = short_option->long_name;
          else
            long_name = short_option->inverted_long_name;
          if (long_name [1] == '!')
            {
              /* BP-style directive. In BP mode, ignore it for compatibility. */
              if (pedantic || !(flag_what_pascal & U_B_D_PASCAL))
                warning ("ignoring BP directive `{$%s}' which is unnecessary in GPC", name);
              return 1;
            }
          LANG_DECODE_OPTION (long_name);
          return 1;
        }
    }
  if (one_letter)
    switch (name [0])
      {
        /* BP-style one-letter directives with options. In BP mode, ignore them for compatibility. */
        case 'c':
        case 'd':
        case 'g':
        case 'o':
        case 'r':
        case 's':
          if (pedantic || !(flag_what_pascal & U_B_D_PASCAL))
            warning ("ignoring BP directive `{$%s}' which is unnecessary in GPC", name);
          return 1;
        /* One-letter directive already handled by preprocessor */
        case 'i':
          return 0;
        /* Message */
        case 'm':
          {
            /* Ignore `{$M 42, $42}' directives in BP mode for compatibility */
            if (flag_what_pascal & U_B_D_PASCAL)
              {
                p = name + 2;
                while ((*p >= '0' && *p <= '9') || *p == '$' || *p == ','
                       || *p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
                  p++;
                if (!*p)
                  return 1;
              }
            fprintf (stderr, "%s\n", name + 2);
            return 1;
          }
        /* Linker file specification */
        case 'l':
          {
            char *p, *filename = name + 2;
            while (*filename == ' ' || *filename == '\t'
                   || *filename == '\n' || *filename == '\r')
              filename++;
            if (next_char == ',')
              {
                gpc_ungetc (' ');
                gpc_ungetc ('l');
              }
            p = name + length - 1;
            while (p > filename && *p != '.')
              p--;
            if (*p != '.')
              {
                /* filename >= name + 2 ! */
                *--filename = 'l';
                *--filename = '-';
                add_to_link_file_list (filename);
              }
            else
              {
                /* Extension given.  Try to locate the file first
                   in `object_path', then in `unit_path'. */
                char *new_filename = locate_file (filename, object_path);
                if (! new_filename)
                  new_filename = locate_file (filename, unit_path);
                if (! new_filename)
                  {
                    error ("file `%s' not found", filename);
                    return 1;
                  }
                if (((p[1] != 'o' && p[1] != 'a') || p[2] != 0)
                    && (p[1] != 's' || p[2] != 'o' || p[3] != 0)
                    && (p[1] != 'l' || p[2] != 'n' || p[3] != 'k' || p[4] != 0))
                  {
                    /* Extension given, but not a linker input file.
                     * Recompile it with --automake, if necessary.
                     */
                    char p1 = p[1], p2 = p[2];
                    char *new_object_filename;
                    struct stat object_status, source_status;
                    if (stat (new_filename, &source_status) != 0)
                      source_status.st_mtime = 0;
                    p[1] = 'o';
                    p[2] = 0;
                    new_object_filename = locate_file (filename, object_path);
                    if (! new_object_filename)
                      new_object_filename = locate_file (filename, unit_path);
                    if (! new_object_filename
                        || stat (new_object_filename, &object_status) != 0)
                      object_status.st_mtime = 0;
                    if (flag_automake > 2
                        || (flag_automake > 1
                            && source_status.st_mtime != 0
                            && (object_status.st_mtime == 0
                                || object_status.st_mtime < source_status.st_mtime)))
                      {
                        p[1] = p1;
                        p[2] = p2;
                        if (compile_module (new_filename,
                                object_destination_path ? object_destination_path
                                                        : unit_destination_path) != 0)
                          error ("`%s' could not be compiled", new_filename);
                        /* compile_module() adds this to the automake temp file.
                         */
                        free (new_filename);
                        if (new_object_filename)
                          free (new_object_filename);
                        return 1;
                      }
                    /* Fall through and link the object file.
                     */
                    free (new_filename);
                    if (new_object_filename)
                      {
                        /* No need to recompile.
                         */
                        new_filename = new_object_filename;
                      }
                    else
                      {
                        /* We cannot or do not want to recompile.
                         * Nevertheless, try to link the object and not
                         * the source file.
                         */
                        new_filename = save_string (filename);
                      }
                  }
                add_to_link_file_list (new_filename);
                free (new_filename);
              }
            return 1;
          }
    }
  warning ("unknown compiler directive `%s'", name);
  return 1;
}

static void
do_comment (comment_type)
     int comment_type;
{
  int comment_count = 1, comment1 = 1, comment2 = 1, c, comment_is_directive, need_comma = 0;
  int directive_name_size = 256, directive_name_length = 0, is_m_directive = 0, is_l_directive = 0, first = 1;
  char in_string = 0, *directive_name = (char *) xmalloc (directive_name_size);
  *directive_name = 0;
  if (comment_type == 1)
    comment2 = flag_mixed_comments;
  else
    comment1 = flag_mixed_comments;
  c = gpc_getc ();
  comment_is_directive = c == '$';
  if (comment_is_directive)
    c = gpc_getc ();
  do
    {
      int is_white = c == ' ' || c == '\t' || c == '\n' || c == '\r';
      if (c == EOF)
        {
          error ("unterminated comment");
          break;
        }
      if (c == '\n')
        lineno++;
      if (in_string && c == in_string)
        in_string = 0;
      else if (!in_string && comment_count == 1 && (c == '"' || c == '\''))
        in_string = c;
      else if (!in_string && comment1 && c == '}')
        comment_count--;
      else if (!in_string && comment2 && c == '*')
        {
          int d = gpc_getc ();
          if (d == ')')
            comment_count--;
          else
            gpc_ungetc (d);
        }
      else if (!in_string && flag_nested_comments && comment1 && c == '{')
        comment_count++;
      else if (!in_string && flag_nested_comments && comment2 && c == '(')
        {
          int d = gpc_getc ();
          if (d == '*')
            comment_count++;
          else
            gpc_ungetc (d);
        }
      else if (!in_string && comment_count == 1 && flag_delphi_comments && (c == '/'))
        {
          int d = gpc_getc ();
          if (d == '/')
            comment_is_directive = 0;
          else
            gpc_ungetc (d);
        }
      else if (comment_count != 1 || !comment_is_directive
               || (directive_name_length == 0 && is_white))
        /* NOTHING */;
      else if (need_comma)
        {
          need_comma = 0;
          if (in_string || c != ',')
            comment_is_directive = 0;
        }
      /* `$local' and `$endlocal' are handled by the preprocessor.
         Ignore these words here, but process the following directives. */
      else if (!in_string && is_white && first && !strcmp (directive_name, "local"))
        {
          first = 0;
          directive_name_length = 0;
        }
      else if (in_string || is_m_directive || c != ',')
        {
          if (!in_string && !(is_m_directive || is_l_directive) && c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
          directive_name [directive_name_length++] = c;
          if (directive_name_length == 2)
            {
              is_m_directive = is_white && directive_name [0] == 'm';
              is_l_directive = is_white && directive_name [0] == 'l';
            }
          if (directive_name_length >= directive_name_size)
            directive_name = (char *) xrealloc (directive_name, directive_name_size *= 2);
          directive_name [directive_name_length] = 0;
        }
      else
        {
          comment_is_directive = process_pascal_directive (directive_name, directive_name_length, c);
          directive_name_length = 0;
          is_m_directive = 0;
          is_l_directive = 0;
          need_comma = c != ',';
          first = 0;
        }
      c = gpc_getc ();
    }
  while (comment_count != 0);
  gpc_ungetc (c);
  if (in_string)
    warning ("unterminated string in compiler directive");
  if (comment_is_directive && !need_comma && strcmp (directive_name, "endlocal") != 0)
    process_pascal_directive (directive_name, directive_name_length, 0);
  free (directive_name);
}

/* Skip white space, including comments,
 * and dispatch compiler directives.
 */
static int
skip_white_space (c)
     register int c;
{
  for (;;)
    {
      switch (c)
        {
        case '(':
          c = gpc_getc ();
          if (c != '*')
            {
              gpc_ungetc (c);
              return '(';
            }
          do_comment (2);
          c = gpc_getc ();
          break;

        case '{':
          do_comment (1);
          c = gpc_getc ();
          break;

        case '\n':
          c = check_newline ();
          break;

        case ' ':
        case '\t':
        case '\f':
        case '\r':
        case '\b':
          c = gpc_getc ();
          break;

        default:
          return c;
        }
    }
}


/* The lexical analzyer itself.
 */

/* Make the token buffer longer, preserving the data in it.
 * P should point to just beyond the last valid character in the old buffer.
 * The value we return is a pointer to the new buffer
 * at a place corresponding to P.
 */
static char *
extend_token_buffer (p)
     char *p;
{
  int offset = p - token_buffer;

  maxtoken = maxtoken * 2 + 10;
  token_buffer = (char *) xrealloc (token_buffer, maxtoken + 2);
  if (token_buffer == 0)
    fatal ("virtual memory exceeded");

  return token_buffer + offset;
}

/* At the beginning of a line, increment the line number
 * and handle a #line directive immediately following.
 */
int
check_newline ()
{
  register int c;
  register int token;
  register int this_can_be_a_char_constant = flag_borland_char_constants;

  lineno++;

  /* Read first nonwhite char on the line.
   */
  c = gpc_getc ();
  while (c == ' ' || c == '\t' || c == '\r')
    c = gpc_getc ();

  if (c != '#')
    {
      /* If not #, return it so caller will use it.
       */
      return c;
    }

  /* Read first nonwhite char after the `#'.
   */
  c = gpc_getc ();
  while (c == ' ' || c == '\t' || c == '\r')
    {
      this_can_be_a_char_constant = 0;
      c = gpc_getc ();
    }

  /* If a letter follows, then if the word here is `line', skip
   * it and ignore it; otherwise, ignore the line, with an error
   * if the word isn't `pragma', `ident', `define', or `undef'.
   */
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
      if (c == 'p')
        {
          if (gpc_getc () == 'r'
              && gpc_getc () == 'a'
              && gpc_getc () == 'g'
              && gpc_getc () == 'm'
              && gpc_getc () == 'a'
              && ((c = gpc_getc ()) == ' ' || c == '\t' || c == '\n' || c == '\r'))
            {
              if (pedantic)
                pedwarn ("ISO Pascal does not allow #pragma directive - ignored");
#if 0 /* jtv@hut.fi: No pragma code yet */
#ifdef HANDLE_SYSV_PRAGMA
              return handle_sysv_pragma (finput, c);
#endif /* HANDLE_SYSV_PRAGMA */
#ifdef HANDLE_PRAGMA
              HANDLE_PRAGMA (finput);
#endif /* HANDLE_PRAGMA */
#endif /* 0 */
              goto skipline;
            }
        }

      else if (c == 'd')
        {
          if (gpc_getc () == 'e'
              && gpc_getc () == 'f'
              && gpc_getc () == 'i'
              && gpc_getc () == 'n'
              && gpc_getc () == 'e'
              && ((c = gpc_getc ()) == ' ' || c == '\t' || c == '\n' || c == '\r'))
            {
              if (pedantic)
                pedwarn ("ISO Pascal does not allow #define directive");
#ifdef DWARF_DEBUGGING_INFO
              if ((debug_info_level == DINFO_LEVEL_VERBOSE)
                  && (write_symbols == DWARF_DEBUG))
                dwarfout_define (lineno, get_directive_line (finput));
#endif /* DWARF_DEBUGGING_INFO */
              goto skipline;
            }
        }
      else if (c == 'u')
        {
          if (gpc_getc () == 'n'
              && gpc_getc () == 'd'
              && gpc_getc () == 'e'
              && gpc_getc () == 'f'
              && ((c = gpc_getc ()) == ' ' || c == '\t' || c == '\n' || c == '\r'))
            {
              if (pedantic)
                pedwarn ("ISO Pascal does not allow #undef directive");
#ifdef DWARF_DEBUGGING_INFO
              if ((debug_info_level == DINFO_LEVEL_VERBOSE)
                  && (write_symbols == DWARF_DEBUG))
                dwarfout_undef (lineno, get_directive_line (finput));
#endif /* DWARF_DEBUGGING_INFO */
              goto skipline;
            }
        }
      else if (c == 'l')
        {
          if (gpc_getc () == 'i'
              && gpc_getc () == 'n'
              && gpc_getc () == 'e'
              && ((c = gpc_getc ()) == ' ' || c == '\t' || c == '\r'))
            {
              this_can_be_a_char_constant = 0;
              goto linenum;
            }
        }
      else if (c == 'i')
        {
          if (gpc_getc () == 'd'
              && gpc_getc () == 'e'
              && gpc_getc () == 'n'
              && gpc_getc () == 't'
              && ((c = gpc_getc ()) == ' ' || c == '\t' || c == '\r'))
            {
              if (pedantic)
                pedwarn ("ISO Pascal does not allow #ident directive");

              /* #ident.  The pedantic warning is now in cccp.c.  */

              /* Here we have just seen `#ident '.
                 A string constant should follow.  */

              while (c == ' ' || c == '\t' || c == '\r')
                c = gpc_getc ();

              /* If no argument, ignore the line.  */
              if (c == '\n')
                return c;

              gpc_ungetc (c);
              token = yylex ();
              if (token != LEX_STRCONST
                  || TREE_CODE (yylval.ttype) != STRING_CST)
                {
                  error ("invalid #ident");
                  goto skipline;
                }

              if (!flag_no_ident)
                {
#ifdef ASM_OUTPUT_IDENT
                  ASM_OUTPUT_IDENT (asm_out_file, TREE_STRING_POINTER (yylval.ttype));
#endif
                }

              /* Skip the rest of this line.  */
              goto skipline;
            }
        }

      error ("undefined or invalid # directive");
      goto skipline;
    }

linenum:
  /*
   * Here we have either `#line' or `#<nonletter>'.
   *
   * With `#line' it is clear that we are reading a line number;
   * with `#<nonletter>' it could be a Borland Pascal character
   * constant.  In all cases, a digit should follow.
   *
   * We interpret this as a line number if a whitespace follows.
   * (The preprocessor generates line number information of this kind.)
   * If the number follows immediately the `#', it is a character
   * constant unless this mechanism is disabled.
   *
   * This is a kludge.  It depends on the fact that gpcpp inserts
   * whitespace between the `#' and the number.
   */

  while (c == ' ' || c == '\t' || c == '\r')
    {
      this_can_be_a_char_constant = 0;
      c = gpc_getc ();
    }

  if (this_can_be_a_char_constant
      && (c == '$' || (c >= '0' && c <= '9')))
    {
      gpc_ungetc (c);
      return '#';
    }

  /* If the # is the only nonwhite char on the line,
   * just ignore it.  Check the new newline.
   */
  if (c == '\n')
    return c;

  /* Something follows the #; read a token.
   */
  gpc_ungetc (c);
  token = yylex ();

  if (token == LEX_INTCONST
      && TREE_CODE (yylval.ttype) == INTEGER_CST)
    {
      int old_lineno = lineno;
      int used_up = 0;

      /* Subtract one, because it is the following line that
       * gets the specified number.
       */
      int l = TREE_INT_CST_LOW (yylval.ttype) - 1;

      /* Is this the last nonwhite stuff on the line?
       */
      c = gpc_getc ();
      while (c == ' ' || c == '\t' || c == '\r')
        c = gpc_getc ();
      if (c == '\n')
        {
          /* No more: store the line number and check following line.
           */
          lineno = l;
          return c;
        }
      gpc_ungetc (c);

      /* More follows: it must be a string constant (filename).  */

      /* The #line directives are generated by cpp.
       */
      is_pascal_source = FALSE;

      token = yylex ();

      is_pascal_source = TRUE;

      if (token != LEX_STRCONST || TREE_CODE (yylval.ttype) != STRING_CST)
        {
          error ("invalid #line");
          goto skipline;
        }

      input_filename
        = (char *) permalloc (TREE_STRING_LENGTH (yylval.ttype) + 1);
      strcpy (input_filename, TREE_STRING_POINTER (yylval.ttype));
      lineno = l;

      /* Each change of file name
         reinitializes whether we are now in a system header.  */
      in_system_header = 0;

      if (main_input_filename == 0)
        main_input_filename = input_filename;

      /* Is this the last nonwhite stuff on the line?  */
      c = gpc_getc ();
      while (c == ' ' || c == '\t' || c == '\r')
        c = gpc_getc ();
      if (c == '\n')
        {
          /* Update the name in the top element of input_file_stack.  */
          if (input_file_stack)
            input_file_stack->name = input_filename;

          return c;
        }
      gpc_ungetc (c);

      token = yylex ();
      used_up = 0;

      /* `1' after file name means entering new file.
         `2' after file name means just left a file.  */

      if (token == LEX_INTCONST
          && TREE_CODE (yylval.ttype) == INTEGER_CST)
        {
          if (TREE_INT_CST_LOW (yylval.ttype) == 1)
            {
              /* Pushing to a new file.  */
              struct file_stack *p
                = (struct file_stack *) xmalloc (sizeof (struct file_stack));
              input_file_stack->line = old_lineno;
              p->next = input_file_stack;
              p->name = input_filename;
              input_file_stack = p;
              input_file_stack_tick++;
#ifdef DWARF_DEBUGGING_INFO
              if (debug_info_level == DINFO_LEVEL_VERBOSE
                  && write_symbols == DWARF_DEBUG)
                dwarfout_start_new_source_file (input_filename);
#endif /* DWARF_DEBUGGING_INFO */

              used_up = 1;
            }
          else if (TREE_INT_CST_LOW (yylval.ttype) == 2)
            {
              /* Popping out of a file.  */
              if (input_file_stack->next)
                {
                  struct file_stack *p = input_file_stack;
                  input_file_stack = p->next;
                  free (p);
                  input_file_stack_tick++;
#ifdef DWARF_DEBUGGING_INFO
                  if (debug_info_level == DINFO_LEVEL_VERBOSE
                      && write_symbols == DWARF_DEBUG)
                    dwarfout_resume_previous_source_file (input_file_stack->line);
#endif /* DWARF_DEBUGGING_INFO */
                }
              else
                error ("#-lines for entering and leaving files don't match");

              used_up = 1;
            }
        }

      /* Now that we've pushed or popped the input stack,
         update the name in the top element.  */
      if (input_file_stack)
        input_file_stack->name = input_filename;

      /* If we have handled a `1' or a `2',
         see if there is another number to read.  */
      if (used_up)
        {
          /* Is this the last nonwhite stuff on the line?  */
          c = gpc_getc ();
          while (c == ' ' || c == '\t' || c == '\r')
            c = gpc_getc ();
          if (c == '\n')
            return c;
          gpc_ungetc (c);

          token = yylex ();
          used_up = 0;
        }

      /* `3' after file name means this is a system header file.  */

      if (token == LEX_INTCONST
          && TREE_CODE (yylval.ttype) == INTEGER_CST
          && TREE_INT_CST_LOW (yylval.ttype) == 3)
        in_system_header = 1;
    }
  else
    error ("invalid #-line");

  /* skip the rest of this line.  */
 skipline:
  if (c == '\n')
    return c;
  while ((c = gpc_getc ()) != EOF && c != '\n');
  return c;
}

#define ENDFILE -1  /* token that represents end-of-file */

/* Read an escape sequence, returning its equivalent as a character,
 * or store 1 in *ignore_ptr if it is backslash-newline.
 */
static int
readescape (ignore_ptr)
     int *ignore_ptr;
{
  register int c = gpc_getc ();
  register int code;
  register unsigned count;
  unsigned firstdig = 0;
  int nonnull;

  switch (c)
    {
    case 'x':
      if (warn_traditional)
        warning ("the meaning of `\\x' varies with -traditional");

      if (flag_traditional)
        return c;

      code = 0;
      count = 0;
      nonnull = 0;
      while (1)
        {
          c = gpc_getc ();
          if (!(c >= 'a' && c <= 'f')
              && !(c >= 'A' && c <= 'F')
              && !(c >= '0' && c <= '9'))
            {
              gpc_ungetc (c);
              break;
            }
          code *= 16;
          if (c >= 'a' && c <= 'f')
            code += c - 'a' + 10;
          if (c >= 'A' && c <= 'F')
            code += c - 'A' + 10;
          if (c >= '0' && c <= '9')
            code += c - '0';
          if (code != 0 || count != 0)
            {
              if (count == 0)
                firstdig = code;
              count++;
            }
          nonnull = 1;
        }
      if (! nonnull)
        error ("\\x used with no following hex digits");
      else if (count == 0)
        /* Digits are all 0's.  Ok.  */
        ;
      else if ((count - 1) * 4 >= TYPE_PRECISION (integer_type_node)
               || (count > 1
                   && ((1 << (TYPE_PRECISION (integer_type_node) - (count - 1) * 4))
                       <= firstdig)))
        pedwarn ("hex escape out of range");
      return code;

    case '0':  case '1':  case '2':  case '3':  case '4':
    case '5':  case '6':  case '7':
      code = 0;
      count = 0;
      while ((c <= '7') && (c >= '0') && (count++ < 3))
        {
          code = (code * 8) + (c - '0');
          c = gpc_getc ();
        }
      gpc_ungetc (c);
      return code;

    case '\\': case '\'': case '"':
      return c;

    case '\n':
      lineno++;
      *ignore_ptr = 1;
      return 0;

    case 'n':
      return TARGET_NEWLINE;

    case 't':
      return TARGET_TAB;

    case 'r':
      return TARGET_CR;

    case 'f':
      return TARGET_FF;

    case 'b':
      return TARGET_BS;

    case 'a':
      if (warn_traditional)
        warning ("the meaning of `\\a' varies with -traditional");

      if (flag_traditional)
        return c;
      return TARGET_BELL;

    case 'v':
#if 0 /* Vertical tab is present in common usage compilers.  */
      if (flag_traditional)
        return c;
#endif
      return TARGET_VT;

    case 'e':
    case 'E':
      pedwarn ("non-ANSI-standard escape sequence, `\\%c'", c);
      return 033;

    case '?':
      return c;

      /* `\(', etc, are used at beginning of line to avoid confusing Emacs.  */
    case '(':
    case '{':
    case '[':
      /* `\%' is used to prevent SCCS from getting confused.  */
    case '%':
      if (pedantic)
        pedwarn ("non-ANSI escape sequence `\\%c'", c);
      return c;
    }
  if (c >= 040 && c < 0177)
    pedwarn ("unknown escape sequence `\\%c'", c);
  else
    pedwarn ("unknown escape sequence: `\\' followed by char code 0x%x", c);
  return c;
}

/* Hook for parse.y: error handling.
 */
void
yyerror (string)
#ifdef EGCS92
     const char *string;
#else /* not EGCS92 */
     char *string;
#endif /* not EGCS92 */
{
  char buf[200];

  strcpy (buf, string);

  /* We can't print string and character constants well
     because the token_buffer contains the result of processing escapes.  */
  if (end_of_file)
    strcat (buf, " at end of input");
  else if (token_buffer[0] == 0)
    strcat (buf, " at null character");
  else if (token_buffer[0] == '"' || token_buffer[0] == '\'')
    strcat (buf, " before string constant");
  else if (token_buffer[0] < 040 || (unsigned char) token_buffer[0] >= 0177)
    sprintf (buf + strlen (buf), " before character 0%o",
             (unsigned char) token_buffer[0]);
  else
    strcat (buf, " before `%s'");

  error (buf, token_buffer);
}

/* If we scan tokens:
 *   INTEGER..      : we must return INTEGER now, and LEX_RANGE maybe next time.
 *   INTEGER.DIGIT  : we have a floating point number
 *   INTEGER.others : we have an error
 */
static int have_DOTS = 0;

/* Skips all of the white space at the current location in the input file.
 */
void
position_after_white_space ()
{
  gpc_ungetc (skip_white_space (gpc_getc ()));
}

static char *exp_store_loc;

static void
store_exp (what)
     int what;
{
  if (what > 9)
    store_exp (what/10);

  *exp_store_loc++ = (what%10)+'0';
}

/* Pack the valid floating point number starting from start
 * by skipping the leading fractional zeroes if the mantissa
 * is zero.  Return the adjust value.
 */
static int
compress_float (start, end)
     char *start;
     char **end;
{
  int adjust  = 0;
  char *first = start;

  while (*first == '0')
    first++;
  if (*first == '.')
    adjust = -1;
  else
    return 0;   /* Nonzero mantissa */

  first++;      /* Skip '.' */

  while (*first == '0')
    {
      first++;
      adjust--;
    }

  if (! *first) /* A zero */
    return 0;

  *start++ = *first++;
  *start++ = '.';

  if (! *first)
    *start++ = '0';
  else
    while ((*start++ = *first++));

  *--start = '\000';
  *end = start;

  return adjust;
}

/* If the declaration found has its PASCAL_REDEFINABLE_DECL attribute
 * bit set, it has the "real" type of the node in it's TREE_TYPE field
 * (if it is a predefined type name, it has TYPE_DECL node there;
 * others have integer_type_node (to prevent crashes in gpc-decl.c)
 * and they have the actual value to return in DECL_INITIAL (decl)
 *
 * But this only returns the TREE_TYPE field or NULL_TREE if it is not
 * a type.
 */
tree
check_if_predefined_type (id)
     tree id;
{
  tree pdef_type = lookup_name (id);

  if (pdef_type != 0
      && TREE_CODE (pdef_type) == VAR_DECL
      && PASCAL_REDEFINABLE_DECL (pdef_type))
    {
      if (TREE_TYPE (pdef_type) != NULL_TREE          /* type specified and */
          && DECL_INITIAL (pdef_type) == NULL_TREE)   /* not a constant */
        pdef_type = TREE_TYPE (pdef_type);
      else
        pdef_type = NULL_TREE;
    }

  return pdef_type;
}

/* Return a string describing the dialects selected by `id'
 * (as in `flag_what_pascal').
 */
char *
which_language (id)
     int id;
{
  switch (id) {
  case STANDARD_PASCAL_LEVEL_0:
         return "ISO 7185 Standard Pascal, level 0";
  case STANDARD_PASCAL_LEVEL_1:
         return "ISO 7185 Standard Pascal, level 1";
  case EXTENDED_PASCAL:
         return "ISO 10206 Extended Pascal";
  case OBJECT_PASCAL:
         return "Object Extensions to ISO 10206 Extended Pascal";
  case UCSD_PASCAL:
         return "UCSD Pascal";
  case BORLAND_PASCAL:
         return "Borland Pascal";
  case BORLAND_DELPHI:
         return "Borland's Object Pascal (Delphi)";
  case PASCAL_SC:
         return "Pascal Extensions for Scientific Calculations (Pascal-SC)";
  case VAX_PASCAL:
         return "VAX Pascal";
  case GNU_PASCAL:
         return "GNU Pascal";
  default:
         return "Internal GPC problem: unknown Pascal dialect";
  }
}

/* Output a list of warnings (or errors, if ERRORS is nonzero)
 * describing the dialects selected by `iclass' (as in `flag_what_pascal').
 */
void
warning_list_languages (iclass, errors)
     unsigned long iclass;
     int errors;
{
  unsigned long mask = 1;

  /* Handle inclusions:
   * Standard 0 <= Standard 1 <= Extended <= Object <= GNU
   * Borland Pascal <= Borland Delphi <= GNU
   * GNU <= GNU, extended syntax
   */
  if (iclass & STANDARD_PASCAL_LEVEL_0)
    iclass &= ~(STANDARD_PASCAL_LEVEL_1 | E_O_G_PASCAL);
  else if (iclass & STANDARD_PASCAL_LEVEL_1)
    iclass &= ~E_O_G_PASCAL;
  else if (iclass & EXTENDED_PASCAL)
    iclass &= ~O_G_PASCAL;
  else if (iclass & OBJECT_PASCAL)
    iclass &= ~GNU_PASCAL;

  if (iclass & BORLAND_PASCAL)
    iclass &= ~D_G_PASCAL;
  else if (iclass & BORLAND_DELPHI)
    iclass &= ~GNU_PASCAL;

  do
    {
      if (iclass & mask)
        {
          if (errors)
            error (" - %s", which_language (mask));
          else
            warning (" - %s", which_language (mask));
        }
      mask <<= 1;
    }
  while (mask && mask <= DIALECT_MASK_MAX);
}

/* Report reserved words in some other language level that
 * are used as identifiers if pedantic option is given.
 * Complain about reserved words in the current language level
 * that are used as identifieres even without pedantic option.
 */
void
warn_about_keyword_redeclaration (kw)
     char *kw;
{
  struct resword *resword_ptr;
  resword_ptr = is_reserved_word (kw, strlen (kw));

  /* If this is a new identifier in a pointer forward reference
   * it is not yet clear that we are redeclaring it.
   */
  if (defining_pointers)
    return;

  if (resword_ptr && !resword_ptr->informed)
    {
      resword_ptr->informed = 1;
      if ((flag_what_pascal & resword_ptr->iclass)
          || flag_pedantic_errors)
        {
          error ("`%s' is a reserved word in:", kw);
          warning_list_languages (resword_ptr->iclass, 1);
        }
      else if (pedantic)
        {
          warning ("`%s' is a reserved word in:", kw);
          warning_list_languages (resword_ptr->iclass, 0);
        }
    }
}

/* Enable/disable keywords.  Using this mechanism we can avoid a lot of
 * shift/reduce and reduce/reduce conflicts:  Keywords will simply fall
 * through and become a new_identifier if not applied in a certain context.
 */
void
enable_keyword (kw)
     char *kw;
{
  register struct resword *resword_ptr;
  resword_ptr = is_reserved_word (kw, strlen (kw));
  if (! resword_ptr)
    abort ();
  /*
   * Do not enable keywords of dialects unselected by command-line options.
   */
  if (! flag_what_pascal
      || (flag_what_pascal & resword_ptr->iclass))
    {
      resword_ptr->disabled = 0;
      if (yydebug)
        fprintf (stderr, "Keyword \"%s\" enabled\n", kw);
    }
}

void
disable_keyword (kw)
     char *kw;
{
  register struct resword *resword_ptr;
  if (yydebug)
    fprintf (stderr, "Keyword \"%s\" disabled\n", kw);
  resword_ptr = is_reserved_word (kw, strlen (kw));
  if (! resword_ptr)
    abort ();
  resword_ptr->disabled = 1;
}

int
keyword_enabled (kw)
     char *kw;
{
  register struct resword *resword_ptr;
  resword_ptr = is_reserved_word (kw, strlen (kw));
  if (! resword_ptr)
    abort ();
  return ! resword_ptr->disabled;
}

/* Initialize the keyword table for the language dialect specified
 * in the command-line options.  For example, disable Borland Pascal
 * reserved words when -fstandard-pascal is given.
 *
 * We set `disabled' to 2 to indicate why this keyword has been
 * disabled, so the same procedure can undo it later.
 */
void
init_language_specification ()
{
  struct resword *resword_ptr = wordlist + MIN_HASH_VALUE;
  int i;
  for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; i++)
    {
      if (resword_ptr->token)
        {
          if (flag_what_pascal
              && (resword_ptr->iclass & flag_what_pascal) == 0
              && resword_ptr->disabled == 0)
            resword_ptr->disabled = 2;
          else if (resword_ptr->disabled == 2)
            resword_ptr->disabled = 0;
        }
      resword_ptr++;
    }
}

/* ~1 means to warn about all underscores without -pedantic.
 */
static int note_underscore = ~1;

/* The main function of the lexical analyzer, as called from the parser.
 */
int
yylex ()
{
  register int c;
  register char *p;
  register int value;
  int wide_flag = 0;
  struct resword *resword_ptr;

  int check_known_ids = 1;
  int underscore_rep = 0;

  /* Zero means to allow GPC to read ISO-style
   * non-10-based numbers `16#deadbeef'.
   */
  static int baseflag = 0;

  /* Now it really starts.  Read one character.
   */
  c = gpc_getc ();

  /* Effectively do c = skip_white_space (c)
   * but do it faster in the usual cases.
   */
  while (1)
    switch (c)
      {
      case ' ':
      case '\t':
      case '\f':
      case '\v':
      case '\r':
      case '\b':
        c = gpc_getc ();
        break;

      case '\n':
      case '(':
      case '{':
      case '\\':
        c = skip_white_space (c);
      default:
        goto found_nonwhite;
      }
 found_nonwhite:

  token_buffer[0] = c;
  token_buffer[1] = 0;

  switch (c)
    {
    case EOF:
      end_of_file = 1;
      token_buffer[0] = 0;
      value = ENDFILE;
      break;

    case '#':  /* Handle Borland Pascal character constants:  #32 == ' ' */
      if (flag_borland_char_constants)
        {
          char code;
          int token;
          baseflag++;
          token = yylex ();
          baseflag--;
          if (PEDANTIC (B_D_PASCAL))
            pedwarn ("char constants with `#' are a Borland Pascal extension");
          if (token != LEX_INTCONST)
            {
              error ("non-numeric token following `#' - ignoring the `#'");
              value = token;
              break;
            }
          value = CHAR_LITERAL;
          if (int_cst_lt_always (yylval.ttype,
                                 TYPE_MIN_VALUE (char_type_node))
              || int_cst_lt_always (TYPE_MAX_VALUE (char_type_node),
                                    yylval.ttype))
            error ("numeric constant out of range for character constant");
          code = TREE_INT_CST_LOW (yylval.ttype);
          /* If char type is signed, sign-extend the constant.  */
          if (TREE_UNSIGNED (char_type_node)
              || ((code >> (BITS_PER_UNIT - 1)) & 1) == 0)
            yylval.ttype = build_int_2 (code & ((1 << BITS_PER_UNIT) - 1), 0);
          else
            yylval.ttype = build_int_2 (code | ((-1) << BITS_PER_UNIT), -1);
          TREE_TYPE (yylval.ttype) = char_type_node;
          break;
        }

    case '^':  /* Handle Borland Pascal character constants:  ^I == '\t' */
      {
        char code = gpc_getc ();
        char ucode = toupper (code);
        char nextcode = gpc_getc ();
        if (lex_caret
            && ucode >= 'A'
            && ucode <= 'Z'
            && ! (isalnum (nextcode) || nextcode == '_'))
          {
            nextcode = skip_white_space (nextcode);
            if (nextcode == '.')
              {
                char nextnextcode = gpc_getc ();
                if (nextnextcode == '.')
                  {
                    /* This is the lower bound of a subrange, not a pointer type.
                     * Make the upper bound become recognized, too.
                     *
                     * lex_caret will be restored by the caller (parse.y).
                     */
                    lex_caret = 0;
                  }
                gpc_ungetc (nextnextcode);
              }
          }
        gpc_ungetc (nextcode);
        if (flag_borland_char_constants && ! lex_caret)
          {
            if (ucode < 'A' || ucode > 'Z' || isalnum (nextcode) || nextcode == '_')
              {
                gpc_ungetc (code);
                value = '^';
                break;
              }
            if (PEDANTIC (B_D_PASCAL))
              pedwarn ("char constants with `^' are a Borland Pascal extension");
            value = CHAR_LITERAL;
            ucode -= 'A' - 1;
            /* If char type is signed, sign-extend the constant.  */
            if (TREE_UNSIGNED (char_type_node)
                || ((code >> (BITS_PER_UNIT - 1)) & 1) == 0)
              yylval.ttype = build_int_2 (ucode & ((1 << BITS_PER_UNIT) - 1), 0);
            else
              yylval.ttype = build_int_2 (ucode | ((-1) << BITS_PER_UNIT), -1);
            TREE_TYPE (yylval.ttype) = char_type_node;
            break;
          }
        else
          {
            gpc_ungetc (code);
            value = '^';
            break;
          }
      }

    case '$':
      {
        if (dollars_in_ident)
          goto letter;
        else
          {
            int ch = gpc_getc ();
            gpc_ungetc (ch);
            switch (ch)
              {
                case '0':  case '1':  case '2':  case '3':  case '4':
                case '5':  case '6':  case '7':  case '8':  case '9':
                case 'A':  case 'B':  case 'C':  case 'D':  case 'E':  case 'F':
                case 'a':  case 'b':  case 'c':  case 'd':  case 'e':  case 'f':
                  goto digit;
                default:
                  value = '$';
                  goto done;
              }
          }
      }

    case 'A':  case 'B':  case 'C':  case 'D':  case 'E':
    case 'F':  case 'G':  case 'H':  case 'I':  case 'J':
    case 'K':  case 'L':  case 'M':  case 'N':  case 'O':
    case 'P':  case 'Q':  case 'R':  case 'S':  case 'T':
    case 'U':  case 'V':  case 'W':  case 'X':  case 'Y':
    case 'Z':
    case 'a':  case 'b':  case 'c':  case 'd':  case 'e':
    case 'f':  case 'g':  case 'h':  case 'i':  case 'j':
    case 'k':  case 'l':  case 'm':  case 'n':  case 'o':
    case 'p':  case 'q':  case 'r':  case 's':  case 't':
    case 'u':  case 'v':  case 'w':  case 'x':  case 'y':
    case 'z':
    case '_':
    letter:
      {
        char prev = '\000';
        p = token_buffer;
        while (isalnum (c) || c == '_' || c == '$')
          {
            if (c == '_')
              {
                if (pedantic && (note_underscore & 1))
                  underscore_rep |= 1;

                if (! prev && (note_underscore & 2))
                  underscore_rep |= 2;

                if (prev == '_' && (note_underscore & 4))
                  underscore_rep |= 4;
              }
            if (p >= token_buffer + maxtoken)
              p = extend_token_buffer (p);

            if (p == token_buffer)
              {
                if (islower (c))
                  c = toupper (c); /* all words start with a CAPITAL letter */
              }
            else if (isupper (c))
              c = tolower(c);      /* Leave only the first letter upper case */

            *p++ = prev = c;
            c = gpc_getc ();
          }
        if (prev == '_' && (note_underscore & 8))
          underscore_rep |= 8;
      }
      *p = 0;
      gpc_ungetc (c);

      value = LEX_ID;
      yylval.itype = 0;

      /* Recognize reserved words
       */
      resword_ptr = is_reserved_word (token_buffer, p - token_buffer);
      if (resword_ptr && resword_ptr->disabled == 0)
        {
          yylval.ttype = get_identifier (token_buffer);
          value = (int) resword_ptr->token;
        }

      /* If we did not find a keyword, look for an identifier declaration
       */
      if (value == LEX_ID)
        {
          yylval.ttype = get_identifier (token_buffer);
          lastiddecl = lookup_name (yylval.ttype);

          /* If the declaration found has its PASCAL_REDEFINABLE_DECL attribute
           * bit set, it has the "real" type of the node in it's TREE_TYPE field
           * (if it is a predefined type name, it has TYPE_DECL node there;
           * others have integer_type_node (to prevent crashes in gpc-decl.c)
           * and they have the actual value to return in DECL_INITIAL (decl)
           */

          if (check_known_ids)
            {
              /* @@@  HP Snake fails pvs 046 if I don't check
               * that it actually is a VAR_DECL.  So something sets
               * the redef_decl bit when it should not be set???
               * (For a component_ref at line 22 of 046 -> signal 11)
               */
              if (lastiddecl != 0
                  && TREE_CODE (lastiddecl) == VAR_DECL
                  && PASCAL_REDEFINABLE_DECL (lastiddecl))
                {
                  if (TREE_TYPE (lastiddecl) != NULL_TREE &&
                      DECL_INITIAL (lastiddecl) == NULL_TREE)
                      /* type specified && not a constant */
                    lastiddecl = TREE_TYPE (lastiddecl);
                  else
                    value = TREE_INT_CST_LOW (DECL_INITIAL (lastiddecl));
                }
            }

          if (underscore_rep && value == LEX_ID && lastiddecl == NULL_TREE
            && ((flag_what_pascal & B_D_PASCAL) == 0))
            {
              if (underscore_rep & 1)
                pedwarn ("ISO 7185 Standard Pascal does not allow underscores in identifiers");
              if (underscore_rep & 2)
                pedwarn ("identifiers should not start with an underscore");
              if (underscore_rep & 4)
                pedwarn ("identifiers should not contain two adjacent underscores");
              if (underscore_rep & 8)
                pedwarn ("identifiers should not end with an underscore");

              /* Give only one each compilation */
              note_underscore &= ~underscore_rep;
            }
        }

      /* Special guest: LEX_PROTECTED.
       * It is no keyword when a ',' or ':' or `;' follows.
       * With this hack we can handle ISO 7185 as well as ISO 10206.
       *
       * The same holds for LEX_VALUE, LEX_STATIC, and LEX_VIEW.
       *
       * The same holds for LEX_INLINE, but it is not required by
       * any known standard.
       */
      if (value == LEX_PROTECTED
          || value == LEX_VALUE
          || value == LEX_STATIC
          || value == LEX_VIEW
          || value == LEX_INLINE)
        {
          char nextchar = skip_white_space (gpc_getc ());
          if (nextchar == ',' || nextchar == ':' || nextchar == ';')
            value = LEX_ID;
          gpc_ungetc (nextchar);
        }

      if (value == LEX_VALUE || value == LEX_ABSOLUTE)
        lex_const_equal = -1;

      break;

    case '(':
      c = gpc_getc ();
      if (c == '.') {
          c = gpc_getc ();
          gpc_ungetc (c);
          if (c == '.') {       /* Allow all params to be unknown (...) */
              have_DOTS = 1;
              value = '(';
          } else {
              token_buffer[1] = '.';
              token_buffer[2] = 0;
              value = '[';
          }
      } else {
          gpc_ungetc (c);
          value = '(';
      }
      break;

    case '.':
      if (have_DOTS == 0)
          c = gpc_getc ();
      else
          have_DOTS = 0;
      switch (c) {
      case '.':
          token_buffer[1] = '.';
          c = gpc_getc ();
          if (c == '.') {
              token_buffer[2] = '.';
              token_buffer[3] = 0;
              value = LEX_ELLIPSIS;
          } else {
              gpc_ungetc (c);
              token_buffer[2] = 0;
              value = LEX_RANGE;
          }
          break;
      case ')':
          value = ']';
          token_buffer[1] = c;
          token_buffer[2] = 0;
          break;
      default:
          gpc_ungetc (c);
          value = '.';
          break;
      }
      break;

    case '0':  case '1':  case '2':  case '3':  case '4':
    case '5':  case '6':  case '7':  case '8':  case '9':
    digit:
      {
        int base = 10;
        int count = 0;
        int largest_digit = 0;
        int numdigits = 0;
        char nextchar = 0;
        /* for multi-precision arithmetic,
           we store only 8 live bits in each short,
           giving us 64 bits of reliable precision */
        short shorts[8];
        int baseflag_save = baseflag;
        int floatflag = 0;  /* Set 1 if we learn this is a floating constant */

        for (count = 0; count < 8; count++)
          shorts[count] = 0;

        if (c == '$')
          {
            baseflag++;
            base = 16;
            c = gpc_getc ();
            if (PEDANTIC (B_D_PASCAL))
              pedwarn ("hexadecimal numbers with `$' are a Borland Pascal extension");
          }

        p = token_buffer;
        *p++ = c;

        /* Read all the digits.  */

        while (isalnum (c) || (c == '.' && floatflag == 0) ||
               (c == '#' && baseflag++ == 0)) {
            if (isdigit (c)) {
                c = c - '0';
            } else if (c == '.') {
                c = gpc_getc ();
                if (c == '.') {
                    have_DOTS = 1;
                    gpc_ungetc (c); /* only peek at character after . */
                    break;      /* INTEGER.. is not floating point */
                } else {
                    if (base != 10)
                        warning ("real constants must be in radix 10");
                    base = 10;
                    if (! isdigit(c)) {
                      if (c == ')')             /* '.)' == ']' */
                        {
                          nextchar = '.';   /* we just missed one */
                          break;            /* Do gpc_ungetc() later  */
                        }
                      gpc_ungetc (c);
                      error ("ISO Pascal requires a digit after decimal point");
                      c = '0'; /* assume zero was present */
                    }
                    *p++ = c;
                    c -= '0';
                    floatflag = 1;
                }
            } else if (c == '#') {
                if (PEDANTIC (E_O_PASCAL))
                  pedwarn ("`radix#value' is an ISO 10206 Extended Pascal extension");
                /* Using Extended Pascals way to input values in different
                 * bases:
                 *     base#value
                 * Base may be in range 2..36
                 */
                base = (shorts[3]<<24) + (shorts[2]<<16) + (shorts[1]<<8) + shorts[0];
                if ((base < 2 || base > 36) ||
                    shorts[7] | shorts[6] | shorts[5] | shorts[4]) {
                    warning ("base value out of range; assuming base 10");
                    base = 10;
                }
                for (count = 0; count < 8; count++)
                    shorts[count] = 0;
                largest_digit = 0;
                numdigits = 0;
                *p++ = (c = gpc_getc ());
                continue;
            } else if (base <= 10) {
                if (base == 10 && ((c == 'E') || (c == 'e'))) {
                    floatflag = 1;
                    break;   /* start of exponent */
                }
                error ("nondigits in number whose radix <= 10");
                c = 0;
            } else if (c >= 'a') {
                c = c - 'a' + 10;
            } else {
                c = c - 'A' + 10;
            }
            if (c >= largest_digit)
                largest_digit = c;
            numdigits++;

            if (floatflag == 0)
              {
                for (count = 0; count < 8; count++)
                  {
                    shorts[count] *= base;
                    if (count)
                      {
                        shorts[count] += (shorts[count-1] >> 8);
                        shorts[count-1] &= (1<<8)-1;
                      }
                    else
                      shorts[0] += c;
                  }
                if (shorts[7] > 0xFF)
                  {
                    error ("value does not fit in longest integer type");
                    /* Avoid further error messages. */
                    for (count = 0; count < 8; count++)
                      shorts[count] = 0;
                  }
              }

            if (p >= token_buffer + maxtoken - 3)
                p = extend_token_buffer (p);
            *p++ = (c = gpc_getc ());
        }

        if (c == '.' && have_DOTS == 0)
            error("Only one decimal point in real number allowed");

        if (numdigits == 0)
          error ("numeric constant with no digits");

        if (largest_digit >= base)
          error ("numeric constant contains digits beyond the radix");

        /* Remove terminating char from the token buffer
           and delimit the string */
        *--p = 0;

        if (floatflag)
          {
            tree type = long_double_type_node;
            int esign   = 1;
            int expon   = 0;
            char *temp  = p;

            /* Compress out the leading zeros by adjusting the exponent */
            int adjust_exp = compress_float (token_buffer, &temp);

            p = temp;

            /* Read explicit exponent if any, and put it in tokenbuf.  */

            if ((c == 'e') || (c == 'E'))
              {
                if (p >= token_buffer + maxtoken - 3)
                  p = extend_token_buffer (p);
                *p++ = c;
                c = gpc_getc ();
                if ((c == '+') || (c == '-'))
                  {
                    if (c == '-')
                      esign = -1;
                    c = gpc_getc ();
                  }
                if (! isdigit (c))
                  error ("real constant exponent has no digits");
                while (isdigit (c))
                  {
                    expon = 10 * expon + c - '0';
                    c = gpc_getc ();
                  }
                expon = esign * expon + adjust_exp;
                if (expon < 0)
                  {
                    *p++ = '-';
                    expon = - expon;
                  }
                exp_store_loc = p;
                store_exp (expon);
                p = exp_store_loc;
              }
            else if (adjust_exp)
              {
                *p++ = 'E';
                if (adjust_exp < 0)
                  {
                    *p++ = '-';
                    adjust_exp = -adjust_exp;
                  }
                exp_store_loc = p;
                store_exp (adjust_exp);
                p = exp_store_loc;
              }

            *p = 0;
            yylval.ttype = build_real (type,
                                       REAL_VALUE_ATOF (token_buffer,
                                                        TYPE_MODE (type)));
#if 0
            if (REAL_VALUE_ISINF (yylval.ttype))
              {
                type = long_double_type_node;
                yylval.ttype = build_real (type,
                                           REAL_VALUE_ATOF (token_buffer,
                                                            TYPE_MODE (type)));
                if (REAL_VALUE_ISINF (yylval.ttype))
                  error ("real constant out of range");
              }
#endif

            if (isalnum (c)) {
                error ("garbage at end of number");
                while (isalnum (c)) {
                    if (p >= token_buffer + maxtoken - 3)
                        p = extend_token_buffer (p);
                    *p++ = c;
                    c = gpc_getc ();
                }
            }

            gpc_ungetc (c);
            if (nextchar)
              gpc_ungetc (nextchar);
            *p = 0;

            TREE_TYPE (yylval.ttype) = type;
            value = LEX_REALCONST;
          }
        else
          {
            tree type;

            if (have_DOTS == 0) { /* already did gpc_ungetc for have_DOTS */
                if (isalnum (c)) {
                    error ("garbage at end of number");
                    while (isalnum (c)) {
                        if (p >= token_buffer + maxtoken - 3)
                            p = extend_token_buffer (p);
                        *p++ = c;
                        c = gpc_getc ();
                    }
                }
                gpc_ungetc (c);
            }
            *p = 0;

            /* This is simplified by the fact that our constant
             * is always positive.
             */
#if HOST_BITS_PER_WIDE_INT <= 32
            yylval.ttype
              = build_int_2 ((shorts[3]<<24) + (shorts[2]<<16) + (shorts[1]<<8) + shorts[0],
                             (shorts[7]<<24) + (shorts[6]<<16) + (shorts[5]<<8) + shorts[4]);
#else /* HOST_BITS_PER_WIDE_INT > 32 */
            yylval.ttype
              = build_int_2 (((HOST_WIDE_INT) shorts[7] << 56)
                             + ((HOST_WIDE_INT) shorts[6] << 48)
                             + ((HOST_WIDE_INT) shorts[5] << 40)
                             + ((HOST_WIDE_INT) shorts[4] << 32)
                             + ((HOST_WIDE_INT) shorts[3] << 24)
                             + ((HOST_WIDE_INT) shorts[2] << 16)
                             + ((HOST_WIDE_INT) shorts[1] << 8)
                             + shorts[0], 0);
#endif /* HOST_BITS_PER_WIDE_INT > 32 */

            PASCAL_TREE_FRESH_INT_CST (yylval.ttype) = 1;

            /* This integer will be marked as being input by the user
             * program, so its type does not really matter.  However,
             * assign it something reasonable.
             */
            if (! INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (integer_type_node), yylval.ttype))
              type = integer_type_node;
            else if (! INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (unsigned_type_node), yylval.ttype))
              type = unsigned_type_node;
            else if (! INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (long_integer_type_node), yylval.ttype))
              type = long_long_integer_type_node;
            else
              type = long_long_unsigned_type_node;
            TREE_TYPE (yylval.ttype) = type;
            TREE_UNSIGNED (yylval.ttype) = TREE_UNSIGNED (TREE_TYPE (yylval.ttype));

            value = LEX_INTCONST;
          }
        baseflag = baseflag_save;
        break;
      }

    case '"':
        if ((pedantic || flag_what_pascal) && is_pascal_source)
          pedwarn ("double quoted strings are a GNU Pascal extension");
        /* FALLTHROUGH */
    case '\'':
      {
        char quote_char = c;
        c = gpc_getc ();    /* first char */
        p = token_buffer + 1;
        if (quote_char == '"')
          value = LEX_STRCONST;
        else
          value = CHAR_LITERAL; /* Let's all WIRTH together 3 times! */

        if (pedantic && wide_flag)
          pedwarn ("ISO Pascal does not allow wide %s",
                   (value == LEX_STRCONST) ? "strings" : "chars");

        while (1) {
            int num;
            while (c != quote_char && c != EOF) {
                if (quote_char == '"' && c == '\\') {
                    int ignore = 0;
                    c = readescape (&ignore);
                    if (ignore)
                        goto skipnewline;
                } else if (c == '\n') {
                    if (pedantic || flag_what_pascal)
                      pedwarn ("newline in char and string constants are a GNU Pascal extension");
                    lineno++;
                }
                if (p == token_buffer + maxtoken)
                    p = extend_token_buffer (p);
                *p++ = c;
            skipnewline:
                c = gpc_getc ();
            }
            if (c == EOF)
              {
                error ("string not terminated before end of file");
                break;
              }
            /* Do we have a closing quote? */
            num = 0;
            do
            {
              if (p == token_buffer + maxtoken)
                p = extend_token_buffer (p);
              if (num & 1)
                *p++ = c;
              num++;
              c = gpc_getc ();
            } while (c == quote_char && quote_char == '\'');
            *p = 0;

            if (p - token_buffer > 2)
              value = LEX_STRCONST;

            if (num & 1)
              { /* string/char constant has terminated */
                if (c != EOF)
                  gpc_ungetc (c);
                break; /* while (1) */
              }

            /* String did not terminate, continue reading it */
        }

        if (p - token_buffer == 1)
          if (value == CHAR_LITERAL || pedantic)
            {
              if (pedantic)
                pedwarn ("empty string literal%s", (value == CHAR_LITERAL) ? " in ''" : "");
              value = LEX_STRCONST;
            }

        if (value == CHAR_LITERAL)
          {
            char code = token_buffer[1];
            /* If char type is signed, sign-extend the constant.  */
            if (TREE_UNSIGNED (char_type_node)
                || ((code >> (BITS_PER_UNIT - 1)) & 1) == 0)
              yylval.ttype = build_int_2(code & ((1 << BITS_PER_UNIT) - 1), 0);
            else
              yylval.ttype = build_int_2(code | ((-1) << BITS_PER_UNIT), -1);

            TREE_TYPE (yylval.ttype) = char_type_node;
          }
        else
          {
            /* string constant */
            yylval.ttype = build_string (p - token_buffer, token_buffer + 1);
            TREE_TYPE (yylval.ttype) = char_array_type_node;
          }
        break;
      }

    case '&':
      {
        char c1;
        token_buffer [1] = c1 = gpc_getc ();
        if (c1 == '&')
          {
            value = LEX_LABEL_ADDR;  /* To take address of a label */
            token_buffer [2] = '\000';
          }
        else
          {
            value = '&';
            gpc_ungetc (c1);
            token_buffer [1] = '\000';
          }
      }

    case '+':
    case '-':
    case '<':
    case '>':
    case '*':
    case '/':
    case '=':
    case ':':
      {
        register int c1;

        token_buffer[1] = c1 = gpc_getc ();
        token_buffer[2] = 0;

        if (c1 == '=') {
            switch (c) {
            case ':': {
                        value = LEX_ASSIGN;
                        lex_const_equal = -1;
                        goto done;
                      }
            case '<': value = LEX_LE; goto done;
            case '>': value = LEX_GE; goto done;
            }
        } else if (c == '<' && c1 == '>') {
            value = LEX_NE; goto done;
        } else if (c == '>' && c1 == '<') {
            value = LEX_SYMDIFF; goto done;
        } else if (c == '=' && c1 == '>') {
            value = LEX_RENAME; goto done;
        } else if (c == '*' && c1 == '*') {
            value = LEX_POWER; goto done;
        } else if (c1 == '>') {
            switch (c) {
            case '+': value = LEX_CEIL_PLUS; goto done;
            case '-': value = LEX_CEIL_MINUS; goto done;
            case '*': value = LEX_CEIL_MULT; goto done;
            case '/': value = LEX_CEIL_DIV; goto done;
            }
        } else if (c1 == '<') {
            switch (c) {
            case '+': value = LEX_FLOOR_PLUS; goto done;
            case '-': value = LEX_FLOOR_MINUS; goto done;
            case '*': value = LEX_FLOOR_MULT; goto done;
            case '/': value = LEX_FLOOR_DIV; goto done;
            }
        }
        gpc_ungetc (c1);
        token_buffer[1] = 0;

        if (lex_const_equal == 0 && c == '=')
          {
            lex_const_equal = -1;
            value = LEX_CONST_EQUAL;
          }
        else
          value = c;
        goto done;
      }

    default:
      value = c;
    }

done:
  if (lex_const_equal >= 0)
    switch (value)
      {
        case '(':
        case '[': lex_const_equal++;
                  break;
        case ')':
        case ']': lex_const_equal--;
                  break;
      }
  return value;
}
