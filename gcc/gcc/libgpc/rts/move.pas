{
`MoveLeft', `MoveRight' and `Move' transfer procedures.

Copyright (C) 1997-2000 Free Software Foundation, Inc.

Authors: Peter Gerwinski <peter@gerwinski.de>
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

unit Move;

interface

uses Endian;

procedure MoveLeft  (const Source; var Dest; Count : SizeType); asmname '_p_moveleft';
procedure MoveRight (const Source; var Dest; Count : SizeType); asmname '_p_moveright';
procedure Move      (const Source; var Dest; Count : SizeType); asmname '_p_move';

function  MemCmp  (const s1, s2; Size : SizeType) : Integer; asmname 'memcmp';
function  MemComp (const s1, s2; Size : SizeType) : Integer; asmname 'memcmp';

implementation

{$B-,I-}

type
  PByte = ^Byte;
  TWord = MedWord;
  PWord = ^TWord;
  TWords = array [0..7] of TWord;
  PWords = ^TWords;

inline function Merge (w1, w2 : TWord; Shift1, Shift2 : Integer) : TWord;
begin
  if BytesBigEndian
    then Merge := (w1 shl Shift1) or (w2 shr Shift2)
    else Merge := (w1 shr Shift1) or (w2 shl Shift2)
end;

inline procedure Mov (Src, Dst : Pointer; i : PtrDiffType);
begin
  PWords (Dst)^ [i] := PWords (Src)^ [i]
end;

{$X+}

procedure MoveLeft (const Source; var Dest; Count : SizeType);
var
  Src, Dst : PByte;
  SrcWord : PWord absolute Src;
  DstWord : PWord absolute Dst;
  SrcWords : PWords absolute Src;
  DstWords : PWords absolute Dst;
  Align, SrcAlign, Words : SizeType;
  Shift1, Shift2 : Integer;
  LastVal, ThisVal : TWord;
begin
  {$local W-} Src := @Source; {$endlocal}
  Dst := @Dest;
  if (Count >= Sizeof (LongestCard)) and not (Dst - Src in [0 .. Sizeof (TWord) - 1]) then
    begin
      Align := PtrCard (- PtrInt (Dst)) mod AlignOf (TWord);
      Dec (Count, Align);
      while Align > 0 do
        begin
          Dst^ := Src^;
          Inc (Src);
          Inc (Dst);
          Dec (Align)
        end;
      SrcAlign := PtrCard (Src) mod AlignOf (TWord);
      if NeedAlignment and (SrcAlign <> 0) then
        begin
          Dec (Src, SrcAlign);
          Shift1 := SrcAlign * BitSizeOf (Byte);
          Shift2 := BitSizeOf (TWord) - Shift1;
          LastVal := SrcWord^;
          Inc (SrcWord);
          Words := Count div Sizeof (TWord);
          while Words > 0 do
            begin
              ThisVal := SrcWord^;
              DstWord^ := Merge (LastVal, ThisVal, Shift1, Shift2);
              LastVal := ThisVal;
              Inc (SrcWord);
              Inc (DstWord);
              Dec (Words)
            end;
          Dec (SrcWord);
          Inc (Src, SrcAlign);
          Count := Count mod Sizeof (TWord)
        end
      else
        begin
          Words := Count div Sizeof (TWords);
          while Words > 0 do
            begin
              Mov (Src, Dst, 0);
              Mov (Src, Dst, 1);
              Mov (Src, Dst, 2);
              Mov (Src, Dst, 3);
              Mov (Src, Dst, 4);
              Mov (Src, Dst, 5);
              Mov (Src, Dst, 6);
              Mov (Src, Dst, 7);
              Inc (SrcWords);
              Inc (DstWords);
              Dec (Words)
            end;
          Count := Count mod Sizeof (TWords)
        end
    end;
  while Count > 0 do
    begin
      Dst^ := Src^;
      Inc (Src);
      Inc (Dst);
      Dec (Count)
    end
end;

procedure MoveRight (const Source; var Dest; Count : SizeType);
var
  Src, Dst : PByte;
  SrcWord : PWord absolute Src;
  DstWord : PWord absolute Dst;
  SrcWords : PWords absolute Src;
  DstWords : PWords absolute Dst;
  Align, SrcAlign, Words : SizeType;
  Shift1, Shift2 : Integer;
  LastVal, ThisVal : TWord;
begin
  {$local W-} Src := Succ (@Source, Count); {$endlocal}
  Dst := Succ (PByte (@Dest), Count);
  if (Count >= Sizeof (LongestCard)) and not (Src - Dst in [0 .. Sizeof (TWord) - 1]) then
    begin
      Align := PtrCard (Dst) mod AlignOf (TWord);
      Dec (Count, Align);
      while Align > 0 do
        begin
          Dec (Src);
          Dec (Dst);
          Dst^ := Src^;
          Dec (Align)
        end;
      SrcAlign := PtrCard (Src) mod AlignOf (TWord);
      if NeedAlignment and (SrcAlign <> 0) then
        begin
          Dec (Src, SrcAlign);
          Shift1 := SrcAlign * BitSizeOf (Byte);
          Shift2 := BitSizeOf (TWord) - Shift1;
          LastVal := SrcWord^;
          Words := Count div Sizeof (TWord);
          while Words > 0 do
            begin
              Dec (SrcWord);
              Dec (DstWord);
              ThisVal := SrcWord^;
              DstWord^ := Merge (ThisVal, LastVal, Shift1, Shift2);
              LastVal := ThisVal;
              Dec (Words)
            end;
          Inc (Src, SrcAlign);
          Count := Count mod Sizeof (TWord)
        end
      else
        begin
          Words := Count div Sizeof (TWords);
          while Words > 0 do
            begin
              Dec (SrcWords);
              Dec (DstWords);
              Mov (Src, Dst, 7);
              Mov (Src, Dst, 6);
              Mov (Src, Dst, 5);
              Mov (Src, Dst, 4);
              Mov (Src, Dst, 3);
              Mov (Src, Dst, 2);
              Mov (Src, Dst, 1);
              Mov (Src, Dst, 0);
              Dec (Words)
            end;
          Count := Count mod Sizeof (TWords)
        end
    end;
  while Count > 0 do
    begin
      Dec (Src);
      Dec (Dst);
      Dst^ := Src^;
      Dec (Count)
    end
end;

procedure Move (const Source; var Dest; Count : SizeType);
begin
  if PtrCard (@Source) < PtrCard (@Dest)
    then MoveRight (Source, Dest, Count)
    else MoveLeft  (Source, Dest, Count)
end;

end.
