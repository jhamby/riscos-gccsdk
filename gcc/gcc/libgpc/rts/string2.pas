{
String handling routines, higher level part

Copyright (C) 1997-2000 Free Software Foundation, Inc.

Authors: Frank Heckenbach <frank@pascal.gnu.de>
         Jukka Virtanen <jtv@hut.fi>

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

unit String2;

interface

uses Internal, String, Error;

(*@@sietse1*)const TStringSize = 2048;
type TString = String (TStringSize);

type
  TChars = packed array [1 .. 1] of Char;
  PChars = ^TChars;

  GPC_PString = ^GPC_String;
  GPC_String (Capacity : Cardinal) = record
    Length : 0 .. Capacity;
    Chars  : packed array [1 .. Capacity + 1] of Char
  end;

  (* Not yet implemented
  StringObject = object { Abstract }
    function  GetCapacity : Integer;           virtual;
    procedure SetLength (NewLength : Integer); virtual;
    function  GetLength : Integer;             virtual;
    function  GetFirstChar : PChars;           virtual;
  end;
  *)

  (*@@ AnyString parameters are not yet implemented, the following is only a draft*)
  AnyStringType = (AnyStringLong,
                   AnyStringUndiscriminated,
                   AnyStringShort,
                   AnyStringFixed,
                   AnyStringCString,
                   AnyStringObject);

  (*@@ only formally for now*)UndiscriminatedString = ^String;

  { Under development. Interface subject to change.
    Use with caution. }
  { When a const or var AnyString parameter is passed, internally
    these records are passed as const parameters. Value AnyString
    parameters are passed like value string parameters. }
  ConstAnyString = record
    Length : Integer;
    Chars  : PChars
  end;

  VarAnyString = record
    Capacity : Integer;
    Chars    : PChars;
    Truncate : Boolean;
  case StringType            : AnyStringType of
    AnyStringLong            : (PLongLength            : ^Cardinal);
    AnyStringUndiscriminated : (PUndiscriminatedString : ^UndiscriminatedString);
    AnyStringShort           : (PShortLength           : ^Byte);
    AnyStringFixed           : (CurrentLength          : Integer);
    (* AnyStringObject          : (PStringObject          : ^StringObject) *)
  end;

  { Capacity is the allocated space (used internally). Count is the
    actual number of environment strings. The CStrings array
    contains the environment strings, terminated by a nil pointer,
    which is not counted in Count. @CStrings can be passed to libc
    routines like execve which expect an environment (see
    GetCEnvironment). }
  PEnvironment = ^TEnvironment;
  TEnvironment (Capacity : Integer) = record
    Count : Integer;
    CStrings : array [1 .. Capacity + 1] of CString
  end;

var
  Environment : PEnvironment = nil; asmname '_p_environment';

procedure GPC_Init_Environment (StartEnvironment : PCStrings); asmname '_p_init_environment';

{ Get an environment variable. If it does not exist, GetEnv returns
  the empty string, which can't be distinguished from a variable
  with an empty value, while CStringGetEnv returns nil then. Note,
  Dos doesn't know empty environment variables, but treats them as
  non-existing, and does not distinguish case in the names of
  environment variables. However, even under Dos, empty environment
  variables and variable names with different case can now be set
  and used within GPC programs. }
function  GetEnv (const EnvVar : String) : TString;                        asmname '_p_getenv';
function  CStringGetEnv (EnvVar : CString) : CString;                      asmname '_p_cstringgetenv';

{ Sets an environment variable with the name given in VarName to the value
  Value. A previous value, if any, is overwritten. }
procedure SetEnv (const VarName, Value : String);                          asmname '_p_setenv';

{ Un-sets an environment variable with the name given in VarName. }
procedure UnSetEnv (const VarName : String);                               asmname '_p_unsetenv';

{ Returns @Environment^.CStrings, converted to PCStrings, to be passed to
  libc routines like execve which expect an environment. }
function  GetCEnvironment : PCStrings;                                     asmname '_p_getcenvironment';

{ Internal routine for Index/Pos }
function  GPC_Index (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Integer; asmname '_p_index';

{ Internal routine for Trim }
function  GPC_Trim (Src : PChar; SrcLength : Integer; Dest : PChar) : Integer; asmname '_p_trim';

{ Internal routine for SubStr/Copy }
function  GPC_SubStr (Src : PChar; SrcLength, aPosition, Count : Integer; Dest : PChar; TruncateFlag : Boolean) : Integer; asmname '_p_substr';

{ Under development }
function VarAnyStringLength (var s : VarAnyString) : Cardinal; asmname '_p_var_anystring_length';
function VarAnyStringSetLength (var s : VarAnyString; NewLength : Cardinal) : Cardinal; asmname '_p_var_anystring_setlength';

procedure GPC_Insert      (const Source: String; var Dest : String;
                           aIndex : Integer; TruncateFlag : Boolean);            asmname '_p_insert';
procedure GPC_Delete      (var s : String; aIndex, Count : Integer);             asmname '_p_delete';

implementation

{$B-,I-,X+}

const
  EnvironmentSizeStep = 16;
  UndiscriminatedStringSizeStep = 64;

procedure GPC_Init_Environment (StartEnvironment : PCStrings);
var i, j : Integer;
begin
  if Environment <> nil then
    begin
      for i := 1 to Environment^.Count do Dispose (Environment^.CStrings [i]);
      Dispose (Environment)
    end;
  i := 0;
  if StartEnvironment <> nil then
    while StartEnvironment^[i] <> nil do Inc (i);
  New (Environment, i);
  Environment^.Count := i;
  for j := 1 to i do
    begin
      Environment^.CStrings [j] := CStringNew (StartEnvironment^[j - 1]);
      {$ifdef __OS_DOS__}
      var Temp : CString = Environment^.CStrings [j];
      while not (Temp^ in [#0, '=']) do
        begin
          Temp^ := UpCase (Temp^);
          Inc (Temp)
        end
      {$endif}
    end;
  Environment^.CStrings [i + 1] := nil
end;

function GetEnvIndex (EnvVar : CString) : Integer;
var l, i : Integer;
begin
  if Environment = nil then return 0;
  l := CStringLength (EnvVar);
  i := 1;
  while (i <= Environment^.Count) and ((MemComp (EnvVar^, Environment^.CStrings [i]^, l) <> 0) or (Environment^.CStrings [i, l] <> '=')) do Inc (i);
  if i > Environment^.Count
    then GetEnvIndex := 0
    else GetEnvIndex := i
end;

function CStringGetEnv (EnvVar : CString) : CString;
var i : Integer;
begin
  i := GetEnvIndex (EnvVar);
  if i = 0
    then CStringGetEnv := nil
    else CStringGetEnv := Environment^.CStrings [i] + CStringLength (EnvVar) + 1
end;

function GetEnv (const EnvVar : String) : TString;
var Temp : CString;
begin
  Temp := CStringGetEnv (EnvVar);
  if Temp = nil then GetEnv := '' else GetEnv := CString2String (Temp)
end;

procedure SetEnv (const VarName, Value : String);
var
  i : Integer;
  NewEnvironment : PEnvironment;
  NewEnvCString : CString;
begin
  i := GetEnvIndex (VarName);
  if i <> 0 then
    Dispose (Environment^.CStrings [i])
  else
    begin
      if Environment^.Count = Environment^.Capacity then
        begin
          New (NewEnvironment, Environment^.Count + EnvironmentSizeStep);
          for i := 1 to Environment^.Count do NewEnvironment^.CStrings [i] := Environment^.CStrings [i];
          NewEnvironment^.Count := Environment^.Count;
          Dispose (Environment);
          Environment := NewEnvironment
        end;
      Inc (Environment^.Count);
      i := Environment^.Count;
      Environment^.CStrings [i + 1] := nil
    end;
  NewEnvCString := NewCString (VarName + '=' + Value);
  Environment^.CStrings [i] := NewEnvCString;
  CSetEnv (VarName, Value, NewEnvCString, False)
end;

procedure UnSetEnv (const VarName : String);
var i, j : Integer;
begin
  i := GetEnvIndex (VarName);
  if i <> 0 then
    with Environment^ do
      begin
        Dispose (CStrings [i]);
        for j := i to Count - 1 do CStrings [j] := CStrings [j + 1];
        CStrings [Count] := nil;
        Dec (Count)
      end;
  CSetEnv (VarName, '', VarName + '=', True)
end;

function GetCEnvironment : PCStrings;
begin
  GetCEnvironment := PCStrings (@Environment^.CStrings)
end;

function GPC_Index (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Integer;
var c : Integer;
begin
  if (Length2 < 0) or (Length1 < 0) then
    InternalError (907); { string length cannot be negative }
  if Length2 = 0 then return 1;
  if Length1 = 0 then return 0;
  if Length1 = 1 then return Ord ((Length2 = 1) and (s1 [0] = s2 [0]));
  if Length2 = 1 then
    begin
      for c := 1 to Length1 do
        if s1 [c - 1] = s2 [0] then return c;
      return 0
    end;
  for c := 1 to Length1 - Length2 + 1 do
    if (s1 [c - 1] = s2 [0]) and
       (s1 [c + Length2 - 2] = s2 [Length2 - 1]) and
       (MemComp (s1 [c], s2 [1], Length2 - 2) = 0) then return c;
  GPC_Index := 0
end;

function GPC_Trim (Src : PChar; SrcLength : Integer; Dest : PChar) : Integer;
var i : Integer;
begin
  i := SrcLength;
  while (i > 0) and (Src [i - 1] = ' ') do Dec (i);
  Move (Src [0], Dest [0], i);
  GPC_Trim := i
end;

function GPC_SubStr (Src : PChar; SrcLength, aPosition, Count : Integer; Dest : PChar; TruncateFlag : Boolean) : Integer;
begin
  GPC_SubStr := 0;
  if aPosition <= 0 then
    if TruncateFlag
      then Exit
      else RuntimeError (801); { Substring cannot start from positions less than 1 }
  if aPosition + Count - 1 > SrcLength then
    if TruncateFlag
      then Count := SrcLength - aPosition + 1
      else RuntimeError (803); { Substring must terminate before end of string }
  if Count < 0 then
    if TruncateFlag
      then Exit
      else RuntimeError (802); { Substring length cannot be negative }
  if Count = 0 then Exit;
  Move (Src [aPosition - 1], Dest [0], Count);
  GPC_SubStr := Count
end;

function VarAnyStringLength (var s : VarAnyString) : Cardinal;
begin
  VarAnyStringLength := 0;
  with s do
    case StringType of
      AnyStringLong            : VarAnyStringLength := PLongLength^;
      AnyStringUndiscriminated : VarAnyStringLength := Length (PUndiscriminatedString^^);
      AnyStringShort           : VarAnyStringLength := PShortLength^;
      AnyStringFixed           : VarAnyStringLength := CurrentLength;
      AnyStringCString         : VarAnyStringLength := CStringLength (CString (Chars));
      (*AnyStringObject          : VarAnyStringLength := PStringObject^.GetLength;*)
    end
end;

function VarAnyStringSetLength (var s : VarAnyString; NewLength : Cardinal) : Cardinal;
begin
  with s do
    begin
      if NewLength > Capacity then
        if Truncate
          then NewLength := Capacity
          else RuntimeError (806); { string too long }
      case StringType of
        AnyStringLong            : PLongLength^ := NewLength;
        AnyStringUndiscriminated : begin
                                     if NewLength > PUndiscriminatedString^^.Capacity then
                                       begin
                                         var Temp : PString;
                                         Temp := PUndiscriminatedString^;
                                         New (PUndiscriminatedString^, (NewLength div UndiscriminatedStringSizeStep + 1) * UndiscriminatedStringSizeStep);
                                         Move (Temp^[1], PUndiscriminatedString^^[1], Length (Temp^));
                                         Dispose (Temp)
                                       end;
                                     GPC_PString (PUndiscriminatedString^)^.Length := NewLength
                                   end;
        AnyStringShort           : PShortLength^ := NewLength;
        AnyStringFixed           : begin
                                     var i : Integer;
                                     for i := NewLength + 1 to CurrentLength do Chars^[i] := ' ';
                                     CurrentLength := NewLength
                                   end;
        AnyStringCString         : Chars^[NewLength + 1] := #0;
        (*AnyStringObject          : PStringObject^.SetLength (NewLength);*)
      end
    end;
  VarAnyStringSetLength := NewLength
end;

(* Not yet implemented
function StringObject.GetCapacity : Integer;
begin
  GetCapacity := 0
end;

procedure StringObject.SetLength (NewLength : Integer);
begin
end;

function StringObject.GetLength : Integer;
begin
  GetLength := 0
end;

function StringObject.GetFirstChar : PChars;
begin
  GetFirstChar := nil
end;
*)

{$CSDefine Length(s) s##_Length}
{$CSDefine Capacity(s) s##_Capacity}
{$CSDefine ConstAnyString(s) s : PChar; Length (s) : Integer}
{$CSDefine VarAnyString(s) s : PChar; Capacity (s), Length (s) : Integer}
{$CSDefine ASCONST(s) @s[1], length (s)}
{$CSDefine ASVAR(s)   @s[1], s.capacity, length (s)}

function Insert_F (ConstAnyString (s); VarAnyString (d);
                   aIndex : Integer; TruncateFlag : Boolean) : Integer;
var Dest : PChar;
begin
  if (aIndex < 1) or (aIndex > Length (d) + 1) or (Length (s) <= 0) then
    Insert_F := Length (d)
  else
    begin
      if Length (s) + Length (d) > Capacity (d) then
        if TruncateFlag then
          begin
            if Length (s) > Capacity (d) - aIndex + 1 then
              Length (s) := Capacity (d) - aIndex + 1;
            Length (d) := Capacity (d) - Length (s)
          end
        else RuntimeError (800); { string too long in `Insert' }
      Dest := d + aIndex - 1;
      MoveRight (Dest [0], Dest [Length (s)], Length (d) - aIndex + 1);
      MoveLeft (s [0], Dest [0], Length (s));
      Insert_F := Length (s) + Length (d)
    end
end;

procedure GPC_Insert (const Source : String; var Dest : String; aIndex : Integer; TruncateFlag : Boolean);
begin
  SetReturnAddress (ReturnAddress (0));
  (*@@*){$local W-} SetLength (Dest, Insert_F (ASCONST (Source), ASVAR (Dest), aIndex, TruncateFlag)); {$endlocal}
  RestoreReturnAddress
end;

function Delete_F (ConstAnyString (s); aIndex, Count : Integer) : Integer;
var Ofs : Integer;
begin
  if aIndex < 1 then
    begin
      Dec (Count, 1 - aIndex);
      aIndex := 1
    end;
  if (Count <= 0) or (aIndex > Length (s)) then
    Delete_F := Length (s)
  else if aIndex + Count > Length (s) then
    Delete_F := aIndex - 1
  else
    begin
      Ofs := aIndex - 1 + Count;
      MoveLeft (s [Ofs], s [aIndex - 1],  Length (s) - Ofs);
      Delete_F := Length (s) - Count
    end
end;

procedure GPC_Delete (var s : String; aIndex, Count : Integer);
begin
  SetLength (s, Delete_F (ASCONST (s), aIndex, Count))
end;

end.
