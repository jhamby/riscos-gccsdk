{
Mostly BP compatible portable WinDos unit

This unit supports most, but not all, of the routines and
declarations of BP's WinDos unit. The following routines and
declarations are not supported, and most likely will never be in a
portable way, for some reasons.

- The procedures GetIntVec and SetIntVec are not supported since
  they make only sense for Dos real-mode programs (and GPC compiled
  programs do not run in real-mode, even on x86 under Dos). A few
  other Dos-only routines (see below in the interface declaration)
  are only supported under DJGPP if `__BP_UNPORTABLE_ROUTINES__' is
  defined (with the `-D__BP_UNPORTABLE_ROUTINES__' option).

- The internal structure of file variables (TFileRec and TTextRec)
  is different in GPC. However, as far as TFDDs are concerned, there
  are other ways to achieve the same in GPC, see the GPC unit.

- FindFirst and FindNext are quite inefficient since they emulate
  all the brain-dead Dos stuff. If at all possible, the standard
  routines OpenDir, ReadDir and CloseDir (in the GPC unit) should be
  used instead.

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

unit WinDos;

interface

uses GPC, System, Dos;

const
  { File attribute constants }
  faReadOnly  = ReadOnly;
  faHidden    = Hidden;    { set for dot files except `.' and `..' }
  faSysFile   = SysFile;   { not supported }
  faVolumeID  = VolumeID;  { not supported }
  faDirectory = Directory;
  faArchive   = Archive;   { means: not executable }
  faAnyFile   = AnyFile;

  { Maximum file name component string lengths }
  fsPathName  = 79;
  fsDirectory = 67;
  fsFileName  = 8;
  fsExtension = 4;

  { FileSplit return flags }
  fcExtension = 1;
  fcFileName  = 2;
  fcDirectory = 4;
  fcWildcards = 8;

  { Flag bit masks -- only used by the unportable Dos routines }
  FCarry     = 1;
  FParity    = 4;
  FAuxiliary = $10;
  FZero      = $40;
  FSign      = $80;
  FOverflow  = $800;

type
  PTextBuf = ^TTextBuf;
  TTextBuf = TextBuf;

  { Search record used by FindFirst and FindNext }
  TSearchRec = {$ifdef __BP_TYPE_SIZES__} packed {$endif} record
     Fill : SearchRecFill;
     Attr : Byte8;
     Time, Size : LongInt;
     Name : {$ifdef __BP_TYPE_SIZES__}
            packed array [0 .. 12] of Char
            {$else}
            TStringBuf
            {$endif};
     Reserved : SearchRec
  end;

  { Date and time record used by PackTime and UnpackTime }
  TDateTime = DateTime;

  { 8086 CPU registers -- only used by the unportable Dos routines }
  TRegisters = Registers;

var
  { Error status variable }
  DosError : Integer; external;

procedure GetDate (var Year, Month, Day, DayOfWeek : Word);    asmname '_p_getdate';
procedure GetTime (var Hour, Minute, Second, Sec100 : Word);   asmname '_p_gettime';
function  DiskFree (Drive : Byte) : LongInt;                   asmname '_p_diskfree';
function  DiskSize (Drive : Byte) : LongInt;                   asmname '_p_disksize';
procedure GetFAttr (var F : GPC_AnyFile; var Attr : Word);     asmname '_p_getfattr';
procedure SetFAttr (var F : GPC_AnyFile; Attr : Word);         asmname '_p_setfattr';
procedure GetFTime (var F : GPC_AnyFile; var aTime : LongInt); asmname '_p_getftime';
procedure SetFTime (var F : GPC_AnyFile; aTime : LongInt);     asmname '_p_setftime';
procedure (*@@fjf260*)WFindFirst (Path : PChar; Attr : Word; var SR : TSearchRec);
procedure (*@@fjf260*)WFindNext  (var SR : TSearchRec);
procedure (*@@fjf260*)WFindClose (var SR : TSearchRec);
procedure UnpackTime (P : LongInt; var T : TDateTime);         asmname '_p_unpacktime';
procedure PackTime (const T : TDateTime; var P : LongInt);     asmname '_p_packtime';
function  FileSearch (Dest, Name, List : PChar) : PChar;
function  FileExpand (Dest, Name : PChar) : PChar;
function  FileSplit (Path, Dir, Name, Ext : PChar) : Word;
function  GetCurDir (Dir : PChar; Drive : Byte) : PChar;
procedure SetCurDir (Dir : PChar);
procedure CreateDir (Dir : PChar);
procedure RemoveDir (Dir : PChar);
function  GetArgCount : Integer;
function  GetArgStr (Dest : PChar; ArgIndex : Integer; MaxLen : Word) : PChar;
function  GetEnvVar (VarName : PChar) : PChar;                 asmname '_p_cstringgetenv';
procedure (*@@fjf260*)WGetCBreak (var Break : Boolean);        asmname '_p_getcbreak';
procedure (*@@fjf260*)WSetCBreak (Break : Boolean);            asmname '_p_setcbreak';
procedure (*@@fjf260*)WGetVerify (var Verify : Boolean);       asmname '_p_getverify';
procedure (*@@fjf260*)WSetVerify (Verify : Boolean);           asmname '_p_setverify';

{$if defined (__BP_UNPORTABLE_ROUTINES__) and defined (DJGPP)}
procedure (*@@fjf260*)WIntr (IntNo : Byte; var Regs : Registers);     asmname '_p_intr';
procedure (*@@fjf260*)WMsDos (var Regs : Registers);                  asmname '_p_msdos';
function  (*@@fjf260*)WDosVersion : Word;                             asmname '_p_dosversion';
procedure (*@@fjf260*)WSetTime (Hour, Minute, Second, Sec100 : Word); asmname '_p_settime';
procedure (*@@fjf260*)WSetDate (Year, Month, Day : Word);             asmname '_p_setdate';
{$endif}

implementation

{$B-,I-}

procedure DosFindFirst (const Path : String; Attr : Word; var SR : SearchRec); asmname '_p_findfirst';
procedure DosFindNext  (var SR : SearchRec);                                   asmname '_p_findnext';
procedure DosFindClose (var SR : SearchRec);                                   asmname '_p_findclose';

procedure ConvertSearchRec (var SR : TSearchRec);
begin
  SR.Attr := SR.Reserved.Attr;
  SR.Time := SR.Reserved.Time;
  SR.Size := SR.Reserved.Size;
  SR.Name := SR.Reserved.Name + #0
end;

procedure (*@@fjf260*)WFindFirst (Path : PChar; Attr : Word; var SR : TSearchRec);
begin
  DosFindFirst (CString2String (Path), Attr, SR.Reserved);
  if DosError = 0 then ConvertSearchRec (SR)
end;

procedure (*@@fjf260*)WFindNext (var SR : TSearchRec);
begin
  DosFindNext (SR.Reserved);
  if DosError = 0 then ConvertSearchRec (SR)
end;

procedure (*@@fjf260*)WFindClose (var SR : TSearchRec);
begin
  DosFindClose (SR.Reserved)
end;

function FileSearch (Dest, Name, List : PChar) : PChar;
begin
  FileSearch := CStringLCopy (Dest, FSearch (CString2String (Name), CString2String (List)), fsPathName)
end;

function FileExpand (Dest, Name : PChar) : PChar;
begin
  FileExpand := CStringLCopy (Dest, FExpand (CString2String (Name)), fsPathName)
end;

function FileSplit (Path, Dir, Name, Ext : PChar) = Res : Word;
var
  d : DirStr;
  n : NameStr;
  e : ExtStr;
  Dummy : CString;
begin
  FSplit (CString2String (Path), d, n, e);
  if Dir  <> nil then Dummy := CStringCopy (Dir,  d);
  if Name <> nil then Dummy := CStringCopy (Name, n);
  if Ext  <> nil then Dummy := CStringCopy (Ext,  e);
  Res := 0;
  if d <> '' then Inc (Res, fcDirectory);
  if n <> '' then Inc (Res, fcFileName);
  if e <> '' then Inc (Res, fcExtension);
  if HasWildCards (n + e) then Inc (Res, fcWildcards)
end;

function GetCurDir (Dir : PChar; Drive : Byte) : PChar;
var Path : String (2);
begin
  if Drive = 0 then
    Path := DirSelf
  else
    Path := Succ ('a', Drive - 1) + ':';
  GetCurDir := CStringLCopy (Dir, FExpand (Path), fsDirectory)
end;

procedure SetCurDir (Dir : PChar);
begin
  ChDir (CString2String (Dir))
end;

procedure CreateDir (Dir : PChar);
begin
  MkDir (CString2String (Dir))
end;

procedure RemoveDir (Dir : PChar);
begin
  RmDir (CString2String (Dir))
end;

function GetArgCount : Integer;
begin
  GetArgCount := ParamCount
end;

function GetArgStr (Dest : PChar; ArgIndex : Integer; MaxLen : Word) : PChar;
begin
  GetArgStr := CStringLCopy (Dest, ParamStr (ArgIndex), MaxLen)
end;

end.
