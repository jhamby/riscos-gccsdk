{
BP compatible printer unit with extensions

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

unit Printer;

interface

{$ifdef __OS_DOS__}

{ Dos-like systems: writing to a printer device }

uses GPC;

var
  { The file name to write printer output into }
  PrinterDeviceName : ^String = @'prn';

{$else}

{ Unix-like systems: printing via a printer program }

uses GPC, Pipe;

const
  EPrinterRead = 610; { printer can only be opened for writing }

var
  { The file name of the printer program. If it contains a '/', it
    will be taken as a complete path, otherwise the file name will
    be searched for in the PATH with FSearchExecutable. }
  PrinterCommand : ^String = @'lpr';

  { Optional command line parameters for the printer program.
    Ignored when nil. }
  PrinterArguments : ^TPStrings = nil;

  { How to deal with the printer spooler after the printer pipe is
    closed, cf. the Pipe unit. }
  PrinterPipeSignal  : Integer = 0;
  PrinterPipeSeconds : Integer = 0;
  PrinterPipeWait    : Boolean = True;

{$endif}

{ Text file opened to default printer }
var
  Lst : Text;

{ Assign a file to the printer. Lst will be assigned to the default
  printer at program start, but other files can be assigned to the
  same or other printers (possibly after changing the variables
  above). SpoolerOutput, if not null, will be redirected from the
  printer spooler's standard output and error. If you use this, note
  that a deadlock might arise when trying to write data to the
  spooler while its output is not being read, though this seems
  quite unlikely, since most printer spoolers don't write so much
  output that could fill a pipe. Under Dos, where no spooler is
  involved, SpoolerOutput, if not null, will be reset to an empty
  file for compatibility. }
procedure AssignPrinter (var f : AnyFile; var SpoolerOutput : AnyFile);

implementation

{$B-,I-}

{$ifdef __OS_DOS__}

procedure AssignPrinter (var f : AnyFile; var SpoolerOutput : AnyFile);
begin
  Assign (f, PrinterDeviceName^);
  if @SpoolerOutput <> nil then
    begin
      Unbind (SpoolerOutput);
      Rewrite (SpoolerOutput);
      Reset (SpoolerOutput)
    end
end;

{$else}

type
  TPrinterTFDDData = record
    f, SpoolerOutput : PAnyFile;
  end;

procedure OpenPrinter (var PrivateData; Mode : TOpenMode);
var Dummy : Pointer;
begin
  Dummy := @PrivateData;
  if not (Mode in [foRewrite, foAppend]) then IOError (EPrinterRead)
end;

{ Be very lazy: don't open the pipe until data are written to it -- not
  as soon as the file is opened because that happens already in the
  initialization of this unit (BP compatibility) }
function WritePrinter (var PrivateData; const Buffer; Size : SizeType) : SizeType;
var
  Data : TPrinterTFDDData absolute PrivateData;
  CharBuf : array [1 .. Size] of Char absolute Buffer;
  Process : PPipeProcess;
begin
  WritePrinter := 0;
  Pipe (Data.f^, Data.SpoolerOutput^, Data.SpoolerOutput^, PrinterCommand^, PrinterArguments^, GetCEnvironment, Process, nil); { this also makes sure this function won't be called again for this file }
  if InOutRes <> 0 then Exit;
  Process^.Signal  := PrinterPipeSignal;
  Process^.Seconds := PrinterPipeSeconds;
  Process^.Wait    := PrinterPipeWait;
  Write (Data.f^, CharBuf);
  if InOutRes = 0 then WritePrinter := Size
end;

procedure AssignPrinter (var f : AnyFile; var SpoolerOutput : AnyFile);
var p : ^TPrinterTFDDData;
begin
  if @SpoolerOutput <> nil then
    begin
      Unbind (SpoolerOutput);
      Rewrite (SpoolerOutput);
      Reset (SpoolerOutput)
    end;
  New (p);
  p^.f := @f;
  p^.SpoolerOutput := @SpoolerOutput;
  AssignTFDD (f, OpenPrinter, nil, nil, nil, WritePrinter, nil, nil, nil, p)
end;

{$endif}

to begin do
begin
  AssignPrinter (Lst, null);
  Rewrite (Lst)
end;

to end do
  Close (Lst);

end.
