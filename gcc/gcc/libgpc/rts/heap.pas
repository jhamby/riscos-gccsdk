{
Heap management routines

Copyright (C) 1991-2000 Free Software Foundation, Inc.

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

unit Heap;

interface

uses Internal, Error;

{ C heap management routines. NOTE: if Release is used anywhere in
  the program, CFreeMem and CReAllocMem may not be used for pointers
  that were not allocated with CGetMem. }
function  CGetMem     (Size : SizeType) : Pointer;                        asmname 'malloc';
procedure CFreeMem    (aPointer : Pointer);                               asmname 'free';
function  CReAllocMem (aPointer : Pointer; NewSize : SizeType) : Pointer; asmname 'realloc';

type
  GetMemType     = ^function (Size : SizeType) : Pointer;
  FreeMemType    = ^procedure (aPointer : Pointer);
  ReAllocMemType = ^function (aPointer : Pointer; NewSize : SizeType) : Pointer;

{ These variables can be set to user-defined routines for memory
  allocation/deallocation. GetMemPtr may return nil when
  insufficient memory is available. GetMem/New will produce a
  runtime error then. }
var
  GetMemPtr     : GetMemType     = @CGetMem;     asmname '_p_getmem_ptr';
  FreeMemPtr    : FreeMemType    = @CFreeMem;    asmname '_p_freemem_ptr';
  ReAllocMemPtr : ReAllocMemType = @CReAllocMem; asmname '_p_reallocmem_ptr';

  { Points to the lowest byte of heap used }
  HeapBegin : Pointer = nil; asmname '_p_heap_begin';

  { Points to the highest byte of heap used }
  HeapHigh  : Pointer = nil; asmname '_p_heap_high';

const
  UndocumentedReturnNil = Pointer (- 1);

{ GPC_GetMem, GPC_FreeMem and GPC_ReAllocMem call the actual
  routines through GetMemPtr, FreeMemPtr and ReAllocMemPtr. }
function  GPC_GetMem     (Size : SizeType) : Pointer;    asmname '_p_malloc';
procedure GPC_FreeMem    (aPointer : Pointer);           asmname '_p_free';
procedure GPC_ReAllocMem (var aPointer : Pointer; NewSize : SizeType); asmname '_p_gpc_reallocmem';

procedure GPC_Mark     (var aPointer : Pointer);       asmname '_p_mark';
procedure GPC_Release  (aPointer : Pointer);           asmname '_p_release';

{ Returns the number of pointers that would be released. aPointer must
  have been marked with Mark. For an example of its usage, see the
  HeapMon unit. }
function  ReleaseCount (aPointer : Pointer) : Integer; asmname '_p_releasecount';

{ GPC_New, GPC_Dispose and ReAllocMem call GPC_GetMem, GPC_FreeMem
  and GPC_ReAllocMem, but also do the stuff necessary for Mark and
  Release. Therefore, GPC_GetMem, GPC_FreeMem and GPC_ReAllocMem
  should not be called directly when using Mark and Release. GetMem
  and FreeMem in a Pascal program will call GPC_New and GPC_Dispose
  internally, not GPC_GetMem and GPC_FreeMem. }
function  GPC_New      (Size : SizeType) : Pointer;    asmname '_p_new';
procedure GPC_Dispose  (aPointer : Pointer);           asmname '_p_dispose';
procedure ReAllocMem   (var aPointer : Pointer; NewSize : SizeType); asmname '_p_reallocmem';

implementation

{$B-,I-}

type
  PMarkList = ^TMarkList;
  TMarkList = record
    Next         : PMarkList;
    PointersUsed : Integer;
    Pointers     : array [0 .. 255] of Pointer
  end;

var
  CurrentMarkList : PMarkList = nil;

function GPC_GetMem (Size : SizeType) = p : Pointer;
begin
  p := GetMemPtr^(Size);
  if (p = nil) or (p = UndocumentedReturnNil) then
    begin
      SetReturnAddress (ReturnAddress (0));
      RuntimeErrorInteger (853, Size); { out of heap when allocating %d bytes }
      RestoreReturnAddress
    end
end;

procedure GPC_FreeMem (aPointer : Pointer);
begin
  FreeMemPtr^(aPointer)
end;

procedure GPC_ReAllocMem (var aPointer : Pointer; NewSize : SizeType);
begin
  aPointer := ReallocMemPtr^(aPointer, NewSize);
  if (aPointer = nil) or (aPointer = UndocumentedReturnNil) then
    begin
      SetReturnAddress (ReturnAddress (0));
      RuntimeErrorInteger (854, NewSize); { out of heap when reallocating %ld bytes }
      RestoreReturnAddress
    end
end;

procedure GPC_Mark (var aPointer : Pointer);
var Temp : PMarkList;
begin
  SetReturnAddress (ReturnAddress (0));
  Temp := GPC_GetMem (SizeOf (Temp^)); { don't use `New' here! }
  RestoreReturnAddress;
  Temp^.Next := CurrentMarkList;
  Temp^.PointersUsed := 0;
  CurrentMarkList := Temp;
  if @aPointer <> nil then aPointer := Temp { GPC_New calls GPC_Mark (null) }
end;

procedure GPC_Release (aPointer : Pointer);
var
  Temp : PMarkList;
  i : Integer;
begin
  Temp := CurrentMarkList;
  while (Temp <> nil) and (Temp <> aPointer) do Temp := Temp^.Next;
  if Temp = nil then
    begin
      SetReturnAddress (ReturnAddress (0));
      RuntimeErrorInteger (852, PtrCard (aPointer)); { address % is not valid for `Release' }
      RestoreReturnAddress
    end;
  repeat
    for i := CurrentMarkList^.PointersUsed - 1 downto 0 do
      if CurrentMarkList^.Pointers [i] <> nil then
        GPC_FreeMem (CurrentMarkList^.Pointers [i]);
    Temp := CurrentMarkList;
    CurrentMarkList := CurrentMarkList^.Next;
    GPC_FreeMem (Temp)
  until Temp = aPointer
end;

function ReleaseCount (aPointer : Pointer) = Count : Integer;
var
  Temp, Last : PMarkList;
  i : Integer;
begin
  Count := 0;
  Temp := CurrentMarkList;
  Last := nil;
  while (Temp <> nil) and (Last <> aPointer) do
    begin
      for i := Temp^.PointersUsed - 1 downto 0 do
        if Temp^.Pointers [i] <> nil then Inc (Count);
      Last := Temp;
      Temp := Temp^.Next
    end
end;

procedure SetHeapHigh (p : Pointer; Size : SizeType);
type
  PBytes = ^TBytes;
  TBytes = array [0 .. MaxVarSize div SizeOf (Byte)] of Byte;
var
  pEnd : Pointer;
begin
  pEnd := @PBytes (p)^[Size - 1];
  if PtrCard (pEnd) > PtrCard (HeapHigh) then HeapHigh := pEnd;
end;

procedure AddToMarkList (p : Pointer);
begin
  if CurrentMarkList^.PointersUsed > High (CurrentMarkList^.Pointers) then
    GPC_Mark (null); { this creates a new TMarkList item }
  CurrentMarkList^.Pointers [CurrentMarkList^.PointersUsed] := p;
  Inc (CurrentMarkList^.PointersUsed)
end;

procedure RemoveFromMarkList (aPointer : Pointer);
var
  p : PMarkList;
  Found : Boolean;
  i : Integer;
begin
  if aPointer = nil then Exit;
  Found := False;
  p := CurrentMarkList;
  while (p <> nil) and not Found do
    begin
      for i := p^.PointersUsed - 1 downto 0 do
        if p^.Pointers [i] = aPointer then
          begin
            p^.Pointers [i] := nil;
            Found := True;
            Break
          end;
      p := p^.Next
    end
end;

function GPC_New (Size : SizeType) = p : Pointer;
begin
  p := GetMemPtr^(Size);
  if p = nil then
    begin
      SetReturnAddress (ReturnAddress (0));
      RuntimeErrorInteger (853, Size); { out of heap when allocating %d bytes }
      RestoreReturnAddress
    end;
  if p = UndocumentedReturnNil then return nil;
  SetHeapHigh (p, Size);
  if CurrentMarkList <> nil then
    begin
      SetReturnAddress (ReturnAddress (0));
      AddToMarkList (p);
      RestoreReturnAddress
    end
end;

procedure GPC_Dispose (aPointer : Pointer);
begin
  RemoveFromMarkList (aPointer);
  if aPointer <> nil then GPC_FreeMem (aPointer)
end;

procedure ReAllocMem (var aPointer : Pointer; NewSize : SizeType);
begin
  RemoveFromMarkList (aPointer);
  GPC_ReAllocMem (aPointer, NewSize);
  SetHeapHigh (aPointer, NewSize);
  if CurrentMarkList <> nil then
    begin
      SetReturnAddress (ReturnAddress (0));
      AddToMarkList (aPointer);
      RestoreReturnAddress
    end
end;

begin
  GPC_Init_Malloc (HeapWarning);
  var p : Pointer = GPC_GetMem (1);
  HeapBegin := p;
  HeapHigh := p
end.
