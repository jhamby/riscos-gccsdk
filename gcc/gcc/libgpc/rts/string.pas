{
String handling routines, lower level part

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

unit String;

interface

uses Internal;

type
  AnyFile = Text; (*@@ create `AnyFile' parameters*)
  PAnyFile = ^AnyFile;

{ TString is a string type that is used for function results and
  local variables, as long as undiscriminated strings are not
  allowed there. The default size of 2048 characters should be
  enough for file names on any system, but can be changed when
  necessary. It should be at least as big as MAXPATHLEN. }

const
  TStringSize = 2048;
  SpaceCharacters = [' ', #9];
  NewLine = "\n"; { the separator of lines within a string }
  LineBreak = {$ifdef __OS_DOS__} "\r\n" {$else} "\n" {$endif}; { the separator of lines within a file }

type
  TString    = String (TStringSize);
  TStringBuf = packed array [0 .. TStringSize] of Char;
  PString    = ^String;
  CharSet    = set of Char;
  PCStrings  = ^TCStrings;
  TCStrings  = array [0 .. MaxVarSize div SizeOf (CString)] of CString;

var
  CParamCount : Integer = 1; asmname '_p_argc';
  CParametersDummy : array [0 .. 1] of CString = ('GPC program', nil);
  CParameters : PCStrings = PCStrings (@CParametersDummy); asmname '_p_argv';

function  MemCmp      (const s1, s2; Size : SizeType) : Integer; asmname 'memcmp';
function  MemComp     (const s1, s2; Size : SizeType) : Integer; asmname 'memcmp';
function  MemCompCase (const s1, s2; Size : SizeType) : Boolean; asmname '_p_memcmpcase';

function  GPC_UpCase      (ch : Char) : Char;                                    attribute (const); asmname '_p_gpc_upcase';
function  GPC_LoCase      (ch : Char) : Char;                                    attribute (const); asmname '_p_gpc_locase';
function  BP_UpCase       (ch : Char) : Char;                                    attribute (const); asmname '_p_bp_upcase';
function  BP_LoCase       (ch : Char) : Char;                                    attribute (const); asmname '_p_bp_locase';

procedure UpCaseString    (var s : String);                                      asmname '_p_upcase_string';
procedure LoCaseString    (var s : String);                                      asmname '_p_locase_string';
function  UpCaseStr       (const s : String) : TString;                          asmname '_p_upcase_str';
function  LoCaseStr       (const s : String) : TString;                          asmname '_p_locase_str';

function  IsUpCase        (ch : Char) : Boolean;                                 attribute (const); asmname '_p_isupcase';
function  IsLoCase        (ch : Char) : Boolean;                                 attribute (const); asmname '_p_islocase';
function  IsAlphaNum      (ch : Char) : Boolean;                                 attribute (const); asmname '_p_isalphanum';
function  IsAlphaNumUnderscore (ch : Char) : Boolean;                            attribute (const); asmname '_p_isalphanumunderscore';
function  IsPrintable     (ch : Char) : Boolean;                                 attribute (const); asmname '_p_isprintable';

function  StrEqualCase    (const s1, s2 : String) : Boolean;                     asmname '_p_strequalcase';

function  Pos             (const SubString, aString : String) : Integer;             asmname '_p_pos';
function  LastPos         (const SubString, aString : String) : Integer;             asmname '_p_lastpos';
function  PosCase         (const SubString, aString : String) : Integer;             asmname '_p_poscase';
function  LastPosCase     (const SubString, aString : String) : Integer;             asmname '_p_lastposcase';
function  CharPos         (const Chars : CharSet; const aString : String) : Integer; asmname '_p_charpos';
function  LastCharPos     (const Chars : CharSet; const aString : String) : Integer; asmname '_p_lastcharpos';

function  PosFrom         (const SubString, aString : String; From : Integer) : Integer;             asmname '_p_posfrom';
function  LastPosTill     (const SubString, aString : String; Till : Integer) : Integer;             asmname '_p_lastpostill';
function  PosFromCase     (const SubString, aString : String; From : Integer) : Integer;             asmname '_p_posfromcase';
function  LastPosTillCase (const SubString, aString : String; Till : Integer) : Integer;             asmname '_p_lastpostillcase';
function  CharPosFrom     (const Chars : CharSet; const aString : String; From : Integer) : Integer; asmname '_p_charposfrom';
function  LastCharPosTill (const Chars : CharSet; const aString : String; Till : Integer) : Integer; asmname '_p_lastcharpostill';

function  IsPrefix        (const Prefix, s : String) : Boolean;                     asmname '_p_isprefix';
function  IsSuffix        (const Suffix, s : String) : Boolean;                     asmname '_p_issuffix';
function  IsPrefixCase    (const Prefix, s : String) : Boolean;                     asmname '_p_isprefixcase';
function  IsSuffixCase    (const Suffix, s : String) : Boolean;                     asmname '_p_issuffixcase';

function  CStringLength      (Src : CString) : SizeType;                            asmname '_p_strlen';
function  CStringEnd         (Src : CString) : CString;                             asmname '_p_strend';
function  CStringNew         (Src : CString) : CString;                             asmname '_p_strdup';
function  CStringComp        (s1, s2 : CString) : Integer;                          asmname '_p_strcmp';
function  CStringCaseComp    (s1, s2 : CString) : Integer;                          asmname '_p_strcasecmp';
function  CStringLComp       (s1, s2 : CString; MaxLen : SizeType) : Integer;       asmname '_p_strlcmp';
function  CStringLCaseComp   (s1, s2 : CString; MaxLen : SizeType) : Integer;       asmname '_p_strlcasecmp';
function  CStringCopy        (Dest, Source : CString) : CString;                    asmname '_p_strcpy';
function  CStringCopyEnd     (Dest, Source : CString) : CString;                    asmname '_p_strecpy';
function  CStringLCopy       (Dest, Source : CString; MaxLen : SizeType) : CString; asmname '_p_strlcpy';
function  CStringMove        (Dest, Source : CString; Count : SizeType) : CString;  asmname '_p_strmove';
function  CStringCat         (Dest, Source : CString) : CString;                    asmname '_p_strcat';
function  CStringLCat        (Dest, Source : CString; MaxLen : SizeType) : CString; asmname '_p_strlcat';
function  CStringCharPos     (Src : CString; Ch : Char) : CString;                  asmname '_p_strscan';
function  CStringLastCharPos (Src : CString; Ch : Char) : CString;                  asmname '_p_strrscan';
function  CStringPos         (aString, SubString : CString) : CString;              asmname '_p_strpos';
function  CStringLastPos     (aString, SubString : CString) : CString;              asmname '_p_strrpos';
function  CStringCasePos     (aString, SubString : CString) : CString;              asmname '_p_strcasepos';
function  CStringLastCasePos (aString, SubString : CString) : CString;              asmname '_p_strrcasepos';
function  CStringUpCase      (s : CString) : CString;                               asmname '_p_strupper';
function  CStringLoCase      (s : CString) : CString;                               asmname '_p_strlower';
function  CStringIsEmpty     (s : CString) : Boolean;                               asmname '_p_strempty';
function  NewCString         (const Source : String) : CString;                     asmname '_p_newcstring';
function  CStringCopyString  (Dest : CString; const Source : String) : CString;     asmname '_p_cstringcopystring';
procedure CopyCString        (Source : CString; var Dest : String);                 asmname '_p_copycstring';

function  NewString       (const s : String) : PString;                          asmname '_p_newstring';
procedure DisposeString   (p : PString);                                         asmname '_p_dispose';

procedure SetString       (var s : String; Buffer : PChar; Count : Integer);     asmname '_p_set_string';
function  StringOfChar    (Ch : Char; Count : Integer) = s : TString;            asmname '_p_string_of_char';

procedure TrimLeft        (var s : String);                                      asmname '_p_trimleft';
procedure TrimRight       (var s : String);                                      asmname '_p_trimright';
procedure TrimBoth        (var s : String);                                      asmname '_p_trimboth';
function  TrimLeftStr     (const s : String) : TString;                          asmname '_p_trimleft_str';
function  TrimRightStr    (const s : String) : TString;                          asmname '_p_trimright_str';
function  TrimBothStr     (const s : String) : TString;                          asmname '_p_trimboth_str';

function  GetStringCapacity (const s : String) : Integer;                        asmname '_p_get_string_capacity';

{ A shortcut for a common use of WriteStr as a function }
function  Integer2String (i : Integer) : TString;                                asmname '_p_Integer2String';

{ Internal routines for the string operators }

{ Compare strings for equality without padding }
function  StrEQ (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean;    asmname '_p_eq';

{ Compare strings for `less-than' without padding }
function  StrLT (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean;    asmname '_p_lt';

{ Compare strings for equality, padding the shorter string with spaces }
function  StrEQPad (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean; asmname '_p_str_eq';

{ Compare strings for `less-than', padding the shorter string with spaces }
function  StrLTPad (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean; asmname '_p_str_lt';

implementation

{$B-,I-,X+}

(*@@inline*) function BP_UpCase (ch : Char) : Char;
begin
  if ch in ['a' .. 'z']
    then BP_UpCase := Pred (ch, Ord ('a') - Ord ('A'))
    else BP_UpCase := ch
end;

(*@@inline*) function BP_LoCase (ch : Char) : Char;
begin
  if ch in ['A' .. 'Z']
    then BP_LoCase := Succ (ch, Ord ('a') - Ord ('A'))
    else BP_LoCase := ch
end;

procedure UpCaseString (var s : String);
var i : Integer;
begin
  for i := 1 to Length (s) do s [i] := UpCase (s [i])
end;

procedure LoCaseString (var s : String);
var i : Integer;
begin
  for i := 1 to Length (s) do s [i] := LoCase (s [i])
end;

function UpCaseStr (const s : String) = Result : TString;
begin
  Result := s;
  UpCaseString (Result)
end;

function LoCaseStr (const s : String) = Result : TString;
begin
  Result := s;
  LoCaseString (Result)
end;

function IsAlphaNumUnderscore (ch : Char) : Boolean;
begin
  IsAlphaNumUnderscore := (ch = '_') or IsAlphaNum (ch)
end;

function MemCompCase (const s1, s2; Size : SizeType) : Boolean;
var
  i : Integer;
  a1 : array [1 .. Size] of Char absolute s1;
  a2 : array [1 .. Size] of Char absolute s2;
begin
  for i := 1 to Size do
    if (a1 [i] <> a2 [i]) and (LoCase (a1 [i]) <> LoCase (a2 [i])) then return False;
  MemCompCase := True
end;

function StrEqualCase (const s1, s2 : String) : Boolean;
begin
  if Length (s1) <> Length (s2)
    then StrEqualCase := False
    else StrEqualCase := MemCompCase (s1 [1], s2 [1], Length (s1))
end;

function Pos (const SubString, aString : String) : Integer;
begin
  Pos := PosFrom (SubString, aString, 1)
end;

function LastPos (const SubString, aString : String) : Integer;
begin
  LastPos := LastPosTill (SubString, aString, Length (aString))
end;

function PosCase (const SubString, aString : String) : Integer;
begin
  PosCase := PosFromCase (SubString, aString, 1)
end;

function LastPosCase (const SubString, aString : String) : Integer;
begin
  LastPosCase := LastPosTillCase (SubString, aString, Length (aString))
end;

function CharPos (const Chars : CharSet; const aString : String) : Integer;
var i : Integer;
begin
  i := 1;
  while (i <= Length (aString)) and not (aString [i] in Chars) do Inc (i);
  if i > Length (aString) then CharPos := 0 else CharPos := i
end;

function LastCharPos (const Chars : CharSet; const aString : String) : Integer;
var i : Integer;
begin
  i := Length (aString);
  while (i > 0) and not (aString [i] in Chars) do Dec (i);
  LastCharPos := i
end;

function PosFrom (const SubString, aString : String; From : Integer) : Integer;
var m, i, n : Integer;
begin
  m := Max (1, From);
  case Length (SubString) of
    0: PosFrom := From;
    1: begin
         i := m;
         while (i <= Length (aString)) and (aString [i] <> SubString [1]) do Inc (i);
         if i > Length (aString) then PosFrom := 0 else PosFrom := i
       end;
    else
      n := Length (aString) - Length (SubString) + 1;
      i := m;
      while (i <= n) and (MemComp (aString [i], SubString [1], Length (SubString)) <> 0) do Inc (i);
      if i > n then PosFrom := 0 else PosFrom := i
  end
end;

function LastPosTill (const SubString, aString : String; Till : Integer) : Integer;
var m, i : Integer;
begin
  m := Max (0, Min (Length (aString), Till));
  case Length (SubString) of
    0: LastPosTill := m + 1;
    1: begin
         i := m;
         while (i > 0) and (aString [i] <> SubString [1]) do Dec (i);
         LastPosTill := i
       end;
    else
      i := m - Length (SubString) + 1;
      while (i > 0) and (MemComp (aString [i], SubString [1], Length (SubString)) <> 0) do Dec (i);
      if i < 0 then LastPosTill := 0 else LastPosTill := i
  end
end;

function PosFromCase (const SubString, aString : String; From : Integer) : Integer;
var m, i, n : Integer;
begin
  m := Max (1, From);
  case Length (SubString) of
    0: PosFromCase := From;
    1: begin
         i := m;
         while (i <= Length (aString)) and (aString [i] <> SubString [1]) and (LoCase (aString [i]) <> LoCase (SubString [1])) do Inc (i);
         if i > Length (aString) then PosFromCase := 0 else PosFromCase := i
       end;
    else
      n := Length (aString) - Length (SubString) + 1;
      i := m;
      while (i <= n) and not MemCompCase (aString [i], SubString [1], Length (SubString)) do Inc (i);
      if i > n then PosFromCase := 0 else PosFromCase := i
  end
end;

function LastPosTillCase (const SubString, aString : String; Till : Integer) : Integer;
var m, i : Integer;
begin
  m := Max (0, Min (Length (aString), Till));
  case Length (SubString) of
    0: LastPosTillCase := m + 1;
    1: begin
         i := m;
         while (i > 0) and (aString [i] <> SubString [1]) and (LoCase (aString [i]) <> LoCase (SubString [1])) do Dec (i);
         LastPosTillCase := i
       end;
    else
      i := m - Length (SubString) + 1;
      while (i > 0) and not MemCompCase (aString [i], SubString [1], Length (SubString)) do Dec (i);
      if i < 0 then LastPosTillCase := 0 else LastPosTillCase := i
  end
end;

function CharPosFrom (const Chars : CharSet; const aString : String; From : Integer) : Integer;
var i : Integer;
begin
  i := Max (1, From);
  while (i <= Length (aString)) and not (aString [i] in Chars) do Inc (i);
  if i > Length (aString) then CharPosFrom := 0 else CharPosFrom := i
end;

function LastCharPosTill (const Chars : CharSet; const aString : String; Till : Integer) : Integer;
var i : Integer;
begin
  i := Max (0, Min (Length (aString), Till));
  while (i > 0) and not (aString [i] in Chars) do Dec (i);
  LastCharPosTill := i
end;

function IsPrefix (const Prefix, s : String) : Boolean;
begin
  (*@@fjf226*)if not (Length (s) >= Length (Prefix)) then IsPrefix := False
    else IsPrefix := EQ (s [1 .. Length (Prefix)], Prefix)
end;

function IsSuffix (const Suffix, s : String) : Boolean;
begin
  (*@@fjf226*)if not (Length (s) >= Length (Suffix)) then IsSuffix := False
    else IsSuffix := EQ (s [Length (s) - Length (Suffix) + 1 .. Length (s)], Suffix)
end;

function IsPrefixCase (const Prefix, s : String) : Boolean;
begin
  (*@@fjf226*)if not (Length (s) >= Length (Prefix)) then IsPrefixCase := False
    else IsPrefixCase := StrEqualCase (s [1 .. Length (Prefix)], Prefix)
end;

function IsSuffixCase (const Suffix, s : String) : Boolean;
begin
  (*@@fjf226*)if not (Length (s) >= Length (Suffix)) then IsSuffixCase := False
    else IsSuffixCase := StrEqualCase (s [Length (s) - Length (Suffix) + 1 .. Length (s)], Suffix)
end;

(*@@inline*) function CStringLength (Src : CString) : SizeType;
var Temp : CString;
begin
  if Src = nil then return 0;
  Temp := Src;
  while Temp^ <> #0 do Inc (Temp);
  CStringLength := Temp - Src
end;

(*@@inline*) function CStringEnd (Src : CString) : CString;
var Temp : CString;
begin
  if Src = nil then return nil;
  Temp := Src;
  while Temp^ <> #0 do Inc (Temp);
  CStringEnd := Temp
end;

function CStringNew (Src : CString) : CString;
var
  Size : SizeType;
  Dest : CString;
begin
  if Src = nil then return nil;
  Size := CStringLength (Src) + 1;
  GetMem (Dest, Size);
  Move (Src^, Dest^, Size);
  CStringNew := Dest
end;

function CStringLComp (s1, s2 : CString; MaxLen : SizeType) : Integer;
var c1, c2: Char;
begin
  if s1 = nil then
    if (s2 = nil) or (s2^ = #0)
      then CStringLComp := 0
      else CStringLComp := -1
  else if s2 = nil then
    if s1^ = #0
      then CStringLComp := 0
      else CStringLComp := 1
  else
    begin
      if MaxLen > 0 then
        repeat
          c1 := s1^;
          c2 := s2^;
          Inc (s1);
          Inc (s2);
          if c1 <> c2 then return Ord (c1) - Ord (c2);
          Dec (MaxLen)
        until (c1 = #0) or (MaxLen = 0);
      CStringLComp := 0
    end
end;

function CStringComp (s1, s2 : CString) : Integer;
begin
  CStringComp := CStringLComp (s1, s2, MaxInt)
end;

function CStringLCaseComp (s1, s2 : CString; MaxLen : SizeType) : Integer;
var c1, c2: Char;
begin
  if s1 = nil then
    if (s2 = nil) or (s2^ = #0)
      then CStringLCaseComp := 0
      else CStringLCaseComp := -1
  else if s2 = nil then
    if s1^ = #0
      then CStringLCaseComp := 0
      else CStringLCaseComp := 1
  else
    begin
      if MaxLen > 0 then
        repeat
          c1 := LoCase (s1^);
          c2 := LoCase (s2^);
          Inc (s1);
          Inc (s2);
          if c1 <> c2 then return Ord (c1) - Ord (c2);
          Dec (MaxLen)
        until (c1 = #0) or (MaxLen = 0);
      CStringLCaseComp := 0
    end
end;

function CStringCaseComp (s1, s2 : CString) : Integer;
begin
  CStringCaseComp := CStringLCaseComp (s1, s2, MaxInt)
end;

function CStringCopy (Dest, Source : CString) : CString;
var Size : SizeType;
begin
  if Source = nil then
    Size := 0
  else
    begin
      Size := CStringLength (Source);
      Move (Source^, Dest^, Size)
    end;
  Dest [Size] := #0;
  CStringCopy := Dest
end;

function CStringCopyEnd (Dest, Source : CString) : CString;
var Size : SizeType;
begin
  if Source = nil then
    Size := 0
  else
    begin
      Size := CStringLength (Source);
      Move (Source^, Dest^, Size)
    end;
  Dest [Size] := #0;
  CStringCopyEnd := Dest + Size
end;

function CStringLCopy (Dest, Source : CString; MaxLen : SizeType) : CString;
var Size : SizeType;
begin
  if Source = nil then
    Size := 0
  else
    begin
      Size := Min (CStringLength (Source), MaxLen);
      Move (Source^, Dest^, Size)
    end;
  Dest [Size] := #0;
  CStringLCopy := Dest
end;

function CStringMove (Dest, Source : CString; Count : SizeType) : CString;
begin
  if Source = nil then
    FillChar (Dest^, Count, 0)
  else
    Move (Source^, Dest^, Count);
  CStringMove := Dest
end;

function CStringCat (Dest, Source : CString) : CString;
begin
  CStringCopy (CStringEnd (Dest), Source);
  CStringCat := Dest
end;

function CStringLCat (Dest, Source : CString; MaxLen : SizeType) : CString;
var s : SizeType;
begin
  s := CStringLength (Dest);
  CStringLCopy (Dest + s, Source, Max (MaxLen, s) - s);
  CStringLCat := Dest
end;

(*@@inline*) function CStringCharPos (Src : CString; Ch : Char) : CString;
var Temp : CString;
begin
  if Src = nil then return nil;
  Temp := Src;
  while (Temp^ <> #0) and (Temp^ <> Ch) do Inc (Temp);
  if Temp^ = Ch then CStringCharPos := Temp else CStringCharPos := nil
end;

(*@@inline*) function CStringLastCharPos (Src : CString; Ch : Char) : CString;
var Temp : CString;
begin
  if Src = nil then return nil;
  Temp := CStringEnd (Src);
  while (Temp <> Src) and (Temp^ <> Ch) do Dec (Temp);
  if Temp^ = Ch then CStringLastCharPos := Temp else CStringLastCharPos := nil
end;

function CStringPos (aString, SubString : CString) : CString;
var
  Temp : CString;
  s : SizeType;
begin
  if (aString = nil) or (SubString = nil) then return aString;
  s := CStringLength (SubString);
  Temp := aString;
  while Temp^ <> #0 do
    begin
      if CStringLComp (Temp, SubString, s) = 0 then return Temp;
      Inc (Temp)
    end;
  CStringPos := nil
end;

function CStringLastPos (aString, SubString : CString) : CString;
var
  Temp : CString;
  s : SizeType;
begin
  if (aString = nil) or (SubString = nil) then return aString;
  s := CStringLength (SubString);
  Temp := CStringEnd (aString);
  while Temp >= aString do
    begin
      if CStringLComp (Temp, SubString, s) = 0 then return Temp;
      Dec (Temp)
    end;
  CStringLastPos := nil
end;

function CStringCasePos (aString, SubString : CString) : CString;
var
  Temp : CString;
  s : SizeType;
begin
  if (aString = nil) or (SubString = nil) then return aString;
  s := CStringLength (SubString);
  Temp := aString;
  while Temp^ <> #0 do
    begin
      if CStringLCaseComp (Temp, SubString, s) = 0 then return Temp;
      Inc (Temp)
    end;
  CStringCasePos := nil
end;

function CStringLastCasePos (aString, SubString : CString) : CString;
var
  Temp : CString;
  s : SizeType;
begin
  if (aString = nil) or (SubString = nil) then return aString;
  s := CStringLength (SubString);
  Temp := CStringEnd (aString);
  while Temp >= aString do
    begin
      if CStringLCaseComp (Temp, SubString, s) = 0 then return Temp;
      Dec (Temp)
    end;
  CStringLastCasePos := nil
end;

function CStringUpCase (s : CString) : CString;
var Temp : CString;
begin
  Temp := s;
  if Temp <> nil then
    while Temp^ <> #0 do
      begin
        Temp^ := UpCase (Temp^);
        Inc (Temp)
      end;
  CStringUpCase := s
end;

function CStringLoCase (s : CString) : CString;
var Temp : CString;
begin
  Temp := s;
  if Temp <> nil then
    while Temp^ <> #0 do
      begin
        Temp^ := LoCase (Temp^);
        Inc (Temp)
      end;
  CStringLoCase := s
end;

function CStringIsEmpty (s : CString) : Boolean;
begin
  CStringIsEmpty := (s = nil) or (s^ = #0)
end;

function NewCString (const Source : String) : CString;
var Dest : CString;
begin
  GetMem (Dest, Length (Source) + 1);
  MoveLeft (Source [1], Dest [0], Length (Source));
  Dest [Length (Source)] := #0;
  NewCString := Dest
end;

function CStringCopyString (Dest : CString; const Source : String) : CString;
begin
  MoveLeft (Source [1], Dest [0], Length (Source));
  Dest [Length (Source)] := #0;
  CStringCopyString := Dest
end;

procedure CopyCString (Source : CString; var Dest : String);
var Source_Length : SizeType;
begin
  if Source = nil then
    SetLength (Dest, 0)
  else
    begin
      Source_Length := Min (CStringLength (Source), Dest.Capacity);
      SetLength (Dest, Source_Length);
      MoveLeft (Source [0], Dest [1], Source_Length)
    end
end;

function NewString (const s : String) = Result : PString;
begin
  New (Result, Length (s));
  Result^ := s
end;

procedure SetString (var s : String; Buffer : PChar; Count : Integer);
var i : Integer;
begin
  SetLength (s, Min (GetStringCapacity (s), Max (0, Min (Count, CStringLength (Buffer)))));
  if Buffer <> nil then
    for i := 1 to (*@@return value of SetLength*)Length (s) do s [i] := Buffer [i - 1]
end;

function StringOfChar (Ch : Char; Count : Integer) = s : TString;
var i : Integer;
begin
  SetLength (s, Min (GetStringCapacity (s), Max (0, Count)));
  for i := 1 to (*@@return value of SetLength*)Length (s) do s [i] := Ch
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
  Delete (s, i + 1, Length (s) - i)
end;

procedure TrimBoth (var s : String);
begin
  TrimLeft (s);
  TrimRight (s)
end;

function TrimLeftStr (const s : String) = Result : TString;
begin
  Result := s;
  TrimLeft (Result)
end;

function TrimRightStr (const s : String) = Result : TString;
begin
  Result := s;
  TrimRight (Result)
end;

function TrimBothStr (const s : String) = Result : TString;
begin
  Result := s;
  TrimBoth (Result)
end;

function GetStringCapacity (const s : String) : Integer;
begin
  GetStringCapacity := s.Capacity
end;

function Integer2String (i : Integer) = s : TString;
begin
  WriteStr (s, i)
end;

function StrEQ (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean;
begin
  StrEq := (Length1 = Length2) and (MemComp (s1 [0], s2 [0], Length1) = 0)
end;

function StrLT (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean;
begin
  if Length1 < Length2
    then StrLT := MemComp (s1 [0], s2 [0], Length1) <= 0
    else StrLT := MemComp (s1 [0], s2 [0], Length2) < 0
end;

function StrEQPad (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) = Result : Boolean;
var
  s : PChar;
  l, c : Integer;
begin
  if Length1 > Length2 then
    begin
      s := s1; s1 := s2; s2 := s;
      l := Length1; Length1 := Length2; Length2 := l
    end;
  Result := MemComp (s1 [0], s2 [0], Length1) = 0;
  if Result and (Length1 <> Length2) then
    for c := Length1 to Length2 - 1 do
      if s2 [c] <> ' ' then return False
end;

function StrLTPad (s1 : PChar; Length1 : Integer; s2 : PChar; Length2 : Integer) : Boolean;
var
  sLong, sShort : PChar;
  lLong, lShort, c, r : Integer;
  s1IsLonger : Boolean;
begin
  s1IsLonger := Length1 > Length2;
  if s1IsLonger then
    begin
      sLong  := s1;
      sShort := s2;
      lLong  := Length1;
      lShort := Length2
    end
  else
    begin
      sLong  := s2;
      sShort := s1;
      lLong  := Length2;
      lShort := Length1
    end;
  r := MemComp (s1 [0], s2 [0], lShort);
  if (r <> 0) or (Length1 = Length2) then return r < 0;
  for c := lShort to lLong - 1 do
    if sLong [c] <> ' ' then
      if s1IsLonger
        then return sLong [c] < ' '
        else return sLong [c] > ' ';
  StrLTPad := False
end;

end.
