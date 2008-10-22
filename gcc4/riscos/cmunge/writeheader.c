/*
 * Copyright (C) 1999-2002 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <string.h>

#include "gfile.h"
#include "error.h"
#include "options.h"
#include "format.h"
#include "comments.h"

#include "copyright.h"

#include "VersionNum" /* CVS maintained version file */

/********* Stylistic switches **********/

/* Define this if you would prefer parameters to prototypes to appear on
   separate lines and thus be more readable */
/* #define INDENT_PROTOTYPES */

/* Alternatively, define this for prototypes which will be line wrapped
   like normal text */
#define FORMAT_PROTOTYPES

/* Define this if you would prefer the header format for prototypes to be
   in the 'classic' CMunge form, or in the revamped and 'modern' version.
   Classic:
     <description>
     <prototype>
     <note>
     <parameters>
     <exit>
   Modern:
     <star divider>
     <function name>
     <description>
     <parameters>
     <exit>
     <note>
     <star divider>

   The style of classic has been retained as much as possible, but because
   of the restructuring, some parts of the headers will look different. This
   is unfortunate, but is unlikely to be reverted just to retain a
   particular look.
 */
/* #define CLASSIC_HEADERS */

/******* End Stylistic switches ********/

/* The maximum line length that we can use */
#define MAXLINELEN (160)

/* Sanity check */
#if LINELEN > MAXLINELEN
#error "Oi! MAXLINELEN exists for a reason! If you increase LINELEN above it, things won't work... increase MAXLINELEN!"
#endif

static FILE *file;

static const char *error;
static const char *regs;

typedef struct argumentpair_s {
  char *type;    /* Variable type */
  char *name;    /* Variable name */
  char *meaning; /* Variable value */
} argumentpair_t;

/* Maximum number of arguments we can declare in the header descriptor */
#define MAXARGS (5)

typedef struct prototypedef_s {
  const char *name;                   /* the function name */
  char *returntype;                   /* type of data returned */
  char *description;                  /* function description */
  char *note;                         /* Notes about this prototype */
  argumentpair_t param[MAXARGS];      /* list of parameters passed in */
  char *exit;                         /* description of return parameters */
} prototypedef_t;

/*************************************************** Gerph *********
 Function:     header_type
 Description:  Return a pointer to a type name, as specified in
               the command string. For example :
                 'void'    -> 'void '
                 'void *'  -> 'void *'
                 '%error%' -> the defined error type, in a similar
                              form to that above
 Parameters:   type-> the typename we're processing
 Returns:      pointer to static string, assumed to be safe until
               called again
 ******************************************************************/
static const char *header_type(const char *type)
{
  static char ourtype[MAXLINELEN];
  if (*type=='%')
  {
    if (strcmp(type,"%error%")==0)
    {
      sprintf(ourtype,"%s *",error);
      return ourtype;
    }
    if (strcmp(type,"%regs%")==0)
    {
      sprintf(ourtype,"%s *",regs);
      return ourtype;
    }
    ErrorFatal("Programmer confusion error (magic return type %s unknown)\n",type);
  }
  if (type[strlen(type)-1]=='*')
    return type;

  sprintf(ourtype,"%s ",type);
  return ourtype;
}

/*************************************************** Gerph *********
 Function:     prototype_function
 Description:  Helper function that just writes the prototype for
               the function
 Parameters:   header-> the prototype definition to write out
 Returns:      none
 ******************************************************************/
static void prototype_function(prototypedef_t *header)
{
  int arg;
  int indent;
  int pos;
  indent = fprintf(file, "%s%s(", header_type(header->returntype),
                                  header->name);
  pos = indent;
  for (arg=0; arg<MAXARGS && header->param[arg].type!=NULL; arg++)
  {
    const char *type = header_type(header->param[arg].type);
    if (arg!=0)
    {
/* JRF: Some strange conditionals, but they make each of the build options
        function in the way you expect */
#if defined(INDENT_PROTOTYPES) || defined(FORMAT_PROTOTYPES)
#ifdef FORMAT_PROTOTYPES
      if (pos + strlen(type) + strlen(header->param[arg].name) + 2 < LINELEN)
        pos += fprintf(file,", ");
      else
#endif
      {
        int c;
        fprintf(file,",\n");
        for (c=indent; c>0; c--)
          fputc(' ',file);
        pos=indent;
      }
#else
      pos += fprintf(file,", ");
#endif
    }
    pos += fprintf(file,"%s%s", type, header->param[arg].name);
  }
  if (arg==0)
    fprintf(file,"void");
  fprintf(file,");\n");
}

/*************************************************** Gerph *********
 Function:     prototype_write
 Description:  Write a function prototype, based on the description
               specified.
 Parameters:   header-> the prototype definition to write out
 Returns:      none
 Note:         If header->name == NULL no prototype will be written
               If header->description == NULL no comment will be
                 written
 ******************************************************************/
static void prototype_write(prototypedef_t *header)
{
#ifdef CLASSIC_HEADERS

  int longest_argname=0;
  int arg;

  /* Header the declaration */
  if (header->description)
    c_comment(file,header->description);

  /* Prototype */
  if (header->name)
    prototype_function(header);

  /* Now the function details */
  if (header->description)
  {
    for (arg=0; arg<MAXARGS && header->param[arg].type!=NULL; arg++)
      if (strlen(header->param[arg].name) > longest_argname)
        longest_argname = strlen(header->param[arg].name);
    if (longest_argname != 0 ||
        header->exit ||
        header->note)
    {
      fprintf(file,"/*\n");
      if (header->note)
        format_wrap(file," * Note:     ",
                         " *           ", header->note,
                         "");

      for (arg=0; arg<MAXARGS && header->param[arg].type!=NULL; arg++)
      {
        char lead1[MAXLINELEN]; /* First line leader */
        char lead2[MAXLINELEN]; /* Subsequent line leaders */

        if (arg==0)
          sprintf(lead1," * On Entry: %-*s = ",longest_argname,
                                            header->param[arg].name);
        else
          sprintf(lead1," *           %-*s = ",longest_argname,
                                            header->param[arg].name);
        sprintf(lead2," *           %-*s   ",longest_argname,"");
        format_wrap(file,lead1,lead2,header->param[arg].meaning,"");
      }
      if (header->exit)
        format_wrap(file," * On Exit:  ",
                         " *           ", header->exit,
                         "");
      fprintf(file, " */\n");
    }
    fprintf(file, "\n");
  }

#else

  int longest_argname=0;
  int arg;

  if (header->description)
  {
    /* Header the declaration */
    fprintf(file,"\n");
    fprintf(file,"/*");
    { int c=2; while (c<LINELEN) { fputc('*',file); c++; } }
    fprintf(file,"\n");

    if (header->name)
      format_wrap(file," * Function:     ",
                       " *               ", header->name,
                       "");
    format_wrap(file," * Description:  ",
                     " *               ", header->description,
                     "");

    for (arg=0; arg<MAXARGS && header->param[arg].type!=NULL; arg++)
      if (strlen(header->param[arg].name) > longest_argname)
        longest_argname = strlen(header->param[arg].name);
    if (longest_argname != 0 ||
        header->exit ||
        header->note)
    {

      for (arg=0; arg<MAXARGS && header->param[arg].type!=NULL; arg++)
      {
        char lead1[MAXLINELEN]; /* First line leader */
        char lead2[MAXLINELEN]; /* Subsequent line leaders */

        if (arg==0)
          sprintf(lead1," * Parameters:   %-*s = ",longest_argname,
                                            header->param[arg].name);
        else
          sprintf(lead1," *               %-*s = ",longest_argname,
                                            header->param[arg].name);
        sprintf(lead2," *               %-*s   ",longest_argname,"");
        format_wrap(file,lead1,lead2,header->param[arg].meaning,"");
      }
      if (header->exit)
        format_wrap(file," * On exit:      ",
                         " *               ", header->exit,
                         "");
      if (header->note)
        format_wrap(file," * Note:         ",
                         " *               ", header->note,
                         "");

      }
    fprintf(file," ");
    { int c=3; while (c<LINELEN) { fputc('*',file); c++; } }
    fprintf(file, "*/\n");
  }

  /* Prototype */
  if (header->name)
    prototype_function(header);

  if (header->description && header->name)
    fprintf(file, "\n");
#endif
}

static void mainp(void) {

  static prototypedef_t header =
  {
    "main",
    "int",
    "Module entry point when entered by OS_Module 2. We are in user mode and "
    "will be using user mode stack in application space. If you're going to "
    "be a module task, then you will need to claim Service_Memory.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "int",
        "argc",
        "Count of the number of arguments passed to the module in R2 when "
        "OS_Module 2 issued." },
      {
        "char *",
        "argv[]",
        "array of pointers to strings passed as arguments to the module" }
    },
    "Return EXIT_SUCCESS for successful completion, or EXIT_FAILURE (or "
    "some other non-zero value for failure."
  };

  prototype_write(&header);
}

static void init(void) {

  static prototypedef_t header =
  {
    NULL, /* Function name is filled in */
    "%error%",
    "Initialise the module, setting up vectors, callbacks and any other "
    "parts of the system necessary for the module to function.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "const char *",
        "tail",
        "pointer to command line (control terminated)" },
      {
        "int",
        "podule_base",
        "address of podule module was started from, or NULL if none" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Return NULL for successful initialisation, or a pointer to an "
    "error block if the module could not start properly."
  };
  header.name=opt.init;

  prototype_write(&header);
}

static void fin(void) {

  static prototypedef_t header =
  {
    NULL, /* Function name is filled in */
    "%error%",
    "Finalise the module, shutting down any systems necessary, "
    "freeing vectors and releasing workspace",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "int",
        "fatal",
        "fatality indicator; 1 if fatal, 0 if reinitialising" },
      {
        "int",
        "podule_base",
        "address of podule module was started from, or NULL if none" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Return NULL for successful finalisation, or a pointer to an error block "
    "if module was not shutdown properly."
  };
  header.name=opt.final;

  prototype_write(&header);
}

static void service(void) {

  static prototypedef_t header =
  {
    NULL, /* Function name is filled in */
    "void",
    "Service call handler routine. All services which are being received "
    "by the module will be passed to this routine.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "int",
        "service",
        "service call number" },
      {
        "%regs%",
        "r",
        "pointer to register block on entry" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Update register values on return by updating r."
  };
  header.name=opt.service;

  prototype_write(&header);
}

static void commands(void) {

  static prototypedef_t header_text =
  {
    NULL, /* No function name; we just want the description here */
    "%error%",
    "Star command and help request handler routines.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "const char *",
        "arg_string",
        "pointer to argument string (control terminated), or output buffer" },
      {
        "int",
        "argc",
        "number of arguments passed" },
      {
        "int",
        "number",
        "command number (see CMD_* definitions below)" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "If number indicates a help entry:\n"
    "\tTo output, assemble zero terminated output into arg_string, "
    "and return help_PRINT_BUFFER to print it.\n"
    "\tTo stay silent, return NULL.\n"
    "\tTo given an error, return an error pointer.\n"
    "\t[In this case, you need to cast the 'const' away]\n"
    "If number indicates a configure option:\n"
    "\tIf arg_string is arg_STATUS, then print status, otherwise use "
    "argc and arg_string to set option.\n"
    "\tReturn NULL for no error.\n"
    "\tReturn one of the four error codes below (configure_*) for a "
    "generic error message.\n"
    "\tReturn an error pointer for a custom error.\n"
    "If number indicates a command entry:\n"
    "\tExecute the command given by number, and arg_string.\n"
    "\tReturn NULL on success,\n"
    "\tReturn a pointer to an error block on failure."
  };

  prototypedef_t header_proto;
  help_list l;
  int n;

  header_proto = header_text;
  header_proto.description=NULL;

#ifdef CLASSIC_HEADERS
#else
  prototype_write(&header_text);
#endif

  if (opt.helpfn)
  {
    header_proto.name = opt.helpfn;
    prototype_write(&header_proto);
  }
  if (opt.command_codesupplied)
  {
    l = opt.commands;
    while (l) {
      if (l->handler)
      {
        header_proto.name = l->handler;
        prototype_write(&header_proto);
      }
      l=l->next;
      n++;
    }
  }

#ifdef CLASSIC_HEADERS
  prototype_write(&header_text);
#endif

  fprintf(file, "#define help_PRINT_BUFFER\t\t((%s *) arg_string)\n", error);
  fprintf(file, "#define arg_CONFIGURE_SYNTAX\t\t((char *) 0)\n");
  fprintf(file, "#define arg_STATUS\t\t\t((char *) 1)\n");
  fprintf(file, "#define configure_BAD_OPTION\t\t((%s *) -1)\n", error);
  fprintf(file, "#define configure_NUMBER_NEEDED\t\t((%s *) 1)\n", error);
  fprintf(file, "#define configure_TOO_LARGE\t\t((%s *) 2)\n", error);
  fprintf(file, "#define configure_TOO_MANY_PARAMS\t((%s *) 3)\n\n", error);

  c_comment(file, "Command numbers, as passed to the command handler "
                  "functions (see above)");
  l = opt.commands;
  n = 0;
  while (l) {
    if (!l->no_handler)
    {
      fprintf(file, "#undef CMD_%s\n", l->name);
      fprintf(file, "#define CMD_%s (%d)\n", l->name, n);
      n++;
    }
    l=l->next;
  }
  fprintf(file, "\n");
}

static void swi_handler(void) {

  static prototypedef_t header_text =
  {
    NULL, /* Function name is filled in */
    "%error%",
    "SWI handler routine. All SWIs for this module will be passed "
    "to these routines.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "int",
        "number",
        "SWI number within SWI chunk (i.e. 0 to 63)" },
      {
        "%regs%",
        "r",
        "pointer to register block on entry" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Return NULL if SWI handled sucessfully, setting return register "
    "values (r0-r9) in r.\n"
    "Return error_BAD_SWI for out of range SWIs.\n"
    "Return an error block for a custom error."
  };

  swi_list l;
  const char *prefix = opt.swi_names->name;
  int swi=opt.swi_base;

  prototypedef_t header_proto;

  header_proto = header_text;
  header_proto.description = NULL;

#ifdef CLASSIC_HEADERS
#else
  prototype_write(&header_text);
#endif

  if (opt.swi_handler)
  {
    c_comment(file, "Function called to handle SWI calls");
    header_proto.name = opt.swi_handler;
    prototype_write(&header_proto);
  }
  if (opt.swi_codesupplied)
  {
    l=opt.swi_names->next; /* Must be valid to get to here */
    c_comment(file, "Functions called to handle particular SWI calls");
    while (l)
    {
      if (l->handler)
      {
        header_proto.name = l->handler;
        prototype_write(&header_proto);
      }
      l=l->next;
    }
  }

#ifdef CLASSIC_HEADERS
  prototype_write(&header_text);
#endif

  /* SWI number definitions */
  c_comment(file, "SWI number definitions");

  fprintf(file, "#define %s_00 (0x%08x)\n", prefix, swi);

  l = opt.swi_names;
  if (l)
    l = l->next;
  while (l) {
    const char *name = l->name;
    int startlen,len;
    startlen=len=strlen(prefix)+1+strlen(name);
    fprintf(file, "#undef %s_%s\n", prefix, name);
    fprintf(file, "#undef X%s_%s\n", prefix, name);
    fprintf(file, "#define %s_%s  ", prefix, name);
    while (len++<24) fputc(' ',file);
    fprintf(file,"(0x%08x)\n", swi);
    fprintf(file, "#define X%s_%s ", prefix, name);
    while (startlen++<24) fputc(' ',file);
    fprintf(file,"(0x%08x)\n", swi+(1<<17));
    swi++;
    l=l->next;
  }
  fprintf(file, "\n");
  c_comment(file, "Special error for 'SWI values out of range for "
                  "this module'");
  fprintf(file, "#define error_BAD_SWI ((%s *) -1)\n", error);
  fprintf(file, "\n");
}

static void swi_decoder(void) {

  if (opt.swi_decoder->handler) {

    static prototypedef_t header_nametonumber =
    {
      NULL, /* Function name is filled in */
      "int",
      "SWI name to number decoding handler.",
      NULL, /* No note */
      { /* Entry parameters */
        {
          "const char *",
          "string",
          "pointer to string to decode (control char terminated)" },
        {
          "void *",
          "pw",
          "private word for module" },
      },
      "If you recognise the string, return the offset within "
      "the SWI chunk (i.e. 0 to 63).\n"
      "Otherwise return < 0"
    };

    static prototypedef_t header_numbertoname =
    {
      NULL, /* Function name is filled in */
      "int",
      "SWI number to name decoding handler.",
      NULL, /* No note */
      { /* Entry parameters */
        {
          "int",
          "number",
          "offset within SWI chunk (0 to 63)" },
        {
          "char *",
          "string",
          "pointer to output buffer" },
        {
          "int",
          "offset",
          "offset within buffer to start writing" },
        {
          "int",
          "size",
          "size of the output buffer" },
        {
          "void *",
          "pw",
          "private word for module" },
      },
      "If the SWI number is recognised, write the SWI name into "
      "the buffer at the correct offset. Do not terminate the string; "
      "return the offset to next free char.\n"
      "If the SWI is unrecognised, return offset unchanged."
    };
    header_nametonumber.name = opt.swi_decoder->name;
    header_numbertoname.name = opt.swi_decoder->handler;

    prototype_write(&header_nametonumber);
    prototype_write(&header_numbertoname);

  } else {

    static prototypedef_t header =
    {
      NULL, /* Function name is filled in */
      "int",
      "SWI number/name decoding handler.",
      NULL, /* No note */
      { /* Entry parameters */
        {
          "int",
          "r[4]",
          "If r[0] < 0 then :\n"
          "\tr[1] points to a control terminatated string which you should "
          "convert to a number.\n"
          "\tIf you recognise the name, set r[0] to the offset within the SWI "
          "chunk.\n"
          "\tIf you do not recognise the name, set r[0] < 0.\n"
          "If r[0] >= 0 then :\n"
          "\tr[0] = offset within the SWI block\n"
          "\tr[1] = pointer to an output buffer\n"
          "\tr[2] = offset within buffer to fill from\n"
          "\tr[3] = pointer to byte beyond the end of the buffer\n"
          "\tIf the offset is recognised, fill the buffer from the required "
          "point (updating, r[2], without a terminator), and return NULL."
          "\tIf the offset is not recognised, return an error pointer.\n" },
        {
          "void *",
          "pw",
          "private word for module" },
      },
      "As defined above."
    };
    header.name = opt.swi_decoder->name;

    prototype_write(&header);
  }
}

static void pdriver_handler(void)
{
  static prototypedef_t header_entry =
  {
    NULL, /* Function name is filled in */
    "extern void",
    "Symbol for entry point to module - NOT a C function.\n"
    "This name should be used as an argument to PDriver_DeclareDriver as "
    "required, but should never be called from C.",
    NULL, /* No note */
    { { NULL } }, /* NO Entry parameters */
    NULL
  };

  static prototypedef_t header_text =
  {
    NULL, /* Function name is filled in */
    "%error%",
    "PDriver handler routine.",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "int",
        "number",
        "PDriver reason code (i.e. 0 to 63)" },
      {
        "%regs%",
        "r",
        "pointer to register block on entry" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Return NULL if PDriver SWI call handled sucessfully, setting return register "
    "values (r0-r9) in r.\n"
    "Return error_BAD_PDRIVER_SWI for out of range SWIs.\n"
    "Return an error block for a custom error."
  };

  pdriver_list *l;
  prototypedef_t header_proto;

  header_entry.name = opt.pdriver_entry;
  prototype_write(&header_entry);
  
  header_proto = header_text;
  header_proto.description = NULL;

#ifdef CLASSIC_HEADERS
#else
  prototype_write(&header_text);
#endif

  c_comment(file, "Functions called to handle particular PDriver SWI calls");
  for (l = opt.pdriver_names; l; l = l->next)
  {
    header_proto.name = l->handler;
    prototype_write(&header_proto);
  }

#ifdef CLASSIC_HEADERS
  prototype_write(&header_text);
#endif

  fprintf(file, "\n");
  c_comment(file, "Special error for 'PDriver SWI values out of range for "
                  "this module'");
  fprintf(file, "#define error_BAD_PDRIVER_SWI ((%s *) -1)\n", error);
  fprintf(file, "\n");
}

static void event_handler(void) {

  static prototypedef_t header_entry =
  {
    NULL, /* Function name is filled in */
    "extern void",
    "Symbol for entry point to module - NOT a C function.\n"
    "This name should be used as an argument to OS_Claim/OS_Release as "
    "required, but should never be called from C.",
    NULL, /* No note */
    { { NULL } }, /* NO Entry parameters */
    NULL
  };

  static prototypedef_t header_handler =
  {
    NULL, /* Function name is filled in */
    "int",
    "Event handler routine. Events will be passed to this routine if "
    "they are in the accept list (or all, if no list was provided).",
    NULL, /* No note */
    { /* Entry parameters */
      {
        "%regs%",
        "r",
        "pointer to register block on entry" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    "Update r to alter return values.\n"
    "Return 0 to claim event (pulling return address from the stack).\n"
    "Return non-0 to pass on (returning via r14)."
  };

  header_entry.name = opt.events->name;

  if (opt.events->handler)
    header_handler.name = opt.events->handler;
  else
  {
    static char buffer[MAXLINELEN];
    sprintf(buffer,"%s_handler",opt.events->name);
    header_handler.name = buffer;
  }

  prototype_write(&header_entry);
  prototype_write(&header_handler);
}

typedef enum {
  ret_int,       /* vector/irq */
  ret_err,       /* generic */
  ret_errspecial /* vector-trap */
} rettype;

static void handlers(handler_list head, const char *type, rettype rtype) {

  int carry_used = 0;
  int error_used = 0;
  handler_list h = head;

  static prototypedef_t header_entry =
  {
    NULL, /* Function name is filled in */
    "extern void",
    "Symbol for entry point to module - NOT a C function.\n"
    "This name should be used as an argument to OS_Claim/OS_Release as "
    "required, but should never be called from C.",
    NULL, /* Note will be updated */
    { { NULL } }, /* NO Entry parameters */
    NULL
  };

  static prototypedef_t header_handler =
  {
    NULL, /* Function name is filled in */
    NULL, /* Type will be filled in */
    NULL, /* Description will be filled in */
    NULL, /* No note */
    { /* Entry parameters */
      {
        "%regs%",
        "r",
        "pointer to register block on entry" },
      {
        "void *",
        "pw",
        "private word for module" },
    },
    NULL /* Exit will be updated */
  };

  if (rtype==ret_errspecial)
  {
    c_comment(file, "vectortrap_f is a function which we call to process "
                    "the remainder of the vector chain.");
    fprintf(file,"typedef %s *(*vectortrap_f)(%s *regs,void *trappw);\n",
                 error,regs);
  }

  switch (rtype)
  {
    case ret_int:
      header_handler.returntype = "int";
      if (head != opt.irqs)
      {
        header_handler.exit = "Update r to alter return values\n"
                              "Return VECTOR_CLAIM to claim (return via stack)\n"
                              "Return VECTOR_PASSON to pass on (return via r14)";
      }
      else
      {
        header_handler.exit = "Update r to alter return values\n"
                              "Return 0 to claim (return via stack)\n"
                              "Return non-0 to pass on (return via r14)";
      }
      break;

    case ret_err:
      header_handler.returntype = "%error%";
      header_handler.exit = "Update r to alter return values\n"
                            "Return NULL to return with V clear\n"
                            "Return an error pointer to set V and r0";
      break;

    case ret_errspecial:
      header_handler.returntype = "%error%";
      header_handler.exit = "Update r to alter return values\n"
                            "Return NULL to return from vector (claiming it)\n"
                            "Return VECTORTRAP_PASSON to return pass on "
                            "to next claimant\n"
                            "Return error pointer to return with V set "
                            "and error in R0";
      break;
    default:
      ErrorFatal("Programmer confusion error (bad return type for handler)");
  }

  if (!h)
    return; /* Nothing to do */

  while (h) {

    if (h->private_word != -1)
    {
      if (h->private_word != 12)
      {
        static char buffer[2048];
        sprintf(buffer,
          "This entry point takes its private word in register r%i rather "
          "than r12 as is more usually used. r12 will be preserved around "
          "this entry point. This type of entry would be used for a callback "
          "such as a SoundDMA linear handler.",h->private_word);
        header_entry.note = buffer;
      }
      else
      {
        header_entry.note = "This entry point will preserve r12.";
      }
    }
    else
    {
      header_entry.note = NULL;
    }

    /* JRF: Really we should be updating the exit comment, but this is easier
            for now. */
    if (h->error_capable)
    {
      header_handler.note =
        "VECTOR_ERROR(error) may also be used to claim this vector, "
        "and return with V set and R0 set to the pointer supplied.";
    }
    else
    {
      header_handler.note = NULL;
    }

    {
      static char buffer[MAXLINELEN];
      sprintf(buffer, "%s handler function", type);
      header_handler.description = buffer;
    }

    header_entry.name = h->name;

    if (h->handler)
    {
      header_handler.name = h->handler;
    }
    else
    {
      static char buffer[MAXLINELEN];
      sprintf(buffer,"%s_handler", h->name);
      header_handler.name = buffer;
    }

    if (rtype==ret_errspecial)
    {
      /* The function prototype for the trap routine, and the private word
         it will take */
      header_handler.param[2].type = "vectortrap_f";
      header_handler.param[2].name = "trap";
      header_handler.param[2].meaning =
          "pointer to function to call to pass on vector";
      header_handler.param[3].type = "void *";
      header_handler.param[3].name = "trappw";
      header_handler.param[3].meaning =
          "private word to pass to trap function";
    }
    else
      header_handler.param[2].type = NULL;

    if (h->carry_capable)
      carry_used = 1;
    if (h->error_capable)
      error_used = 1;

    prototype_write(&header_entry);
    prototype_write(&header_handler);

    h = h->next;
  }
  if (rtype==ret_errspecial)
  {
    c_comment(file, "VECTORTRAP_PASSON should *only* be used to pass "
                    "through calls which you have no interest in "
                    "whatsoever. It is available as an optimisation "
                    "over calling the trap function as a return "
                    "parameter.");
    fprintf(file,"#define VECTORTRAP_PASSON ((%s *)1)\n\n", error);
  }

  if (rtype==ret_err && carry_used)
  {
    c_comment(file, "VENEER_SETCARRY can be returned from routines which "
                    "have been marked as capable of having a carry flag "
                    "returned to return with the carry flag set. All other "
                    "registers are preserved.");
    fprintf(file,"#define VENEER_SETCARRY ((%s *)2)\n\n", error);
  }

  {
    static int wrote_vectors = 0;
    /* We don't write out on IRQs because a) they're deprecated and
                                          b) it makes a mess of the way
                                             that we handle it */
    if (rtype==ret_int && !wrote_vectors && head != opt.irqs)
    {
      wrote_vectors = 1;
      c_comment(file, "VECTOR_PASSON can be returned from vectors to pass "
                      "the call on to other handlers.");
      fprintf(file,"#define VECTOR_PASSON (1)\n");
      c_comment(file, "VECTOR_CLAIM can be returned from vectors to claim "
                      "the vector and return with the updated register "
                      "block.");
      fprintf(file,"#define VECTOR_CLAIM (0)\n\n");
      if (error_used)
      {
        c_comment(file, "VECTOR_ERROR(err) can be returned from routines which "
                        "have been marked as capable of having an error returned "
                        "as well as claiming the call. All other registers "
                        "are preserved if the error pointer is not NULL.");
        fprintf(file,"#define VECTOR_ERROR(err) ((int)(err))\n\n");
      }
    }
  }
}

static void errors(void) {

  int errnum=opt.error_base;

  if (errnum)
  {
    c_comment(file, "Error base for this module");
    fprintf(file, "#define ERROR_BASE (0x%08x)\n", errnum);
  }

  if (opt.errors)
  {
    error_list l=opt.errors;
    /* This is very icky, and nasty, and horrid, BUT it's the only way I can
       see to do it */
    fprintf(file,"\
/* Error messages embedded in header, based at &%x */\n",errnum);
    while (l)
    {
      int num;
      if (l->number)
        num=l->number;
      else
        num=errnum++;
      fprintf(file,"extern void __err_%s(void); /* &%x */\n",l->name,num);
      l = l->next;
    }
    l=opt.errors;
    while (l)
    {
      fprintf(file,"#define %s ((%s*)(int)__err_%s)\n",l->name,error,l->name);
      l = l->next;
    }
  }
  fprintf(file,"\n");
}

void WriteHeader(void) {

  if (!opt.hfile)
    return;

  error = (opt.oslib) ? "os_error" : "_kernel_oserror";
  regs  = (opt.oslib) ? "os_register_block" : "_kernel_swi_regs";

  file = file_write(opt.hfile, remove_onfail);

  if (file == NULL)
    ErrorFatal("Couldn't create header file: %s", opt.hfile);

  c_comment(file,"Generated by CMunge " Module_FullVersionAndDate "\n"
                 "CMunge " Copyright_CMunge);
  fprintf(file, "\n");

  {
    char *name = opt.title;
    if (name==NULL || *name=='\0')
    {
      name = strrchr(opt.infile,'.');
      if (name==NULL)
        name="anonymous";
      else
        name++;
    }
    fprintf(file, "#ifndef _CMUNGE_%s_H_\n",name);
    fprintf(file, "#define _CMUNGE_%s_H_\n\n",name);
  }

  if (opt.oslib)
    fprintf(file, "#include \"oslib/os.h\"\n\n");
  else
    fprintf(file, "#include \"kernel.h\"\n\n");

  fprintf(file, "#define CMUNGE_VERSION (%i)\n",Module_Version);
  fprintf(file, "#define CMHG_VERSION   (531) /* Nearest equivalent version */\n\n");

  if (!opt.mode_errors)
  {
    fprintf(file, "#define Module_Title\t\t\"%s\"\n", opt.title);
#if 0
    fprintf(file, "#define Module_Help\t\t\"%s\"\n", opt.help);

    /* JRF: Odd deviance from CMHG - we use the full help string, it uses just
            the name. To my mind, we're doing things more sensibly given the
            name of the define */
#else
    /* JRF: Changed my mind; we should emulate CMHG as closely as possible */
    {
      char *str = opt.help;
      fprintf(file, "#define Module_Help\t\t\"");
      while (*str!='\0' && *str!='\t')
      {
        fputc(*str, file);
        str++;
      }
      fprintf(file, "\"\n");
    }
#endif
    fprintf(file, "#define Module_VersionString\t\"%d.%02d\"\n", opt.version/100, opt.version%100);
    fprintf(file, "#define Module_VersionNumber\t%d\n", opt.version);

    fprintf(file, "#ifndef Module_Date\n");
    fprintf(file, "#define Module_Date\t\t\"%s\"\n", opt.date);
    fprintf(file, "#endif\n");

    if (opt.mfile)
      fprintf(file, "#define Module_MessagesFile\t\"%s\"\n", opt.mfile);
  }

  fprintf(file, "\n");

  fprintf(file, "#ifdef __cplusplus\n");
  fprintf(file, "extern \"C\" {\n");
  fprintf(file, "#endif\n\n");

  if (opt.base)
    fprintf(file, "extern const int Image__RO_Base;\n\n");

  if (opt.runnable == run_clib)
    mainp();
  if (opt.init)
    init();
  if (opt.final)
    fin();
  if (opt.service)
    service();
  if (opt.helpfn || opt.command_codesupplied)
    commands();
  if (opt.swi_handler || opt.swi_codesupplied)
    swi_handler();
  if (opt.swi_decoder)
    swi_decoder();
  if (opt.pdriver_entry)
    pdriver_handler();
  if (opt.events)
    event_handler();
  if (opt.irqs)
    handlers(opt.irqs, "IRQ", ret_int);
  if (opt.generics)
    handlers(opt.generics, "Generic", ret_err);
  if (opt.vectors)
    handlers(opt.vectors, "Vector", ret_int);
  if (opt.vector_traps)
    handlers(opt.vector_traps, "Vector traps", ret_errspecial);
  if (opt.error_base || opt.errors)
    errors();

  fprintf(file, "#ifdef __cplusplus\n");
  fprintf(file, "}\n");
  fprintf(file, "#endif\n\n");

  fprintf(file, "#endif\n");

  file_close(file);
}
