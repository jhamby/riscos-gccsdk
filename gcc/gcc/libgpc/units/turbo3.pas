{
Turbo Pascal 3.0 compatibility unit

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

unit Turbo3;

interface

uses GPC, System, CRT;

var
  Kbd : Text;
  CBreak : Boolean absolute CheckBreak;

procedure AssignKbd (var F : (*AnyFile*)Text);
function  (*@@fjf260*)MemAvail3 : Integer;
function  (*@@fjf260*)MaxAvail3 : Integer;
function  LongFileSize (var F : AnyFile) : Real;
function  LongFilePos  (var F : AnyFile) : Real;
procedure LongSeek     (var F : AnyFile; aPosition : Real);
procedure (*@@fjf260*)LowVideo3;
procedure (*@@fjf260*)HighVideo3;

implementation

{$B-,I-}

function Kbd_Read (var PrivateData; var Buffer; Size : SizeType) = BytesRead : SizeType;
var
  CharBuf : array [1 .. Size] of Char absolute Buffer;
  Dummy : Pointer;
begin
  Dummy := @PrivateData;
  BytesRead := 0;
  repeat
    Inc (BytesRead);
    CharBuf [BytesRead] := ReadKey;
    if CharBuf [BytesRead] = #0 then CharBuf [BytesRead] := #27
  until (BytesRead = Size) or not KeyPressed
end;

procedure AssignKbd (var F : (*AnyFile*)Text);
begin
  AssignTFDD (F, nil, nil, nil, Kbd_Read, nil, nil, nil, nil, nil)
end;

function MemAvail3 : Integer;
begin
  MemAvail3 := MemAvail div 16
end;

function MaxAvail3 : Integer;
begin
  MaxAvail3 := MaxAvail div 16
end;

function LongFileSize (var F : AnyFile) : Real;
begin
  LongFileSize := FileSize (F)
end;

function LongFilePos (var F : AnyFile) : Real;
begin
  LongFilePos := FilePos (F)
end;

procedure LongSeek (var F : AnyFile; aPosition : Real);
begin
  Seek (F, Round (aPosition))
end;

procedure LowVideo3;
begin
  TextColor (LightGray);
  TextBackground (Black)
end;

procedure HighVideo3;
begin
  TextColor (Yellow);
  TextBackground (Black)
end;

to begin do
begin
  NormAttr := Yellow + $10 * Black;
  AssignKbd (Kbd);
  Reset (Kbd)
end;

end.
