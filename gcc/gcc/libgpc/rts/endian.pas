{
Routines to handle endianness

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

unit Endian;

interface

(*@@fjf252.pas*)type sizetype=cardinal;

{ Boolean constants about endianness and alignment }

const
  BitsBigEndian  = {$ifdef __BITS_LITTLE_ENDIAN__} False
                   {$else}{$ifdef __BITS_BIG_ENDIAN__} True
                   {$else}{$error Bit endianness is not defined!}
                   {$endif}{$endif};

  BytesBigEndian = {$ifdef __BYTES_LITTLE_ENDIAN__} False
                   {$else}{$ifdef __BYTES_BIG_ENDIAN__} True
                   {$else}{$error Byte endianness is not defined!}
                   {$endif}{$endif};

  WordsBigEndian = {$ifdef __WORDS_LITTLE_ENDIAN__} False
                   {$else}{$ifdef __WORDS_BIG_ENDIAN__} True
                   {$else}{$error Word endianness is not defined!}
                   {$endif}{$endif};

  NeedAlignment  = {$ifdef __NEED_ALIGNMENT__} True
                   {$else} False {$endif};

{ Convert single variables from or to little or big endian format.
  This only works for a single variable or a plain array of a simple
  type. For more complicated structures, this has to be done for
  each component separately! Currently, ConvertFromFooEndian and
  ConvertToFooEndian are the same, but this might not be the case on
  middle-endian machines. Therefore, we provide different names. }
procedure ReverseBytes            (var Buf; ElementSize, Count : SizeType); asmname '_p_reversebytes';
procedure ConvertFromLittleEndian (var Buf; ElementSize, Count : SizeType); asmname '_p_convertlittleendian';
procedure ConvertFromBigEndian    (var Buf; ElementSize, Count : SizeType); asmname '_p_convertbigendian';
procedure ConvertToLittleEndian   (var Buf; ElementSize, Count : SizeType); asmname '_p_convertlittleendian';
procedure ConvertToBigEndian      (var Buf; ElementSize, Count : SizeType); asmname '_p_convertbigendian';

{ Read a block from a file and convert it from little or
  big endian format. This only works for a single variable or a
  plain array of a simple type, note the comment for
  `ConvertFromLittleEndian' and `ConvertFromBigEndian'. }
(*@@iocritical*)procedure BlockReadLittleEndian   (var aFile : File; var   Buf; ElementSize, Count : SizeType); asmname '_p_blockread_littleendian';
(*@@iocritical*)procedure BlockReadBigEndian      (var aFile : File; var   Buf; ElementSize, Count : SizeType); asmname '_p_blockread_bigendian';

{ Write a block variable to a file and convert it to little or big
  endian format before. This only works for a single variable or a
  plain array of a simple type. Apart from this, note the comment
  for `ConvertToLittleEndian' and `ConvertToBigEndian'. }
(*@@iocritical*)procedure BlockWriteLittleEndian  (var aFile : File; const Buf; ElementSize, Count : SizeType); asmname '_p_blockwrite_littleendian';
(*@@iocritical*)procedure BlockWriteBigEndian     (var aFile : File; const Buf; ElementSize, Count : SizeType); asmname '_p_blockwrite_bigendian';

{ Read and write strings from/to binary files, where the length is
  stored in the given endianness and with a fixed size (64 bits),
  and therefore is independent of the system. }
(*@@iocritical*)procedure ReadStringLittleEndian  (var f : File; var s : String);   asmname '_p_ReadStringLittleEndian';
(*@@iocritical*)procedure ReadStringBigEndian     (var f : File; var s : String);   asmname '_p_ReadStringBigEndian';
(*@@iocritical*)procedure WriteStringLittleEndian (var f : File; const s : String); asmname '_p_WriteStringLittleEndian';
(*@@iocritical*)procedure WriteStringBigEndian    (var f : File; const s : String); asmname '_p_WriteStringBigEndian';

implementation

{$B-,I-}

procedure ReverseBytes (var Buf; ElementSize, Count : SizeType);
var
  i, j, o1, o2 : SizeType;
  b : Byte;
  ByteBuf : array [1 .. ElementSize * Count] of Byte absolute Buf;
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
var TempBuf : array [1 .. ElementSize * Count] of Byte;
begin
  Move (Buf, TempBuf, ElementSize * Count);
  ConvertToLittleEndian (TempBuf, ElementSize, Count);
  BlockWrite (aFile, TempBuf, ElementSize * Count)
end;

procedure BlockWriteBigEndian (var aFile : File; const Buf; ElementSize, Count : SizeType);
var TempBuf : array [1 .. ElementSize * Count] of Byte;
begin
  Move (Buf, TempBuf, ElementSize * Count);
  ConvertToBigEndian (TempBuf, ElementSize, Count);
  BlockWrite (aFile, TempBuf, ElementSize * Count)
end;

type
  StringLengthType = Cardinal (64);

procedure ReadStringLittleEndian (var f : File; var s : String);
var StringLength : StringLengthType;
begin
  BlockRead (f, StringLength, SizeOf (StringLength));
  if InOutRes = 0 then
    begin
      ConvertFromLittleEndian (StringLength, SizeOf (StringLength), 1);
      {$local X+} SetLength (s, StringLength); {$endlocal}
      BlockRead (f, s [1], StringLength)
    end
end;

procedure ReadStringBigEndian (var f : File; var s : String);
var StringLength : StringLengthType;
begin
  BlockRead (f, StringLength, SizeOf (StringLength));
  if InOutRes = 0 then
    begin
      ConvertFromBigEndian (StringLength, SizeOf (StringLength), 1);
      {$local X+} SetLength (s, StringLength); {$endlocal}
      BlockRead (f, s [1], StringLength)
    end
end;

procedure WriteStringLittleEndian (var f : File; const s : String);
var StringLength : StringLengthType;
begin
  StringLength := Length (s);
  ConvertToLittleEndian (StringLength, SizeOf (StringLength), 1);
  BlockWrite (f, StringLength, SizeOf (StringLength));
  BlockWrite (f, s [1], Length (s))
end;

procedure WriteStringBigEndian (var f : File; const s : String);
var StringLength : StringLengthType;
begin
  StringLength := Length (s);
  ConvertToBigEndian (StringLength, SizeOf (StringLength), 1);
  BlockWrite (f, StringLength, SizeOf (StringLength));
  BlockWrite (f, s [1], Length (s))
end;

end.
