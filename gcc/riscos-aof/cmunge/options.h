/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#ifndef OPTIONS_H

#define OPTIONS_H

/* List of integers */

typedef struct int_list_s *int_list;

typedef struct int_list_s {
  unsigned int val;
  int_list     next;
} int_list_s;

/* List of help commands */

typedef struct help_list_s *help_list;

typedef struct help_list_s {
  int          min_args;
  int          max_args;
  unsigned int gstrans;
  int          fs_command;
  int          status;
  int          configure;
  int          help;
  int          add_syntax;
  int          international;
  char        *invalid_syntax;
  char        *help_text;
  char        *name;
  char        *handler;
  help_list    next;
} help_list_s;

/* Include list */

typedef struct include_list_s *include_list;

typedef struct include_list_s {
  char         *path;
  include_list  next;
} include_list_s;

/* SWI list */

typedef struct swi_list_s *swi_list;

typedef struct swi_list_s {
  char     *name;
  char     *handler;
  swi_list  next;
} swi_list_s;

/* Handler list */

typedef struct handler_list_s *handler_list;

typedef struct handler_list_s {
  char         *name;
  char         *handler;
  int          private_word;
  int          carry_capable; /* Set for generics if it is capable of setting
                                 carry on return */
  handler_list  next;
  /* extradata contains any additional information required for the handler.
     At present it is used for the event handler to reference an int_list
     of the events which will be passed through.
   */
  void         *extradata;
} handler_list_s;

/* Error list */

typedef struct error_list_s *error_list;

typedef struct error_list_s {
  char         *name;
  char         *message;
  unsigned int number; /* 0 to auto-allocate, number for an explicit number */
  error_list  next;
} error_list_s;

/* Preprocessor defines */
typedef struct defines_list_s *defines_list;

typedef struct defines_list_s {
  char         *name;
  char         *value;  /* May be null for -Dx (instead of -Dx=y) */
  int           invert; /* 0 for define, 1 for undefine */
  defines_list  next;
} defines_list_s;

/* APCS variant flags */
#define APCS_REENTRANT     (1<<0) /* Reentrant APCS */
#define APCS_32BIT         (1<<1) /* 32bit APCS (not 26bit) */
#define APCS_FPE3          (1<<2) /* FPE3 (not FPE2) */
#define APCS_SWSTACKCHECK  (1<<3) /* Software stack checking */
#define APCS_FPREGARGS     (1<<4) /* Floating point registers */
#define APCS_FP            (1<<5) /* Frame pointer */

/* Shortcut checks so that it's easier to see what's happening */
#define CODE32   (opt.apcs & APCS_32BIT)
#define CODE26   (!CODE32)

/* Structure representing the APCS options which can be used in opt.apcs */
typedef struct apcsoptions_s {
  char *name;        /* Argument name */
  int minabbrev;     /* Minimum number of characters for abbreviation */
  unsigned long bic; /* Bits to clear */
  unsigned long orr; /* Bits to set */
  char *desc;        /* Description of the flag */
} apcsoptions_t;
extern apcsoptions_t apcsoptions[];

/* Main module structure */

typedef struct {
  enum {
    run_none,
    run_blib, /* Call _blib_entermodule - obsolete feature of CMHG */
    run_clib, /* Call _clib_entermodule */
    run_rmaapp_simple  /* Set up a simple application with RMA stack - note
                          that this is exceptionally experimental */
  } runnable;	                /* module-is-runnable */
  unsigned long apcs;
  int apcs_used;                /* Flagged if -apcs option given */
  int toolchain_used;           /* Flagged if -t<blah> option given */
  enum {
    tc_norcroft,
    tc_gcc,
    tc_lcc
  } toolchain;
  int           reentrant;	/* module-is-not-reentrant */
  int           cplusplus;      /* module-is-c-plus-plus */
  char         *service;	/* service-call-handler */
  int_list      services;	/* service-call-handler */
  char         *title;		/* title-string */
  char         *help;		/* help-string */
  char         *helpfn;		/* help-handler */
  help_list     commands;	/* command-keyword-table */
  int           command_codesupplied; /* commands include handler routines */
  int           swi_base;	/* swi-shunk-base-number */
  char         *swi_handler;	/* swi-handler-code */
  swi_list      swi_names;	/* swi-decoding-table */
  int           swi_codesupplied; /* swis include handler routines */
  handler_list  swi_decoder;	/* swi-decoding-code */
  int           error_base;     /* error base */
  handler_list  irqs;		/* irq-handlers */
  handler_list  events;		/* event-handler */
  char         *init;		/* initialisation-code */
  char         *lib_init;	/* library-enter-code */
  char         *final;		/* finalisation-code */
  handler_list  generics;	/* generic-veneers */
  handler_list	vectors;	/* vector-handlers */
  handler_list	vector_traps;	/* vector-traps */
  error_list    errors;         /* error-identifiers */
  int           errors_special; /* errors have been given numbers */
  int           version;
  char         *date;
  char         *datestring;     /* User supplied date string */

  /* The files we're going to use */
  int          blank;           /* Are we producing a blank template file ? */
  char         *ofile;		/* the AOF file */
  char         *sfile;		/* the assembler file */
  int           stemp;          /* is the assembler in a temporary file? */
  char         *hfile;		/* the C header */
  char         *x_hdr;		/* the assembler SWI header */
  char         *x_h;		/* the C SWI header */
  char         *mfile;		/* Message file */
  char         *dfile;		/* Depend file */

  /* Various output tweaks */
  int           oslib;          /* should produced C header be OSLib or Acorn compatible? */
  int           base;           /* should I define Image__RO_Base? */
  int           mode_errors;    /* are we processing as 'errors only' mode ? */

  /* For throwback */
  int           throwback;
  char         *infile;
  int           atline;

  /* For pre-processing */
  char         *pfile;          /* preprocessed file */
  defines_list  defines;        /* any defines added */
  include_list  includes;       /* Include Path List */

  /* Options from the CLI */
  int           cmhg;           /* We /are/ CMHG, to all intents and purposes */
} options;

extern options opt;

void Options_Init(void);

void Options_CL(int argc, char *argv[]);

void Options_CheckSanity(void);

void Options_CheckSanityPostRead(void);

#endif
