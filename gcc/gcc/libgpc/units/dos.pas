{
Portable BP compatible Dos unit

This unit supports most of the routines and declarations of BP's Dos
unit. The following routines and declarations are not supported, and
most likely will never be in a portable way, for some reasons.

NOTES:

- The procedures Keep, GetIntVec, SetIntVec are not supported since
  they make only sense for Dos real-mode programs (and GPC compiled
  programs do not run in real-mode, even on x86 under Dos). A few
  other Dos-only routines (see below in the interface declaration)
  are only supported under DJGPP if `__BP_UNPORTABLE_ROUTINES__' is
  defined (with the `-D__BP_UNPORTABLE_ROUTINES__' option).

- The internal structure of file variables (FileRec and TextRec) is
  different in GPC. However, as far as TFDDs are concerned, there
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
{$if __GPC_RELEASE__ < 20000412}
{$error This unit requires GPC release 20000412 or newer}
{$endif}

unit Dos;

interface

uses GPC, System;

type
  Byte8 = Cardinal (8);
  TDosAttr = Word;
  GPC_AnyFile = AnyFile; { in order to have AnyFile parameters,
                           while AnyFile is redefined below }

const
  { File attribute constants }
  ReadOnly      = $01;
  Hidden        = $02; { set for dot files except '.' and '..' }
  SysFile       = $04; { not supported }
  VolumeID      = $08; { not supported }
  Directory     = $10;
  Archive       = $20; { means: not executable }
  {$local W-} AnyFile = $3F; {$endlocal}

  { Flag bit masks -- only used by the unportable Dos routines }
  FCarry     = 1;
  FParity    = 4;
  FAuxiliary = $10;
  FZero      = $40;
  FSign      = $80;
  FOverflow  = $800;

  { DosError codes }
  DosError_FileNotFound = 2;
  DosError_PathNotFound = 3;
  DosError_AccessDenied = 5;
  DosError_InvalidMem   = 9;
  DosErorr_InvalidEnv   = 10;
  DosError_NoMoreFiles  = 18;
  DosError_IOError      = 29;
  DosError_ReadFault    = 30;

type
  { String types. Not used in this unit, but declared for
    compatibility. }
  ComStr  = String [127];        { Command line string }
  PathStr = String [79];         { File pathname string }
  DirStr  = String [67];         { Drive and directory string }
  NameStr = String [8];          { File name string }
  ExtStr  = String [4];          { File extension string }

  TextBuf = array [0 .. 127] of Char;

  { Search record used by FindFirst and FindNext }
  SearchRecFill = packed array [1 .. 21] of Byte8;
  SearchRec = {$ifdef __BP_TYPE_SIZES__} packed {$endif} record
    Fill : SearchRecFill;
    Attr : Byte8;
    Time, Size : LongInt;
    Name : {$ifdef __BP_TYPE_SIZES__}
           String [12]
           {$else}
           TString
           {$endif}
  end;

  { Date and time record used by PackTime and UnpackTime }
  DateTime = record
    Year, Month, Day, Hour, Min, Sec : Word
  end;

  { 8086 CPU registers -- only used by the unportable Dos routines }
  Registers = record
  case Boolean of
    False : (AX, BX, CX, DX, BP, SI, DI, DS, ES, Flags : Word);
    True  : (AL, AH, BL, BH, CL, CH, DL, DH : Byte)
  end;

var
  { Error status variable }
  DosError : Integer = 0;

procedure GetDate (var Year, Month, Day, DayOfWeek : Word);                     asmname '_p_getdate';
procedure GetTime (var Hour, Minute, Second, Sec100 : Word);                    asmname '_p_gettime';
function  DiskFree (Drive : Byte) : LongInt;                                    asmname '_p_diskfree';
function  DiskSize (Drive : Byte) : LongInt;                                    asmname '_p_disksize';
procedure GetFAttr (var F : GPC_AnyFile; var Attr : TDosAttr);                  asmname '_p_getfattr';
procedure SetFAttr (var F : GPC_AnyFile; Attr : TDosAttr);                      asmname '_p_setfattr';
procedure GetFTime (var F : GPC_AnyFile; var aTime : LongInt);                  asmname '_p_getftime';
procedure SetFTime (var F : GPC_AnyFile; aTime : LongInt);                      asmname '_p_setftime';
procedure FindFirst (const Path : String; Attr : TDosAttr; var SR : SearchRec); asmname '_p_findfirst';
procedure FindNext  (var SR : SearchRec);                                       asmname '_p_findnext';
procedure FindClose (var SR : SearchRec);                                       asmname '_p_findclose';
procedure UnpackTime (P : LongInt; var T : DateTime);                           asmname '_p_unpacktime';
procedure PackTime (const T : DateTime; var P : LongInt);                       asmname '_p_packtime';
function  FSearch (const aFileName, DirList : String) : TString;                asmname '_p_fsearch';
function  FExpand (const Path : String) : TString;                              asmname '_p_fexpand';
procedure FSplit  (const Path : String; var Dir, Name, Ext : String);           asmname '_p_fsplit';
function  EnvCount : Integer;
function  EnvStr (EnvIndex : Integer) : TString;
function  GetEnv (const EnvVar : String) : TString;                             asmname '_p_getenv';
procedure SwapVectors;
{ Exec executes a process via Execute, so RestoreTerminal is called with
  the argument True before and False after executing the process. }
procedure Exec (const Path, Params : String);
function  DosExitCode : Word;

{ @@ Dummies except for DJGPP }
procedure GetCBreak (var Break : Boolean);                                      asmname '_p_getcbreak';
procedure SetCBreak (Break : Boolean);                                          asmname '_p_setcbreak';

{ Dummies except for DJGPP (under the assumption that any real OS
  knows by itself when and how to verify its disks. }
procedure GetVerify (var Verify : Boolean);                                     asmname '_p_getverify';
procedure SetVerify (Verify : Boolean);                                         asmname '_p_setverify';

{ Unportable Dos-only routines and declarations }

{$if defined (__BP_UNPORTABLE_ROUTINES__) and defined (DJGPP)}
{ These are unportable Dos-only declarations and routines, since
  interrupts are Dos and CPU specific (and have no place in a
  high-level program, anyway). }
procedure Intr (IntNo : Byte; var Regs : Registers);                            asmname '_p_intr';
procedure MsDos (var Regs : Registers);                                         asmname '_p_msdos';

{ This is an unportable Dos-only routine, since the format of the
  return value of this function is Dos specific. Though probably all
  other system have versions numbers as well, returning them here
  would usually not do what is expected, e.g. testing if certain Dos
  features are present by comparing the version number. }
function  DosVersion : Word;                                                    asmname '_p_dosversion';

{ These are unportable Dos-only routines, changing the system date
  and time is a system administration task, and have no place in a
  high-level program. }
procedure SetTime (Hour, Minute, Second, Sec100 : Word);                        asmname '_p_settime';
procedure SetDate (Year, Month, Day : Word);                                    asmname '_p_setdate';
{$endif}

implementation

{$B-,I-}

type
  PLongInt = ^LongInt;

var
  DosExitCodeVar : Word = 0;

procedure GetDate (var Year, Month, Day, DayOfWeek : Word);
var t : TimeStamp;
begin
  GetTimeStamp (t);
  Year      := t.Year;
  Month     := t.Month;
  Day       := t.Day;
  DayOfWeek := t.DayOfWeek
end;

procedure GetTime (var Hour, Minute, Second, Sec100 : Word);
var t : TimeStamp;
begin
  GetTimeStamp (t);
  Hour   := t.Hour;
  Minute := t.Minute;
  Second := t.Second;
  Sec100 := t.MicroSecond div 10000
end;

function DiskFree (Drive : Byte) : LongInt;
var
  Path : String (2);
  Buf : StatFSBuffer;
begin
  if Drive = 0 then
    Path := DirSelf
  else
    Path := Succ ('a', Drive - 1) + ':';
  StatFS (Path, Buf);
  if IOResult = 0 then
    DiskFree := Buf.BlockSize * Buf.BlocksFree
  else
    begin
      DosError := DosError_AccessDenied;
      DiskFree := - 1
    end
end;

function DiskSize (Drive : Byte) : LongInt;
var
  Path : String (2);
  Buf : StatFSBuffer;
begin
  if Drive = 0 then
    Path := DirSelf
  else
    Path := Succ ('a', Drive - 1) + ':';
  StatFS (Path, Buf);
  if IOResult = 0 then
    DiskSize := Buf.BlockSize * Buf.BlocksTotal
  else
    begin
      DosError := DosError_AccessDenied;
      DiskSize := - 1
    end
end;

procedure GetFAttr (var F : GPC_AnyFile; var Attr : TDosAttr);
var
  b : BindingType;
  s : TString;
  d : OrigInt;
begin
  b := Binding (F);
  Attr := 0;
  if not (b.Bound and (b.Existing or b.Directory or b.Special)) then
    DosError := DosError_FileNotFound
  else
    begin
      DosError := 0;
      if b.Directory      then Attr := Attr or Directory;
      if not b.Writable   then Attr := Attr or ReadOnly;
      if not b.Executable then Attr := Attr or Archive;
      d := Length (b.Name);
      while (d > 0) and not (b.Name [d] in DirSeparators) do Dec (d);
      if (Length (b.Name) > d + 1) and (b.Name [d + 1] =  '.') and
        ((Length (b.Name) > d + 2) or  (b.Name [d + 2] <> '.')) then
        Attr := Attr or Hidden
    end
end;

procedure SetFAttr (var F : GPC_AnyFile; Attr : TDosAttr);
var b : BindingType;
begin
  b := Binding (F);
  if not b.Bound then
    begin
      DosError := DosError_FileNotFound;
      Exit
    end;
  if Attr and ReadOnly = 0
    then or  (b.Mode, fmUserWritable) { Set only user write permissions, for reasons of safety! }
    else and (b.Mode, not (fmUserWritable or fmGroupWritable or fmOthersWritable));
  if Attr and Archive = 0
    then or  (b.Mode, fmUserExecutable or fmGroupExecutable or fmOthersExecutable)
    else and (b.Mode, not (fmUserExecutable or fmGroupExecutable or fmOthersExecutable));
  ChMod (F, b.Mode);
  if IOResult <> 0 then DosError := DosError_AccessDenied
end;

procedure GetFTime (var F : GPC_AnyFile; var aTime : LongInt);
var
  b : BindingType;
  Year, Month, Day, Hour, Minute, Second : OrigInt;
  dt : DateTime;
begin
  b := Binding (F);
  if not (b.Bound and (b.Existing or b.Directory or b.Special)) then
    DosError := DosError_FileNotFound
  else
    begin
      if b.ModificationTime >= 0 then
        begin
          UnixTimeToTime (b.ModificationTime, Year, Month, Day, Hour, Minute, Second);
          dt.Year  := Year;
          dt.Month := Month;
          dt.Day   := Day;
          dt.Hour  := Hour;
          dt.Min   := Minute;
          dt.Sec   := Second;
          PackTime (dt, aTime)
        end
      else
        aTime := 0;
      DosError := 0
    end
end;

procedure SetFTime (var F : GPC_AnyFile; aTime : LongInt);
var
  dt : DateTime;
  ut: UnixTimeType;
begin
  UnpackTime (aTime, dt);
  with dt do ut := TimeToUnixTime (Year, Month, Day, Hour, Min, Sec);
  DosError := DosError_AccessDenied;
  if ut >= 0 then
    begin
      SetFileTime (F, ut);
      if IOResult = 0 then DosError := 0
    end
end;

{ Since there's no explicit closing of FindFirst/FindNext, FindList keeps
  tracks of all running searches so they can be closed automatically when
  necessary, and Magic indicates if a SearchRec is currently in use. }

const
  srOpened = $2424d00f;
  srDone   = $4242f00d;

type
  TSRFillInternal = packed record
    Magic  : OrigInt;
    Unused : packed array [1..SizeOf (SearchRecFill) - SizeOf (OrigInt)] of Byte
  end;

  PPFindList = ^PFindList;
  PFindList  = ^TFindList;
  TFindList  = record
    Next : PFindList;
    SR   : ^SearchRec;
    Dir,
    Name,
    Ext  : TString;
    Attr : TDosAttr;
    PDir : Pointer;
  end;

var
  FindList : PFindList = nil;

procedure CloseFind (PTemp : PPFindList);
var Temp : PFindList;
begin
  Temp := PTemp^;
  CloseDir (Temp^.PDir);
  TSRFillInternal (Temp^.SR^.Fill).Magic := srDone;
  PTemp^ := Temp^.Next;
  Dispose (Temp)
end;

procedure FindFirst (const Path : String; Attr : TDosAttr; var SR : SearchRec);
var
  Temp : PFindList;
  PTemp : PPFindList;
begin
  { If SR was used before, close it first }
  PTemp := @FindList;
  while (PTemp^ <> nil) and (PTemp^^.SR <> @SR) do PTemp := @PTemp^^.Next;
  if PTemp^ <> nil then
    begin
      CloseFind (PTemp);
      if IOResult <> 0 then DosError := DosError_ReadFault
    end;
  if Attr and not (ReadOnly or Archive) = VolumeID then
    begin
      DosError := DosError_NoMoreFiles;
      Exit
    end;
  New (Temp);
  FSplit (Path, Temp^.Dir, Temp^.Name, Temp^.Ext);
  if Temp^.Dir = '' then Temp^.Dir := DirSelf + DirSeparator;
  if Temp^.Ext = '' then Temp^.Ext := ExtSeparator;
  Temp^.SR := @SR;
  Temp^.Attr := Attr;
  Temp^.PDir := OpenDir (Temp^.Dir);
  if IOResult <> 0 then
    begin
      TSRFillInternal (SR.Fill).Magic := srDone;
      Dispose (Temp);
      DosError := DosError_NoMoreFiles;
      Exit
    end;
  TSRFillInternal (SR.Fill).Magic := srOpened;
  Temp^.Next := FindList;
  FindList := Temp;
  FindNext (SR)
end;

procedure FindNext (var SR : SearchRec);
var
  Temp : PFindList;
  PTemp : PPFindList;
  Name, Dir, Nam, Ext : TString;
  F : Text;
  TmpAttr : TDosAttr;
  TmpTime : LongInt;

  { Emulate Dos brain-damaged file name wildcard matching }
  function MatchPart (const aName, Mask : String) : Boolean;
  var i : OrigInt;
  begin
    for i := 1 to Length (Mask) do
      case Mask [i] of
        '?' : ;
        '*' : return True;
        else
          if (i > Length (aName)) or (FileNameLoCase (aName [i]) <> FileNameLoCase (Mask [i])) then return False
      end;
    MatchPart := Length (Mask) >= Length (aName)
  end;

begin
  DosError := 0;
  { Check if SR is still valid }
  case TSRFillInternal (SR.Fill).Magic of
    srOpened : ;
    srDone   : begin
                 DosError := DosError_NoMoreFiles;
                 Exit
               end;
    else
      DosError := DosError_InvalidMem;
      Exit
  end;
  PTemp := @FindList;
  while (PTemp^ <> nil) and (PTemp^^.SR <> @SR) do PTemp := @PTemp^^.Next;
  Temp := PTemp^;
  if Temp = nil then
    begin
      DosError := DosError_InvalidMem;
      Exit
    end;
  repeat
    Name := ReadDir (Temp^.PDir);
    if Name = '' then
      begin
        CloseFind (PTemp);
        if IOResult = 0
          then DosError := DosError_NoMoreFiles
          else DosError := DosError_ReadFault;
        Exit
      end;
    Assign (F, Temp^.Dir + Name);
    GetFAttr (F, TmpAttr);
    SR.Attr := TmpAttr;
    FSplit (Name, Dir, Nam, Ext);
    if Ext = '' then Ext := ExtSeparator
  until MatchPart (Nam, Temp^.Name) and MatchPart (Ext, Temp^.Ext) and
        { Emulate Dos brain-damaged file attribute matching }
        ((Temp^.Attr and (Hidden or SysFile) <> 0) or (TmpAttr and Hidden    = 0)) and
        ((Temp^.Attr and Directory           <> 0) or (TmpAttr and Directory = 0));
  SR.Name := Name;
  if DosError <> 0 then Exit;
  GetFTime (F, TmpTime);
  SR.Time := TmpTime;
  if Binding (F).Existing then
    begin
      Reset (F);
      SR.Size := FileSize (F);
      Close (F)
    end
  else
    SR.Size := 0
end;

procedure FindClose (var SR : SearchRec);
var PTemp : PPFindList;
begin
  PTemp := @FindList;
  while (PTemp^ <> nil) and (PTemp^^.SR <> @SR) do PTemp := @PTemp^^.Next;
  if PTemp^ <> nil then
    begin
      CloseFind (PTemp);
      if IOResult <> 0 then DosError := DosError_ReadFault
    end
end;

procedure UnpackTime (P : LongInt; var T : DateTime);
begin
  T.Year  := (P shr 25) and $7f + 1980;
  T.Month := (P shr 21) and $f;
  T.Day   := (P shr 16) and $1f;
  T.Hour  := (P shr 11) and $1f;
  T.Min   := (P shr 5) and $3f;
  T.Sec   := 2 * (P and $1f)
end;

procedure PackTime (const T : DateTime; var P : LongInt);
begin
  P := (LongInt (T.Year) - 1980) shl 25 + LongInt (T.Month) shl 21 + LongInt (T.Day) shl 16
       + T.Hour shl 11 + T.Min shl 5 + T.Sec div 2
end;

function EnvCount : Integer;
begin
  EnvCount := Environment^.Count
end;

function EnvStr (EnvIndex : Integer) : TString;
begin
  if (EnvIndex < 1) or (EnvIndex > EnvCount)
    then EnvStr := ''
    else EnvStr := CString2String (Environment^.CStrings [EnvIndex])
end;

procedure SwapVectors;
begin
  { Nothing to be done }
end;

procedure Exec (const Path, Params : String);
begin
  DosExitCodeVar := Execute (Path + ' ' + Params);
  if IOResult <> 0 then DosError := DosError_FileNotFound
end;

function DosExitCode : Word;
begin
  DosExitCode := DosExitCodeVar
end;

{$ifdef DJGPP}

type
  TDPMIRegs = record
    EDI, ESI, EBP, Reserved, EBX, EDX, ECX, EAX : Word32;
    Flags, ES, DS, FS, GS, IP, CS, SP, SS : Word16
  end;

procedure RealModeInterrupt (InterruptNumber : Integer; var Regs : TDPMIRegs); asmname '__dpmi_int';

procedure Intr (IntNo : Byte; var Regs : Registers);
var DPMIRegs : TDPMIRegs;
begin
  FillChar (DPMIRegs, SizeOf (DPMIRegs), 0);
  with DPMIRegs do
    begin
      EDI := Regs.DI;
      ESI := Regs.SI;
      EBP := Regs.BP;
      EBX := Regs.BX;
      EDX := Regs.DX;
      ECX := Regs.CX;
      EAX := Regs.AX;
      Flags := Regs.Flags;
      ES := Regs.ES;
      DS := Regs.DS;
      RealModeInterrupt (IntNo, DPMIRegs);
      Regs.DI := EDI;
      Regs.SI := ESI;
      Regs.BP := EBP;
      Regs.BX := EBX;
      Regs.DX := EDX;
      Regs.CX := ECX;
      Regs.AX := EAX;
      Regs.Flags := Flags;
      Regs.ES := ES;
      Regs.DS := DS
    end
end;

procedure MsDos (var Regs : Registers);
begin
  Intr ($21, Regs)
end;

function DosVersion : Word;
var Regs : Registers;
begin
  Regs.AX := $3000;
  MsDos (Regs);
  DosVersion := Regs.AX
end;

procedure SetTime (Hour, Minute, Second, Sec100 : Word);
var Regs : Registers;
begin
  Regs.AX := $2d00;
  Regs.CX := $100 * Hour + Minute;
  Regs.DX := $100 * Second + Sec100;
  MsDos (Regs)
end;

procedure SetDate (Year, Month, Day : Word);
var Regs : Registers;
begin
   Regs.AX := $2b00;
   Regs.CX := Year;
   Regs.DX := $100 * Month + Day;
   MsDos (Regs)
end;

procedure GetCBreak (var Break : Boolean);
var Regs : Registers;
begin
  Regs.AX := $3300;
  MsDos (Regs);
  Break := Regs.DL <> 0
end;

procedure SetCBreak (Break : Boolean);
var Regs : Registers;
begin
  Regs.AX := $3301;
  Regs.DX := Ord (Break);
  MsDos (Regs)
end;

procedure GetVerify (var Verify : Boolean);
var Regs : Registers;
begin
  Regs.AX := $5400;
  MsDos (Regs);
  Verify := Regs.AL <> 0
end;

procedure SetVerify (Verify : Boolean);
var Regs : Registers;
begin
  Regs.AX := $2e00 + Ord (Verify);
  MsDos (Regs)
end;

{$else}

var
  LastCBreak, LastVerify : Boolean = True;

procedure GetCBreak (var Break : Boolean);
begin
  Break := LastCBreak
end;

procedure SetCBreak (Break : Boolean);
begin
  LastCBreak := Break
end;

procedure GetVerify (var Verify : Boolean);
begin
  Verify := LastVerify
end;

procedure SetVerify (Verify : Boolean);
begin
  LastVerify := Verify
end;

{$endif}

to end do
  while FindList <> nil do
    begin
      var i : OrigInt = IOResult;
      CloseFind (@FindList);
      InOutRes := i
    end;

end.
