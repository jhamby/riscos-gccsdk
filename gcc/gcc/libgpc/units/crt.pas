{
CRT (Crt Replacement Tool)
Portable BP compatible CRT unit for GPC with many extensions

This unit is aware of terminal types. This means programs using this
unit will work whether run locally or while being logged in remotely
from a system with a completely different terminal type (as long as
the appropriate terminfo entry is present on the system where the
program is run).

NOTES:

- The CRT unit needs the ncurses and panel libraries which should be
  available for almost any system. For Dos systems, where ncurses is
  not available, it is configured to use the PDCurses and panel
  libraries instead. On Unix systems with X11, it can also use
  PDCurses (xcurses) and xpanel to produce X11 programs. The
  advantage is that they don't need an xterm with a valid terminfo
  entry and may look a little nicer than an xterm, but the
  disadvantage is that they will only run under X. The ncurses and
  PDCurses libraries (including panel and xpanel, resp.) can be
  found in ftp://agnes.dida.physik.uni-essen.de/gnu-pascal/libs/
  (Note that ncurses is already installed on many Unix systems.)

  When an X11 version under Unix is wanted, give `-DX11' when
  compiling crt.pas and crtc.c (or when compiling crt.pas or a
  program that uses CRT with `--automake'). On pre-X11R6 systems,
  give `-DNOX11R6' additionally. You might also have to give the
  path to the X11 libraries with `-L', e.g. `-L /usr/X11/lib'.

- A few features cannot be implemented in a portable way and are
  only available on some systems:

      Sound, NoSound 1)                  -------------------.
      GetShiftState                      --------------.    |
      TextMode 2)                        ---------.    |    |
      Interrupt signal (Ctrl-C) handling ----.    |    |    |
                                             |    |    |    |
  Linux/x86 3) (terminal)                    X    X 4) X 5) X 5)
  Other Unix (terminal)                      X    -    -    -
  Unix (X11 version)                         X    -    X    -
  Dos (DJGPP)                                X    X    X    X
  MS-Windows (Cygwin or mingw)               X    X 6) X    -

  Notes:

  1) If you define NO_CRT_DUMMY_SOUND while compiling CRT, you will
     get linking errors when your program tries to use Sound/NoSound
     on a platform where it's not supported (which is useful to
     detect at compile time if playing sound is a major task of your
     program). Otherwise, Sound/NoSound will simply do nothing
     (which is usually acceptable if the program uses these routines
     just for an occasional beep).

  2) Changing to monochrome modes works on all platforms. Changing
     the screen size only works on those indicated. However, even on
     the platforms not supported, the program will react to screen
     size changes by external means (e.g. changing the window size
     with the mouse if running in a GUI window or resizing a console
     or virtual terminal).

  3) Probably also on other processors, but I've had no chance to
     test this yet.

  4) Only on local consoles. Changing the screen size requires
     the command lines to be called for the different modes to be
     stored as CStrings in the variables crt_setfont_command_80_25,
     crt_setfont_command_80_50, crt_setfont_command_40_25 and
     crt_setfont_command_40_50. (These variables are not present on
     other platforms!) The default settings only take care of the 80
     column modes (25 and 50 rows), and require the program setfont
     to be present in the PATH, and require root permissions, either
     to the program compiled with CRT or to resizecons (called by
     setfont). To achieve the latter, once do a
     "chmod u+s `which resizecons`" as root, but only if you really
     want each user to be allowed to change the console size.
     However, using the program SVGATextMode(8), you can get better
     results (e.g., 40 column modes if set up accordingly, or in
     fact any mode between 16x8 and 160x100 or whatever your graphic
     card supports ;-).

  5) Only on local consoles.

  6) Only with PDCurses, not with ncurses. Changing the number of
     screen *columns* doesn't work in a full-screen session.

- When CRT is initialized (automatically or explicitly; see the
  comments for CRTInit), the screen is cleared, and at the end of
  the program, the cursor is placed at the bottom of the screen
  (curses behaviour).

- All the other things (including most details like color and
  function key constants) are compatible with BP's CRT unit, and
  there are many extensions that BP's unit does not have.

- When the screen size is changed by an external event (e.g.
  resizing an xterm or changing the screen size from another VC
  under Linux), the virtual "function key" kbScreenSizeChanged is
  returned. Applications can use the virtual key to resize their
  windows. kbScreenSizeChanged will not be returned if the screen
  size change was initiated by the program itself (by using
  TextMode). Note that TextMode will set the current panel to the
  full screen size, set the text attribute to the default and clear
  the window (BP compatibility).

- After the screen size has been changed, with using TextMode or by
  an external event, ScreenSize will return the new screen size.
  The current window and all panels will have been adjusted to the
  new screen size. This means, if their right or lower ends are
  outside the new screen size, the windows are moved to the left
  and/or top as far as necessary. If this is not enough, i.e., if
  they are wider/higher than the new screen size, they are shrinked
  to the total screen width/height. When the screen size is
  enlarged, window sizes are not changed, with one exception:
  Windows that extend through the whole screen width/height are
  enlarged to the whole new screen width/height (in particular,
  full-screen windows remain full-screen). This behaviour might not
  be optimal for all purposes, but you can always resize your
  windows in your application after the screen size change.

- When trying to write portable programs, don't rely on exactly the
  same look of your output and the availability of all the key
  combinations. Some kinds of terminals support only some of the
  display attributes and special characters, and usually not all of
  the keys declared are really available. Therefore, it's safer to
  provide the same function on different key combinations and to not
  use the more exotic ones.

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
}

{$gnu-pascal}
{$if __GPC_RELEASE__ < 20000412}
{$error This unit requires GPC release 20000412 or newer}
{$endif}

unit {$ifdef THIS_IS_WINCRT} WinCRT {$else} CRT {$endif};

interface

uses GPC;

const
  { CRT modes }
  BW40          = 0;            { 40x25 Black/White }
  CO40          = 1;            { 40x25 Color }
  BW80          = 2;            { 80x25 Black/White }
  CO80          = 3;            { 80x25 Color }
  Mono          = 7;            { 80x25 Black/White }
  Font8x8       = 256;          { Add-in for 80x43 or 80x50 mode }

  { Mode constants for Turbo Pascal 3.0 compatibility }
  C40           = CO40;
  C80           = CO80;

  { Foreground and background color constants }
  Black         = 0;
  Blue          = 1;
  Green         = 2;
  Cyan          = 3;
  Red           = 4;
  Magenta       = 5;
  Brown         = 6;
  LightGray     = 7;

  { Foreground color constants }
  DarkGray      = 8;
  LightBlue     = 9;
  LightGreen    = 10;
  LightCyan     = 11;
  LightRed      = 12;
  LightMagenta  = 13;
  Yellow        = 14;
  White         = 15;

  { Add-in for blinking }
  Blink         = 128;

type
  TTextAttr = Byte;

var
  { If False (default: True), catch interrupt signals (SIGINT;
    Ctrl-C), and other flow control characters as well as SIGTERM,
    SIGHUP and perhaps other signals }
  CheckBreak  : Boolean = True; asmname 'crt_CheckBreak';

  { If True (default : False), replace Ctrl-Z by #0 in input }
  CheckEOF    : Boolean = False; asmname 'crt_CheckEOF';

  { Ignored -- meaningless in this context }
  DirectVideo : Boolean = True;

  { Ignored -- curses or the terminal driver will take care of that
    when necessary }
  CheckSnow   : Boolean = False;

  { Current text mode }
  LastMode    : Word = 3; asmname 'crt_LastMode';

  { Current text attribute }
  TextAttr    : TTextAttr = 7; asmname 'crt_TextAttr';

  { Window upper left coordinates. *Obsolete*! Please see WindowMin
    below. }
  WindMin     : Word = not Word (0); asmname 'crt_WindMin';

  { Window lower right coordinates. *Obsolete*! Please see WindowMax
    below. }
  WindMax     : Word = not Word (0); asmname 'crt_WindMax';

procedure AssignCRT (var F : Text);
function  KeyPressed : Boolean;            asmname 'crt_keypressed';
function  ReadKey : Char;                  asmname 'crt_readkey';

{ Not effective on all platforms }
procedure TextMode (Mode : Integer);

procedure Window (x1, y1, x2, y2 : Integer);
procedure GotoXY (x, y : Integer);         asmname 'crt_gotoxy';
function  WhereX : Integer;                asmname 'crt_wherex';
function  WhereY : Integer;                asmname 'crt_wherey';
procedure ClrScr;                          asmname 'crt_clrscr';
procedure ClrEOL;                          asmname 'crt_clreol';
procedure InsLine;                         asmname 'crt_insline';
procedure DelLine;                         asmname 'crt_delline';
procedure TextColor (Color : TTextAttr);
procedure TextBackground (Color : TTextAttr);
procedure LowVideo;
procedure HighVideo;
procedure NormVideo;
procedure Delay (MS : Word);               asmname 'crt_delay';

{ Not available on all platforms }
procedure Sound (Hz : Word);               asmname 'crt_sound';
procedure NoSound;                         asmname 'crt_nosound';

{ ======================== Extensions over BP's CRT ======================== }

{
  Initializes the CRT unit. Should be called before using any of
  CRT's routines.

  Note: For BP compatibility, CRT is initizalized automatically when
  (almost) any of its routines are used for the first time. In this
  case, some defaults are set to match BP more closely. In
  particular, the PC charset (see SetPCCharSet) is enabled then
  (disabled otherwise), and the update level (see SetCRTUpdate) is
  set to UpdateRegularly (UpdateWaitInput otherwise). This feature
  is meant for BP compatibility *only*. Don't rely on it when
  writing a new program. Use CRTInit then, and set the defaults to
  the values you want explicitly.

  SetCRTUpdate is one of those few routines which will not cause CRT
  to be initialized immediately, and a value set with it will
  survive both automatic and explicit initialization, so you can use
  it to set the update level without caring which way CRT will be
  initialized. (This does not apply to SetPCCharSet. Since it works
  on a per-panel basis, it has to initialize CRT first, so there is
  a panel to start with.)

  If you terminate the program before calling CRTInit or any routine
  that causes automatic initialization, curses will never be
  initialized, so e.g., the screen won't be cleared. This can be
  useful, e.g., to check the command line arguments (or anything
  else) and if there's a problem, write an error and abort. Just be
  sure to write the error to StdErr, not Output (because Output will
  be assigned to CRT, and therefore writing to Output will cause CRT
  to be initialized, and because errors belong to StdErr, anyway),
  and to call `RestoreTerminal (True)' before (just to be sure, in
  case some code -- perhaps added later, or hidden in the
  initialization of some unit -- does initialize CRT).
}
procedure CRTInit; asmname 'crt_init';

{ If CRT is initialized automatically, not via CRTInit, and
  CRTAutoInitProc is not nil, it will be called before actually
  initializing CRT. }
var
  CRTAutoInitProc : procedure = nil; asmname 'crt_auto_init_proc';

{ Aborts with a runtime error saying that CRT was not initialized.
  If you set CRTAutoInitProc to this procedure, you can effectively
  disable CRT's automatical initialization. }
procedure CRTNotInitialized; asmname 'crt_not_initialized';

{ Set terminal to shell or curses mode. An internal procedure
  registered by CRT via RegisterRestoreTerminal does this as well,
  so CRTSetCursesMode has to be called only in unusual situations,
  e.g. after executing a process that changes terminal modes, but
  does not restore them (e.g. because it crashed or was killed), and
  the process was not executed with the Execute routine, and
  RestoreTerminal was not called otherwise. If you set it to False
  temporarily, be sure to set it back to True before doing any
  further CRT operations, otherwise the result may be strange. }
procedure CRTSetCursesMode (On : Boolean); asmname 'crt_set_curses_mode';

{ Do the same as `RestoreTerminal (True)', but also clear the screen
  after restoring the terminal (except for XCurses, because it uses
  its own window). }
procedure RestoreTerminalClearCRT;

{ Keyboard and character graphics constants -- BP compatible! =:-}
{$i crt.inc}

var
  { Tells whether the XCurses version of CRT is used }
  XCRT : Boolean = {$ifdef XCURSES} True {$else} False {$endif}; asmname 'crt_XCRT';

  { If True (not default), the Beep procedure and writing #7 do a
    Flash instead }
  VisualBell : Boolean = False; asmname 'crt_VisualBell';

type
  TKey = Word;

  TCursorShape = (CursorIgnored, CursorHidden, CursorNormal, CursorFat, CursorBlock);

  TCRTUpdate = (UpdateNever, UpdateWaitInput, UpdateInput,
                UpdateRegularly, UpdateAlways);

  TPoint = record
    X, Y : Integer
  end;

  PCharAttr = ^TCharAttr;
  TCharAttr = record
    Ch        : Char;
    Attr      : TTextAttr;
    PCCharSet : Boolean
  end;

  PCharAttrs = ^TCharAttrs;
  TCharAttrs = array [1 .. MaxVarSize div SizeOf (TCharAttr)] of TCharAttr;

  TWindowXY = packed record
    {$ifdef __BYTES_BIG_ENDIAN__}
    Fill : Integer (BitSizeOf (Word) - 16);
    Y, X : Word (8)
    {$else}
    X, Y : Word (8);
    Fill : Integer (BitSizeOf (Word) - 16)
    {$endif}
  end;

{ Make sure TWindowXY really has the same size as WindMin and
  WindMax. If not, compilation will abort here with `division by
  zero'. Otherwise, the value of the constant will always be 1, and
  is of no further interest. }
const
  AssertTWindowXYSize = 1 / Ord ((SizeOf (TWindowXY) = SizeOf (WindMin)) and
                                 (SizeOf (TWindowXY) = SizeOf (WindMax)));

var
  { Window upper and left coordinates. More comfortable to access
    than WindMin, but also *obsolete*. WindMin and WindowMin still
    work, but have the problem that they implicitly limit the window
    size to 255x255 characters. Though that's not really small for a
    text window, it's easily possible to create bigger ones (e.g. in
    an xterm with a small font, on a high resolution screen and/or
    extending over several virutal desktops). When using coordinates
    greater than 254, they will be set to 254 in WindowMin/WindowMax
    (@@not yet implemented), so e.g. programs which do `Inc
    (WindowMin.X)' will not fail quite as badly (but probably still
    fail). The routines Window and GetWindow use Integer
    coordinates, and don't suffer from any of these problems, so
    they should be used instead. }
  WindowMin : TWindowXY absolute WindMin;

  { Window lower right coordinates. More comfortable to access than
    WindMax, but also *obsolete* (see the comments for WindowMin).
    Use Window and GetWindow instead. }
  WindowMax : TWindowXY absolute WindMax;

  { The attribute set by NormVideo }
  NormAttr : TTextAttr = 7; asmname 'crt_NormAttr';

  { Tells whether the current mode is monochrome }
  IsMonoMode : Boolean = False; asmname 'crt_IsMonoMode';

  { This value can be set to a combination of the shFoo constants
    and will be ORed to the actual shift state returned by
    GetShiftState. This can be used to easily simulate shift keys on
    systems where they can't be accessed. }
  VirtualShiftState : Integer = 0; asmname 'crt_VirtualShiftState';

{ Returns the size of the screen. Note: In BP's WinCRT unit,
  ScreenSize is a variable. But since writing to it from a program
  is pointless, anyway, providing a function here should not cause
  any incompatibility. }
function  ScreenSize : TPoint; asmname 'crt_GetScreenSize';

{ Tell which modifier keys are currently pressed. The result is a
  combination of the shFoo constants defined in crt.inc, or 0 on
  systems where this function is not supported. If supported,
  ReadKey automatically converts kbIns and kbDel keys to kbShIns and
  kbShDel, resp., if shift is pressed. }
function  GetShiftState : Integer; asmname 'crt_getshiftstate';

{ Get the extent of the current window. Use this procedure rather
  than reading WindMin and WindMax or WindowMin and WindowMax, since
  this routine allows for window sizes larger than 255. The
  resulting coordinates are 1-based (like in Window, unlike WindMin,
  WindMax, WindowMin and WindowMax) Any of the parameters may be
  null in case you're interested in only some of the coordinates. }
procedure GetWindow (var x1, y1, x2, y2 : Integer);

{
  Determine when the screen is updated. The possible values are the
  following. The given conditions *guarantee* updates. However,
  updates may occur more frequently (even if the update level is set
  to UpdateNever). About the default value, see the comments for
  CRTInit.

  UpdateNever     : never (unless explicitly requested with
                    CRTUpdate)
  UpdateWaitInput : before Delay and CRT input, unless typeahead is
                    detected
  UpdateInput     : before Delay and CRT input
  UpdateRegularly : before Delay and CRT input and otherwise in
                    regular intervals without causing too much
                    refresh. This uses a timer on some systems
                    (currently, Unix with ncurses). This was created
                    for BP compatibility, but for many applications,
                    a lower value causes less flickering in the
                    output, and additionally, timer signals won't
                    disturb other operations. Under DJGPP, this
                    always updates immediately, but this fact should
                    not mislead DJGPP users into thinking this is
                    always so.
  UpdateAlways    : after each output. This can be very slow. (Not
                    so under DJGPP, but this fact should not mislead
                    DJGPP users...)
}
procedure SetCRTUpdate (UpdateLevel : TCRTUpdate); asmname 'crt_setupdatelevel';

{ Do an update now, independently of the update level }
procedure CRTUpdate; asmname 'crt_update_immediately';

{ Do an update now and completely redraw the screen }
procedure CRTRedraw; asmname 'crt_redraw';

{ Return Ord (key) for normal keys and $100 * Ord (fkey) for function keys }
function  ReadKeyWord : TKey; asmname 'crt_readkeyword';

{ Extract the character and scan code from a TKey value }
function  Key2Char (k : TKey) : Char;
function  Key2Scan (k : TKey) : Char;

{ Convert a key to upper/lower case if it is a letter, leave it
  unchanged otherwise }
function  UpCaseKey (k : TKey) : TKey;
function  LoCaseKey (k : TKey) : TKey;

{ Check if k is a pseudo key generated by a deadly signal trapped }
function  IsDeadlySignal (k : TKey) : Boolean;

{ Produce a beep or a screen flash }
procedure Beep;  asmname 'crt_beep';
procedure Flash; asmname 'crt_flash';

{ Get size of current window (calculated using GetWindow) }
function  GetXMax : Integer;
function  GetYMax : Integer;

{ Get/goto an absolute position }
function  WhereXAbs : Integer;
function  WhereYAbs : Integer;
procedure GotoXYAbs (X, Y : Integer);

{ Turn scrolling on or off }
procedure SetScroll (State : Boolean); asmname 'crt_setscroll';

{ Read back whether scrolling is enabled }
function  GetScroll : Boolean; asmname 'crt_getscroll';

{ Determine whether to interpret non-ASCII characters as PC ROM
  characters (True), or in a system dependent way (False). About the
  default, see the comments for CRTInit. }
procedure SetPCCharSet (PCCharSet : Boolean); asmname 'crt_setpccharset';

{ Read back the value set by SetPCCharSet }
function  GetPCCharSet : Boolean; asmname 'crt_getpccharset';

{ Determine whether to interpret #7, #8, #10, #13 as control
  characters (True, default), or as graphics characters (False) }
procedure SetControlChars (UseControlChars : Boolean); asmname 'crt_setcontrolchars';

{ Read back the value set by SetControlChars }
function  GetControlChars : Boolean; asmname 'crt_getcontrolchars';

procedure SetCursorShape (Shape : TCursorShape); asmname 'crt_setcursorshape';
function  GetCursorShape : TCursorShape; asmname 'crt_getcursorshape';

procedure HideCursor;
procedure HiddenCursor;
procedure NormalCursor;
procedure FatCursor;
procedure BlockCursor;
procedure IgnoreCursor;

{ Simulates a block cursor by writing a block character onto the
  cursor position. The procedure automatically finds the topmost
  visible panel whose shape is not CursorIgnored and places the
  simulated cursor there (just like the hardware cursor), with
  matching attributes, if the cursor shape is CursorFat or
  CursorBlock (otherwise, no simulated cursor is shown).

  Calling this procedure again makes the simulated cursor disappear.
  In particular, to get the effect of a blinking cursor, you have to
  call the procedure repeatedly (say, 8 times a second). CRT will
  not do this for you, since it does not intend to be your main
  event loop. }
procedure SimulateBlockCursor; asmname 'crt_SimulateBlockCursor';

{ Makes the cursor simulated by SimulateBlockCursor disappear if it
  is active. Does nothing otherwise. You should call this procedure
  after using SimulateBlockCursor before doing any further CRT
  output (though failing to do so should not hurt except for
  possibly leaving the simulated cursor in its old position longer
  than it should). }
procedure SimulateBlockCursorOff; asmname 'crt_SimulateBlockCursorOff';

function  GetTextColor : Integer;
function  GetTextBackground : Integer;

{ Write string at the given position without moving the cursor.
  Truncated at the right margin. }
procedure WriteStrAt (x, y : Integer; s : String; Attr : TTextAttr);

{ Write (several copies of) a char at then given position without
  moving the cursor. Truncated at the right margin. }
procedure WriteCharAt (x, y, Count : Integer; Ch : Char; Attr : TTextAttr);

{ Write characters with specified attributes at the given position
  without moving the cursor. Truncated at the right margin. }
procedure WriteCharAttrAt (x, y, Count : Integer; CharAttr : PCharAttrs); asmname 'crt_writecharattrat';

{ Write a char while moving the cursor }
procedure WriteChar (Ch : Char);

{ Read a character from a screen position }
procedure ReadChar (x, y : Integer; var Ch : Char; var Attr : TTextAttr); asmname 'crt_readchar';

{ Change only text attributes, leave characters. Truncated at the
  right margin. }
procedure ChangeTextAttr (x, y, Count : Integer; NewAttr : TTextAttr);

{ Fill current window }
procedure FillWin (Ch : Char; Attr : TTextAttr); asmname 'crt_fillwin';

{ Calculate size of memory required for ReadWin in current window. }
function  WinSize : SizeType; asmname 'crt_winsize';

{ Save window contents. Buf must be WinSize bytes large. }
procedure ReadWin (var Buf); asmname 'crt_readwin';

{ Restore window contents saved by ReadWin. The size of the current
  window must match the size of the window from which ReadWin was
  used, but the position may be different. }
procedure WriteWin (const Buf); asmname 'crt_writewin';

type
  WinState = record
    x1, y1, x2, y2, WhereX, WhereY, NewX1, NewY1, NewX2, NewY2 : Integer;
    TextAttr : TTextAttr;
    CursorShape : TCursorShape;
    TextMode : Integer;
    Buffer : ^Byte
  end;

{ Save window position and size, cursor position, text attribute and
  cursor shape -- NOT the window contents. }
procedure SaveWin (var State : WinState);

{ Make a new window (like Window), and save the contents of the
  screen below the window as well as the position and size, cursor
  position, text attribute and cursor shape of the old window. }
procedure MakeWin (var State : WinState; x1, y1, x2, y2 : Integer);

{ Create window in full size, save previous text mode and all values
  that MakeWin does. }
procedure SaveScreen (var State : WinState);

{ Restore the data saved by SaveWin, MakeWin or SaveScreen. }
procedure RestoreWin (var State : WinState);

{ Panels }

type
  TPanel = Pointer;

function  GetActivePanel : TPanel;                                                         asmname 'crt_GetActivePanel';
procedure PanelNew                 (x1, y1, x2, y2 : Integer; BindToBackground : Boolean); asmname 'crt_PanelNew';
procedure PanelDelete              (Panel : TPanel);                                       asmname 'crt_PanelDelete';
procedure PanelBindToBackground    (Panel : TPanel; Bind : Boolean);                       asmname 'crt_PanelBindToBackground';
function  PanelIsBoundToBackground (Panel : TPanel) : Boolean;                             asmname 'crt_PanelIsBoundToBackground';
procedure PanelActivate            (Panel : TPanel);                                       asmname 'crt_PanelActivate';
procedure PanelHide                (Panel : TPanel);                                       asmname 'crt_PanelHide';
procedure PanelShow                (Panel : TPanel);                                       asmname 'crt_PanelShow';
function  PanelHidden              (Panel : TPanel) : Boolean;                             asmname 'crt_PanelHidden';
procedure PanelTop                 (Panel : TPanel);                                       asmname 'crt_PanelTop';
procedure PanelBottom              (Panel : TPanel);                                       asmname 'crt_PanelBottom';
procedure PanelMoveAbove           (Panel, Above : TPanel);                                asmname 'crt_PanelMoveAbove';
procedure PanelMoveBelow           (Panel, Below : TPanel);                                asmname 'crt_PanelMoveBelow';
function  PanelAbove               (Panel : TPanel) : TPanel;                              asmname 'crt_PanelAbove';
function  PanelBelow               (Panel : TPanel) : TPanel;                              asmname 'crt_PanelBelow';

{ TPCRT compatibility }

{ Write a string at the given position without moving the cursor.
  Truncated at the right margin. }
procedure WriteString (const s : String; y, x : Integer);

{ Write a string at the given position with the given attribute
  without moving the cursor. Truncated at the right margin. }
procedure FastWriteWindow (const s : String; y, x : Integer; Attr : TTextAttr);

{ Write a string at the given absolute position with the given
  attribute without moving the cursor. Truncated at the right
  margin. }
procedure FastWrite       (const s : String; y, x : Integer; Attr : TTextAttr);

{ WinCrt compatibility }

const
  cw_UseDefault = Integer ($8000);

var
  WindowOrg  : TPoint = (cw_UseDefault, cw_UseDefault); { Ignored }
  WindowSize : TPoint = (cw_UseDefault, cw_UseDefault); { Ignored }
  Cursor     : TPoint = (0, 0); { Cursor location, 0-based } asmname 'crt_cursor_pos';
  Origin     : TPoint = (0, 0); { Ignored }
  InactiveTitle : PChar = '(Inactive %s)'; { Ignored }
  AutoTracking : Boolean = True; { Ignored }
  WindowTitle : {$ifdef __BP_TYPE_SIZES__}
                array [0 .. 79] of Char
                {$else}
                TStringBuf
                {$endif}; { CRT window title, ignored }

procedure InitWinCrt; asmname 'crt_initwincrt';

{ Halts the program }
procedure DoneWinCrt; attribute (noreturn); asmname 'crt_donewincrt';

procedure WriteBuf (Buffer : PChar; Count : SizeType); asmname 'crt_writebuf';

function  ReadBuf (Buffer : PChar; Count : SizeType) : SizeType; asmname 'crt_readbuf';

{ 0-based coordinates! }
procedure CursorTo (x, y : Integer); asmname 'crt_cursorto';

{ Dummy }
procedure ScrollTo (x, y : Integer); asmname 'crt_scrollto';

{ Dummy }
procedure TrackCursor; asmname 'crt_trackcursor';

implementation

{$B-,I-}

{$ifdef X11}
  {$L crtx.c, XPanel, XCurses, Xaw, Xmu, Xt, X11}
  {$ifndef NOX11R6} {$L SM, ICE, Xext} {$endif}
  { XCurses under Solaris needs additional libraries. When linking
    dynamically, they are automatically referenced by the other libraries.
    For static linking, however, we have to name them explicitly. }
  {$ifdef __sun__} {$L socket, w, nsl, intl, dl} {$endif}
{$elif defined(USE_PDCURSES)} {$L crtc.c, panel, curso}
{$else} {$L crtc.c, panel, ncurses} {$endif}

procedure CRT_Done; C;
procedure CRT_ChangeTextMode (Color, Columns40, Lines50 : Boolean); C;
procedure CRT_UnGetCh (k : TKey); C;
function  CRT_Read  (var PrivateData; var   Buffer; Size : SizeType) : SizeType; C;
function  CRT_Write (var PrivateData; const Buffer; Size : SizeType) : SizeType; C;
function  CRT_Get_Input_FD : Integer; C;
function  CRT_Get_Output_FD : Integer; C;
procedure CRT_Select (var PrivateData; var ReadSelect, WriteSelect, ExceptSelect : Boolean); C;
procedure CRT_Restore_Terminal_No_CRT; C;
procedure CRT_Restore_Terminal_CRT; C;
procedure CRT_ClearTerminal; C;
procedure CRT_Check_WinChanged; C;

{$define DO_CRT_CHECK_WINCHANGED
  begin
    SetReturnAddress (ReturnAddress (0));
    CRT_Check_WinChanged;
    RestoreReturnAddress
  end
}

const
  MonoModes = [BW40, BW80, Mono];

var
  TerminalNoCRT : Boolean = False; asmname 'crt_terminal_no_crt';
  Signaled      : volatile Boolean = False; asmname 'crt_Signaled';
  CRTScreenSize : TPoint = (- 1, - 1); asmname 'crt_ScreenSize';

procedure TextColor (Color: TTextAttr);
begin
  if Color and $f0<>0 then Color := (Color and $f) or Blink;
  TextAttr := (TextAttr and $70) or Color
end;

procedure TextBackground (Color : TTextAttr);
begin
  TextAttr := (TextAttr and $8f) or ((Color and 7) shl 4)
end;

function GetTextColor : Integer;
begin
  GetTextColor := TextAttr and $8f
end;

function GetTextBackground : Integer;
begin
  GetTextBackground := (TextAttr and $70) shr 4
end;

procedure LowVideo;
begin
  TextAttr := TextAttr and not 8
end;

procedure HighVideo;
begin
  TextAttr := TextAttr or 8
end;

procedure NormVideo;
begin
  TextAttr := NormAttr
end;

function ModeIsMono (Mode : Integer) : Boolean; asmname 'crt_modeismono';
function ModeIsMono (Mode : Integer) : Boolean;
begin
  ModeIsMono := (Mode and $ff) in MonoModes
end;

procedure TextMode (Mode : Integer);
begin
  DO_CRT_CHECK_WINCHANGED;
  CRT_ChangeTextMode (not ModeIsMono (Mode), (Mode and not Font8x8) in [BW40, CO40], Mode and Font8x8 <> 0);
  NormVideo;
  Window (1, 1, CRTScreenSize.X, CRTScreenSize.Y);
  ClrScr
end;

procedure Window (x1, y1, x2, y2 : Integer);
begin
  DO_CRT_CHECK_WINCHANGED;
  if (x1 >= 1) and (x1 <= x2) and (x2 <= CRTScreenSize.X) and
     (y1 >= 1) and (y1 <= y2) and (y2 <= CRTScreenSize.Y) then
    begin
      WindowMin.X := x1 - 1;
      WindowMin.Y := y1 - 1;
      WindowMax.X := x2 - 1;
      WindowMax.Y := y2 - 1;
      GotoXY (1, 1)
    end
end;

function ScreenSize : TPoint;
begin
  DO_CRT_CHECK_WINCHANGED;
  ScreenSize := CRTScreenSize
end;

procedure GetWindow (var x1, y1, x2, y2 : Integer);
begin
  DO_CRT_CHECK_WINCHANGED;
  if @x1 <> nil then x1 := WindowMin.X + 1;
  if @y1 <> nil then y1 := WindowMin.Y + 1;
  if @x2 <> nil then x2 := WindowMax.X + 1;
  if @y2 <> nil then y2 := WindowMax.Y + 1
end;

function CRT_SelectFunc (var PrivateData; Writing : Boolean) : Integer;
var Dummy : Pointer;
begin
  DO_CRT_CHECK_WINCHANGED;
  Dummy := @PrivateData;
  if Writing
    then CRT_SelectFunc := CRT_Get_Output_FD
    else CRT_SelectFunc := CRT_Get_Input_FD
end;

procedure AssignCRT (var F : Text);
begin
  AssignTFDD (F, nil, CRT_SelectFunc, CRT_Select, CRT_Read, CRT_Write, nil, nil, nil, nil)
end;

procedure WriteStrAt (x, y : Integer; s : String; Attr : TTextAttr);
var OrigAttr : TTextAttr;
begin
  DO_CRT_CHECK_WINCHANGED;
  OrigAttr := TextAttr;
  TextAttr := Attr;
  WriteString (s, y, x);
  TextAttr := OrigAttr
end;

procedure WriteCharAt (x, y, Count : Integer; Ch : Char; Attr : TTextAttr);
var
  OrigAttr : TTextAttr;
  Temp : array [1 .. Count] of Char;
  i : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  for i := 1 to Count do Temp [i] := Ch;
  OrigAttr := TextAttr;
  TextAttr := Attr;
  WriteString (Temp, y, x);
  TextAttr := OrigAttr
end;

procedure WriteChar (Ch : Char);
var Dummy : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  Dummy := CRT_Write (Dummy, Ch, 1)
end;

procedure WriteString (const s : String; y, x : Integer);
var
  OrigX, OrigY, Dummy, Size : Integer;
  UseControlCharsSave, ScrollSave : Boolean;
begin
  DO_CRT_CHECK_WINCHANGED;
  OrigX := WhereX;
  OrigY := WhereY;
  GotoXY (x, y);
  UseControlCharsSave := GetControlChars;
  SetControlChars (False);
  ScrollSave := GetScroll;
  SetScroll (False);
  Size := Min (Length (s), GetXMax - x + 1);
  if Size > 0 then Dummy := CRT_Write (Dummy, s [1], Size);
  SetScroll (ScrollSave);
  SetControlChars (UseControlCharsSave);
  GotoXY (OrigX, OrigY)
end;

procedure FastWriteWindow (const s : String; y, x : Integer; Attr : TTextAttr);
begin
  DO_CRT_CHECK_WINCHANGED;
  WriteStrAt (x, y, s, Attr)
end;

procedure FastWrite (const s : String; y, x : Integer; Attr : TTextAttr);
begin
  DO_CRT_CHECK_WINCHANGED;
  WriteStrAt (x - WindowMin.X, y - WindowMin.Y, s, Attr)
end;

procedure ChangeTextAttr (x, y, Count : Integer; NewAttr : TTextAttr);
var
  OrigX, OrigY, i : Integer;
  Ch : Char;
  OrigAttr, Attr : TTextAttr;
  ScrollSave : Boolean;
begin
  DO_CRT_CHECK_WINCHANGED;
  OrigAttr := TextAttr;
  OrigX := WhereX;
  OrigY := WhereY;
  GotoXY (x, y);
  ScrollSave := GetScroll;
  SetScroll (False);
  for i := 1 to Min (Count, GetXMax - x + 1) do
    begin
      ReadChar (x + i - 1, y, Ch, Attr);
      TextAttr := NewAttr;
      WriteChar (Ch)
    end;
  SetScroll (ScrollSave);
  GotoXY (OrigX, OrigY);
  TextAttr := OrigAttr
end;

function Key2Char (k : TKey) : Char;
begin
  if k div $100 <> 0
    then Key2Char := #0
    else Key2Char := Chr (k)
end;

function Key2Scan (k : TKey) : Char;
begin
  Key2Scan := Chr (k div $100)
end;

function UpCaseKey (k : TKey) : TKey;
var ch : Char;
begin
  ch := Key2Char (k);
  if ch = #0
    then UpCaseKey := k
    else UpCaseKey := Ord (UpCase (ch))
end;

function LoCaseKey (k : TKey) : TKey;
var ch : Char;
begin
  ch := Key2Char (k);
  if ch = #0
    then LoCaseKey := k
    else LoCaseKey := Ord (LoCase (ch))
end;

function IsDeadlySignal (k : TKey) : Boolean;
begin
  IsDeadlySignal := (k = kbInt) or (k = kbTerm) or (k = kbHUp)
end;

function GetXMax : Integer;
var x1, x2 : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  GetWindow (x1, null, x2, null);
  GetXMax := x2 - x1 + 1
end;

function GetYMax : Integer;
var y1, y2 : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  GetWindow (null, y1, null, y2);
  GetYMax := y2 - y1 + 1
end;

function WhereXAbs : Integer;
var x1 : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  GetWindow (x1, null, null, null);
  WhereXAbs := WhereX + x1 - 1
end;

function WhereYAbs : Integer;
var y1 : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  GetWindow (null, y1, null, null);
  WhereYAbs := WhereY + y1 - 1
end;

procedure GotoXYAbs (X, Y : Integer);
var x1, y1 : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  GetWindow (x1, y1, null, null);
  GotoXY (X - x1 + 1, Y - y1 + 1)
end;

procedure HideCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorHidden)
end;

procedure HiddenCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorHidden)
end;

procedure NormalCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorNormal)
end;

procedure FatCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorFat)
end;

procedure BlockCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorBlock)
end;

procedure IgnoreCursor;
begin
  DO_CRT_CHECK_WINCHANGED;
  SetCursorShape (CursorIgnored)
end;

procedure SaveWin (var State : WinState);
begin
  DO_CRT_CHECK_WINCHANGED;
  with State do
    begin
      GetWindow (x1, y1, x2, y2);
      NewX1 := x1;
      NewY1 := y1;
      NewX2 := x2;
      NewY2 := y2
    end;
  State.WhereX      := WhereX;
  State.WhereY      := WhereY;
  State.TextAttr    := TextAttr;
  State.CursorShape := GetCursorShape;
  State.TextMode    := - 1;
  State.Buffer      := nil
end;

procedure MakeWin (var State : WinState; x1, y1, x2, y2 : Integer);
begin
  DO_CRT_CHECK_WINCHANGED;
  SaveWin (State);
  Window (x1, y1, x2, y2);
  with State do GetWindow (NewX1, NewY1, NewX2, NewY2);
  GetMem (State.Buffer, WinSize);
  ReadWin (State.Buffer^)
end;

procedure SaveScreen (var State : WinState);
begin
  DO_CRT_CHECK_WINCHANGED;
  MakeWin (State, 1, 1, CRTScreenSize.X, CRTScreenSize.Y);
  State.TextMode := LastMode
end;

procedure RestoreWin (var State : WinState);
begin
  DO_CRT_CHECK_WINCHANGED;
  if State.TextMode <> - 1 then
    begin
      if State.TextMode <> LastMode then TextMode (State.TextMode);
      Window (1, 1, CRTScreenSize.X, CRTScreenSize.Y)
    end;
  SetCursorShape (State.CursorShape);
  if State.Buffer <> nil then
    begin
      with State do Window (NewX1, NewY1, NewX2, NewY2);
      WriteWin (State.Buffer^);
      FreeMem (State.Buffer);
      State.Buffer := nil
    end;
  with State do Window (x1, y1, x2, y2);
  GotoXY (State.WhereX, State.WhereY);
  TextAttr := State.TextAttr
end;

procedure InitWinCrt;
begin
  DO_CRT_CHECK_WINCHANGED
end;

procedure DoneWinCrt;
begin
  Halt
end;

procedure WriteBuf (Buffer : PChar; Count : SizeType);
var Dummy : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  if Count > 0 then Dummy := CRT_Write (Dummy, Buffer^, Count)
end;

function ReadBuf (Buffer : PChar; Count : SizeType) : SizeType;
var Dummy : Integer;
begin
  DO_CRT_CHECK_WINCHANGED;
  ReadBuf := CRT_Read (Dummy, Buffer^, Count)
end;

procedure CursorTo (x, y : Integer);
begin
  DO_CRT_CHECK_WINCHANGED;
  GotoXY (x + 1, y + 1)
end;

procedure ScrollTo (x, y : Integer);
begin
end;

procedure TrackCursor;
begin
end;

procedure RestoreTerminalClearCRT;
begin
  DO_CRT_CHECK_WINCHANGED; { We DO need this here, otherwise the tputs in
                             clrscr() called by crt_clearterminal() can
                             cause a segfault. }
  RestoreTerminal (True);
  CRT_ClearTerminal
end;

procedure CRT_RegisterRestoreTerminal; asmname 'crt_registerrestoreterminal';
procedure CRT_RegisterRestoreTerminal;
begin
  RegisterRestoreTerminal (True,  CRT_Restore_Terminal_No_CRT);
  RegisterRestoreTerminal (False, CRT_Restore_Terminal_CRT)
end;

function CRT_GetProgramName : CString; asmname 'crt_get_program_name';
function CRT_GetProgramName : CString;
begin
  CRT_GetProgramName := CParameters^[0]
end;

function CRT_GetCEnvironment : PCStrings; asmname 'crt_get_c_environment';
function CRT_GetCEnvironment : PCStrings;
begin
  CRT_GetCEnvironment := GetCEnvironment
end;

procedure CRTNotInitialized;
begin
  SetReturnAddress (ReturnAddress (0));
  RuntimeError (880); { CRT was not initialized }
  RestoreReturnAddress
end;

procedure Signal_Handler (Signal : TKey); asmname 'crt_signal_handler';
procedure Signal_Handler (Signal : TKey);
begin
  if TerminalNoCRT and (Signal = kbInt) then Exit;
  Signaled := True;
  if CheckBreak then
    begin
      if Signal = kbInt then Write ('^C');
      Halt (255)
    end
  else
    CRT_UnGetCh (Signal)
end;

procedure CRT_Fatal (Reason : Integer); attribute (noreturn); asmname 'crt_fatal';
procedure CRT_Fatal;
begin
  case Reason of
    1 :  InternalError (950); { CRT: cannot initialize curses }
    2 :  RuntimeError  (881); { attempt to delete invalid CRT panel }
    3 :  RuntimeError  (882); { attempt to delete last CRT panel }
    4 :  RuntimeError  (883); { attempt to activate invalid CRT panel }
    else InternalError (951)  { cannot create CRT window }
  end
end;

to begin do
  begin
    AssignCRT (Input);
    Reset (Input);
    AssignCRT (Output);
    Rewrite (Output)
  end;

to end do
  begin
    Close (Input);
    Reset (Input, '-');
    Close (Output);
    Rewrite (Output, '-');
    CRT_Done
  end;

end.
