{
Some utility routines for compatibility to some units available for
BP, like some `Turbo Power' units.

@@NOTE - SOME OF THE ROUTINES IN THIS UNIT MAY NOT WORK CORRECTLY.
TEST CAREFULLY AND USE WITH CARE!

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Authors: Dr Abimbola A. Olowofoyeku <African_Chief@bigfoot.com>
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

{$gnu-pascal}
{$if __GPC_RELEASE__ < 20000412}
{$error This unit requires GPC release 20000412 or newer}
{$endif}

unit GPCUtil;

interface

uses GPC;

{ Replace all occurences of OldC with NewC in s and return the
  result }
function  ReplaceChar (const s : String; OldC, NewC : Char) : TString;

{ Return the current working directory }
function  ThisDirectory : TString; asmname '_p_get_current_directory';

{ Does a directory exist? }
function  IsDirectory (const aFileName : String) : Boolean; asmname '_p_directory_exists';

{ Break a string into 2 parts, using Ch as a marker }
function  BreakStr (const Src : String; var Dest1, Dest2 : String; Ch : Char) : Boolean;

{ Convert a CString to an Integer }
function  PChar2Int (s : CString) : Integer;

{ Convert a CString to a LongInt }
function  PChar2Long (s : CString) : LongInt;

{ Convert a CString to a Double }
function  PChar2Double (s : CString) : Double;

{ Search for s as an executable in the path and return its location
  (full pathname) }
function  PathLocate (const s : String) : TString;

{ Copy file Src to Dest and return the number of bytes written }
function  CopyFile (const Src, Dest : String; BufSize : Integer) : LongInt;

{ Copy file Src to Dest and return the number of bytes written;
  report the number of bytes written versus total size of the source
  file }
function  CopyFileEx (const Src, Dest : String; BufSize : Integer;
  function Report (Reached, Total : LongInt) : LongInt) : LongInt;

{ Turbo Power compatibility }

{ Execute the program prog. Dummy1 and Dummy2 are for compatibility
  only; they are ignored. }
function  ExecDos (const Prog : String; Dummy1 : Boolean; Dummy2 : Pointer) : Integer;

{ Return whether Src exists in the path as an executable -- if so
  return its full location in Dest }
function  ExistOnPath (const Src : String; var Dest : String) : Boolean;

{ Does file name s exist? }
function  ExistFile (const aFileName : String) : Boolean; asmname '_p_file_exists';

{ Return just the directory path of Path. Returns DirSelf +
  DirSeparator if Path contains no directory. }
function  JustPathName (const Path : String) : TString; asmname '_p_dir_from_path';

{ Return just the file name part without extension of Path. Empty if
  Path contains no file name. }
function  JustFileName (const Path : String) : TString; asmname '_p_name_from_path';

{ Return just the extension of Path. Empty if Path contains no
  extension. }
function  JustExtension (const Path : String) : TString; asmname '_p_ext_from_path';

{ Change the extension of s to Ext (do not include the dot!) }
function  ForceExtension (const s, Ext : String) : TString;

{ Return the full pathname of Path }
function  FullPathName (const Path : String) : TString; asmname '_p_fexpand';

{ Add a DirSeparator to the end of s if there is not already one }
function  AddBackSlash (const s : String) : TString; asmname '_p_forceadddirseparator';

{ Convert Integer to PChar; uses CStringNew to allocate memory for
  the result, so you must call StrDispose to free the memory later }
function  Int2PChar (i : Integer) : PChar;

{ Convert Integer to string }
function  Int2Str (i : Integer) : TString;

{ Convert string to Integer }
function  Str2Int (const s : String; var i : Integer) : Boolean;

{ Convert string to LongInt }
function  Str2Long (const s : String; var i : LongInt) : Boolean;

{ Convert string to Double }
function  Str2Real (const s : String; var i : Double) : Boolean;

{ Return a string stripped of leading spaces }
function  TrimLead (const s : String) : TString; asmname '_p_trimleft_str';

{ Return a string stripped of trailing spaces }
function  TrimTrail (const s : String) : TString; asmname '_p_trimright_str';

{ Return a string stripped of leading and trailing spaces }
function  Trim (const s : String) : TString; asmname '_p_trimboth_str';

{ Return a string right-padded to length Len with ch }
function  PadCh (const s : String; ch : Char; Len : Integer) : TString;

{ Return a string right-padded to length Len with spaces }
function  Pad (const s : String; Len : Integer) : TString;

{ Return a string left-padded to length Len with ch }
function  LeftPadCh (const s : String; ch : Char; Len : Byte) : TString;

{ Return a string left-padded to length Len with blanks }
function  LeftPad (const s : String; Len : Integer) : TString;

{ Convert a string to lowercase }
function  StLoCase (const s : String) : TString; asmname '_p_locase_str';

{ Convert a string to uppercase }
function  StUpCase (const s : String) : TString; asmname '_p_upcase_str';

{ Uniform access to big memory blocks for GPC and BP. Of course, for
  programs that are meant only for GPC, you can use the usual New/Dispose
  routines. But for programs that should compile with GPC and BP, you can
  use the following routines for GPC. In the GPC unit for BP (gpc-bp.pas),
  you can find emulations for BP that try to provide access to as much
  memory as possible, despite the limitations of BP. The backdraw is that
  this memory cannot be used freely, but only with the following moving
  routines. }

type
  PBigMem = ^TBigMem;
  TBigMem (MaxNumber : Integer) = record
    { Public fields }
    Number, BlockSize : SizeType;
    Mappable : Boolean;
    { Private fields }
    Pointers : array [1 .. MaxNumber] of ^Byte
  end;

{ Note: the number of blocks actually allocated may be smaller than
  WantedNumber. Check the Number field of the result. }
function  AllocateBigMem (WantedNumber, aBlockSize : SizeType; WantMappable : Boolean) : PBigMem;
procedure DisposeBigMem (p : PBigMem);
procedure MoveToBigMem (var Source; p : PBigMem; BlockNumber : SizeType);
procedure MoveFromBigMem (p : PBigMem; BlockNumber : SizeType; var Dest);
{ Maps a big memory block into normal addressable memory and returns
  its address. The memory must have been allocated with
  WantMappable = True. The mapping is only valid until the next
  MapBigMem call. }
function  MapBigMem (p : PBigMem; BlockNumber : SizeType) : Pointer;

implementation

{$B-,I-}

function PathLocate (const s : String) : TString;
begin
  PathLocate := FSearchExecutable (s, GetEnv (PathEnvVar))
end;

function ExistOnPath (const Src : String; var Dest : String) = Existing : Boolean;
begin
  Dest := PathLocate (Src);
  Existing := Dest <> '';
  if Existing then Dest := FExpand (Dest)
end;

function ForceExtension (const s, Ext : String) = Res : TString;
var i : Integer;
begin
  i := LastPos (ExtSeparator, s);
  if (i = 0) or (CharPosFrom (DirSeparators, s, i) <> 0)
    then Res := s
    else Res := Copy (s, 1, i - 1);
  if (Ext <> '') and (Ext [1] <> ExtSeparator) then Res := Res + ExtSeparator;
  Res := Res + Ext
end;

function ExecDos (const Prog : String; Dummy1 : Boolean; Dummy2 : Pointer) : Integer;
var
  Dummy_1 : Boolean;
  Dummy_2 : Pointer;
begin
  Dummy_1 := Dummy1;
  Dummy_2 := Dummy2;
  ExecDos := Execute (Prog)
end;

function PadCh (const s : String; ch : Char; Len : Integer) = Padded : TString;
begin
  Padded := s;
  if Length (Padded) < Len then Padded := Padded + StringOfChar (ch, Len - Length (Padded))
end;

function Pad (const s : String; Len : Integer) : TString;
begin
  Pad := PadCh (s, ' ', Len)
end;

function LeftPadCh (const s : String; ch : Char; Len : Byte) = Padded : TString;
begin
  Padded := s;
  if Length (Padded) < Len then Padded := StringOfChar (ch, Len - Length (Padded)) + Padded
end;

function LeftPad (const s : String; Len : Integer) : TString;
begin
  LeftPad := LeftPadCh (s, ' ', Len)
end;

function Int2PChar (i : Integer) : PChar;
var s : TString;
begin
  Str (i, s);
  Int2PChar := CStringNew (s)
end;

function Int2Str (i : Integer) = s : TString;
begin
  Str (i, s)
end(*@@*)(*$local W-*);(*$endlocal*)

function Str2Int (const s : String; var i : Integer) : Boolean;
var e : Integer;
begin
  Val (s, i, e);
  Str2Int := e = 0
end;

function Str2Long (const s : String; var i : LongInt) : Boolean;
var e : Integer;
begin
  Val (s, i, e);
  Str2Long := e = 0
end;

function Str2Real (const s : String; var i : Double) : Boolean;
var e : Integer;
begin
  Val (s, i, e);
  Str2Real := e = 0
end;

function CopyFile (const Src, Dest : String; BufSize : Integer) : LongInt;
begin
  CopyFile := CopyFileEx (Src, Dest, BufSize, nil)
end;

function CopyFileEx (const Src, Dest : String; BufSize : Integer;
  function Report (Reached, Total : LongInt) : LongInt) = BytesCopied : LongInt;
var
  Size : LongInt;
  Count : Integer;
  SrcFile, DestFile : File;
  Buf : ^Byte;
  B : BindingType;
begin
  Reset (SrcFile, Src, 1);
  if IOResult <> 0 then
    begin
      BytesCopied := - 2;
      Exit
    end;
  Rewrite (DestFile, Dest, 1);
  if IOResult <> 0 then
    begin
      Close (SrcFile);
      BytesCopied := - 3;
      Exit
    end;
  B := Binding (SrcFile);
  Size := FileSize (SrcFile);
  GetMem (Buf, BufSize);
  BytesCopied := 0;
  repeat
    BlockRead (SrcFile, Buf^, BufSize, Count);
    Inc (BytesCopied, Count);
    if IOResult <> 0 then
      BytesCopied := - 100 { Read error }
    else if Count > 0 then
      begin
        BlockWrite (DestFile, Buf^, Count);
        if IOResult <> 0 then
          BytesCopied := - 200 { Write error }
        else if Assigned (Report) and_then (Report (BytesCopied, Size) < 0) then
          BytesCopied := - 300 { User Abort }
      end
  until (BytesCopied < 0) or (Count = 0);
  FreeMem (Buf);
  Close (SrcFile);
  if BytesCopied >= 0 then
    begin
      SetFileTime ((*@@AnyFile*)AnyFile( DestFile), B.ModificationTime);
      ChMod ((*@@AnyFile*)AnyFile( DestFile), B.Mode)
    end;
  Close (DestFile)
end;

function BreakStr (const Src : String; var Dest1, Dest2 : String; Ch : Char) : Boolean;
var i : Integer;
begin
  i := Pos (Ch, Src);
  BreakStr := i <> 0;
  if i = 0 then i := Length (Src) + 1;
  Dest1 := Trim (Copy (Src, 1, i - 1));
  Dest2 := Trim (Copy (Src, i + 1))
end;

{$local X+}
function PChar2Int (s : CString) = i : Integer;
begin
  ReadStr (s, i)
end;

function PChar2Long (s : CString) = i : LongInt;
begin
  ReadStr (s, i)
end;

function PChar2Double (s : CString) = x : Double;
begin
  ReadStr (s, x)
end;
{$endlocal}

function ReplaceChar (const s : String; OldC, NewC : Char) = Res : TString;
var i : Integer;
begin
  Res := s;
  if OldC <> NewC then
    for i := 1 to Length (Res) do
      if Res [i] = OldC then Res [i] := NewC
end;

function AllocateBigMem (WantedNumber, aBlockSize : SizeType; WantMappable : Boolean) = p : PBigMem;
begin
  New (p, WantedNumber);
  with p^ do
    begin
      Mappable := WantMappable;
      BlockSize := aBlockSize;
      Number := 0;
      while Number < WantedNumber do
        begin
          Pointers [Number + 1] := CGetMem (BlockSize);
          if Pointers [Number + 1] = nil then Break;
          Inc (Number)
        end
    end
end;

procedure DisposeBigMem (p : PBigMem);
var i : Integer;
begin
  for i := 1 to p^.Number do CFreeMem (p^.Pointers [i]);
  Dispose (p)
end;

procedure MoveToBigMem (var Source; p : PBigMem; BlockNumber : SizeType);
begin
  Move (Source, p^.Pointers [BlockNumber]^, p^.BlockSize)
end;

procedure MoveFromBigMem (p : PBigMem; BlockNumber : SizeType; var Dest);
begin
  Move (p^.Pointers [BlockNumber]^, Dest, p^.BlockSize)
end;

function MapBigMem (p : PBigMem; BlockNumber : SizeType) : Pointer;
begin
  if not p^.Mappable then RuntimeError (857); { attempt to map unmappable memory }
  MapBigMem := p^.Pointers [BlockNumber]
end;

end.
