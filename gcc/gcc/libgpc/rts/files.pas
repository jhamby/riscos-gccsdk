{
File routines

Copyright (C) 1997-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Frank Heckenbach <frank@pascal.gnu.de>

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

unit Files;

interface

uses Internal, Error, Time, String, String2, FileName;

{ bind.c }

procedure GetBinding   (protected var aFile : AnyFile; var aBinding : BindingType); asmname '_p_binding';
procedure ClearBinding (var aBinding : BindingType);                                asmname '_p_clearbinding';

{ TFDD interface @@ Subject to change! Use with caution! }

type
  TOpenMode   = (foReset, foRewrite, foAppend, foSeekRead, foSeekWrite, foSeekUpdate);
  TOpenProc   = procedure (var PrivateData; Mode : TOpenMode);
  TSelectFunc = function  (var PrivateData; Writing : Boolean) : Integer; { called before select(), must return a handle }
  TSelectProc = procedure (var PrivateData; var ReadSelect, WriteSelect, ExceptSelect : Boolean); { called before and after select() }
  TReadFunc   = function  (var PrivateData; var   Buffer; Size : SizeType) : SizeType;
  TWriteFunc  = function  (var PrivateData; const Buffer; Size : SizeType) : SizeType;
  TFileProc   = procedure (var PrivateData);
  TFlushProc  = TFileProc;
  TCloseProc  = TFileProc;
  TDoneProc   = TFileProc;

procedure AssignTFDD (var f : AnyFile;
                      OpenProc    : TOpenProc;
                      SelectFunc  : TSelectFunc;
                      SelectProc  : TSelectProc;
                      ReadFunc    : TReadFunc;
                      WriteFunc   : TWriteFunc;
                      FlushProc   : TFlushProc;
                      CloseProc   : TCloseProc;
                      DoneProc    : TDoneProc;
                      PrivateData : Pointer);       asmname '_p_assign_tfdd';

procedure SetTFDD    (var f : AnyFile;
                      OpenProc    : TOpenProc;
                      SelectFunc  : TSelectFunc;
                      SelectProc  : TSelectProc;
                      ReadFunc    : TReadFunc;
                      WriteFunc   : TWriteFunc;
                      FlushProc   : TFlushProc;
                      CloseProc   : TCloseProc;
                      DoneProc    : TDoneProc;
                      PrivateData : Pointer);       asmname '_p_set_tfdd';

{ Any parameter except f may be null }
procedure GetTFDD    (var f : AnyFile;
                      var OpenProc    : TOpenProc;
                      var SelectFunc  : TSelectFunc;
                      var SelectProc  : TSelectProc;
                      var ReadFunc    : TReadFunc;
                      var WriteFunc   : TWriteFunc;
                      var FlushProc   : TFlushProc;
                      var CloseProc   : TCloseProc;
                      var DoneProc    : TDoneProc;
                      var PrivateData : Pointer);   asmname '_p_get_tfdd';

type
  Natural = 1 .. MaxInt;
  IOSelectEvents = (SelectReadOrEOF, SelectRead, SelectEOF, SelectWrite, SelectException, SelectAlways);

const
  IOSelectEventMin = (*Low (IOSelectEvents);*)SelectReadOrEOF;
  IOSelectEventMax = Pred (SelectAlways);

type
  IOSelectType = record
    f : PAnyFile;
    Wanted : set of IOSelectEvents;
    Occurred : set of IOSelectEventMin .. IOSelectEventMax
  end;

{ Waits for one of several events to happen. Returns when one or
  more of the wanted events for one of the files occur. If they have
  already occurred before calling the function, it returns
  immediately. MicroSeconds can specify a timeout. If it is 0, the
  function will return immediately, whether or not an event has
  occurred. If it is negative, the function will wait forever until
  an event occurs. The Events parameter can be null, in which case
  the function only waits for the timeout. If any of the file
  pointers (f) in Events are nil or the files pointed to are closed,
  they are simply ignored for convenience.

  It returns the index of one of the files for which any event has
  occurred. If events have occurred for several files, is it
  undefined which of these file's index is returned. If no event
  occurs until the timeout, 0 is returned. If an error occurs or the
  target system does not have a select() system call and Events is
  not null, a negative value is returned. In the Occurred field of
  the elements of Events, events that have occurred are set. The
  state of events not wanted is undefined.

  The possible events are:
  SelectReadOrEOF: the file is at EOF or data can be read now.
  SelectRead:      data can be read now.
  SelectEOF:       the file is at EOF.
  SelectWrite:     data can be written now.
  SelectException: an exception occurred on the file.
  SelectAlways:    if this is set, *all* requested events will be
                   checked for this file in any case. Otherwise,
                   checks may be skipped if already another event
                   for this or another file was found.

  Notes:
  Checking for EOF requires some reading ahead internally (just like
  the EOF function) which can be avoided by setting SelectReadOrEOF
  instead of SelectRead and SelectEOF. If this is followed by, e.g.,
  a BlockRead with 4 parameters, the last parameter will be 0 if and
  only the file is at EOF, and otherwise, data will be read directly
  from the file without reading ahead and buffering.

  SelectAlways should be set for files for which events are
  considered to be of higher priority than others. Otherwise, if one
  is interested in just any event, not setting SelectAlways may be a
  little faster. }
function IOSelect (var Events : array [m .. n : Natural] of IOSelectType; MicroSeconds : MicroSecondTimeType) : Integer; asmname '_p_ioselect';

{ A simpler interface to SelectIO for the most common use. Waits for
  SelectReadOrEOF on all files and returns an index. }
function IOSelectRead (const Files : array [m .. n : Natural] of PAnyFile; MicroSeconds : MicroSecondTimeType) : Integer; asmname '_p_ioselectread';

procedure SleepMicroSeconds (MicroSeconds : Integer); asmname '_p_sleep_microseconds';

{ Bind a filename to an external file }
procedure AssignFile (var T : AnyFile; const Name : String);                       asmname '_p_assign';
procedure Internal_Assign (var T : AnyFile; Name : CString; NameLength : Integer); asmname '_p_internal_assign';
procedure AssignBinary (var T : Text; const Name : String);                        asmname '_p_assign_binary';
procedure AssignHandle (var T : AnyFile; Handle : Integer);                        asmname '_p_assign_handle';

{ BP compatible seeking routines }
function  SeekEOF  (var f : Text) : Boolean; asmname '_p_seekeof';
function  SeekEOLn (var f : Text) : Boolean; asmname '_p_seekeoln';

{ Under development }
procedure AnyStringTFDD_Reset (var f : AnyFile; var Buf : ConstAnyString); asmname '_p_anystring_tfdd_reset';
(*procedure AnyStringTFDD_Rewrite (var f : AnyFile; var Buf : VarAnyString); asmname '_p_anystring_tfdd_rewrite';*)
procedure StringTFDD_Reset (var f : AnyFile; var Buf : ConstAnyString; const s : String); asmname '_p_string_tfdd_reset';
(*procedure StringTFDD_Rewrite (var f : AnyFile; var Buf : VarAnyString; var s : String); asmname '_p_string_tfdd_rewrite';*)

function PExecute (ProgramName : CString; ArgV: PCStrings; var ErrMsg : String; Flags : Integer) : Integer; asmname '_p_pexecute';

{ Returns a description for a signal }
function StrSignal (Signal : Integer) : TString; asmname '_p_strsignal';

{ The following routine is meant *only* as a work-around for a
  problem under IRIX where writing reals with Write/Writeln/WriteStr
  does not work. This function will disappear after the problem has
  been solved. Width and Prec are the formatting parameters
  specified after `:'s in Write. If not wanted, NoWidth and
  NoPrecision can be passed. }
const
  NoWidth = Low (Integer);
  NoPrecision = Low (Integer);
function LongReal2Str (Num : LongReal; Width, Prec : Integer) : TString; asmname '_p_longreal2str';

implementation

{$B-,I-}

procedure AssignFile (var T : AnyFile; const Name : String);
var B : BindingType;
begin
  Unbind (T);
  B := Binding (T);
  B.Name := Name;
  B.Force := True;
  Bind (T, B)
end;

procedure Internal_Assign (var T : AnyFile; Name : CString; NameLength : Integer);
var
  B : BindingType;
  Buf : array [1 .. NameLength] of Char {$local W-} absolute Name^; {$endlocal}
begin
  Unbind (T);
  B := Binding (T);
  B.Name := Buf;
  Bind (T, B)
end;

procedure AssignBinary (var T : Text; const Name : String);
var B : BindingType;
begin
  Unbind (T);
  B := Binding (T);
  B.Name := Name;
  B.Force := True;
  B.TextBinary := True;
  Bind (T, B)
end;

procedure AssignHandle (var T : AnyFile; Handle : Integer);
var B : BindingType;
begin
  Unbind (T);
  B := Binding (T);
  B.Handle := Handle;
  Bind (T, B)
end;

function SeekEOF (var f : Text) : Boolean;
begin
  repeat
    if EOF (f) then
      return True
    else if EOLn (f) then
      Readln (f)
    else if f^ > ' ' then
      return False
    else
      Get (f)
  until False
end;

function SeekEOLn (var f : Text) : Boolean;
begin
  repeat
    if EOF (f) or EOLn (f) then
      return True
    else if f^ > ' ' then
      return False
    else
      Get (f)
  until False
end;

function IOSelect (var Events : array [m .. n : Natural] of IOSelectType; MicroSeconds : MicroSecondTimeType) : Integer;
var i : Integer;
begin
  SetReturnAddress (ReturnAddress (0));
  if (@Events = nil) or (m > n) then
    IOSelect := InternalIOSelect (nil, 0, 0, MicroSeconds)
  else
    begin
      var InternalEvents : array [m .. n] of InternalIOSelectType;
      for i := m to n do
        with Events [i], InternalEvents [i] do
          begin
            WantedReadOrEOF := SelectReadOrEOF in Wanted;
            WantedRead      := SelectRead      in Wanted;
            WantedEOF       := SelectEOF       in Wanted;
            WantedWrite     := SelectWrite     in Wanted;
            WantedException := SelectException in Wanted;
            WantedAlways    := SelectAlways    in Wanted;
            fi := f
          end;
      IOSelect := InternalIOSelect (@InternalEvents [m], m, n - m + 1, MicroSeconds);
      for i := m to n do
        with Events [i], InternalEvents [i] do
          begin
            Occurred := [];
            if OccurredReadOrEOF then Include (Occurred, SelectReadOrEOF);
            if OccurredRead      then Include (Occurred, SelectRead);
            if OccurredEOF       then Include (Occurred, SelectEOF);
            if OccurredWrite     then Include (Occurred, SelectWrite);
            if OccurredException then Include (Occurred, SelectException)
          end
    end;
  RestoreReturnAddress
end;

function IOSelectRead (const Files : array [m .. n : Natural] of PAnyFile; MicroSeconds : MicroSecondTimeType) : Integer;
var i : Integer;
begin
  SetReturnAddress (ReturnAddress (0));
  if (@Files = nil) or (m > n) then
    IOSelectRead := InternalIOSelect (nil, 0, 0, MicroSeconds)
  else
    begin
      var InternalEvents : array [m .. n] of InternalIOSelectType;
      for i := m to n do
        with InternalEvents [i] do
          begin
            WantedReadOrEOF := True;
            WantedRead      := False;
            WantedEOF       := False;
            WantedWrite     := False;
            WantedException := False;
            WantedAlways    := False;
            fi := Files [i]
          end;
      IOSelectRead := InternalIOSelect (@InternalEvents [m], m, n - m + 1, MicroSeconds)
    end;
  RestoreReturnAddress
end;

procedure SleepMicroSeconds (MicroSeconds : Integer);
var Dummy : Integer;
begin
  Dummy := IOSelect (null, MicroSeconds)
end;

function StringTFDD_Read (var PrivateData; var Buffer; Size : SizeType) = Result : SizeType;
begin
  with ConstAnyString (PrivateData) do
    begin
      Result := Min (Size, Length);
      Move (Chars^, Buffer, Result);
      {$local X+} Inc (Chars, Result); {$endlocal}
      Dec (Length, Result)
    end
end;

function StringTFDD_Write (var PrivateData; const Buffer; Size : SizeType) = Result : SizeType;
var CurLength : Cardinal;
begin
  with VarAnyString (PrivateData) do
    begin
      CurLength := VarAnyStringLength (VarAnyString (PrivateData));
      Result := Max (0, VarAnyStringSetLength (VarAnyString (PrivateData),
        Min (Capacity, CurLength + Size)) - CurLength);
      Move (Buffer, Chars^[CurLength + 1], Result);
      if Truncate then Result := Size
    end
end;

procedure AnyStringTFDD_Reset (var f : AnyFile; var Buf : ConstAnyString);
begin
  AssignTFDD (f, nil, nil, nil, StringTFDD_Read, nil, nil, nil, nil, @Buf);
  Reset (f)
end;

procedure AnyStringTFDD_Rewrite (var f : AnyFile; var Buf : VarAnyString);
var Dummy : Integer;
begin
  Dummy := VarAnyStringSetLength (Buf, 0);
  AssignTFDD (f, nil, nil, nil, nil, StringTFDD_Write, nil, nil, nil, @Buf);
  Rewrite (f)
end;

procedure StringTFDD_Reset (var f : AnyFile; var Buf : ConstAnyString; const s : String);
begin
  Buf.Length := Length (s);
  Buf.Chars := PChars (@s[1]);
  AnyStringTFDD_Reset (f, Buf)
end;

procedure StringTFDD_Rewrite (var f : AnyFile; var Buf : VarAnyString; var s : String);
begin
  Buf.Capacity := s.Capacity;
  Buf.Chars := PChars (@s[1]);
  Buf.Truncate := True; (*@@*)
  Buf.StringType := AnyStringLong;
  Buf.PLongLength := @Cardinal (GPC_PString (@s)^.Length);
  AnyStringTFDD_Rewrite (f, Buf)
end;

function PExecute (ProgramName : CString; ArgV: PCStrings; var ErrMsg : String; Flags : Integer) : Integer;
var
  ErrMsg_Fmt, ErrMsg_Arg : CString = nil;
  i : Integer;
begin
  PExecute := PExecuteC (ProgramName, ArgV, CParameters^[0],
    GetTempDirectory + 'ccXXXXXX', ErrMsg_Fmt, ErrMsg_Arg, Flags);
  if ErrMsg_Fmt = nil then
    ErrMsg := ''
  else
    begin
      ErrMsg := CString2String (ErrMsg_Fmt);
      i := Pos ('%s', ErrMsg);
      if (ErrMsg_Arg <> nil) and (i <> 0) then
        begin
          Delete (ErrMsg, i, 2);
          Insert (CString2String (ErrMsg_Arg), ErrMsg, i)
        end
    end
end;

function StrSignal (Signal : Integer) : TString;
begin
  StrSignal := CString2String (CStringStrSignal (Signal))
end;

function LongReal2Str (Num : LongReal; Width, Prec : Integer) = Res : TString;
var NewLength : Integer;
begin
  GPC_WriteStr_Real (PChars (@Res [1]), NewLength, Res.Capacity, Num, Width, Prec);
  {$local X+} SetLength (Res, NewLength) {$endlocal}
end(*@@superfluous warning*)(*$local W-*);(*$endlocal*)

end.
