/*
Support routines for the CRT unit

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Author: Frank Heckenbach <frank@pascal.gnu.de>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.
*/

/* Prevent stdlib.h from declaring a beep() function which would
   conflict with PDCurses' one */
#ifdef __MINGW32__
#undef _NO_OLDNAMES
#define _NO_OLDNAMES 1
#endif

/* @@ Request old behaviour of signal handlers, in particular
   non-restarting system calls */
#ifdef linux
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include "crtc.h"

int crt_get_input_fd (void);
int crt_get_output_fd (void);

#ifdef USE_PDCURSES

#ifdef XCURSES

#include <xcurses.h>
#include <xpanel.h>
char *XCursesProgramName;

/* Try to shut down cleanly when X11 is shut down */
#include <setjmp.h>
static jmp_buf crt_ShutDownJump;
static int crt_ShutDownJumpActive = FALSE;

#else

#include <curses.h>
#include <panel.h>

#endif

#define REVERSE_WORKS FALSE
#define INVIS_WORKS   TRUE
#define HAVE_RAW_OUTPUT

/* This is undocumented, but there doesn't seem to be another way! */
#define SETCURSOR(x,y) wmove (curscr, y, x)

int crt_get_input_fd ()
{
  return PDC_get_input_fd ();
}

#ifdef XCURSES

int crt_get_output_fd ()
{
  return - 1;
}

/* We don't have to clear the terminal since XCurses programs run in
   their own windows, anyway. */
static void crt_clear_terminal ()
{
}

#else

int crt_get_output_fd ()
{
  return fileno (stdout);
}

extern int PDC_clr_scrn (WINDOW *win);

static void crt_clear_terminal ()
{
  PDC_clr_scrn (stdscr);
}
#endif
#endif

#ifdef USE_NCURSES
#include <ncurses.h>
#include <panel.h>
#include <term.h>
#define REVERSE_WORKS TRUE
#define INVIS_WORKS   FALSE
#undef  HAVE_RAW_OUTPUT

/* This is undocumented, but there doesn't seem to be another way! */
#define SETCURSOR(x,y) wmove (newscr, y, x)

int crt_get_input_fd ()
{
  return fileno (stdin);
}

int crt_get_output_fd ()
{
  return fileno (stdout);
}

static int putch (int c)
{
  return putchar (c);
}

static void crt_clear_terminal ()
{
  tputs (clear_screen, lines > 0 ? lines : 1, putch);
  fflush (stdout);
}

static WINDOW *resize_window (WINDOW *w, int new_lines, int new_columns)
{
  wresize (w, new_lines, new_columns);
  return w;
}
#endif

#ifndef A_ALTCHARSET
#define A_ALTCHARSET 0
#endif

typedef unsigned char Boolean;
typedef unsigned int  TKey;
typedef unsigned char Char;
typedef unsigned char TTextAttr;
typedef enum { CursorIgnored, CursorHidden, CursorNormal, CursorFat, CursorBlock } TCursorShape;
typedef enum { UpdateNever, UpdateWaitInput, UpdateInput, UpdateRegularly, UpdateAlways } TCRTUpdate;
typedef struct { int x, y; } TPoint;

typedef struct
{
  Char Ch;
  TTextAttr Attr;
  Boolean PCCharSet;
} TCharAttr;

typedef struct
{
  int curses_key;
  Char crt_key;
} TKeyTable;

typedef struct TPanelData
{
  struct TPanelData *Next;
  WINDOW      *w;
  PANEL       *panel;
  Boolean      BoundToBackground, Hidden, ScrollState, PCCharSet, UseControlChars;
  TCursorShape CursorShape;
  unsigned int WindMin, WindMax, LastWindMin, LastWindMax;
  TTextAttr    TextAttr;
} TPanelData;
typedef TPanelData* TPanel;

extern int crt_VirtualShiftState;
extern unsigned int crt_LastMode, crt_WindMin, crt_WindMax;
extern Boolean crt_CheckBreak, crt_CheckEOF, crt_VisualBell, crt_IsMonoMode, crt_terminal_no_crt;
extern TTextAttr crt_TextAttr, crt_NormAttr;
extern void (*crt_auto_init_proc) (void);
extern TPoint crt_ScreenSize, crt_cursor_pos;

/* modified by signal handlers */
extern volatile Boolean crt_Signaled;
volatile Boolean crt_screen_size_changed = FALSE, crt_ShutDown = FALSE;
volatile TKey crt_unget_key_buf = 0;

extern char   *crt_get_program_name (void);
extern char  **crt_get_c_environment (void);
extern void    crt_registerrestoreterminal (void);
extern Boolean crt_modeismono (int);
extern void    crt_signal_handler (TKey sig);
extern void    crt_fatal (int) __attribute__((noreturn));
extern void    _p_SetReturnAddress (void *);
extern void    _p_RestoreReturnAddress (void);

#define SAVE_RETURN_ADDRESS _p_SetReturnAddress (__builtin_return_address (0))
#define RESTORE_RETURN_ADDRESS _p_RestoreReturnAddress ()
#define DO_RETURN_ADDRESS(stmt) \
do \
  { \
    SAVE_RETURN_ADDRESS; \
    stmt; \
    RESTORE_RETURN_ADDRESS; \
  } \
while (0)

static void crt_refresh (void);
void crt_check_winchanged (void);

#define DO_CRT_CHECK_WINCHANGED DO_RETURN_ADDRESS (crt_check_winchanged ())

/*
There are a few features that cannot be implemented in a portable
way, but rather for different systems individually. There are
defaults for systems where they are not specially supported, see the
comment in crt.pas. The following table lists these items, how to
check them in CRTDemo, and how to implement them on a new system.
Implementations for new systems should go into separate files,
included with appropriate ifdefs below this comment. Prototypes of
the functions to be implemented can be found there.

- Playing sounds with Sound and NoSound. In CRTDemo, this can be
  checked with the `Sound check' in the `Various checks' window.

  To support them on a system, define HAVE_CRT_SOUND and implement
  the functions crt_sound() (start playing a sound of the given
  frequency, until the next crt_sound() or crt_nosound() call), and
  crt_nosound() (stop playing any sound).

- Getting the keyboard shift state. In CRTDemo, this can be checked
  in the `Modifier Keys' window.

  By default, this is supported on PDCurses platforms. To support
  this on another system (or to override the PDCurses version),
  define HAVE_CRT_GETSHIFTSTATE and implement the function
  crt_getshiftstate() (return the keyboard shift state as a
  combination of the shFoo constants, ORed with the value of the
  variable crt_VirtualShiftState).

- Changing the screen size with TextMode. In CRTDemo, this can be
  checked by changing `Screen columns' and/or `Screen lines' in the
  `Status' window.

  By default, this is supported on PDCurses platforms except X11
  (and changing the number of columns doesn't work everywhere). To
  support this on another system (or to override the PDCurses
  version), define HAVE_CRT_GETSHIFTSTATE and implement the function
  crt_settextmode() (set the screen size to 40/80 columns and 25/50
  (or 43) lines, depending on the arguments).

- Catching INT (Ctrl-C) and/or TERM, HUP and under XCurses also PIPE
  signals to let them be treated by CRT, dependent on CheckBreak. In
  CRTDemo, this can be checked by verifying that the program reacts
  to interrupt (Ctrl-C) keys (and if applicable, suspend (Ctrl-Z)
  keys as well as to being killed with INT, TERM, HUP signals, or
  for XCurses, shutting down X11) appropriately, depending on the
  setting of `Break checking' in the `Status' window. Also, catching
  WINCH signals. In CRTDemo, this can by checked by resizing the
  terminal or window by external means and verifying that the
  `Status' window recognizes the new screen size.

  By default, handlers are implemented for any of these signals
  which are actually defined on the system. These handlers have been
  found working on all systems tested so far, so usually you won't
  have to worry about them. If you want to implement different
  handlers, define HAVE_CRT_SIGNAL_HANDLERS, and implement the
  function crt_init_signals() (install the signal handlers at the
  start of the program). The signal handlers, in turn, should call
  crt_signal_handler() whenever an INT, TERM, HUP or similar signal
  or SIGPIPE under XCurses is received, giving it as an argument a
  pseudo key code for the signal, and make sure that the program is
  not aborted by the signal. Reading a character from the curses
  window should be interrupted by the signal (which ought to happen
  by default), in order for the program to receive the pseudo key
  immediately. The signal handler for WINCH should set
  crt_screen_size_changed and crt_Signaled, and call any previously
  installed signal handler (e.g. ncurses' one).
*/
void crt_sound (unsigned Hz);
void crt_nosound (void);
int crt_getshiftstate (void);
static void crt_settextmode (Boolean Columns40, Boolean Lines50);
static void crt_init_signals (void);

#if defined(__linux__) && defined(__i386__) && !defined(XCURSES)
#include "crtlinux386.h"
#elif defined(DJGPP)
#include "crtdjgpp.h"
#endif

#define MAXLENGTH 4096

TCRTUpdate      crt_update_level = UpdateWaitInput;
TCursorShape    crt_last_shape = - 1;
Char            crt_fkey_buf = 0, crt_line_buf [MAXLENGTH], *crt_line_buf_pos = crt_line_buf;
size_t          crt_line_buf_count = 0;
chtype          crt_attrs [8] [8], crt_monoattrs [8] [8];
TPanel          crt_ActivePanel = NULL, crt_PanelList = NULL;
PANEL          *crt_SimulateBlockCursorPanel = NULL;
int             crt_inited = 0, crt_key_buf = 0,
                crt_refresh_inhibit = 0, crt_refresh_flag = 0,
                crt_colors [8] = { COLOR_BLACK, COLOR_BLUE,    COLOR_GREEN,  COLOR_CYAN,
                                   COLOR_RED,   COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE };
Boolean         crt_ColorFlag = FALSE,
                crt_HasColors = FALSE,
                crt_linux_console = FALSE,
                crt_LastCheckBreak = - 1,
                crt_pending_refresh = FALSE,
                crt_PCCharSet = FALSE,
                crt_UpdateLevelChanged = FALSE,
                crt_SimulateBlockCursorActive = FALSE;

#define WindXMin (crt_WindMin & 0xff)
#define WindXMax (crt_WindMax & 0xff)
#define WindYMin (crt_WindMin >> 8)
#define WindYMax (crt_WindMax >> 8)

#define KEY_HUP    (KEY_MAX + 1)
#ifndef KEY_RESIZE
#define KEY_RESIZE (KEY_MAX + 2)
#endif

#define FKEY1     KEY_F (1)
#define FKEYSH1   KEY_F (13)
#define FKEYCTRL1 KEY_F (25)
#define FKEYALT1  KEY_F (37)

static TKeyTable KeyTable [] =
{
  { KEY_BACKSPACE, chBkSp },
  { KEY_SUSPEND,   26     },
  { '\n',          chCR   },
  #ifdef USE_PDCURSES
  { '\r',          chCR   },
  { PADSTAR,       '*'    },
  { PADMINUS,      '-'    },
  { PADPLUS,       '+'    },
  { CTL_ENTER,     chLF   },
  { CTL_PADENTER,  chLF   },
  { CTL_PADSTAR,   chLF   },
  { CTL_PADMINUS,  chCR   },
  { CTL_PADPLUS,   11     },
  #else
  { '\r',          chLF   },
  #endif
  { 0,             0      }
};

static TKeyTable FKeyTable [] =
{
  { KEY_RESIZE,     ksScreenSizeChanged },
  { KEY_BTAB,       ksShTab     },
  { KEY_LEFT,       ksLeft      },
  { KEY_RIGHT,      ksRight     },
  { KEY_UP,         ksUp        },
  { KEY_DOWN,       ksDown      },
  { KEY_A3,         ksPgUp      },
  { KEY_PPAGE,      ksPgUp      },
  { KEY_C3,         ksPgDn      },
  { KEY_NPAGE,      ksPgDn      },
  { KEY_A1,         ksHome      },
  { KEY_HOME,       ksHome      },
  { KEY_C1,         ksEnd       },
  { KEY_END,        ksEnd       },
  { KEY_IC,         ksIns       },
  { KEY_DC,         ksDel       },
  { KEY_B2,         ksCenter    },
  { KEY_CANCEL,     ksCancel    },
  { KEY_COPY,       ksCopy      },
  { KEY_UNDO,       ksUndo      },
  { KEY_REDO,       ksRedo      },
  { KEY_OPEN,       ksOpen      },
  { KEY_CLOSE,      ksClose     },
  { KEY_COMMAND,    ksCommand   },
  { KEY_CREATE,     ksCreate    },
  { KEY_EXIT,       ksExit      },
  { KEY_FIND,       ksFind      },
  { KEY_HELP,       ksHelp      },
  { KEY_MARK,       ksMark      },
  { KEY_MESSAGE,    ksMessage   },
  { KEY_MOVE,       ksMove      },
  { KEY_NEXT,       ksNext      },
  { KEY_PREVIOUS,   ksPrevious  },
  { KEY_OPTIONS,    ksOptions   },
  { KEY_REFERENCE,  ksReference },
  { KEY_REFRESH,    ksRefresh   },
  { KEY_REPLACE,    ksReplace   },
  { KEY_RESTART,    ksRestart   },
  { KEY_SUSPEND,    ksSuspend   },
  { KEY_RESUME,     ksResume    },
  { KEY_SAVE,       ksSave      },
  #ifdef XCURSES
  /* XCurses returns these for *shifted* keys (which is not wrong :-).
     However, we don't have key codes for shifted keys, but we can get
     the shift state via crt_getshiftstate(). Control-key combinations
     are obtained using crt_getshiftstate(). */
  { KEY_SLEFT,      ksLeft      },
  { KEY_SRIGHT,     ksRight     },
  { KEY_SHOME,      ksHome      },
  { KEY_SEND,       ksEnd       },
  { KEY_SDC,        ksDel       },
  { KEY_SIC,        ksIns       },
  #else
  { KEY_SLEFT,      ksCtrlLeft  },
  { KEY_SRIGHT,     ksCtrlRight },
  { KEY_SHOME,      ksCtrlHome  },
  { KEY_SEND,       ksCtrlEnd   },
  { KEY_SDC,        ksCtrlDel   },
  { KEY_SIC,        ksCtrlIns   },
  #endif
  #ifdef USE_PDCURSES
  { CTL_TAB,        ksCtrlTab   },
  { ALT_TAB,        ksAltTab    },
  { KEY_A2,         ksUp        },
  { KEY_B1,         ksLeft      },
  { KEY_B3,         ksRight     },
  { KEY_C2,         ksDown      },
  { CTL_LEFT,       ksCtrlLeft  },
  { CTL_RIGHT,      ksCtrlRight },
  { CTL_UP,         ksCtrlUp    },
  { CTL_DOWN,       ksCtrlDown  },
  { CTL_PGUP,       ksCtrlPgUp  },
  { CTL_PGDN,       ksCtrlPgDn  },
  { CTL_HOME,       ksCtrlHome  },
  { CTL_END,        ksCtrlEnd   },
  { CTL_INS,        ksCtrlIns   },
  { CTL_DEL,        ksCtrlDel   },
  { CTL_PADSTOP,    ksCtrlDel   },
  { CTL_PADCENTER,  ksCtrlCentr },
  { ALT_LEFT,       ksAltLeft   },
  { ALT_RIGHT,      ksAltRight  },
  { ALT_UP,         ksAltUp     },
  { ALT_DOWN,       ksAltDown   },
  { ALT_PGUP,       ksAltPgUp   },
  { ALT_PGDN,       ksAltPgDn   },
  { ALT_HOME,       ksAltHome   },
  { ALT_END,        ksAltEnd    },
  { ALT_INS,        ksAltIns    },
  { ALT_DEL,        ksAltDel    },
  { ALT_ENTER,      ksAltEnter  },
  { ALT_PADENTER,   ksAltEnter  },
  { ALT_PADSTAR,    ksAltPStar  },
  { ALT_PADMINUS,   ksAltPMinus },
  { ALT_PADPLUS,    ksAltPPlus  },
  { ALT_ESC,        ksAltEsc    },
  { ALT_BKSP,       ksAltBkSp   },
  { ALT_MINUS,      ksAltMinus  },
  { ALT_EQUAL,      ksAltEqual  },
  { ALT_LBRACKET,   ksAltLBrack },
  { ALT_RBRACKET,   ksAltRBrack },
  { ALT_SEMICOLON,  ksAltSemic  },
  { ALT_FQUOTE,     ksAltFQuote },
  { ALT_BQUOTE,     ksAltBQuote },
  { ALT_COMMA,      ksAltComma  },
  { ALT_STOP,       ksAltStop   },
  { ALT_FSLASH,     ksAltFSlash },
  { ALT_BSLASH,     ksAltBSlash },
  { ALT_0,          ksAlt0      },
  { ALT_1,          ksAlt1      },
  { ALT_2,          ksAlt2      },
  { ALT_3,          ksAlt3      },
  { ALT_4,          ksAlt4      },
  { ALT_5,          ksAlt5      },
  { ALT_6,          ksAlt6      },
  { ALT_7,          ksAlt7      },
  { ALT_8,          ksAlt8      },
  { ALT_9,          ksAlt9      },
  { ALT_A,          ksAltA      },
  { ALT_B,          ksAltB      },
  { ALT_C,          ksAltC      },
  { ALT_D,          ksAltD      },
  { ALT_E,          ksAltE      },
  { ALT_F,          ksAltF      },
  { ALT_G,          ksAltG      },
  { ALT_H,          ksAltH      },
  { ALT_I,          ksAltI      },
  { ALT_J,          ksAltJ      },
  { ALT_K,          ksAltK      },
  { ALT_L,          ksAltL      },
  { ALT_M,          ksAltM      },
  { ALT_N,          ksAltN      },
  { ALT_O,          ksAltO      },
  { ALT_P,          ksAltP      },
  { ALT_Q,          ksAltQ      },
  { ALT_R,          ksAltR      },
  { ALT_S,          ksAltS      },
  { ALT_T,          ksAltT      },
  { ALT_U,          ksAltU      },
  { ALT_V,          ksAltV      },
  { ALT_W,          ksAltW      },
  { ALT_X,          ksAltX      },
  { ALT_Y,          ksAltY      },
  { ALT_Z,          ksAltZ      },
  #endif
  { FKEYSH1 + 10,   ksShF11     },
  { FKEYSH1 + 11,   ksShF12     },
  { FKEYCTRL1 + 10, ksCtrlF11   },
  { FKEYCTRL1 + 11, ksCtrlF12   },
  { FKEYALT1 + 10,  ksAltF11    },
  { FKEYALT1 + 11,  ksAltF12    },
  { 0,              ksUnknown   }
};

static TKeyTable ShiftKeyTable [] =
{
  { chTab, ksShTab },
  { 0,     0       }
};

static TKeyTable ShiftFKeyTable [] =
{
  { ksIns, ksShIns },
  { ksDel, ksShDel },
  { 0,     0       }
};

static TKeyTable CtrlFKeyTable [] =
{
  { ksLeft,   ksCtrlLeft  },
  { ksRight,  ksCtrlRight },
  { ksUp,     ksCtrlUp    },
  { ksDown,   ksCtrlDown  },
  { ksPgUp,   ksCtrlPgUp  },
  { ksPgDn,   ksCtrlPgDn  },
  { ksHome,   ksCtrlHome  },
  { ksEnd,    ksCtrlEnd   },
  { ksIns,    ksCtrlIns   },
  { ksDel,    ksCtrlDel   },
  { ksCenter, ksCtrlCentr },
  { ksF1,     ksCtrlF1    },
  { ksF2,     ksCtrlF2    },
  { ksF3,     ksCtrlF3    },
  { ksF4,     ksCtrlF4    },
  { ksF5,     ksCtrlF5    },
  { ksF6,     ksCtrlF6    },
  { ksF7,     ksCtrlF7    },
  { ksF8,     ksCtrlF8    },
  { ksF9,     ksCtrlF9    },
  { ksF10,    ksCtrlF10   },
  { ksF11,    ksCtrlF11   },
  { ksF12,    ksCtrlF12   },
  { 0,        0           }
};

static TKeyTable AltFKeyTable [] =
{
  { ksLeft,   ksAltLeft  },
  { ksRight,  ksAltRight },
  { ksUp,     ksAltUp    },
  { ksDown,   ksAltDown  },
  { ksPgUp,   ksAltPgUp  },
  { ksPgDn,   ksAltPgDn  },
  { ksHome,   ksAltHome  },
  { ksEnd,    ksAltEnd   },
  { ksIns,    ksAltIns   },
  { ksDel,    ksAltDel   },
  { 0,        0          }
};

static TKeyTable AltKeyTable [] =
{
  { ' ',  ksAltSpace },
  { chLF, ksAltEnter },
  { chCR, ksAltEnter },
  { 0,    0          }
};

static TKeyTable AltGrKeyTable [] =
{
  { '0',   ksAltGr0 },
  { '1',   ksAltGr1 },
  { '2',   ksAltGr2 },
  { '3',   ksAltGr3 },
  { '4',   ksAltGr4 },
  { '5',   ksAltGr5 },
  { '6',   ksAltGr6 },
  { '7',   ksAltGr7 },
  { '8',   ksAltGr8 },
  { '9',   ksAltGr9 },
  { 'A',   ksAltGrA },
  { 'B',   ksAltGrB },
  { 'C',   ksAltGrC },
  { 'D',   ksAltGrD },
  { 'E',   ksAltGrE },
  { 'F',   ksAltGrF },
  { 'G',   ksAltGrG },
  { 'H',   ksAltGrH },
  { 'I',   ksAltGrI },
  { 'J',   ksAltGrJ },
  { 'K',   ksAltGrK },
  { 'L',   ksAltGrL },
  { 'M',   ksAltGrM },
  { 'N',   ksAltGrN },
  { 'O',   ksAltGrO },
  { 'P',   ksAltGrP },
  { 'Q',   ksAltGrQ },
  { 'R',   ksAltGrR },
  { 'S',   ksAltGrS },
  { 'T',   ksAltGrT },
  { 'U',   ksAltGrU },
  { 'V',   ksAltGrV },
  { 'W',   ksAltGrW },
  { 'X',   ksAltGrX },
  { 'Y',   ksAltGrY },
  { 'Z',   ksAltGrZ },
  #ifdef USE_PDCURSES
  { ALT_0, ksAltGr0 },
  { ALT_1, ksAltGr1 },
  { ALT_2, ksAltGr2 },
  { ALT_3, ksAltGr3 },
  { ALT_4, ksAltGr4 },
  { ALT_5, ksAltGr5 },
  { ALT_6, ksAltGr6 },
  { ALT_7, ksAltGr7 },
  { ALT_8, ksAltGr8 },
  { ALT_9, ksAltGr9 },
  { ALT_A, ksAltGrA },
  { ALT_B, ksAltGrB },
  { ALT_C, ksAltGrC },
  { ALT_D, ksAltGrD },
  { ALT_E, ksAltGrE },
  { ALT_F, ksAltGrF },
  { ALT_G, ksAltGrG },
  { ALT_H, ksAltGrH },
  { ALT_I, ksAltGrI },
  { ALT_J, ksAltGrJ },
  { ALT_K, ksAltGrK },
  { ALT_L, ksAltGrL },
  { ALT_M, ksAltGrM },
  { ALT_N, ksAltGrN },
  { ALT_O, ksAltGrO },
  { ALT_P, ksAltGrP },
  { ALT_Q, ksAltGrQ },
  { ALT_R, ksAltGrR },
  { ALT_S, ksAltGrS },
  { ALT_T, ksAltGrT },
  { ALT_U, ksAltGrU },
  { ALT_V, ksAltGrV },
  { ALT_W, ksAltGrW },
  { ALT_X, ksAltGrX },
  { ALT_Y, ksAltGrY },
  { ALT_Z, ksAltGrZ },
  #endif
  { 0,     0        }
};

static TKeyTable ExtraKeyTable [] =
{
  { '0', ksExtra0 },
  { '1', ksExtra1 },
  { '2', ksExtra2 },
  { '3', ksExtra3 },
  { '4', ksExtra4 },
  { '5', ksExtra5 },
  { '6', ksExtra6 },
  { '7', ksExtra7 },
  { '8', ksExtra8 },
  { '9', ksExtra9 },
  { 'A', ksExtraA },
  { 'B', ksExtraB },
  { 'C', ksExtraC },
  { 'D', ksExtraD },
  { 'E', ksExtraE },
  { 'F', ksExtraF },
  { 'G', ksExtraG },
  { 'H', ksExtraH },
  { 'I', ksExtraI },
  { 'J', ksExtraJ },
  { 'K', ksExtraK },
  { 'L', ksExtraL },
  { 'M', ksExtraM },
  { 'N', ksExtraN },
  { 'O', ksExtraO },
  { 'P', ksExtraP },
  { 'Q', ksExtraQ },
  { 'R', ksExtraR },
  { 'S', ksExtraS },
  { 'T', ksExtraT },
  { 'U', ksExtraU },
  { 'V', ksExtraV },
  { 'W', ksExtraW },
  { 'X', ksExtraX },
  { 'Y', ksExtraY },
  { 'Z', ksExtraZ },
  { 0,   0       }
};

#ifndef USE_PDCURSES
static TKeyTable EscKeyTable [] =
{
  { KEY_BACKSPACE, ksAltBkSp   },
  { chBkSp,        ksAltBkSp   },
  { chTab,         ksAltTab    },
  { chLF,          ksAltEnter  },
  { chCR,          ksAltEnter  },
  { chEsc,         ksAltEsc    },
  { ' ',           ksAltSpace  },
  { '-',           ksAltMinus  },
  { '=',           ksAltEqual  },
  { '[',           ksAltLBrack },
  { ']',           ksAltRBrack },
  { ';',           ksAltSemic  },
  { '\'',          ksAltFQuote },
  { '`',           ksAltBQuote },
  { ',',           ksAltComma  },
  { '.',           ksAltStop   },
  { '/',           ksAltFSlash },
  { '\\',          ksAltBSlash },
  { 'A',           ksAltA      },
  { 'B',           ksAltB      },
  { 'C',           ksAltC      },
  { 'D',           ksAltD      },
  { 'E',           ksAltE      },
  { 'F',           ksAltF      },
  { 'G',           ksAltG      },
  { 'H',           ksAltH      },
  { 'I',           ksAltI      },
  { 'J',           ksAltJ      },
  { 'K',           ksAltK      },
  { 'L',           ksAltL      },
  { 'M',           ksAltM      },
  { 'N',           ksAltN      },
  { 'O',           ksAltO      },
  { 'P',           ksAltP      },
  { 'Q',           ksAltQ      },
  { 'R',           ksAltR      },
  { 'S',           ksAltS      },
  { 'T',           ksAltT      },
  { 'U',           ksAltU      },
  { 'V',           ksAltV      },
  { 'W',           ksAltW      },
  { 'X',           ksAltX      },
  { 'Y',           ksAltY      },
  { 'Z',           ksAltZ      },
  { '0',           ksAlt0      },
  { '1',           ksAlt1      },
  { '2',           ksAlt2      },
  { '3',           ksAlt3      },
  { '4',           ksAlt4      },
  { '5',           ksAlt5      },
  { '6',           ksAlt6      },
  { '7',           ksAlt7      },
  { '8',           ksAlt8      },
  { '9',           ksAlt9      },
  { 0,             0           }
};
#endif

#if !defined(USE_PDCURSES) || defined(XCURSES)
static chtype chars_0_31 [32] =
{
  ' ', 'O', 'O', '%',   0, '#', '*', 'o', 'o', 'o', 'o', '/', '+', 'd', 'A', '*',
    0,   0,   0, '!', '#', '$',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

static chtype chars_128_255 [128] =
{
  'C', 'u', 'e', 'a', 'a', 'a', 'a', 'c', 'e', 'e', 'e', 'i', 'i', 'i', 'A', 'A',
  'E', 'a', 'A', 'o', 'o', 'o', 'u', 'u', 'y', 'O', 'U', 'C',   0, 'Y', 'P', 'f',
  'a', 'i', 'o', 'u', 'n', 'N',   0,   0, '?',   0,   0, '/', '/', '!',   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  'a', 'B', 'T',   0, 'E', 'o', 'u', 't', 'O', 'O', 'O', 'O', 'o', 'o', 'E', 'A',
  '=',   0,   0,   0, '/', '/', '%', '=',   0, '.', '.', 'V', 'n', '2',   0, ' '
};
#endif

#ifndef HAVE_CRT_SETTEXTMODE
static void crt_settextmode (Boolean Columns40, Boolean Lines50)
{
  (void) Columns40;
  (void) Lines50;
  #ifdef USE_PDCURSES
  #ifdef XCURSES
  /* don't call resize_term() here -- leads to segfaults (PDCurses 2.4beta.22-May-99) */
  #else
  resize_term (Lines50 ? 50 : 25, Columns40 ? 40 : 80);
  resize_term (Lines50 ? 50 : 25, Columns40 ? 40 : 80); /* @@ does not always work the first time -- DJGPP bug? */
  crt_screen_size_changed = TRUE;
  #endif
  #endif
}
#endif

#ifndef HAVE_CRT_GETSHIFTSTATE
int crt_getshiftstate ()
{
  int state = crt_VirtualShiftState;
  #ifdef USE_PDCURSES
  unsigned long modifiers;
  DO_CRT_CHECK_WINCHANGED;
  modifiers = PDC_get_key_modifiers ();
  if (modifiers & PDC_KEY_MODIFIER_SHIFT  ) state |= shShift;
  if (modifiers & PDC_KEY_MODIFIER_CONTROL) state |= shCtrl;
  if (modifiers & PDC_KEY_MODIFIER_ALT    ) state |= shAlt;
  #ifdef XCURSES
  if (modifiers & PDC_KEY_MODIFIER_MOD4   ) state |= shExtra;
  if (modifiers & PDC_KEY_MODIFIER_MOD3   ) state = shAltGr;
  #else
  if (modifiers & PDC_KEY_MODIFIER_NUMLOCK) state |= shExtra;
  #endif
  #endif
  return state;
}
#endif

#ifndef HAVE_CRT_SIGNAL_HANDLERS
#ifdef SIGINT
static void int_handler (int sig)
{
  (void) sig;
  if (crt_CheckBreak) /* PDCurses under DJGPP gets a SIGINT for Ctrl-C even in raw mode */
    crt_signal_handler (0x100 * ksInt);
  signal (SIGINT, &int_handler);
}
#endif

#ifdef SIGTERM
static void term_handler (int sig)
{
  (void) sig;
  #ifdef XCURSES
  crt_ShutDown = TRUE;
  #endif
  crt_signal_handler (0x100 * ksTerm);
  signal (SIGTERM, &term_handler);
}
#endif

#ifdef SIGHUP
static void hup_handler (int sig)
{
  (void) sig;
  #ifdef XCURSES
  crt_ShutDown = TRUE;
  #endif
  crt_signal_handler (0x100 * ksHUp);
  signal (SIGHUP, &hup_handler);
}
#endif

#if defined(SIGPIPE) && defined(XCURSES)
static void pipe_handler (int sig)
{
  (void) sig;
  crt_ShutDown = TRUE;
  crt_signal_handler (0x100 * ksHUp);
  signal (SIGPIPE, SIG_IGN);
  if (crt_ShutDownJumpActive)
    longjmp (crt_ShutDownJump, 1);
}
#endif

#if defined(SIGTSTP) && !defined(XCURSES)
static void (*crt_old_tstp_handler) (int);

static void tstp_handler (int sig)
{
  (void) sig;
  signal (SIGTSTP, crt_old_tstp_handler);
  kill (getpid (), SIGTSTP);
  signal (SIGTSTP, &tstp_handler);
  crt_screen_size_changed = TRUE; /* might have changed while we were stopped */
}
#endif

#ifdef SIGWINCH
static void (*crt_old_winch_handler) (int);

static void winch_handler (int sig)
{
  crt_screen_size_changed = TRUE;
  crt_Signaled = 1;
  if (crt_old_winch_handler != SIG_ERR && crt_old_winch_handler != SIG_IGN && crt_old_winch_handler != SIG_DFL)
    (*crt_old_winch_handler) (sig);
  signal (SIGWINCH, &winch_handler);
}
#endif

static void crt_init_signals ()
{
#ifdef SIGINT
  signal (SIGINT, &int_handler);
#endif
#ifdef SIGTERM
  signal (SIGTERM, &term_handler);
#endif
#ifdef SIGHUP
  signal (SIGHUP, &hup_handler);
#endif
#if defined(SIGPIPE) && defined(XCURSES)
  signal (SIGPIPE, &pipe_handler);
#endif
#if defined(SIGTSTP) && !defined(XCURSES)
  crt_old_tstp_handler = signal (SIGTSTP, &tstp_handler);
#endif
#ifdef SIGWINCH
  crt_old_winch_handler = signal (SIGWINCH, &winch_handler);
#endif
}
#endif

#if !defined(HAVE_CRT_SOUND) && !defined(NO_CRT_DUMMY_SOUND)
void crt_sound (unsigned Hz)
{
}

void crt_nosound ()
{
}
#endif

#if defined(USE_NCURSES) || defined(XCURSES)

#include <sys/time.h>

/* Time for delayed refreshes */
#define REFRESH_SECONDS 0
#define REFRESH_MICROSECONDS 100000

static void alarm_handler (int sig)
{
  (void) sig;
  crt_refresh ();
}

static void no_alarm_handler (int sig)
{
  (void) sig;
}

/* Must call crt_refresh(), either immediately, or delayed (e.g.
   using a timer signal).
   - Calling crt_refresh() immediately is useful if curses'
     wrefresh() does not take a long time, like PDCurses under DJGPP
     which uses direct hardware access. The CRT window will then
     always be up to date. However, if wrefresh() takes a long time,
     this will slow down the program substantially.
   - Calling crt_refresh() delayed (e.g. Unix with ncurses): By
     using a timer, the number of calls to wrefresh() can be reduced
     so the program will not be slowed down too much. On the other
     hand, the timer guarantees that the window will not lag behind
     more than a specified amount of time. crt_refresh() is
     re-entrant, so the timer signal handler can call it without
     worries. Also, crt_schedule_refresh() will not be called when
     there is a pending refresh (i.e. crt_schedule_refresh() was
     called, but crt_refresh() was not called since then), so it can
     set the timer without worrying about disturbing a previously
     scheduled refresh.
*/
static void crt_schedule_refresh ()
{
  static struct itimerval timerval = { { 0, 0 }, { REFRESH_SECONDS, REFRESH_MICROSECONDS } };
  signal (SIGALRM, &alarm_handler);
  setitimer (ITIMER_REAL, &timerval, 0);
}

/* Called before a Delay is started, a (blocking) read is done, a
   child process is executed, or the update level is changed, i.e.
   when a scheduled refresh should not be done any more because it
   or its timer event might disturb another operation. If
   crt_schedule_refresh() has set a timer, this function must clear
   it, otherwise it might interfere with the Delay or the read, so
   e.g. Delay might get too short or the read will not read in all
   characters. crt_refresh() will be called after this function, so
   the timer can safely be cleared without losing the refresh. */
static void crt_stop_refresh ()
{
  static struct itimerval no_timerval = { { 0, 0 }, { 0, 0 } };
  signal (SIGALRM, &no_alarm_handler);
  setitimer (ITIMER_REAL, &no_timerval, 0);
}

#else

static inline void crt_schedule_refresh ()
{
  crt_refresh ();
}

static void crt_stop_refresh ()
{
}
#endif

static void crt_setcbreak ()
{
  if (crt_CheckBreak)
    {
      noraw ();
      cbreak ();
    }
  else
    raw ();
}

static void crt_nodelay (bool bf)
{
  crt_setcbreak ();
  nodelay (crt_ActivePanel -> w, bf);
  nodelay (stdscr, bf);
}

static void crt_checkcheckbreak ()
{
  if (crt_CheckBreak != crt_LastCheckBreak)
    {
      crt_LastCheckBreak = crt_CheckBreak;
      crt_setcbreak ();
    }
}

static TPanel CheckPanel (TPanel Panel)
{
  TPanel PList = crt_PanelList;
  while (PList && PList != Panel) PList = PList -> Next;
  return PList;
}

TPanel crt_PanelAbove (TPanel Panel);
TPanel crt_PanelAbove (TPanel Panel)
{
  PANEL *p;
  if (Panel && !CheckPanel (Panel)) return NULL;
  p = Panel ? Panel -> panel : NULL;
  do
    p = panel_above (p);
  while (p && !panel_userptr (p));
  return p ? (TPanel) panel_userptr (p) : NULL;
}

TPanel crt_PanelBelow (TPanel Panel);
TPanel crt_PanelBelow (TPanel Panel)
{
  PANEL *p;
  if (Panel && !CheckPanel (Panel)) return NULL;
  p = Panel ? Panel -> panel : NULL;
  do
    p = panel_below (p);
  while (p && !panel_userptr (p));
  return p ? (TPanel) panel_userptr (p) : NULL;
}

static TCursorShape GetAbsCursorPos (int *PosX, int *PosY, chtype *CharUnderCursor)
{
  int covered, x = 0, y = 0;
  TCursorShape shape;
  TPanel Panel, Panel2;
  for (Panel = crt_PanelBelow (NULL), shape = CursorIgnored;
       Panel && (shape = Panel -> CursorShape) == CursorIgnored;
       Panel = crt_PanelBelow (Panel));
  if (Panel)
    {
      getyx (Panel -> w, y, x);
      x += Panel -> LastWindMin & 0xff;
      y += Panel -> LastWindMin >> 8;
      for (covered = 0, Panel2 = crt_PanelAbove (Panel); !covered && Panel2; Panel2 = crt_PanelAbove (Panel2))
        if (x >= (int) (Panel2 -> LastWindMin & 0xff) && x <= (int) (Panel2 -> LastWindMax & 0xff) &&
            y >= (int) (Panel2 -> LastWindMin >> 8)   && y <= (int) (Panel2 -> LastWindMax >> 8)) covered = 1;
      if (covered) shape = CursorHidden;
      if (CharUnderCursor) *CharUnderCursor = winch (Panel -> w);
    }
  if (shape == CursorIgnored) shape = CursorHidden;
  *PosX = x;
  *PosY = y;
  return shape;
}

static void crt_update ()
{
  int x, y;
  TCursorShape shape;
  if (crt_screen_size_changed || crt_ShutDown) return;
  #ifdef XCURSES
  crt_ShutDownJumpActive = TRUE;
  if (setjmp (crt_ShutDownJump))
    return;
  #endif
  shape = GetAbsCursorPos (&x, &y, NULL);
  update_panels ();
  if (shape != crt_last_shape)
    {
      crt_last_shape = shape;
      curs_set ((shape == CursorHidden) ? 0 :
                (shape == CursorNormal) ? 1 : 2);
    }
  if (shape == CursorHidden)
    {
      SETCURSOR (0, 0);
      wmove (stdscr, 0, 0);
    }
  else
    {
      SETCURSOR (x, y);
      wmove (stdscr, y, x);
    }
  doupdate ();
  #ifdef XCURSES
  crt_ShutDownJumpActive = FALSE;
  #endif
}

/* Must be re-entrant! */
static void crt_refresh ()
{
  if (crt_refresh_inhibit == 0)
    {
      crt_refresh_inhibit++;
      crt_pending_refresh = FALSE;
      crt_update ();
      (void) idlok (crt_ActivePanel -> w, FALSE);
      crt_refresh_inhibit--;
    }
  else
    crt_refresh_flag++;
}

static void crt_need_refresh ()
{
  crt_checkcheckbreak ();
  if (crt_update_level >= UpdateAlways || crt_refresh_flag)
    {
      crt_refresh_flag = 0;
      crt_refresh ();
    }
  else if (crt_update_level >= UpdateRegularly && !crt_pending_refresh)
    {
      crt_pending_refresh = TRUE;
      crt_schedule_refresh ();
    }
}

void crt_SimulateBlockCursor (void);
void crt_SimulateBlockCursor ()
{
  if (!crt_SimulateBlockCursorActive)
    {
      int x, y;
      chtype CharUnderCursor;
      if (GetAbsCursorPos (&x, &y, &CharUnderCursor) >= CursorFat)
        {
          crt_SimulateBlockCursorActive = TRUE;
          if (!crt_SimulateBlockCursorPanel)
            {
              WINDOW *w = newwin (1, 1, y, x);
              if (!w) DO_RETURN_ADDRESS (crt_fatal (5));
              keypad (w, TRUE);
              leaveok (w, FALSE);
              crt_SimulateBlockCursorPanel = new_panel (w);
              set_panel_userptr (crt_SimulateBlockCursorPanel, NULL);
            }
          else
            move_panel (crt_SimulateBlockCursorPanel, y, x);
          mvwaddch (panel_window (crt_SimulateBlockCursorPanel), 0, 0,
            (crt_linux_console ? (chBlock | A_ALTCHARSET) : ACS_BLOCK)
            | (CharUnderCursor & A_ATTRIBUTES & ~A_ALTCHARSET));
          show_panel (crt_SimulateBlockCursorPanel);
          top_panel (crt_SimulateBlockCursorPanel);
        }
    }
  else
    {
      crt_SimulateBlockCursorActive = FALSE;
      if (crt_SimulateBlockCursorPanel) hide_panel (crt_SimulateBlockCursorPanel);
    }
  crt_need_refresh ();
}

void crt_SimulateBlockCursorOff (void);
void crt_SimulateBlockCursorOff ()
{
  if (crt_SimulateBlockCursorActive) crt_SimulateBlockCursor ();
}

void crt_PanelHide (TPanel Panel);
void crt_PanelHide (TPanel Panel)
{
  if (CheckPanel (Panel) && !Panel -> Hidden)
    {
      crt_refresh_inhibit++;
      hide_panel (Panel -> panel);
      Panel -> Hidden = TRUE;
      crt_refresh_inhibit--;
      crt_need_refresh ();
    }
}

void crt_PanelShow (TPanel Panel);
void crt_PanelShow (TPanel Panel)
{
  if (CheckPanel (Panel) && Panel -> Hidden)
    {
      crt_refresh_inhibit++;
      show_panel (Panel -> panel);
      Panel -> Hidden = FALSE;
      crt_refresh_inhibit--;
      crt_need_refresh ();
    }
}

Boolean crt_PanelHidden (TPanel Panel);
Boolean crt_PanelHidden (TPanel Panel)
{
  return CheckPanel (Panel) && Panel -> Hidden;
}

void crt_PanelTop (TPanel Panel);
void crt_PanelTop (TPanel Panel)
{
  if (!CheckPanel (Panel)) return;
  if (Panel -> Hidden) crt_PanelShow (Panel);
  crt_refresh_inhibit++;
  top_panel (Panel -> panel);
  crt_refresh_inhibit--;
  crt_need_refresh ();
}

void crt_PanelBottom (TPanel Panel);
void crt_PanelBottom (TPanel Panel)
{
  if (!CheckPanel (Panel)) return;
  if (Panel -> Hidden) crt_PanelShow (Panel);
  crt_refresh_inhibit++;
  bottom_panel (Panel -> panel);
  crt_refresh_inhibit--;
  crt_need_refresh ();
}

void crt_PanelMoveAbove (TPanel Panel, TPanel Above);
void crt_PanelMoveAbove (TPanel Panel, TPanel Above)
{
  if (!CheckPanel (Panel)) return;
  if (!CheckPanel (Above))
    crt_PanelBottom (Panel);
  else if (Above -> Hidden)
    crt_PanelHide (Panel);
  else if (Above != Panel)
    {
      PANEL *p, *p2;
      if (Panel -> Hidden) crt_PanelShow (Panel);
      crt_refresh_inhibit++;
      top_panel (Panel -> panel);
      p = panel_above (Above -> panel);
      while (p && p != Panel -> panel)
        {
          p2 = panel_above (p);
          top_panel (p);
          p = p2;
        }
      crt_refresh_inhibit--;
      crt_need_refresh ();
    }
}

void crt_PanelMoveBelow (TPanel Panel, TPanel Below);
void crt_PanelMoveBelow (TPanel Panel, TPanel Below)
{
  if (!CheckPanel (Panel)) return;
  if (!CheckPanel (Below))
    crt_PanelTop (Panel);
  else if (Below -> Hidden)
    crt_PanelHide (Panel);
  else if (Below != Panel)
    {
      PANEL *p, *p2;
      if (Panel -> Hidden) crt_PanelShow (Panel);
      crt_refresh_inhibit++;
      bottom_panel (Panel -> panel);
      p = panel_below (Below -> panel);
      while (p && p != Panel -> panel)
        {
          p2 = panel_below (p);
          bottom_panel (p);
          p = p2;
        }
      crt_refresh_inhibit--;
      crt_need_refresh ();
    }
}

void crt_PanelBindToBackground (TPanel Panel, Boolean Bind);
void crt_PanelBindToBackground (TPanel Panel, Boolean Bind)
{
  if (CheckPanel (Panel)) Panel -> BoundToBackground = Bind;
}

Boolean crt_PanelIsBoundToBackground (TPanel Panel);
Boolean crt_PanelIsBoundToBackground (TPanel Panel)
{
  return CheckPanel (Panel) && Panel -> BoundToBackground;
}

static WINDOW *crt_newwin (int x1, int y1, int x2, int y2, Boolean BoundToBackground)
{
  int xsize = x2 - x1 + 1, ysize = y2 - y1 + 1;
  WINDOW *w = newwin (ysize, xsize, y1, x1);
  if (!w) DO_RETURN_ADDRESS (crt_fatal (5));
  if (BoundToBackground)
    {
      chtype Buffer [xsize + 1];
      int yc;
      for (yc = 0; yc < ysize; yc++)
        {
          mvwinchnstr (stdscr, yc + y1, x1, Buffer, xsize);
          mvwaddchnstr (w, yc, 0, Buffer, xsize);
        }
    }
  keypad (w, TRUE);
  leaveok (w, FALSE);
  crt_last_shape = - 1;
  return w;
}

/* Save contents of the panel to stdscr to imitate BP behaviour */
static void savetostdscr (TPanel Panel)
{
  if (Panel -> BoundToBackground)
    {
      PANEL *panel;
      int yc,
          LastXMin = Panel -> LastWindMin & 0xff,
          LastYMin = Panel -> LastWindMin >> 8,
          LastXSize = (Panel -> LastWindMax & 0xff) - LastXMin + 1,
          LastYSize = (Panel -> LastWindMax >> 8) - LastYMin + 1;
      chtype Buffer [LastXSize + 1];
      for (yc = 0; yc < LastYSize; yc++)
        {
          mvwinchnstr (Panel -> w, yc, 0, Buffer, LastXSize);
          mvwaddchnstr (stdscr, yc + LastYMin, LastXMin, Buffer, LastXSize);
        }
      wnoutrefresh (stdscr);
      for (panel = panel_above (NULL); panel; panel = panel_above (panel))
        touchwin (panel_window (panel));
    }
}

static void crt_dosetscroll ()
{
  scrollok (crt_ActivePanel -> w, crt_ActivePanel -> ScrollState);
}

static void update_crt_cursor_pos ()
{
  getyx (crt_ActivePanel -> w, crt_cursor_pos.y, crt_cursor_pos.x);
}

static void win_changed ()
{
  int x1, y1, x2, y2, x, y;
  WINDOW *oldwin = crt_ActivePanel -> w;
  crt_refresh_inhibit++;
  x2 = WindXMax; if (x2 < 0) x2 = 0; if (x2 >= crt_ScreenSize.x) x2 = crt_ScreenSize.x - 1;
  y2 = WindYMax; if (y2 < 0) y2 = 0; if (y2 >= crt_ScreenSize.y) y2 = crt_ScreenSize.y - 1;
  x1 = WindXMin; if (x1 < 0) x1 = 0; if (x1 > x2) x1 = x2;
  y1 = WindYMin; if (y1 < 0) y1 = 0; if (y1 > y2) y1 = y2;
  getyx (crt_ActivePanel -> w, y, x);
  x += crt_ActivePanel -> LastWindMin & 0xff;
  y += crt_ActivePanel -> LastWindMin >> 8;
  savetostdscr (crt_ActivePanel);
  crt_ActivePanel -> w = crt_newwin (x1, y1, x2, y2, crt_ActivePanel -> BoundToBackground);
  replace_panel (crt_ActivePanel -> panel, crt_ActivePanel -> w);
  delwin (oldwin);
  crt_last_shape = - 1;
  crt_dosetscroll ();
  if (x < x1) x = x1;
  if (x > x2) x = x2;
  if (y < y1) y = y1;
  if (y > y2) y = y2;
  wmove (crt_ActivePanel -> w, y - y1, x - x1);
  crt_ActivePanel -> LastWindMin = crt_WindMin = x1 + (y1 << 8);
  crt_ActivePanel -> LastWindMax = crt_WindMax = x2 + (y2 << 8);
  crt_refresh_inhibit--;
  update_crt_cursor_pos ();
  crt_need_refresh ();
}

static void crt_UpdateDataToActivePanel ()
{
  if (crt_ActivePanel)
    {
      crt_ActivePanel -> TextAttr = crt_TextAttr;
      crt_ActivePanel -> WindMin  = crt_WindMin;
      crt_ActivePanel -> WindMax  = crt_WindMax;
    }
}

static void crt_UpdateDataFromActivePanel ()
{
  if (crt_ActivePanel)
    {
      crt_TextAttr = crt_ActivePanel -> TextAttr;
      crt_WindMin = crt_ActivePanel -> WindMin;
      crt_WindMax = crt_ActivePanel -> WindMax;
      update_crt_cursor_pos ();
    }
}

static void crt_PanelActivate_Internal (TPanel Panel)
{
  if (!CheckPanel (Panel)) DO_RETURN_ADDRESS (crt_fatal (4));
  if (Panel == crt_ActivePanel) return;
  crt_UpdateDataToActivePanel ();
  crt_ActivePanel = Panel;
  crt_UpdateDataFromActivePanel ();
}

static void crt_PanelNew_Internal (int x1, int y1, int x2, int y2, Boolean BindToBackground)
{
  TPanel NewPanel = malloc (sizeof (TPanelData));
  crt_refresh_inhibit++;
  NewPanel -> Next = crt_PanelList;
  crt_PanelList = NewPanel;
  if (x2 < 1) x2 = 1; if (x2 > crt_ScreenSize.x) x2 = crt_ScreenSize.x;
  if (y2 < 1) y2 = 1; if (y2 > crt_ScreenSize.y) y2 = crt_ScreenSize.y;
  if (x1 < 1) x1 = 1; if (x1 > x2) x1 = x2;
  if (y1 < 1) y1 = 1; if (y1 > y2) y1 = y2;
  NewPanel -> BoundToBackground = BindToBackground;
  NewPanel -> Hidden = FALSE;
  NewPanel -> ScrollState     = crt_ActivePanel ? crt_ActivePanel -> ScrollState     : TRUE;
  NewPanel -> PCCharSet       = crt_ActivePanel ? crt_ActivePanel -> PCCharSet       : crt_PCCharSet;
  NewPanel -> UseControlChars = crt_ActivePanel ? crt_ActivePanel -> UseControlChars : TRUE;
  NewPanel -> CursorShape     = crt_ActivePanel ? crt_ActivePanel -> CursorShape     : CursorNormal;
  NewPanel -> TextAttr        = crt_TextAttr;
  NewPanel -> w = crt_newwin (x1 - 1, y1 - 1, x2 - 1, y2 - 1, NewPanel -> BoundToBackground);
  NewPanel -> panel = new_panel (NewPanel -> w);
  set_panel_userptr (NewPanel -> panel, (void *) NewPanel);
  NewPanel -> LastWindMin = NewPanel -> WindMin = (x1 - 1) + ((y1 - 1) << 8);
  NewPanel -> LastWindMax = NewPanel -> WindMax = (x2 - 1) + ((y2 - 1) << 8);
  crt_PanelActivate_Internal (NewPanel);
  wmove (NewPanel -> w, 0, 0);
  update_crt_cursor_pos ();
  crt_dosetscroll ();
  crt_refresh_inhibit--;
}

static void GetLastMode ()
{
  if (crt_HasColors)
    {
      crt_LastMode = 0;
      if (crt_ColorFlag)          crt_LastMode += 1;
      if (crt_ScreenSize.x >= 80) crt_LastMode += 2;
    }
  else
    crt_LastMode = 7;
  if (crt_ScreenSize.y >= 43) crt_LastMode += 0x100;
  crt_IsMonoMode = crt_modeismono (crt_LastMode);
}

static int crt_attr2cursattr (TTextAttr attr)
{
  #ifdef XCURSES
  /* PDCurses under X has serious problems writing black on black. :-(
     So we write some other color on black and output spaces (see chtransform()) */
  if ((attr & ~0x88) == 0) attr = 7;
  #endif
  return (crt_ColorFlag ? crt_attrs : crt_monoattrs) [attr & 7] [(attr >> 4) & 7] |
         (((attr & 8) && (crt_ColorFlag || (attr & 0x77))) ? A_BOLD : 0) |
         ((attr & 0x80) ? A_BLINK : 0);
}

static void crt_rawout (Boolean flag)
{
  #ifdef HAVE_RAW_OUTPUT
  raw_output (flag);
  #else
  (void) flag;
  #endif
}

static void crt_set_typeahead ()
{
  typeahead ((crt_update_level >= UpdateInput) ? - 1 : crt_get_input_fd ());
}

void crt_init (void);
void crt_init ()
{
  int fg, bg, c, cursattr;
  if (crt_inited) return;
  SAVE_RETURN_ADDRESS;
  #ifdef XCURSES
  XCursesProgramName = crt_get_program_name ();
  #endif
  crt_screen_size_changed = FALSE;
  if (!initscr ()) crt_fatal (1);
  crt_inited = 1;
  #ifdef XCURSES
  sleep (1); /* @@There is a problem with XCurses not always starting up
                correctly (race condition?). Until it is solved, this
                sleep() is an ugly and unreliable way to avoid it
                sometimes...  */
  #endif
  #ifdef USE_PDCURSES
  PDC_save_key_modifiers (TRUE);
  #else
  crt_linux_console = strncmp (termname (), "linux", 5) == 0;
  #endif
  #if !defined(USE_PDCURSES) || defined(XCURSES)
  chars_0_31 [ 4] = ACS_DIAMOND;
  chars_0_31 [16] = ACS_RARROW;
  chars_0_31 [17] = ACS_LARROW;
  chars_0_31 [18] = ACS_VLINE;
  chars_0_31 [22] = ACS_S9;
  chars_0_31 [23] = ACS_DARROW;
  chars_0_31 [24] = ACS_UARROW;
  chars_0_31 [25] = ACS_DARROW;
  chars_0_31 [26] = ACS_RARROW;
  chars_0_31 [27] = ACS_LARROW;
  chars_0_31 [28] = ACS_LLCORNER;
  chars_0_31 [29] = ACS_HLINE;
  chars_0_31 [30] = ACS_UARROW;
  chars_0_31 [31] = ACS_DARROW;
  chars_128_255 [ 28] = ACS_STERLING;
  chars_128_255 [ 38] = ACS_DEGREE;
  chars_128_255 [ 39] = ACS_DEGREE;
  chars_128_255 [ 41] = ACS_ULCORNER;
  chars_128_255 [ 42] = ACS_URCORNER;
  chars_128_255 [ 46] = ACS_LARROW;
  chars_128_255 [ 47] = ACS_RARROW;
  chars_128_255 [ 48] = ACS_CKBOARD;
  chars_128_255 [ 49] = ACS_CKBOARD;
  chars_128_255 [ 50] = ACS_CKBOARD;
  chars_128_255 [ 51] = ACS_VLINE;
  chars_128_255 [ 52] = ACS_RTEE;
  chars_128_255 [ 53] = ACS_RTEE;
  chars_128_255 [ 54] = ACS_RTEE;
  chars_128_255 [ 55] = ACS_URCORNER;
  chars_128_255 [ 56] = ACS_URCORNER;
  chars_128_255 [ 57] = ACS_RTEE;
  chars_128_255 [ 58] = ACS_VLINE;
  chars_128_255 [ 59] = ACS_URCORNER;
  chars_128_255 [ 60] = ACS_LRCORNER;
  chars_128_255 [ 61] = ACS_LRCORNER;
  chars_128_255 [ 62] = ACS_LRCORNER;
  chars_128_255 [ 63] = ACS_URCORNER;
  chars_128_255 [ 64] = ACS_LLCORNER;
  chars_128_255 [ 65] = ACS_BTEE;
  chars_128_255 [ 66] = ACS_TTEE;
  chars_128_255 [ 67] = ACS_LTEE;
  chars_128_255 [ 68] = ACS_HLINE;
  chars_128_255 [ 69] = ACS_PLUS;
  chars_128_255 [ 70] = ACS_LTEE;
  chars_128_255 [ 71] = ACS_LTEE;
  chars_128_255 [ 72] = ACS_LLCORNER;
  chars_128_255 [ 73] = ACS_ULCORNER;
  chars_128_255 [ 74] = ACS_BTEE;
  chars_128_255 [ 75] = ACS_TTEE;
  chars_128_255 [ 76] = ACS_LTEE;
  chars_128_255 [ 77] = ACS_HLINE;
  chars_128_255 [ 78] = ACS_PLUS;
  chars_128_255 [ 79] = ACS_BTEE;
  chars_128_255 [ 80] = ACS_BTEE;
  chars_128_255 [ 81] = ACS_TTEE;
  chars_128_255 [ 82] = ACS_TTEE;
  chars_128_255 [ 83] = ACS_LLCORNER;
  chars_128_255 [ 84] = ACS_LLCORNER;
  chars_128_255 [ 85] = ACS_ULCORNER;
  chars_128_255 [ 86] = ACS_ULCORNER;
  chars_128_255 [ 87] = ACS_PLUS;
  chars_128_255 [ 88] = ACS_PLUS;
  chars_128_255 [ 89] = ACS_LRCORNER;
  chars_128_255 [ 90] = ACS_ULCORNER;
  chars_128_255 [ 91] = ACS_BLOCK;
  chars_128_255 [ 92] = ACS_BLOCK;
  chars_128_255 [ 93] = ACS_BLOCK;
  chars_128_255 [ 94] = ACS_BLOCK;
  chars_128_255 [ 95] = ACS_BLOCK;
  chars_128_255 [ 99] = ACS_PI;
  chars_128_255 [113] = ACS_PLMINUS;
  chars_128_255 [114] = ACS_GEQUAL;
  chars_128_255 [115] = ACS_LEQUAL;
  chars_128_255 [120] = ACS_DEGREE;
  chars_128_255 [126] = ACS_BULLET;
  #endif
  cbreak ();
  noecho ();
  scrollok (stdscr, TRUE);
  crt_rawout (TRUE);
  crt_ColorFlag = crt_HasColors = has_colors ();
  if (crt_HasColors)
    {
      start_color ();
      c = 0;
      for (bg = 0; bg < 8; bg++)
        for (fg = 0; fg < 8; fg++)
          if (INVIS_WORKS && bg == fg && bg > 0)
            crt_attrs [fg] [bg] = crt_attrs [0] [bg] | A_INVIS;
          else if (REVERSE_WORKS && fg < bg)
            crt_attrs [fg] [bg] = crt_attrs [bg] [fg] | A_REVERSE;
          else
            {
              if (init_pair (++c, crt_colors [fg], crt_colors [bg]) == ERR)
                {
                  fprintf (stderr, "could not create color pair (%i,%i)", fg, bg);
                  crt_inited = 0;
                  #ifdef XCURSES
                  XCursesExit ();
                  #endif
                  exit (1);
                }
              crt_attrs [fg] [bg] = COLOR_PAIR (c);
            }
      for (bg = 0; bg < 8; bg++)
        for (fg = 0; fg < 8; fg++)
          crt_monoattrs [fg] [bg] = crt_attrs [7] [0];
      crt_monoattrs [0] [0] = crt_attrs [0] [0];
      crt_monoattrs [1] [0] = crt_attrs [1] [0];
      crt_monoattrs [0] [7] = crt_attrs [0] [7];
    }
  else
    {
      for (bg = 0; bg < 8; bg++)
        for (fg = 0; fg < 8; fg++)
          crt_monoattrs [fg] [bg] = A_NORMAL;
      crt_monoattrs [0] [0] = A_INVIS;
      crt_monoattrs [1] [0] = A_UNDERLINE;
      crt_monoattrs [0] [7] = A_REVERSE;
    }
  cursattr = crt_attr2cursattr (crt_NormAttr);
  attrset (cursattr);
  bkgdset (cursattr);
  erase ();
  crt_init_signals ();
  keypad (stdscr, TRUE);
  getmaxyx (stdscr, crt_ScreenSize.y, crt_ScreenSize.x);
  wrefresh (stdscr); /* prevents flickering at the beginning of XCurses programs */
  GetLastMode ();
  crt_PanelNew_Internal (1, 1, crt_ScreenSize.x, crt_ScreenSize.y, TRUE);
  typeahead (- 1);
  crt_set_typeahead ();
  crt_registerrestoreterminal ();
  RESTORE_RETURN_ADDRESS;
}

static void crt_bp_autoinit ()
{
  static Boolean initializing = FALSE;
  if (initializing) /* *crt_auto_init_proc causes CRT to auto init again (bad boy)... */
    initializing = FALSE;
  else
    {
      initializing = TRUE;
      if (crt_auto_init_proc) (*crt_auto_init_proc) ();
      if (!initializing) return;
      initializing = FALSE;
    }
  crt_PCCharSet = TRUE;
  if (!crt_UpdateLevelChanged) crt_update_level = UpdateRegularly;
  crt_init ();
}

static void adjust_size (int *x1, int *y1, int *x2, int *y2, TPoint LastScreenSize)
{
  if (*x2 >= crt_ScreenSize.x)
    {
      *x1 -= (*x2 - crt_ScreenSize.x + 1);
      if (*x1 < 0) *x1 = 0;
      *x2 = crt_ScreenSize.x - 1;
    }
  else if (*x1 == 0 && *x2 == LastScreenSize.x - 1)
    *x2 = crt_ScreenSize.x - 1;
  if (*y2 >= crt_ScreenSize.y)
    {
      *y1 -= (*y2 - crt_ScreenSize.y + 1);
      if (*y1 < 0) *y1 = 0;
      *y2 = crt_ScreenSize.y - 1;
    }
  else if (*y1 == 0 && *y2 == LastScreenSize.y - 1)
    *y2 = crt_ScreenSize.y - 1;
}

static void adjust_panel (PANEL *panel, WINDOW **w, int x1, int y1, int x2, int y2)
{
  int x, y, xsize = x2 - x1 + 1, ysize = y2 - y1 + 1;
  WINDOW *new;
  /* It's not very clear what has to be updated in the windows and
     panels, and in which order (in the different curses libraries).
     So, we rather do too much than too little. First resize, then
     move, otherwise the move could fail, because the window might
     not fit on the screen. */
  getyx ((*w), y, x);
  new = resize_window ((*w), ysize, xsize);
  if (new) *w = new;
  panel->win = *w;
  replace_panel (panel, (*w));
  move_panel (panel, y1, x1);
  if (x >= xsize) x = xsize - 1;
  if (y >= ysize) y = ysize - 1;
  wmove ((*w), y, x);
  keypad ((*w), TRUE);
  leaveok ((*w), FALSE);
}

void crt_check_winchanged ()
{
  PANEL *panel;
  TPanel p;
  TPoint LastScreenSize;
  SAVE_RETURN_ADDRESS;
  if (!crt_inited) crt_bp_autoinit ();
  #ifdef USE_PDCURSES
  /* In XCurses, this wouldn't be necessary (but doesn't hurt)
     because the program will get a SIGWINCH, but e.g. under mingw,
     it doesn't get a SIGWINCH, so we need this additional check. */
  if (is_termresized ()) crt_screen_size_changed = TRUE;
  #endif
  if (crt_screen_size_changed)
    {
      crt_refresh_inhibit++;
      #ifdef USE_PDCURSES
      /* XCurses: resize the terminal after the user resized the window */
      if (is_termresized ()) resize_term (0, 0);
      /* work around a bug in the PDCurses panel library */
      {
        extern PANEL __stdscr_pseudo_panel;
        __stdscr_pseudo_panel.win = stdscr;
        __stdscr_pseudo_panel.wendy = LINES;
        __stdscr_pseudo_panel.wendx = COLS;
      }
      #endif
      napms (100); /* wait for a new unimap to be loaded if necessary */
      /* The following code is a bit tricky...
         doupdate() will update the screen size in ncurses, but the
         size or position of any window may have become invalid, so
         they must not be updated now! */
      untouchwin (stdscr);
      for (panel = panel_above (NULL); panel; panel = panel_above (panel))
        untouchwin (panel_window (panel));
      if (!crt_ShutDown)
        doupdate ();
      LastScreenSize = crt_ScreenSize;
      getmaxyx (stdscr, crt_ScreenSize.y, crt_ScreenSize.x);
      /* Resize all CRT panels */
      crt_UpdateDataToActivePanel ();
      for (p = crt_PanelList; p; p = p -> Next)
        {
          Boolean WinChanged = p -> WindMin != p -> LastWindMin ||
                               p -> WindMax != p -> LastWindMax;
          int x1, y1, x2, y2;
          unsigned int NewWindMin, NewWindMax;
          x1 = (p -> LastWindMin & 0xff);
          y1 = (p -> LastWindMin >> 8);
          x2 = (p -> LastWindMax & 0xff);
          y2 = (p -> LastWindMax >> 8);
          adjust_size (&x1, &y1, &x2, &y2, LastScreenSize);
          NewWindMin = x1 + (y1 << 8);
          NewWindMax = x2 + (y2 << 8);
          if (NewWindMin != p -> LastWindMin || NewWindMax != p -> LastWindMax)
            {
              p -> LastWindMin = NewWindMin;
              p -> LastWindMax = NewWindMax;
              if (!WinChanged)
                {
                  p -> WindMin = p -> LastWindMin;
                  p -> WindMax = p -> LastWindMax;
                }
              adjust_panel (p -> panel, &p -> w, x1, y1, x2, y2);
              if (p -> Hidden) hide_panel (p -> panel);
            }
        }
      /* Handle panels that don't belong to CRT windows (e.g., the one
         for the simulated block cursor) */
      for (panel = panel_above (NULL); panel; panel = panel_above (panel))
        if (!panel_userptr (panel))
          {
            WINDOW *w = panel_window (panel);
            int x1, y1, x2, y2, xsize, ysize, oldx1, oldy1, oldx2, oldy2;
            getbegyx (w, y1, x1);
            getmaxyx (w, ysize, xsize);
            oldx1 = x1;
            oldy1 = y1;
            oldx2 = x2 = x1 + xsize - 1;
            oldy2 = y2 = y1 + ysize - 1;
            adjust_size (&x1, &y1, &x2, &y2, LastScreenSize);
            if (x1 != oldx1 || y1 != oldy1 || x2 != oldx2 || y2 != oldy2)
              adjust_panel (panel, &w, x1, y1, x2, y2);
          }
      crt_UpdateDataFromActivePanel ();
      /* Now we can refresh everything */
      touchwin (stdscr);
      for (panel = panel_above (NULL); panel; panel = panel_above (panel))
        touchwin (panel_window (panel));
      /* Overcome ncurses' buffering of the cursor shape, because sometimes
         when the screen size is changed (e.g. by SVGATextMode), also the
         cursor shape is reset. */
      curs_set (1);
      curs_set (0);
      crt_last_shape = CursorHidden;
      crt_dosetscroll ();
      crt_update ();
      crt_screen_size_changed = FALSE;
      GetLastMode ();
      crt_refresh_inhibit--;
      if (crt_key_buf == 0) crt_key_buf = KEY_RESIZE;
    }
  if (crt_WindMin != crt_ActivePanel -> LastWindMin || crt_WindMax != crt_ActivePanel -> LastWindMax)
    win_changed ();
  RESTORE_RETURN_ADDRESS;
}

void crt_setscroll (Boolean state);
void crt_setscroll (Boolean state)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_ActivePanel -> ScrollState = state;
  crt_dosetscroll ();
}

Boolean crt_getscroll (void);
Boolean crt_getscroll ()
{
  DO_CRT_CHECK_WINCHANGED;
  return crt_ActivePanel -> ScrollState;
}

void crt_setcursorshape (TCursorShape shape);
void crt_setcursorshape (TCursorShape shape)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_ActivePanel -> CursorShape = shape;
  crt_need_refresh ();
}

TCursorShape crt_getcursorshape (void);
TCursorShape crt_getcursorshape ()
{
  DO_CRT_CHECK_WINCHANGED;
  return crt_ActivePanel -> CursorShape;
}

TPanel crt_GetActivePanel (void);
TPanel crt_GetActivePanel ()
{
  DO_CRT_CHECK_WINCHANGED;
  return crt_ActivePanel;
}

void crt_PanelActivate (TPanel Panel);
void crt_PanelActivate (TPanel Panel)
{
  if (Panel == crt_ActivePanel) return;
  DO_CRT_CHECK_WINCHANGED;
  DO_RETURN_ADDRESS (crt_PanelActivate_Internal (Panel));
}

void crt_PanelNew (int x1, int y1, int x2, int y2, Boolean BindToBackground);
void crt_PanelNew (int x1, int y1, int x2, int y2, Boolean BindToBackground)
{
  DO_CRT_CHECK_WINCHANGED;
  DO_RETURN_ADDRESS (crt_PanelNew_Internal (x1, y1, x2, y2, BindToBackground));
  crt_need_refresh ();
}

void crt_PanelDelete (TPanel Panel);
void crt_PanelDelete (TPanel Panel)
{
  TPanel *PList = &crt_PanelList;
  while (*PList && *PList != Panel) PList = &((*PList) -> Next);
  if (!*PList) DO_RETURN_ADDRESS (crt_fatal (2));
  DO_CRT_CHECK_WINCHANGED;
  crt_refresh_inhibit++;
  *PList = Panel -> Next;
  if (!crt_PanelList) DO_RETURN_ADDRESS (crt_fatal (3));
  savetostdscr (Panel);
  del_panel (Panel -> panel);
  delwin (Panel -> w);
  free (Panel);
  if (Panel == crt_ActivePanel)
    {
      crt_ActivePanel = NULL;
      crt_PanelActivate_Internal (crt_PanelList);
    }
  crt_last_shape = - 1;
  crt_refresh_inhibit--;
  crt_need_refresh ();
}

static TTextAttr cursattr2attr (int cursattr)
{
  int attr = 0;
  cursattr &= (A_ATTRIBUTES & ~A_ALTCHARSET);
  if (crt_attr2cursattr (crt_NormAttr) == cursattr) return crt_NormAttr;
  while (attr < 0x100 && crt_attr2cursattr (attr) != cursattr) attr++;
  return (attr == 0x100) ? crt_NormAttr : attr;
}

static void crt_setattr ()
{
  int cursattr;
  DO_CRT_CHECK_WINCHANGED;
  cursattr = crt_attr2cursattr (crt_TextAttr);
  wattrset (crt_ActivePanel -> w, cursattr);
  wbkgdset (crt_ActivePanel -> w, cursattr | ' ');
}

void crt_update_immediately (void);
void crt_update_immediately ()
{
  DO_CRT_CHECK_WINCHANGED;
  if (crt_update_level == UpdateRegularly) crt_stop_refresh ();
  typeahead (- 1);
  crt_refresh ();
  crt_set_typeahead ();
}

void crt_redraw (void);
void crt_redraw ()
{
  PANEL *panel;
  DO_CRT_CHECK_WINCHANGED;
  touchwin (stdscr);
  for (panel = panel_above (NULL); panel; panel = panel_above (panel))
    touchwin (panel_window (panel));
  clearok (curscr, TRUE);
  crt_last_shape = - 1;
  crt_update ();
}

void crt_setupdatelevel (TCRTUpdate level);
void crt_setupdatelevel (TCRTUpdate level)
{
  if (crt_inited)
    {
      DO_CRT_CHECK_WINCHANGED;
      if (crt_update_level == UpdateRegularly) crt_update_immediately ();
    }
  crt_update_level = level;
  crt_UpdateLevelChanged = TRUE;
  if (crt_inited) crt_set_typeahead ();
}

void crt_restore_terminal_no_crt (void);
void crt_restore_terminal_no_crt ()
{
  if (!crt_inited || crt_terminal_no_crt || crt_ShutDown) return;
  if (crt_ActivePanel) DO_RETURN_ADDRESS (crt_update_immediately ());
  crt_terminal_no_crt = TRUE;
  #ifdef USE_PDCURSES
  savetty ();
  #endif
  endwin ();
  #if defined(SIGTSTP) && !defined(XCURSES)
  signal (SIGTSTP, SIG_DFL);
  #endif
}

void crt_restore_terminal_crt (void);
void crt_restore_terminal_crt ()
{
  if (crt_ShutDown) return;
  DO_CRT_CHECK_WINCHANGED;
  #ifdef USE_PDCURSES
  resetty ();
  #endif
  if (!crt_inited || !crt_terminal_no_crt) return;
  #if defined(SIGTSTP) && !defined(XCURSES)
  signal (SIGTSTP, &tstp_handler);
  #endif
  crt_rawout (TRUE);
  clearok (curscr, TRUE);
  crt_last_shape = - 1;
  crt_update ();
  #ifdef USE_PDCURSES
  PDC_save_key_modifiers (TRUE);
  #endif
  crt_terminal_no_crt = FALSE;
}

void crt_clearterminal (void);
void crt_clearterminal ()
{
  crt_restore_terminal_no_crt ();
  crt_clear_terminal ();
}

void crt_done (void);
void crt_done ()
{
  if (!crt_inited || crt_ShutDown) return;
  if (crt_ActivePanel && !crt_terminal_no_crt)
    {
      DO_RETURN_ADDRESS (crt_update_immediately ());
      endwin ();
    }
  crt_inited = 0;
  #ifdef XCURSES
  XCursesExit ();
  #endif
}

void crt_set_curses_mode (Boolean on);
void crt_set_curses_mode (Boolean on)
{
  if (!crt_inited || crt_ShutDown) return;
  if (on)
    {
      reset_prog_mode ();
      crt_rawout (TRUE);
    }
  else
    {
      crt_rawout (FALSE);
      reset_shell_mode ();
    }
}

void crt_delay (unsigned ms);
void crt_delay (unsigned ms)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_update_immediately ();
  napms (ms);
}

static int get_ch (void)
{
  int i;
  if (crt_ShutDown)
    return KEY_HUP;
  #ifdef XCURSES
  crt_ShutDownJumpActive = TRUE;
  if (setjmp (crt_ShutDownJump))
    i = KEY_HUP;
  else
  #endif
    i = wgetch (stdscr); /* use stdscr rather than crt_ActivePanel -> w, so a non-top panel will not be redrawn */
  #ifdef XCURSES
  crt_ShutDownJumpActive = FALSE;
  #endif
  return i;
}

static int wgetk ()
{
  int i = get_ch ();
  if (i == KEY_RESIZE) crt_screen_size_changed = TRUE;
  if (crt_screen_size_changed)
    {
      DO_CRT_CHECK_WINCHANGED;
      if (i != KEY_RESIZE && i != ERR) crt_key_buf = i;
      if (crt_key_buf == KEY_RESIZE) crt_key_buf = 0;
      i = KEY_RESIZE;
    }
  return i;
}

void crt_select (void *PrivateData, Boolean *ReadSelect, Boolean *WriteSelect, Boolean *ExceptSelect);
void crt_select (void *PrivateData, Boolean *ReadSelect, Boolean *WriteSelect, Boolean *ExceptSelect)
{
  (void) PrivateData;
  DO_CRT_CHECK_WINCHANGED;
  if (*ReadSelect)
    {
      if (crt_update_level == UpdateRegularly)
        {
          crt_stop_refresh ();
          crt_refresh ();
        }
      (*ReadSelect) = !!(crt_unget_key_buf || crt_key_buf || crt_fkey_buf || crt_screen_size_changed);
    }
  if (crt_get_output_fd () >= 0) (*WriteSelect) = FALSE;
  (*ExceptSelect) = FALSE;
}

static Boolean crt_keypressed_internal ()
{
  int ch;
  crt_nodelay (TRUE);
  ch = wgetk ();
  if (ch == ERR) return 0;
  crt_key_buf = ch;
  return 1;
}

Boolean crt_keypressed (void);
Boolean crt_keypressed ()
{
  DO_CRT_CHECK_WINCHANGED;
  if (crt_update_level >= UpdateInput) crt_update_immediately ();
  if (crt_unget_key_buf || crt_key_buf || crt_fkey_buf) return 1;
  return crt_keypressed_internal ();
}

void crt_changetextmode (Boolean Color, Boolean Columns40, Boolean Lines50);
void crt_changetextmode (Boolean Color, Boolean Columns40, Boolean Lines50)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_update_immediately ();
  crt_ColorFlag = crt_HasColors && Color;
  crt_settextmode (Columns40, Lines50);
  crt_screen_size_changed = TRUE;
  crt_setattr ();
  crt_redraw ();
  GetLastMode ();
  crt_keypressed_internal ();
  if (crt_key_buf == KEY_RESIZE) crt_key_buf = 0;
}

void crt_ungetch (TKey ch);
void crt_ungetch (TKey ch)
{
  crt_unget_key_buf = ch;
}

static Char find_key (TKeyTable *table, int ch)
{
  while (table -> curses_key != 0 && table -> curses_key != ch) table++;
  return table -> crt_key;
}

/* Don't use toupper() since it messes up the function key codes */
static int upcase (int ch)
{
  if (ch >= 'a' && ch <= 'z')
    return ch - 'a' + 'A';
  else
    return ch;
}

TKey crt_readkeyword (void);
TKey crt_readkeyword ()
{
  int ch, chu, shiftstate;
  Char c, cs;
  (void) &ch; /* don't put it into a register because of longjmp() */
  DO_CRT_CHECK_WINCHANGED;
  if (crt_update_level > UpdateWaitInput ||
      ((crt_update_level == UpdateWaitInput) &&
       !(crt_unget_key_buf || crt_key_buf || crt_keypressed_internal ())))
    crt_update_immediately ();
  if (crt_unget_key_buf)
    {
      TKey k = crt_unget_key_buf;
      crt_unget_key_buf = 0;
      return k;
    }
  else if (crt_key_buf)
    {
      ch = crt_key_buf;
      crt_key_buf = 0;
      if (ch == KEY_RESIZE)
        {
          int i;
          crt_nodelay (TRUE);
          i = get_ch ();
          if (i != KEY_RESIZE && i == ERR) crt_key_buf = i;
        }
    }
  else
    {
      crt_nodelay (FALSE);
      do
        {
          ch = wgetk ();
          if (crt_unget_key_buf) /* may have been set via crt_ungetch() by a handler for SIGINT etc. */
            {
              TKey k = crt_unget_key_buf;
              crt_unget_key_buf = 0;
              if (ch != ERR) crt_key_buf = ch;
              return k;
            }
        }
      while (ch == ERR);
    }
  if (ch == KEY_HUP)
    return 0x100 * ksHUp;
  #ifndef USE_PDCURSES
  if (ch == chEsc)
    {
      crt_nodelay (TRUE);
      ch = wgetk ();
      if (ch == ERR) return chEsc;
      c = find_key (EscKeyTable, upcase (ch));
      if (c != 0)
        return 0x100 * c;
      if (ch >= FKEY1 && ch <= FKEY1 + 9)
        return 0x100 * (ksAltF1 + ch - FKEY1);
      if (ch >= FKEY1 + 10 && ch <= FKEY1 + 11)
        return 0x100 * (ksAltF11 + ch - (FKEY1 + 10));
      crt_key_buf = ch;
      return chEsc;
    }
  #endif
  shiftstate = crt_getshiftstate ();
  if ((c = find_key (KeyTable, ch))) ch = c;
  chu = upcase (ch);
  if ((shiftstate & shShift) && (cs = find_key (ShiftKeyTable, chu))) return 0x100 * cs;
  if ((shiftstate & shAlt  ) && (cs = find_key (AltKeyTable  , chu))) return 0x100 * cs;
  if ((shiftstate & shAltGr) && (cs = find_key (AltGrKeyTable, chu))) return 0x100 * cs;
  if ((shiftstate & shExtra) && (cs = find_key (ExtraKeyTable, chu))) return 0x100 * cs;
  if (1 <= ch && ch <= 0xff) return ch;
  if (ch >= FKEY1 && ch <= FKEY1 + 9)
    {
      if (shiftstate & shAnyAlt)
        c = ksAltF1 + ch - FKEY1;
      else if (shiftstate & shCtrl)
        c = ksCtrlF1 + ch - FKEY1;
      else if (shiftstate & shShift)
        c = ksShF1 + ch - FKEY1;
      else
        c = ksF1 + ch - FKEY1;
    }
  else if (ch >= FKEY1 + 10 && ch <= FKEY1 + 11)
    {
      if (shiftstate & shAnyAlt)
        c = ksAltF11 + ch - (FKEY1 + 10);
      else if (shiftstate & shCtrl)
        c = ksCtrlF11 + ch - (FKEY1 + 10);
      else if (shiftstate & shShift)
        c = ksShF11 + ch - (FKEY1 + 10);
      else
        c = ksF11 + ch - (FKEY1 + 10);
    }
  else if (ch >= FKEYSH1 && ch <= FKEYSH1 + 9)
    c = ksShF1 + ch - FKEYSH1;
  else if (ch >= FKEYCTRL1 && ch <= FKEYCTRL1 + 9)
    c = ksCtrlF1 + ch - FKEYCTRL1;
  else if (ch >= FKEYALT1 && ch <= FKEYALT1 + 9)
    c = ksAltF1 + ch - FKEYALT1;
  else
    c = find_key (FKeyTable, ch);
  if ((shiftstate & shShift) && (cs = find_key (ShiftFKeyTable, c)))
    c = cs;
  else if ((shiftstate & shCtrl) && (cs = find_key (CtrlFKeyTable, c)))
    c = cs;
  else if ((shiftstate & shAlt) && (cs = find_key (AltFKeyTable, c)))
    c = cs;
  return 0x100 * c;
}

Char crt_readkey (void);
Char crt_readkey ()
{
  Char tmp;
  int ch;
  if (crt_fkey_buf)
    {
      tmp = crt_fkey_buf;
      crt_fkey_buf = 0;
      return tmp;
    }
  DO_RETURN_ADDRESS (ch = crt_readkeyword ());
  if (ch & 0xff) return ch;
  crt_fkey_buf= ch / 0x100;
  return 0;
}

void crt_gotoxy (int x, int y);
void crt_gotoxy (int x, int y)
{
  DO_CRT_CHECK_WINCHANGED;
  wmove (crt_ActivePanel -> w, y - 1, x - 1);
  crt_cursor_pos.x = x - 1;
  crt_cursor_pos.y = y - 1;
  crt_need_refresh ();
}

int crt_wherex (void);
int crt_wherex ()
{
  int x, y;
  DO_CRT_CHECK_WINCHANGED;
  getyx (crt_ActivePanel -> w, y, x);
  return x + 1;
}

int crt_wherey (void);
int crt_wherey ()
{
  int x, y;
  DO_CRT_CHECK_WINCHANGED;
  getyx (crt_ActivePanel -> w, y, x);
  return y + 1;
}

size_t crt_winsize (void);
size_t crt_winsize ()
{
  DO_CRT_CHECK_WINCHANGED;
  return (WindXMax - WindXMin + 1) * (WindYMax - WindYMin + 1) * sizeof (chtype);
}

void crt_readwin (chtype *buf);
void crt_readwin (chtype *buf)
{
  DO_CRT_CHECK_WINCHANGED;
  {
    int xsize = WindXMax - WindXMin + 1, ysize = WindYMax - WindYMin + 1, yc, sx, sy;
    chtype temp [xsize + 1];
    getyx (crt_ActivePanel -> w, sy, sx);
    for (yc = 0; yc < ysize; yc++)
      {
        mvwinchnstr (crt_ActivePanel -> w, yc, 0, temp, xsize);
        memcpy (buf + xsize * yc, temp, xsize * sizeof (chtype)); /* don't copy the 0 terminator! */
      }
    wmove (crt_ActivePanel -> w, sy, sx);
  }
}

void crt_writewin (chtype *buf);
void crt_writewin (chtype *buf)
{
  DO_CRT_CHECK_WINCHANGED;
  {
    int xsize = WindXMax - WindXMin + 1, ysize = WindYMax - WindYMin + 1, yc, sx, sy;
    getyx (crt_ActivePanel -> w, sy, sx);
    for (yc = 0; yc < ysize; yc++)
      mvwaddchnstr (crt_ActivePanel -> w, yc, 0, buf + xsize * yc, xsize);
    wmove (crt_ActivePanel -> w, sy, sx);
    crt_need_refresh ();
  }
}

void crt_clrscr (void);
void crt_clrscr ()
{
  crt_setattr ();
  werase (crt_ActivePanel -> w);
  wmove (crt_ActivePanel -> w, 0, 0);
  crt_cursor_pos.x = crt_cursor_pos.y = 0;
  crt_need_refresh ();
}

void crt_clreol (void);
void crt_clreol ()
{
  crt_setattr ();
  wclrtoeol (crt_ActivePanel -> w);
  crt_need_refresh ();
}

void crt_insline (void);
void crt_insline ()
{
  crt_setattr ();
  winsertln (crt_ActivePanel -> w);
  (void) idlok (crt_ActivePanel -> w, TRUE);
  crt_need_refresh ();
}

void crt_delline (void);
void crt_delline ()
{
  crt_setattr ();
  wdeleteln (crt_ActivePanel -> w);
  (void) idlok (crt_ActivePanel -> w, TRUE);
  crt_need_refresh ();
}

void crt_setpccharset (Boolean PCCharSet);
void crt_setpccharset (Boolean PCCharSet)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_ActivePanel -> PCCharSet = PCCharSet;
}

Boolean crt_getpccharset (void);
Boolean crt_getpccharset ()
{
  DO_CRT_CHECK_WINCHANGED;
  return crt_ActivePanel -> PCCharSet;
}

void crt_setcontrolchars (Boolean UseControlChars);
void crt_setcontrolchars (Boolean UseControlChars)
{
  DO_CRT_CHECK_WINCHANGED;
  crt_ActivePanel -> UseControlChars = UseControlChars;
}

Boolean crt_getcontrolchars (void);
Boolean crt_getcontrolchars ()
{
  DO_CRT_CHECK_WINCHANGED;
  return crt_ActivePanel -> UseControlChars;
}

size_t crt_read (void *PrivateData, Char *buffer, size_t size);
size_t crt_read (void *PrivateData, Char *buffer, size_t size)
{
  int res;
  size_t n;
  Char *p;
  (void) PrivateData;
  crt_setattr ();
  if (!crt_line_buf_count)
    {
      crt_nodelay (FALSE);
      crt_rawout (FALSE);
      echo ();
      if (crt_update_level >= UpdateWaitInput) crt_update_immediately ();
      do
        {
          errno = 0;
          res = wgetnstr (crt_ActivePanel -> w, crt_line_buf, MAXLENGTH - 1);
        }
      while
        #ifdef EINTR
        (res == ERR && errno == EINTR);
        #else
        (0);
        #endif
      if (res == ERR)
        crt_line_buf_count = 0;
      else
        {
          if (crt_CheckEOF)
            {
              do
                {
                  p = strchr (crt_line_buf, 26);
                  if (p) *p = 0;
                }
              while (p);
            }
          crt_line_buf_count = strlen (crt_line_buf);
          crt_line_buf [crt_line_buf_count++] = '\n';
        }
      crt_line_buf_pos = crt_line_buf;
      noecho ();
      crt_rawout (TRUE);
    }
  n = (crt_line_buf_count < size) ? crt_line_buf_count : size;
  memcpy (buffer, crt_line_buf_pos, n);
  crt_line_buf_pos += n;
  crt_line_buf_count -= n;
  return n;
}

void crt_flash (void);
void crt_flash ()
{
  DO_CRT_CHECK_WINCHANGED;
  flash ();
}

void crt_beep (void);
void crt_beep ()
{
  DO_CRT_CHECK_WINCHANGED;
  if (crt_VisualBell)
    crt_flash ();
  else
    {
      beep ();
      crt_delay (100);
    }
}

static chtype chtransform (Char ch, TTextAttr attr, Boolean PCCharSet)
{
  Boolean pccs = PCCharSet || (ch < ' ');
  if (ch == 0
  #ifdef XCURSES
    || (crt_ColorFlag && (attr & 7) == ((attr >> 4) & 7))
  #endif
    || (attr & ~(crt_ColorFlag ? 0x80 : 0x88)) == 0)
    return ' ';
  #if !defined(USE_PDCURSES) || defined(XCURSES)
  if ((!crt_linux_console && pccs)
      || ch == chBell || ch == chBkSp || ch == chTab || ch == chLF || ch == chFF
      || ch == chCR || ch == chEsc || ch == 14 || ch == 15 || ch == 155)
    {
      if (ch < 32) return chars_0_31 [ch];
      if (ch >= 128)
        {
          chtype c = chars_128_255 [ch - 128];
          #ifdef XCURSES
          /* ACS_BLOCK is not supported in all charsets, but we can emulate it */
          if (c == ACS_BLOCK)
            c = ' ' | crt_attr2cursattr ((attr >> 4) | ((attr & 15) << 4));
          #endif
          return c;
        }
    }
  #endif
  if (!pccs && iscntrl (ch)) return ' ';
  return (crt_linux_console && pccs) ? ch | A_ALTCHARSET : ch;
}

void crt_readchar (int x, int y, Char *ch, TTextAttr *attr);
void crt_readchar (int x, int y, Char *ch, TTextAttr *attr)
{
  int sx, sy, cc;
  chtype c;
  crt_setattr ();
  getyx (crt_ActivePanel -> w, sy, sx);
  c = mvwinch (crt_ActivePanel -> w, y - 1, x - 1);
  *attr = cursattr2attr (c);
  c &= A_CHARTEXT;
  for (cc = 0x20; cc < 0x120 && (chtransform (cc % 0x100, 7, crt_ActivePanel -> PCCharSet) & A_CHARTEXT) != c; cc++);
  *ch = cc % 0x100;
  wmove (crt_ActivePanel -> w, sy, sx);
}

void crt_fillwin (Char ch, TTextAttr attr);
void crt_fillwin (Char ch, TTextAttr attr)
{
  DO_CRT_CHECK_WINCHANGED;
  wbkgdset (crt_ActivePanel -> w, crt_attr2cursattr (attr) | chtransform (ch, attr, crt_ActivePanel -> PCCharSet));
  werase (crt_ActivePanel -> w);
  crt_setattr ();
  crt_need_refresh ();
}

size_t crt_write (void *PrivateData, const Char *buffer, size_t size);
size_t crt_write (void *PrivateData, const Char *buffer, size_t size)
{
  size_t i;
  Char ch;
  (void) PrivateData;
  crt_setattr ();
  for (i = 0; i < size; i++)
    {
      ch = buffer [i];
      if (crt_ActivePanel -> UseControlChars)
        {
          if (ch == chBell)
            {
              crt_beep ();
              continue;
            }
          else if (ch == chBkSp)
            {
              int x, y;
              getyx (crt_ActivePanel -> w, y, x);
              if (x > 0)
                wmove (crt_ActivePanel -> w, y, x - 1);
              continue;
            }
          else if (ch == chLF)
            {
              int x, y;
              getyx (crt_ActivePanel -> w, y, x);
              if (y + WindYMin >= WindYMax)
                {
                  if (crt_ActivePanel -> ScrollState) wscrl (crt_ActivePanel -> w, 1);
                  wmove (crt_ActivePanel -> w, y, 0);
                }
              else
                wmove (crt_ActivePanel -> w, y + 1, 0);
              continue;
            }
          else if (ch == chCR)
            {
              int x, y;
              getyx (crt_ActivePanel -> w, y, x);
              wmove (crt_ActivePanel -> w, y, 0);
              continue;
            }
        }
      waddch (crt_ActivePanel -> w, chtransform (ch, crt_TextAttr, crt_ActivePanel -> PCCharSet));
    }
  update_crt_cursor_pos ();
  crt_need_refresh ();
  return size;
}

void crt_writecharattrat (int x, int y, int Count, TCharAttr *CharAttr);
void crt_writecharattrat (int x, int y, int Count, TCharAttr *CharAttr)
{
  crt_setattr ();
  {
    int m = WindXMax - WindXMin + 1 - x + 1;
    if (Count > m) Count = m;
    if (Count > 0)
      {
        chtype buf [Count];
        int cattr = 0, LastAttr = - 1, sx, sy, i;
        TTextAttr Attr;
        for (i = 0; i < Count; i++)
          {
            Attr = CharAttr [i].Attr;
            if (Attr != LastAttr)
              {
                cattr = crt_attr2cursattr (Attr);
                LastAttr = Attr;
              }
            buf [i] = chtransform (CharAttr [i].Ch, Attr, CharAttr [i].PCCharSet) | cattr;
          }
        getyx (crt_ActivePanel -> w, sy, sx);
        wmove (crt_ActivePanel -> w, y - 1, x - 1);
        crt_setscroll (FALSE);
        waddchnstr (crt_ActivePanel -> w, buf, Count);
        crt_setscroll (crt_ActivePanel -> ScrollState);
        wmove (crt_ActivePanel -> w, sy, sx);
        crt_need_refresh ();
      }
  }
}
