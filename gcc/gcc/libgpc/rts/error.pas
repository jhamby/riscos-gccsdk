{
Error handling routines

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

unit Error;

interface

uses Internal, String;

const
  ERead = 413;
  EWrite = 414;
  EWriteReadOnly = 422;

var
  { BP compatible InOutRes variable }
  GPC_InOutRes : Integer = 0; asmname '_p_inoutres';

  { CString parameter to some error messages, NOT the text of the error
    message (the latter can be obtained with GetErrorMessage) }
  GPC_InOutResStr : CString = nil; asmname '_p_inoutres_str';

  { Error number (after runtime error) or exit status (after Halt) or
    0 (during program run and after succesful termination). }
  ExitCode : Integer = 0; asmname '_p_exitcode';

  { Contains the address of the code where a runtime occurred, nil
    if no runtime error occurred. }
  ErrorAddr : Pointer = nil; asmname '_p_erroraddr';

  { Error message }
  ErrorMessageString : TString = ''; asmname '_p_errormessagestring';

  RTSDebugFlag  : Integer = 0;     asmname '_p_debug';
  RTSWarnFlag   : Boolean = False; asmname '_p_warn';
  AbortOnError  : Boolean = False; asmname '_p_abort_on_error';
  ErrorFD       : Integer = -1;    asmname '_p_error_fd';
  ErrorFileName : CString = nil;   asmname '_p_error_file_name';

function  GetErrorMessage                 (n : Integer) : CString;                   asmname '_p_errmsg';
procedure RuntimeError                    (n : Integer);                             attribute (noreturn); asmname '_p_error';
procedure RuntimeErrorInteger             (n : Integer; i : MedInt);                 attribute (noreturn); asmname '_p_error_integer';
procedure RuntimeErrorCString             (n : Integer; s : CString);                attribute (noreturn); asmname '_p_error_string';
procedure InternalError                   (n : Integer);                             attribute (noreturn); asmname '_p_internal_error';
procedure InternalErrorInteger            (n : Integer; i : MedInt);                 attribute (noreturn); asmname '_p_internal_error_integer';
procedure RuntimeWarning                  (Message : CString);                       asmname '_p_warning';
procedure RuntimeWarningInteger           (Message : CString; i : MedInt);           asmname '_p_warning_integer';
procedure RuntimeWarningCString           (Message : CString; s : CString);          asmname '_p_warning_string';
procedure DebugStatement                  (const FileName : String; Line : Integer); asmname '_p_debug_statement';
procedure HeapWarning                     (s : CString);                             asmname '_p_heap_warning'; { For GNU malloc }
procedure GPC_RunError                    (n : Integer);                             attribute (noreturn); asmname '_p_runerror';
procedure StartTempIOError;                                                          asmname '_p_start_temp_io_error';
function  EndTempIOError : Integer;                                                  asmname '_p_end_temp_io_error';
(*iocritical*)procedure IOError                         (n : Integer);                             asmname '_p_io_error';
(*iocritical*)procedure IOErrorInteger                  (n : Integer; i : MedInt);                 asmname '_p_io_error_integer';
(*iocritical*)procedure IOErrorCString                  (n : Integer; s : CString);                asmname '_p_io_error_string';
(*iocritical*)procedure IOErrorFile                     (n : Integer; protected var f : AnyFile);  asmname '_p_io_error_file';
function  GPC_IOResult : Integer;                                                    asmname '_p_ioresult';
function  GetIOErrorMessage : CString;                                               asmname '_p_get_io_error_message';
procedure CheckInOutRes;                                                             asmname '_p_check_inoutres';
procedure GPC_Halt (aExitCode : Integer);                                            attribute (noreturn); asmname '_p_halt';

{ Registers a procedure to be called to restore the terminal for
  another process that accesses the terminal, or back for the
  program itself. Used e.g. by the CRT unit. The procedures must
  allow for being called multiple times in any order, even at the
  end of the program (see the comment for RestoreTerminal). }
procedure RegisterRestoreTerminal (ForAnotherProcess : Boolean; procedure Proc); asmname '_p_RegisterRestoreTerminal';

{ Unregisters a procedure registered with RegisterRestoreTerminal.
  Returns False if the procedure had not been registered, and True
  if it had been registered and was unregistered successfully. }
function UnregisterRestoreTerminal (ForAnotherProcess : Boolean; procedure Proc) : Boolean; asmname '_p_UnregisterRestoreTerminal';

{ Calls the procedures registered by RegisterRestoreTerminal. When
  restoring the terminal for another process, the procedures are
  called in the opposite order of registration. When restoring back
  for the program, they are called in the order of registration.

  `RestoreTerminal (True)' will also be called at the end of the
  program, before outputting any runtime error message. It can also
  be used if you want to write an error message and exit the program
  (especially when using e.g. the CRT unit). For this purpose, to
  avoid side effects, call RestoreTerminal immediately before
  writing the error message (to StdErr, not to Output!), and then
  exit the program (e.g. with Halt). }
procedure RestoreTerminal (ForAnotherProcess : Boolean); asmname '_p_RestoreTerminal';

procedure AtExit (procedure Proc); asmname '_p_atexit';
procedure Finalize; asmname '_p_finalize';

procedure SetReturnAddress (Address : Pointer);                                      asmname '_p_SetReturnAddress';
procedure RestoreReturnAddress;                                                      asmname '_p_RestoreReturnAddress';

var
  { Signal actions }
  SignalDefault : TSignalHandler; asmname '_p_SIG_DFL'; external;
  SignalIgnore  : TSignalHandler; asmname '_p_SIG_IGN'; external;
  SignalError   : TSignalHandler; asmname '_p_SIG_ERR'; external;

  { Signals. The constants are set to the signal numbers, and
    are 0 for signals not defined. }
  { POSIX signals }
  SigHUp    : Integer; asmname '_p_SIGHUP'; external;
  SigInt    : Integer; asmname '_p_SIGINT'; external;
  SigQuit   : Integer; asmname '_p_SIGQUIT'; external;
  SigIll    : Integer; asmname '_p_SIGILL'; external;
  SigAbrt   : Integer; asmname '_p_SIGABRT'; external;
  SigFPE    : Integer; asmname '_p_SIGFPE'; external;
  SigKill   : Integer; asmname '_p_SIGKILL'; external;
  SigSegV   : Integer; asmname '_p_SIGSEGV'; external;
  SigPipe   : Integer; asmname '_p_SIGPIPE'; external;
  SigAlrm   : Integer; asmname '_p_SIGALRM'; external;
  SigTerm   : Integer; asmname '_p_SIGTERM'; external;
  SigUsr1   : Integer; asmname '_p_SIGUSR1'; external;
  SigUsr2   : Integer; asmname '_p_SIGUSR2'; external;
  SigChld   : Integer; asmname '_p_SIGCHLD'; external;
  SigCont   : Integer; asmname '_p_SIGCONT'; external;
  SigStop   : Integer; asmname '_p_SIGSTOP'; external;
  SigTStp   : Integer; asmname '_p_SIGTSTP'; external;
  SigTTIn   : Integer; asmname '_p_SIGTTIN'; external;
  SigTTOu   : Integer; asmname '_p_SIGTTOU'; external;

  { Non-POSIX signals }
  SigTrap   : Integer; asmname '_p_SIGTRAP'; external;
  SigIOT    : Integer; asmname '_p_SIGIOT'; external;
  SigEMT    : Integer; asmname '_p_SIGEMT'; external;
  SigBus    : Integer; asmname '_p_SIGBUS'; external;
  SigSys    : Integer; asmname '_p_SIGSYS'; external;
  SigStkFlt : Integer; asmname '_p_SIGSTKFLT'; external;
  SigUrg    : Integer; asmname '_p_SIGURG'; external;
  SigIO     : Integer; asmname '_p_SIGIO'; external;
  SigPoll   : Integer; asmname '_p_SIGPOLL'; external;
  SigXCPU   : Integer; asmname '_p_SIGXCPU'; external;
  SigXFSz   : Integer; asmname '_p_SIGXFSZ'; external;
  SigVTAlrm : Integer; asmname '_p_SIGVTALRM'; external;
  SigProf   : Integer; asmname '_p_SIGPROF'; external;
  SigPwr    : Integer; asmname '_p_SIGPWR'; external;
  SigInfo   : Integer; asmname '_p_SIGINFO'; external;
  SigLost   : Integer; asmname '_p_SIGLOST'; external;
  SigWinCh  : Integer; asmname '_p_SIGWINCH'; external;

  { Signal subcodes (only used on some systems, -1 if not used) }
  IllReservedAddress        : Integer; asmname '_p_ILL_RESAD_FAULT'; external;
  IllPriviledgedInstruction : Integer; asmname '_p_ILL_PRIVIN_FAULT'; external;
  IllReservedOp             : Integer; asmname '_p_ILL_RESOP_FAULT'; external;
  FPEIntegerOverflow        : Integer; asmname '_p_FPE_INTOVF_TRAP'; external;
  FPEIntegerDivisionByZero  : Integer; asmname '_p_FPE_INTDIV_TRAP'; external;
  FPESubscriptRange         : Integer; asmname '_p_FPE_SUBRNG_TRAP'; external;
  FPERealOverflow           : Integer; asmname '_p_FPE_FLTOVF_TRAP'; external;
  FPERealDivisionByZero     : Integer; asmname '_p_FPE_FLTDIV_TRAP'; external;
  FPERealUnderflow          : Integer; asmname '_p_FPE_FLTUND_TRAP'; external;
  FPEDecimalOverflow        : Integer; asmname '_p_FPE_DECOVF_TRAP'; external;

implementation

{$B-,I-}

{$ifndef HAVE_NO_RTS_CONFIG_H}
{$include "rts-config.inc"}
{$endif}

const
  InternalErrorString = 'internal error: ';

  ErrorMessages : array [1 .. 220] of record
    Number  : Integer;
    Message : CString
  end =
  (
    { Note: use just `%' for the optional argument to the messages.
      The errors are not written using one of the `printf' functions
      anymore, but a more Pascalish formatting that gets knowledge
      about the type of the argument from its caller. Any character
      following the `%' becomes part of the actual error message! }

    { Leave the `Byte' range free for program specific errors. }

    { Signal handlers }
    (257, 'hangup signal received'),
    (258, 'interrupt signal received'),
    (259, 'quit signal received'),
    (260, 'illegal instruction signal received'),
    (261, 'trap signal received'),
    (262, 'I/O trap signal received'),
    (263, 'emulator trap signal received'),
    (264, 'floating point exception signal received'),
    (266, 'bus error signal received'),
    (267, 'segmentation fault signal received'),
    (268, 'bad system call signal received'),
    (269, 'broken pipe signal received'),
    (270, 'alarm signal received'),
    (271, 'termination signal received'),
    (284, 'reserved adressing signal received'),
    (285, 'privileged instruction signal received'),
    (286, 'reserved operand signal received'),

    { Unsorted errors }
    (300, 'an error which was reported during compilation'),
    (301, 'array index out of bounds'),
    (302, 'variant access error'),
    (303, 'attempt to dereference nil pointer'),
    (304, 'attempt to dereference undefined pointer'),
    (307, 'scalar parameter out of bounds'),
    (308, 'set parameter out of bounds'),
    (309, 'range error in set constructor'),
    (317, 'input data out of bounds'),
    (318, 'output data out of bounds'),
    (323, 'dispose applied to nil pointer'),
    (324, 'dispose applied to undefined pointer'),
    (326, 'index parameter of `Pack'' out of bounds'),
    (329, 'index parameter of `Unpack'' out of bounds'),
    (332, 'argument to `Sqr'' out of range'),
    (337, 'argument to `Chr'' out of range'),
    (338, 'argument to `Succ'' out of range'),
    (339, 'argument to `Pred'' out of range'),
    (343, 'attempt to use an undefined value'),
    (348, 'function undefined upon return'),
    (349, 'value to be assigned is out of bounds'),
    (351, '`Case'' selector value matches no case constant'),
    (352, 'initial value of `For'' control variable out of range'),
    (353, 'final value of `For'' control variable out of range'),
    (354, 'integer data out of range'),
    (355, 'index type of conformant array out of range'),
    (380, 'call to predefined procedure `Bug'''),
    (381, 'assert failure'),
    (382, 'attempt to use undefined value of ordinal type'),
    (383, 'attempt to use undefined value of set type'),
    (384, 'attempt to use undefined value of integer type'),
    (385, 'attempt to use undefined value of real type'),
    (386, 'attempt to use undefined value of pointer type'),

    { I/O errors (range 400 .. 699) that are handled via InOutRes }

    { I/O errors: File and general I/O errors }
    { For errors raised with IOERROR_FILE, the "%" will be replaced by
      "file `foo.bar'" for external files or "internal file `foo'" for
      internal files, so don't include "file" in the error message }
  { (400, 'cannot open %'), }
    (401, 'cannot open directory `%'''),
    (402, '`Bind'' applied to non-bindable %'),
    (403, '`Binding'' applied to non-bindable %'),
    (404, '`Unbind'' applied to non-bindable %'),
    (405, 'could not open `%'''),
    (406, 'attempt to read past end of random access %'),
    (407, '% has not been opened'),
  { (408, 'attempt to test `EOF'' for random access %'),
    (409, '% is not opened for `Seek'' operations'), }
    (410, 'attempt to access elements before beginning of random access %'),
    (411, 'attempt to modify read only %'),
  { (412, 'random access % back stepping failed'), }
    (413, 'read error'),
    (414, 'write error'),
    (415, 'cannot read all the data from % in `BlockRead'''),
    (416, '`Extend'' could not seek to end of %'),
    (417, '`FilePos'' could not get file position of %'),
    (418, 'error while closing %'),
    (419, 'cannot prompt user for external name bindings for %'),
    (420, 'cannot query user for external name bindings for %'),
    (421, 'EOT character given for query of name for %'),
    (422, 'cannot write to read only %'),
    (423, 'ftruncate failed when re-opening % with `Rewrite'''),
    (424, 'invalid string length in `Bind'' of %'),
    (425, 'truncation failed for %'),
    (426, '`SeekRead'' to write only %'),
    (427, '`SeekRead'' seek failed on %'),
    (428, '`SeekRead'' failed to reset position of %'),
    (429, '`SeekWrite'' seek failed on %'),
    (430, '`SeekUpdate'' to read-only or write-only %'),
    (431, '`SeekUpdate'' seek failed on %'),
    (432, '`SeekUpdate'' failed to reset position of %'),
    (433, '`Update'' failed to reset the position of %'),
    (436, '`Reset'', `SeekUpdate'' or `SeekRead'' to nonexistent %'),
    (438, '`Truncate'' or `DefineSize'' applied to read only %'),
    (439, '`Update'' with an undefined buffer in %'),
    (440, 'reference to buffer variable of % with undefined value'),
    (441, 'file already bound to `%'''),
    (442, '% cannot be opened for reading'),
    (443, '% cannot be opened for writing'),
    (444, '% cannot be opened for updating'),
    (445, '% cannot be extended'),
    (446, 'cannot get the size of %'),
    (450, '% is not open for writing'),
  { (451, '% must be opened before writing'), }
    (452, '% is not open for reading'),
  { (453, '% must be opened before reading'), }
    (454, 'attempt to read past end of %'),
    (455, '`EOF'' tested for unopened %'),
    (456, '`EOLn'' tested for unopened %'),
    (457, '`EOLn'' tested for % when `EOF'' is true'),
    (458, '`EOLn'' applied to a non-text %'),
  { (460, '% not found'),
    (461, '% cannot be accessed'),
    (462, 'attempt to open % as external'),
    (463, '% is write protected'), }
    (464, 'error when reading from %'),
    (465, 'cannot read all the data from %'),
    (466, 'error when writing to %'),
    (467, 'cannot write all the data to %'),
    (468, 'cannot erase %'),
    (469, '`Erase'': external file `%'' has no external name'),
    (471, '`Erase'': % does not exist'),
    (472, 'permission denied to erase %'),
    (473, '`Erase'': cannot erase directory `%'''),
    (474, 'I/O error when trying to erase %'),
    (475, 'cannot rename %'),
    (476, '`Rename'': external file `%'' has no external name'),
    (477, 'cannot rename opened %'),
    (478, '`Rename'': % does not exist'),
    (479, 'permission denied to rename %'),
    (480, '`Rename'': cannot overwrite directory `%'''),
    (481, 'I/O error when trying to rename %'),
    (482, '`Rename'': cannot overwrite file `%'''),
    (483, 'cannot change to directory `%'''),
    (484, 'cannot make directory `%'''),
    (485, 'cannot remove directory `%'''),
    (486, '`SetFTime'': file `%'' has no external name'),
    (487, 'cannot set time for %'),
    (488, '`Execute'': cannot execute program'),
    (489, '`StatFS'': function not supported'),
    (490, 'cannot stat file system `%'''),
    (491, '`ChMod'': file `%'' has no external name'),
    (492, '`ChMod'': % does not exist'),
    (493, 'permission denied to change mode of %'),
    (494, 'I/O error when trying to change mode of %'),
    (495, 'cannot open directory `%'''),
    (496, 'cannot close directory'),
    (497, 'no temporary file name found'),

    { I/O errors: Read errors }
    (550, 'attempt to read past end of string in `ReadStr'''),
    (551, 'digit expected after sign'),
    (552, 'sign or digit expected'),
    (553, 'overflow while reading integer'),
    (554, 'digit expected after decimal point'),
    (555, 'digit expected while reading exponent'),
    (556, 'exponent out of range'),
    (557, 'digit expected after `$'' in integer constant'),
    (558, 'digit expected after `#'' in integer constant'),
    (559, 'only one base specifier allowed in integer constant'),
    (560, 'base out of range (2..36)'),
    (561, 'invalid digit'),
    (562, 'digit or `.'' expected after sign'),
    (563, 'overflow while reading real number'),
    (564, 'underflow while reading real number'),
    (565, 'extra characters after number in `Val'''), { only used internally }
    (566, 'invalid Boolean value read'),
    (567, 'invalid enumaration value read'),

    { I/O errors: Write errors }
    (580, 'fixed field width cannot be negative'),
    (581, 'fixed real fraction field width cannot be negative'),
    (582, 'string capacity exceeded in `WriteStr'''),

    { I/O errors: Application of direct access routines to non-direct files.
      They can be warnings or errors, depending on _p_force_direct_files }
    (590, 'direct access routine `GetSize'' applied to non-direct %'),
    (591, 'direct access routine `SeekRead'' applied to non-direct %'),
    (592, 'direct access routine `SeekWrite'' applied to non-direct %'),
    (593, 'direct access routine `SeekUpdate'' applied to non-direct %'),
    (594, 'direct access routine `Empty'' applied to non-direct %'),
    (595, 'direct access routine `Update'' applied to non-direct %'),
    (596, 'direct access routine `Position'' applied to non-direct %'),

    { I/O errors: device specific errors }
    (600, 'cannot fork `%'''),
    (601, 'cannot spawn `%'''),
    (610, 'printer can only be opened for writing'),
    (620, 'unknown serial port #%'),
    (621, 'serial port #% cannot be opened'),

    { Mathematical errors }
    (700, 'overflow in exponentiation'),
    (701, 'in `x pow y'', x must be >= 0 if y < 0 and y is not an integer'),
    (703, 'executed `x pow y'' when x is zero and y < 0'),
    (704, 'cannot take `Arg'' of zero'),
    (706, 'executed `x pow y'' when complex x is zero and y < 0'),
    (707, 'argument to `Ln'' is <= 0'),
    (708, 'argument to `Sqrt'' is < 0'),
    (709, 'significancy lost in `Cos'' - result set to zero'),
    (710, 'significancy lost in `Sin'' - result set to zero'),
    (711, 'floating point division by zero'),
    (712, 'integer division by 0'),
    (713, 'integer overflow'),
    (714, 'second operand of `mod'' is <= 0'),
    (715, 'floating point overflow'),
    (716, 'floating point underflow'),
    (717, 'decimal overflow'),
    (718, 'subscript error'),

    { Time and date errors }
    (750, 'invalid date supplied to library function `Date'''),
    (751, 'invalid time supplied to library function `Time'''),

    { String errors (except string I/O errors) }
    (800, 'string too long in `Insert'''),
    (801, 'substring cannot start from a position less than 1'),
    (802, 'substring length cannot be negative'),
    (803, 'substring must terminate before end of string'),
    (806, 'string too long'),

    { Memory management errors }
    (850, 'stack overflow'),
    (851, 'heap overflow'),
    (852, 'address % is not valid for `Release'''),
    (853, 'out of heap when allocating % bytes'),
    (854, 'out of heap when reallocating % bytes'),
  { (855, 'attempt to use disposed pointer'),
    (856, 'attempt to use disposed object'), }
    (857, 'attempt to map unmappable memory'),

    { Errors for units }
    (870, 'BP compatible 6 byte `Real'' type does not support NaNs'),
    (871, 'BP compatible 6 byte `Real'' type does not support infinity'),
    (872, 'underflow while converting to BP compatible 6 byte `Real'' type'),
    (873, 'overflow while converting to BP compatible 6 byte `Real'' type'),
    (874, 'cannot convert denormalized number to BP compatible 6 byte `Real'' type'),

    (880, 'CRT was not initialized'),
    (881, 'attempt to delete invalid CRT panel'),
    (882, 'attempt to delete last CRT panel'),
    (883, 'attempt to activate invalid CRT panel'),

    { Internal errors }
    (900, 'compiler calls `Readln'' incorrectly'),
    (901, 'compiler calls `Writeln'' incorrectly'),
    (902, 'unknown code in `Read'''),
    (903, 'unknown code in `ReadStr'''),
    (904, 'unknown code in `Write'''),
    (905, 'unknown code in `WriteStr'''),
    (906, 'unknown string code in `WriteStr'''),
    (907, 'string length cannot be negative'),
    (908, 'incorrect reading of a string'),
    (909, 'unknown string function called'),
    (910, 'read buffer underflow');
    (911, 'invalid file open mode');
    (914, '_p_initfdr has not been called for file'),
    (921, 'unknown mode in _p_open'),
    (922, 'undocumented error code % in exponentiation'),
    (925, 'file has no internal name'),

    { Internal errors for units }
    (950, 'CRT: cannot initialize curses'),
    (951, 'cannot create CRT window')
  );

  SignalTable : array [1 .. 24] of record
    Signal, Code : ^Integer; (*@@*)
    ErrorNumber { negative if warning } : Integer
  end =
  (
    (@SigHUp,  nil, 257),
    (@SigInt,  nil, 258),
    (@SigQuit, nil, 259),
    (@SigIll,  @IllReservedAddress, 284),
    (@SigIll,  @IllPriviledgedInstruction, 285),
    (@SigIll,  @IllReservedOp, 286),
    (@SigIll,  nil, 260),
    (@SigFPE,  @FPEIntegerOverflow, 713),
    (@SigFPE,  @FPEIntegerDivisionByZero, 712),
    (@SigFPE,  @FPESubscriptRange, 718),
    (@SigFPE,  @FPERealOverflow, 715),
    (@SigFPE,  @FPERealDivisionByZero, 711),
    (@SigFPE,  @FPERealUnderflow, -716),
    (@SigFPE,  @FPEDecimalOverflow, 717),
    (@SigFPE,  nil, 264),
    (@SigSegV, nil, 267),
    (@SigPipe, nil, 269),
    (@SigAlrm, nil, 270),
    (@SigTerm, nil, 271),
    (@SigTrap, nil, 261),
    (@SigIOT,  nil, 262),
    (@SigEMT,  nil, 263),
    (@SigBus,  nil, 266),
    (@SigSys,  nil, 268)
  );

var
  TempIOErrorFlag : Boolean = False;
  TempInOutRes : Integer = 0;

function GetErrorMessage (n : Integer) : CString;
var i : Integer;
begin
  for i := Low (ErrorMessages) to High (ErrorMessages) do
    if ErrorMessages [i].Number = n then return ErrorMessages [i].Message;
  GetErrorMessage := 'internal error: unknown error code'
end;

{ Very simple replacement for `sprintf'. Probably not very useful
  for general purposes, therefore not in the interface. Only here to
  format the error messages above. }
function FormatStr (Format : CString; const Argument : String) = s : TString;
var i : Integer;
begin
  s := CString2String (Format);
  i := Pos ('%', s);
  if i = 0 then
    s := 'internal error: error handling was called incorrectly: ' + s + Argument
  else
    begin
      Delete (s, i, 1);
      Insert (Argument, s, i)
    end
end;

function StartRuntimeWarning = Result : Boolean;
begin
  Result := RTSWarnFlag or (RTSDebugFlag <> 0);
  if Result then Write (StdErr, ParamStr (0), ': warning: ')
end;

procedure RuntimeWarning (Message : CString);
begin
  if StartRuntimeWarning then
    Writeln (StdErr, CString2String (Message))
end;

procedure RuntimeWarningInteger (Message : CString; i : MedInt);
begin
  if StartRuntimeWarning then
    Writeln (StdErr, FormatStr (Message, Integer2String (i)))
end;

procedure RuntimeWarningCString (Message : CString; s : CString);
begin
  if StartRuntimeWarning then
    Writeln (StdErr, FormatStr (Message, CString2String (s)))
end;

procedure DebugStatement (const FileName : String; Line : Integer);
begin
  Writeln (StdErr, FileName, ':', Line, ': DebugStatement')
end;

procedure HeapWarning (s : CString);
begin
  RuntimeWarningCString ('heap warning: %', s)
end;

type
  PProcedure = ^procedure;
  PProcList = ^TProcList;
  TProcList = record
    Next, Prev : PProcList;
    Proc : PProcedure
  end;

var
  RestoreTerminalProcs : array [Boolean] of PProcList = (nil, nil);
  AtExitProcs : PProcList = nil;

procedure InsertProcList (var List : PProcList; procedure Proc);
var p : PProcList;
begin
  New (p);
  p^.Proc := @Proc;
  p^.Prev := nil;
  p^.Next := List;
  if p^.Next <> nil then p^.Next^.Prev := p;
  List := p
end;

procedure RegisterRestoreTerminal (ForAnotherProcess : Boolean; procedure Proc);
begin
  InsertProcList (RestoreTerminalProcs [ForAnotherProcess], Proc)
end;

function UnregisterRestoreTerminal (ForAnotherProcess : Boolean; procedure Proc) : Boolean;
var p : PProcList;
begin
  p := RestoreTerminalProcs [ForAnotherProcess];
  while (p <> nil) and (p^.Proc <> @Proc) do p := p^.Next;
  if p = nil then
    UnregisterRestoreTerminal := False
  else
    begin
      if p^.Next <> nil then p^.Next^.Prev := p^.Prev;
      if p^.Prev = nil
        then RestoreTerminalProcs [ForAnotherProcess] := p^.Next
        else p^.Prev^.Next := p^.Next;
      Dispose (p);
      UnregisterRestoreTerminal := True
    end
end;

procedure RestoreTerminal (ForAnotherProcess : Boolean);
var p : PProcList;
begin
  SetReturnAddress (ReturnAddress (0));
  p := RestoreTerminalProcs [ForAnotherProcess];
  if ForAnotherProcess then
    while p <> nil do
      begin
        p^.Proc^;
        p := p^.Next
      end
  else if p <> nil then
    begin
      while p^.Next <> nil do p := p^.Next;
      while p <> nil do
        begin
          p^.Proc^;
          p := p^.Prev
        end
    end;
  RestoreReturnAddress
end;

procedure AtExit (procedure Proc);
begin
  InsertProcList (AtExitProcs, Proc)
end;

type String32 = String (32); (*@@TString can cause stack overflow in a signal handler*)
function ReturnAddr2Hex (p : Pointer) = s : String32;
const HexDigits : array [0 .. $f] of Char = '0123456789abcdef';
var i, j : PtrInt;
begin
  s := '';
  { Subtract 1 to get a pointer to the last byte of the corresponding call
    instruction. Might not be fool-proof, but perhaps the best we can do. }
  i := PtrInt (p) - 1;
  j := 1;
  while j <= i div $10 do
    j := $10 * j;
  while j > 0 do
    begin
      s := s + HexDigits [i div j];
      i := i mod j;
      j := j div $10
    end
end;

procedure Finalize1;
var
  p : PProcList;
  Proc : PProcedure;
  i : Integer;
  a : Pointer;
  Dummy : Boolean;
begin
  while AtExitProcs <> nil do
    begin
      p := AtExitProcs;
      AtExitProcs := AtExitProcs^.Next;
      Proc := p^.Proc;
      Dispose (p);
      Proc^
    end;
  FlushAllFiles (False);
  RestoreTerminal (True);
  GPC_Done_Files;
  if (ErrorMessageString <> '') and WriteErrorMessage (ParamStr (0) + ': ' + ErrorMessageString + NewLine, True) then
    begin
      Dummy := WriteErrorMessage (ErrorMessageString + NewLine, False);
      Dummy := WriteErrorMessage (ReturnAddr2Hex (ErrorAddr) + NewLine, False);
      Dummy := WriteErrorMessage ('Stack trace:' + NewLine, False);
      i := 0;
      repeat
        case i of
           0 : a := ReturnAddress  (0);
          {$ifdef HAVE_RETURN_ADDRESS_NON_ZERO}
           1 : a := ReturnAddress  (1);
           2 : a := ReturnAddress  (2);
           3 : a := ReturnAddress  (3);
           4 : a := ReturnAddress  (4);
           5 : a := ReturnAddress  (5);
           6 : a := ReturnAddress  (6);
           7 : a := ReturnAddress  (7);
           8 : a := ReturnAddress  (8);
           9 : a := ReturnAddress  (9);
          10 : a := ReturnAddress (10);
          11 : a := ReturnAddress (11);
          12 : a := ReturnAddress (12);
          13 : a := ReturnAddress (13);
          14 : a := ReturnAddress (14);
          15 : a := ReturnAddress (15);
          {$endif}
          else a := nil
        end;
        if a <> nil then
          Dummy := WriteErrorMessage (ReturnAddr2Hex (a) + NewLine, False);
        Inc (i)
      until (i = 16) or (a = nil)
    end
end;

procedure Finalize;
begin
  ErrorMessageString := '';
  ExitCode := 0;
  ErrorAddr := nil;
  Finalize1
end;

procedure GPC_Halt (aExitCode : Integer);
begin
  ErrorMessageString := '';
  ExitCode := aExitCode;
  ErrorAddr := nil;
  Finalize1;
  ExitProgram (aExitCode, False)
end;

var
  CurrentReturnAddr : Pointer = nil;
  CurrentReturnAddrCounter : Integer = 0;

procedure SetReturnAddress (Address : Pointer);
begin
  if CurrentReturnAddrCounter = 0 then CurrentReturnAddr := Address;
  Inc (CurrentReturnAddrCounter)
end;

procedure RestoreReturnAddress;
begin
  Dec (CurrentReturnAddrCounter);
  if CurrentReturnAddrCounter = 0 then CurrentReturnAddr := nil
end;

procedure FinishErrorMessage (n : Integer);
var s : String32;
begin
  ExitCode := n;
  ErrorAddr := CurrentReturnAddr;
  CurrentReturnAddr := nil;
  CurrentReturnAddrCounter := 0;
  s := ReturnAddr2Hex (ErrorAddr);

  (*@@@@Grmpf! How else can we stop GPC from wasting stack space (important when called from a signal handler?@@*)
  var foo : String (Length (ErrorMessageString));
  Move (ErrorMessageString, foo, SizeOf (foo));

  WriteStr (ErrorMessageString, foo, ' (error #', n, ' at ', s, ')')
end;

procedure EndRuntimeError (n : Integer); attribute (noreturn); asmname '_p_end_runtime_error';
procedure EndRuntimeError (n : Integer);
begin
  FinishErrorMessage (n);
  Finalize1;
  ExitProgram (n, AbortOnError)
end;

procedure RuntimeError (n : Integer);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := CString2String (GetErrorMessage (n));
  EndRuntimeError (n)
end;

procedure RuntimeErrorInteger (n : Integer; i : MedInt);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := FormatStr (GetErrorMessage (n), Integer2String (i));
  EndRuntimeError (n)
end;

procedure RuntimeErrorCString (n : Integer; s : CString);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := FormatStr (GetErrorMessage (n), CString2String (s));
  EndRuntimeError (n)
end;

procedure InternalError (n : Integer);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := InternalErrorString + CString2String (GetErrorMessage (n));
  EndRuntimeError (n)
end;

procedure InternalErrorInteger (n : Integer; i : MedInt);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := InternalErrorString + FormatStr (GetErrorMessage (n), Integer2String (i));
  EndRuntimeError (n)
end;

procedure GPC_RunError (n : Integer);
begin
  SetReturnAddress (ReturnAddress (0));
  ErrorMessageString := 'runtime error';
  EndRuntimeError (n)
end;

procedure StartTempIOError;
begin
  TempInOutRes := GPC_IOResult;
  TempIOErrorFlag := True
end;

function EndTempIOError : Integer;
begin
  EndTempIOError := IOResult;
  GPC_InOutRes := TempInOutRes;
  TempIOErrorFlag := False
end;

procedure IOError (n : Integer);
begin
  GPC_InOutRes := n;
  if not TempIOErrorFlag then
    begin
      if GPC_InOutResStr <> nil then Dispose (GPC_InOutResStr);
      GPC_InOutResStr := nil
    end
end;

procedure IOErrorInteger (n : Integer; i : MedInt);
begin
  GPC_InOutRes := n;
  if not TempIOErrorFlag then
    begin
      if GPC_InOutResStr <> nil then Dispose (GPC_InOutResStr);
      GPC_InOutResStr := NewCString (Integer2String (i))
    end
end;

procedure IOErrorCString (n : Integer; s : CString);
begin
  GPC_InOutRes := n;
  if not TempIOErrorFlag then
    begin
      if GPC_InOutResStr <> nil then Dispose (GPC_InOutResStr);
      GPC_InOutResStr := CStringNew (s)
    end
end;

procedure IOErrorFile (n : Integer; protected var f : AnyFile);
begin
  IOErrorCString (n, CString2String (GetInternalFileName (f)))
end;

(*@@maur3.pas inline*) function GPC_IOResult : Integer;
begin
  GPC_IOResult := GPC_InOutRes;
  GPC_InOutRes := 0
end;

function GetIOErrorMessage : CString;
var Buf : static CString = nil;
begin
  if GPC_InOutResStr <> nil then
    begin
      if Buf <> nil then Dispose (Buf);
      Buf := NewCString (FormatStr (GetErrorMessage (GPC_IOResult), CString2String (GPC_InOutResStr)));
      GetIOErrorMessage := Buf
    end
  else
    GetIOErrorMessage := GetErrorMessage (GPC_IOResult)
end;

procedure CheckInOutRes;
var Temp : Integer;
begin
  Temp := GPC_IOResult;
  if Temp <> 0 then
    begin
      SetReturnAddress (ReturnAddress (0));
      if GPC_InOutResStr <> nil
        then RuntimeErrorCString (Temp, GPC_InOutResStr)
        else RuntimeError (Temp)
    end
end;

procedure SignalHandler (Signal, Code : Integer);
var
  i, n : Integer;
  HandlerReset, Dummy : Boolean;
begin
  HandlerReset := InstallSignalHandler (Signal, SignalDefault, True, False, null, null);
  i := Low (SignalTable);
  while (i <= High (SignalTable)) and not ((SignalTable [i].Signal^ = Signal) and
         ((SignalTable [i].Code = nil) or (SignalTable [i].Code^ = Code))) do
    Inc (i);
  n := SignalTable [i].ErrorNumber;
  if n < 0 then
    begin
      RuntimeWarning (GetErrorMessage (-n));
      Dummy := InstallSignalHandler (Signal, TSignalHandler (@SignalHandler), True, True, null, null)
    end
  else
    begin
      WriteStr (ErrorMessageString, CString2String (GetErrorMessage (n)), ' (#', n, ')');
      { Return address not available or meaningful in a signal handler }
      SetReturnAddress (Pointer ($deadbeef));
      FinishErrorMessage (n);
var foo:CString;foo:=ErrorMessageString;
      Write (StdErr, ParamStr (0), ': ', (*@@@@Grmpf! How else can we stop GPC from wasting stack space?@@*){$local X+}foo){$endlocal};
      ErrorMessageString := '';
      if HandlerReset then
        Dummy := Kill (ProcessID, Signal)
    end
end;

type
  TSignalSet = set of Byte;

procedure InstallHandler (const Signals : TSignalSet);
var
  Signal : Integer;
  Dummy : Boolean;
begin
  for Signal in Signals do
    Dummy := InstallSignalHandler (Signal, TSignalHandler (@SignalHandler), True, True, null, null)
end;

(*$if 0*)
@@@@@@@@Doesn't work. Signal handlers have limited stack space (at
least on some systems), and it seems impossible to persuade GPC not
to waste huge amounts of stack... :-(
begin
  InstallHandler ((*@@*)(*$local W-*)[SigHUp, SigInt, { SigQuit, } SigIll, SigFPE, SigSegV,
    SigPipe, SigAlrm, SigTerm, SigTrap, SigIOT, SigEMT, SigBus, SigSys])(*$endlocal*)
(*$endif*)
end.
