{
This unit implements some features found in Standard Pascal,
Extended Pascal and GNU Pascal for Borland Pascal. It is meant as a
help for gradually converting BP programs to GPC, while maintaining
the ability to compile them with BP as long as possible, e.g., to
compare their behaviour under BP and GPC. It is NOT meant as yet
another BP extension unit for programs that are not intended to be
compiled with GPC. Therefore, this unit comes as part of the GPC
package. Besides, since the interface of some of GPC's routines can
change, and this unit will be changed accordingly, it is not
reasonable at this point of time to distribute this unit separately
from GPC.

IMPORTANT NOTE: This unit is distributed under the GNU GPL. Please
read the file COPYING for the exact terms of the GPL. In short, the
GPL says that you must distribute any code that uses this unit under
the GPL as well, which means that you have to make the source code
available whenever you distribute a binary of the code, and that you
must allow recipients to modify the code and redistribute it under
the GPL. However, if you compile your code with GPC, it will not use
this unit, but rather GPC's own units which, if compiled with a GNU
compiler, do not cause the resulting executable to be covered by the
GNU General Public License, so you can, e.g., distribute programs
compiled with GPC as binaries without making available their source
code if you respect the other relevant licenses.

This units provides a little bit of GPC compatibility for BP, and
removes some silly restrictions of BP (e.g., the fixed string sizes
in FSplit, or having to call SwapVectors and make available the free
memory explicitly before/after calling Exec), but of course not the
really bad ones (like the 64 KB limit, or the 255 characters limit
for strings, or the 0..255 limit for sets). Some routines here have
restrictions that are not present in GPC (e.g., the Min and Max
functions here only work on integers while GPC's built-in Min and
Max functions work on all ordinal types and real numbers). It does
not emulate all GPC functions that could be emulated on BP, by far,
and there are many GPC functions that cannot possibly be emulated on
BP, so the usefulness of this unit is quite limited, and you will
probably find that it's better to give up the ability to compile
your code with BP after some time while you make sure that your code
works as expected with GPC. The purpose of this unit is to fill in
this time.

How to use this unit:

- Install: Copy this file to gpc.pas in a unit directory that is
  searched by BP, but NOT by GPC (e.g. c:\bp\units, if you have
  installed BP in c:\bp). Also copy GPC's crt.inc file into this
  directory, and make sure that BP searches it for include files as
  well. When you get a new release of GPC, don't forget to install
  the new versions of this unit and crt.inc that come with it. If
  you are using BP on a real file system (e.g. Linux with DosEmu),
  you can set symlinks to the files installed with GPC once and for
  all, but on a Dos file system, you'll have to copy the files after
  each GPC release.

- Use: In your code, simply add a `uses GPC' statement (which will
  also work under GPC, but use GPC's own GPC unit then). If you use
  any of the Strings, Dos and/or CRT units, use those units BEFORE
  the GPC unit, since this unit overwrites some of those units'
  routines.

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

{$ifdef __GPC__}
{$error This unit is meant to provide GPC compatibility for BP.
GPC does not need this unit. If GPC tries to use it, you have
installed it in a wrong place. Please install it in a unit
directory that is searched by BP, but not by GPC.}
{$endif}

{$ifndef VER70} This unit is only for TP/BP 7.0 {$endif}

unit GPC;

interface

{$B-,I-,A+,F+,N+}

uses Strings, Dos, CRT;

{ System extensions }

const
  BitsBigEndian          = False;
  BytesBigEndian         = False;
  WordsBigEndian         = False;
  NeedAlignment          = False;

  MaxVarSize             = $fff0;
  Binding_Name_Length    = High (PathStr);

  SpaceCharacters        = [' ', #9];

  OSDosFlag              = True;
  QuotingCharacter       = #0;
  PathSeparator          = ';';
  DirSeparator           = '\';
  DirSeparators          = [':', '\'];
  ExtSeparator           = '.';
  DirRoot                = '\';
  DirSelf                = '.';
  DirParent              = '..';
  NullDeviceName         = 'nul';
  TTYDeviceName          = 'con';
  ConsoleDeviceName      = 'con';
  EnvVarCharsFirst       = ['A' .. 'Z', 'a' .. 'z', '_'];
  EnvVarChars            = EnvVarCharsFirst + ['0' .. '9'];
  PathEnvVar             = 'PATH';
  ShellEnvVar            = 'COMSPEC';
  ShellExecCommand       = '/c';
  ConfigFileMask         = '*.cfg';
  FileNamesCaseSensitive = False;

  NewLine = #13#10;
  LineBreak = #13#10;
  WildCardChars = ['*', '?', '[', ']'];
  FileNameSpecialChars = (WildCardChars + SpaceCharacters + ['{', '}', '$', QuotingCharacter]) - DirSeparators;

  HeapErrorRunError = 0;
  HeapErrorNil      = 1;
  HeapErrorRetry    = 2;

  MonthNames : array [1 .. 12] of String [9] =
    ('January', 'February', 'March', 'April', 'May', 'June',
     'July', 'August', 'September', 'October', 'November', 'December');

  MonthLength : array [1 .. 12] of Integer =
    (31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);

  { File mode constants that are ORed for BindingType.Mode and ChMod. }
  fmSetUID           = 2048;
  fmSetGID           = 1024;
  fmSticky           = 512;
  fmUserReadable     = 256;
  fmUserWritable     = 128;
  fmUserExecutable   = 64;
  fmGroupReadable    = 32;
  fmGroupWritable    = 16;
  fmGroupExecutable  = 8;
  fmOthersReadable   = 4;
  fmOthersWritable   = 2;
  fmOthersExecutable = 1;

type
  TChars      = packed array [1 .. MaxVarSize div SizeOf (Char)] of Char;
  PChars      = ^TChars;
  CString     = PChar;
  PCStrings   = ^TCStrings;
  TCStrings   = array [0 .. MaxVarSize div SizeOf (CString) - 1] of CString;
  TString     = String;
  PString     = ^String;
  CharSet     = set of Char;
  Cardinal    = Word;
  ByteInt     = ShortInt;
  ByteCard    = Byte;
  ShortWord   = Byte;
  ShortCard   = ShortWord;
  MedInt      = Integer;
  MedWord     = Word;
  MedCard     = Word;
  LongWord    = LongInt; { Not available as an unsigned integer in BP }
  LongCard    = LongWord;
  LongestInt  = LongInt;
  LongestWord = LongWord;
  LongestCard = LongCard;
  SizeType    = Word;
  PtrDiffType = Word;
  PtrInt      = LongInt;
  PtrWord     = LongWord;
  PtrCard     = LongCard;
  DirPtr      = ^SearchRec;

  AnyFile     = Text;
  PAnyFile    = ^AnyFile;

  GlobBuffer = record
    Count    : Integer;
    Result   : PCStrings;
    Internal : Pointer
  end;

  TimeStamp = packed record
    DateValid,
    TimeValid   : Boolean;
    Year        : Integer;
    Month       : 1 .. 12;
    Day         : 1 .. 31;
    DayOfWeek   : 0 .. 6;  { 0 means Sunday }
    Hour        : 0 .. 23;
    Minute      : 0 .. 59;
    Second      : 0 .. 61;
    MicroSecond : 0 .. 999999
  end;

  UnixTimeType = LongInt;
  MicroSecondTimeType = Comp;

  BindingType = packed record
    Bound             : Boolean;
    Force             : Boolean;      { Ignored }
    Extensions_Valid  : Boolean;
    Readable          : Boolean;
    Writable          : Boolean;
    Executable        : Boolean;
    Existing          : Boolean;
    Directory         : Boolean;
    Special           : Boolean;
    SymLink           : Boolean;
    AccessTime,                       { These times are Unix times (seconds since 1970-01-01, 00:00:00), }
    ModificationTime,                 { but relative to the current time zone (unlike Unix time) because }
    ChangeTime        : UnixTimeType; { time zone information is not generally available under Dos. }
    User,                             { Dummy }
    Group,                            { Dummy }
    Mode,                             { Access permissions }
    Device,                           { Device the file is on }
    INode             : Integer;      { Dummy }
    TextBinary        : Boolean;      { Does nothing! Implementing it would take a TFDD to do the conversion,
                                        and overwritten Reset, Rewrite and Append procedures for text files,
                                        but those would conflict with the built-in ones for typed and untyped
                                        files, and I don't think it's worth that effort to support an
                                        obsolete compiler. }
    Handle            : Integer;      { Ignored }
    Name              : String [Binding_Name_Length]
  end;

var
  Null : Integer absolute 0 : 0; { Only Integer }
  StdErr : Text;

{ Only for Integers }
function  Min (a, b : Integer) : Integer;
function  Max (a, b : Integer) : Integer;

function  InPortB  (PortNumber : Word) : Byte;
function  InPortW  (PortNumber : Word) : Word;
procedure OutPortB (PortNumber : Word; Val : Byte);
procedure OutPortW (PortNumber, Val : Word);

{ Dummies }
function  IOPerm (From, Num : MedCard; On : Integer) : Integer;
function  IOPL (Level : Integer) : Integer;
function  GetUID : Integer;
function  SetEUID (EUID : Integer) : Integer;

{ Emulates malloc(), i.e. returns nil if not enough memory available }
function  CGetMem (Size : SizeType) : Pointer;

function  HeapErrorNilReturn (Size : Word) : Integer;

function  MemComp (const s1, s2; Size : SizeType) : Integer;

{ Uniform access to big memory blocks for GPC and BP. See GPC's GPCUtil
  unit. In real mode, these routines will try to use XMS and EMS to
  provide access to as much memory as possible. }

type
  PBigMem = ^TBigMem;
  TBigMem = record
    { Public fields }
    Number, BlockSize : SizeType;
    Mappable : Boolean;
    { Private fields }
    {$ifdef MSDOS}
    XMSHandle, EMSHandle, XMSKBPerBlock, EMSPagesPerBlock, XMSBlocks, EMSBlocks,
    {$endif}
    ConventionalBlocks : Word;
    PConventional : array [1 .. $400] of Pointer
  end;

{ Note: the number of blocks actually allocated may be smaller than
  WantedNumber. Check the Number field of the result. }
function  AllocateBigMem (WantedNumber, aBlockSize : SizeType; WantMappable : Boolean) : PBigMem;
procedure DisposeBigMem (p : PBigMem);
procedure MoveToBigMem (var Source; p : PBigMem; BlockNumber : SizeType);
procedure MoveFromBigMem (p : PBigMem; BlockNumber : SizeType; var Dest);
{ Maps a big memory block into normal addressable memory and returns its
  address. The memory must have been allocated with WantMappable = True.
  The mapping is only valid until the next MapBigMem call. }
function  MapBigMem (p : PBigMem; BlockNumber : SizeType) : Pointer;

procedure ReverseBytes            (var Buf; ElementSize, Count : SizeType);
procedure ConvertFromLittleEndian (var Buf; ElementSize, Count : SizeType);
procedure ConvertFromBigEndian    (var Buf; ElementSize, Count : SizeType);
procedure ConvertToLittleEndian   (var Buf; ElementSize, Count : SizeType);
procedure ConvertToBigEndian      (var Buf; ElementSize, Count : SizeType);

procedure BlockReadLittleEndian   (var aFile : File; var   Buf; ElementSize, Count : SizeType);
procedure BlockReadBigEndian      (var aFile : File; var   Buf; ElementSize, Count : SizeType);
procedure BlockWriteLittleEndian  (var aFile : File; const Buf; ElementSize, Count : SizeType);
procedure BlockWriteBigEndian     (var aFile : File; const Buf; ElementSize, Count : SizeType);

{$P+}
procedure ReadStringLittleEndian  (var f : File; var s : String);
procedure ReadStringBigEndian     (var f : File; var s : String);
procedure WriteStringLittleEndian (var f : File; const s : String);
procedure WriteStringBigEndian    (var f : File; const s : String);
{$P-}

function  NewString (const s : String) : PString;
procedure DisposeString (p : PString);
function  IsUpCase             (ch : Char) : Boolean;
function  IsLoCase             (ch : Char) : Boolean;
function  IsAlphaNum           (ch : Char) : Boolean;
function  IsAlphaNumUnderscore (ch : Char) : Boolean;
function  IsPrintable          (ch : Char) : Boolean;
function  UpCase (Ch : Char) : Char; { Overwrites System version -- handles some international characters }
function  LoCase (Ch : Char) : Char;
function  UpCaseStr (s : String) : String;
function  LoCaseStr (s : String) : String;
{$P+}
procedure UpCaseString    (var s : String);
procedure LoCaseString    (var s : String);
procedure SetLength       (var s : String; NewLength : Integer);
function  StringOfChar    (Ch : Char; Count : Integer) : TString;
procedure TrimLeft        (var s : String);
procedure TrimRight       (var s : String);
procedure TrimBoth        (var s : String);
function  TrimLeftStr     (s : String) : TString;
function  TrimRightStr    (s : String) : TString;
function  TrimBothStr     (s : String) : TString;
function  GetStringCapacity (var s : String) : Integer; { NOTE: the parameter must be var (not const),
                                                          otherwise BP gets the capacity wrong! }
{$P-}

function  LastPos         (const SubStr, Str : String) : Integer;
function  CharPos         (const Chars : CharSet; const Str : String) : Integer;
function  LastCharPos     (const Chars : CharSet; const Str : String) : Integer;

function  PosFrom         (const SubStr, Str : String; From : Integer) : Integer;
function  LastPosTill     (const SubStr, Str : String; Till : Integer) : Integer;
function  CharPosFrom     (const Chars : CharSet; const Str : String; From : Integer) : Integer;
function  LastCharPosTill (const Chars : CharSet; const Str : String; Till : Integer) : Integer;

function  IsPrefix     (const Prefix, s : String) : Boolean;
function  IsSuffix     (const Suffix, s : String) : Boolean;

function  NewCString   (const Source : String) : CString;

function  Integer2String (i : Integer) : TString;

{ Only for text files. This routine is rather ridiculous than a real
  emulation, but it's good for a few things, like finding out if a
  file is writable or getting its modification time. }
procedure GetBinding (var f : Text; var aBinding : BindingType);

function  IsLeapYear (Year : Integer) : Boolean;
procedure Sleep (Seconds : Integer);
procedure SleepMicroSeconds (MicroSeconds : LongInt);
function  GetMicroSecondTime : MicroSecondTimeType;
function  GetCPUTime (var MicroSecond { : LongInt }) : LongInt; { type of MicroSecond is omitted, so one can pass null }

function  AddDirSeparator    (s : String) : TString;
function  RemoveDirSeparator (s : String) : TString;
function  GetCurrentDirectory : TString;
function  GetTempDirectory    : TString;
function  ExpandEnvironment (var s : String) : Boolean;
function  FileExists      (const aFileName : String) : Boolean;
function  DirectoryExists (      aFileName : String) : Boolean;
function  PathExists      (const aFileName : String) : Boolean;
function  RelativePath    (const Path : String; OnlyCurDir, Quoted : Boolean) : TString;
function  DirFromPath     (const Path : String) : TString;
function  NameFromPath    (const Path : String) : TString;
function  ExtFromPath     (const Path : String) : TString;
function  NameExtFromPath (const Path : String) : TString;
function  OpenDir  (Name : String) : DirPtr;
function  ReadDir  (Dir : DirPtr) : TString;
procedure CloseDir (Dir : DirPtr);
function  HasWildCards (const s : String) : Boolean;
function  FileNameMatch (const Pattern, Name : String) : Boolean;
procedure Glob     (var Buf : GlobBuffer; Pattern : String); { Dummy }
procedure GlobFree (var Buf : GlobBuffer);
function  ConfigFileName (Prefix, Name : String; Global : Boolean) : TString;
function  DataDirectoryName (Prefix, Name : String) : TString;

procedure GetTimeStamp (var aTimeStamp : TimeStamp);

procedure CheckInOutRes;
function  GetIOErrorMessage : CString;

procedure Close     (var f : Text); { Overwrites System version -- does not cause an error if file was not open }
procedure CloseFile (var f : File); { The same for untyped files -- can't be called `Close' as well :-( }

function  Execute (CmdLine : String) : Integer;

type
  TProcedure = procedure;

procedure RegisterRestoreTerminal (ForAnotherProcess : Boolean; Proc : TProcedure);
function  UnregisterRestoreTerminal (ForAnotherProcess : Boolean; Proc : TProcedure) : Boolean;
procedure RestoreTerminal (ForAnotherProcess : Boolean);

{ TFDD (interface is subject to change) }

type
  TOpenMode   = (foReset, foRewrite, foAppend, foSeekRead, foSeekWrite, foSeekUpdate);
  TOpenProc   = procedure (var PrivateData; Mode : TOpenMode);
  TSelectFunc = function  (var PrivateData; Writing : Boolean) : Integer;
  TSelectProc = procedure (var PrivateData; var ReadSelect, WriteSelect, ExceptSelect : Boolean);
  TReadFunc   = function  (var PrivateData; var   Buffer; Size : SizeType) : SizeType;
  TWriteFunc  = function  (var PrivateData; const Buffer; Size : SizeType) : SizeType;
  TFileProc   = procedure (var PrivateData);
  TFlushProc  = TFileProc;
  TCloseProc  = TFileProc;
  TDoneProc   = TFileProc;

procedure AssignTFDD (var f : AnyFile;
                      OpenProc    : TOpenProc;
                      SelectFunc  : TSelectFunc; { will not be called in BP! }
                      SelectProc  : TSelectProc; { will not be called in BP! }
                      ReadFunc    : TReadFunc;
                      WriteFunc   : TWriteFunc;
                      FlushProc   : TFlushProc;
                      CloseProc   : TCloseProc;
                      DoneProc    : TDoneProc;   { will not be called in BP! }
                      PrivateData : Pointer);

{ Strings extensions }

function CString2String (Str : CString) : TString;

{ Printer extensions }

procedure AssignLst (var f : Text);

{ GetOpt -- very primitive }

const
  EndOfOptions      = #255;
  NoOption          = #1;
  UnknownOption     = '?';
  LongOption        = #0;
  UnknownLongOption = '?';

  FirstNonOption         : Integer = 0;
  HasOptionArgument      : Boolean = False;
  OptionArgument         : TString = '';
  UnknownOptionCharacter : Char    = '?';
  GetOptErrorFlag        : Boolean = False;

function GetOpt (OptString : CString) : Char;

{ Dos extensions }

type
  TDosAttr = Word;

const
  { DosError codes }
  DosError_FileNotFound = 2;
  DosError_PathNotFound = 3;
  DosError_AccessDenied = 5;
  DosError_InvalidMem   = 9;
  DosErorr_InvalidEnv   = 10;
  DosError_NoMoreFiles  = 18;
  DosError_IOError      = 29;
  DosError_ReadFault    = 30;

procedure FindFirst (const Path : String; Attr : Word; var SR : SearchRec); { Overwrites Dos version -- see FindNext }
procedure FindNext  (var SR : SearchRec); { Overwrites Dos version -- transforms resulting file name to lower case }
procedure FindClose (var SR : SearchRec); { Does nothing }
function  FExpand   (const Path : String) : TString; { Overwrites Dos version -- transforms result to lower case,
                                                       removes trailing `.' when necessary. Returns '' if nonexistent path. }
{$P+}
procedure FSplit (const Path : String; var Dir, Name, Ext : String); { Overwrites Dos version -- accepts strings of any size }
{$P-}
function  GetEnv (const EnvVar : String) : TString; { Repeats Dos version }
procedure SwapVectors; { Overwrites Dos version -- does nothing }
procedure Exec (const Path, ComLine : String); { Overwrites Dos version -- does SwapVectors automatically, calls
                                                 RestoreTerminal, frees up some conventional memory space if
                                                 possible (in real mode) }

{ CRT extensions }

type
  TTextAttr = Byte;

procedure CRTInit;
function  KeyPressed : Boolean; { Overwrites CRT version -- see ReadKey }
function  ReadKey : Char; { Overwrites CRT version -- handles Shift-Insert and Shift-Delete }
procedure TextMode (Mode : Integer); { Overwrites CRT version -- updates some internal data }

{ Keyboard and character graphics constants, from GPC's crt.inc }
{$i crt.inc}

const
  XCRT       : Boolean = False;
  VisualBell : Boolean = False; { If set, Beep does a Flash instead }

type
  TKey = Word;

  TCursorShape = (CursorIgnored, CursorHidden, CursorNormal, CursorFat, CursorBlock);

  TCRTUpdate = (UpdateNever, UpdateWaitInput, UpdateInput,
                UpdateRegularly, UpdateAlways);

  TPoint = record
    X, Y : Integer
  end;

  TWindowXY = record
    X, Y : Byte
  end;

  PCharAttr = ^TCharAttr;
  TCharAttr = record
    Ch        : Char;
    Attr      : TTextAttr;
    PCCharSet : Boolean
  end;

  PCharAttrs = ^TCharAttrs;
  TCharAttrs = array [1 .. MaxVarSize div SizeOf (TCharAttr)] of TCharAttr;

var
  { WindMin, WindMax, WindowMin and WindowMax are OBSOLETE! Use Window
    and GetWindow instead. }
  WindowMin : TWindowXY absolute WindMin;
  WindowMax : TWindowXY absolute WindMax;

  ScreenSize : TPoint; { Contains the size of the screen }

  IsMonoMode : Boolean;

const
  VirtualShiftState : Integer = 0;

{ Tell which modifier keys are currently pressed (or 0 on systems where
  this is not supported). }
function  GetShiftState : Integer;

{ Get the extent of the current window. Use this procedure rather than
  reading WindMin and WindMax or WindowMin and WindowMax, since this
  routine allows for window sizes larger than 255. }
procedure GetWindow (var x1, y1, x2, y2 : Integer);

{ Dummies }
procedure SetCRTUpdate (Update : TCRTUpdate);
procedure CRTUpdate;

{ Returns Ord (key) for normal keys and $100 * Ord (fkey) for function keys }
function  ReadKeyWord : TKey;

{ Extract the character and scan code from a TKey value }
function  Key2Char (k : TKey) : Char;
function  Key2Scan (k : TKey) : Char;

{ Convert a key to upper/lower case if it is a letter, leave it unchanged
  otherwise }
function  UpCaseKey (k : TKey) : TKey;
function  LoCaseKey (k : TKey) : TKey;

{ Produce a beep or a screen flash }
procedure Beep;
procedure Flash;

{ Get size of current window (calculated using GetWindow) }
function  GetXMax : Integer;
function  GetYMax : Integer;

{ Get/goto an absolute position }
function  WhereXAbs : Integer;
function  WhereYAbs : Integer;
procedure GotoXYAbs (X, Y : Integer);

procedure SetCursorShape (Shape : TCursorShape);
function  GetCursorShape : TCursorShape;

procedure HideCursor;
procedure HiddenCursor;
procedure NormalCursor;
procedure FatCursor;
procedure BlockCursor;
procedure IgnoreCursor;

function  GetTextColor : Integer;
function  GetTextBackground : Integer;

{ Write string at given position without moving the cursor. Truncated at
  right margin. }
procedure WriteStrAt (x, y : Integer; s : String; Attr : TTextAttr);

{ Write (several copies of) a char at given position without moving the
  cursor. Truncated at right margin. }
procedure WriteCharAt (x, y, Count : Integer; Ch : Char; Attr : TTextAttr);

{ Write characters with specified attributes at the given position without
  moving the cursor. Truncated at right margin. }
procedure WriteCharAttrAt (x, y, Count : Integer; CharAttr : PCharAttrs);

{ Write a char while moving the cursor }
procedure WriteChar (Ch : Char);

{ Read a character from a screen posistion }
procedure ReadChar (x, y : Integer; var Ch : Char; var Attr : TTextAttr);

{ Change only text attribute, leave character }
procedure ChangeTextAttr (x, y, Count : Integer; NewAttr : TTextAttr);

{ Fill current window }
procedure FillWin (Ch : Char; Attr : TTextAttr);

{ Calculate size of memory required for ReadWin in current window. }
function  WinSize : SizeType;

{ Save window contents. Buf must be WinSize bytes large. }
procedure ReadWin (var Buf);

{ Restore window contents saved by ReadWin. The size of the current window
  must match the size of the window from which ReadWin was used, but the
  position may be different. }
procedure WriteWin (const Buf);

type
  WinState = record
    x1, y1, x2, y2, WhereX, WhereY, NewX1, NewY1, NewX2, NewY2 : Integer;
    TextAttr : TTextAttr;
    CursorShape : TCursorShape;
    TextMode : Integer;
    BufSize : SizeType;
    Buffer : ^Byte
  end;

{ Save window position and size, cursor position, text attribute and cursor
  shape -- NOT the window contents. }
procedure SaveWin (var State : WinState);

{ Make a new window (like Window), and save the contents of the screen
  below the window as well as the position and size, cursor position,
  text attribute and cursor shape of the old window. }
procedure MakeWin (var State : WinState; x1, y1, x2, y2 : Integer);

{ Create window in full size, save previous text mode and all values
  that MakeWin does. }
procedure SaveScreen (var State : WinState);

{ Restore the data saved by SaveWin, MakeWin or SaveScreen. }
procedure RestoreWin (var State : WinState);

{ TPCRT compatibility }
{ Write a string at a given position without moving the cursor. Truncated at
  right margin. }
procedure WriteString (const s : String; y, x : Integer);

{ Write a string at a given position with a given attribute without moving
  the cursor. Truncated at right margin. }
procedure FastWriteWindow (const s : String; y, x : Integer; Attr : TTextAttr);

{ Write a string at a given absolute position with a given attribute without
  moving the cursor. Truncated at right margin. }
procedure FastWrite       (const s : String; y, x : Integer; Attr : TTextAttr);

{
Trapping runtime errors, see the Trap unit. Besides the notes in that
unit, for BP additionally note:
- Trap has been tested with BP version 7.0 only and mainly in real
  mode, but it may work with TP 6.0 or on other platforms, too.
- You should not install any ExitProc's after calling TrapExec.
- Real mode only: You must not call TrapExec from an overlayed unit.
- protected mode/Windoze only: You should call TrapExec from a code
  segment with the following attributes: FIXED PRELOAD PERMANENT.
  (Though I'm not sure if this is really necessary.)
}
const
  TrappedExitCode  : Integer = 0;
  TrappedErrorAddr : Pointer = nil;

type
  TTrapProc = procedure (Trapped : Boolean);

procedure TrapExec (p : TTrapProc);

implementation

const
  StartCPUTimeSeconds      : LongInt = 0;
  StartCPUTimeMicroSeconds : LongInt = 0;
  MonthOffset : array [1 .. 12] of Integer = (0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334);

var
  ExitSave : Pointer;

{ System extensions }

function Min (a, b : Integer) : Integer;
begin
  if a < b then Min := a else Min := b
end;

function Max (a, b : Integer) : Integer;
begin
  if a > b then Max := a else Max := b
end;

function InPortB (PortNumber : Word) : Byte;
begin
  InPortB := Port [PortNumber]
end;

function InPortW (PortNumber : Word) : Word;
begin
  InPortW := PortW [PortNumber]
end;

procedure OutPortB (PortNumber : Word; Val : Byte);
begin
  Port [PortNumber] := Val
end;

procedure OutPortW (PortNumber, Val : Word);
begin
  PortW [PortNumber] := Val
end;

function IOPerm (From, Num : MedCard; On : Integer) : Integer;
begin
  IOPerm := 0
end;

function IOPL (Level : Integer) : Integer;
begin
  IOPL := 0
end;

function GetUID : Integer;
begin
  GetUID := 0
end;

function SetEUID (EUID : Integer) : Integer;
begin
  SetEUID := 0
end;

function CGetMem (Size : SizeType) : Pointer;
var p, HeapErrorSave : Pointer;
begin
  HeapErrorSave := HeapError;
  HeapError := @HeapErrorNilReturn;
  GetMem (p, Size);
  HeapError := HeapErrorSave;
  CGetMem := p
end;

function HeapErrorNilReturn (Size : Word) : Integer;
begin
  HeapErrorNilReturn := HeapErrorNil
end;

function MemComp (const s1, s2; Size : SizeType) : Integer; Assembler;
asm
   mov  dx, ds
   lds  si, s1
   les  di, s2
   cld
   mov  ax, Size
   mov  cx, ax
   shr  cx, 1
   jcxz @1
   repe cmpsw
   jne  @2
@1:shr  ax, 1
   jnc  @3
   cmpsb
@2:mov  ax, 1
   ja   @4
   mov  ax, -1
   jb   @4
@3:mov  ax, 0
@4:mov  ds, dx
end;

procedure ReverseBytes (var Buf; ElementSize, Count : SizeType);
var
  i, j, o1, o2 : SizeType;
  b : Byte;
  ByteBuf : array [1 .. { ElementSize * Count -- BP can't handle this } MaxVarSize div SizeOf (Byte)] of Byte absolute Buf;
begin
  for i := 0 to Count - 1 do
    for j := 1 to ElementSize div 2 do
      begin
        o1 := i * ElementSize + j;
        o2 := i * ElementSize + ElementSize + 1 - j;
        b := ByteBuf [o1];
        ByteBuf [o1] := ByteBuf [o2];
        ByteBuf [o2] := b
      end
end;

procedure ConvertFromLittleEndian (var Buf; ElementSize, Count : SizeType);
begin
  if BytesBigEndian then ReverseBytes (Buf, ElementSize, Count)
end;

procedure ConvertFromBigEndian (var Buf; ElementSize, Count : SizeType);
begin
  if not BytesBigEndian then ReverseBytes (Buf, ElementSize, Count)
end;

procedure ConvertToLittleEndian (var Buf; ElementSize, Count : SizeType);
begin
  if BytesBigEndian then ReverseBytes (Buf, ElementSize, Count)
end;

procedure ConvertToBigEndian (var Buf; ElementSize, Count : SizeType);
begin
  if not BytesBigEndian then ReverseBytes (Buf, ElementSize, Count)
end;

type
  TByteArray = array [0 .. MaxVarSize div SizeOf (Byte)] of Byte;
  PByteArray = ^TByteArray;

procedure BlockReadLittleEndian (var aFile : File; var Buf; ElementSize, Count : SizeType);
begin
  BlockRead (aFile, Buf, ElementSize * Count);
  if InOutRes = 0 then
    ConvertFromLittleEndian (Buf, ElementSize, Count)
end;

procedure BlockReadBigEndian (var aFile : File; var Buf; ElementSize, Count : SizeType);
begin
  BlockRead (aFile, Buf, ElementSize * Count);
  if InOutRes = 0 then
    ConvertFromBigEndian (Buf, ElementSize, Count)
end;

procedure BlockWriteLittleEndian (var aFile : File; const Buf; ElementSize, Count : SizeType);
var p : Pointer;
begin
  GetMem (p, ElementSize * Count);
  Move (Buf, p^, ElementSize * Count);
  ConvertToLittleEndian (p^, ElementSize, Count);
  BlockWrite (aFile, p^, ElementSize * Count);
  FreeMem (p, ElementSize * Count)
end;

procedure BlockWriteBigEndian (var aFile : File; const Buf; ElementSize, Count : SizeType);
var p : Pointer;
begin
  GetMem (p, ElementSize * Count);
  Move (Buf, p^, ElementSize * Count);
  ConvertToBigEndian (p^, ElementSize, Count);
  BlockWrite (aFile, p^, ElementSize * Count);
  FreeMem (p, ElementSize * Count)
end;

{$P+}
procedure ReadStringLittleEndian (var f : File; var s : String);
var StringLength, Temp : LongInt;
begin
  BlockRead (f, StringLength, SizeOf (StringLength));
  { Ignore the high bytes, BP can't handle them }
  BlockRead (f, Temp, SizeOf (Temp));
  if InOutRes = 0 then
    begin
      ConvertFromLittleEndian (StringLength, SizeOf (StringLength), 1);
      { Ignore characters exceeding 255, BP can't handle them }
      Temp := StringLength;
      if Temp > $ff then Temp := $ff;
      SetLength (s, Temp);
      BlockRead (f, s [1], Temp);
      Seek (f, FilePos (f) + StringLength - Temp)
    end
end;

procedure ReadStringBigEndian (var f : File; var s : String);
var StringLength, Temp : LongInt;
begin
  { Ignore the high bytes, BP can't handle them }
  BlockRead (f, Temp, SizeOf (Temp));
  BlockRead (f, StringLength, SizeOf (StringLength));
  if InOutRes = 0 then
    begin
      ConvertFromBigEndian (StringLength, SizeOf (StringLength), 1);
      { Ignore characters exceeding 255, BP can't handle them }
      Temp := StringLength;
      if Temp > $ff then Temp := $ff;
      SetLength (s, Temp);
      BlockRead (f, s [1], Temp);
      Seek (f, FilePos (f) + StringLength - Temp)
    end
end;

procedure WriteStringLittleEndian (var f : File; const s : String);
var StringLength : LongInt;
begin
  StringLength := Length (s);
  ConvertToLittleEndian (StringLength, SizeOf (StringLength), 1);
  BlockWrite (f, StringLength, SizeOf (StringLength));
  StringLength := 0;
  BlockWrite (f, StringLength, SizeOf (StringLength));
  BlockWrite (f, PChar (@s [1])^ { BP requires this nonsense }, Length (s))
end;

procedure WriteStringBigEndian (var f : File; const s : String);
var StringLength : LongInt;
begin
  StringLength := 0;
  BlockWrite (f, StringLength, SizeOf (StringLength));
  StringLength := Length (s);
  ConvertToBigEndian (StringLength, SizeOf (StringLength), 1);
  BlockWrite (f, StringLength, SizeOf (StringLength));
  BlockWrite (f, PChar (@s [1])^ { BP requires this nonsense }, Length (s))
end;
{$P-}

function NewString (const s : String) : PString;
var p : PString;
begin
  GetMem (p, Length (s) + 1);
  p^ := s;
  NewString := p
end;

procedure DisposeString (p : PString);
begin
  if p <> nil then FreeMem (p, Length (p^) + 1)
end;

const
  UpCaseSet = ['A' .. 'Z', 'é', 'ô', 'ö', 'A', 'E', 'I', 'O', 'U', 'è', 'í', 'Ä', 'ê', '•'];
  LoCaseSet = ['a' .. 'z', 'Ñ', 'î', 'Å', 'É', 'Ö', '†', 'à', 'â', 'ä', 'ã', 'å',
               'ç', '°', 'ì', 'ï', '¢', 'ñ', 'ó', '£', 'Ü', 'ë', 'á', 'Ç', '§'];

function IsUpCase (ch : Char) : Boolean;
begin
  IsUpCase := ch in UpCaseSet
end;

function IsLoCase (ch : Char) : Boolean;
begin
  IsLoCase := ch in LoCaseSet
end;

function IsAlphaNum (ch : Char) : Boolean;
begin
  IsAlphaNum := ch in (UpCaseSet + LoCaseSet + ['0' .. '9'])
end;

function IsAlphaNumUnderscore (ch : Char) : Boolean;
begin
  IsAlphaNumUnderscore := ch in (UpCaseSet + LoCaseSet + ['0' .. '9', '_'])
end;

function IsPrintable (ch : Char) : Boolean;
begin
  IsPrintable := ch >= ' '
end;

function UpCase (Ch : Char) : Char;
begin
  case Ch of
    #0 .. Pred ('a') :   UpCase := Ch;
    'a' .. 'z' :         UpCase := Chr (Ord (Ch) - Ord ('a') + Ord ('A'));
    'Ñ' :                UpCase := 'é';
    'î' :                UpCase := 'ô';
    'Å' :                UpCase := 'ö';
    'É', 'Ö', '†' :      UpCase := 'A';
    'à', 'â', 'ä' :      UpCase := 'E';
    'ã', 'å', 'ç', '°' : UpCase := 'I';
    'ì', 'ï', '¢' :      UpCase := 'O';
    'ñ', 'ó', '£' :      UpCase := 'U';
    'Ü' :                UpCase := 'è';
    'ë' :                UpCase := 'í';
    'á' :                UpCase := 'Ä';
    'Ç' :                UpCase := 'ê';
    '§' :                UpCase := '•';
    else                 UpCase := Ch
  end
end;

function LoCase (Ch : Char) : Char;
begin
  case Ch of
    #0 .. Pred ('A') :   LoCase := Ch;
    'A' .. 'Z' :         LoCase := Chr (Ord (Ch) - Ord ('A') + Ord ('a'));
    'é' :                LoCase := 'Ñ';
    'ô' :                LoCase := 'î';
    'ö' :                LoCase := 'Å';
    'è' :                LoCase := 'Ü';
    'í' :                LoCase := 'ë';
    'Ä' :                LoCase := 'á';
    'ê' :                LoCase := 'Ç';
    '•' :                LoCase := '§';
    else                 LoCase := Ch
  end
end;

function UpCaseStr (s : String) : String;
begin
  UpCaseString (s);
  UpCaseStr := s
end;

function LoCaseStr (s : String) : String;
begin
  LoCaseString (s);
  LoCaseStr := s
end;

{$P+}
procedure UpCaseString (var s : String);
var z : Integer;
begin
  for z := 1 to Length (s) do s [z] := UpCase (s [z])
end;

procedure LoCaseString (var s : String);
var z : Integer;
begin
  for z := 1 to Length (s) do s [z] := LoCase (s [z])
end;

procedure SetLength (var s : String; NewLength : Integer);
begin
  s [0] := Chr (Min (GetStringCapacity (s), Max (0, NewLength)))
end;

function StringOfChar (Ch : Char; Count : Integer) : TString;
var
  s : TString;
  i : Integer;
begin
  SetLength (s, Min (GetStringCapacity (s), Max (0, Count)));
  for i := 1 to Length (s) do s [i] := Ch;
  StringOfChar := s
end;

procedure TrimLeft (var s : String);
var i : Integer;
begin
  i := 1;
  while (i <= Length (s)) and (s [i] in SpaceCharacters) do Inc (i);
  Delete (s, 1, i - 1)
end;

procedure TrimRight (var s : String);
var i : Integer;
begin
  i := Length (s);
  while (i > 0) and (s [i] in SpaceCharacters) do Dec (i);
  Delete (s, i + 1, Length(s) - i)
end;

procedure TrimBoth (var s : String);
begin
  TrimLeft (s);
  TrimRight (s)
end;

function TrimLeftStr (s : String) :TString;
begin
  TrimLeft (s);
  TrimLeftStr := s
end;

function TrimRightStr (s : String) : TString;
begin
  TrimRight (s);
  TrimRightStr := s
end;

function TrimBothStr (s : String) : TString;
begin
  TrimBoth (s);
  TrimBothStr := s
end;

function GetStringCapacity (var s : String) : Integer;
begin
  GetStringCapacity := High (s)
end;
{$P-}

function LastPos (const SubStr, Str : String) : Integer;
begin
  LastPos := LastPosTill (SubStr, Str, Length (Str))
end;

function CharPos (const Chars : CharSet; const Str : String) : Integer;
var i : Integer;
begin
  i := 1;
  while (i <= Length (Str)) and not (Str [i] in Chars) do Inc (i);
  if i > Length (Str) then CharPos := 0 else CharPos := i
end;

function LastCharPos (const Chars : CharSet; const Str : String) : Integer;
var i : Integer;
begin
  i := Length (Str);
  while (i > 0) and not (Str [i] in Chars) do Dec (i);
  LastCharPos := i
end;

function PosFrom (const SubStr, Str : String; From : Integer) : Integer;
var m, i, n : Integer;
begin
  m := Max (1, From);
  case Length (SubStr) of
    0: PosFrom := From;
    1: begin
         i := m;
         while (i <= Length (Str)) and (Str [i] <> SubStr [1]) do Inc (i);
         if i > Length (Str) then PosFrom := 0 else PosFrom := i
       end;
    else
      n := Length (Str) - Length (SubStr) + 1;
      i := m;
      while (i <= n) and (MemComp (Str [i], SubStr [1], Length (SubStr)) <> 0) do Inc (i);
      if i > n then PosFrom := 0 else PosFrom := i
  end
end;

function LastPosTill (const SubStr, Str : String; Till : Integer) : Integer;
var m, i : Integer;
begin
  m := Max (0, Min (Length (Str), Till));
  case Length (SubStr) of
    0: LastPosTill := m + 1;
    1: begin
         i := m;
         while (i > 0) and (Str [i] <> SubStr [1]) do Dec (i);
         LastPosTill := i
       end;
    else
      i := m - Length (SubStr) + 1;
      while (i > 0) and (MemComp (Str [i], SubStr [1], Length (SubStr)) <> 0) do Dec (i);
      if i < 0 then LastPosTill := 0 else LastPosTill := i
  end
end;

function CharPosFrom (const Chars : CharSet; const Str : String; From : Integer) : Integer;
var i : Integer;
begin
  i := Max (1, From);
  while (i <= Length (Str)) and not (Str [i] in Chars) do Inc (i);
  if i > Length (Str) then CharPosFrom := 0 else CharPosFrom := i
end;

function LastCharPosTill (const Chars : CharSet; const Str : String; Till : Integer) : Integer;
var i : Integer;
begin
  i := Max (0, Min (Length (Str), Till));
  while (i > 0) and not (Str [i] in Chars) do Dec (i);
  LastCharPosTill := i
end;

function IsPrefix (const Prefix, s : String) : Boolean;
begin
  IsPrefix := (Length (s) >= Length (Prefix)) and (Copy (s, 1, Length (Prefix)) = Prefix)
end;

function IsSuffix (const Suffix, s : String) : Boolean;
begin
  IsSuffix := (Length (s) >= Length (Suffix)) and (Copy (s, Length (s) - Length (Suffix) + 1, Length (Suffix)) = Suffix)
end;

function NewCString (const Source : String) : CString;
var Dest : CString;
begin
  GetMem (Dest, Length (Source) + 1);
  Move (Source [1], Dest [0], Length (Source));
  Dest [Length (Source)] := #0;
  NewCString := Dest
end;

function Integer2String (i : Integer) : TString;
var s : TString;
begin
  Str (i, s);
  Integer2String := s
end;

procedure GetBinding (var f : Text; var aBinding : BindingType);
var
  Attr : TDosAttr;
  dt : DateTime;
  p, y2 : Integer;
  sr : SearchRec;
  ExpandedName : PathStr;
begin
  with aBinding do
    begin
      Name := TextRec (f).Name;
      p := Pos (#0, Name);
      if p <> 0 then Name [0] := Chr (p - 1);
      ExpandedName := FExpand (Name);
      { GetFTime works only on opened files, and FileSize doesn't work on
        text files, so we have to get this information via FindFirst }
      FindFirst (Name, Dos.AnyFile - VolumeID, sr);
      if DosError = 0 then
        begin
          UnPackTime (sr.Time, dt);
          with dt do
            begin
              y2 := Year - Ord (Month <= 2);
              ModificationTime := Sec + 60 * (Min + 60 * (Hour + 24 *
                                    LongInt (Day - 1 + MonthOffset [Month] +
                                     365 * (LongInt (Year) - 1970) + (y2 - 1968) div 4 -
                                     (y2 - 1900) div 100 + (y2 - 1600) div 400)))
            end;
          Attr := sr.Attr
        end
      else
        begin
          ModificationTime := - 1;
          Attr := 0
        end;
      Bound            := True;
      Force            := False;
      Extensions_Valid := True;
      Existing         := (DosError = 0) and (Attr and Dos.Directory = 0);
      Readable         := Existing;
      Writable         := (DosError <> 0) or (Attr and (Dos.Directory or ReadOnly) = 0);
                          { we pretend any non-existing file is writable, which is not exactly true ;-}
      Directory        := (DosError = 0) and (Attr and  Dos.Directory <> 0);
      Special          := False; (*@@should check for Dos devices, and unset Existing if it is one*)
      SymLink          := False;
      Executable       := Directory;
      AccessTime       := ModificationTime;
      ChangeTime       := ModificationTime;
      User             := - 1;
      Group            := - 1;
      Mode             := 0;
      if Readable   then Mode := Mode or fmUserReadable or fmGroupReadable or fmOthersReadable;
      if Writable   then Mode := Mode or fmUserWritable;
      if Executable then Mode := Mode or fmUserExecutable or fmGroupExecutable or fmOthersExecutable;
      Device           := Ord (UpCase (ExpandedName [1])) - Ord ('A') + 1;
      INode            := - 1;
      TextBinary       := False;
      Handle           := - 1
    end
end;

function IsLeapYear (Year : Integer) : Boolean;
begin
  IsLeapYear := (Year mod 4 = 0) and ((Year mod 100 <> 0) or (Year mod 400 = 0))
end;

procedure Sleep (Seconds : Integer);
begin
  Delay (1000 * Seconds)
end;

procedure SleepMicroSeconds (MicroSeconds : LongInt);
begin
  if MicroSeconds > 0 then
    Delay ((MicroSeconds + 500) div 1000)
end;

function GetMicroSecondTime : MicroSecondTimeType;
var
  Year, Month, Day, DOW, Hour, Min, Sec, Sec100 : Word;
  y2 : Integer;
  Seconds : LongInt;
begin
  GetDate (Year, Month, Day, DOW);
  GetTime (Hour, Min, Sec, Sec100);
  y2 := Year - Ord (Month <= 2);
  Seconds := Sec + 60 * (Min + 60 * (Hour + 24 *
               LongInt (Day - 1 + MonthOffset [Month] +
                365 * (LongInt (Year) - 1970) + (y2 - 1968) div 4 -
                (y2 - 1900) div 100 + (y2 - 1600) div 400)));
  GetMicroSecondTime := 1000000.0 * Seconds + 10000 * LongInt (Sec100)
end;

function GetCPUTime (var MicroSecond { : LongInt }) : LongInt;
var
  h, m, s, s100 : Word;
  Seconds, MicroSeconds : LongInt;
begin
  GetTime (h, m, s, s100);
  Seconds := (LongInt (h) * 60 + m) * 60 + s - StartCPUTimeSeconds;
  MicroSeconds := 10000 * LongInt (s100) - StartCPUTimeMicroSeconds;
  if MicroSeconds < 0 then
    begin
      Dec (Seconds);
      Inc (MicroSeconds, 1000000)
    end;
  if @MicroSecond <> nil then LongInt (MicroSecond) := MicroSeconds;
  GetCPUTime := Seconds
end;

function AddDirSeparator (s : String) : TString;
begin
  if (s <> '') and not (s [Length (s)] in DirSeparators)
    and DirectoryExists (s) then s := s + DirSeparator;
  AddDirSeparator := s
end;

function RemoveDirSeparator (s : String) : TString;
begin
  if (s <> '') and (s [Length (s)] in DirSeparators) then
    Delete (s, Length (s), 1);
  RemoveDirSeparator := s
end;

function GetCurrentDirectory : TString;
begin
  GetCurrentDirectory := FExpand (DirSelf)
end;

function GetTempDirectory : TString;
const TempDirectory : String = '';

  function CheckDir (const s : String) : Boolean;
  begin
    TempDirectory := s;
    if TempDirectory = '' then
      begin
        CheckDir := False;
        Exit
      end;
    if TempDirectory [Length (TempDirectory)] <> DirSeparator then
      TempDirectory := TempDirectory + DirSeparator;
    CheckDir := DirectoryExists (TempDirectory)
  end;

begin
  if not (
           CheckDir (TempDirectory) or
           CheckDir (GetEnv ('TEMP')) or
           CheckDir (GetEnv ('TMP')) or
           CheckDir (GetEnv ('TEMPDIR')) or
           CheckDir (GetEnv ('TMPDIR')) or
           CheckDir ('c:\temp\') or
           CheckDir ('c:\tmp\') or
           CheckDir ('\temp\') or
           CheckDir ('\tmp\') or
           CheckDir ('.\')
         ) then TempDirectory := '';
  GetTempDirectory := TempDirectory
end;

function ExpandEnvironment (var s : String) : Boolean;
var
  p, q : Integer;
  EnvName, Env : TString;
begin
  p := 0;
  repeat
    repeat
      Inc (p);
      if p > Length (s) then
        begin
          ExpandEnvironment := True;
          Exit
        end
    until (s [p] = '$') or ((p = 1) and (s [p] = '~'));
    q := p + 1;
    if s [p] = '~' then
      EnvName := 'HOME'
    else
      begin
        if (q <= Length (s)) and (s [q] in EnvVarCharsFirst) then
          while (q <= Length (s)) and (s [q] in EnvVarChars) do Inc (q);
        EnvName := Copy (s, p + 1, q - p - 1)
      end;
    Delete (s, p, q - p);
    Env := GetEnv (EnvName);
    if Env = '' then
      begin
        s := EnvName;
        ExpandEnvironment := False;
        Exit
      end;
    Insert (Env, s, p)
  until False
end;

function FileExists (const aFileName : String) : Boolean;
var
  f : File;
  Attr : TDosAttr;
begin
  Assign (f, aFileName);
  GetFAttr (f, Attr);
  FileExists := (DosError = 0) and (Attr and Directory = 0)
end;

function DirectoryExists (aFileName : String) : Boolean;
var
  f : File;
  Attr : TDosAttr;
begin
  if (Length (aFileName) > 3) and (aFileName [Length (aFileName)] = DirSeparator) then
    Delete (aFileName, Length (aFileName), 1);
  Assign (f, aFileName);
  GetFAttr (f, Attr);
  DirectoryExists := (DosError = 0) and (Attr and Directory <> 0)
end;

function PathExists (const aFileName : String) : Boolean;
var
  f : File;
  Attr : TDosAttr;
begin
  Assign (f, aFileName);
  GetFAttr (f, Attr);
  PathExists := DosError = 0
end;

function RelativePath (const Path : String; OnlyCurDir, Quoted : Boolean) : TString;
var Res, p : TString;
begin
  Res := FExpand (Path);
  p := AddDirSeparator (FExpand (DirSelf));
  if (Length (Res) >= Length (p)) and
    (Copy (Res, 1, Length (p)) = p) and
    (not OnlyCurDir or (LastCharPos (DirSeparators, Res) <= Length (p))) then
    Delete (Res, 1, Length (p));
  RelativePath := Res
end;

function DirFromPath (const Path : String) : TString;
var Dir, Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  if Dir = '' then Dir := DirSelf + DirSeparator;
  DirFromPath := Dir
end;

function NameFromPath (const Path : String) : TString;
var Dir, Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  NameFromPath := Name
end;

function ExtFromPath (const Path : String) : TString;
var Dir, Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  ExtFromPath := Ext
end;

function NameExtFromPath (const Path : String) : TString;
var Dir, Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  NameExtFromPath := Name + Ext
end;

function OpenDir (Name : String) : DirPtr;
var Dir : DirPtr;
begin
  New (Dir);
  FindFirst (AddDirSeparator (Name) + '*.*', Dos.AnyFile - VolumeID, Dir^);
  if DosError <> 0 then
    begin
      Dispose (Dir);
      InOutRes := 100;
      Dir := nil
    end;
  OpenDir := Dir
end;

function ReadDir (Dir : DirPtr) : TString;
begin
  ReadDir := LoCaseStr (Dir^.Name);
  if Dir^.Name <> '' then
    begin
      FindNext (Dir^);
      if DosError <> 0 then Dir^.Name := ''
    end
end;

procedure CloseDir (Dir : DirPtr);
begin
  if Dir <> nil then Dispose (Dir)
end;

function HasWildCards (const s : String) : Boolean;
begin
  HasWildCards := CharPos (WildCardChars, s) <> 0
end;

function FileNameMatch (const Pattern, Name : String) : Boolean;
var
  zp, zn, z2 : Integer;
  ch : Char;
  s : set of Char;
  Negate : Boolean;
  Pattern2 : TString;
begin
  FileNameMatch := False;
  zn := 1;
  zp := 1;
  while zp <= Length (Pattern) do
    begin
      ch := Pattern [zp];
      Inc (zp);
      if ch = '*' then
        begin
          while (zp <= Length (Pattern)) and (Pattern [zp] = '*') do Inc (zp);
          Pattern2 := Copy (Pattern, zp, Length (Pattern) - zp + 1);
          for z2 := Length (Name) + 1 downto zn do
            if FileNameMatch (Pattern2, Copy (Name, z2, Length (Name) - z2 + 1)) then
              begin
                FileNameMatch := True;
                Exit
              end;
          Exit
        end;
      if zn > Length (Name) then Exit;
      if ch = '['
      then
        begin
          Negate := (zp <= Length (Pattern)) and (Pattern [zp] = '^');
          if Negate then Inc (zp);
          s := [];
          while (zp <= Length (Pattern)) and (Pattern [zp] <> ']') do
            begin
              ch := Pattern [zp];
              Inc (zp);
              if Pattern [zp] = '-'
                then
                  begin
                    s := s + [ch .. Pattern [zp + 1]];
                    Inc (zp, 2)
                  end
                else s := s + [ch]
            end;
          Inc (zp);
          if not (Name [zn] in s) xor Negate then Exit
        end
      else
        if (Name [zn] <> ch) and (ch <> '?') then Exit;
      Inc (zn)
    end;
  FileNameMatch := zn > Length (Name)
end;

procedure Glob (var Buf : GlobBuffer; Pattern : String);
begin
  Buf.Count := 0
end;

procedure GlobFree (var Buf : GlobBuffer);
begin
end;

function ConfigFileName (Prefix, Name : String; Global : Boolean) : TString;
var Dir, PName, Ext : TString;
begin
  FSplit (LoCaseStr (ParamStr (0)), Dir, PName, Ext);
  if Name <> '' then PName := Name;
  if Global
    then ConfigFileName := Dir + PName + '.ini'
    else ConfigFileName := Dir + PName + '.cfg'
end;

function DataDirectoryName (Prefix, Name : String) : TString;
begin
  DataDirectoryName := DirFromPath (LoCaseStr (ParamStr (0)))
end;

procedure GetTimeStamp (var aTimeStamp : TimeStamp);
var y, m, d, dow, h, mi, s, s100 : Word;
begin
  GetDate (y, m, d, dow);
  GetTime (h, mi, s, s100);
  with aTimeStamp do
    begin
      DateValid   := True;
      TimeValid   := True;
      Year        := y;
      Month       := m;
      Day         := d;
      DayOfWeek   := dow;
      Hour        := h;
      Minute      := mi;
      Second      := s;
      MicroSecond := 10000 * s100
    end
end;

procedure CheckInOutRes;
var Temp : Integer;
begin
  Temp := IOResult;
  if Temp <> 0 then RunError (Temp)
end;

const
  E_CloseIgnore = 103;

function GetIOErrorMessage : CString;
const
  ErrMsgs : array [1 .. 35] of record
    Code : Integer;
    MsgStr : CString
  end =
    ((Code :   1; MsgStr : 'Invalid Dos function code'),
     (Code :   4; MsgStr : 'Too many open files'),
     (Code :   6; MsgStr : 'Invalid file handle'),
     (Code :   8; MsgStr : 'Not enough memory'),
     (Code :  12; MsgStr : 'Invalid file access code'),
     (Code :  15; MsgStr : 'Invalid drive number'),
     (Code :  16; MsgStr : 'Cannot remove current directory'),
     (Code :  17; MsgStr : 'Cannot rename across drives'),
     (Code : 100; MsgStr : 'Disk read error'),
     (Code : 101; MsgStr : 'Disk write error'),
     (Code : 102; MsgStr : 'File not assigned'),
     (Code : 103; MsgStr : 'File not open'),
     (Code : 104; MsgStr : 'File not open for input'),
     (Code : 105; MsgStr : 'File not open for output'),
     (Code : 106; MsgStr : 'Invalid numeric format'),
     (Code : 150; MsgStr : 'Disk is write-protected'),
     (Code : 152; MsgStr : 'Drive not ready'),
     (Code : 154; MsgStr : 'CRC error in data'),
     (Code : 156; MsgStr : 'Disk seek error'),
     (Code : 158; MsgStr : 'Sector not found'),
     (Code : 159; MsgStr : 'Printer out of paper'),
     (Code : 160; MsgStr : 'Device write fault'),
     (Code : 161; MsgStr : 'Device read fault'),
     (Code : 162; MsgStr : 'Hardware failure'),
     (Code : 200; MsgStr : 'Division by zero'),
     (Code : 201; MsgStr : 'Range check error'),
     (Code : 202; MsgStr : 'Stack overflow error'),
     (Code : 203; MsgStr : 'Out of memory'),
     (Code : 204; MsgStr : 'Invalid pointer operation'),
     (Code : 205; MsgStr : 'Floating point overflow'),
     (Code : 206; MsgStr : 'Floating point underflow'),
     (Code : 207; MsgStr : 'Invalid floating point operation'),
     (Code : 208; MsgStr : 'Overlay manager not installed'),
     (Code : 209; MsgStr : 'Overlay file read error'),
     (Code : 210; MsgStr : 'Object not initialized'));
var i, n : Integer;
begin
  n := IOResult;
  i := Low (ErrMsgs);
  while (i <= High (ErrMsgs)) and (ErrMsgs [i].Code <> n) do Inc (i);
  if i <= High (ErrMsgs)
    then GetIOErrorMessage := ErrMsgs [i].MsgStr
    else GetIOErrorMessage := 'Unknown error code'
end;

procedure Close (var f : Text);
var Res : Integer;
begin
  Res := IOResult;
  System.Close (f);
  if (Res <> 0) or (InOutRes = E_CloseIgnore) then InOutRes := Res
end;

procedure CloseFile (var f : File);
var Res : Integer;
begin
  Res := IOResult;
  System.Close (f);
  if (Res <> 0) or (InOutRes = E_CloseIgnore) then InOutRes := Res
end;

{ TFDD (interface may be subject to change) }

type
  TTFDDUserData = record
    fReadFunc    : TReadFunc;
    fWriteFunc   : TWriteFunc;
    fFlushProc   : TFlushProc;
    fPrivateData : Pointer;
  end;

  { UserData does not have enough room for all our pointers, so we store
    some of them in the Name field. This is a very dirty hack, but well...
    it's only BP ;-}
  TTFDDUserDataName = record
    Unused       : Pointer; { Don't use the first byte, we store a #0 there }
    fOpenProc    : TOpenProc;
    fCloseProc   : TCloseProc;
    Fill         : array [12 .. 79] of Char
  end;

function TFDDInput (var f : TextRec) : Integer;
begin
  f.BufPos := 0;
  with TTFDDUserData (f.UserData) do
    if @fReadFunc = nil
      then f.BufEnd := 0
      else f.BufEnd := fReadFunc (fPrivateData^, f.BufPtr^, f.BufSize);
  TFDDInput := IOResult
end;

function TFDDOutput (var f : TextRec) : Integer;
var
  p : PChar;
  Size : SizeType;
  Res : Integer;
begin
  p := PChar (f.BufPtr);
  Size := f.BufPos;
  while (InOutRes = 0) and (Size > 0) do
    begin
      with TTFDDUserData (f.UserData) do
        if @fWriteFunc = nil
          then Res := 0
          else Res := fWriteFunc (fPrivateData^, p^, Size);
      Dec (Size, Res);
      Inc (p, Res);
      if (Res = 0) and (InOutRes = 0) then InOutRes := 100 { Disk read error }
    end;
  f.BufPos := 0;
  TFDDOutput := IOResult
end;

function TFDDFlush (var f : TextRec) : Integer;
begin
  InOutRes := TFDDOutput (f);
  if InOutres = 0 then
    with TTFDDUserData (f.UserData) do
      if @fFlushProc <> nil then fFlushProc (fPrivateData^);
  TFDDFlush := IOResult
end;

function TFDDDummy (var f : TextRec) : Integer;
begin
  TFDDDummy := IOResult
end;

function TFDDClose (var f : TextRec) : Integer;
begin
  with TTFDDUserData (f.UserData), TTFDDUserDataName (f.Name) do
    if @fCloseProc <> nil then fCloseProc (fPrivateData^);
  TFDDClose := IOResult
end;

function TFDDOpen (var f : TextRec) : Integer;
var Mode : TOpenMode;
begin
  if f.Mode = fmInput then
    begin
      f.InOutFunc := @TFDDInput;
      f.FlushFunc := @TFDDDummy;
      Mode := foReset
    end
  else
    begin
      f.InOutFunc := @TFDDOutput;
      f.FlushFunc := @TFDDFlush;
      if f.Mode = fmOutput
        then Mode := foRewrite
        else Mode := foAppend;
      f.Mode := fmOutput
    end;
  f.CloseFunc := @TFDDClose;
  with TTFDDUserData (f.UserData), TTFDDUserDataName (f.Name) do
    if @fOpenProc <> nil then fOpenProc (fPrivateData^, Mode);
  TFDDOpen := IOResult
end;

procedure AssignTFDD (var f : AnyFile;
                      OpenProc    : TOpenProc;
                      SelectFunc  : TSelectFunc; { will not be called in BP! }
                      SelectProc  : TSelectProc; { will not be called in BP! }
                      ReadFunc    : TReadFunc;
                      WriteFunc   : TWriteFunc;
                      FlushProc   : TFlushProc;
                      CloseProc   : TCloseProc;
                      DoneProc    : TDoneProc;   { will not be called in BP! }
                      PrivateData : Pointer);
begin
  with TextRec (f), TTFDDUserData (UserData), TTFDDUserDataName (Name) do
    begin
      Handle := $ffff;
      Mode := fmClosed;
      BufSize := SizeOf (Buffer);
      BufPtr := @Buffer;
      OpenFunc := @TFDDOpen;
      fOpenProc    := OpenProc;
      fReadFunc    := ReadFunc;
      fWriteFunc   := WriteFunc;
      fFlushProc   := FlushProc;
      fCloseProc   := CloseProc;
      fPrivateData := PrivateData;
      Name [0] := #0
    end
end;

{ Strings extensions }

function CString2String (Str : CString) : TString;
begin
  CString2String := StrPas (Str)
end;

{ Printer extensions }

procedure AssignLst (var f : Text);
begin
  Assign (f, 'LPT1');
  Rewrite (f);
  asm
     mov  bx, f.TextRec.Handle
     mov  ax, $4400
     int  $21
     or   dl, $20
     mov  dh, 0
     mov  ax, $4401
     int  $21
  end
end;

{ GetOpt -- very primitive }

function GetOpt (OptString : CString) : Char;
var
  OptStr, s : String;
  p, o : Integer;
begin
  if FirstNonOption = 0 then FirstNonOption := 1;
  GetOpt := EndOfOptions;
  OptionArgument := '';
  HasOptionArgument := False;
  if FirstNonOption > ParamCount then Exit;
  OptStr := CString2String (OptString);
  s := ParamStr (FirstNonOption);
  if (Length (s) < 2) or (s [1] <> '-') then
    begin
      if OptStr [1] = '-' then
        begin
          GetOpt := NoOption;
          OptionArgument := s;
          HasOptionArgument := True;
          Inc (FirstNonOption)
        end;
      Exit
    end;
  Inc (FirstNonOption);
  p := Pos (s [2], OptStr);
  if p = 0 then
    begin
      GetOpt := UnknownOption;
      UnknownOptionCharacter := s [2];
      if GetOptErrorFlag then Writeln (StdErr, ParamStr (0), ': unrecognized option -- ', s [2]);
      Exit
    end;
  GetOpt := s [2];
  Inc (p);
  o := p;
  while (o <= Length (OptStr)) and (OptStr [o] = ':') do Inc (o);
  Dec (o, p);
  if o = 0 then
    begin
      if (Length (s) > 2) and GetOptErrorFlag then
        Writeln (StdErr, ParamStr (0), ': ignoring `', Copy (s, 3, Length (s) - 2), ''' after `-', s [2], '''');
      Exit
    end;
  if Length (s) > 2 then
    begin
      OptionArgument := Copy (s, 2, Length (s) - 2);
      HasOptionArgument := True
    end
  else
    if o = 1 then
      begin
        OptionArgument := ParamStr (FirstNonOption);
        HasOptionArgument := FirstNonOption <= ParamCount;
        if FirstNonOption <= ParamCount then Inc (FirstNonOption)
      end
end;

{ Dos extensions }

procedure FindFirst (const Path : String; Attr : Word; var SR : SearchRec);
var i : Integer;
begin
  Dos.FindFirst (Path, Attr, SR);
  for i := Low (SR.Name) to High (SR.Name) do SR.Name [i] := LoCase (SR.Name [i])
end;

procedure FindNext  (var SR : SearchRec);
var i : Integer;
begin
  Dos.FindNext (SR);
  for i := Low (SR.Name) to High (SR.Name) do SR.Name [i] := LoCase (SR.Name [i])
end;

procedure FindClose (var SR : SearchRec);
begin
end;

function FExpand (const Path : String) : TString;
var d, n, e, s : TString;
begin
  FSplit (Path, d, n, e);
  if not DirectoryExists (d) then
    FExpand := ''
  else
    begin
      s := LoCaseStr (Dos.FExpand (Path));
      if s [Length (s)] = ExtSeparator then Delete (s, Length (s), 1);
      FExpand := s
    end
end;

{$P+}
procedure FSplit (const Path : String; var Dir, Name, Ext : String);
var
  aDir  : DirStr;
  aName : NameStr;
  aExt  : ExtStr;
begin
  Dos.FSplit (Path, aDir, aName, aExt);
  Dir  := aDir;
  Name := aName;
  Ext  := aExt
end;
{$P-}

function GetEnv (const EnvVar : String) : TString;
begin
  GetEnv := Dos.GetEnv (EnvVar)
end;

procedure SwapVectors;
begin
end;

{$ifdef MSDOS}
procedure SetMemTop (MemTop : Pointer); Assembler;
asm
   les  bx, MemTop
   mov  ax, es
   add  bx, 15
   mov  cl, 4
   shr  bx, cl
   add  bx, ax
   mov  ax, PrefixSeg
   sub  bx, ax
   mov  es, ax
   mov  ah, $4a
   int  $21
end;
{$endif}

procedure Exec (const Path, ComLine : String);
begin
  RestoreTerminal (True);
  {$ifdef MSDOS} SetMemTop (HeapPtr); {$endif}
  Dos.SwapVectors;
  Dos.Exec (Path, ComLine);
  Dos.SwapVectors;
  {$ifdef MSDOS} SetMemTop (HeapEnd); {$endif}
  RestoreTerminal (False)
end;

function Execute (CmdLine : String) : Integer;
begin
  Exec (GetEnv (ShellEnvVar), ShellExecCommand + ' ' + CmdLine);
  InOutRes := DosError;
  Execute := DosExitCode
end;

type
  PRestoreTerminalProcs = ^TRestoreTerminalProcs;
  TRestoreTerminalProcs = record
    Next, Prev : PRestoreTerminalProcs;
    Proc : TProcedure
  end;

const
  RestoreTerminalProcs : array [Boolean] of PRestoreTerminalProcs = (nil, nil);

procedure RegisterRestoreTerminal (ForAnotherProcess : Boolean; Proc : TProcedure);
var p : PRestoreTerminalProcs;
begin
  New (p);
  p^.Proc := Proc;
  p^.Prev := nil;
  p^.Next := RestoreTerminalProcs [ForAnotherProcess];
  if p^.Next <> nil then p^.Next^.Prev := p;
  RestoreTerminalProcs [ForAnotherProcess] := p
end;

function UnregisterRestoreTerminal (ForAnotherProcess : Boolean; Proc : TProcedure) : Boolean;
var p : PRestoreTerminalProcs;
begin
  p := RestoreTerminalProcs [ForAnotherProcess];
  while (p <> nil) and (@p^.Proc <> @Proc) do p := p^.Next;
  if p = nil then
    UnregisterRestoreTerminal := False
  else
    begin
      if p^.Next <> nil then p^.Next^.Prev := p^.Prev;
      if p^.Prev = nil
        then RestoreTerminalProcs [ForAnotherProcess] := p^.Next
        else p^.Prev^.Next := p^.Next;
      Dispose (p);
      UnregisterRestoreTerminal := True
    end
end;

procedure RestoreTerminal (ForAnotherProcess : Boolean);
var p : PRestoreTerminalProcs;
begin
  p := RestoreTerminalProcs [ForAnotherProcess];
  if ForAnotherProcess then
    while p <> nil do
      begin
        p^.Proc;
        p := p^.Next
      end
  else if p <> nil then
    begin
      while p^.Next <> nil do p := p^.Next;
      while p <> nil do
        begin
          p^.Proc;
          p := p^.Prev
        end
    end
end;

{ CRT extensions }

const
  MonoModes = [BW40, BW80, Mono];
  MaxScreenSize = MaxInt;
  CurrentCursorShape : TCursorShape = CursorNormal;

type
  TScreenChar = record
    ScrCh : Char;
    Attrib : TTextAttr
  end;

  TScreenBuffer = array [0 .. MaxScreenSize - 1] of TScreenChar;

var
  CRTOutFile : Text;
  OrigCursor : Word;
  ScreenBuffer : ^TScreenBuffer;
  KBReadFunc : Byte;

function GetShiftState : Integer;
var State : Integer;
begin
  State := Mem [Seg0040 : $17] and (shShift or shCtrl or shAlt);
  GetShiftState := VirtualShiftState or State or ((State and shAlt) * (shAltGr div shAlt))
end;

procedure SetCRTUpdate (Update : TCRTUpdate);
begin
end;

procedure CRTUpdate;
begin
end;

function ReadKeyWord : TKey;
var k : TKey;
begin
  asm
     mov ah, KBReadFunc
     int $16
     mov k, ax
  end;
  if (Lo (k) = $e0) and (Hi (k) >= 70) then k := k and $ff00;
  if Lo (k) <> 0 then k := k and $ff;
  case k of
    kbIns : if GetShiftState and shShift <> 0 then k := kbShIns;
    kbDel : if GetShiftState and shShift <> 0 then k := kbShDel;
  end;
  ReadKeyWord := k
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

procedure GetWindow (var x1, y1, x2, y2 : Integer);
begin
  if @x1 <> nil then x1 := WindowMin.X + 1;
  if @y1 <> nil then y1 := WindowMin.Y + 1;
  if @x2 <> nil then x2 := WindowMax.X + 1;
  if @y2 <> nil then y2 := WindowMax.Y + 1
end;

function GetXMax : Integer;
var x1, x2 : Integer;
begin
  GetWindow (x1, null, x2, null);
  GetXMax := x2 - x1 + 1
end;

function GetYMax : Integer;
var y1, y2 : Integer;
begin
  GetWindow (null, y1, null, y2);
  GetYMax := y2 - y1 + 1
end;

function WhereXAbs : Integer;
var x1 : Integer;
begin
  GetWindow (x1, null, null, null);
  WhereXAbs := WhereX + x1 - 1
end;

function WhereYAbs : Integer;
var y1 : Integer;
begin
  GetWindow (null, y1, null, null);
  WhereYAbs := WhereY + y1 - 1
end;

procedure GotoXYAbs (X, Y : Integer);
var x1, y1 : Integer;
begin
  GetWindow (x1, y1, null, null);
  GotoXY (X - x1 + 1, Y - y1 + 1)
end;

procedure SetCursorShape (Shape : TCursorShape);

  procedure SetCursor (Cursor : Word); Assembler;
  asm
    mov ah, 1
    mov cx, Cursor
    int $10
  end;

begin
  CurrentCursorShape := Shape;
  case Shape of
    CursorIgnored,
    CursorHidden : SetCursor ($2000);
    CursorNormal : SetCursor (OrigCursor);
    CursorFat    : SetCursor (OrigCursor and $ff + (OrigCursor div 2) and $ff00);
    CursorBlock  : SetCursor (OrigCursor and $ff);
  end
end;

function GetCursorShape : TCursorShape;
begin
  GetCursorShape := CurrentCursorShape
end;

procedure HideCursor;
begin
  SetCursorShape (CursorHidden)
end;

procedure HiddenCursor;
begin
  SetCursorShape (CursorHidden)
end;

procedure NormalCursor;
begin
  SetCursorShape (CursorNormal)
end;

procedure FatCursor;
begin
  SetCursorShape (CursorFat)
end;

procedure BlockCursor;
begin
  SetCursorShape (CursorBlock)
end;

procedure IgnoreCursor;
begin
  SetCursorShape (CursorIgnored)
end;

function GetTextColor : Integer;
begin
  GetTextColor := TextAttr and $8F
end;

function GetTextBackground : Integer;
begin
  GetTextBackground := (TextAttr and $70) shr 4
end;

procedure Flash;
var i, j : Integer;
begin
  for i := 1 to 2 do
    begin
      for j := 0 to ScreenSize.X * ScreenSize.Y - 1 do
        ScreenBuffer^[j].Attrib := ScreenBuffer^[j].Attrib xor $7f;
      Delay (10)
    end
end;

procedure Beep;
begin
  if VisualBell then
    Flash
  else
    begin
      Sound (2800);
      Delay (120);
      NoSound
    end
end;

procedure WriteChar (Ch : Char);
begin
  Write (CRTOutFile, Ch)
end;

procedure WriteString (const s : String; y, x : Integer);
var OrigX, OrigY : Integer;
begin
  OrigX := WhereX;
  OrigY := WhereY;
  GotoXY (x, y);
  Write (CRTOutFile, s);
  GotoXY (OrigX, OrigY)
end;

procedure FastWriteWindow (const s : String; y, x : Integer; Attr : TTextAttr);
begin
  WriteStrAt (x, y, s, Attr)
end;

procedure FastWrite (const s : String; y, x : Integer; Attr : TTextAttr);
begin
  WriteStrAt (x - WindowMin.X, y - WindowMin.Y, s, Attr)
end;

procedure ReadChar (x, y : Integer; var Ch : Char; var Attr : TTextAttr);
begin
  with ScreenBuffer^[(y + WindowMin.Y - 1) * Mem [Seg0040 : $4a] + x + WindowMin.X - 1] do
    begin
      Ch := ScrCh;
      Attr := Attrib
    end
end;

procedure TSync; near; Assembler; { Registers: DX: Port }
asm
@1:in   al, dx
   test al, 1
   jne  @1
   cli
@2:in   al, dx
   test al, 1
   je   @2
end;

procedure CRTExtBlock; near; Assembler; { Registers: CL: Buffer length; DX: Coordinates; SI: Procedure }
asm
   mov   es, Seg0040
   mov   al, dh
   mul   byte ptr es:[$4a]
   xor   dh, dh
   add   ax, dx
   shl   ax, 1
   mov   di, ax
   mov   es, Word ptr ScreenBuffer+2
   mov   dx, $3da
   cld
   cmp   CheckSnow, 1
   jmp   si
end;

procedure CRTExtScrOp; near; Assembler; { Registers: CL: Buffer length; SI: Write procedure; BP: Stack frame }
asm
   mov   dl, [bp+14]
   dec   dl
   add   dl, WindowMin.X
   jc    @1
   cmp   dl, WindowMax.X
   ja    @1
   mov   dh, [bp+12]
   dec   dh
   add   dh, WindowMin.Y
   jc    @1
   cmp   dh, WindowMax.Y
   ja    @1
   xor   ch, ch
   jcxz  @1
   mov   al, WindowMax.X
   sub   al, dl
   inc   al
   cmp   cl, al
   JB    @2
   mov   cl, al
@2:jmp   CRTExtBlock
@1:
end;

procedure CRTExtDispatch; near; Assembler; { Registers }
asm
   xor   cx, cx
   mov   dx, WindMin
   mov   cl, WindowMax.X
   sub   cl, dl
   inc   cx
@1:push  cx
   push  dx
   push  si
   call  CRTExtBlock
   pop   si
   pop   dx
   pop   cx
   inc   dh
   cmp   dh, WindowMax.Y
   jbe   @1
   mov   sp, bp
   pop   bp
   retf  4 { !!! }
end;

procedure CRTExtWriteStr; near; Assembler; { Registers }
asm
   push  ds
   mov   ah, [bp+6]
   lds   si, [bp+8]
   inc   si
   jc    @2
@1:lodsb
   mov   bx, ax
   call  TSync
   mov   ax, bx
   stosw
   sti
   loop  @1
   jmp   @3
@2:lodsb
   stosw
   loop  @2
@3:pop   ds
end;

procedure CRTExtWriteChar; near; Assembler; { Registers }
asm
   mov   al, [bp+8]
   mov   ah, [bp+6]
   jc    @2
   mov   bx, ax
@1:call  TSync
   mov   ax, bx
   stosw
   sti
   loop  @1
   jmp   @3
@2:rep   stosw
@3:
end;

procedure CRTExtWriteCharAttr; near; Assembler; { Registers }
asm
   push  ds
   lds   si, [bp+6]
   jc    @2
@1:lodsw
   mov   bx, ax
   call  TSync
   mov   ax, bx
   stosw
   sti
   lodsb
   loop  @1
   jmp   @3
@2:movsw
   lodsb
   loop  @2
@3:pop   ds
end;

procedure WriteStrAt (x, y : Integer; s : String; Attr : TTextAttr); Assembler;
asm
   les   bx, s
   mov   cl, es:[bx]
   mov   si, Offset @1
   call  CRTExtScrOp
   jmp   @2
@1:jmp   CRTExtWriteStr
@2:
end;

procedure WriteCharAt (x, y, Count : Integer; Ch : Char; Attr : TTextAttr); Assembler;
asm
   mov   cx, Count
   mov   si, Offset @1
   call  CRTExtScrOp
   jmp   @2
@1:jmp   CRTExtWriteChar
@2:
end;

procedure WriteCharAttrAt (x, y, Count : Integer; CharAttr : PCharAttrs); Assembler;
asm
   mov   cx, Count
   mov   si, Offset @1
   call  CRTExtScrOp
   jmp   @2
@1:jmp   CRTExtWriteCharAttr
@2:
end;

procedure ChangeTextAttr1 (x, y, Count : Integer; NewAttr, Dummy : TTextAttr); Assembler;
asm
   mov   si, Offset @1
   mov   cx, Count
   call  CRTExtScrOp
   jmp   @5
@1:push  ds
   push  es
   pop   ds
   mov   bl, NewAttr
   mov   si, di
   jc    @3
@2:call  TSync
   lodsw
   mov   ah, bl
   stosw
   sti
   loop  @2
   jmp   @4
@3:lodsw
   mov   ah, bl
   stosw
   loop  @3
@4:pop   ds
   retn
@5:
end;

procedure ChangeTextAttr (x, y, Count : Integer; NewAttr : TTextAttr);
begin
  ChangeTextAttr1 (x, y, Count, NewAttr, 0)
end;

procedure FillWin (Ch : Char; Attr : TTextAttr); Assembler;
asm
   mov   si, Offset @1
   jmp   CRTExtDispatch
@1:jmp   CRTExtWriteChar
end;

function WinSize : SizeType;
begin
  WinSize := 2 * GetYMax * GetXMax
end;

procedure CRTExtCopyWin; near; Assembler; { Registers }
asm
   jc    @2
@1:lodsw
   mov   bx, ax
   call  TSync
   mov   ax, bx
   stosw
   sti
   loop  @1
   jmp   @3
@2:rep   movsw
@3:
end;

procedure ReadWin (var Buf); Assembler;
asm
   mov   si, Offset @1
   jmp   CRTExtDispatch
@1:push  ds
   push  es
   pop   ds
   mov   si, di
   les   di, Buf
   call  CRTExtCopyWin
   mov   Word ptr Buf, di
   pop   ds
   retn
end;

procedure WriteWin (const Buf); Assembler;
asm
   mov   si, Offset @1
   jmp   CRTExtDispatch
@1:push  ds
   lds   si, Buf
   call  CRTExtCopyWin
   mov   Word ptr Buf, si
   pop   ds
   retn
end;

procedure SaveWin (var State : WinState);
begin
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
  State.BufSize     := 0;
  State.Buffer      := nil
end;

procedure MakeWin (var State : WinState; x1, y1, x2, y2 : Integer);
begin
  SaveWin (State);
  Window (x1, y1, x2, y2);
  with State do GetWindow (NewX1, NewY1, NewX2, NewY2);
  State.BufSize := WinSize;
  GetMem (State.Buffer, State.BufSize);
  ReadWin (State.Buffer^)
end;

procedure SaveScreen (var State : WinState);
begin
  MakeWin (State, 1, 1, ScreenSize.X, ScreenSize.Y);
  State.TextMode := LastMode
end;

procedure RestoreWin (var State : WinState);
begin
  if State.TextMode <> - 1 then
    begin
      if State.TextMode <> LastMode then TextMode (State.TextMode);
      Window (1, 1, ScreenSize.X, ScreenSize.Y)
    end;
  if State.Buffer <> nil then
    begin
      with State do Window (NewX1, NewY1, NewX2, NewY2);
      WriteWin (State.Buffer^);
      FreeMem (State.Buffer, State.BufSize);
      State.BufSize := 0;
      State.Buffer := nil
    end;
  with State do Window (x1, y1, x2, y2);
  GotoXY (State.WhereX, State.WhereY);
  TextAttr := State.TextAttr;
  SetCursorShape (State.CursorShape)
end;

function GetScreenSeg : Word; Assembler;
asm
   push es
   mov  es, Seg0040
   mov  ax, SegB800
   cmp  byte ptr es:[$49], 7
   jne  @1
   mov  ax, SegB000
@1:pop  es
end;

procedure CRTExtInit;
begin
  ScreenBuffer := Ptr (GetScreenSeg, 0);
  if Seg (ScreenBuffer^) = SegB000
    then OrigCursor := $c0d
    else OrigCursor := $d0e;
  ScreenSize.X := WindowMax.X + 1;
  ScreenSize.Y := WindowMax.Y + 1;
  IsMonoMode := (LastMode and $ff) in MonoModes;
  NormalCursor
end;

const
  ReadKeyScan : Char = #0;

procedure CRTInit;
begin
end;

function KeyPressed : Boolean;
begin
  KeyPressed := (ReadKeyScan <> #0) or Crt.KeyPressed
end;

function ReadKey: Char;
var
  k : TKey;
  ch : Char;
begin
  if ReadKeyScan <> #0 then
    begin
      ReadKey := ReadKeyScan;
      ReadKeyScan := #0
    end
  else
    begin
      k := ReadKeyWord;
      ch := Key2Char (k);
      if ch = #0 then ReadKeyScan := Key2Scan (k);
      ReadKey := ch
    end
end;

procedure TextMode (Mode : Integer);
begin
  CRT.TextMode (Mode);
  CRTExtInit
end;

type
  PBigMemList = ^TBigMemList;
  TBigMemList = record
    Next : PBigMemList;
    p : PBigMem
  end;

const
  BigMemList : PBigMemList = nil;

{$ifdef MSDOS}
type
  TXMSData = record
    Length  : LongInt;
    SHandle : Word;
    SOffset : LongInt;
    DHandle : Word;
    DOffset : Longint
  end;

var
  XMSAdr, EMSPtr : Pointer;

procedure XMSError (ErrorCode : Byte);
begin
  RestoreTerminal (True);
  Writeln (StdErr, 'XMS error #', ErrorCode);
  Halt (125)
end;

function XMSInstalled : Boolean; Assembler;
const Installed : Integer = - 1;
asm
   mov ax, Installed
   or  ax, ax
   jge @2
   mov ax, $4300
   int $2f
   cmp al, $80
   mov al, False
   jne @1
   mov ax, $4310
   int $2f
   mov Word ptr XMSAdr, bx
   mov Word ptr XMSAdr + 2, es
   mov al, True
@1:mov Installed, ax
@2:
end;

function XMSMaxAvail : Word; Assembler;
asm
   mov  ah, 8
   call [XMSAdr]
   or   ax, ax
   jne  @1
   cmp  bl, $a0
   je   @1
   push bx
   call XMSError
   xor  ax, ax
@1:
end;

function XMSGetMem (Size : Word) : Word; Assembler;
asm
   mov  ah, 9
   mov  dx, Size
   call [XMSAdr]
   or   ax, ax
   jne  @1
   push bx
   call XMSError
   xor  dx, dx
@1:mov  ax, dx
end;

procedure XMSFreeMem (Handle : Word); Assembler;
asm
   mov  ah, $a
   mov  dx, Handle
   call [XMSAdr]
   or   ax, ax
   jne  @1
   push bx
   call XMSError
@1:
end;

procedure XMSCopy (var XMSData : TXMSData); Assembler;
asm
   push ds
   pop  es
   push ds
   mov  ah, $b
   lds  si, XMSData
   call [es:XMSAdr]
   pop  ds
   or   ax, ax
   jne  @1
   push bx
   call XMSError
@1:
end;

procedure MoveToXMS (var Source; Handle : Word; BlockLength, Offset : LongInt);
var XMSData : TXMSData;
begin
  with XMSData do
    begin
      Length := BlockLength;
      SHandle := 0;
      Pointer (SOffset) := @Source;
      DHandle := Handle;
      DOffset := Offset
    end;
  XMSCopy (XMSData)
end;

procedure MoveFromXMS (Handle : Word; var Dest; BlockLength, Offset : LongInt);
var XMSData : TXMSData;
begin
  with XMSData do
    begin
      Length := BlockLength;
      SHandle := Handle;
      SOffset := Offset;
      DHandle := 0;
      Pointer (DOffset) := @Dest
    end;
  XMSCopy (XMSData)
end;

procedure EMSError (ErrorCode : Byte);
begin
  RestoreTerminal (True);
  Writeln (StdErr, 'EMS error #', ErrorCode);
  Halt (125)
end;

function EMSInstalled : Boolean; Assembler;
const
  EMMName : array [1 .. 8] of Char = 'EMMXXXX0';
  Installed : Integer = - 1;
asm
   mov ax, Installed
   or  ax, ax
   jge @2
   mov  ax, $3567
   int  $21
   mov  si, Offset EMMName
   mov  di, $a
   mov  cx, 4
   cld
   repe cmpsw
   mov  ax, 0
   jne  @1
   mov  ah, $41
   int  $67
   or   ah, ah
   mov  ax, False
   jne  @1
   mov  Word ptr EMSPtr, ax
   mov  Word ptr EMSPtr + 2, bx
   mov  ax, True
@1:mov Installed, ax
@2:
end;

function EMSPages : Word; Assembler;
asm
   mov  ah, $42
   int  $67
   or   ah, ah
   je   @1
   push ax
   call EMSError
   xor  bx, bx
@1:mov  ax, bx
end;

function EMSAllocatePages (Pages : Word) : Word; Assembler;
asm
   mov  ah, $43
   mov  bx, Pages
   int  $67
   or   ah, ah
   je   @1
   push ax
   call EMSError
   xor  dx, dx
@1:mov  ax, dx
end;

procedure EMSDeallocatePages (Handle : Word); Assembler;
asm
   mov  ah, $45
   mov  dx, Handle
   int  $67
   or   ah, ah
   je   @1
   push ax
   call EMSError
@1:
end;

procedure EMSMapPage (Handle, LPage, PPage : Word); Assembler;
asm
   mov  ax, PPage
   mov  bx, LPage
   mov  dx, Handle
   mov  ah, $44
   int  $67
   or   ah, ah
   je   @1
   push ax
   call EMSError
@1:
end;
{$endif}

procedure MoveToBigMem (var Source; p : PBigMem; BlockNumber : SizeType);
var i : Word;
begin
  with p^ do
    {$ifdef MSDOS}
    if BlockNumber <= XMSBlocks then
      MoveToXMS (Source, XMSHandle, BlockSize, LongInt (1024) * XMSKBPerBlock * (BlockNumber - 1))
    else if BlockNumber <= XMSBlocks + EMSBlocks then
      begin
        for i := 0 to EMSPagesPerBlock - 1 do
          EMSMapPage (EMSHandle, EMSPagesPerBlock * (BlockNumber - XMSBlocks - 1) + i, i);
        Move (Source, EMSPtr^, BlockSize)
      end
    else
    {$endif}
      Move (Source, PConventional [BlockNumber {$ifdef MSDOS} - XMSBlocks - EMSBlocks {$endif}]^, BlockSize)
end;

procedure MoveFromBigMem (p : PBigMem; BlockNumber : SizeType; var Dest);
var i : Integer;
begin
  with p^ do
    {$ifdef MSDOS}
    if BlockNumber <= XMSBlocks then
      MoveFromXMS (XMSHandle, Dest, BlockSize, LongInt (1024) * XMSKBPerBlock * (BlockNumber - 1))
    else if BlockNumber <= XMSBlocks + EMSBlocks then
      begin
        for i := 0 to EMSPagesPerBlock - 1 do
          EMSMapPage (EMSHandle, EMSPagesPerBlock * (BlockNumber - XMSBlocks - 1) + i, i);
        Move (EMSPtr^, Dest, BlockSize)
      end
    else
    {$endif}
      Move (PConventional [BlockNumber {$ifdef MSDOS} - XMSBlocks - EMSBlocks {$endif}]^, Dest, BlockSize)
end;

function MapBigMem (p : PBigMem; BlockNumber : SizeType) : Pointer;
var i : Integer;
begin
  with p^ do
    begin
      if not Mappable then
        begin
          RestoreTerminal (True);
          Writeln (StdErr, 'attempt to map unmappable memory');
          Halt (125)
        end;
      if XMSBlocks <> 0 then
        begin
          RestoreTerminal (True);
          Writeln (StdErr, 'internal error in MapBigMem');
          Halt (125)
        end;
      {$ifdef MSDOS}
      if BlockNumber <= EMSBlocks then
        begin
          for i := 0 to EMSPagesPerBlock - 1 do
            EMSMapPage (EMSHandle, EMSPagesPerBlock * (BlockNumber - 1) + i, i);
          MapBigMem := EMSPtr
        end
      else
      {$endif}
        MapBigMem := PConventional [BlockNumber {$ifdef MSDOS} - EMSBlocks {$endif}]
    end
end;

function AllocateBigMem (WantedNumber, aBlockSize : SizeType; WantMappable : Boolean) : PBigMem;
var
  p : PBigMem;
  i : Integer;
  pl : PBigMemList;
begin
  New (p);
  with p^ do
    begin
      Mappable := WantMappable;
      BlockSize := aBlockSize;
      ConventionalBlocks := (MemAvail - $20000) div BlockSize;
      if ConventionalBlocks > High (PConventional) then ConventionalBlocks := High (PConventional);
      {$ifdef MSDOS}
      XMSHandle := 0;
      EMSHandle := 0;
      XMSBlocks := 0;
      EMSBlocks := 0;
      if (WantedNumber > ConventionalBlocks) and EMSInstalled then
        begin
          EMSPagesPerBlock := (LongInt (BlockSize) + $3fff) div $4000;
          EMSBlocks := EMSPages div EMSPagesPerBlock;
          if EMSBlocks > WantedNumber then EMSBlocks := WantedNumber;
          Dec (WantedNumber, EMSBlocks);
          if EMSBlocks <> 0 then EMSHandle := EMSAllocatePages (EMSPagesPerBlock * EMSBlocks)
        end;
      if not WantMappable and (WantedNumber > ConventionalBlocks) and XMSInstalled then
        begin
          XMSKBPerBlock := (LongInt (BlockSize) + $3ff) div $400;
          XMSBlocks := XMSMaxAvail div XMSKBPerBlock;
          if XMSBlocks > WantedNumber then XMSBlocks := WantedNumber;
          Dec (WantedNumber, XMSBlocks);
          if XMSBlocks <> 0 then XMSHandle := XMSGetMem (XMSKBPerBlock * XMSBlocks)
        end;
      {$endif}
      if ConventionalBlocks > WantedNumber then ConventionalBlocks := WantedNumber;
      for i := 1 to ConventionalBlocks do GetMem (PConventional [i], BlockSize);
      Number := ConventionalBlocks {$ifdef MSDOS} + XMSBlocks + EMSBlocks {$endif}
    end;
  New (pl);
  pl^.Next := BigMemList;
  pl^.p := p;
  BigMemList := pl;
  AllocateBigMem := p
end;

procedure DisposeBigMem (p : PBigMem);
var
  i : Integer;
  pl : PBigMemList;
  pp : ^PBigMemList;
begin
  with p^ do
    begin
      {$ifdef MSDOS}
      if XMSHandle <> 0 then XMSFreeMem (XMSHandle);
      if EMSHandle <> 0 then EMSDeallocatePages (EMSHandle);
      XMSHandle := 0;
      EMSHandle := 0;
      {$endif}
      for i := 1 to ConventionalBlocks do FreeMem (PConventional [i], BlockSize);
      ConventionalBlocks := 0
    end;
  pp := @BigMemList;
  while (pp^ <> nil) and (pp^^.p <> p) do pp := @pp^^.Next;
  if pp^ <> nil then
    begin
      pl := pp^;
      pp^ := pp^^.Next;
      Dispose (pl)
    end
end;

type
  TPtrRec = record Ofs, Sgm : Word end;

var
  TrapExitSave,
  TrapAddr : Pointer;
  TrapSP,
  TrapBP : Word;

{$S-}
procedure TrapExit; far;
begin
  if ErrorAddr <> nil then
    begin
      ExitProc := @TrapExit;
      TrappedErrorAddr := ErrorAddr;
      TrappedExitCode := ExitCode;
      ErrorAddr := nil;
      ExitCode := 0;
      asm
        mov  sp, TrapSP
        mov  bp, TrapBP
        mov  al, 1
        jmp  [TrapAddr]
      end
    end
  else
    begin
      ExitProc := TrapExitSave;
      if TrappedErrorAddr <> nil then
        begin
          ErrorAddr := TrappedErrorAddr;
          ExitCode := TrappedExitCode
        end
    end
end;
{$S+}

function SetTrap : Boolean; far; Assembler;
asm
   mov  ax, Offset TrapExit
   mov  dx, Seg TrapExit
   cmp  ExitProc.TPtrRec.Ofs, ax
   jne  @1
   cmp  ExitProc.TPtrRec.Sgm, dx
   je   @2
@1:xchg ExitProc.TPtrRec.Ofs, ax
   xchg ExitProc.TPtrRec.Sgm, dx
   mov  TrapExitSave.TPtrRec.Ofs, ax
   mov  TrapExitSave.TPtrRec.Sgm, dx
@2:mov  TrapBP, bp
   mov  si, sp
   {$ifdef WINDOWS}
   add  si, 4
   add  TrapBP, 6
   {$endif}
   les  di, ss:[si]
   mov  TrapAddr.TPtrRec.Ofs, di
   mov  TrapAddr.TPtrRec.Sgm, es
   add  si, 4
   mov  TrapSP, si
   xor  ax, ax
end;

procedure TrapExec (p : TTrapProc);
begin
  p (SetTrap);
  if ExitProc = @TrapExit then ExitProc := TrapExitSave
end;

procedure GPCExit; far;
begin
  ExitProc := ExitSave;
  RestoreTerminal (True);
  while BigMemList <> nil do DisposeBigMem (BigMemList^.p) { Important! Removing this leads to system-wide memory leaks! }
end;

var
  i, j : Integer;
  h, m, s, s100 : Word;

begin
  Assign (StdErr, NullDeviceName);
  Rewrite (StdErr);
  TextRec (StdErr).Handle := 2;
  { Automatically check for a 80286 or higher in real mode if $G+ is set }
  {$ifdef MSDOS} {$ifopt G+}
  if Test8086 < 1 then
    begin
      Writeln (StdErr, 'This program needs at least an 80286 processor.');
      Halt (1)
    end;
  {$endif} {$endif}
  Randomize;
  ExitSave := ExitProc;
  ExitProc := @GPCExit;
  GetTime (h, m, s, s100);
  StartCPUTimeSeconds := (LongInt (h) * 60 + m) * 60 + s;
  StartCPUTimeMicroSeconds := 10000 * LongInt (s100);
  KBReadFunc := Mem [Seg0040 : $96] and $10;
  AssignCRT (CRTOutFile);
  Rewrite (CRTOutFile);
  CRTExtInit
end.
